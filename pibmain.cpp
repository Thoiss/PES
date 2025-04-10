#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>  // Voor inet_pton()
#include <netinet/in.h> // Voor adressen
#include <sys/socket.h> // Voor sockets
#include <unistd.h>     // Voor close()
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/select.h> // Voor fd_set en select()
#include <sys/time.h>   // Voor struct timeval

#define PORT 8888 // Wemos port
#define PI_A_PORT 8080 // Pi-A port
#define TIMEOUT 3 // Timeout in seconden
#define Devices 2

// IP-adressen van de Wemos-apparaten
const char *wemos_ips[] = {
    "192.168.137.24", // Wemos 1
    "192.168.137.71" // Wemos 2
};
const char *pi_a_ip = "192.168.137.31"; // Raspberry Pi A             
struct sockaddr_in serv_addr;
char buffer[1024] = {0};
int PiWaarde_Knop = 0;
int pi_a_socket;
int Waarde_Knop = 0;
int Pi_a_Led = 0;
char RGBSend[15] = " - RGBWAARDE";
int RGBWaarde = 0;
bool Versturen = false;
ssize_t valread;


int connect_to_wemos(const char *ip)
{ // Verbind met Wemos
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    { // socket creation for Wemos
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET; // Ipv4
    serv_addr.sin_port = htons(PORT); 

    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0)
    { // ip adres toewijzen
        printf("Ongeldig adres of niet ondersteund: %s\n", ip);
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    { // maak verbinding met apparaat (wemos)
        printf("Verbinding mislukt met: %s\n", ip);
        return -1;
    }

    printf("Verbonden met %d Wemos op %s\n", sock, ip);
    return sock;
}
int connect_to_pi(const char *ip, int port)
{ // Verbind met Pi-a
    int sock;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    { // Create socket for Pi
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0)
    {
        return -1;
    }

    // Zet socket op non-blocking mode
        int flags = fcntl(sock, F_GETFL, 0); // request huidige flags
        fcntl(sock, F_SETFL, flags | O_NONBLOCK); // voeg nonblock toe aan flags

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        fd_set fdset;
        struct timeval tv;
        FD_ZERO(&fdset);
        FD_SET(sock, &fdset);
        tv.tv_sec = TIMEOUT; // Timeout in seconden
        tv.tv_usec = 0;

        if (select(sock + 1, NULL, &fdset, NULL, &tv) > 0)
        {
            int so_error;
            socklen_t len = sizeof(so_error);
            getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);
            if (so_error == 0)
            {
                return sock;
            }
        }
        close(sock);
        return -1;
    }

    return sock;
}

int vraag_knop_status(int Socket)
{                                      // Stuur request naar wemos voor knopstatus
    const char *hello = "Hello from client\n";
    const char *Led = "Led\n";
    const char *wemosStatus = "wemosStatus\n";


    memset(buffer, 0, sizeof(buffer)); // buffer leegmaken
    const char *token = "";
    send(Socket, hello, strlen(hello), 0); // stuur hello naar Socket x
    // sleep(2);
    printf("%d: Hello message sent\n", Socket);

    if (Versturen) // check of iets uitgevoerd moet worden, true wordt gezet op de functie hierna (binnenkomende statusberichten) (ln. 157 o.a.)
    {
        if (Waarde_Knop == 1 )
        {
            send(Socket, Led, strlen(Led), 0);
            printf("Led message sent\n");
            Waarde_Knop = 0;
        }
        if (RGBWaarde != 0)
        {
            snprintf(RGBSend, sizeof(RGBSend), "%d - RGBWAARDE\n", RGBWaarde);
            send(Socket, RGBSend, strlen(RGBSend), 0);
            printf("RGB message sent\n");
            RGBWaarde = 0;
        }  
        if (Pi_a_Led == 1 ) // If bericht van Pi-A ontvangen
        {
            send(Socket, Led, strlen(Led), 0);
            printf("Pi-Led message sent\n");
            // Pi_a_Led = 0;
            // to do : ack 
        }
        if(Socket == 5){ // bij de laatste Socket, zet versturen op false
        Versturen = false;
        Pi_a_Led = 0;
        }
    }
    send(Socket, wemosStatus, strlen(wemosStatus), 0); // status request
    read(Socket, buffer, sizeof(buffer) - 1); // leest response
    printf("Buffer: %s\n", buffer); // print bericht
    token = strtok(buffer, " "); // leest/parst tot 1e spatie in bericht 'FALSE PiFALSE RGBFALSE' -> wijst toe op 'token'
    if (strcmp(token, "TRUE") == 0) // als token klopt...
    { // kijk of wat ontvangen is vanaf Arduino IDE code == 'TRUE'
        Waarde_Knop = 1;
        Versturen = true;
    }
    token = strtok(NULL, " "); // de rest van de string (onthouden door strtok) lezen tot spatie
    if (strcmp(token, "PiTRUE") == 0)
    {
        PiWaarde_Knop = 1;
    }
    token = strtok(NULL, " "); // de rest van de string (onthouden door strtok) lezen tot spatie
    printf("Huidig: %s\n", token);
    if (strcmp(token, "1") == 0 || strcmp(token, "2") == 0 || strcmp(token, "3") == 0)
    {
        printf("Huidig: %s\n", token);
        RGBWaarde = atoi(token); // Converteer de string naar een integer
        printf("RGBWaarde: %d\n", RGBWaarde);
        Versturen = true;
    }
    memset(buffer, 0, sizeof(buffer));
    return 0;
}

int Pi_Connectie()
{
    const char *PiLed = "LED";
    const char *PiStatus = "Status";

    if (PiWaarde_Knop == 1)
    {
        send(pi_a_socket, PiLed, strlen(PiLed), 0); // Stuur bericht dat knop is ingedrukt
        printf("Bericht LED verstuurt naar Pi-A \n");
        PiWaarde_Knop = 0;
    }
    memset(buffer, 0, sizeof(buffer));
    send(pi_a_socket, PiStatus, strlen(PiStatus), 0); // status request naar Pi-A
    printf("Bericht Status verstuurt naar Pi-A \n");
    valread = read(pi_a_socket, buffer, sizeof(buffer) - 1);
    buffer[valread] = '\0'; // Zorg ervoor dat de string correct wordt afgesloten
    printf("Ontvangen van Pi-A: %s\n", buffer);
    if (strcmp(buffer, "aan") == 0)
    {
        Pi_a_Led = 1;
        Versturen = true; // stuur naar wemos dat lampje aan moet
    }
    memset(buffer, 0, sizeof(buffer));
    return 0;
}
int main()
{
    int Socket [Devices] = {0};
    int i = 0;
    pi_a_socket = connect_to_pi(pi_a_ip, PI_A_PORT);
    while (1)
    {
        Pi_Connectie();
        while (i < Devices)
        {
            Socket[i] = connect_to_wemos(wemos_ips[i]); // connect naar wemos op basis van ip
            printf("socket = %d\n", Socket[i]);
            i++;
        }
        for (int i = 0; i < Devices; i++)
        { // uitlezen
            printf("Vraag Socket = %d\n", Socket[i]);
            vraag_knop_status(Socket[i]);
        }
    }

    return 0;
}

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>  // Voor inet_pton()
#include <netinet/in.h> // Voor adressen
#include <sys/socket.h> // Voor sockets
#include <unistd.h>     // Voor close()
#include <stdlib.h>
#include <stdbool.h>

#define PORT 8888
#define PI_A_PORT 8080
#define TIMEOUT 3 // Timeout in seconden
#define Devices 2

// IP-adressen van de Wemos-apparaten
const char *wemos_ips[] = {
    "192.168.137.225", // Wemos 1
    "192.168.137.211"  // Wemos 2
};
const char *pi_a_ip = "192.168.137.40"; // Raspberry Pi A
int client_fd;                          // File descriptor voor socket (nummertje voor Wemos in print)
struct sockaddr_in serv_addr;
char buffer[1024] = {0};
char RGBSend[15] = " - RGBWAARDE";
char *hello = "Hello from client\n";
char *sluiten = "End\n";
char *Led = "Led\n";
char *Knop = "Knop\n";
char *PiTest = "PiTest\n";
char *RGB = "RGB\n";
char *PiLed = "LED\n";
int Waarde_Knop = 0;
int PiWaarde_Knop = 0;
int RGBWaarde = 0;
int Socket = 0;
bool Versturen = false;
ssize_t valread;

int connect_to_wemos(const char *ip)
{ // Verbind met Wemos
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    { // socket creation
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
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
int connect_to_device(const char *ip, int port)
{ // Verbind met Pi-a
    int sock;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0)
    {
        printf("Ongeldig adres of niet ondersteund: %s\n", ip);
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Verbinding mislukt met: %s\n", ip);
        close(sock);
        return -1;
    }

    printf("Verbonden met %s op poort %d\n", ip, port);
    return sock;
}
int vraag_knop_status(int Socket)
{                                      // Stuur request naar wemos voor knopstatus
    memset(buffer, 0, sizeof(buffer)); // buffer leegmaken

    send(client_fd, hello, strlen(hello), 0); // stuur hello naar client_fd
    printf("%d: Hello message sent\n", Socket);
    // usleep(500000);
    // memset(buffer, 0, sizeof(buffer));
    // read(client_fd, buffer, sizeof(buffer) - 1);
    // printf("%s\n", buffer);
    // memset(buffer, 0, sizeof(buffer));

    if (Versturen)
    {
        if (Waarde_Knop == 1)
        {
            send(client_fd, Led, strlen(Led), 0);
            printf("Led message sent\n");
            // usleep(500000);
            // read(client_fd, buffer, sizeof(buffer) - 1);
            // printf("%s\n", buffer);
            // memset(buffer, 0, sizeof(buffer));
            Waarde_Knop = 0;
        }
        if (RGBWaarde != 0)
        {
            snprintf(RGBSend, sizeof(RGBSend), "%d - RGBWAARDE\n", RGBWaarde);
            send(client_fd, RGBSend, strlen(RGBSend), 0);
            printf("RGB message sent\n");
            // usleep(500000);
            // read(client_fd, buffer, sizeof(buffer) - 1);
            // printf("%s\n", buffer);
            // memset(buffer, 0, sizeof(buffer));
            RGBWaarde = 0;
        }
        Versturen = false;
    }

    memset(buffer, 0, sizeof(buffer)); // reset buffer
    send(client_fd, Knop, strlen(Knop), 0);
    // Toevoegen-aanpassen 5,D1  (Destination socket, Destination Pin)
    read(client_fd, buffer, sizeof(buffer) - 1); // lees wat er binnenkomt
    printf("%s\n", buffer);
    // usleep(500000);
    if (strcmp(buffer, "TRUE") == 0)
    { // kijk of wat ontvangen is vanaf Arduino IDE code == 'TRUE'
        Waarde_Knop = 1;
        Versturen = true;
    }
    memset(buffer, 0, sizeof(buffer));
    send(client_fd, PiTest, strlen(PiTest), 0);

    read(client_fd, buffer, sizeof(buffer) - 1); // zelfde maar voor PiTRUE
    printf("%s\n", buffer);
    if (strcmp(buffer, "PiTRUE") == 0)
    {
        PiWaarde_Knop = 1;
    }
    // usleep(500000);
    memset(buffer, 0, sizeof(buffer));

    send(client_fd, RGB, strlen(RGB), 0);

    read(client_fd, buffer, sizeof(buffer) - 1); // zelfde maar voor PiTRUE
    printf("%s\n", buffer);

    if (strcmp(buffer, "RGBFALSE") != 0)
    {
        RGBWaarde = atoi(buffer); // Converteer de string naar een integer
        Versturen = true;
    }
    // usleep(500000);
    memset(buffer, 0, sizeof(buffer));

    // while (strcmp(buffer, "Verbinding wordt afgesloten door server") != 0)
    // {
    //     send(client_fd, sluiten, strlen(sluiten), 0);
    //     read(client_fd, buffer, sizeof(buffer) - 1);
    //     printf("End message sent\n");
    //     sleep(2);
    // }
}

int main()
{
    int pi_a_socket = connect_to_device(pi_a_ip, PI_A_PORT);
    if (pi_a_socket >= 0)
    {
        printf("Succesvol verbonden met Raspberry Pi A!\n");
    }
    while (1)
    {
        if (Socket < Devices)
        {
            printf("%d\n", Socket);
            client_fd = connect_to_wemos(wemos_ips[Socket]); // Wijst socket toe aan client_fd
            // zet_led_aan();
            sleep(2); // Wacht even voordat de volgende check start
            Socket++;
        }
        for (int i = 0; i < Devices; i++)
        { // uitlezen
            // sleep(1);
            client_fd = i + 4; // 3 zonder Pi-A 4 Met Pi-A
            vraag_knop_status(i);
        }
        if (PiWaarde_Knop == 1)
        {
            send(pi_a_socket, PiLed, strlen(PiLed), 0); // Stuur bericht dat knop is ingedrukt
            printf("Bericht LED verstuurt naar Pi-A \n");
            PiWaarde_Knop = 0;
        }
        // // Ontvang bericht van Pi-A
        // if (pi_a_socket)
        // {
        //     valread = read(pi_a_socket, buffer, sizeof(buffer) - 1);
        //     buffer[valread] = '\0';
        //     printf(buffer);
        //     memset(buffer, 0, sizeof(buffer));
        // }
    }
    return 0;
}

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>  // Voor inet_pton()
#include <netinet/in.h> // Voor adressen
#include <sys/socket.h> // Voor sockets
#include <unistd.h>     // Voor close()
#include <stdlib.h>
#include <stdbool.h>

#define PORT 8888
#define TIMEOUT 3  // Timeout in seconden
#define Devices 2

// IP-adressen van de Wemos-apparaten
const char *wemos_ips[] = {
    "192.168.137.141",  // Wemos 1
    "192.168.137.206"   // Wemos 2
};
int client_fd;  // File descriptor voor socket
struct sockaddr_in serv_addr;
char buffer[1024] = {0};
char *hello = "Hello from client\n";
char *sluiten = "End\n";
char *Led = "Led\n";
char *Knop = "Knop\n";
int Waarde_Knop = 0;
int Socket = 0;
bool Versturen = false;

int connect_to_wemos(const char *ip) {
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        printf("Ongeldig adres of niet ondersteund: %s\n", ip);
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Verbinding mislukt met: %s\n", ip);
        return -1;
    }

    printf("Verbonden met %d Wemos op %s\n", sock,ip);
    return sock;
}

int vraag_knop_status(int Socket) {
    memset(buffer, 0, sizeof(buffer));

    send(client_fd, hello, strlen(hello), 0);
    printf("%d: Hello message sent\n",Socket);
    usleep(500000);
    read(client_fd, buffer, sizeof(buffer) - 1);
    printf("%s\n", buffer);
    memset(buffer, 0, sizeof(buffer));

    if(Versturen){
        if(Waarde_Knop == 1){
            send(client_fd, Led, strlen(Led), 0);
            printf("Led message sent\n");
            usleep(500000);
            read(client_fd, buffer, sizeof(buffer) - 1);
            printf("%s\n", buffer);
            memset(buffer, 0, sizeof(buffer));
            Waarde_Knop=0;    
        }
    }

    memset(buffer, 0, sizeof(buffer));
    send(client_fd, Knop, strlen(Knop), 0);
    read(client_fd, buffer, sizeof(buffer) - 1);
    printf("%s\n", buffer);
    usleep(500000);
    memset(buffer, 0, sizeof(buffer));
//Toevoegen-aanpassen 5,D1  (Destination socket, Destination Pin)
    read(client_fd, buffer, sizeof(buffer) - 1);
    printf("%s\n", buffer);
    usleep(500000);
    if (strcmp(buffer, "TRUE") == 0) {
        Waarde_Knop = 1;
        Versturen = true;
    }
    memset(buffer, 0, sizeof(buffer));
    // while (strcmp(buffer, "Verbinding wordt afgesloten door server") != 0)
    // {
    //     send(client_fd, sluiten, strlen(sluiten), 0);
    //     read(client_fd, buffer, sizeof(buffer) - 1);
    //     printf("End message sent\n");
    //     sleep(2);
    // }
}
int main() {
    while (1) {
        if(Socket < Devices){
        printf("%d\n", Socket);
        client_fd = connect_to_wemos(wemos_ips[Socket]);
        // zet_led_aan();
        sleep(2);  // Wacht even voordat de volgende check start
        Socket++;
    }
    for(int i = 0; i < Devices; i++){
    // sleep(1);
    client_fd = i+3;
    vraag_knop_status(i);
    }
}
    return 0;
}

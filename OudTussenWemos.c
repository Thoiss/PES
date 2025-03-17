// Pi-B
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>  // for ip inet_pton()
#include <netinet/in.h> // for address
#include <sys/select.h> // for io multiplexing (select)
#include <sys/socket.h> // for socket
#include <unistd.h>     // for close()
#define PORT 8888
int wemosPro(int argc, char const *argv[]); // Lees waarde van knop uit
int wemosCopy(int argc, char const *argv[]); // Verstuur bericht dat Led aan moet
int socketProConnect(int argc, char const *argv[]); // Create socket pro + Connect
int socketCopy(int argc, char const *argv[]); // Connect naar andere Wemos (IP)
int status, valread, client_fd;
struct sockaddr_in serv_addr;
char *hello = "Hello from client \n";
char *sluiten = "End\n";
char *Led = "Led\n";
char buffer[1024] = {0};
char*Knop = "Knop\n";
int Waarde_Knop = 0;

int main(int argc, char const *argv[])
{
socketProConnect(argc,argv); // Verbind met wemos pro (1)
socketCopy(argc,argv);    
}
// Create socket pro + Connect
int socketProConnect(int argc, char const *argv[]){
if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "192.168.137.141", &serv_addr.sin_addr) <= 0)
    {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((status = connect(client_fd, (struct sockaddr *)&serv_addr,
                          sizeof(serv_addr))) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    wemosPro(argc,argv);
}
// Lees waarde van knop uit
int wemosPro(int argc, char const *argv[])
{
    memset(buffer, 0, sizeof(buffer));
    send(client_fd, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    valread = read(client_fd, buffer, 1024 - 1);
    printf("%s\n", buffer);
    memset(buffer, 0, sizeof(buffer));
    sleep(1);
    send(client_fd, Knop, strlen(Knop), 0);
    valread = read(client_fd, buffer, 1024 - 1);
    printf("%s\n", buffer);
    memset(buffer, 0, sizeof(buffer));

    valread = read(client_fd, buffer, 1024 - 1);
    printf("%s\n", buffer);
    if (strcmp(buffer, "TRUE") == 0) {
        printf("Connectie naar andere\n");
        Waarde_Knop = 1;
    }
    sleep(1);
    memset(buffer, 0, sizeof(buffer));
    while (strcmp(buffer, "Verbinding wordt afgesloten door server") != 0)
    {
        send(client_fd, sluiten, strlen(sluiten), 0);
        valread = read(client_fd, buffer, 1024 - 1);
        printf("End message sent\n");
        sleep(2);
    }
    
    printf("%s\n", buffer);
    if(Waarde_Knop == 1){
    printf("Functie socketCopy\n");
     socketCopy(argc,argv);   
    }
}
// Create en Connect naar andere Wemos (IP)
int socketCopy(int argc, char const *argv[]){ 
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "192.168.137.206", &serv_addr.sin_addr) <= 0)
    {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((status = connect(client_fd, (struct sockaddr *)&serv_addr,
                          sizeof(serv_addr))) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
 wemosCopy(argc,argv);
}
// Verstuur bericht dat Led aan moet
int wemosCopy(int argc, char const *argv[])
{
    memset(buffer, 0, sizeof(buffer));
    send(client_fd, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    valread = read(client_fd, buffer, 1024 - 1);
    printf("%s\n", buffer);
    memset(buffer, 0, sizeof(buffer));
    
    sleep(1);
    if(Waarde_Knop == 1){
        send(client_fd, Led, strlen(Led), 0);
        printf("Led message sent\n");
        valread = read(client_fd, buffer, 1024 - 1);
        printf("%s\n", buffer);
        memset(buffer, 0, sizeof(buffer));
        Waarde_Knop=0;    
    }
    while (strcmp(buffer, "Verbinding wordt afgesloten door server") != 0)
    {
        send(client_fd, sluiten, strlen(sluiten), 0);
        valread = read(client_fd, buffer, 1024 - 1);
        printf("End message sent\n");
        sleep(2);
    }
    printf("%s\n", buffer);
    memset(buffer, 0, sizeof(buffer));   
    }

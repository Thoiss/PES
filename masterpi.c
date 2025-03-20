// Server side C program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <asm-generic/socket.h>
#include <wiringPi.h> 
#include <wiringPiI2C.h>

#define PORT 8080

#define SLAVE_ADDRESS 0x11  // Het I2C-adres van de STM32 slave
#define SLAVE_ADDRESS2 0x13


int main(int argc, char const *argv[])
{

    int fd;
    int fb;
    int button_status;
    int server_fd, new_socket;
        ssize_t valread;
        struct sockaddr_in address;
        int opt = 1;
        socklen_t addrlen = sizeof(address);
        char buffer[1024] = {0};
        char *hello = "Hello from server";

        if (wiringPiSetup() == -1) {
            printf("Fout bij het instellen van wiringPi\n");
            return 1;
        }
    
        // Open de I2C bus en connecteer met de slave
        fd = wiringPiI2CSetup(SLAVE_ADDRESS);
        if (fd == -1) {
            printf("Fout bij het openen van slave 1\n");
            return 1;
        }
        fb = wiringPiI2CSetup(SLAVE_ADDRESS2);
        if (fb == -1) {
            printf("Fout bij het openen van slave 2\n");
            return 1;
        }

        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        // Forcefully attaching socket to the port 8080
        if (setsockopt(server_fd, SOL_SOCKET,
                       SO_REUSEADDR | SO_REUSEPORT, &opt,
                       sizeof(opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        // Forcefully attaching socket to the port 8080
        if (bind(server_fd, (struct sockaddr *)&address,
                 sizeof(address)) < 0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 &addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
    while (1)
    {
        
        valread = read(new_socket, buffer, sizeof(buffer) - 1);
buffer[valread] = '\0'; // Zorgt ervoor dat de string correct wordt afgesloten
if (buffer != 0){
printf("Ontvangen bericht: %s\n", buffer);
}
if(strcmp(buffer, "LED")) {
    wiringPiI2CWrite(fb, '1');
    delay(1000);
    wiringPiI2CWrite(fb, '0');
}

        // send(new_socket, "yeet", strlen(hello), 0);
        // printf("%s\n", buffer);
        // send(new_socket, hello, strlen(hello), 0);
        // printf("Hello message sent\n");

        // closing the connected socket
        // close(new_socket);
        // closing the listening socket
        // close(server_fd);
    }
    return 0;
}

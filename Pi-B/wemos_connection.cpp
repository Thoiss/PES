#include "globals.h"
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int connect_to_wemos(const char *ip)
{ // Verbind met Wemos
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    { // socket creation for Wemos
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET; // Ipv4
    serv_addr.sin_port   = htons(PORT);

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

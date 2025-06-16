#include "wemos_connection.hpp"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netinet/tcp.h>

int WemosConnection::connectToWemos(const char* ip) {
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    struct timeval to = { .tv_sec = 0, .tv_usec = 50000  };
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &to, sizeof(to));
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &to, sizeof(to));


    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons(PORT);

    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
        printf("Ongeldig adres of niet ondersteund: %s\n", ip);
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Verbinding mislukt met: %s\n", ip);
        close(sock);
        return -1;
    }

    fd_set wfds;
    FD_ZERO(&wfds);
    FD_SET(sock, &wfds);
    struct timeval tv = { .tv_sec = 0, .tv_usec = 500000 };
    select(sock + 1, NULL, &wfds, NULL, &tv);

    printf("Verbonden met Wemos op %s\n", ip);
    return sock;
}
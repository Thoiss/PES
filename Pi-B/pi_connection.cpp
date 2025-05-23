#include "pi_connection.hpp"
#include "globals.h"
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <cstdio>
#include <cstring>

int PiConnection::connectToPi(const char *ip, int port)
{
    int sock;
    struct sockaddr_in local_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        return -1;
    }

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &local_addr.sin_addr) <= 0)
    {
        return -1;
    }

    // Zet socket op non-blocking mode
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    if (connect(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0)
    {
        fd_set fdset;
        struct timeval tv;
        FD_ZERO(&fdset);
        FD_SET(sock, &fdset);
        tv.tv_sec = TIMEOUT;
        tv.tv_usec = 0;

        if (select(sock + 1, NULL, &fdset, NULL, &tv) > 0)
        {
            int so_error;
            socklen_t len = sizeof(so_error);
            getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);
            if (so_error == 0)
            {
                pi_a_socket = sock;
                return sock;
            }
        }
        close(sock);
        return -1;
    }
    pi_a_socket = sock;
    return sock;
}

int PiConnection::handlePiConnection()
{
    if (pi_a_socket <= 0)
    {
        return -1;
    }

    const char *PiLed = "LED";
    const char *PiStatus = "Status";
    int retry = 0;
    if (PiWaarde_Knop == 1)
    {
        send(pi_a_socket, PiLed, strlen(PiLed), 0);
        printf("Bericht LED verstuurt naar Pi-A \n");
        PiWaarde_Knop = 0;
    }
    memset(buffer, 0, sizeof(buffer));
    send(pi_a_socket, PiStatus, strlen(PiStatus), 0);

    int valread = read(pi_a_socket, buffer, sizeof(buffer) - 1);
    buffer[valread] = '\0';
    char *token = strtok(buffer, " "); // Parse until the first space
    if (token == nullptr)
    {
        printf("Niks binnen\n");
        return -1; // Handle invalid token (e.g., empty response)
    }
    if (strcmp(token, "TRUE") == 0)
    { // If token is 'TRUE'
    }
    else if (strcmp(token, "FALSE") != 0)
    {
        return 0;
    }

    if (strcmp(buffer, "aan") == 0)
    {
        Pi_a_Led = 1;
        Versturen = true;
    }
    memset(buffer, 0, sizeof(buffer));
    return 0;
}
int PiConnection::verlichtingwaarde()
{int retry = 0;
    if (pi_a_socket <= 0)
    {
        return -1;
    }

    const char *PiVerlichting = "STMEncoder";
    memset(buffer, 0, sizeof(buffer));
    send(pi_a_socket, PiVerlichting, strlen(PiVerlichting), 0);
    int valread = read(pi_a_socket, buffer, sizeof(buffer) - 1);
    buffer[valread] = '\0';
    while  (retry <= 3)
    {
        int valread = read(pi_a_socket, buffer, sizeof(buffer) - 1);
        buffer[valread] = '\0';
        char *token = strtok(buffer, "-"); // Parse until the first -
        retry++;
        if (token == nullptr)
        {
            printf("Niks binnen\n");
            return -1; 
        }
    }
    // verlictingswaarde wordt alleen verstuurd wanneer deze veranderd is
    verlichtingsWaarde = atoi(token);
    if (verlichtingsWaarde != oudVerlichtingswaarde && !statusVerlichting)
    {
        oudVerlichtingswaarde = verlichtingsWaarde;
        // Versturen = true;
        printf("Verlichtingswaarde: %d\n", verlichtingsWaarde);
    }
    // statusVerlichting wordt een toggle op 1
    token = strtok(nullptr, "-");
    int verlichtingStatus = atoi(token);
    if (verlichtingStatus == 1)
    {
        // Versturen = true;
        statusVerlichting = !statusVerlichting;
        printf("statusVerlichting: %d\n", statusVerlichting);
        
    }
    // statusServo wordt een toggle op 1
    token = strtok(nullptr, "-");
    statusServo = atoi(token);
    if (statusServo == 1)
    {
        // Versturen = true;
        printf("statusServo: %d\n", statusServo);
    }
    memset(buffer, 0, sizeof(buffer));
    return 0;
}
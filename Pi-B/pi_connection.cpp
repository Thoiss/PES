#include "globals.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int connect_to_pi(const char *ip, int port)
{ // Verbind met Pi-A
    int sock;
    struct sockaddr_in local_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    { // Create socket for Pi
        return -1;
    }

    local_addr.sin_family = AF_INET;
    local_addr.sin_port   = htons(port);

    if (inet_pton(AF_INET, ip, &local_addr.sin_addr) <= 0)
    {
        return -1;
    }

    // Zet socket op non-blocking mode
    int flags = fcntl(sock, F_GETFL, 0);        // request huidige flags
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);   // voeg nonblock toe aan flags

    if (connect(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0)
    {
        fd_set fdset;
        struct timeval tv;
        FD_ZERO(&fdset);
        FD_SET(sock, &fdset);
        tv.tv_sec  = TIMEOUT; // Timeout in seconden
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

int Pi_Connectie()
{
    const char *PiLed    = "LED";
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
    buffer[valread] = '\0';  
    printf("Ontvangen van Pi-A: %s\n", buffer);

    if (strcmp(buffer, "aan") == 0)
    {
        Pi_a_Led   = 1;
        Versturen = true; // stuur naar wemos dat lampje aan moet
    }
    memset(buffer, 0, sizeof(buffer));
    return 0;
}

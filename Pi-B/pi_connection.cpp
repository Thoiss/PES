#include "pi_connection.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <cstdio>
#include <cstring>
#include "wemos_aansturen.hpp"
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

int PiConnection::encoderVerlichting(int socket[])
{
    WemosAansturen aanstuurder;
    int retry = 0;
    if (pi_a_socket <= 0)
    {
        return -1;
    }

    const char *PiVerlichting = "STMEncoder";
    memset(buffer, 0, sizeof(buffer));
    send(pi_a_socket, PiVerlichting, strlen(PiVerlichting), 0);
    int valread = read(pi_a_socket, buffer, sizeof(buffer) - 1);
    buffer[valread] = '\0';
    printf("Ontvangen PiA:%s\n", buffer);

    while (retry <= 15 && valread <= 0)
    {
        usleep(250000); // 250 ms
        valread = read(pi_a_socket, buffer, sizeof(buffer) - 1);
        buffer[valread] = '\0';
        printf("Ontvangen PiA:%s\n", buffer);
        retry++;
    }

    if (valread <= 0 || strcmp(buffer, "niet correct ontvangen") == 0)
    {
        printf("Niks binnen\n");
        return -1;
    }

    char *token = strtok(buffer, "-");
    if (token == nullptr)
    {
        printf("Kan eerste veld niet lezen uit buffer: %s\n", buffer);
        return -1;
    }

    int verlichtingStatus = atoi(token);

    token = strtok(nullptr, "-");
    if (token == nullptr)
    {
        printf("Kan tweede veld niet lezen uit buffer: %s\n", buffer);
        return -1;
    }

    verlichtingsWaarde = atoi(token);

    if (verlichtingStatus == 1)
    {
        Versturen = true;
        statusVerlichting = !statusVerlichting;
        printf("statusVerlichting: %d\n", statusVerlichting);
    }

    if (verlichtingsWaarde != oudVerlichtingswaarde)
    {
        Versturen = true;
        verlichtingsWaarde = verlichtingsWaarde * 2;
        oudVerlichtingswaarde = verlichtingsWaarde;
        printf("Verlichtingswaarde: %d\n", verlichtingsWaarde);
    }

    if (Versturen)
    {
        aanstuurder.stuurWemosAan(socket[1], 1, statusVerlichting, verlichtingsWaarde, Versturen);
    }

    memset(buffer, 0, sizeof(buffer));
    return 0;
}

int PiConnection::routeVerlichting(int socket[])
{
    WemosAansturen routeAanstuurder;
    int retry = 0;
    if (pi_a_socket <= 0)
    {
        return -1;
    }

    const char *PiVerlichting = "routeVerlichting";
    memset(buffer, 0, sizeof(buffer));
    send(pi_a_socket, PiVerlichting, strlen(PiVerlichting), 0);
    int valread = read(pi_a_socket, buffer, sizeof(buffer) - 1);
    buffer[valread] = '\0';
    printf("Ontvangen PiA:%s\n", buffer);
    while (retry <= 15 && valread <= 0)
    {
        usleep(250000); // 250 ms
        valread = read(pi_a_socket, buffer, sizeof(buffer) - 1);
        buffer[valread] = '\0';
        printf("Ontvangen PiA:%s\n", buffer);
        retry++;
    }
    if (valread <= 0 || buffer == "niet correct ontvangen")
    {
        printf("Niks binnen\n");
        return -1;
    }
    // statusRouteVerlichting wordt een toggle op 1
    int statusRouteVerlichting = atoi(token);
    if (statusRouteVerlichting == 1)
    {
        Versturen = true;
        statusRouteVerlichting = !statusRouteVerlichting;
        printf("routeVerlichting: %d\n", statusRouteVerlichting);
        
    }
    if (Versturen){
        routeAanstuurder.routeWemos(socket[1], 1,statusRouteVerlichting, Versturen ); // Aansturen van Wemos 1
    }
    memset(buffer, 0, sizeof(buffer));
    return 0;
}
int PiConnection::ontvangTemperatuurData()
{
    if (pi_a_socket <= 0) {
        printf("Socket niet verbonden.\n");
        return -1;
    }

    const char *tempRequest = "getTemp";
    memset(buffer, 0, sizeof(buffer));

    // Stuur commando naar server
    if (send(pi_a_socket, tempRequest, strlen(tempRequest), 0) < 0) {
        perror("Send mislukt");
        return -1;
    }

    // Wacht op antwoord
    int valread = read(pi_a_socket, buffer, sizeof(buffer) - 1);
    if (valread <= 0) {
        perror("Fout bij lezen van server");
        return -1;
    }

    buffer[valread] = '\0';
    printf("Temperatuurgegevens ontvangen van server: %s\n", buffer);

    return 0;
}

#include "pi_run.hpp"
#include <cstdio>
#include <cstring>
#include <unistd.h>   // for close(), sleep()
#include <ctime>      // for time(), difftime()

int piRun::Loop() {
    WemosAansturen aanstuurder;

    char message[256] = {0};                // Bericht om te versturen
    char lastMessage[Devices][256] = {{0}}; // Vorig bericht per Wemos
    WemosConnection wemosConns[Devices];    // Wemos verbindingen
    PiConnection piConn;

    char tempBuffer[128] = {0};             // Buffer voor temperatuurdata
    time_t lastTempUpdate = 0;              // Tijdstip van laatste temp update

    printf("Verbind met Pi-A...\n");

    while (1) {
        // Connectie met Pi-A maken indien niet verbonden
        if (piSocket <= 0) {
            piSocket = piConn.connectToPi(pi_a_ip, PI_A_PORT);
            printf("Resultaat verbinding Pi-A: %d\n", piSocket);
        } else if (piSocket > 0) {
            piConn.encoderVerlichting(Socket);
            piConn.routeVerlichting(Socket);
        }

        // Elke 18 seconden temperatuurdata ophalen
        time_t now = time(NULL);
        if (difftime(now, lastTempUpdate) >= 18) {
            if (piConn.ontvangTemperatuurData(tempBuffer, sizeof(tempBuffer)) != 0) {
                strcpy(tempBuffer, "Geen temperatuurdata");
            }
            printf("Ontvangen temperatuurdata: %s\n", tempBuffer);
            lastTempUpdate = now;
        }

        // Loop over Wemos apparaten
        for (int i = 0; i < Devices; ++i) {
            // Verbinding maken indien nodig
            if (Socket[i] <= 0) {
                int sock = wemosConns[i].connectToWemos(wemos_ips[i]);
                if (sock > 0) {
                    Socket[i] = sock;
                    printf("Wemos[%d] verbonden, socket = %d\n", i, sock);
                } else {
                    Socket[i] = 0;
                    continue;
                }
            }

            if (Socket[i] > 0) {
                // Bericht samenstellen, specifiek voor lichtkrant (Wemos[2])
                if (i == 2) {
                    snprintf(message, sizeof(message), "text:Temp binnen/buiten: %s\n", tempBuffer);
                } else {
                    snprintf(message, sizeof(message), "text:Status update\n");
                }

                // Versturen alleen als bericht gewijzigd
                if (strncmp(message, lastMessage[i], sizeof(lastMessage[i])) != 0) {
                    int sent = send(Socket[i], message, strlen(message), 0);
                    if (sent > 0) {
                        printf("Bericht naar Wemos[%d] gestuurd: %s\n", i, message);
                        strncpy(lastMessage[i], message, sizeof(lastMessage[i]) - 1);
                        lastMessage[i][sizeof(lastMessage[i]) - 1] = '\0';
                    } else {
                        printf("Versturen naar Wemos[%d] mislukt, socket sluiten.\n", i);
                        close(Socket[i]);
                        Socket[i] = 0;
                    }
                }
            }
        }

        // Eventueel status logging per socket
        for (int j = 0; j < Devices; ++j) {
            if (Socket[j] > 0) {
                printf("Huidige Socket[%d] = %d\n", j, Socket[j]);
                // Hier eventueel extra aansturing
                // aanstuurder.stuurWemosAan(Socket[j], j);
            } else {
                printf("Socket[%d] invalid\n", j);
            }
        }

        // Voorkom 100% CPU load
        //sleep(1); // 1 seconde pauze, kan je aanpassen
    }

    return 0;
}

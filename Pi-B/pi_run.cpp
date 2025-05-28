#include "pi_run.hpp"
int piRun::Loop(){


    WemosAansturen aanstuurder;

    char message[256] = {0};  // Buffer to hold message from Pi-A
    char lastMessage[Devices][256] = {{0}};  // One per Wemos
    // Create persistent WemosConnection objects
    WemosConnection wemosConns[Devices];
    // Connect to Pi-A
    PiConnection piConn;
    printf("Verbind met Pi-A...\n");

    while (1)
    {
        if (piSocket <= 0) {
            piSocket = piConn.connectToPi(pi_a_ip, PI_A_PORT);
            printf("Resultaat verbinding: %d\n", piSocket);
        }

        else if (piSocket > 0 ) {
            piConn.encoderVerlichting(Socket);
            piConn.routeVerlichting(Socket);
        }

        //sleep(1); // tijdelijk toegevoegd om overload aan status berichten te voorkomen
        int i = 0;
        // Connect to Wemos devices if not already connected
        for (int i = 0; i < Devices; ++i)
        {
            if (Socket[i] <= 0) {
                int sock = wemosConns[i].connectToWemos(wemos_ips[i]);
                if (sock > 0) {
                    Socket[i] = sock;
                    printf("Wemos[%d], socket = %d\n", i, sock);
                } else {
                    //printf("Failed to connect to Wemos[%d]\n", i);
                    Socket[i] = 0;
                    continue;
                }
            }

            // Send message only if changed
            if (strncmp(message, lastMessage[i], sizeof(lastMessage[i])) != 0) {
                int sent = send(Socket[i], message, strlen(message), 0);
                if (sent > 0) {
                    printf("Display message sent to Wemos[%d]: %s\n", i, message);
                    strncpy(lastMessage[i], message, sizeof(lastMessage[i]) - 1);
                    lastMessage[i][sizeof(lastMessage[i]) - 1] = '\0';
                } else {
                    printf("Failed to send to Wemos[%d], closing socket.\n", i);
                    close(Socket[i]);
                    Socket[i] = 0;
                }
            }
        }

       
        for (int j = 0; j < Devices; ++j)
        {
            if (Socket[j] > 0) {
                printf("Huidige Socket = %d\n", Socket[j]);
                // StatusHandler handler(Socket[j], aanstuurder);
               // aanstuurder.stuurWemosAan(Socket[j], j);
            } else {
                printf("Socket[%d] invalid\n", j);
            }
        }

        // Avoid 100% CPU usage
 //       std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
//g++ -std=c++17 Test_Lichkrant_main.cpp $(ls *.cpp | grep -v main.cpp) -o Licht_krant_test
#include "globals.h"
#include "wemos_connection.hpp"
#include "pi_connection.hpp"
#include "status_handler.hpp"
#include "wemos_aansturen.hpp"

int main()
{
    int Socket[Devices] = {0};
    WemosAansturen aanstuurder;

    const char* message = "text:PES groep E\n";
    char lastMessage[Devices][256] = {{0}};  // One per Wemos

    // Create persistent WemosConnection objects
    WemosConnection wemosConns[Devices];

    // Connect to Pi-A
    PiConnection piConn;
    printf("Verbind met Pi-A...\n");

    while (1)
    {
        int status = piConn.connectToPi(pi_a_ip, PI_A_PORT);
        printf("Resultaat verbinding: %d\n", status);
        int tellerPerformance = 0;

        int i = 0;
        // Haal status van Pi-A op en stuur LED-bericht indien nodig
        piConn.handlePiConnection();
        // Haal verlichtingswaarde + status Servo op
        piConn.verlichtingwaarde();
        sleep(2); // tijdelijk toegevoegd om overload aan status berichten te voorkomen

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

        // Request button status from valid sockets
        for (int j = 0; j < Devices; ++j)
        {
            tellerPerformance++;
            if (Socket[j] > 0) {
                printf("%d Huidige Socket = %d\n", tellerPerformance, Socket[j]);
                StatusHandler handler(Socket[j], aanstuurder);
                handler.vraagKnopStatus();
            } else {
                printf("Socket[%d] invalid\n", j);
            }
        }

        // Avoid 100% CPU usage
 //       std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

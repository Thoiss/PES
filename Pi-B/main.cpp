// Compileren (in de juiste folder):
//   g++ *.cpp -o piBmain

#include "globals.h"
#include "wemos_connection.hpp"
#include "pi_connection.hpp"
#include "status_handler.hpp"
#include "wemos_aansturen.hpp"


int main()
{
    
    int Socket[Devices] = {0};
    
    WemosAansturen aanstuurder;

    // Eerst Pi-A verbinden
    PiConnection piConn;
    printf("Verbind met Pi-A...\n");
    while (1)
    {
    int status = piConn.connectToPi(pi_a_ip, PI_A_PORT);
    printf("Resultaat verbinding: %d\n", status);
    int tellerPerformance =0;


        int i = 0;
        // Haal status van Pi-A op en stuur LED-bericht indien nodig
        piConn.handlePiConnection();
        sleep(2); // tijdelijk toegevoegd om overload aan status berichten te voorkomen
        
        // Verbind met alle Wemos-apparaten
        while (i < Devices)
        {
            WemosConnection wemosConn;
            Socket[i] = wemosConn.connectToWemos(wemos_ips[i]);
            printf("socket = %d\n", Socket[i]);
            i++;
        }

        // Voor elke Wemos: vraag knopstatus met StatusHandler
        for (int j = 0; j < Devices; j++)
        {
            tellerPerformance++;
            printf("%d Huidige Socket = %d\n", tellerPerformance, Socket[j]);
            StatusHandler handler(Socket[j], aanstuurder);
            handler.vraagKnopStatus();
            
        }
    }

    return 0;
}

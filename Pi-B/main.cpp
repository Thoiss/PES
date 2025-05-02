//Compileren (wel in de goede folder zitten in de terminal)
// g++ *.cpp -o piBmain

#include "globals.h"
#include "wemos_connection.h"
#include "pi_connection.h"
#include "status_handler.h"

int main()
{
    int Socket[Devices] = {0};
    int i = 0;

    // Eerst Pi-A verbinden
    pi_a_socket = connect_to_pi(pi_a_ip, PI_A_PORT);

    while (1)
    {
        // Haal status van Pi-A op en stuur LED-bericht indien nodig
        Pi_Connectie();

        // Verbind met alle Wemos-apparaten
        while (i < Devices)
        {
            Socket[i] = connect_to_wemos(wemos_ips[i]);
            printf("socket = %d\n", Socket[i]);
            i++;
        }

        // Voor elke Wemos: vraag knopstatus en verwerk response
        for (int j = 0; j < Devices; j++)
        {
            printf("Vraag Socket = %d\n", Socket[j]);
            vraag_knop_status(Socket[j]);
        }
    }

    return 0;
}

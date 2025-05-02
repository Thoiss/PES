#include "wemos_aansturen.hpp"

WemosAansturen::WemosAansturen()
    : tellerdevices(0)
{
    memset(versturenbuffer, 0, sizeof(versturenbuffer));
}

int WemosAansturen::stuurWemosAan(char* deviceNaam, int socket)
{
    const char* Led = "Led\n";

    if (Versturen) // check of iets uitgevoerd moet worden
    {
        tellerdevices++;

        if (Waarde_Knop == 1 && strcmp(deviceNaam, "LITE") == 0)
        {
            send(socket, Led, strlen(Led), 0);
            while (1) {
                read(socket, versturenbuffer, sizeof(versturenbuffer) - 1);
                send(socket, Led, strlen(Led), 0);
                printf("Led message sent\n");
                printf("Huidige buffer in aansturen: %s\n", versturenbuffer);
                memset(versturenbuffer, 0, sizeof(versturenbuffer));
                if (strcmp(versturenbuffer, "LEDAAN") != 0) {
                    break;
                }
            }
            Waarde_Knop = 0;
        }

        if (RGBWaarde != 0 && strcmp(deviceNaam, "LITE") == 0)
        {
            snprintf(RGBSend, sizeof(RGBSend), "%d - RGBWAARDE\n", RGBWaarde);
            send(socket, RGBSend, strlen(RGBSend), 0);

            while (1) {
                read(socket, versturenbuffer, sizeof(versturenbuffer) - 1);
                send(socket, RGBSend, strlen(RGBSend), 0);
                printf("RGB message sent\n");
                memset(versturenbuffer, 0, sizeof(versturenbuffer));
                if (strcmp(versturenbuffer, "RGBAAN") != 0) {
                    break;
                }
            }
            RGBWaarde = 0;
        }

        if (Pi_a_Led == 1 && strcmp(deviceNaam, "LITE") == 0)
        {
            read(socket, versturenbuffer, sizeof(versturenbuffer) - 1);
            send(socket, Led, strlen(Led), 0);
            printf("Pi-Led message sent\n");
            memset(versturenbuffer, 0, sizeof(versturenbuffer));
            Pi_a_Led = 0;
        }
        if (tellerdevices == Devices)
        {
            Versturen = false;
            tellerdevices = 0;
            
        }

        memset(versturenbuffer, 0, sizeof(versturenbuffer));
    }

    return 0;
}

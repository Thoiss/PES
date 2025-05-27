#include "wemos_aansturen.hpp"

WemosAansturen::WemosAansturen()
    : tellerdevices(0)
{
    memset(versturenbuffer, 0, sizeof(versturenbuffer));
}

int WemosAansturen::stuurWemosAan(int socket)
{
    if (Versturen) 
    {
        tellerdevices++;
         printf("RGB Waarde: %d\n", RGBWaarde);
            snprintf(RGBSend, sizeof(RGBSend),"%d - %d - RGBWAARDE\n",statusVerlichting, verlichtingsWaarde);
            send(socket, RGBSend, strlen(RGBSend), 0);
            RGBWaarde = 0;
            Versturen = false;
            printf("RGB Waarde: %d\n", RGBWaarde);


        memset(versturenbuffer, 0, sizeof(versturenbuffer));
    }

    return 0;
}

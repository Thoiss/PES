#include "wemos_aansturen.hpp"

WemosAansturen::WemosAansturen()
    : tellerdevices(0)
{
    memset(versturenbuffer, 0, sizeof(versturenbuffer));
}

int WemosAansturen::stuurWemosAan(int socket, int deviceIndex, int statusVerlichting, int verlichtingsWaarde, bool Versturen)
{
    if (Versturen) 
    {
        if (deviceIndex == 1){
            printf("Socket correct\n");
            tellerdevices++;
            snprintf(RGBSend, sizeof(RGBSend),"%d - %d - RGBWAARDE\n",statusVerlichting, verlichtingsWaarde);
            send(socket, RGBSend, strlen(RGBSend), 0);
            Versturen = false;
        }

        memset(versturenbuffer, 0, sizeof(versturenbuffer));
    }

    return 0;
}

int WemosAansturen::routeWemos(int socket, int deviceIndex, int statusRouteVerlichting, bool Versturen)
{
    if (Versturen) 
    {
        if (deviceIndex == 1){
            printf("Socket correct\n");
            tellerdevices++;
            snprintf(routeSend, sizeof(routeSend),"routeVerlichting: %d \n",statusRouteVerlichting);
            send(socket, routeSend, strlen(routeSend), 0);
            Versturen = false;
        }

        memset(versturenbuffer, 0, sizeof(versturenbuffer));
    }

    return 0;
}
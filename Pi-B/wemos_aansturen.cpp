#include "wemos_aansturen.hpp"

WemosAansturen::WemosAansturen()
{
    memset(versturenbuffer, 0, sizeof(versturenbuffer));
}

int WemosAansturen::stuurWemosAan(int socket, int deviceIndex, int statusVerlichting, int verlichtingsWaarde, bool Versturen)
{
    if (Versturen) 
    {
        if (deviceIndex == 1){
            printf("Socket correct\n");
            snprintf(RGBSend, sizeof(RGBSend),"%d - %d - RGBWAARDE\n",statusVerlichting, verlichtingsWaarde);
            printf("Verlichting %s\n", RGBSend);
            if(send(socket, RGBSend, strlen(RGBSend), 0) <= 0) 
            {
                perror("Fout bij versturen naar Wemos. Socket wordt geclosed.");
                //close(socket);
                return -1;
                
            }
            
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
            snprintf(routeSend, sizeof(routeSend),"%d\n",statusRouteVerlichting);
            printf("%s\n", routeSend);
            if(send(socket, routeSend, strlen(routeSend), 0) <= 0) 
            {
                perror("Fout bij versturen naar Wemos. Socket wordt geclosed.");
                //close(socket);
                return -1;
                
            }
            Versturen = false;
        }

        memset(versturenbuffer, 0, sizeof(versturenbuffer));
    }

    return 0;
}
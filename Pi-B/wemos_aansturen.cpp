#include "wemos_aansturen.hpp"

WemosAansturen::WemosAansturen()
{
    memset(versturenbuffer, 0, sizeof(versturenbuffer));
}
// stuur wemos met Verlichting aan (d.m.v. Encoder)
int WemosAansturen::stuurWemosAan(int socket, int deviceIndex, int statusVerlichting, int verlichtingsWaarde, bool Versturen)
{
    if (Versturen) 
    {
        // is correcte wemos meegegeven in de functie?        
        if (deviceIndex == 1){
            printf("Socket correct\n");
            // voeg de waardes toe in 1 bericht
            snprintf(RGBSend, sizeof(RGBSend),"%d - %d - RGBWAARDE\n",statusVerlichting, verlichtingsWaarde);
            printf("Verlichting %s\n", RGBSend);
            // stuur bericht
            if(send(socket, RGBSend, strlen(RGBSend), 0) <= 0) 
            {
                perror("Fout bij versturen naar Wemos. Socket wordt geclosed.");
                return -1;
            }

            Versturen = false;
        }
        // leeg buffer
        memset(versturenbuffer, 0, sizeof(versturenbuffer));
    }

    return 0;
}

// stuur wemos met Routeverlichting aan (d.m.v. RFID)
int WemosAansturen::routeWemos(int socket, int deviceIndex, int statusRouteVerlichting, bool Versturen)
{
    if (Versturen) 
    {
        // is correcte wemos meegegeven in de functie?
        if (deviceIndex == 1){
            printf("Socket correct\n");
            snprintf(routeSend, sizeof(routeSend),"%d\n",statusRouteVerlichting);
            printf("%s\n", routeSend);
            // stuur bericht
            if(send(socket, routeSend, strlen(routeSend), 0) <= 0) 
            {
                perror("Fout bij versturen naar Wemos. Socket wordt geclosed.");
                return -1;
            }
            Versturen = false;
        }
        // leeg buffer
        memset(versturenbuffer, 0, sizeof(versturenbuffer));
    }

    return 0;
}
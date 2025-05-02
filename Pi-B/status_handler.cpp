#include "globals.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int vraag_knop_status(int Socket)
{ // Stuur request naar wemos voor knopstatus
    const char *hello       = "Hello from client\n";
    const char *Led         = "Led\n";
    const char *wemosStatus = "wemosStatus\n";

    memset(buffer, 0, sizeof(buffer));        // buffer leegmaken
    send(Socket, hello, strlen(hello), 0);    // stuur hello naar Socket x
    printf("%d: Hello message sent\n", Socket);

    if (Versturen) // check of iets uitgevoerd moet worden, true wordt gezet door Pi_Connectie()
    {
        if (Waarde_Knop == 1 )
        {
            send(Socket, Led, strlen(Led), 0);
            printf("Led message sent\n");
            Waarde_Knop = 0;
        }
        if (RGBWaarde != 0)
        {
            snprintf(RGBSend, sizeof(RGBSend), "%d - RGBWAARDE\n", RGBWaarde);
            send(Socket, RGBSend, strlen(RGBSend), 0);
            printf("RGB message sent\n");
            RGBWaarde = 0;
        }  
        if (Pi_a_Led == 1 ) // If bericht van Pi-A ontvangen
        {
            send(Socket, Led, strlen(Led), 0);
            printf("Pi-Led message sent\n");
        }
        if (Socket == 5) { // bij de laatste Socket, zet versturen op false
            Versturen = false;
            Pi_a_Led  = 0;
        }
    }

    send(Socket, wemosStatus, strlen(wemosStatus), 0); // status request
    read(Socket, buffer, sizeof(buffer) - 1);          // leest response
    printf("Buffer: %s\n", buffer);                     // print bericht

    char *token = strtok(buffer, " "); // leest/parst tot 1e spatie
    if (strcmp(token, "TRUE") == 0)    // als token klopt...
    { // kijk of ontvangen == 'TRUE'
        Waarde_Knop = 1;
        Versturen    = true;
    }
    token = strtok(NULL, " ");         // volgende deel
    if (strcmp(token, "PiTRUE") == 0)
    {
        PiWaarde_Knop = 1;
    }
    token = strtok(NULL, " ");         // volgende deel
    printf("Huidig: %s\n", token);
    if (strcmp(token, "1") == 0 || strcmp(token, "2") == 0 || strcmp(token, "3") == 0)
    {
        RGBWaarde = atoi(token);       // Converteer naar integer
        printf("RGBWaarde: %d\n", RGBWaarde);
        Versturen = true;
    }
    memset(buffer, 0, sizeof(buffer));
    return 0;
}

#include "status_handler.hpp"

StatusHandler::StatusHandler(int socket, WemosAansturen& aansturen)
    : socket_(socket), wemosAansturen_(aansturen)
{}

int StatusHandler::vraagKnopStatus()     // Stuur request naar wemos voor knopstatus
{
    const char *hello      = "Hello from client\n";
    const char *wemosStatus = "wemosStatus\n";
    char *deviceNaam;
    int retry = 0;
    send(socket_, wemosStatus, strlen(wemosStatus), 0);   // status request
    memset(buffer, 0, sizeof(buffer));
    read(socket_, buffer, sizeof(buffer) - 1);            // leest response
    // printf("Buffer na aansturen wemos: %s\n", buffer);
    while (buffer[0] == '\0' && retry < 3) {   
        retry++;
        memset(buffer, 0, sizeof(buffer));
        read(socket_, buffer, sizeof(buffer) - 1);
        printf("Buffer while loop: %s\n", buffer);
        send(socket_, "Test\n", strlen("Test\n"), 0);
    }

    if (buffer[0] != '\0') {
        printf("Start token: %s\n", buffer);
        char *token = strtok(buffer, " ");    // leest/parst tot 1e spatie

        if (strcmp(token, "TRUE") == 0) {     // als token 'TRUE'
            send(socket_, "LED_ACK\n", strlen("LED_ACK\n"), 0);
            Waarde_Knop = 1;
            Versturen = true;
        }
        else if (strcmp(token, "FALSE") != 0) {
            return 0;
        }

        token = strtok(nullptr, " ");         // volgende deel
        if (strcmp(token, "PiTRUE") == 0) {
            send(socket_, "PILED_ACK\n", strlen("PILED_ACK\n"), 0);
            PiWaarde_Knop = 1;
        }

        token = strtok(nullptr, " ");         // volgende deel
        if (strcmp(token, "1") == 0
         || strcmp(token, "2") == 0
         || strcmp(token, "3") == 0) {
            RGBWaarde = atoi(token);          // Converteer naar integer
            send(socket_, "RGB_ACK\n", strlen("RGB_ACK\n"), 0);
            printf("RGBWaarde: %d\n", RGBWaarde);
            Versturen = true;
        }
        token = strtok(nullptr, " ");         // volgende deel
        deviceNaam = token;
    }
    wemosAansturen_.stuurWemosAan(deviceNaam, socket_);
    memset(buffer, 0, sizeof(buffer));
    return 0;
}

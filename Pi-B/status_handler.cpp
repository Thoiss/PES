#include "status_handler.hpp"

StatusHandler::StatusHandler(int socket, WemosAansturen& aansturen)
    : socket_(socket), wemosAansturen_(aansturen)
{
    memset(buffer, 0, sizeof(buffer));  // Ensure buffer is initialized
}

int StatusHandler::vraagKnopStatus() // Send request to Wemos for button status
{
    const char* hello = "Hello from client\n";
    const char* wemosStatus = "wemosStatus\n";
    char* deviceNaam = nullptr;
    int retry = 0;

    if (socket_ <= 0) {
        printf("Invalid socket\n");
        return -1;  // Early exit if the socket is invalid
    }

    // Send the request for status
    send(socket_, wemosStatus, strlen(wemosStatus), 0);
    memset(buffer, 0, sizeof(buffer));

    // Read the response from the socket
    ssize_t bytesRead = read(socket_, buffer, sizeof(buffer) - 1);
    if (bytesRead <= 0) {
        printf("Failed to read from socket or no data received.\n");
        return -1;  // Early exit if reading fails
    }

    printf("Buffer after sending Wemos request: %s\n", buffer);

    // Retry mechanism if no data or empty response
    while (buffer[0] == '\0' && retry < 3) {
        retry++;
        memset(buffer, 0, sizeof(buffer));
        read(socket_, buffer, sizeof(buffer) - 1);
        printf("Retry %d, Buffer: %s\n", retry, buffer);
        send(socket_, "Test\n", strlen("Test\n"), 0);  // Send test message during retries
    }

    if (buffer[0] == '\0') {
        printf("No valid response after retries.\n");
        return -1;  // Exit if no response received after retries
    }

    printf("Start token: %s\n", buffer);
    char* token = strtok(buffer, " ");  // Parse until the first space

    if (token == nullptr) {
        printf("Invalid token in response.\n");
        return -1;  // Handle invalid token (e.g., empty response)
    }

    // Handle the token and perform corresponding actions
    if (strcmp(token, "TRUE") == 0) {  // If token is 'TRUE'
        send(socket_, "LED_ACK\n", strlen("LED_ACK\n"), 0);
        Waarde_Knop = 1;
        Versturen = true;
    } else if (strcmp(token, "FALSE") != 0) {
        return 0;
    }

    token = strtok(nullptr, " ");  // Next part of the response
    if (token != nullptr && strcmp(token, "PiTRUE") == 0) {
        send(socket_, "PILED_ACK\n", strlen("PILED_ACK\n"), 0);
        PiWaarde_Knop = 1;
    }
// Oud wemos RGB waarde met cases 
    // token = strtok(nullptr, " ");  // Next part of the response
    // if (token != nullptr && (strcmp(token, "1") == 0 || strcmp(token, "2") == 0 || strcmp(token, "3") == 0)) {
    //     RGBWaarde = atoi(token);  // Convert to integer
    //     send(socket_, "RGB_ACK\n", strlen("RGB_ACK\n"), 0);
    //     printf("RGBWaarde: %d\n", RGBWaarde);
    //     Versturen = true;
    // }

    token = strtok(nullptr, " ");  // Last part of the response
    deviceNaam = token;  // Assign device name

    if (deviceNaam != nullptr) {
        wemosAansturen_.stuurWemosAan(deviceNaam, socket_);
    } else {
        printf("No device name found in the response.\n");
        return -1;  // Exit if no device name found
    }

    memset(buffer, 0, sizeof(buffer));  // Clear buffer after handling
    return 0;  // Successful handling
}

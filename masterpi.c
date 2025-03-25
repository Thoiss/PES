#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>

#define PORT 8080
#define SLAVE_ADDRESS 0x11  // Het I2C-adres van de STM32 slave
#define SLAVE_ADDRESS2 0x13

int button_status;

int send_message(int socket, char *message) {
    // Stuur een bericht naar de client
    if (send(socket, message, strlen(message), 0) < 0) {
        perror("Send failed");
        return -1;
    }
    return 0;
}

int main(int argc, char const *argv[]) {
    int fd, fb;
    int server_fd, new_socket, max_sd, activity, i, valread;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = {0};
    fd_set readfds; // File descriptor set voor select()
    int client_sockets[30]; // Array om actieve client sockets op te slaan
    int num_clients = 0; // Aantal actieve clients

    if (wiringPiSetup() == -1) {
        printf("Fout bij het instellen van wiringPi\n");
        return 1;
    }

    // Open de I2C bus en connecteer met de slave
    fd = wiringPiI2CSetup(SLAVE_ADDRESS);
    if (fd == -1) {
        printf("Fout bij het openen van slave 1\n");
        return 1;
    }
    fb = wiringPiI2CSetup(SLAVE_ADDRESS2);
    if (fb == -1) {
        printf("Fout bij het openen van slave 2\n");
        return 1;
    }

    // Socket aanmaken
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Opties instellen voor hergebruik van adres en poort
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind de socket aan poort 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server luistert op poort %d...\n", PORT);

    // Zet de server socket op non-blocking
    if (fcntl(server_fd, F_SETFL, O_NONBLOCK) < 0) {
        perror("fcntl failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Initialiseer alle client sockets op -1 (geen verbinding)
    for (i = 0; i < 30; i++) {
        client_sockets[i] = -1;
    }

    while (1) {
        FD_ZERO(&readfds);  // Reset de file descriptor set
        FD_SET(server_fd, &readfds);  // Voeg de server socket toe aan de set
        max_sd = server_fd;

        // Voeg alle client sockets toe aan de readfds
        for (i = 0; i < num_clients; i++) {
            int sd = client_sockets[i];

            // Voeg alleen actieve client sockets toe
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }

            if (sd > max_sd) {
                max_sd = sd;
            }
        }

        // Wachten op activiteit (verbinding of data)
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)) {
            printf("Select error\n");
        }

        // Check of er een inkomende verbinding is
        if (FD_ISSET(server_fd, &readfds)) {
            new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
            if (new_socket < 0) {
                perror("accept");
                continue;
            }

            printf("Nieuwe client verbonden!\n");

            // Zet de client socket op non-blocking
            if (fcntl(new_socket, F_SETFL, O_NONBLOCK) < 0) {
                perror("fcntl failed");
                close(new_socket);
                continue;
            }

            // Voeg de nieuwe client toe aan de set van client_sockets
            if (num_clients < 1) {
                client_sockets[num_clients++] = new_socket;
            } else {
                printf("Maximaal aantal clients bereikt.\n");
            }
        }

        // Controleer of er data van een client is
        for (i = 0; i < num_clients; i++) {
            int sd = client_sockets[i];

            if (FD_ISSET(sd, &readfds)) {
                valread = read(sd, buffer, sizeof(buffer) - 1);
                if (valread > 0) {
                    buffer[valread] = '\0'; // Zorg ervoor dat de string goed eindigt
                    printf("Ontvangen bericht van client %d: %s\n", sd, buffer);

                    if(strcmp(buffer, "LED") == 0) {
                    wiringPiI2CWrite(fb, '1');
                    delay(1000);
                    wiringPiI2CWrite(fb, '0');
                    }
                    if(strcmp(buffer, "Status") == 0) {
                        for (i = 0; i < num_clients; i++) {
                            button_status = wiringPiI2CRead(fd);
                            if (button_status == 0) {
                            int sd = client_sockets[i];
                            char message[] = "uit";
                            if (send_message(sd, message) == 0) {
                                printf("knopstatus client: %d.\n", sd);
                                }
                            }
                            if (button_status == 1) {
                                int sd = client_sockets[i];
                                char message[] = "aan";
                                if (send_message(sd, message) == 0) {
                                    printf("knopstatus client: %d.\n", sd);
                                    }
                                }
                        }
                         }
                     }
                }
        }
        memset(buffer, 0, sizeof(buffer)); // Reset de buffer
    }

    return 0;
}

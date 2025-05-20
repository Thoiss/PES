#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>   // Voor inet_pton()
#include <netinet/in.h>  // Voor sockaddr_in
#include <sys/socket.h>  // Voor socket(), connect()
#include <unistd.h>      // Voor read(), close()
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>       // Voor fcntl()
#include <sys/select.h>  // Voor fd_set, select()
#include <sys/time.h>    // Voor struct timeval

#define PORT      8888   // Wemos port
#define PI_A_PORT 8080   // Pi-A port
#define TIMEOUT   3      // Timeout in seconden
#define Devices   3      // Aantal Wemos apparaten

// IP-adressen van de Wemos-apparaten
extern const char *wemos_ips[Devices];
// Raspberry Pi A             
extern const char *pi_a_ip;

// Voor connecties
extern struct sockaddr_in serv_addr;
extern int pi_a_socket;

// Leesbuffer
extern char buffer[1024];
extern ssize_t valread;

// Knop- en LED-waarden
extern int PiWaarde_Knop;
extern int Waarde_Knop;
extern int Pi_a_Led;
extern char RGBSend[15];
extern int RGBWaarde;
extern bool Versturen;

#endif // GLOBALS_H

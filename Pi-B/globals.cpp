#include "globals.h"

// IP-adressen
const char *wemos_ips[Devices] = {
     "192.168.137.20", // Wemos 1
     "192.168.137.30",  // Wemos 2
     "192.168.137.40" // Wemos 3
};
const char *pi_a_ip = "192.168.137.46"; // Raspberry Pi A

// Socket-structuur voor connect_to_wemos()
struct sockaddr_in serv_addr;

// Socket voor Pi-A
int pi_a_socket;

// Leesbuffer
char buffer[1024] = {0};
ssize_t valread;

// Knop- en LED-waarden
int PiWaarde_Knop = 0;
int Waarde_Knop    = 0;
int Pi_a_Led       = 0;
char RGBSend[15]   = " - RGBWAARDE";
int RGBWaarde      = 0;
bool Versturen     = false;

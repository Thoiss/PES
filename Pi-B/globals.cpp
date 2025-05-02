#include "globals.h"

// IP-adressen
const char *wemos_ips[Devices] = {
    "192.168.137.152", // Wemos 1
    "192.168.137.224"  // Wemos 2
};
const char *pi_a_ip = "192.168.137.31"; // Raspberry Pi A

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

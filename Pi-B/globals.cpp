#include "globals.h"

// IP-adressen
const char *wemos_ips[Devices] = {
     "192.168.137.20", // Wemos 1 (Pro - inputs)
     "192.168.137.30",  // Wemos 2 (Lite - lampjes)
     "192.168.137.40" //Wemos 3 (Lite - lichtkrant)
};
const char *pi_a_ip = "192.168.137.46"; // Raspberry Pi A

// Socket-structuur voor connect_to_wemos()
struct sockaddr_in serv_addr;

// Leesbuffer
char buffer[1024] = {0};
ssize_t valread;

// Knop- en LED-waarden
int PiWaarde_Knop = 0;
int Waarde_Knop    = 0;
int Pi_a_Led       = 0;
char RGBSend[30]   = " - RGBWAARDE";
int RGBWaarde      = 0;
bool Versturen     = false;
int verlichtingsWaarde = 0;
int statusVerlichting =0;
#include "globals.h"

WiFiServer server(8888);
char Status[200] = "";
WiFiClient clients[MAX_CLIENTS];
unsigned long lastActiveTime[MAX_CLIENTS] = { 0 };

int Print_Knop = 0;
int PiPrint_Knop = 0;
int RGB_Preset_Knop = 0;
int RGBWaarde = 0;

// WiFi
const char* ssid = "pinetwerk";
const char* password = "bok12345";
IPAddress ip(192, 168, 137, 20);        // IP van deze Wemos
IPAddress gateway(192, 168, 137, 1);    // IP van de Raspberry Pi (de gateway)
IPAddress subnet(255, 255, 255, 0);   // Subnetmasker

#include "globals.h"

// WiFi
const char* ssid     = "pinetwerk";
const char* password = "bok12345";
IPAddress ip(192, 168, 137, 30);        // IP van deze Wemos
IPAddress gateway(192, 168, 137, 1);    // IP van de Raspberry Pi (de gateway)
IPAddress subnet(255, 255, 255, 0);   // Subnetmasker
  
// Status buffer
char Status[200] = "";

// Server & clients
WiFiServer server(8888);
WiFiClient clients[MAX_CLIENTS];
unsigned long lastActiveTime[MAX_CLIENTS] = { 0 };

// Knop-flags
int Print_Knop = 0;
int PiPrint_Knop = 0;

// SHT31-sensor
Adafruit_SHT31 sht31 = Adafruit_SHT31();

// LED-timer
Ticker ledTimer;

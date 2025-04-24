#ifndef GLOBALS_H
#define GLOBALS_H

#include <ESP8266WiFi.h>

#define MAX_CLIENTS 5                               // Aantal toegestane clients
#define Time_Delay 10000                            // 10 seconden timeout
#define BUTTON_PIN D1                               // Knop op D1 (GPIO5)
#define PI_PIN D2                                   // Knop op D2 (GPIO4)
#define RGB_Button D5                               // RGB_Button op D5 (GPIO14)

extern WiFiServer server;
extern char Status[200];
extern WiFiClient clients[MAX_CLIENTS];
extern unsigned long lastActiveTime[MAX_CLIENTS];

extern int Print_Knop;
extern int PiPrint_Knop;
extern int RGB_Preset_Knop;
extern int RGBWaarde;

// WiFi
extern const char* ssid;
extern const char* password;
extern IPAddress ip;        // IP van deze Wemos
extern IPAddress gateway;    // IP van de Raspberry Pi (de gateway)
extern IPAddress subnet;   // Subnetmasker

#endif  // GLOBALS_H

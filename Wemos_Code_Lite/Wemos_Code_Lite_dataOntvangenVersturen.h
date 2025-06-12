#ifndef WEMOS_LITE_DATAHANDLER_H
#define WEMOS_LITE_DATAHANDLER_H
#include <ESP8266WiFi.h>
#define MAX_CLIENTS 5
#define RED_PIN_1   D6 // Diffused RGB-led
#define GREEN_PIN_1 D7 // Diffused RGB-led
#define BLUE_PIN_1  D5 // Diffused RGB-led

#define RED_PIN_2   D2 // Non-Diffused RGB-led
#define GREEN_PIN_2 D3 // Non-Diffused RGB-led
#define BLUE_PIN_2  D4 // Non-Diffused RGB-led
void dataOntvangenVersturen(WiFiClient clients[]);
void setRGB1Color(int, int, int);
void setRGB2Color(int, int, int);
void RGB1Aanzetten();
void RGB2Aanzetten();
void routeVerlichtingAan();
int waarde = 0;
#endif // WEMOS_LITE_DATAHANDLER_H

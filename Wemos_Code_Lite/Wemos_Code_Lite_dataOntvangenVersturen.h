#ifndef WEMOS_LITE_DATAHANDLER_H
#define WEMOS_LITE_DATAHANDLER_H
#include <ESP8266WiFi.h>
#define MAX_CLIENTS 5
void dataOntvangenVersturen(WiFiClient clients[]);
void RGBAanzetten();
#endif // WEMOS_LITE_DATAHANDLER_H

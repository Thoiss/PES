#ifndef WEMOS_LITE_NEWCLIENT_H
#define WEMOS_LITE_NEWCLIENT_H
// Server & clients
#include <ESP8266WiFi.h>
#define MAX_CLIENTS 5
void newClient(WiFiServer &server, WiFiClient clients[]);
#endif // WEMOS_LITE_NEWCLIENT_H

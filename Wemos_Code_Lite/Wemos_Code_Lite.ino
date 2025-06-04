#include "Wemos_Code_Lite_newClient.h"
#include "Wemos_Code_Lite_dataOntvangenVersturen.h"
IPAddress ip(192, 168, 137, 30);        // IP van deze Wemos
IPAddress gateway(192, 168, 137, 1);    // IP van de Raspberry Pi (de gateway)
IPAddress subnet(255, 255, 255, 0);   // Subnetmasker
const char* ssid     = "pinetwerk";
const char* password = "bok12345";
WiFiServer server(8888);
WiFiClient clients[MAX_CLIENTS];
unsigned long lastActiveTime[MAX_CLIENTS] = { 0 };
void setup() {
  Serial.begin(57600);
  Serial.println("\nOpstarten...");
  WiFi.config(ip, gateway, subnet);
  WiFi.hostname("wemosLite");
  Serial.println("Sensor gevonden!");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nVerbonden met WiFi!");
  Serial.print("IP Adres: ");
  Serial.println(WiFi.localIP());
  server.begin();
  RGB1Aanzetten();
  RGB2Aanzetten();
}

void loop() {
  newClient(server, clients);
  dataOntvangenVersturen(clients);
}

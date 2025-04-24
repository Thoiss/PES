#include "globals.h"
#include "Wemos_Code_Lite_newClient.h"
#include "Wemos_Code_Lite_dataOntvangenVersturen.h"

void setup() {
  Serial.begin(115200);
  Serial.println("\nOpstarten...");
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

  pinMode(LED_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  newClient();
  dataOntvangenVersturen();
}

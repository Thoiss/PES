#include <ESP8266WiFi.h>
#include "globals.h"
#include "Wemos_Code_Pro_newClient.h"
#include "Wemos_Code_Pro_dataOntvangenVersturen.h"
#include "Wemos_Code_Pro_inputs.h"

void setup() {
  Serial.begin(57600);
  Serial.println("\nOpstarten...");
  WiFi.config(ip, gateway, subnet);
  WiFi.hostname("wemosPro");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nVerbonden met WiFi!");
  Serial.print("IP Adres: ");
  Serial.println(WiFi.localIP());

  server.begin();  // Start de server


  pinMode(PI_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RGB_Button, INPUT_PULLUP);
}

void loop() {
  newClient();
  dataOntvangenVersturen();
  input_controle();
}
#include "LichtkrantServer.h"
#include <ESP8266WiFi.h>
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>

// Matrix Display Config
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 5
#define DATA_PIN 13  // D7
#define CLK_PIN  14  // D5
#define CS_PIN   15  // D8

MD_Parola matrix = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
String scrollText = "enabled";

// WiFi Config
const char* ssid = "pinetwerk";
const char* password = "bok12345";
IPAddress ip(192, 168, 137, 40);
IPAddress gateway(192, 168, 137, 1);
IPAddress subnet(255, 255, 255, 0);
WiFiServer server(8888);  // Define server before using it

// Create LichtkrantServer object AFTER matrix and scrollText are defined
LichtkrantServer lichtkrant(server, matrix, scrollText);

// Client/Status Config
#define MAX_CLIENTS 5
WiFiClient clients[MAX_CLIENTS];
unsigned long lastActiveTime[MAX_CLIENTS] = { 0 };

void setup() {
  Serial.begin(115200);
  Serial.println("\nOpstarten...");

  // Setup WiFi
  WiFi.config(ip, gateway, subnet);
  WiFi.hostname("wemosLiteLichtkrant");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nVerbonden met WiFi!");
  Serial.print("IP Adres: ");
  Serial.println(WiFi.localIP());
  server.begin();

  // Setup Matrix
  matrix.begin();
  matrix.setIntensity(2);
  matrix.displayClear();
  matrix.displayText(scrollText.c_str(), PA_CENTER, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void loop() {
  lichtkrant.start();
}

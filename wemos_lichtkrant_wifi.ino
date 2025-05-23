#include <ESP8266WiFi.h>
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>

// WiFi Config
const char* ssid = "pinetwerk";
const char* password = "bok12345";
IPAddress ip(192, 168, 137, 40);
IPAddress gateway(192, 168, 137, 1);
IPAddress subnet(255, 255, 255, 0);
WiFiServer server(8888);

// Matrix Display Config
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 5
#define DATA_PIN 13  // D7
#define CLK_PIN  14  // D5
#define CS_PIN   15  // D8
MD_Parola matrix = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
String scrollText = "enabled";

// Client/Status Config
#define MAX_CLIENTS 5
#define Time_Delay 10000
#define BUTTON_PIN D1
#define PI_PIN D2
#define RGB_Button D5
WiFiClient clients[MAX_CLIENTS];
unsigned long lastActiveTime[MAX_CLIENTS] = { 0 };
char Status[200] = "";
int Print_Knop = 0;
int PiPrint_Knop = 0;
int RGB_Preset_Knop = 0;
int RGBWaarde = 0;

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

  // Setup buttons
  pinMode(PI_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RGB_Button, INPUT_PULLUP);

  // Setup Matrix
  matrix.begin();
  matrix.setIntensity(2);
  matrix.displayClear();
  matrix.displayText(scrollText.c_str(), PA_CENTER, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void loop() {
  // Matrix animation
  if (matrix.displayAnimate()) {
    matrix.displayReset();
  }

  // Check new clients
  WiFiClient newClient = server.available();
  if (newClient) {
    bool added = false;
    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (!clients[i]) {
        clients[i] = newClient;
        lastActiveTime[i] = millis();
        Serial.printf("Client verbonden op slot: %d\n", i);
        added = true;
        break;
      }
    }
    if (!added) {
      Serial.println("Server vol! Nieuwe client wordt geweigerd.");
      newClient.stop();
    }
  }

  // Handle existing clients
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i]) {
      if (clients[i].connected()) {
        if (clients[i].available()) {
          String data = clients[i].readStringUntil('\n');
          data.trim();
          Serial.printf("Ontvangen van client %d: %s\n", i, data.c_str());

          // Handle display command
          if (data.startsWith("text:")) {
            scrollText = data.substring(5);
            Serial.println("Nieuw tekst voor matrix: " + scrollText);
            matrix.displayClear();
            matrix.displayText(scrollText.c_str(), PA_CENTER, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
            clients[i].println("Display updated.");
          }

          // Status check
          else if (data == "wemosStatus") {
            memset(Status, 0, sizeof(Status));
            strcat(Status, Print_Knop ? " TRUE" : " FALSE");
            strcat(Status, PiPrint_Knop ? " PiTRUE" : " PiFALSE");
            if (RGB_Preset_Knop) {
              char RGBWaardeStr[10];
              sprintf(RGBWaardeStr, "%d", RGBWaarde);
              strcat(Status, " ");
              strcat(Status, RGBWaardeStr);
            } else {
              strcat(Status, " RGBFALSE");
            }
            Print_Knop = PiPrint_Knop = RGB_Preset_Knop = 0;
            clients[i].println(Status);
          }

          // Disconnect client
          else if (data == "End") {
            clients[i].println("Verbinding wordt afgesloten door server");
            Serial.printf("Client %d is losgekoppeld.\n", i);
            clients[i].stop();
            clients[i] = WiFiClient();
          }
        }
      }
    }
  }

  // Button checks
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(200);
    if (digitalRead(BUTTON_PIN) == LOW && Print_Knop == 0) {
      Serial.println("Knop is ingedrukt.");
      Print_Knop = 1;
    }
  }

  if (digitalRead(PI_PIN) == LOW) {
    delay(200);
    if (digitalRead(PI_PIN) == LOW && PiPrint_Knop == 0) {
      Serial.println("PiKnop is ingedrukt.");
      PiPrint_Knop = 1;
    }
  }

  if (digitalRead(RGB_Button) == LOW) {
    delay(250);
    if (digitalRead(RGB_Button) == LOW && RGB_Preset_Knop == 0) {
      Serial.println("RGB Knop is ingedrukt.");
      RGBWaarde++;
      if (RGBWaarde >= 4) RGBWaarde = 1;
      RGB_Preset_Knop = 1;
    }
  }
}

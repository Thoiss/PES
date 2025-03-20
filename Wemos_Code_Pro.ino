#include <ESP8266WiFi.h>
const char* ssid = "pinetwerk";
const char* password = "bok12345";
WiFiServer server(8888);

#define MAX_CLIENTS 5                               // Aantal toegestane clients
#define Time_Delay 10000                            // 10 seconden timeout
#define BUTTON_PIN D1                               // Knop op D1 (GPIO5)
#define PI_PIN D2                                   // Knop op D2 (GPIO4)
#define RGB_Button D5                               // RGB_Button op D5 (GPIO14)
WiFiClient clients[MAX_CLIENTS];                    // Array om clients op te slaan
unsigned long lastActiveTime[MAX_CLIENTS] = { 0 };  // Houd de laatste activiteit per client bij

int Print_Knop = 0;
int PiPrint_Knop = 0;
int RGB_Preset_Knop = 0;
int RGBWaarde = 0;


void setup() {
  Serial.begin(115200);
  Serial.println("\nOpstarten...");
  WiFi.begin(ssid, password);
  // Wachten op verbinding
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nVerbonden met WiFi!");
  Serial.print("IP Adres: ");
  Serial.println(WiFi.localIP());

  server.begin();  // Start de server

  pinMode(PI_PIN, INPUT_PULLUP);      // Knop Pi communicatie
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Knop als input met interne pull-up
  pinMode(RGB_Button, INPUT_PULLUP);
}

void loop() {
  // Check voor nieuwe clients
  WiFiClient newClient = server.available();
  if (newClient) {
    Serial.println("Nieuwe client probeert te verbinden...");
    bool added = false;

    // Toevoegen aan een lege plek in de array
    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (!clients[i]) {  // Als de slot leeg is (geen actieve client)
        clients[i] = newClient;
        lastActiveTime[i] = millis();  // Start timer voor deze client
        Serial.print("Client verbonden op slot: ");
        Serial.println(i);
        added = true;
        break;
      }
    }

    if (!added) {
      Serial.println("Server vol! Nieuwe client wordt geweigerd.");
      newClient.stop();  // Sluit verbinding af als er geen plek is
    }
  }

  // Check actieve clients
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i]) {  // Als er een actieve client is
      if (clients[i].connected()) {
        if (clients[i].available()) {
          String data = clients[i].readStringUntil('\n');
          // String data = "";

          // char c = clients[i].read();
          // data += c;

          data.trim();
          Serial.print("Ontvangen van client ");
          Serial.print(i);
          Serial.print(": ");
          Serial.println(data);
          // clients[i].print("Echo: " + data);  // Stuur bericht terug


          if (data == "End") {
            clients[i].print("Verbinding wordt afgesloten door server");
            Serial.print("Client ");
            Serial.print(i);
            Serial.println(" is losgekoppeld.");
            clients[i].stop();          // Sluit client af
            clients[i] = WiFiClient();  // Maak de slot weer vrij
          }
          if (data == "Knop") {
            if (Print_Knop == 1){
            Serial.print("Op data is knop uitgelezen");
              clients[i].print("TRUE"); //5,D1  (Destination socket, Destination Pin)
              Print_Knop = 0;
            }
            else{
              clients[i].print("FALSE");
            }
          }
          if (data == "RGB") {
            if (RGB_Preset_Knop == 1){
            Serial.print("RGB Waarde verstuurd \n");
              clients[i].print(RGBWaarde);
              RGB_Preset_Knop = 0;
            }
            else{
              clients[i].print("RGBFALSE");
            }
          }
          if (data == "PiTest") {
            if (PiPrint_Knop == 1){
            Serial.print("Op data is knop uitgelezen");
              clients[i].print("PiTRUE"); 
              PiPrint_Knop = 0;
            }
            else{
              clients[i].print("PiFALSE");
            }
          }
        }
      }

      // **Check timeout van 10 seconden**
      // if (millis() - lastActiveTime[i] > Time_Delay) {
      //   Serial.print("Client ");
      //   Serial.print(i);
      //   Serial.println(" is losgekoppeld wegens inactiviteit.");
      //   clients[i].stop();          // Sluit client af
      //   clients[i] = WiFiClient();  // Maak de slot weer vrij
      // }
    }
  }

  int buttonState = digitalRead(BUTTON_PIN);  // Lees de knopstatus voor de test tussen Wemos
  int PibuttonState = digitalRead(PI_PIN);  // Lees de knopstatus voor de Pi
  int RGB_Preset_buttonState = digitalRead(RGB_Button);

  if (buttonState == LOW) {  // Knop ingedrukt
    delay(300);
    if (buttonState == LOW) {
      if (Print_Knop == 0) {
        Serial.println("Knop is ingedrukt.");
      }
      Print_Knop = 1;
    }

  }
  if (RGB_Preset_buttonState == LOW) {  // Knop ingedrukt
    delay(300);
    if (RGB_Preset_buttonState == LOW) {
      if (RGB_Preset_Knop == 0) {
        Serial.println("RGB Knop is ingedrukt.");
        RGBWaarde ++;
        if(RGBWaarde >= 4){
          RGBWaarde = 1;
        }
      }
      RGB_Preset_Knop = 1;
    }

  }
  if (PibuttonState == LOW) {  // Knop ingedrukt
    delay(300);
    if (PibuttonState == LOW) {
      if (PiPrint_Knop == 0) {
        Serial.println("PiKnop is ingedrukt.");
      }
      PiPrint_Knop = 1;
    }

  }
}

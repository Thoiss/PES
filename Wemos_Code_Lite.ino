#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>

const char* ssid = "pinetwerk";
const char* password = "bok12345";
WiFiServer server(8888);

#define MAX_CLIENTS 5                               // Aantal toegestane clients
#define Time_Delay 10000                            // 10 seconden timeout
#define SDA_PIN D2  // Standaard voor Wemos D1 mini (GPIO4)
#define SCL_PIN D1  // Standaard voor Wemos D1 mini (GPIO5)
#define LED_PIN D5                                  // LED op D5 (GPIO14)
#define RED_PIN D6   // Rood op D6
#define GREEN_PIN D7 // Groen op D7
#define BLUE_PIN D8  // Blauw op D8
void setRGBColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);   // Rood kanaal
  analogWrite(GREEN_PIN, green); // Groen kanaal
  analogWrite(BLUE_PIN, blue);  // Blauw kanaal
}


WiFiClient clients[MAX_CLIENTS];                    // Array om clients op te slaan
unsigned long lastActiveTime[MAX_CLIENTS] = { 0 };  // Houd de laatste activiteit per client bij

int Print_Knop = 0;
int PiPrint_Knop = 0;
Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup() {
  Serial.begin(115200);
  Serial.println("\nOpstarten...");
  Wire.begin(SDA_PIN, SCL_PIN);
  // Initialiseer de SHT3X sensor
  if (!sht31.begin()) {
    Serial.println("Sensor niet gevonden!");
    while (1);  // Stop de uitvoering als de sensor niet gevonden wordt
  }

  Serial.println("Sensor gevonden!");
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

  pinMode(LED_PIN, OUTPUT);           // LED als output
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  // Check voor nieuwe clients
  WiFiClient newClient = server.available();
          Serial.print("RGB");
          setRGBColor(150, 50, 50);
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
  float temperature = sht31.readTemperature();
  if (!isnan(temperature)) {
    // Print de temperatuur in de Serial Monitor
    Serial.print("Huidige temperatuur: ");
    Serial.print(temperature);
    Serial.println(" C");
  } else {
    Serial.println("Fout bij het uitlezen van de temperatuur.");
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


          lastActiveTime[i] = millis();  // Reset de timeout timer
          if (data == "Led") {
            digitalWrite(LED_PIN, HIGH);
            delay(1500);
            digitalWrite(LED_PIN, LOW);
          }
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



  // int buttonState = digitalRead(BUTTON_PIN);  // Lees de knopstatus voor de test tussen Wemos
  // int PibuttonState = digitalRead(PI_PIN);  // Lees de knopstatus voor de Pi
  // if (buttonState == LOW) {  // Knop ingedrukt
  //   delay(300);
  //   if (buttonState == LOW) {
  //     if (Print_Knop == 0) {
  //       Serial.println("Knop is ingedrukt.");
  //     }
  //     Print_Knop = 1;
  //   }

  // }
  // if (PibuttonState == LOW) {  // Knop ingedrukt
  //   delay(300);
  //   if (PibuttonState == LOW) {
  //     if (PiPrint_Knop == 0) {
  //       Serial.println("PiKnop is ingedrukt.");
  //     }
  //     PiPrint_Knop = 1;
  //   }

  // }
}

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>
#include <Ticker.h>

const char* ssid = "pinetwerk";
const char* password = "bok12345";
char Status[200] = "";
WiFiServer server(8888);
Ticker ledTimer;
#define MAX_CLIENTS 5     // Aantal toegestane clients
#define Time_Delay 10000  // 10 seconden timeout
#define SDA_PIN D2        // Standaard voor Wemos D1 mini (GPIO4)
#define SCL_PIN D1        // Standaard voor Wemos D1 mini (GPIO5)
#define LED_PIN D5        // LED op D5 (GPIO14)
#define RED_PIN D6        // Rood op D6
#define GREEN_PIN D7      // Groen op D7
#define BLUE_PIN D8       // Blauw op D8
void setRGBColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);      // Rood kanaal
  analogWrite(GREEN_PIN, green);  // Groen kanaal
  analogWrite(BLUE_PIN, blue);    // Blauw kanaal
}

void ledUitZetten() {
  digitalWrite(LED_PIN, LOW);
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
    while (1)
      ;  // Stop de uitvoering als de sensor niet gevonden wordt
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

  pinMode(LED_PIN, OUTPUT);  // LED als output
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  // Check voor nieuwe clients
  WiFiClient newClient = server.available();
  // Serial.print("RGB");
  // setRGBColor(0, 250, 0);
  if (newClient) {
    Serial.println("Nieuwe client probeert te verbinden...");
    bool added = false;

    // Toevoegen aan een lege plek in de array
    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (!clients[i]) {  // Als de slot leeg is (geen actieve client)
        clients[i] = newClient;
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
    // Serial.print("Huidige temperatuur: ");
    // Serial.print(temperature);
    // Serial.println(" C");
  } else {
    Serial.println("Fout bij het uitlezen van de temperatuur.");
  }

  // Check actieve clients
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i]) {  // Als er een actieve client is
      if (clients[i].connected()) {
        if (clients[i].available()) {
          String data = clients[i].readStringUntil('\n');

          data.trim();
          Serial.print("Ontvangen van client ");
          Serial.print(i);
          Serial.print(": ");
          Serial.println(data);
          // clients[i].print("Echo: " + data);  // Stuur bericht terug
          if (data == "wemosStatus") {
            memset(Status, 0, sizeof(Status));
            if (Print_Knop == 1) {
              Serial.print("Op data is knop uitgelezen");
              strcat(Status, " TRUE");
              Print_Knop = 0;
              Serial.println("True toegevoegd ");
            } else {
              strcat(Status, " FALSE");
            }
            if (PiPrint_Knop == 1) {
              Serial.print("Op data is knop uitgelezen");
              strcat(Status, " PiTRUE");
              Serial.println("PiTrue toegevoegd ");
              PiPrint_Knop = 0;
            } else {
              strcat(Status, " PiFALSE");
            }

            strcat(Status, " RGBFALSE");
            clients[i].print(Status);
          }


          if (data == "Led") {
            digitalWrite(LED_PIN, HIGH);
            ledTimer.once(1.5, ledUitZetten);  // Zet de LED uit na 1.5 seconden
            // delay(1500);
            // digitalWrite(LED_PIN, LOW);
          }
          if (data == "End") {
            clients[i].print("Verbinding wordt afgesloten door server");
            Serial.print("Client ");
            Serial.print(i);
            Serial.println(" is losgekoppeld.");
            clients[i].stop();          // Sluit client af
            clients[i] = WiFiClient();  // Maak de slot weer vrij
          }


          if (data.indexOf("RGBWAARDE") != -1) {  // Check of "RGBWAARDE" aanwezig is
            int dashIndex = data.indexOf(" - ");  // Zoek de positie van " - "

            if (dashIndex != -1) {                              // Controleer of " - " bestaat in de string
              String numString = data.substring(0, dashIndex);  // Pak het deel vóór " - "
              int preset = numString.toInt();                   // Zet om naar een integer

              Serial.print("Ontvangen preset: ");
              Serial.println(preset);

              // **Pas RGB-kleur aan op basis van preset**
              switch (preset) {
                case 0:
                  setRGBColor(0, 0, 0);  // LED uit
                  continue;
                case 1:
                  setRGBColor(255, 50, 50);  // Rood
                  continue;
                case 2:
                  setRGBColor(50, 255, 50);  // Groen
                  continue;
                case 3:
                  setRGBColor(50, 50, 255);  // Blauw
                  continue;
                default:
                  Serial.println("Ongeldige preset ontvangen!");
                  continue;
              }
            }
          }
        }
      }
    }
  }
}

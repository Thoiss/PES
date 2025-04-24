#include "globals.h"
#define deviceNaam "LITE"
void setRGBColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}
void ledUitZetten() {
  digitalWrite(LED_PIN, LOW);
}

void dataOntvangenVersturen() {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i] && clients[i].connected() && clients[i].available()) {
      String data = clients[i].readStringUntil('\n');
      data.trim();
      Serial.print("Ontvangen van client ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(data);

      // wemosStatus
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

      if (data == "Hello from client") {
        clients[i].print(deviceNaam);
      }
      // Led-aansturing
      if (data == "Led") {
        clients[i].print("LEDAAN");
        digitalWrite(LED_PIN, HIGH);
        ledTimer.once(1.5, ledUitZetten);
      }

      // Disconnect
      if (data == "End") {
        clients[i].print("Verbinding wordt afgesloten door server");
        Serial.print("Client ");
        Serial.print(i);
        Serial.println(" is losgekoppeld.");
        clients[i].stop();
        clients[i] = WiFiClient();
      }
      
      // RGBWAARDE preset
      if (data.indexOf("RGBWAARDE") != -1) {
        clients[i].print("RGBAAN");
        int dashIndex = data.indexOf(" - ");
        if (dashIndex != -1) {
          String numString = data.substring(0, dashIndex);
          int preset = numString.toInt();
          Serial.print("Ontvangen preset: ");
          Serial.println(preset);
          switch (preset) {
            case 0: setRGBColor(0, 0, 0); continue;
            case 1: setRGBColor(255, 50, 50); continue;
            case 2: setRGBColor(50, 255, 50); continue;
            case 3: setRGBColor(50, 50, 255); continue;
            default: Serial.println("Ongeldige preset ontvangen!"); continue;
          }
        }
      }
    }
  }
}

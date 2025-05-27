#include "globals.h"
#define deviceNaam " LITE"
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

      // Disconnect
      if (data == "End") {
        clients[i].print("Verbinding wordt afgesloten door server");
        Serial.print("Client ");
        Serial.print(i);
        Serial.println(" is losgekoppeld.");
        clients[i].stop();
        clients[i] = WiFiClient();
      }
      //RGBWAARDE
      else if (data.indexOf("RGBWAARDE") != -1) {
        int firstDash = data.indexOf(" - ");
        int secondDash = data.indexOf(" - ", firstDash + 3);  // begin na eerste dash

        if (firstDash != -1 && secondDash != -1) {
          String statusStr = data.substring(0, firstDash);
          String waardeStr = data.substring(firstDash + 3, secondDash);

          int status = statusStr.toInt();
          int waarde = waardeStr.toInt();

          Serial.print("Status verlichting: ");
          Serial.println(status);
          Serial.print("Verlichtingswaarde: ");
          Serial.println(waarde);

          if (status == 1) {
            setRGBColor(waarde, waarde, waarde);
          } else {
            Serial.println("Verlichting uit of genegeerd.");
            setRGBColor(0, 0, 0);
          }
        }
      }
    }
  }
}

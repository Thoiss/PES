#include "Wemos_Code_Lite_dataOntvangenVersturen.h"
#define deviceNaam " LITE"

void setRGB1Color(int red, int green, int blue) {
  analogWrite(RED_PIN_1, red);
  analogWrite(GREEN_PIN_1, green);
  analogWrite(BLUE_PIN_1, blue);
}

void setRGB2Color(int red, int green, int blue) {
  analogWrite(RED_PIN_2, red);
  analogWrite(GREEN_PIN_2, green);
  analogWrite(BLUE_PIN_2, blue);
}

void RGB1Aanzetten(){
  pinMode(RED_PIN_1, OUTPUT);
  pinMode(GREEN_PIN_1, OUTPUT);
  pinMode(BLUE_PIN_1, OUTPUT);
}

void RGB2Aanzetten(){
  pinMode(RED_PIN_2, OUTPUT);
  pinMode(GREEN_PIN_2, OUTPUT);
  pinMode(BLUE_PIN_2, OUTPUT);
}

void dataOntvangenVersturen(WiFiClient clients[]) {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i] && clients[i].connected() && clients[i].available()) {
      String data = clients[i].readStringUntil('\n');
      data.trim();
      Serial.print("Ontvangen van client ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(data);

      if (data == "1") {
        delay(5000);
        waarde = waarde / 100;
        if (waarde *90 < 5) {
            setRGB2Color(10, 100, 10);
            delay(1500);
            setRGB1Color(10, 100, 10);
            delay(1500);
        } else {
            setRGB2Color(waarde * 10, waarde * 90, waarde * 10);
            delay(1500);
            setRGB1Color(waarde * 10, waarde * 90, waarde * 10);
            delay(1500);
        }
      }
 
      if (data == "2") {
        delay(5000);
        Serial.println("Verlichting dimmen");
        waarde = waarde / 100;
        Serial.println(waarde);
        if (waarde *90 < 5) {

            setRGB2Color(100, 10, 10);
            delay(1500);
            setRGB1Color(100, 10, 10);
            delay(1500);
        } else {
            setRGB2Color(waarde * 90, waarde * 10, waarde * 10);
            delay(1500);
            setRGB1Color(waarde * 90, waarde * 10, waarde * 10);
            delay(1500);
        }
      }
      //RGBWAARDE
      else if (data.indexOf("RGBWAARDE") != -1) {
        int firstDash = data.indexOf(" - ");
        int secondDash = data.indexOf(" - ", firstDash + 3);  // begin na eerste dash

        if (firstDash != -1 && secondDash != -1) {
          String statusStr = data.substring(0, firstDash);
          String waardeStr = data.substring(firstDash + 3, secondDash);

          int status = statusStr.toInt();
          waarde = waardeStr.toInt();

          Serial.print("Status verlichting: ");
          Serial.println(status);
          Serial.print("Verlichtingswaarde: ");
          Serial.println(waarde);

          if (status == 1) {
            setRGB1Color(waarde, waarde, waarde);
            setRGB2Color(waarde, waarde, waarde);
          } else {
            Serial.println("Verlichting uit of genegeerd.");
            setRGB1Color(0, 0, 0);
            setRGB2Color(0, 0 ,0);
          }
        }
      }
    }
  }
}

#include "globals.h"
#include <Arduino.h>
#define deviceNaam "PRO"
void dataOntvangenVersturen() {
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

          if (data == "wemosStatus") {
            memset(Status, 0, sizeof(Status));
            if (Print_Knop == 1) {
              Serial.print("Op data is knop uitgelezen");
              strcat(Status, " TRUE");
            } else {
              strcat(Status, " FALSE");
            }
            if (PiPrint_Knop == 1) {
              Serial.print("Op data is knop uitgelezen");
              strcat(Status, " PiTRUE");
            } else {
              strcat(Status, " PiFALSE");
            }
            if (RGB_Preset_Knop == 1) {
              Serial.print("RGB Waarde binnengekregen \n");
              char RGBWaardeStr[10];
              sprintf(RGBWaardeStr, "%d", RGBWaarde);
              strcat(Status, " ");
              Serial.printf("RGB Waarde string: %s\n", RGBWaardeStr);
              strcat(Status, RGBWaardeStr);
              Serial.printf("Status: %s\n", Status);
            } else {
              strcat(Status, " RGBFALSE");
            }
            clients[i].print(Status);
          }
          if (data == "Hello from client") {
            clients[i].print(deviceNaam);
          }
          if (data == "LED_ACK") {
            Print_Knop = 0;
          }
          if (data == "PILED_ACK") {
            PiPrint_Knop = 0;
          }
          if (data == "RGB_ACK") {
            RGB_Preset_Knop = 0;
            ;
          }

          if (data == "End") {
            clients[i].print("Verbinding wordt afgesloten door server");
            Serial.print("Client ");
            Serial.print(i);
            Serial.println(" is losgekoppeld.");
            clients[i].stop();
            clients[i] = WiFiClient();
          }
        }
      }
    }
  }
}
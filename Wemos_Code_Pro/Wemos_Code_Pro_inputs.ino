#include "globals.h"
#include <Arduino.h>

void input_controle() {
  int buttonState = digitalRead(BUTTON_PIN);
  int PibuttonState = digitalRead(PI_PIN);
  int RGB_Preset_buttonState = digitalRead(RGB_Button);

  if (buttonState == LOW) {  // Knop ingedrukt
     delay(100);
    if (buttonState == LOW) {
      if (Print_Knop == 0) {
        Serial.println("Knop is ingedrukt.");
      }
      Print_Knop = 1;
    }
  }
  if (RGB_Preset_buttonState == LOW) {  // Knop ingedrukt
     delay(100);
    if (RGB_Preset_buttonState == LOW) {
      if (RGB_Preset_Knop == 0) {
        Serial.println("RGB Knop is ingedrukt.");
        RGBWaarde++;
        if (RGBWaarde >= 4) {
          RGBWaarde = 1;
        }
      }
      RGB_Preset_Knop = 1;
    }
  }
  if (PibuttonState == LOW) {  // Knop ingedrukt
     delay(100);
    if (PibuttonState == LOW) {
      if (PiPrint_Knop == 0) {
        Serial.println("PiKnop is ingedrukt.");
      }
      PiPrint_Knop = 1;
    }
  }
}
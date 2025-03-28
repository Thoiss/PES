#include "I2Cverbinding.h"
#include <stdio.h>
#include <stdlib.h>
#include <Wire.h>

#define BUTTON_PIN PA0 

void knopSetup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

}

int leesstatus() {  // Lees de status van de knop (de pin is LOW als de knop is ingedrukt)
    bool buttonState = digitalRead(BUTTON_PIN);

    if (buttonState == LOW) {
    delay(300);  // Debouncing voor de knop
    if (digitalRead(BUTTON_PIN) == LOW) {
      Slave1_Knop_Waarde = 1;
      Serial.println("Knop ingedrukt");
    }
  } else {
    Slave1_Knop_Waarde = 0;
  }

    return Slave1_Knop_Waarde;
}
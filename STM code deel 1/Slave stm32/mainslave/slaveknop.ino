#include "I2Cverbinding.h"
#include <stdio.h>
#include <stdlib.h>
#include <Wire.h>

#define BUTTON_PIN PA0
#define BUTTON_PIN2 PA4 

void knopSetup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
}

int leesstatus() {  // Lees de status van de knop (de pin is LOW als de knop is ingedrukt)
    bool buttonState1 = digitalRead(BUTTON_PIN);
    bool buttonState2 = digitalRead(BUTTON_PIN2);

    if (buttonState1 == LOW) {
        delay(150);  // Debouncing voor knop 1
        if (digitalRead(BUTTON_PIN) == LOW) {
            Slave1_Knop_Waarde = 1;
            Serial.println("Knop 1 ingedrukt");
            return Slave1_Knop_Waarde;
        }
    }

    if (buttonState2 == LOW) {
        delay(50);  // Debouncing voor knop 2
        if (digitalRead(BUTTON_PIN2) == LOW) {
            Slave1_Knop_Waarde = 3;
            Serial.println("Knop 2 ingedrukt");
            return Slave1_Knop_Waarde;
        }
    }

    // Geen van beide knoppen ingedrukt
    Slave1_Knop_Waarde = 0;
    return Slave1_Knop_Waarde;
}
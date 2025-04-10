#include <Wire.h>
#include "I2Cverbinding.h"

#define LED_PIN PA11


void led_Setup() {
  pinMode(LED_PIN, OUTPUT);
}

void led_Aansturen(int bytes){
  if (Wire.available()) {
        char command = Wire.read();  // Lees het ontvangen commando
        Serial.print("Ontvangen commando: ");
        Serial.println(command);

        if (command == '1') {
            Serial.println("LED AAN!");
            digitalWrite(LED_PIN, HIGH);
        } 
        
        else if (command == '0') {
            Serial.println("LED UIT!");
            digitalWrite(LED_PIN, LOW);
        }
  }
}
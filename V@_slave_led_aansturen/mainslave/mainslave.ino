#include <Wire.h>
#include "I2Cverbinding.h"
#include "slave_led_aansturen.h"

#define SLAVE_ADDRESS 0x13

void setup() {

  I2Csetup(SLAVE_ADDRESS);     //I2c verbinding initialiseren met de SDA op BP7 en de SCL op PB6, met als adres 0x11
  led_Setup();              //de knop initialiseren
  Wire.onReceive(led_Aansturen);    // Functie voor ontvangen data van de master
}

void loop() {
}

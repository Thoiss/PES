#include "I2Cverbinding.h"
#include "slave_led_aansturen.h"
#include <stdio.h>
#include <stdlib.h>
#include <Wire.h>

void I2Csetup(int adres) {

  Wire.setSDA(SDA);
  Wire.setSCL(SCL);
  Serial.begin(115200);
  Serial.println("STM32 Slave met led");
  Wire.begin(adres);
}

void requestData() {
  Wire.write(Slave1_Knop_Waarde);   // Stuur de knopstatus terug naar de master
}
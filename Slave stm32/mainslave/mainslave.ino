#include <Wire.h>
#include "I2Cverbinding.h"
#include "slaveknop.h"

#define adresslave 0x11

void setup() {

  I2Csetup(adresslave);     //I2c verbinding initialiseren met de SDA op BP7 en de SCL op PB6, met als adres 0x11
  knopSetup();              //de knop initialiseren
  Wire.onRequest(requestData);  //als er data wordt gevraagd data geven, Wire regelt dat na deze regel de slave reageerd op een master zonder dit in de while te hoeven loopen.
}

void loop() {
  Slave1_Knop_Waarde = leesstatus();
}

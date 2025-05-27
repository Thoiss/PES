#ifndef SHT3XSENSOR_H
#define SHT3XSENSOR_H
#include <cstddef>


#include "I2CDevice.h"
#include <cstdint>

class SHT3XSensor : public I2CDevice {
public:
    SHT3XSensor(int address = 0x2C);
    bool readTemperatureAndHumidity(float& temperature, float& humidity);

private:
    uint8_t calculateCRC(const uint8_t* data, size_t len);
};

#endif

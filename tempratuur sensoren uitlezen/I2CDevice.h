#ifndef I2CDEVICE_H
#define I2CDEVICE_H
#include <cstdint>
#include <cstddef>


class I2CDevice {
protected:
    int fd;
    int address;
    
public:
    I2CDevice(int address);
    virtual ~I2CDevice();

    virtual bool openDevice();
    virtual void closeDevice();

    bool writeBytes(const uint8_t* data, size_t length);
    bool readBytes(uint8_t* buffer, size_t length);
};

#endif

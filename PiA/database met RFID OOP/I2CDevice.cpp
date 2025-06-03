#include "I2CDevice.h"
#include <wiringPiI2C.h>
#include <unistd.h>   // for close()
#include <iostream>
#include <fcntl.h>



I2CDevice::I2CDevice(int addr) : address(addr), fd(-1) {}

I2CDevice::~I2CDevice() {
    closeDevice();
}

bool I2CDevice::openDevice() {
    fd = wiringPiI2CSetup(address);
    if (fd == -1) {
        std::cerr << "Failed to open I2C device with WiringPi\n";
        return false;
    }
    return true;
}

void I2CDevice::closeDevice() {
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}

bool I2CDevice::writeBytes(const uint8_t* data, size_t length) {
    if (write(fd, data, length) != static_cast<ssize_t>(length)) {
        perror("I2C write failed");
        return false;
    }
    return true;
}

bool I2CDevice::readBytes(uint8_t* buffer, size_t length) {
    if (read(fd, buffer, length) != static_cast<ssize_t>(length)) {
        perror("I2C read failed");
        return false;
    }
    return true;
}

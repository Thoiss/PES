#include "SHT3XSensor.h"
#include <unistd.h>   // for usleep()
#include <iostream>      
#include <fcntl.h>       // for open
#include <sys/ioctl.h>   // for ioctl
#include <linux/i2c-dev.h>
#include <cstring>
#include <cstdio>



#define CMD_MEASURE_HIGHREP 0x2400
#define POLYNOMIAL 0x31

SHT3XSensor::SHT3XSensor(int addr) : I2CDevice(addr) {}

bool SHT3XSensor::readTemperatureAndHumidity(float& temperature, float& humidity) {
    // Raw file descriptor from wiringPi
    char i2c_device[] = "/dev/i2c-1";
    int fd_raw = open(i2c_device, O_RDWR);
    if (fd_raw < 0) {
        perror("Unable to open I2C device");
        return false;
    }

    if (ioctl(fd_raw, I2C_SLAVE, this->address) < 0) {
        perror("Failed to acquire bus access");
        close(fd_raw);
        return false;
    }

    // Send measurement command (0x2400 in MSB first)
    uint8_t cmd[2] = {0x24, 0x00};
    if (write(fd_raw, cmd, 2) != 2) {
        perror("Failed to write measurement command");
        close(fd_raw);
        return false;
    }

    usleep(15000); // Wait for measurement

    uint8_t data[6];
    if (read(fd_raw, data, 6) != 6) {
        perror("Failed to read data");
        close(fd_raw);
        return false;
    }

    close(fd_raw);

    int temp_raw = (data[0] << 8) | data[1];
    int hum_raw  = (data[3] << 8) | data[4];

    temperature = -45 + 175 * ((float)temp_raw / 65535.0);
    humidity    = 100 * ((float)hum_raw / 65535.0);

    return true;
}

uint8_t SHT3XSensor::calculateCRC(const uint8_t* data, size_t len) {
    uint8_t crc = 0xFF;
    for (size_t j = 0; j < len; j++) {
        crc ^= data[j];
        for (int i = 0; i < 8; i++) {
            crc = (crc & 0x80) ? (crc << 1) ^ POLYNOMIAL : (crc << 1);
        }
    }
    return crc;
}

#include "SHT3XSensor.h"
#include <iostream>
#include <cstdint>

int main() {
    SHT3XSensor sensor(0x44);
    SHT3XSensor sensor2(0x45);

    float temperature = 0.0f, humidity = 0.0f;

    std::cout << "Opening sensor 1 (0x44)...\n";
    if (!sensor.openDevice()) {
        std::cerr << "Failed to open sensor 1\n";
        return 1;
    }

    if (sensor.readTemperatureAndHumidity(temperature, humidity)) {
        std::cout << "Sensor 1 - Temperature: " << temperature << " °C\n";
        std::cout << "Sensor 1 - Humidity: " << humidity << " %\n";
    } else {
        std::cerr << "Failed to read from sensor 1\n";
    }
    sensor.closeDevice();

    std::cout << "Opening sensor 2 (0x45)...\n";
    if (!sensor2.openDevice()) {
        std::cerr << "Failed to open sensor 2\n";
        return 1;
    }

    if (sensor2.readTemperatureAndHumidity(temperature, humidity)) {
        std::cout << "Sensor 2 - Temperature: " << temperature << " °C\n";
        std::cout << "Sensor 2 - Humidity: " << humidity << " %\n";
    } else {
        std::cerr << "Failed to read from sensor 2\n";
    }
    sensor2.closeDevice();

    return 0;
}

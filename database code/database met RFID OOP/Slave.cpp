#include "Slave.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
#include <unistd.h>

Slave::Slave(int address) : address(address), fd(-1) {}

bool Slave::init() {
    if (wiringPiSetup() == -1) {
        std::cerr << "Fout bij het instellen van wiringPi\n";
        return false;
    }

    fd = wiringPiI2CSetup(address);
    if (fd == -1) {
        std::cerr << "Fout bij het openen van de slave op adres: " << address << "\n";
        return false;
    }

    return true;
}

void Slave::schrijfCommando(int commando) {
    if (fd != -1) {
        wiringPiI2CWrite(fd, commando);
    }
}

int Slave::leesKaart() {
    if (fd != -1) {
        return wiringPiI2CRead(fd);
    }
    return -1; // foutcode
}


int Slave::leesTerminal() {
    if (fd != -1) {
        return wiringPiI2CRead(fd);
    }
    return -1; 
}

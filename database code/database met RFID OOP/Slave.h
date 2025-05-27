#ifndef SLAVE_H
#define SLAVE_H

class Slave {
private:
    int fd;               // File descriptor voor de I2C verbinding
    int address;          // I2C adres van de slave

public:
    Slave(int address);

    bool init();
    int leesKaart();
    int leesTerminal();
    int leesTerminal2(char* buffer, int maxLen);
    void schrijfCommando(int commando);
};

#endif
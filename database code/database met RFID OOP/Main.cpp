//g++ main.cpp Slave.cpp Database.cpp TCPServer.cpp -o main -lwiringPi -lmariadb

#include "TCPServer.h"
#include <iostream>
#include <unistd.h>
#include <string.h>

int main() {
    Database db("localhost", "julian", "Root", "rfid_db");
    Slave s1(0x0f);
    Slave s2(0x11);

    s1.init();  //init van slave rfid
    s2.init();  //init van slave2
    db.init();  //init van database

    TCPServer server(8080, s1, db, s2);
    server.run();
    return 0;
    }
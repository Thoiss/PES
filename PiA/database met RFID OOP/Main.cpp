//g++ *.cpp -o main -lwiringPi -lmariadb

#include "TCPServer.h"
#include <iostream>
#include <unistd.h>
#include <string.h>

int main() {
    Database db("localhost", "julian", "Root", "rfid_db");
    Slave s1(0x0f);
    Slave s2(0x11);
    Slave s3(0x14);
    Slave s4(0x43);

    s1.init();  //init van slave rfid
    s2.init();  //init van slave2
    s3.init();  //init van slave Encoder
    db.init();  //init van database
    s4.init();  //init van servo

    TCPServer server(8080, s1, db, s2, s3, s4);
    server.run();
    return 0;
    }
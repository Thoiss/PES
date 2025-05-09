//++ main.cpp Slave.cpp Database.cpp -o DBMASTER -lwiringPi -lmariadb

#include "Database.h"
#include "Slave.h"
#include <iostream>
#include <unistd.h>

int main() {
    Database db("localhost", "julian", "Root", "rfid_db");
    Slave s1(0x0f);

    s1.init();  //init van slave
    db.init();  //init van database

    while(1){
        s1.schrijfCommando(1);              //commando 1 sturen
        int kaart = s1.leesKaart();         //data van de slave verkrijgen

        if (kaart != 48 && kaart != -1) {   //kijken of het daadwerkelijk data van een nieuwe kaart is
            std::cout << "Kaart = " << kaart << std::endl;  //uitprinten welke kaart is gevonden
            std::string data = std::to_string(kaart);      //kaart omzetten naar string zodat het in de db gezet kan worden
            db.schrijven(data, "raspberry2");                 //naar de db schrijven
        }

        sleep(1);
    }

    return 0;
    }
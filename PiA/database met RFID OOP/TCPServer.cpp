#include "TCPServer.h"
#include "Slave.h"
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <cstring>
#include <iostream>
#include <algorithm>
#include "SHT3XSensor.h"
#include "I2CDevice.h"

TCPServer::TCPServer(int port, Slave& s1, Database& db, Slave &s2, Slave &s3, Slave &s4)
    : port_(port), server_fd_(-1), s1(s1), db(db), s2(s2), s3(s3), s4(s4){}

void TCPServer::setupSocket() {
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    if (bind(server_fd_, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd_, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if (fcntl(server_fd_, F_SETFL, O_NONBLOCK) < 0) {
        perror("fcntl failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server luistert op poort " << port_ << "...\n";
}

void TCPServer::run() {
    setupSocket();

    // Temperatuursensoren initialiseren
    SHT3XSensor insideSensor(0x44);   // Binnen
    SHT3XSensor outsideSensor(0x45);  // Buiten
    bool deurAlOpen = false;

    if (!insideSensor.openDevice() || !outsideSensor.openDevice()) {
        std::cerr << "Kon sensoren niet openen\n";
        return;
    }

    while (true) {
        // Lees temperatuur
        float tempInside = 0.0f, humInside = 0.0f;
        float tempOutside = 0.0f, humOutside = 0.0f;

        bool insideRead = insideSensor.readTemperatureAndHumidity(tempInside, humInside);
        bool outsideRead = outsideSensor.readTemperatureAndHumidity(tempOutside, humOutside);

        if (insideRead && outsideRead) {
    std::cout << "Binnen: " << tempInside << " °C, Buiten: " << tempOutside << " °C\n";

    if (insideRead && outsideRead) {
        std::cout << "Binnen: " << tempInside << " °C, Buiten: " << tempOutside << " °C\n";

        if (tempInside > 26.0f && tempOutside < tempInside && !deurAlOpen) {
            schrijfNaarDeurTempAan();
            deurAlOpen = true;
            std::cout << "Deur open vanwege temperatuur\n";
        }
        else if (tempInside < 25.0f && deurAlOpen) {
            schrijfNaarDeurUit_Temp();
            deurAlOpen = false;
            std::cout << "Deur dicht vanwege temperatuur\n";
        }
    }
}

        // Uitvoering van bestaande logica
        // schrijfNaarDeurUit();     // stuur deur dicht commando
        verwerkKaart();           // verwerk RFID kaart
        waardeVerlichting();      // lees helderheid
        standVerlichting();       // lees status verlichting
        standservo();             // lees status servo

        int personen = db.tellerUniekePersonen();
        std::cout << "personen: " << personen << "\n";

        // TCP socket handling
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(server_fd_, &readfds);
        int max_sd = server_fd_;

        for (int client : client_sockets_) {
            if (client > 0) {
                FD_SET(client, &readfds);
                if (client > max_sd) max_sd = client;
            }
        }

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000; // 100ms select timeout

        int activity = select(max_sd + 1, &readfds, nullptr, nullptr, &timeout);

        if ((activity < 0) && (errno != EINTR)) {
            std::cerr << "Select error\n";
        }

        if (FD_ISSET(server_fd_, &readfds)) {
            acceptNewClient();
        }

        handleClientActivity(readfds);
    }

    // Sensoren afsluiten (alleen bereikt bij break/exit, anders oneindige loop)
    insideSensor.closeDevice();
    outsideSensor.closeDevice();
}


void TCPServer::acceptNewClient() {
    sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int new_socket = accept(server_fd_, (struct sockaddr*)&address, &addrlen);

    if (new_socket >= 0) {
        fcntl(new_socket, F_SETFL, O_NONBLOCK);
        client_sockets_.push_back(new_socket);
  //      std::cout << "Nieuwe client verbonden!\n";
    }
}

void TCPServer::handleClientActivity(fd_set& readfds) {
    char buffer[1024];

    for (auto it = client_sockets_.begin(); it != client_sockets_.end();) {
        int sd = *it;

        if (FD_ISSET(sd, &readfds)) {
            int valread = read(sd, buffer, sizeof(buffer) - 1);
            if (valread <= 0) {
          //      std::cout << "Client " << sd << " heeft de verbinding verbroken\n";
                close(sd);
                it = client_sockets_.erase(it);
                continue;
            }

            buffer[valread] = '\0';
            std::string message(buffer);
        //    std::cout << "Client ontvangen: " << message << "\n";
            handleClientMessage(sd, message);
        }

        ++it;
    }
}

void TCPServer::handleClientMessage(int client_fd, const std::string& message) {
    if (message == "routeVerlichting") {
        std::cout << "Routeverlichting ontvangen\n";
        std::string statusRouteStr = std::to_string(routeVerlichting);
        sendMessage(client_fd, statusRouteStr);
        routeVerlichting = 0;
        }
    else if (message == "STMEncoder") {
        std::string encoderVerlichting = 
        std::to_string(statusverlichting) + "-" + 
        std::to_string(verlichtingwaarde);
        std::cout << encoderVerlichting<< "\n";
        sendMessage(client_fd, encoderVerlichting);
        statusverlichting =0;
        }
    else {
        sendMessage(client_fd, "niet correct ontvangen");
    }
}

void TCPServer::sendMessage(int client_fd, const std::string& message) {
    send(client_fd, message.c_str(), message.length(), 0);
}
void TCPServer::waardeVerlichting(){
   s3.schrijfCommando(1);
    char buf[16];
verlichtingwaarde = s3.leesTerminal();
 //   std::cout << "Verlichting helderheid: " << verlichtingwaarde<< std::endl;
//   sleep(1);
  
}
void TCPServer::standVerlichting(){
    s3.schrijfCommando(2);
    char buf[16];
   // if(statusverlichting ==0){
    statusverlichting = s3.leesTerminal();
    //}
    std::cout << "Stand verlichting helderheid: " << statusverlichting<< std::endl;
    // sleep(1);
}
void TCPServer::standservo(){
    s3.schrijfCommando(3);
    char buf[16];
    statusservo = s3.leesTerminal();
 //   std::cout << "Stand servo: " << statusservo<< std::endl;
    // sleep(1);
    if (statusservo == 1) {
        schrijfNaarDeur_RFID();
    }
    s3.schrijfCommando(4);
}
void TCPServer::verwerkKaart() {
    s1.schrijfCommando(1);  // commando 1 om kaart te lezen
    int pasID = s1.leesKaart();

    if (pasID != 0 && pasID != -1) {
        std::string data = std::to_string(pasID);
        std::cout << "Kaart = " << data << std::endl;
     //   schrijfNaarDeur();

        if (db.bestaatRfid(data)) {
            db.verwijderRfid(data);
     //       std::cout << "UID verwijderd uit DB.\n";
        } else {
            std::string persoon = db.checkGebruiker(data);
            routeVerlichting = db.checkLichtStatus(data);
            if (persoon != "guest") {
            schrijfNaarDeur_RFID();
            //naam naar lichtkrant sturen
            db.schrijvenrfid(data, persoon);
            }
     //       std::cout << "UID toegevoegd aan DB.\n";
        }
        //naar de I2C commando sturen voor openen deur
    }
}

void TCPServer::schrijfNaarDeur_RFID(){
    s4.schrijfCommando(1);
  //  std::cout << "deur is open\n";
}
void TCPServer::schrijfNaarDeurUit_NOOD(){
    s4.schrijfCommando(0);
  //  std::cout << "deur is dicht\n";
}
void TCPServer::schrijfNaarDeurTempAan(){
    s4.schrijfCommando(2);
}
void TCPServer::schrijfNaarDeurUit_Temp(){
    s4.schrijfCommando(3);
}
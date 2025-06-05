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

TCPServer::TCPServer(int port, Slave& s1, Database& db, Slave& s2, Slave& s3, Slave& s4)
    : port_(port), server_fd_(-1), s1(s1), db(db), s2(s2), s3(s3), s4(s4),
      tempInside_(0.0f), tempOutside_(0.0f), humInside_(0.0f), humOutside_(0.0f) {}

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

    SHT3XSensor insideSensor(0x44);
    SHT3XSensor outsideSensor(0x45);
    bool deurAlOpen = false;

    if (!insideSensor.openDevice() || !outsideSensor.openDevice()) {
        std::cerr << "Kon sensoren niet openen\n";
        return;
    }

    while (true) {
        bool insideRead = insideSensor.readTemperatureAndHumidity(tempInside_, humInside_);
        bool outsideRead = outsideSensor.readTemperatureAndHumidity(tempOutside_, humOutside_);

        if (insideRead && outsideRead) {
            std::cout << "Binnen: " << tempInside_ << " °C, Buiten: " << tempOutside_ << " °C\n";

            if (tempInside_ > 27.0f && tempOutside_ < tempInside_ && !deurAlOpen) {//dit was 26 f
                schrijfNaarDeurTempAan();
                deurAlOpen = true;
                std::cout << "Deur open vanwege temperatuur\n";
            }
            else if (tempInside_ < 26.0f && deurAlOpen) {//dit was 25 f
                schrijfNaarDeurUit_Temp();
                deurAlOpen = false;
                std::cout << "Deur dicht vanwege temperatuur\n";
            }
        }

        verwerkKaart();
        waardeVerlichting();
        standVerlichting();
        standservo();
        opvragen_reset_noodknop_status();
        // lampaansturen();

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
        timeout.tv_usec = 100000;

        int activity = select(max_sd + 1, &readfds, nullptr, nullptr, &timeout);

        if ((activity < 0) && (errno != EINTR)) {
            std::cerr << "Select error\n";
        }

        if (FD_ISSET(server_fd_, &readfds)) {
            acceptNewClient();
        }

        handleClientActivity(readfds);
    }

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
    }
}

void TCPServer::handleClientActivity(fd_set& readfds) {
    char buffer[1024];

    for (auto it = client_sockets_.begin(); it != client_sockets_.end();) {
        int sd = *it;

        if (FD_ISSET(sd, &readfds)) {
            int valread = read(sd, buffer, sizeof(buffer) - 1);
            if (valread <= 0) {
                close(sd);
                it = client_sockets_.erase(it);
                continue;
            }

            buffer[valread] = '\0';
            std::string message(buffer);
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
        std::cout << encoderVerlichting << "\n";
        sendMessage(client_fd, encoderVerlichting);
        statusverlichting = 0;
         std::cout << "STM Encoder data verstuurd" << encoderVerlichting << std::endl;
    }
    else if (message == "temperatuur") {
        std::string tempStr =
            "Binnen: " + std::to_string(tempInside_) + " °C, " +
            std::to_string(humInside_) + " % RV | " +
            "Buiten: " + std::to_string(tempOutside_) + " °C, " +
            std::to_string(humOutside_) + " % RV";
        sendMessage(client_fd, tempStr);
    }
    else if (message == "persoon") {
        int personen = db.tellerUniekePersonen();
        std::cout << "personen: " << personen << "\n";

        std::string tempPers = std::to_string(personen);
        sendMessage(client_fd, tempPers);
    }
    else {
        sendMessage(client_fd, "niet correct ontvangen");
        std::cout << "Niet correct ontvangen verstuurd" << std::endl;
    }
}

void TCPServer::sendMessage(int client_fd, const std::string& message) {
    send(client_fd, message.c_str(), message.length(), 0);
}

void TCPServer::waardeVerlichting() {
    s3.schrijfCommando(1);
    verlichtingwaarde = s3.leesTerminal();
}

void TCPServer::standVerlichting() {
    if (statusverlichting == 0 && verlichtinglezen == false) {
    s3.schrijfCommando(2);
    statusverlichting = s3.leesTerminal();
    }
    if (statusverlichting ==1) {
        verlichtinglezen = true;
    } 
    if (verlichtinglezen == true){
        verlichtinglezenstatus ++;
        if (verlichtinglezenstatus >= 10) {
            verlichtinglezen = false;
            verlichtinglezenstatus = 0;
    }
}
    std::cout << "Stand verlichting helderheid: " << statusverlichting << std::endl;
}

void TCPServer::opvragen_reset_noodknop_status() {
    s3.schrijfCommando(5);
    reset_noodknop_status = s3.leesTerminal();
    if (reset_noodknop_status == 1){
        reset_noodknop();
    }
    std::cout << "status resetknop:  " << reset_noodknop_status << std::endl;
}

void TCPServer::standservo() {
    s3.schrijfCommando(3);
    statusservo = s3.leesTerminal();
    if (statusservo == 1) {
        schrijfNaarDeur_RFID();
    }
    s3.schrijfCommando(4);
}

void TCPServer::verwerkKaart() {
    s1.schrijfCommando(1);
    int pasID = s1.leesKaart();

    if (pasID != 0 && pasID != -1) {
        std::string data = std::to_string(pasID);
        std::cout << "Kaart = " << data << std::endl;

        if (db.bestaatRfid(data)) {
            db.verwijderRfid(data);
        } else {
            std::string persoon = db.checkGebruiker(data);
            if (persoon != "guest") {
                schrijfNaarDeur_RFID();
                db.schrijvenrfid(data, persoon);
                routeVerlichting = db.checkLichtStatus(data);
            }
        }
    }
}

void TCPServer::schrijfNaarDeur_RFID() {
    s4.schrijfCommando(1);
}

void TCPServer::schrijfNaarDeurUit_NOOD() {
    s4.schrijfCommando(0);
}

void TCPServer::schrijfNaarDeurTempAan() {
    s4.schrijfCommando(2);
}

void TCPServer::schrijfNaarDeurUit_Temp() {
    s4.schrijfCommando(3);
}

void TCPServer::reset_noodknop() {
    s4.schrijfCommando(4);
}

void TCPServer::lampaansturen(){
    noodknop_status = 0;
    s4.schrijfCommando(5);
    noodknop_status = s4.leesTerminal();
    if (noodknop_status == 1){
        s3.schrijfCommando(6);
    }
}
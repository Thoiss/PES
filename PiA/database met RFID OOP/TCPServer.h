#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <vector>
#include <string>
#include "Slave.h"
#include "Database.h"

class TCPServer {
public:
    TCPServer(int port, Slave& s1, Database& db, Slave& s2, Slave& s3, Slave& s4);
    void run();

private:
    int routeVerlichting = 0;
    int port_;
    int server_fd_;
    int statusverlichting = 0;
    int verlichtingwaarde = 0;
    int statusservo = 0;
    int reset_noodknop_status;
    int noodknop_status;
    bool verlichtinglezen = false;
    int verlichtinglezenstatus = 0;
    std::vector<int> client_sockets_;
    Slave& s1;
    Slave& s2;
    Slave& s3;
    Slave& s4;
    Database& db;

    void setupSocket();
    void acceptNewClient();
    void handleClientActivity(fd_set& readfds);
    void handleClientMessage(int client_fd, const std::string& message);
    void sendMessage(int client_fd, const std::string& message);
    void verwerkKaart();
    void waardeVerlichting();
    void standVerlichting();
    void standservo();
    void schrijfNaarDeur_RFID();
    void schrijfNaarDeurUit_NOOD(); 
    void schrijfNaarDeurTempAan();
    void schrijfNaarDeurUit_Temp();
    void reset_noodknop(); 
    void opvragen_reset_noodknop_status();
    void lampaansturen();
    float tempInside_, tempOutside_, humInside_, humOutside_;
};

#endif
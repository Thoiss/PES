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
    int port_;
    int server_fd_;
    int statusverlichting = 0;
    int verlichtingwaarde = 0;
    int statusservo = 0;
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
    void schrijfNaarDeuraan();
    void schrijfNaarDeurUit_NOOD(); 
    void schrijfNaarDeurTempAan();
    void schrijfNaarDeurUit_Sluis(); 
};

#endif
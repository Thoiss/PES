#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <vector>
#include <string>
#include "Slave.h"
#include "Database.h"

class TCPServer {
public:
    TCPServer(int port, Slave& s1, Database& db, Slave& s2);
    void run();

private:
    int port_;
    int server_fd_;
    std::vector<int> client_sockets_;
    Slave& s1;
    Slave& s2;
    Database& db;

    void setupSocket();
    void acceptNewClient();
    void handleClientActivity(fd_set& readfds);
    void handleClientMessage(int client_fd, const std::string& message);
    void sendMessage(int client_fd, const std::string& message);
    void verwerkKaart();
};

#endif
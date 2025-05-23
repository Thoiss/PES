#include "TCPServer.h"
#include "Slave.h"
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <cstring>
#include <iostream>
#include <algorithm>

TCPServer::TCPServer(int port, Slave& s1, Database& db, Slave &s2)
    : port_(port), server_fd_(-1), s1(s1), db(db), s2(s2) {}

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

    while (true) {
        verwerkKaart();
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
        timeout.tv_usec = 100000; // 100ms

        int activity = select(max_sd + 1, &readfds, nullptr, nullptr, &timeout);

        if ((activity < 0) && (errno != EINTR)) {
            std::cerr << "Select error\n";
        }

        if (FD_ISSET(server_fd_, &readfds)) {
            acceptNewClient();
        }

        handleClientActivity(readfds);
    }
}

void TCPServer::acceptNewClient() {
    sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int new_socket = accept(server_fd_, (struct sockaddr*)&address, &addrlen);

    if (new_socket >= 0) {
        fcntl(new_socket, F_SETFL, O_NONBLOCK);
        client_sockets_.push_back(new_socket);
        std::cout << "Nieuwe client verbonden!\n";
    }
}

void TCPServer::handleClientActivity(fd_set& readfds) {
    char buffer[1024];

    for (auto it = client_sockets_.begin(); it != client_sockets_.end();) {
        int sd = *it;

        if (FD_ISSET(sd, &readfds)) {
            int valread = read(sd, buffer, sizeof(buffer) - 1);
            if (valread <= 0) {
                std::cout << "Client " << sd << " heeft de verbinding verbroken\n";
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
    if (message == "LED") {
        s2.schrijfCommando('1');
        sleep(1);
        s2.schrijfCommando('0');
    }
    else if (message == "Status") {
        int status = s2.leesKaart();
        if (status == 0) sendMessage(client_fd, "uit");
        else if (status == 1) sendMessage(client_fd, "aan");
        else if (status == 3) {
            sendMessage(client_fd, "trigger");
            s2.schrijfCommando('1');
            sleep(1);
            s1.schrijfCommando('0');
        }
    } else {
        sendMessage(client_fd, "niet correct ontvangen");
    }
}

void TCPServer::sendMessage(int client_fd, const std::string& message) {
    send(client_fd, message.c_str(), message.length(), 0);
}

void TCPServer::verwerkKaart() {
    s1.schrijfCommando(1);  // commando 1 om kaart te lezen
    int pasID = s1.leesKaart();

    if (pasID != 48 && pasID != -1) {
        std::string data = std::to_string(pasID);
        std::cout << "Kaart = " << data << std::endl;

        if (db.bestaatRfid(data)) {
            db.verwijderRfid(data);
            std::cout << "UID verwijderd uit DB.\n";
        } else {
            db.schrijvenrfid(data, "raspberry2");
            std::cout << "UID toegevoegd aan DB.\n";
        }
        //naar de I2C commando sturen voor openen deur
    }
    sleep(1);
}

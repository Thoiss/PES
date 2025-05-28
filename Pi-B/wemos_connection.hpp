#ifndef WEMOS_CONNECTION_HPP
#define WEMOS_CONNECTION_HPP
#define PORT      8888   // Wemos port

#include "wemos_connection.hpp"
#include <arpa/inet.h>   // Voor inet_pton()
#include <netinet/in.h>  // Voor sockaddr_in
#include <sys/socket.h>  // Voor socket(), connect()
#include <unistd.h>      // Voor read(), close()



class WemosConnection {
public:

    int connectToWemos(const char* ip);

private:
struct sockaddr_in serv_addr;
};
#endif // WEMOS_CONNECTION_HPP
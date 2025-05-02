#ifndef PI_CONNECTION_HPP
#define PI_CONNECTION_HPP

#include "globals.h"
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <cstdio>
#include <cstring>

class PiConnection {
public:

    int connectToPi(const char* ip, int port);


    int handlePiConnection();
};
#endif
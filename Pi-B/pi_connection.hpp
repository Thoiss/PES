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
    int verlichtingwaarde();
    int handlePiConnection();
private:
    int pi_a_socket = 0;  
    int oudVerlichtingswaarde = 0;  
    int verlichtingsWaarde = 0;
    bool statusVerlichting = false;
    int statusServo = 0;
    char *token = nullptr;
};
#endif
#ifndef PI_CONNECTION_HPP
#define PI_CONNECTION_HPP


#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#define TIMEOUT   3      // Timeout in seconden

class PiConnection {
public:
    int connectToPi(const char* ip, int port);
    int encoderVerlichting(int socket[]);
    int routeVerlichting(int socket[]);
    int ontvangTemperatuurData(char* outputBuffer, size_t bufSize);
    int ontvangPersoonData(char* outputBuffer, size_t bufSize);
private:
    int pi_a_socket = 0;  
    int statusServo = 0;
    char *token = nullptr;
    char buffer[1024] = {0};
    bool Versturen     = false;
    int statusVerlichting =0;
    int verlichtingsWaarde = 0;
    int oudVerlichtingswaarde = 0;  
    int statusRouteVerlichting = 0;
     
};
#endif
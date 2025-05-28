#ifndef WEMOS_AANSTUREN_HPP
#define WEMOS_AANSTUREN_HPP


#include <unistd.h>
#include <cstring>
#include <cstdio>
#include "pi_connection.hpp"

class WemosAansturen {
public:
    WemosAansturen();

    int stuurWemosAan( int socket, int deviceIndex, int statusVerlichting, int verlichtingsWaarde, bool Versturen);
    int routeWemos(int socket, int deviceIndex, int statusRouteVerlichting, bool Versturen);

private:
    int tellerdevices;
    char versturenbuffer[1024];
    char RGBSend[30];
    char routeSend[30];
};

#endif // WEMOS_AANSTUREN_HPP

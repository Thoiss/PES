#ifndef WEMOS_AANSTUREN_HPP
#define WEMOS_AANSTUREN_HPP

#include "globals.h"
#include <unistd.h>
#include <cstring>
#include <cstdio>

class WemosAansturen {
public:
    WemosAansturen();

    int stuurWemosAan(char* deviceNaam, int socket);

private:
    int tellerdevices;
    char versturenbuffer[1024];
};

#endif // WEMOS_AANSTUREN_HPP

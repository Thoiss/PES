#ifndef STATUS_HANDLER_HPP
#define STATUS_HANDLER_HPP

#include "globals.h"      // bevat o.a. extern char buffer[], Waarde_Knop, Versturen, etc.
#include <unistd.h>       // voor read/send
#include <cstring>        // voor strlen, strcmp, memset
#include <cstdio>         // voor printf
#include "wemos_aansturen.hpp"

class StatusHandler {
public:
    StatusHandler(int socket, WemosAansturen& aansturen);


private:
    int socket_;
    WemosAansturen& wemosAansturen_; 
};

#endif

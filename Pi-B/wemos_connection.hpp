#ifndef WEMOS_CONNECTION_HPP
#define WEMOS_CONNECTION_HPP

#include "globals.h"


class WemosConnection {
public:

    int connectToWemos(const char* ip);
};

#endif // WEMOS_CONNECTION_HPP
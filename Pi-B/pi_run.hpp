#ifndef PI_RUN_HPP
#define PI_RUN_HPP

#include "wemos_connection.hpp"
#include "pi_connection.hpp"
#include "wemos_aansturen.hpp"
#define PI_A_PORT 8080 // Pi-A port
#define Devices 3      // Aantal Wemos apparaten

class piRun
{
public:
    int Loop();

private:
    const char *pi_a_ip = "192.168.137.46"; // Raspberry Pi A
    int Socket[Devices] = {0};
    int Error = 0;
    int piSocket = -1;
    const char *wemos_ips[Devices] = {
        "192.168.137.20", // Wemos 1 (Pro - inputs)
        "192.168.137.30", // Wemos 2 (Lite - lampjes)
        "192.168.137.40"  // Wemos 3 (Lite - lichtkrant)
    };
};

#endif
#ifndef PI_CONNECTION_H
#define PI_CONNECTION_H

#include "globals.h"

// Verbind met Pi-A
int connect_to_pi(const char *ip, int port);

// Stuur status/LED-bericht naar Pi-A en verwerk antwoord
int Pi_Connectie();

#endif // PI_CONNECTION_H

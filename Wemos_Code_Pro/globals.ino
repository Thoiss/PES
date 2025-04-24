#include "globals.h"

WiFiServer server(8888);
char Status[200] = "";
WiFiClient clients[MAX_CLIENTS];
unsigned long lastActiveTime[MAX_CLIENTS] = { 0 };

int Print_Knop = 0;
int PiPrint_Knop = 0;
int RGB_Preset_Knop = 0;
int RGBWaarde = 0;
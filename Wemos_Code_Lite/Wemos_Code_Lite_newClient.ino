#include "globals.h"

void newClient() {
  WiFiClient nc = server.available();
  if (nc) {
    Serial.println("Nieuwe client probeert te verbinden...");
    bool added = false;
    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (!clients[i]) {
        clients[i] = nc;
        Serial.print("Client verbonden op slot: ");
        Serial.println(i);
        added = true;
        break;
      }
    }
    if (!added) {
      Serial.println("Server vol! Nieuwe client wordt geweigerd.");
      nc.stop();
    }
  }
}

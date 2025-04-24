#include "globals.h"
#include <Arduino.h>

void newClient() {
  // Check voor nieuwe clients
  WiFiClient newClient = server.available();
  if (newClient) {
    Serial.println("Nieuwe client probeert te verbinden...");
    bool added = false;

    // Toevoegen aan een lege plek in de array
    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (!clients[i]) {  // Als de slot leeg is (geen actieve client)
        clients[i] = newClient;
        lastActiveTime[i] = millis();  // Start timer voor deze client
        Serial.print("Client verbonden op slot: ");
        Serial.println(i);
        added = true;
        break;
      }
    }

    if (!added) {
      Serial.println("Server vol! Nieuwe client wordt geweigerd.");
      newClient.stop();  // Sluit verbinding af als er geen plek is
    }
  }
}
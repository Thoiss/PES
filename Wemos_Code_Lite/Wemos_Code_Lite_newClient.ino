

void newClient(WiFiServer &server, WiFiClient clients[]) {
  //check voor nieuwe clients
  WiFiClient nc = server.available();
  if (nc) {
    Serial.println("Nieuwe client probeert te verbinden...");
    bool added = false;
    // Toevoegen aan een lege plek in de array
    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (!clients[i]) { //als de slot leeg is (geen actieve client)
        clients[i] = nc;
        Serial.print("Client verbonden op slot: ");
        Serial.println(i);
        added = true;
        break;
      }
    }
    if (!added) {
      Serial.println("Server vol! Nieuwe client wordt geweigerd.");
      nc.stop(); //Sluit verbinding af als er geen plek is
    }
  }
}


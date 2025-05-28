#include "LichtkrantServer.h"

LichtkrantServer::LichtkrantServer(WiFiServer& srv, MD_Parola& mat, String& text)
  : server(srv), matrix(mat), scrollText(text) {
  for (int i = 0; i < MAX_CLIENTS; ++i) {
    clients[i] = WiFiClient();
    lastActiveTime[i] = 0;
  }
}

void LichtkrantServer::start() {
  // Matrix animation
  if (matrix.displayAnimate()) {
    matrix.displayReset();
  }

  checkNewClients();
  handleExistingClients();
}

void LichtkrantServer::checkNewClients() {
  WiFiClient newClient = server.available();
  if (newClient) {
    bool added = false;
    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (!clients[i]) {
        clients[i] = newClient;
        lastActiveTime[i] = millis();
        Serial.printf("Client verbonden op slot: %d\n", i);
        added = true;
        break;
      }
    }
    if (!added) {
      Serial.println("Server vol! Nieuwe client wordt geweigerd.");
      newClient.stop();
    }
  }
}

void LichtkrantServer::handleExistingClients() {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i]) {
      if (clients[i].connected()) {
        if (clients[i].available()) {
          String data = clients[i].readStringUntil('\n');
          data.trim();
          Serial.printf("Ontvangen van client %d: %s\n", i, data.c_str());

          if (data.startsWith("text:")) {
            scrollText = data.substring(5);
            Serial.println("Nieuw tekst voor matrix: " + scrollText);
            matrix.displayClear();
            matrix.displayText(scrollText.c_str(), PA_CENTER, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
            clients[i].println("Display updated.");
          }
          else if (data == "wemosStatus") {
            clients[i].println("NO_BUTTON_STATUS");
          }
          else if (data == "End") {
            clients[i].println("Verbinding wordt afgesloten door server");
            Serial.printf("Client %d is losgekoppeld.\n", i);
            clients[i].stop();
            clients[i] = WiFiClient();
          }
        }
      }
    }
  }
}

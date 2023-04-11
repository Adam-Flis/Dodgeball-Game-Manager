#include "main.hpp"

// Replace with your network credentials
const char* ssid = "Dodgeball";
const char* password = "OSUBallz";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void startServer(){
  
  // Connect to Wi-Fi
  WiFi.softAP(ssid, password, 1, 0, 4);
  IPAddress IP = WiFi.softAPIP();
  Serial.println(IP); // IP = 192.168.4.1  
  server.begin();

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for index
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Route for home side
  server.on("/home", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", side_html);
  });

  // Route for away side
  server.on("/away", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", side_html);
  });

  // Send a GET request to <ESP_IP>/updateServer?
  server.on("/updateServer", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String msg;
    String side;
    
    // GET state on <ESP_IP>/update?
    if (request->hasParam("state")) {
      // GET side on <ESP_IP>/update?
      if (request->hasParam("side")) {
        side = request->getParam("side")->value();
      }
      msg = request->getParam("state")->value();
      updateSideServer(msg, side);
    } else if (request->hasParam("min")) {
      int min = request->getParam("min")->value().toInt();
      int max = request->getParam("max")->value().toInt();
      updateMinMax(min, max);
    }

    request->send(200, "text/plain", "OK");
  });

  // Send a GET request to <ESP_IP>/time
  server.on("/updateClient", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", updateSideClient());
  });  

  // Start server
  server.begin();
}
  





#include "main.hpp"

void startServer(){  
  
  // Connect to Wi-Fi
  WiFi.softAP(ssid, password, 1, 0, 4);  
  IPAddress IP = WiFi.softAPIP();

  Serial.println("Soft APIP: " + IP.toString()); // APIP = 192.168.4.1 
  Serial.println("Local IP: " + WiFi.localIP().toString()); // IP = 0.0.0.0

  // Handle requests for index html
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Handle requests for team1 html
  server.on("/team1", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", shotclock_html);
  });

  // Handle requests for team2 html
  server.on("/team2", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", shotclock_html);
  });

  // Handle requests for game clock html
  server.on("/gameclock", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", gameclock_html);
  });

  // Handle requests for updateServer call
  server.on("/updateServer", HTTP_GET, [](AsyncWebServerRequest *request) {
    
    // GET things on <ESP_IP>/update?
    String side = request->getParam("side")->value();
    if (side == "index") {
      String min = request->getParam("min")->value();
      String max = request->getParam("max")->value();
      updateIndex(min, max); // Update the index
    } else if (side == "gameclock") {
      String type = request->getParam("type")->value();
      String value = request->getParam("value")->value();
      gameClk.updateState(type, value); // Update the game clock
    } else {
      String type = request->getParam("type")->value();
      String value = request->getParam("value")->value();
      if (side == "team1") team1.updateState(type, value); // Update team 1
      else if (side == "team2") team2.updateState(type, value); // Update team 2
    }          

    request->send(200, "text/plain", "OK");
  });

  Serial.println("Requests configured");

  // Handle Web Server Events
  server.addHandler(&events);
  Serial.println("Handlers configured");

  server.begin(); // Start server
  Serial.println("Server started");
}








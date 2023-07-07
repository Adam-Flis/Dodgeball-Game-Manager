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

  // Handle requests for settings html
  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", settings_html);
  });

  // // Handle requests for new game html
  // server.on("/newgame", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send_P(200, "text/html", newgame_html);
  // });

  // Handle requests for practice html
  server.on("/practice", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", practice_html);
  });

  // Handle requests for updateServer call
  server.on("/updateServer", HTTP_GET, [](AsyncWebServerRequest *request) {
    
    // GET things on <ESP_IP>/update?
    String side = request->getParam("side")->value();
    if (side == "index") { // Update index
      String min = request->getParam("min")->value();
      String max = request->getParam("max")->value();
      updateIndex(min, max);
    } else if (side == "gameclock") { // Update game clock
      String type = request->getParam("type")->value();
      String value = request->getParam("value")->value();
      gameClk.updateState(type, value); // Update the game clock
    } else if (side == "team1" || side == "team2") { // Update shot clock
      String type = request->getParam("type")->value();
      String value = request->getParam("value")->value();
      if (side == "team1") team1.updateState(type, value); // Update team 1
      else if (side == "team2") team2.updateState(type, value); // Update team 2
    } else if (side == "load") { // Update
      updateClient();
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








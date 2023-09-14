/*******************************************************
 * Intellectual Property Notice
 *
 * All the contents of this software/code, including but not limited to
 * algorithms, designs, methodologies, and ideas (collectively referred
 * to as "the Contents"), are the intellectual property of Digital Sports Solutions
 * LLC. The Contents are protected by intellectual property laws and all rights
 * are reserved.
 *
 * Unauthorized use, reproduction, or distribution of the Contents,
 * in whole or in part, is strictly prohibited and may result in legal
 * action.
 *
 * For inquiries or licensing opportunities, please contact
 * adamflis2002@gmail.com
 *******************************************************/

#include "ESPAsyncWebServer.h"
#include "main.hpp"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create a WebSocket server object on port 80
AsyncWebSocket ws("/ws");

// Network credentials
const char* ssid = "NCDAShotclock";
const char* password = "Dodgeball";

void initWifi() {
  // Connect to Wi-Fi
  WiFi.softAP(ssid, password, 1, 0, 10);  
  IPAddress IP = WiFi.softAPIP();

  Serial.println("Soft APIP: " + IP.toString()); // APIP = 192.168.4.1 
  Serial.println("Local IP: " + WiFi.localIP().toString()); // IP = 0.0.0.0
}

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    // A new client connected
    Serial.println(F("WebSocket client connected"));
  } else if (type == WS_EVT_DISCONNECT) {
    // A client disconnected
    Serial.println(F("WebSocket client disconnected"));
  }
}

void startServer(){  
  
  // Handle requests for index html
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/index.html", "text/html");
  });

  // Handle requests for team1 html
  server.on("/team1", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/shotclock.html", "text/html");
  });

  // Handle requests for team2 html
  server.on("/team2", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/shotclock.html", "text/html");
  });

  // Handle requests for game clock html
  server.on("/gameclock", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/gameclock.html", "text/html");
  });

  // Handle requests for settings html
  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/settings.html", "text/html");
  });

  // Handle requests for practice html
  server.on("/practice", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/practice.html", "text/html");
  });

  // Handle requests for obs data
  server.on("/obs", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/data.txt", "text/html");
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

  server.serveStatic("/", SD, "/");
  Serial.println(F("Requests configured"));

  // Set up WebSocket server and event handlers
  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);
  Serial.println(F("Handlers configured"));

  server.begin(); // Start server
  Serial.println(F("Server started"));
}
#include "main.hpp"

// Replace with your network credentials
const char* ssid = "NCDAShotclock";
const char* password = "Dodgeball";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void startServer(){  
  
  // Connect to Wi-Fi
  WiFi.softAP(ssid, password, 1, 0, 4);  
  IPAddress IP = WiFi.softAPIP();

  server.begin(); // Start server
  Serial.println("Server started");

  Serial.println("Soft APIP: " + IP.toString()); // APIP = 192.168.4.1 
  Serial.println("Local IP: " + WiFi.localIP().toString()); // IP = 0.0.0.0 

  mountSPIFFS();
  //byte audioBuffer[1024];
  //loadWAVFile("/audio.wav", audioBuffer, sizeof(audioBuffer));

  // Handle requests for index html
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Handle requests for home side html
  server.on("/home", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", side_html);
  });

  // Handle requests for away side html
  server.on("/away", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", side_html);
  });

  // server.on("/alarm.wav", HTTP_GET, [](AsyncWebServerRequest *request) {
  //     AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/alarm.wav", "audio/wav");
  //     response->addHeader("Content-Disposition", "inline; filename=alarm.wav");
  //     request->send(response);
  // });

  // Handle requests for updateServer call
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

  // Handle requests for updateClient call
  server.on("/updateClient", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", updateSideClient());
  });

  Serial.println("Requests configured");
}

void mountSPIFFS() {
  if(!SPIFFS.begin(true)){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
  } else {
      Serial.println("SPIFFS mounted");
  } 
}







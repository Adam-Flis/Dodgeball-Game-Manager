#ifndef MAIN_HPP
#define MAIN_HPP

#include <Adafruit_NeoPixel.h>
#include <stdio.h>
#include <stdlib.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "HardwareSerial.h"

#include "shotClock.hpp"
#include "gameClock.hpp"
#include "buzzer.hpp"

extern long largeNums[];
extern long smallNums[];

void startServer();
void loadEvents();
void updateSide(String side, String type, String value);
void updateIndex(String min, String max);

String updateSideClient();
String rgbToHex(int r, int g, int b);

extern const char* ssid;
extern const char* password;

// Create AsyncWebServer object on port 80
extern AsyncWebServer server;

extern ShotClock team1;
extern ShotClock team2;
extern GameClock gameClk;
extern Buzzer buzzer;
extern AsyncEventSource indexEvents;

extern const char index_html[];
extern const char shotclock_html[];

#endif
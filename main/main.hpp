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
void updateIndex(String min, String max);

String getData();
String rgbToHex(int r, int g, int b);
void updateClient();

extern const char* ssid;
extern const char* password;

// Create AsyncWebServer object on port 80
extern AsyncWebServer server;

extern ShotClock team1;
extern ShotClock team2;
extern GameClock gameClk;
extern Buzzer buzzer;
extern AsyncEventSource events;

extern const char index_html[];
extern const char gameclock_html[];
extern const char shotclock_html[];
extern const char settings_html[];
//extern const char newgame_html[];
extern const char practice_html[];

#endif
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

#ifndef MAIN_HPP
#define MAIN_HPP

#include <Adafruit_NeoPixel.h>
#include <stdio.h>
#include <stdlib.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncWebSocket.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "HardwareSerial.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include "functions.hpp"
#include "server.hpp"
#include "sd.hpp"
#include "shotClock.hpp"
#include "gameClock.hpp"
#include "buzzer.hpp"

extern long largeNums[];
extern long smallNums[];

// Class instances
extern ShotClock team1;
extern ShotClock team2;
extern GameClock gameClk;
extern Buzzer buzzer1;
extern Buzzer buzzer2;

//extern const char index_html[];
//extern const char gameclock_html[];
//extern const char shotclock_html[];
//extern const char settings_html[];
//extern const char newgame_html[];
//extern const char practice_html[];

#endif
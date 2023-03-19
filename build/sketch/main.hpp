#line 1 "C:\\Users\\adamf_6sjoanp\\Documents\\Coding\\NCDA-Shotclock\\main\\main.hpp"
#ifndef MAIN_H
#define MAIN_H

#include <Adafruit_NeoPixel.h>
#include <stdio.h>
#include <stdlib.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

struct display {
  int pin;
  int numPixels;
  Adafruit_NeoPixel pixels = Adafruit_NeoPixel(pin, numPixels, NEO_GRB + NEO_KHZ800);
  int red;
  int green;
  int blue;
};

typedef struct side {
  int cnt = 0;
  int preCnt = 0;
  int resetMax = 10;
  int curMax = 10;
  int playerCnt = 0;
  int timeoutCnt = 0;
  bool paused = false;
  bool countUp = true;
  bool _10sec = true;
  struct display disp;
} side;

extern long numbers[] = {
  0b000111111111111111111,  // [0] 0
  0b000111000000000000111,  // [1] 1
  0b111111111000111111000,  // [2] 2
  0b111111111000000111111,  // [3] 3
  0b111111000111000000111,  // [4] 4
  0b111000111111000111111,  // [5] 5
  0b111000111111111111111,  // [6] 6
  0b000111111000000000111,  // [7] 7
  0b111111111111111111111,  // [8] 8
  0b111111111111000111111,  // [9] 9
  0b000000000000000000000,  // [10] off
};

void configure(int pin, int numPixels, struct side *side);

void displayNumber(int num, int segment, struct display *disp);
void setColor(struct display *disp, int red, int green, int blue);
void blackout(struct display *disp);

void wifi();
int getValue(int num, struct side *side);
void count(int num, struct side *side);

void update(int num);
void updateColor(int red, int green, int blue);
String getTimeValue(char side);

#endif
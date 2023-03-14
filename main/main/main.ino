#include <Adafruit_NeoPixel.h>
#include <stdio.h>
#include <stdlib.h>

struct display {
  int pin = 0;
  int numPixels = 0;
  Adafruit_NeoPixel pixels = Adafruit_NeoPixel(pin, numPixels, NEO_GRB + NEO_KHZ800);
  int red = 0;
  int green = 0;
  int blue = 0;
};

struct side {
  int time = 0;
  int playerCnt = 0;
  int timeoutCnt = 0;
  bool paused = false;
  bool countUp = false;
  bool _10sec = false;
  struct display disp;
};

long numbers[] = {
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

struct side Home;
struct side Away;
struct side Middle;

void setup() { 
  Serial.begin(115200);
  configure(12, 42, &Home);
  delay(200);
}

void loop() {
  setColor();
  for (int i = 0; i < 15; i++) {
    if (Home.countUp) {
        setNumbers(i + 1, &(Home.disp));
    } else {
        setNumbers(abs(i - 15), &(Home.disp));
    }
    Home.disp.pixels.show();
    delay(1000);
  }  
}

void configure(int pin, int numPixels, struct side *side) {
  side.disp.pin = pin;
  side.disp.numPixels = numPixels;
  side.disp.pixels.updateLength(42);
  side.disp.pixels.setPin(12);
  side.disp.pixels.begin();
}

void setNumbers(int num, struct display *disp) {
  // int _1000s = num / 1000;
  // int _100s = num / 100;
  int _10s = num / 10;
  int _1s = num % 10;
  displayNumber(_10s, 1, disp);
  displayNumber(_1s, 0, disp);
}

void displayNumber(int num, int segment, struct display *disp) {
  /*
   * 
      __ __ __        __ __ __          __ __ __        12 13 14  
    __        __    __        __      __        __    11        15
    __        __    __        __      __        __    10        16
    __        __    __        __  42  __        __    _9        17
      __ __ __        __ __ __          __ __ __        20 19 18  
    __        65    __        44  43  __        21    _8        _0
    __        __    __        __      __        __    _7        _1
    __        __    __        __      __        __    _6        _2
      __ __ __       __ __ __           __ __ __       _5 _4 _3   
   */
 
  // segment from left to right: 3, 2, 1, 0
  int startindex = 0;
  switch (segment) {
    case 0:
      startindex = 0;
      break;
    case 1:
      startindex = 21;
      break;
    case 2:
      startindex = 44;
      break;
    case 3:
      startindex = 65;
      break;    
  }

  for (int i = 0; i < 21; i++){
    ((numbers[num] & 1 << i) == 1 << i) ? 
    disp->pixels.setPixelColor(i + startindex, disp->pixels.Color(disp->red, disp->green, disp->blue)) : 
    disp->pixels.setPixelColor(i + startindex, disp->pixels.Color(0, 0, 0));
  } 

}

void setColor() {
  Home.disp.red = random(0, 255);
  Home.disp.green = random(0, 255);
  Home.disp.blue = random(0, 255);
}

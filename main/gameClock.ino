#include "main.hpp"

void GameClock::configure(int pin) {
    pixels.setPin(pin);
    pixels.begin();            
}

// Color functions
void GameClock::setColor(int red, int green, int blue) {
    color[0] = red;
    color[1] = green;
    color[2] = blue;
    display();
}

int GameClock::getRed() {
    return color[0];
}

int GameClock::getGreen() {
    return color[1];
}

int GameClock::getBlue() {
    return color[2];
}

void GameClock::setDisplay(int num, int segment) {

    // Segment from left to right: 3, 2, 1, 0
    int startindex = 0;
    switch (segment) {
    case 0:
        startindex = 0;
        break;
    case 1:
        startindex = 14;
        break;
    case 2:
        startindex = 30;
        break;
    case 3:
        startindex = 44;
    }

    for (int i = 0; i < 14; i++) {
        ((smallNums[num] & 1 << i) == 1 << i) ? 
        pixels.setPixelColor(i + startindex, pixels.Color(color[0], color[1], color[2])) : 
        pixels.setPixelColor(i + startindex, 0x000000);
    }
}

void GameClock::setColon() {
    pixels.setPixelColor(28, pixels.Color(color[0], color[1], color[2]));
    pixels.setPixelColor(29, pixels.Color(color[0], color[1], color[2]));
}

void GameClock::display() {
    if (!blink) {
        setDisplay(min / 10, 3);
        setDisplay(min % 10, 2);
        setColon();
        setDisplay(sec / 10, 1);
        setDisplay(sec % 10, 0);
        pixels.show();
    } else { off(); }
}

void GameClock::off() {
    pixels.clear();
    pixels.show();
}

void GameClock::setSec(int val) {
    sec = val;
}

void GameClock::setMin(int val) {
    min = val;
}

int GameClock::getSec() {
    return sec;
}

int GameClock::getMin() {
    return min;
}

unsigned long GameClock::getTime() {
    return timer;
}

void GameClock::setTime(unsigned long val) {
    timer = val;
}

void GameClock::decrement(){
    sec--;
    if(sec < 0){
        min--;
        sec = 59;
    }
    if(min < 0){
        min = 0;
        sec = 0;
    }
}

void GameClock::toggleBlink(){
    blink = !blink;
}
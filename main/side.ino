#include "main.hpp"

int minTime = 10, maxTime = 15;

// Min functions
void setMin(int val) {
    minTime = val;
}

int getMin() {
    return minTime;
}

// Max functions
void setMax(int val) {
    maxTime = val;
}

int getMax() {
    return maxTime;
}

void Side::configure(int pin) {
    pixels.setPin(pin);
    pixels.begin();            
}

// Color functions
void Side::setColor(int red, int green, int blue) {
    color[0] = red;
    color[1] = green;
    color[2] = blue;
    displayNumber(cnt);
}

int Side::getRed() {
    return color[0];
}

int Side::getGreen() {
    return color[1];
}

int Side::getBlue() {
    return color[2];
}

void Side::setDisplay(int num, int segment) {
    /*
          __ __ __        12 13 14  
        __        __    11        15
        __        __    10        16
        __        __    _9        17
          __ __ __        20 19 18  
        __        21    _8        _0
        __        __    _7        _1
        __        __    _6        _2
          __ __ __        _5 _4 _3   
    */

    // Segment from left to right: 1, 0
    int startindex = 0;
    switch (segment) {
    case 0:
        startindex = 0;
        break;
    case 1:
        startindex = 21;
        break;  
    }

    for (int i = 0; i < 21; i++){
        ((numbers[num] & 1 << i) == 1 << i) ? 
        pixels.setPixelColor(i + startindex, pixels.Color(color[0], color[1], color[2])) : 
        pixels.setPixelColor(i + startindex, 0x000000);
    }
}

void Side::displayNumber(int num) {
    if (!blink) {
        int sec = convertNumber(num);
        setDisplay(sec / 10, 1);
        setDisplay(sec % 10, 0);
        pixels.show();
    } else { off(); }
    
}

void Side::off() {
    pixels.clear();
    pixels.show();
}

int Side::convertNumber(int num) {
    int ret = num;
    // Count up or down
    if (countUp) ret = cntMax - num;            
    return ret;
}

int Side::getCount() { 
    return cnt; 
}

void Side::decrementCounts() {
    if (cnt > 0) cnt--;            
    if (preCnt > 0) preCnt--;            
}

void Side::reset() {
    preCnt = cnt;
    cnt = resetMax;
    cntMax = resetMax;
    blink = false;
    violation = false;
    timer = millis();

    // Reset to inital value
    displayNumber(cnt);        
}

void Side::undo() {
    int temp = cnt;        
    cnt = preCnt;
    preCnt = temp;
    blink = false;
    violation = false;

    // Update display
    displayNumber(cnt); 
}

// Clock violation functions
void Side::setViolation(bool state) {
    violation = state;
}

bool Side::getViolation() {
    return violation;
}

// Count up or down functions
void Side::updateResetMax() {
    if (isMin) resetMax = getMin();
    else resetMax = getMax();
}

void Side::toggleDirection() {
    countUp = !countUp;
    displayNumber(cnt);
}

bool Side::getDirection() {
    return countUp;
}

void Side::toggleDuration() {
    isMin = !isMin;
    updateResetMax();
}

bool Side::getDuration() {
    return isMin;
}

// Paused functions
void Side::togglePause() {
    paused = !paused;
    if (paused) {
        blink = false;
        violation = false;
        timeDelta = millis() - timer;
        displayNumber(cnt);
    }
    else timer = millis() - timeDelta;
}

bool Side::getPaused() {
    return paused;
}

// Blink functions
void Side::toggleBlink() {
    blink = !blink;
}

// Time functions
unsigned long Side::getTime() {
    return timer;
}

void Side::setTime(unsigned long val) {
    timer = val;
}

// Update variable states depending on what the server sends
void Side::updateState(String str) {
    if (str == "toggleDur") toggleDuration(); // Toggle min/max sec
    else if (str == "reset") reset(); // Reset
    else if (str == "undo") undo(); // Undo reset call
    else if(str == "toggleDir") toggleDirection(); // Toggle count up or down
    else if (str == "pause") togglePause(); // Toggle paused
    else if (str.substring(0, 5) == "color") { // Change color
        // Get red, green, blue values
        int red = str.substring(str.indexOf("r=") + 2, 
                        str.indexOf("g=")).toInt();
        int green = str.substring(str.indexOf("g=") + 2, 
                                str.indexOf("b=")).toInt();
        int blue = str.substring(str.indexOf("b=") + 2).toInt();
        setColor(red, green, blue);
    }
    
}
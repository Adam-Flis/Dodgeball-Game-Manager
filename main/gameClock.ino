#include "main.hpp"

void GameClock::configure(int pin, String str) {
    name = str;
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
    display();
}

void GameClock::setMin(int val) {
    min = val;
    display(); 
}

int GameClock::getSec() {
    return sec;
}

int GameClock::getMin() {
    return min;
}

unsigned long GameClock::getTimer() {
    return timer;
}

void GameClock::resetTimer() {
    timer = millis();
}

void GameClock::decrement(){
    sec--;
    if (sec < 0){
        min--;
        sec = 59;
    }
    if (min < 0){
        min = 0;
        sec = 0;
    }
    display();
}

// Pause functions
void GameClock::togglePause() {
    if (!paused) {
        pause();
    }
    else unpause();    
}

void GameClock::pause() {
    if (!paused) {
        paused = true;
        blink = false;
        //fade = true;
        //brightness = 1.0;
        timeDelta = millis() - timer;
        display();
    }
}

void GameClock::unpause() {
    if (paused) {
        paused = false;
        blink = false;
        //fade = false;
        //brightness = 1.0;
        timer = millis() - timeDelta;
        display();
    }
}

bool GameClock::getPaused() {
    return paused;
}

// Blink functions
void GameClock::toggleBlink(){
    blink = !blink;
}

// Middle of a point functions
void GameClock::setMidPoint(bool val) {
    midPoint = val;
}

bool GameClock::getMidPoint() {
    return midPoint;
}

// Half functions
void GameClock::setHalf(String str) {
    half = str;
}

String GameClock::getHalf() {
    return half;
}

// Name functions
void GameClock::setName(String str) {
    name = str;
}

String GameClock::getName() {
    return name;
}

// Update functions
void GameClock::updateState(String type, String value) {
    if (type == "pause") {
        pause();
        team1.pause();
        team2.pause();
        buzzer.run(3);
    } else if (type == "resume") {
        unpause();
        team1.unpause();
        team2.unpause();
        buzzer.run(3);
    } else if (type == "timeout") {
        if (value == "team1") {
            team1.subTimeout();
            team2.reset();
        } else if (value == "team2") {
            team2.subTimeout();
            team1.reset();
        } else if (value == "official") {            
            pause();          
            team1.pause();
            team1.reset();
            team2.pause();            
            team2.reset();
        }
    } else if (type == "endpoint" && value != "close") {
        if (value == "team1") {
            team1.addPoint();
        } else if (value == "team2") {
            team2.addPoint();
        }
        midPoint = false;
    } else if (type == "startpoint") {
        buzzer.run(3);
        midPoint = true;
        unpause();
    }
    else if (type == "color") {
        // Get red, green, blue values
        int red = value.substring(value.indexOf("r=") + 2, 
                        value.indexOf("g=")).toInt();
        int green = value.substring(value.indexOf("g=") + 2, 
                                value.indexOf("b=")).toInt();
        int blue = value.substring(value.indexOf("b=") + 2).toInt();
        setColor(red, green, blue);
    } else if (type == "minutes") {
        int val = value.toInt();
        setMin(val);
    } else if (type == "seconds") {
        int val = value.toInt();
        setSec(val);
    } else if (type == "half") {
        setHalf(value);
    } 
    updateClient();
}
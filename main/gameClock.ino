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

#include "main.hpp"

void GameClock::configure(uint8_t pin, String str) {
    name = str;
    pixels.setPin(pin);
    pixels.begin();            
}

// Color functions
void GameClock::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    color[0] = red;
    color[1] = green;
    color[2] = blue;    
    display();
}

uint8_t GameClock::getRed() {
    return color[0];
}

uint8_t GameClock::getGreen() {
    return color[1];
}

uint8_t GameClock::getBlue() {
    return color[2];
}

void GameClock::setDisplay(uint8_t num, uint8_t segment) {
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

    for (uint8_t i = 0; i < 14; i++) {
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
        if (official) {
            dispMin = 99 - toMin;
            dispSec = 59 - toSec;
        }
        setDisplay(dispMin / 10, 3);
        setDisplay(dispMin % 10, 2);
        setColon();
        setDisplay(dispSec / 10, 1);
        setDisplay(dispSec % 10, 0);
        pixels.show();
    } else { off(); }
}

void GameClock::off() {
    pixels.clear();
    pixels.show();
}

uint8_t GameClock::getDisplaySec() {
    return dispSec;
}

uint8_t GameClock::getDisplayMin() {
    return dispMin;
}

// Time functions
void GameClock::setSec(uint8_t val) {
    clkSec = val;
    if (!timeout) dispSec = val;
    display();
}

void GameClock::setMin(uint8_t val) {
    clkMin = val;
    if (!timeout) dispMin = val;
    display();
}

uint8_t GameClock::getSec() {
    return clkSec;
}

uint8_t GameClock::getMin() {
    return clkMin;
}

// Timeout functions
void GameClock::setTimeoutSec(uint8_t val) {
    toSec = val;
    if (timeout) dispSec = val;
    display();
}

void GameClock::setTimeoutMin(uint8_t val) {
    toMin = val;
    if (timeout) dispMin = val;
    display();
}

uint8_t GameClock::getTimeoutSec() {
    return toSec;
}

uint8_t GameClock::getTimeoutMin() {
    return toMin;
}

bool GameClock::getTimeout() {
    return timeout;
}

void GameClock::setTimeout(bool val) {
    timeout = val;
    if (timeout) {
        dispSec = toSec;
        dispMin = toMin;
    } else {
        dispSec = clkSec;
        dispMin = clkMin;
    }
    display();
}

// Timer functions
unsigned long GameClock::getTimer() {
    return timer;
}

void GameClock::resetTimer() {
    timer = millis();
}

void GameClock::decrement(){  
    if (timeout) {
        if (toSec > 0) { // Check if seconds is not 0
            toSec--;
        } else if (toSec == 0 && toMin > 0) { // Check if seconds is 0 and minutes is not 0
            toSec = 59;
            toMin--;            
        }
        dispSec = toSec;
        dispMin = toMin;
    } else {        
        if (clkSec > 0) { // Check if seconds and minutes are not 0
            clkSec--;
        } else if (clkSec == 0 && clkMin > 0) { // Check if seconds is 0 and minutes is not 0
            clkSec = 59;
            clkMin--;            
        }
        dispSec = clkSec;
        dispMin = clkMin;
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

// Tournament Name functions
void GameClock::setTournamentName(String str) {
    tournamentName = str;
}

String GameClock::getTournamentName() {
    return tournamentName;
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
        gameClk.pause();
        team1.pause();
        team2.pause();
        buzzer1.play(3);
        buzzer2.play(3);
    } else if (type == "resume") {
        gameClk.unpause();
        //team1.unpause();
        //team2.unpause();
        buzzer1.play(3);
        buzzer2.play(3);
    } else if (type == "timeout" && value != "close") {
        timeout = true;
        official = false;
        if (value == "team1") {
            team1.subTimeout();
            team2.reset();
            gameClk.setTimeoutMin(0);
            gameClk.setTimeoutSec(30);
        } else if (value == "team2") {
            team2.subTimeout();
            team1.reset();
            gameClk.setTimeoutMin(0);
            gameClk.setTimeoutSec(30);
        } else if (value == "official") {            
            team1.reset();
            team2.reset();
            official = true;
            gameClk.setTimeoutMin(99);
            gameClk.setTimeoutSec(59);
        }
        gameClk.unpause();
    } else if (type == "endtimeout") {
        timeout = false;
        official = false;
        gameClk.pause();
        gameClk.setMin(clkMin);
        gameClk.setSec(clkSec);
    }
    else if (type == "endpoint" && value != "close") {
        if (value == "team1") {
            team1.addPoint();
        } else if (value == "team2") {
            team2.addPoint();
        }
        midPoint = false;
    } else if (type == "startpoint") {
        buzzer1.play(3);
        buzzer2.play(3);
        midPoint = true;
        gameClk.unpause();
    }
    else if (type == "color") {
        // Get red, green, blue values
        uint8_t red = value.substring(value.indexOf("r=") + 2, 
                        value.indexOf("g=")).toInt();
        uint8_t green = value.substring(value.indexOf("g=") + 2, 
                                value.indexOf("b=")).toInt();
        uint8_t blue = value.substring(value.indexOf("b=") + 2).toInt();
        setColor(red, green, blue);
    } else if (type == "minutes") {
        uint8_t val = value.toInt();
        setMin(val);
    } else if (type == "seconds") {
        uint8_t val = value.toInt();
        setSec(val);
    } else if (type == "half") {
        setHalf(value);
    } else if (type== "tournamentName") {
        setTournamentName(value);
    } else if (type == "swap") {
        uint8_t temp = team1.getPin();
        team1.setPin(team2.getPin());
        team2.setPin(temp);
        team1.display();
        team2.display();
        temp = buzzer1.getPin();
        buzzer1.setPin(buzzer2.getPin()); 
        buzzer2.setPin(temp);
    }
    updateClient();
}
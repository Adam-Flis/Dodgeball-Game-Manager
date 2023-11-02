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
void GameClock::setColor(uint8_t red, uint8_t green, uint8_t blue, bool secondary) {
    if (secondary) {
        secondaryColor[0] = red;
        secondaryColor[1] = green;
        secondaryColor[2] = blue;
    } else {
        color[0] = red;
        color[1] = green;
        color[2] = blue;
    }   
    display();
}

uint8_t GameClock::getRed(bool secondary) {
    if (secondary) return secondaryColor[0];
    else return color[0];
}

uint8_t GameClock::getGreen(bool secondary) {
    if (secondary) return secondaryColor[1];
    else return color[1];    
}

uint8_t GameClock::getBlue(bool secondary) {
    if (secondary) return secondaryColor[2];
    else return color[2];    
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
            break;
    } 

    uint8_t red, green, blue;
    if (secondary) {
        // Set secondary color values
        red = secondaryColor[0];
        green = secondaryColor[1];
        blue = secondaryColor[2];
    } else {
        // Set primary color values
        red = color[0];
        green = color[1];
        blue = color[2];
    }

    for (uint8_t i = 0; i < 14; i++) {
        ((smallNums[num] & (1 << i)) == (1 << i)) ? 
        pixels.setPixelColor(i + startindex, pixels.Color(red, green, blue)) : 
        pixels.setPixelColor(i + startindex, 0x000000);
    }
}

void GameClock::setColon() {
    uint8_t red, green, blue;
    if (secondary) {
        // Set secondary color values
        red = secondaryColor[0];
        green = secondaryColor[1];
        blue = secondaryColor[2];
    } else {
        // Set primary color values
        red = color[0];
        green = color[1];
        blue = color[2];
    }

    pixels.setPixelColor(28, pixels.Color(red, green, blue));
    pixels.setPixelColor(29, pixels.Color(red, green, blue));
}


void GameClock::display() {      
    if (!blink) {
        if (official) {
            dispMin = 99 - secMin;
            dispSec = 59 - secSec;
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
    if (!secondary) dispSec = val;
    display();
}

void GameClock::setMin(uint8_t val) {
    clkMin = val;
    if (!secondary) dispMin = val;
    display();
}

uint8_t GameClock::getSec() {
    return clkSec;
}

uint8_t GameClock::getMin() {
    return clkMin;
}

// Secondary time functions
void GameClock::setSecondarySec(uint8_t val) {
    secSec = val;
    if (secondary) dispSec = val;
    display();
}

void GameClock::setSecondaryMin(uint8_t val) {
    secMin = val;
    if (secondary) dispMin = val;
    display();
}

uint8_t GameClock::getSecondarySec() {
    return secSec;
}

uint8_t GameClock::getSecondaryMin() {
    return secMin;
}

bool GameClock::getSecondary() {
    return secondary;
}

void GameClock::setSecondary(bool val) {
    secondary = val;
    if (secondary) {
        dispSec = secSec;
        dispMin = secMin;
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
    if (secondary) {
        if (secSec > 0) { // Check if seconds is not 0
            secSec--;
        } else if (secSec == 0 && secMin > 0) { // Check if seconds is 0 and minutes is not 0
            secSec = 59;
            secMin--;            
        }
        dispSec = secSec;
        dispMin = secMin;
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
        secondary = true;
        official = false;
        if (value == "team1") {
            team1.subTimeout();
            team2.reset();
            gameClk.setSecondaryMin(0);
            gameClk.setSecondarySec(30);
        } else if (value == "team2") {
            team2.subTimeout();
            team1.reset();
            gameClk.setSecondaryMin(0);
            gameClk.setSecondarySec(30);
        } else if (value == "official") {            
            team1.reset();
            team2.reset();
            official = true;
            gameClk.setSecondaryMin(99);
            gameClk.setSecondarySec(59);
        }
        gameClk.unpause();
    } else if (type == "endtimeout") {
        secondary = false;
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
        setColor(red, green, blue, false);
    } else if (type == "secondaryColor") {
        // Get red, green, blue values
        uint8_t red = value.substring(value.indexOf("r=") + 2, 
                        value.indexOf("g=")).toInt();
        uint8_t green = value.substring(value.indexOf("g=") + 2, 
                                value.indexOf("b=")).toInt();
        uint8_t blue = value.substring(value.indexOf("b=") + 2).toInt();
        setColor(red, green, blue, true);
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
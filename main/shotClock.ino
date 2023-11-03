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

uint8_t minTime = 10, maxTime = 15;

// Min functions
void setMin(uint8_t val) {
    minTime = val;
}

uint8_t getMin() {
    return minTime;
}

// Max functions
void setMax(uint8_t val) {
    maxTime = val;
}

uint8_t getMax() {
    return maxTime;
}

// Configure LEDs and events
void ShotClock::configure(uint8_t pin, String str) {
    side = str;
    pixels.setPin(pin);
    pixels.begin();            
}

// Color functions
void ShotClock::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    color[0] = red;
    color[1] = green;
    color[2] = blue;
    display();
}

uint8_t ShotClock::getRed() {
    return color[0];
}

uint8_t ShotClock::getGreen() {
    return color[1];
}

uint8_t ShotClock::getBlue() {
    return color[2];
}

// Brightness functions
void ShotClock::setBrightness(float val) {
    brightness = val;
    display();
}

float ShotClock::getBrightness() {
    return brightness;
}

// Display functions
void ShotClock::setDisplay(uint8_t num, uint8_t segment) {

    // Segment from left to right: 1, 0
    uint8_t startindex = 0;
    switch (segment) {
    case 0:
        startindex = 0;
        break;
    case 1:
        startindex = 21;
        break;  
    }

    if (segment == 1 && convertNumber(cnt) < 10) {
        num = 10;
    }

    for (uint8_t i = 0; i < 21; i++){
        ((largeNums[num] & 1 << i) == 1 << i) ? 
        pixels.setPixelColor(i + startindex, pixels.Color(color[0] * brightness, color[1] * brightness, color[2] * brightness)) : 
        pixels.setPixelColor(i + startindex, 0x000000);
    }
}

void ShotClock::display() {    
    if (!blink) {
        uint8_t sec = convertNumber(cnt);
        setDisplay(sec / 10, 1);
        setDisplay(sec % 10, 0);
        pixels.show();
    } else { off(); }
}

void ShotClock::off() {
    pixels.clear();
    pixels.show();
}

// Counter functions
uint8_t ShotClock::convertNumber(uint8_t num) {
    uint8_t ret = num;
    // Count up or down
    if (!countDown) ret = cntMax - num;
    return ret;
}

uint8_t ShotClock::getCount() { 
    return cnt; 
}

void ShotClock::decrement() {
    if (cnt > 0) cnt--;            
    if (preCnt > 0) preCnt--;            
}

void ShotClock::setCount(uint8_t val) {
    preCnt = cnt;
    cnt = val;
}

// Other functions
void ShotClock::reset() {
    preCnt = cnt;
    cnt = resetMax;
    cntMax = resetMax;
    blink = false;
    violation = false;
    timer = millis();

    // Update display
    display();
}

void ShotClock::undo() {
    int temp = cnt;        
    cnt = preCnt;
    preCnt = temp;
    blink = false;
    violation = false;

    // Update display
    display();
}

// Timer functions
unsigned long ShotClock::getTimer() {
    return timer;
}

void ShotClock::resetTimer() {
    timer = millis();
}

// Violation functions
void ShotClock::setViolation(bool state) {
    violation = state;
}

void ShotClock::toggleViolation() {
    violation = !violation;
}

bool ShotClock::getViolation() {
    return violation;
}

// Direction functions
void ShotClock::setDirection(bool state) {
    countDown = state;
}

void ShotClock::toggleDirection() {
    countDown = !countDown;    
    display();
}

String ShotClock::getDirection() {
    if (countDown) return "down";
    return "up";
}

// Duration functions
void ShotClock::setDuration(bool state) {
    isMin = state;
    updateResetMax();  
}

void ShotClock::toggleDuration() {
    isMin = !isMin;
    updateResetMax();
}

String ShotClock::getDuration() {
    if (isMin) return "minimum";
    return "maximum";
}

// Pause functions
void ShotClock::togglePause() {
    if (!paused) {
        pause();
    }
    else unpause();    
}

void ShotClock::pause() {
    if (!paused) {
        paused = true;
        blink = false;
        fade = true;
        brightness = 1.0;
        timeDelta = millis() - timer;
        display();
    }
}

void ShotClock::unpause() {
    if (paused) {
        paused = false;
        blink = false;
        fade = false;
        brightness = 1.0;
        timer = millis() - timeDelta;
        display();
    }
}

bool ShotClock::getPaused() {
    return paused;
}

// Blink/fade functions
void ShotClock::toggleBlink() {
    blink = !blink;
}

void ShotClock::setFade(bool state) {
    fade = state;
}

bool ShotClock::getFade() {
    return fade;
}

// Player functions
void ShotClock::addPlayer() {
    players++;
}

void ShotClock::subPlayer() {
    players--;
}

uint8_t ShotClock::getPlayers() {
    return players;
}

void ShotClock::setPlayers(uint8_t val) {
    players = val;
}

// Timeout functions
void ShotClock::addTimeout() {
    timeouts++;
}

void ShotClock::subTimeout() {
    timeouts--;
}

uint8_t ShotClock::getTimeouts() {
    return timeouts;
}

void ShotClock::setTimeouts(uint8_t val) {
    timeouts = val;
}

// Point functions
void ShotClock::addPoint() {
    points++;
}

void ShotClock::subPoint() {
    points--;
}

uint8_t ShotClock::getPoints() {
    return points;
}

void ShotClock::setPoints(uint8_t val) {
    points = val;
}

// Name functions
void ShotClock::setName(String val) {
    name = val;
}

String ShotClock::getName() {
    return name;
}

String ShotClock::getSide() {
    return side;
}

// Pin functions
uint8_t ShotClock::getPin() {
    return pixels.getPin();
}

void ShotClock::setPin(uint8_t val) {
    pixels.setPin(val);
}

// Update functions
// Count up or down functions
void ShotClock::updateResetMax() {
    if (isMin) resetMax = getMin();
    else resetMax = getMax();
}

// Update variable states depending on what the server sends
void ShotClock::updateState(String type, String value) {
    if (type == "duration") toggleDuration(); // Toggle min/max sec
    else if (type == "reset") reset(); // Reset
    else if (type == "undo") {
        undo(); // Undo reset call
        if (side == "Team 1") buzzer1.play(0.5);
        else if (side == "Team 2") buzzer2.play(0.5);
    }
    else if(type == "direction") toggleDirection(); // Toggle count up or down
    else if (type == "pause") togglePause(); // Toggle paused
    else if (type == "color") { // Change color
        // Get red, green, blue values
        uint8_t red = value.substring(value.indexOf("r=") + 2, 
                        value.indexOf("g=")).toInt();
        uint8_t green = value.substring(value.indexOf("g=") + 2, 
                                value.indexOf("b=")).toInt();
        uint8_t blue = value.substring(value.indexOf("b=") + 2).toInt();
        setColor(red, green, blue);
    } else if (type == "players") { // Update player count    
        uint8_t num = value.toInt();
        players = num;
    } else if (type == "points") { // Update points
        uint8_t num = value.toInt();
        points = num;
    } else if (type == "timeouts") { // Update timeout count
        uint8_t num = value.toInt();
        timeouts = num;
    } else if (type == "name") { // Update name
        name = value;
    } else if (type == "timer") { // Update shot clock
        reset();
        uint8_t num = value.toInt();
        cnt = num;
    }
    updateClient();
}



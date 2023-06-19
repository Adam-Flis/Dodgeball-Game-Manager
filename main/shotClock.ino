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

void ShotClock::configure(int pin, String str) {
    side = str;
    String newStr = str;
    newStr.replace(" ", "");
    newStr.toLowerCase();
    events = AsyncEventSource("/events/" + newStr);
    pixels.setPin(pin);
    pixels.begin();            
}

// Color functions
void ShotClock::setColor(int red, int green, int blue) {
    color[0] = red;
    color[1] = green;
    color[2] = blue;
    display();
}

int ShotClock::getRed() {
    return color[0];
}

int ShotClock::getGreen() {
    return color[1];
}

int ShotClock::getBlue() {
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

void ShotClock::setDisplay(int num, int segment) {

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
        ((largeNums[num] & 1 << i) == 1 << i) ? 
        pixels.setPixelColor(i + startindex, pixels.Color(color[0] * brightness, color[1] * brightness, color[2] * brightness)) : 
        pixels.setPixelColor(i + startindex, 0x000000);
    }
}

void ShotClock::display() {    
    if (!blink) {
        int sec = convertNumber(cnt);
        setDisplay(sec / 10, 1);
        setDisplay(sec % 10, 0);
        pixels.show();
    } else { off(); }
    updatePage();
}

void ShotClock::off() {
    pixels.clear();
    pixels.show();
}

int ShotClock::convertNumber(int num) {
    int ret = num;
    // Count up or down
    if (!countDown) ret = cntMax - num;
    return ret;
}

int ShotClock::getCount() { 
    return cnt; 
}

void ShotClock::decrement() {
    if (cnt > 0) cnt--;            
    if (preCnt > 0) preCnt--;            
}

void ShotClock::reset() {
    preCnt = cnt;
    cnt = resetMax;
    cntMax = resetMax;
    blink = false;
    violation = false;
    timer = millis();

    // Reset to inital value
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

// Clock violation functions
void ShotClock::setViolation(bool state) {
    violation = state;
}

bool ShotClock::getViolation() {
    return violation;
}

// Count up or down functions
void ShotClock::updateResetMax() {
    if (isMin) resetMax = getMin();
    else resetMax = getMax();
}

void ShotClock::toggleDirection() {
    countDown = !countDown;    
    display();
}

bool ShotClock::getDirection() {
    return countDown;
}

void ShotClock::toggleDuration() {
    isMin = !isMin;
    updatePage();
    updateResetMax();
}

bool ShotClock::getDuration() {
    return isMin;
}

// Paused functions
void ShotClock::togglePause() {
    paused = !paused;
    if (paused) {
        pause();
    }
    else unpause();
}

void ShotClock::pause() {
    if (paused) {
        paused = true;
        blink = false;
        fade = true;
        brightness = 1.0;
        timeDelta = millis() - timer;
        display();
    }
}

void ShotClock::unpause() {
    if (!paused) {
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

// Blink functions
void ShotClock::toggleBlink() {
    blink = !blink;
}

// Fade functions
void ShotClock::setFade(bool state) {
    fade = state;
}

bool ShotClock::getFade() {
    return fade;
}

// Time functions
unsigned long ShotClock::getTime() {
    return timer;
}

void ShotClock::setTime() {
    timer = millis();
}

// Update variable states depending on what the server sends
void ShotClock::updateState(String type, String value) {
    if (type == "duration") toggleDuration(); // Toggle min/max sec
    else if (type == "reset") reset(); // Reset
    else if (type == "undo") undo(); // Undo reset call
    else if(type == "direction") toggleDirection(); // Toggle count up or down
    else if (type == "pause") togglePause(); // Toggle paused
    else if (type == "color") { // Change color
        // Get red, green, blue values
        int red = value.substring(value.indexOf("r=") + 2, 
                        value.indexOf("g=")).toInt();
        int green = value.substring(value.indexOf("g=") + 2, 
                                value.indexOf("b=")).toInt();
        int blue = value.substring(value.indexOf("b=") + 2).toInt();
        setColor(red, green, blue);
    } else if (type == "players") { // Update player count    
        int num = value.toInt();
        playerCnt = num;
        updatePage();
    } else if (type == "points") { // Update points
        int num = value.toInt();
        points = num;
        updatePage();
    } else if (type == "timeouts") { // Update timeout count
        int num = value.toInt();
        timeoutCnt = num;
        updatePage();
    } else if (type == "name") { // Update name
        name = value;
        updatePage();
        updateIndex("undefined", "undefined");
    } else if (type == "load") { // Update page
        updatePage();
    }
}

void ShotClock::updatePage() {
    // Create JSON object
    DynamicJsonDocument doc(512);
    if (name == "") {
        doc["title"] = side + ": Shot Clock";
    } else {
        doc["title"] = side + ": " + name + " Shot Clock";
    }
    doc["name"] = name;
    doc["paused"] = paused;
    doc["timer"] = convertNumber(cnt);
    doc["duration"] = isMin;
    doc["minLength"] = getMin();
    doc["maxLength"] = getMax();
    doc["points"] = points;
    doc["players"] = playerCnt;
    doc["timeouts"] = timeoutCnt;    
    doc["color"] = rgbToHex(color[0], color[1], color[2]);
    doc["direction"] = countDown;
    String ret;
    serializeJson(doc, ret);
    events.send(ret.c_str(), "update", millis());
}

String ShotClock::getName() {
    return name;
}
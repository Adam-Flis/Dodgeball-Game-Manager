#include "main.hpp"

Side home;
Side away;

// Setup
void setup() { 
    Serial.begin(115200);
    
    delay(100);

    home.configure(18);
    away.configure(25);
    Serial.println("Digits configured");

    delay(100);

    startServer();

    delay(100);

    Serial.println("Setup finished");
}

// Loop that runs everything
void loop() {
    // Home shot clock
    if (!home.getPaused()) { // Check if clock is not paused
        if (millis() - home.getTime() >= 1000) { // 1 second has passed
            home.setTime(millis());
            home.decrementCounts();
            home.displayNumber(home.getCount());
            if (home.getCount() == 0) {
                home.toggleBlink();           
            }  
        }
        if (home.getCount() == 0) { // Shot clock violation has occurred
            home.setViolation(true);
        }   
    }

    // Away shot clock
    if (!away.getPaused()) { // Check if clock is not paused
        if (millis() - away.getTime() >= 1000) { // 1 second has passed
            away.setTime(millis());
            away.decrementCounts();
            away.displayNumber(away.getCount());
            if (away.getCount() == 0) {
                away.toggleBlink();           
            }  
        }
        if (away.getCount() == 0) { // Shot clock violation has occurred
            away.setViolation(true);
        }
    }

    delay(10);
}

// Update function that gets called from server
void updateSideServer(String str, String side) {
    if (side == "home") {
        home.updateState(str);
    } else if (side == "away") {
        away.updateState(str);
    }
}

// Update min/max function that gets called from server
void updateMinMax(int min, int max) {
    setMin(min);
    setMax(max);
    home.updateResetMax();
    away.updateResetMax();
}

// Update function that updates the webpage
String updateSideClient() {

    // Create JSON object
    DynamicJsonDocument doc(512);

    // Both sides objects
    JsonObject bothObj = doc.createNestedObject("both");
    bothObj["max"] = getMax();
    bothObj["min"] = getMin();

    // Home elements
    JsonObject homeObj = doc.createNestedObject("home");
    homeObj["count"] = home.convertNumber(home.getCount());
    homeObj["paused"] = home.getPaused();
    homeObj["violation"] = home.getViolation();
    homeObj["countDown"] = !home.getDirection();
    homeObj["isMax"] = !home.getDuration();
    
    // Color elements
    JsonObject homeColorObj = homeObj.createNestedObject("color");
    homeColorObj["red"] = home.getRed();
    homeColorObj["green"] = home.getGreen();
    homeColorObj["blue"] = home.getBlue();

    // Away elements
    JsonObject awayObj = doc.createNestedObject("away");
    awayObj["count"] = away.convertNumber(away.getCount());
    awayObj["paused"] = away.getPaused();
    awayObj["violation"] = away.getViolation();
    awayObj["countDown"] = !away.getDirection();
    awayObj["isMax"] = !away.getDuration();    
    
    // Color elements
    JsonObject awayColorObj = awayObj.createNestedObject("color");
    awayColorObj["red"] = away.getRed();
    awayColorObj["green"] = away.getGreen();
    awayColorObj["blue"] = away.getBlue();

    // Convert JSON object to string
    String ret;
    serializeJson(doc, ret);
    return ret;
}
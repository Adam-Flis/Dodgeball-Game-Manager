#include "main.hpp"

Side home;
Side away;

// Setup
void setup() { 
  Serial.begin(115200);
  delay(100);
  startServer();
  delay(100);
  home.configure(25);
  delay(100);
  Serial.println("Setup Finished");
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

    delay(10);
}

// Update function that gets called from server
void updateSideServer(String str, String side) {
    if (side == "Home") {
        home.updateState(str);
    } else if (side == "Away") {
        away.updateState(str);
    }
}

// Update function that updates the webpage
String updateSideClient() {

    // Create JSON object
    DynamicJsonDocument doc(256);
    JsonObject homeObj = doc.createNestedObject("home");
    homeObj["count"] = home.convertNumber(home.getCount());
    homeObj["paused"] = home.getPaused();
    homeObj["violation"] = home.getViolation();
    homeObj["countDown"] = !home.getDirection();
    homeObj["_15sec"] = !home.getDuration();
    
    JsonObject colorObj = homeObj.createNestedObject("color");
    colorObj["red"] = home.getRed();
    colorObj["green"] = home.getGreen();
    colorObj["blue"] = home.getBlue();

    // JsonObject awayObj = doc.createNestedObject("away");
    // awayObj["count"] = away.getCount();
    // awayObj["paused"] = away.isPaused();
    // awayObj["countup"] = away.isCountUp();
    // awayObj["_10sec"] = away.is10Sec();

    // Convert JSON object to string
    String ret;
    serializeJson(doc, ret);
    return ret;
}
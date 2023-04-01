#include "main.hpp"

Side home;
Side away;

void setup() { 
  Serial.begin(115200);
  startServer();
  delay(100);
  home.configure(25);
  delay(200);
  Serial.println("Setup Finished");
}

void loop() {
    if (!home.getPaused() && millis() - home.getTime() >= 1000) { // 1 second has passed on home clock
        home.setTime(millis());
        home.decrementCounts();
        home.displayNumber(home.getCount());
        Serial.println(home.getCount());
        if (home.getCount() == 0) {
            home.toggleBlink();
        }        
    }    

    delay(10);
}

void updateSideServer(String str, String side) {
    if (side == "Home") {
        home.updateState(str);
    } else if (side == "Away") {
        away.updateState(str);
    }
}

String updateSideClient(String side) {
    String ret = "99";
    if (side == "Home") {
        ret = String(home.convertNumber(home.getCount()));
    }
    return ret;
}
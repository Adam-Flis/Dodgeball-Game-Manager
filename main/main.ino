#include "HardwareSerial.h"
#include "main.hpp"

extern long largeNums[] = {
  0b111111111000111111111, // [0] 0
  0b000000111000000000111, // [1] 1
  0b000111111111111111000, // [2] 2
  0b000111111111000111111, // [3] 3
  0b111000111111000000111, // [4] 4
  0b111111000111000111111, // [5] 5
  0b111111000111111111111, // [6] 6
  0b000111111000000000111, // [7] 7
  0b111111111111111111111, // [8] 8
  0b111111111111000111111, // [9] 9
  0b000000000000000000000, // [10] off
};

extern long smallNums[] = {
  0b11111100111111, // [0] 0
  0b00001100000011, // [1] 1
  0b00111111111100, // [2] 2
  0b00111111001111, // [3] 3
  0b11001111000011, // [4] 4
  0b11110011001111, // [5] 5
  0b11110011111111, // [6] 6
  0b00111100000011, // [7] 7
  0b11111111111111, // [8] 8
  0b11111111001111, // [9] 9
  0b00000000000000, // [10] off
};

// Replace with your network credentials
const char* ssid = "NCDAShotclock";
const char* password = "Dodgeball";

ShotClock team1;
ShotClock team2;
GameClock gameClk;
Buzzer buzzer;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events/index
AsyncEventSource indexEvents("/events/index"); 

// Setup
void setup() { 
    Serial.begin(115200);
    delay(100);

    team1.configure(26, "Team 1");
    team2.configure(25, "Team 2");
    gameClk.configure(12);
    Serial.println("Displays configured");
    buzzer.configure(18, OUTPUT);

    delay(100);

    startServer();

    delay(100);
    // gameClk.setMin(0);
    // gameClk.setSec(05);
    team1.display();
    team2.display();
    Serial.println("Setup finished");
}

// Loop that runs everything
void loop() {

    // Game clock
    // if (!gameClk.getPaused()) { // Check if clock is not paused
    //     if (millis() - gameClk.getTime() >= 1000) { // 1 second has passed
    //     gameClk.setTime(millis());
    //     gameClk.decrement();
    //     gameClk.display();
    //     if(gameClk.getMin() == 0 && gameClk.getSec() == 0){
    //         gameClk.toggleBlink();
    //         buzzer.toggle();
    //     }            
    // }    
    
    // team1 shot clock
    if (millis() - team1.getTime() >= 1000) { // 1 second has passed
        team1.setTime();
        if (!team1.getPaused()) { // Check if clock is not paused
            team1.decrement();
            team1.display();
            if (team1.getCount() == 0) {
                team1.setViolation(true);
                team1.toggleBlink();
            }  
        }      
    }

    // team2 shot clock
    if (millis() - team2.getTime() >= 1000) { // 1 second has passed
        team2.setTime();
        if (!team2.getPaused()) { // Check if clock is not paused
            team2.decrement();
            team2.display();
            if (team2.getCount() == 0) {
                team2.setViolation(true);
                team2.toggleBlink(); 
            }
        }
    } 
    
    /*
    // Fade in/out
    if (team1.getFade()) {
        float brightness = team1.getBrightness();
        static bool increasing = true;

        if (increasing) {
            brightness += 0.01;
            if (brightness >= 1.0) {
                brightness = 1.0;
                increasing = false;
            }
        } else {
            brightness -= 0.01;
            if (brightness <= 0.3) {
                brightness = 0.3;
                increasing = true;
            }
        }
        team1.setBrightness(brightness);
    }

    if (team2.getFade()) {
        float brightness = team2.getBrightness();
        static bool increasing = true;

        if (increasing) {
            brightness += 0.01;
            if (brightness >= 1.0) {
                brightness = 1.0;
                increasing = false;
            }
        } else {
            brightness -= 0.01;
            if (brightness <= 0.3) {
                brightness = 0.3;
                increasing = true;
            }
        }
        team2.setBrightness(brightness);
    }
    */

    // Shot clock violation has occurred
    // if (team1.getViolation()) { 
    //     team2.pause();
    //     //gameClk.pause();
    //     if (!buzzer.state()) {
    //         buzzer.setTime();
    //     }
    //     buzzer.run(3);
    // }
    // if (team2.getViolation()) {
    //     team1.pause();
    //     //gameClk.pause();
    //     if (!buzzer.state()) {
    //         buzzer.setTime();
    //     }
    //     buzzer.run(3);   
    // }

    delay(10);
}

// Update function that gets called from server
void updateSide(String side, String type, String value) {
    if (side == "team1") {
        team1.updateState(type, value);
    } else if (side == "team2") {
        team2.updateState(type, value);
    }
}

// Update min/max function that gets called from server
void updateIndex(String min, String max) {

    if (min != "undefined" && max != "undefined") {
        setMin(min.toInt());
        setMax(max.toInt());
        team1.updateResetMax();
        team2.updateResetMax();
        team1.updatePage();
        team2.updatePage();
    }    

    // Create JSON object
    DynamicJsonDocument doc(512);   
    if (team1.getName() == "") {
        doc["team1Name"] = "Team 1";
    } else {
        doc["team1Name"] = "Team 1:<br>" + team1.getName();
    } 

    if (team2.getName() == "") {
        doc["team2Name"] = "Team 2";
    } else {
        doc["team2Name"] = "Team 2:<br>" + team2.getName();
    } 
    doc["min"] = getMin();
    doc["max"] = getMax();
    String ret;
    serializeJson(doc, ret);
    indexEvents.send(ret.c_str(), "update", millis());
}

// Convert RGB to hex
String rgbToHex(int r, int g, int b) {
  String red = String(r, HEX);
  String green = String(g, HEX);
  String blue = String(b, HEX);

  String hex = "#";
  if (red.length() == 1) {
    hex += "0";
  }
  hex += red;
  if (green.length() == 1) {
    hex += "0";
  }
  hex += green;
  if (blue.length() == 1) {
    hex += "0";
  }
  hex += blue;
  
  return hex;
}
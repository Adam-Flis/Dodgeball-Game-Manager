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

// Define variables
long largeNums[] = {
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

long smallNums[] = {
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

bool update = false;

// Class instances
ShotClock team1;
ShotClock team2;
GameClock gameClk;
Buzzer buzzer1;
Buzzer buzzer2;

// Setup
void setup() { 
    Serial.begin(115200);
    delay(100);

    // Configure displays
    team1.configure(27, "Team 1");
    team2.configure(25, "Team 2");
    gameClk.configure(26, "Game Clock");
    Serial.println("Displays configured");

    delay(100);

    // Configure buzzers
    buzzer1.configure(15, OUTPUT);
    buzzer2.configure(2, OUTPUT);
    Serial.println("Buzzers configured");

    delay(100);

    initSDCard(); // Initialize SD card
    startServer(); // Start server

    delay(100);   

    // Display initial values 
    team1.display();
    team2.display();
    gameClk.display();
    Serial.println("Setup finished");
}

// Loop that runs everything
void loop() {
    
    // Game clock
    if (millis() - gameClk.getTimer() >= 1000) { // 1 second has passed
        gameClk.resetTimer();
        if (!gameClk.getPaused()) { // Check if clock is not paused
            gameClk.decrement();
            gameClk.display();
            if (gameClk.getDisplayMin() == 0 && gameClk.getDisplaySec() == 0) {
                if (gameClk.getTimeout()) {
                    gameClk.setTimeout(false);
                    gameClk.pause();
                    gameClk.display();
                } else {
                    //gameClk.setViolation(true);
                    gameClk.toggleBlink();
                }
            }
            update = true;  
        }
    }
    
    // Team 1 shot clock
    if (millis() - team1.getTimer() >= 1000) { // 1 second has passed
        team1.resetTimer();
        if (!team1.getPaused()) { // Check if clock is not paused
            team1.decrement();
            team1.display();
            if (team1.getCount() == 0) {
                //team1.setViolation(true);
                team1.toggleBlink();
            }
            update = true;  
        }
    }

    // Team 2 shot clock
    if (millis() - team2.getTimer() >= 1000) { // 1 second has passed
        team2.resetTimer();
        if (!team2.getPaused()) { // Check if clock is not paused
            team2.decrement();
            team2.display();
            if (team2.getCount() == 0) {
                //team2.setViolation(true);
                team2.toggleBlink(); 
            }
            update = true;
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

    if (update) {
        updateClient();
        update = false;
    }

    // Update buzzer
    buzzer1.update();
    buzzer2.update();

    delay(20);
}
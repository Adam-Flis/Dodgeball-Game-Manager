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

#ifndef GAMECLOCK_HPP
#define GAMECLOCK_HPP

class GameClock {
    private:
        uint8_t dispMin = 25,
            dispSec = 0,
            timeDelta = 0,
            secMin = 0,
            secSec = 0,
            clkMin = 25,
            clkSec = 0;
        unsigned long timer = millis();
        uint8_t color[3] = {255, 0, 0};
        uint8_t secondaryColor[3] = {0, 255, 255};
        bool paused = true,
             blink = false,
             midPoint = false,
             secondary = false,
             official = false;
        String name = "",
               tournamentName = "",
               half = "1st";
        Adafruit_NeoPixel pixels = Adafruit_NeoPixel(58, NULL, NEO_GRB + NEO_KHZ800);
        
    public:
        void configure(uint8_t pin, String str);
        
        // Color functions
        void setColor(uint8_t red, uint8_t green, uint8_t blue, bool secondary);
        uint8_t getRed(bool secondary);
        uint8_t getGreen(bool secondary);
        uint8_t getBlue(bool secondary);

        // Display functions
        void setDisplay(uint8_t num, uint8_t segment);
        void setColon();
        void display();
        void off();
        uint8_t getDisplaySec();
        uint8_t getDisplayMin();        
        
        // Time functions
        void setSec(uint8_t val);
        void setMin(uint8_t val);
        uint8_t getSec();
        uint8_t getMin();        

        // Secondary time functions
        void setSecondarySec(uint8_t val);
        void setSecondaryMin(uint8_t val);
        uint8_t getSecondarySec();
        uint8_t getSecondaryMin();
        bool getSecondary();
        void setSecondary(bool val);

        // Timer functions
        unsigned long getTimer();
        void resetTimer();
        void decrement();
        
        // Pause functions
        void togglePause();
        void pause();
        void unpause();
        bool getPaused();

        // Blink functions
        void toggleBlink();

        // Midpoint functions
        void setMidPoint(bool val);
        bool getMidPoint();

        // Half functions
        void setHalf(String str);
        String getHalf();

        // Tournament Name functions
        void setTournamentName(String str);
        String getTournamentName();

        // Name functions        
        void setName(String str);
        String getName();        

        // Update functions
        void updateState(String type, String value);
};

#endif
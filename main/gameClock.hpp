#include "main.hpp"

#ifndef GAMECLOCK_HPP
#define GAMECLOCK_HPP

class GameClock {
    private:
        int dispMin = 25,
            dispSec = 0,
            timeDelta = 0,
            toMin = 0,
            toSec = 0,
            clkMin = 25,
            clkSec = 0;
        unsigned long timer = millis();
        int color[3] = {255, 0, 0};
        bool paused = true,
             blink = false,
             midPoint = false,
             timeout = false,
             official = false;
        String name = "",
               half = "1st";
        Adafruit_NeoPixel pixels = Adafruit_NeoPixel(58, NULL, NEO_GRB + NEO_KHZ800);
        
    public:
        void configure(int pin, String str);
        
        // Color functions
        void setColor(int red, int green, int blue);
        int getRed();
        int getGreen();
        int getBlue();

        // Display functions
        void setDisplay(int num, int segment);
        void setColon();
        void display();
        void off();
        int getDisplaySec();
        int getDisplayMin();        
        
        // Time functions
        void setSec(int val);
        void setMin(int val);
        int getSec();
        int getMin();        

        // Timeout functions
        void setTimeoutSec(int val);
        void setTimeoutMin(int val);
        int getTimeoutSec();
        int getTimeoutMin();
        bool getTimeout();
        void setTimeout(bool val);

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

        // Name functions        
        void setName(String str);
        String getName();        

        // Update functions
        void updateState(String type, String value);
};

#endif
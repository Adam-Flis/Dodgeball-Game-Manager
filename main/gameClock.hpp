#include "main.hpp"

#ifndef GAMECLOCK_HPP
#define GAMECLOCK_HPP

class GameClock {
    private:
        int min = 25,
            sec = 0,
            timeDelta = 0;
        unsigned long timer = millis();
        int color[3] = {255, 0, 0};
        bool paused = true,
             blink = false,
             midPoint = false;
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
        
        // Counter functions
        void setSec(int val);
        void setMin(int val);
        int getSec();
        int getMin();
        void decrement();

        // Timer functions
        unsigned long getTimer();
        void resetTimer();
        
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
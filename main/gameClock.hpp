#include "main.hpp"

#ifndef GAMECLOCK_HPP
#define GAMECLOCK_HPP

class GameClock {
    private:
        int min = 0,
            sec = 0,
            timeDelta = 0;
        unsigned long timer = millis();
        int color[3] = {255, 0, 0};
        bool paused = true,
             blink = false;
        Adafruit_NeoPixel pixels = Adafruit_NeoPixel(58, NULL, NEO_GRB + NEO_KHZ800);

    public:
        void configure(int pin);

        void setColor(int red, int green, int blue);
        int getRed();
        int getGreen();
        int getBlue();

        void setDisplay(int num, int segment);
        void setColon();
        void display();
        void off();
        
        void setSec(int val);
        void setMin(int val);
        int getSec();
        int getMin();

        unsigned long getTime();
        void setTime(unsigned long val);

        void decrement();
        void toggleBlink();
        bool getPaused();
        void pause();
};

#endif
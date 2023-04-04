#include "main.hpp"

#ifndef SIDE_HPP
#define SIDE_HPP

class Side {
    private:
        int cnt = 0,
            cntMax = 0,
            preCnt = 0,
            timeDelta = 0,
            resetMax = 10,
            playerCnt = 12,
            timeoutCnt = 3;
        unsigned long timer = millis();
        int color[3] = {255, 0, 0};
        bool paused = true,
            blink = false,
            countUp = true,
            _10sec = true,
            violation = false;
        Adafruit_NeoPixel pixels = Adafruit_NeoPixel(42, NULL, NEO_GRB + NEO_KHZ800);
    public:
        void configure(int pin);

        void setColor(int red, int green, int blue);
        int getRed();
        int getGreen();
        int getBlue();

        void setDisplay(int num, int segment);
        void displayNumber(int num);
        void off();

        int convertNumber(int num);
        int getCount();
        void decrementCounts();

        void reset();
        void undo();

        void setViolation(bool state);
        bool getViolation();

        void toggleDirection();
        bool getDirection();

        void toggleDuration();
        bool getDuration();
        
        unsigned long getTime();
        void setTime(unsigned long val);

        void togglePause();
        bool getPaused();

        void toggleBlink();

        void updateState(String str);
};

#endif
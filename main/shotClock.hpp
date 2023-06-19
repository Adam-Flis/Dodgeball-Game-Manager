#include "main.hpp"

#ifndef SHOTCLOCK_HPP
#define SHOTCLOCK_HPP

class ShotClock {
    private:
        int cnt = 10,
            cntMax = 10,
            preCnt = 0,
            timeDelta = 0,
            resetMax = 10,
            playerCnt = 12,
            timeoutCnt = 3,
            points = 0;
        unsigned long timer = millis();
        int color[3] = {255, 0, 0};
        float brightness = 1.0;
        bool paused = true,
            blink = false,
            fade = true,
            countDown = true,
            isMin = true,
            violation = false;
        String side = "",
               name = "";
        Adafruit_NeoPixel pixels = Adafruit_NeoPixel(42, NULL, NEO_GRB + NEO_KHZ800);             

    public:
        AsyncEventSource events = AsyncEventSource("/events"); 
        void configure(int pin, String str);

        void setColor(int red, int green, int blue);
        int getRed();
        int getGreen();
        int getBlue();

        void setBrightness(float val);
        float getBrightness();

        void setDisplay(int num, int segment);
        void display();
        void off();

        int convertNumber(int num);
        int getCount();
        void decrement();

        void reset();
        void undo();

        void setViolation(bool state);
        bool getViolation();

        void updateResetMax();
        void toggleDirection();
        bool getDirection();

        void toggleDuration();
        bool getDuration();
        
        unsigned long getTime();
        void setTime();

        void togglePause();
        void pause();
        void unpause();
        bool getPaused();

        void toggleBlink();
        void setFade(bool state);
        bool getFade();

        void updateState(String type, String value);
        void updatePage();
        
        String getName();
};

void setMin(int val);
void setMax(int val);
int getMin();
int getMax();

#endif
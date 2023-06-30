#include "main.hpp"

#ifndef BUZZER_HPP
#define BUZZER_HPP

class Buzzer {
    private:
        bool buzzer = LOW;
        int pin;
        float runTime = 0;
        unsigned long timer = 0;
    public:
        void configure(int pin_, uint8_t mode);
        void toggle();
        bool state();
        void on();
        void off();
        void resetTimer();
        unsigned long getTimer();
        float getRunTime();
        void run(float sec);
};

#endif
#include "main.hpp"

#ifndef BUZZER_HPP
#define BUZZER_HPP

class Buzzer {
    private:
        bool buzzer = LOW;
        int pin;
        unsigned long time = 0;
    public:
        void configure(int pin_, uint8_t mode);
        void toggle();
        bool state();
        void on();
        void off();
        void setTime();
        void run(int sec);
};

#endif
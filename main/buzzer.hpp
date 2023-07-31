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

#ifndef BUZZER_HPP
#define BUZZER_HPP

class Buzzer {
    private:
        bool buzzer = false;
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
        void play(float duration);
        void update();

        int getPin();
        void setPin(int pin_);
};

#endif
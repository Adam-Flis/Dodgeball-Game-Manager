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

void Buzzer::configure(int pin_, uint8_t mode) {
    pin = pin_;
    pinMode(pin, mode);
}

void Buzzer::toggle() {
    buzzer = !buzzer;
    digitalWrite(pin, buzzer);    
}

bool Buzzer::state() {
    return buzzer;
}

void Buzzer::on() {
    buzzer = true;
    digitalWrite(pin, buzzer);
}

void Buzzer::off() {
    buzzer = false;
    digitalWrite(pin, buzzer);
}

void Buzzer::resetTimer() {
    timer = millis();
}

unsigned long Buzzer::getTimer() {
    return timer;
}

float Buzzer::getRunTime() {
    return runTime;
}

void Buzzer::play(float duration) {
    resetTimer();
    runTime = duration;
}

void Buzzer::update() {
    // Check if the buzzer duration has elapsed
    if (millis() - timer <= runTime * 1000) {
        on();
    } else {
        off();
    }
}

void Buzzer::setPin(int pin_) {
    pin = pin_;
}

int Buzzer::getPin() {
    return pin;
}

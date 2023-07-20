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

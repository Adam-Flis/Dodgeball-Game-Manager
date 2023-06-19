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

void Buzzer::setTime() {
    time = millis();
}

void Buzzer::run(int sec) {
  unsigned long currentMillis = millis();

  // Convert the duration from seconds to milliseconds
  unsigned long buzzerDuration = sec * 1000;

  // Check if the buzzer should be turned on
    if (currentMillis - time < buzzerDuration) {
        on();
    } else {
        off();
    }
}
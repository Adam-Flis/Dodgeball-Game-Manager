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

#ifndef SHOTCLOCK_HPP
#define SHOTCLOCK_HPP
class ShotClock {
    private:
        uint8_t cnt = 10,
            cntMax = 10,
            preCnt = 0,
            timeDelta = 0,
            resetMax = 10,
            players = 12,
            timeouts = 2,
            points = 0;
        unsigned long timer = millis();
        uint8_t color[3] = {255, 0, 0};
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
        void configure(uint8_t pin, String str);

        // Color functions
        void setColor(uint8_t red, uint8_t green, uint8_t blue);
        uint8_t getRed();
        uint8_t getGreen();
        uint8_t getBlue();

        // Brightness functions
        void setBrightness(float val);
        float getBrightness();

        // Display functions
        void setDisplay(uint8_t num, uint8_t segment);
        void display();
        void off();

        // Counter functions
        uint8_t convertNumber(uint8_t num);
        uint8_t getCount();
        void decrement();
        void setCount(uint8_t val);

        // Other functions
        void reset();
        void undo();

        // Timer functions        
        unsigned long getTimer();
        void resetTimer();

        // Violation functions
        void setViolation(bool state);
        void toggleViolation();
        bool getViolation();

        // Direction functions
        void setDirection(bool state);
        void toggleDirection();
        String getDirection();

        // Duration functions
        void setDuration(bool state);
        void toggleDuration();
        String getDuration();

        // Pause functions
        void togglePause();
        void pause();
        void unpause();
        bool getPaused();

        // Blink/fade functions
        void toggleBlink();
        void setFade(bool state);
        bool getFade();

        // Player functions
        void addPlayer();
        void subPlayer();
        uint8_t getPlayers();
        void setPlayers(uint8_t val);

        // Timeout functions
        void addTimeout();
        void subTimeout();
        uint8_t getTimeouts();
        void setTimeouts(uint8_t val);

        // Point functions
        void addPoint();
        void subPoint();        
        uint8_t getPoints();
        void setPoints(uint8_t val);        
        
        // Name functions        
        void setName(String str);
        String getName();
        String getSide();

        // Pin functions
        uint8_t getPin();
        void setPin(uint8_t val);

        // Update functions
        void updateResetMax();
        void updateState(String type, String value);
};

// Both shot clocks share the same min/max values
void setMin(uint8_t val);
void setMax(uint8_t val);
uint8_t getMin();
uint8_t getMax();

#endif
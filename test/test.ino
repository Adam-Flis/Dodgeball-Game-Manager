#include <Adafruit_NeoPixel.h>
#define PIN 12
#define NUMPIXELS 42
Adafruit_NeoPixel pixels =
    Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 100; // timing delay in milliseconds
int redColor = 0;
int greenColor = 0;
int blueColor = 0;
void setup() { pixels.begin(); }
void loop() {
  setColor();
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(redColor, greenColor, blueColor));
    pixels.show();
    delay(delayval);
  }
}
void setColor() {
  redColor = random(0, 255);
  greenColor = random(0, 255);
  blueColor = random(0, 255);
}
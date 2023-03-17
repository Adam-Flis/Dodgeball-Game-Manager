#include "main.hpp"

void configure(int pin, int numPixels, struct side *side) {
  side->disp.pin = pin;
  side->disp.pixels.setPin(pin);
  side->disp.numPixels = numPixels;
  side->disp.pixels.updateLength(numPixels);  
  side->disp.pixels.begin();
  setColor(&(side->disp), 255, 0, 0);
}

void displayNumber(int num, int segment, struct display *disp) {
  /*
   * 
      __ __ __        __ __ __          __ __ __        12 13 14  
    __        __    __        __      __        __    11        15
    __        __    __        __      __        __    10        16
    __        __    __        __  42  __        __    _9        17
      __ __ __        __ __ __          __ __ __        20 19 18  
    __        65    __        44  43  __        21    _8        _0
    __        __    __        __      __        __    _7        _1
    __        __    __        __      __        __    _6        _2
      __ __ __       __ __ __           __ __ __       _5 _4 _3   
   */
 
  // segment from left to right: 3, 2, 1, 0
  int startindex = 0;
  switch (segment) {
    case 0:
      startindex = 0;
      break;
    case 1:
      startindex = 21;
      break;
    case 2:
      startindex = 44;
      break;
    case 3:
      startindex = 65;
      break;    
  }

  for (int i = 0; i < 21; i++){
    ((numbers[num] & 1 << i) == 1 << i) ? 
    disp->pixels.setPixelColor(i + startindex, disp->pixels.Color(disp->red, disp->green, disp->blue)) : 
    disp->pixels.setPixelColor(i + startindex, disp->pixels.Color(0, 0, 0));
  } 

}

void setColor(struct display *disp, int red, int green, int blue) {
  disp->red = red;
  disp->green = green;
  disp->blue = blue;
}

void blackout(struct display *disp) {
  for (int i = 0; i < disp->numPixels; i++) {
    disp->pixels.setPixelColor(i, disp->pixels.Color(0, 0, 0));
  }
  disp->pixels.show();
}
#include "main.hpp"

struct side Home;
struct side Away;
struct side Middle;

long timer = 0;
bool blink = false;
bool paused = true;

void setup() { 
  Serial.begin(115200);
  wifi();
  delay(100);
  configure(25, 42, &Home);
  delay(200);
  Serial.println("Setup Finished");
  timer = millis();
}

void loop() {

  if (!paused && millis() - timer >= 1000) { // 1 second has passed
    timer = millis();

    if (Home.cnt == 0) {
      blink = !blink;
    }   
    
    // Decrement count variables
    if(Home.cnt > 0){
      Home.cnt--;
    }
    if (Home.preCnt > 0) {
      Home.preCnt--;
    }

    // Display
    if (Home.cnt >= 0 && !blink) {
      count(Home.cnt, &Home);      
    } 

    // Flash lights on violation
    if (blink) {
      blackout(&(Home.disp));
    }    
  } else if (paused) {
    count(Home.cnt, &Home); 
    int delta = millis() - timer;
    timer = millis() + delta;
  }

  delay(10);  
} 

void update(int num) {
    switch (num) {
    case 0: // Toggle 10 second
      Home._10sec = true;
      Home.resetMax = 10;
      break;
    case 1: // Toggle 15 second
      Home._10sec = false;
      Home.resetMax = 15;
      break;
    case 2: // Reset
      Home.preCnt = Home.cnt;
      Home.cnt = Home.resetMax;
      Home.curMax = Home.resetMax;

      // Reset to inital value
      count(Home.cnt, &Home);

      blink = false;
      timer = millis();
      break;
    case 3: // Swap cnt and preCnt, revert reset
    {
      int temp = Home.cnt;        
      Home.cnt = Home.preCnt;
      Home.preCnt = temp;
      blink = false;
      break;
    }
    case 4: // Toggle count up
      Home.countUp = true;
      count(Home.cnt, &Home);
      break;
    case 5: // Toggle count down
      Home.countUp = false;
      count(Home.cnt, &Home);
      break;
    case 6:// Toggle pause
      paused = !paused;
    default:
      break;
    }
}

void updateColor(int red, int green, int blue) {
  setColor(&(Home.disp), red, green, blue);
}

String getTimeValue(char side) {
  switch (side) {
    case 'H':
        return String(getValue(Home.cnt, &Home));
    default:
      return "99";
  }
}



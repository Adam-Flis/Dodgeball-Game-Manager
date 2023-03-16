#include "main.hpp"

struct side Home;
struct side Away;
struct side Middle;

long timer = 0;
int curMax = 0;
bool blink = false;

void setup() { 
  Serial.begin(115200);
  wifi();
  delay(100);
  configure(12, 42, &Home);
  delay(200);
  Serial.println("Setup Finished");
  timer = millis();
}

void loop() {

  if(millis() - timer >= 1000) { // 1 second has passed
    timer = millis();
    blink = !blink;
    
    // Decrement count variables
    if(Home.cnt > 0){
      Home.cnt--;
    }
    if (Home.preCnt > 0) {
      Home.preCnt--;
    }

    // Display
    if (Home.cnt >= 0) {
      // Count up or down
      if (!Home.countUp) {
        count(Home.cnt, &Home);
      } else {
        count(curMax - Home.cnt, &Home);
      }
    } 

    // Flash lights on violation
    // if (cnt == 0 && blink) {
    //   blackout(&(Home.disp));
    // }

  }

  delay(10);  
}

void update(int num) {
    switch (num) {
    case 0: // Toggle 10 second
      Home._10sec = true;
      Home.cntMax = 10;
      break;
    case 1: // Toggle 15 second
      Home._10sec = false;
      Home.cntMax = 15;
      break;
    case 2: // Reset
      Home.preCnt = Home.cnt;
      Home.cnt = Home.cntMax;
      curMax = Home.cntMax;

      // Reset to inital value
      if (!Home.countUp) {
        count(Home.cnt, &Home);
      } else {
        count(curMax - Home.cnt, &Home);
      }
      
      timer = millis();
      break;
    case 3: // Swap cnt and preCnt, revert reset
    {
      int temp = Home.cnt;        
      Home.cnt = Home.preCnt;
      Home.preCnt = temp;
      break;
    }
    case 4: // Toggle count down
      Home.countUp = false;
      break;
    case 5: // Toggle count up
      Home.countUp = true;
      break;
    default:
      break;
    }
}

void updateColor(int red, int green, int blue) {
  setColor(&(Home.disp), red, green, blue);
}



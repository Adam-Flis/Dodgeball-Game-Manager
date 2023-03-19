#include <Arduino.h>
#line 1 "C:\\Users\\adamf_6sjoanp\\Documents\\Coding\\NCDA-Shotclock\\main\\main.ino"
#include "main.hpp"

struct side Home;
struct side Away;
struct side Middle;

long timer = 0;
bool blink = false;
bool paused = true;

#line 11 "C:\\Users\\adamf_6sjoanp\\Documents\\Coding\\NCDA-Shotclock\\main\\main.ino"
void setup();
#line 21 "C:\\Users\\adamf_6sjoanp\\Documents\\Coding\\NCDA-Shotclock\\main\\main.ino"
void loop();
#line 61 "C:\\Users\\adamf_6sjoanp\\Documents\\Coding\\NCDA-Shotclock\\main\\main.ino"
void update(int num);
#line 105 "C:\\Users\\adamf_6sjoanp\\Documents\\Coding\\NCDA-Shotclock\\main\\main.ino"
void updateColor(int red, int green, int blue);
#line 109 "C:\\Users\\adamf_6sjoanp\\Documents\\Coding\\NCDA-Shotclock\\main\\main.ino"
String getTimeValue(char side);
#line 3 "C:\\Users\\adamf_6sjoanp\\Documents\\Coding\\NCDA-Shotclock\\main\\counter.ino"
void count(int num, struct side *side);
#line 10 "C:\\Users\\adamf_6sjoanp\\Documents\\Coding\\NCDA-Shotclock\\main\\counter.ino"
int getValue(int num, struct side *side);
#line 3 "C:\\Users\\adamf_6sjoanp\\Documents\\Coding\\NCDA-Shotclock\\main\\display.ino"
void configure(int pin, int numPixels, struct side *side);
#line 12 "C:\\Users\\adamf_6sjoanp\\Documents\\Coding\\NCDA-Shotclock\\main\\display.ino"
void displayNumber(int num, int segment, struct display *disp);
#line 51 "C:\\Users\\adamf_6sjoanp\\Documents\\Coding\\NCDA-Shotclock\\main\\display.ino"
void setColor(struct display *disp, int red, int green, int blue);
#line 57 "C:\\Users\\adamf_6sjoanp\\Documents\\Coding\\NCDA-Shotclock\\main\\display.ino"
void blackout(struct display *disp);
#line 140 "C:\\Users\\adamf_6sjoanp\\Documents\\Coding\\NCDA-Shotclock\\main\\server.ino"
void wifi();
#line 11 "C:\\Users\\adamf_6sjoanp\\Documents\\Coding\\NCDA-Shotclock\\main\\main.ino"
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

  if (millis() - timer >= 1000) { // 1 second has passed
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
    
  } 
  // else {
  //   count(Home.cnt, &Home); 
  //   int delta = millis() - timer;
  //   while (paused) {
  //     timer = millis() + delta;
  //     delay(10);
  //   }    
  // }

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
    case 6:
      //paused = !paused;
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



#line 1 "C:\\Users\\adamf_6sjoanp\\Documents\\Coding\\NCDA-Shotclock\\main\\counter.ino"
#include "main.hpp"

void count(int num, struct side *side) {
        int sec = getValue(num, side);
        displayNumber(sec / 10, 1, &(side->disp));
        displayNumber(sec % 10, 0, &(side->disp));
        side->disp.pixels.show();
}

int getValue(int num, struct side *side) {
        int ret = num;
        if (side->countUp) { // Count up or down
                ret = side->curMax - num;
        }
        return ret;
}
#line 1 "C:\\Users\\adamf_6sjoanp\\Documents\\Coding\\NCDA-Shotclock\\main\\display.ino"
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
#line 1 "C:\\Users\\adamf_6sjoanp\\Documents\\Coding\\NCDA-Shotclock\\main\\server.ino"
#include "main.hpp"

// Replace with your network credentials
const char* ssid = "Dodgeball";
const char* password = "adamflis";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #2196F3}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
    button{
        scale: 2;
        margin-top: 40px;
    }
    .slidertxt {
        margin-top: 40px;
    }
    .timer {
			height: 150px;
			width: 150px;
			border: 3px solid black;
			margin: 0px auto;
      margin-bottom: -15px;
			display: flex;
			justify-content: center;
			align-items: center;
      text-align: center;
		}
    .timer p {
			margin: 0;      
			display: flex;
			align-items: center;
			justify-content: center;
      font-size: 120px;
      color: #ff0000;
		}

  </style>
</head>

<body>
  <h2 style="margin-bottom: -10px">NCDA Shotclock</h2>
  <h4>
  <label style="font-size: 20px;">Timer Value:</label>
  <div class="timer">
    <p id="timerText">10</p>
  </div>  
  </h4>
  <button id="reset" onclick="update(2, this)">RESET</button>
  <br>
  <button id="revert" onclick="update(3, this)">REVERT</button>
  <br>
  <button id="pause" onclick="update(6, this)">PAUSE</button>
  <h4 class="slidertxt">
  <label>Clock Length:</label>
    <div style="display: flex; flex-direction: column; align-items: center;">
      <div style="display: flex; align-items: center;">
        <span style="margin-right: 10px;">10</span>
        <label class="switch">
          <input type="checkbox" onchange="update(0, this)">
          <span class="slider"></span>
        </label>
        <span style="margin-left: 10px;">15</span>
      </div>
    </div>
  </h4>
  <h4 class="slidertxt">    
  <label>Count Direction:</label>
    <div style="display: flex; flex-direction: column; align-items: center;">
      <div style="display: flex; align-items: center;">
        <span style="margin-right: 10px;">Up</span>
            <label class="switch">
                <input type="checkbox" onchange="update(4, this)">
                <span class="slider"></span>
            </label>
            <span style="margin-left: 10px;">Dn</span>
            </div>
        </div>
    </h4>
    <h4>
      <label>Clock Color:</label>
      <input type="color" value="#ff0000" onchange="updateColor(this)">
    </h4>
<script>

function update(num, element) {
    var xhr = new XMLHttpRequest();
    if (element.checked) {
      num++;
    }
    xhr.open("GET", `/update?state=${num}`, true);    
    xhr.send();
}

function updateColor(element) {
  var xhr = new XMLHttpRequest();
  const color = element.value;
  const timer = document.getElementById("timerText");
  timer.style.color = color;
  const red = parseInt(color.substr(1, 2), 16);
  const green = parseInt(color.substr(3, 2), 16);
  const blue = parseInt(color.substr(5, 2), 16);
  console.log(`red: ${red}, green: ${green}, blue: ${blue}`);
  xhr.open("GET", `/update?r=${red}&g=${green}&b=${blue}`, true);
  xhr.send();
}

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("timerText").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/time", true);
  xhttp.send();
}, 100);

</script>
</body>
</html>
)rawliteral";


void wifi(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.println(IP);
  server.begin();

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/update?state=<inputMessage>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String msg;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam("state")) {
      msg = request->getParam("state")->value();
      update(msg.toInt());      
    } else if (request->hasParam("r")) {
      int red = request->getParam("r")->value().toInt();
      int green = request->getParam("g")->value().toInt();
      int blue = request->getParam("b")->value().toInt();
      updateColor(red, green, blue);
    }
    request->send(200, "text/plain", "OK");
  });

  // Send a GET request to <ESP_IP>/time
  server.on("/time", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", getTimeValue('H'));
  });

  // Start server
  server.begin();
}
  






#include "main.hpp"
#include "stdlib_noniso.h"

// Replace with your network credentials
const char* ssid = "Dodgeball";
const char* password = "OSUBallz";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
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
  <button id="reset" onclick="update('reset', this)">RESET</button>
  <br>
  <button id="undo" onclick="update('undo', this)">UNDO</button>
  <br>
  <button id="pause" onclick="update('pause', this)">PAUSE</button>
  <h4 class="slidertxt">
  <label>Clock Length:</label>
    <div style="display: flex; flex-direction: column; align-items: center;">
      <div style="display: flex; align-items: center;">
        <span style="margin-right: 10px;">10</span>
        <label class="switch">
          <input type="checkbox" id="length" onchange="update('toggleDur', this)">
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
                <input type="checkbox" id="direction" onchange="update('toggleDir', this)">
                <span class="slider"></span>
            </label>
            <span style="margin-left: 10px;">Dn</span>
            </div>
        </div>
    </h4>
    <h4>
      <label>Clock Color:</label>
      <input type="color" id="colorPicker" value="#ff0000" onchange="update('color', this)">
    </h4>
<script>

// Update the server when an action occurs 
function update(str, element) {    
    if (str === 'color') {

      // Convert hex to red, green, blue values
      const red = parseInt(element.value.substr(1, 2), 16);
      const green = parseInt(element.value.substr(3, 2), 16);
      const blue = parseInt(element.value.substr(5, 2), 16);

      // Build string
      str += `r=${red}g=${green}b=${blue}`;
    }
    var xhr = new XMLHttpRequest();
    xhr.open("GET", `/updateServer?state=${str}`, true);    
    xhr.send();
}

const rgbToHex = (color) => {
  const r = color[0].toString(16).padStart(2, "0");
  const g = color[1].toString(16).padStart(2, "0");
  const b = color[2].toString(16).padStart(2, "0");
  return `#${r}${g}${b}`;
};

var lastVibrateTime = 0;
setInterval(function() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var response = JSON.parse(this.responseText);
      var home = response.home;

      // Update page elements
      document.getElementById("timerText").innerHTML = home.count;
      document.getElementById("length").checked = home._15sec;
      document.getElementById("direction").checked = home.countDown;
      var pauseBtn = document.getElementById("pause");
      if (home.paused) {
        pauseBtn.innerHTML = "RESUME";
      } else {
        pauseBtn.innerHTML = "PAUSE";
      }

      // Update color
      timerElement = document.getElementById("timerText");
      colorElement = document.getElementById("colorPicker");

      var color = home.color;
      const rgb = [color.red, color.green, color.blue];
      const hex = rgbToHex(rgb);

      timerElement.style.color = hex;
      colorElement.value = hex;

      // Vibrate
      if ("vibrate" in navigator) {
        // Vibration API supported
        if (home.violation && (Date.now() - lastVibrateTime) >= 1000) { // Vibrate every second if violation occurred 
          navigator.vibrate(500);
          lastVibrateTime = Date.now();
        }        
      }
    }
  };

  xhttp.open("GET", "/updateClient", true);
  xhttp.send();
}, 100);

</script>
</body>
</html>
)rawliteral";


void startServer(){
  
  // Connect to Wi-Fi
  WiFi.softAP(ssid, password, 1, 0, 4);
  IPAddress IP = WiFi.softAPIP();
  Serial.println(IP); // IP = 192.168.4.1  
  server.begin();

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/updateServer?
  server.on("/updateServer", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String msg;
    String side = "Home"; // Make this get a stored value based on page  
    // GET input value on <ESP_IP>/update?
    if (request->hasParam("state")) {
      msg = request->getParam("state")->value();
      updateSideServer(msg, side);
    }
    request->send(200, "text/plain", "OK");
  });

  // Send a GET request to <ESP_IP>/time
  server.on("/updateClient", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", updateSideClient());
  });  

  // Start server
  server.begin();
}
  





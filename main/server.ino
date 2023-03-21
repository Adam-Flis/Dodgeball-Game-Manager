#include "main.hpp"

// Replace with your network credentials
const char* ssid = "Dodgeball";
const char* password = "OSUBallz";

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
  
  // Connect to Wi-Fi
  WiFi.softAP(ssid, password, 1, 0, 4);
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
  





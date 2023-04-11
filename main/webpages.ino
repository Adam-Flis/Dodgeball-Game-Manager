const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
	<title>NCDA Shotclock</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
	<style>
    label {
      display: flex; 
      justify-content: center; 
      font-size: 24px
    }
		/* Style for button container */
		.side-container {
			display: flex;
			flex-direction: row;
			justify-content: center;
			align-items: center;
		}
		/* Style for buttons */
		button {
			padding: 14px 20px;
			margin: 10px;
			border: none;
			cursor: pointer;
			border-radius: 5px;
			font-size: 16px;
      background-color: #ADD8E6;
		}
    .link {
      color: inherit; /* Inherit the color from its parent */
      text-decoration: none; /* Remove underline */
    }
		.counter-container {
      display: grid;
      justify-content: center;
      grid-template-columns: 10% 10%;
      grid-gap: 10px;
      margin-top: 10px;
    }    
    .counter-container label {
      font-size: 16px;
    }    
    .counter-container input {
      width: 100%;
      text-align: center;
      padding: 5px;
      font-size: 16px;
      border-radius: 5px;
      border: 1px solid #ccc;
      box-sizing: border-box;
      background-color: #e6bbad;
    }
	</style>
</head>
<body>
  <h1 style="margin-bottom: 10px; text-align: center;">NCDA Shotclock</h1>

  <label for="side-select">Side Select:</label>
  <div class="side-container" id="side-select">    
		<button id="home-btn"><a href="/home" class="link">Home</a></button>
		<button id="away-btn"><a href="/away" class="link">Away</a></button>
	</div>

	<label for="edit-counter">Edit Counter:</label>
  <div class="counter-container" style="text-align: center;">
    <div>
      <label for="min">Min:</label>
      <input type="number" id="min" name="min" min="0" max="15" value="10">
    </div>
    <div>
      <label for="max">Max:</label>
      <input type="number" id="max" name="max" min="15" max="99" value="15">
    </div>
  </div>

	<script>
    const minInput = document.getElementById('min');
    const maxInput = document.getElementById('max');    
    
    // Function to run on page load
    window.onload = load();
    function load() {
      updatePage();
      startUpdatePage();
    }

    // Check min/max 
    function checkMinMax() {
      let min = parseInt(minInput.value);
      let max = parseInt(maxInput.value);
      if (min > max) {
        minInput.value = max;
        min = max;
      } 

      if (max > 99) {
        max = 99;
        maxInput.value = max;
      } else if (max < 0) {
        max = 0;
        maxInput.value = max;
      } 

      if (min > 99) {
        min = 99;
        minInput.value = min;
      } else if (min < 0) {
        min = 0;
        minInput.value = min;
      }
      minInput.setAttribute('max', max);
      maxInput.setAttribute('min', min);
      update(min, max);
    }

    // Update the server when an action occurs 
    function update(min, max) {    
      var xhr = new XMLHttpRequest();
      xhr.open("GET", `/updateServer?min=${min}&max=${max}`, true);    
      xhr.send();
    }
  
    // Update HTML page based on server information
    function updatePage() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          var response = JSON.parse(this.responseText);
          // Update min/max values
          minInput.value = parseInt(response.both.min);
          maxInput.value = parseInt(response.both.max);
        }
      }
      xhttp.open("GET", "/updateClient", true);
      xhttp.send();
    }

    var intervalId;
    function startUpdatePage() {
      intervalId = setInterval(updatePage, 50);
    }
    
    function stopUpdatePage() {
      clearInterval(intervalId);
    }
    
    minInput.addEventListener('blur', function() {
      checkMinMax();
      startUpdatePage();
    });
    
    maxInput.addEventListener('blur', function() {
      checkMinMax();
      startUpdatePage();
    });
    
    minInput.addEventListener('focus', stopUpdatePage);
    maxInput.addEventListener('focus', stopUpdatePage);

</script> 
</body>
</html>
)rawliteral";

const char side_html[] PROGMEM = R"rawliteral(
<html>
<head>
  <title>NCDA Shotclock</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
        font-family: Arial; 
        display: inline-block; 
        text-align: center;
    }
    h2 {
        font-size: 3.0rem;
    }
    p {
        font-size: 3.0rem;
    }
    .timer {
        height: 150px;
        width: 150px;
        border: 3px solid black;
        margin: 0px auto;
        margin-bottom: -40px;
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
    button {
        scale: 2;
        font-size: 24px;
        margin-top: 50px;
        padding: 5px;
    }
    .switch {
        position: relative; 
        display: inline-block; 
        width: 120px; 
        height: 68px
    } 
    .switch input {
        display: none
    }
    .slider {
        position: absolute; 
        top: 0; 
        left: 0; 
        right: 0; 
        bottom: 0; 
        background-color: #ccc; 
        border-radius: 34px
    }
    .slider:before {
        position: absolute; 
        content: ""; 
        height: 52px; 
        width: 52px; 
        left: 8px; 
        bottom: 8px; 
        background-color: #fff; 
        -webkit-transition: .4s; 
        transition: .4s; 
        border-radius: 68px
    }
    input:checked+.slider {
        background-color: #2196F3
    }
    input:checked+.slider:before {
        -webkit-transform: translateX(52px); 
        -ms-transform: translateX(52px); 
        transform: translateX(52px)}

    .slidertxt {
        margin-top: 40px;
    }    

  </style>
</head>

<body>
  <a href="#" onclick="window.history.back();" style="position: absolute; top: 0; left: 0; font-size: 24px;"><---</a>
  <h1 style="margin-bottom: -10px" id="title">Side</h1>
  <h4>
  <label style="font-size: 20px;">Timer</label>
  <div class="timer">
    <p id="timerText">10</p>
  </div>  
  </h4>
  <button id="reset" onclick="update('reset', this)" style="font-size: 24px; padding: px;">RESET</button>
  <br>
  <button id="undo" onclick="update('undo', this)">UNDO</button>
  <br>
  <button id="pause" onclick="update('pause', this)">PAUSE</button>
  <h4 class="slidertxt">
  <label>Clock Length:</label>
    <div style="display: flex; flex-direction: column; align-items: center;">
      <div style="display: flex; align-items: center;">
        <span style="margin-right: 10px;" id="min-length">10</span>
        <label class="switch">
          <input type="checkbox" id="length" onchange="update('toggleDur', this)">
          <span class="slider"></span>
        </label>
        <span style="margin-left: 10px;" id="max-length">15</span>
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

var sideName;
function capitalize(str) {
  return str.charAt(0).toUpperCase() + str.slice(1).toLowerCase();
}

// Function to run on page load
window.onload = load();
function load() {
  const path = window.location.pathname;
  sideName = path.substring(path.lastIndexOf('/') + 1);
  document.getElementById("title").innerHTML = capitalize(sideName);
  vibrate(500);
  updatePage();  
}

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
    xhr.open("GET", `/updateServer?side=${sideName}&state=${str}`, true);    
    xhr.send();
}

const rgbToHex = (color) => {
  const r = color[0].toString(16).padStart(2, "0");
  const g = color[1].toString(16).padStart(2, "0");
  const b = color[2].toString(16).padStart(2, "0");
  return `#${r}${g}${b}`;
};

function vibrate(ms) {
  if ("vibrate" in window.navigator) {
    // Android
    window.navigator.vibrate(ms);
  } else if ("webkitVibrate" in window.navigator) {
    // iOS (only works in Safari)
    window.navigator.webkitVibrate(ms);
  } else if ("mozVibrate" in window.navigator) {
    // Firefox
    window.navigator.mozVibrate(ms);
  } else {
    console.log("Vibration not supported");
  }
}

var lastVibrateTime = 0;
// Update HTML page based on server information
function updatePage() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var response = JSON.parse(this.responseText);

      var side;
      if (sideName == "home") {
        side = response.home;
      } else {
        side = response.away;
      }

      // Update min/max text
      document.getElementById("min-length").innerHTML = response.both.min;
      document.getElementById("max-length").innerHTML = response.both.max;

      // Update page elements
      document.getElementById("timerText").innerHTML = side.count;
      document.getElementById("length").checked = side.isMax;
      document.getElementById("direction").checked = side.countDown;
      var pauseBtn = document.getElementById("pause");
      if (side.paused) {
        pauseBtn.innerHTML = "RESUME";
      } else {
        pauseBtn.innerHTML = "PAUSE";
      }

      // Update color
      timerElement = document.getElementById("timerText");
      colorElement = document.getElementById("colorPicker");

      var color = side.color;
      const rgb = [color.red, color.green, color.blue];
      const hex = rgbToHex(rgb);

      timerElement.style.color = hex;
      colorElement.value = hex;

      if (side.violation && (Date.now() - lastVibrateTime) >= 1000) { // Vibrate every second if violation occurred 
        vibrate(500);
        lastVibrateTime = Date.now();
      }      
    };
  }
  xhttp.open("GET", "/updateClient", true);
  xhttp.send();
}

setInterval(updatePage, 50);

</script>
</body>
</html>
)rawliteral";
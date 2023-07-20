const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>NCDA Digital Game Controller</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 0;
      background-color: #f5f5f5;
    }

    .navbar {
      background-color: #3f51b5;
      color: white;
      font-size: 20px;
      height: 50px;
      display: flex;
      align-items: center;
      justify-content: space-between;
      padding: 0px 20px;
      box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);
    }

    .navbar button {
      font-weight: bold;
      font-size: 20px;
      cursor: pointer;
      background: none;
      border: none;
      padding: 0;
      color: white;
    }

    .navbar .home {
      font-size: 30px;
    }

    .section {
      display: flex;
      flex-direction: column;
      align-items: center;
      padding: 15px;
      margin-top: 20px;
      transition: all 0.3s ease;
    }

    .section label {
      font-size: 20px;
      font-weight: bold;
      margin-bottom: 10px;
    }

    .section div {
      display: flex;
      justify-content: center;
      gap: 15px;
    }

    .section button {
      background-color: #fff;
      border: none;
      color: black;
      padding: 10px 20px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 20px;
      cursor: pointer;
      width: 170px;
      border-radius: 5px;
      box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);
      transition: all 0.3s ease;
    }    

    .section button p {
      margin: 0;
      padding: 2px;
      line-height: 1;
    }

    table {
      display: table;
      text-align: center;
      font-size: 20px;
    }

    .number-input {
      display: flex;
      background-color: #fff;
      justify-content: center;
      align-items: center;
      border: none;
      border-radius: 5px;
      padding: 10px;
      box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);
      transition: all 0.3s ease;
    }
    
    .number-input input {
      width: 50px;
      height: 30px;
      background: none;
      text-align: center;
      border: none;
      font-size: 20px;
      -moz-appearance: textfield;
      appearance: textfield;
      outline: none;
    }

    .number-input input::-webkit-inner-spin-button,
    .number-input input::-webkit-outer-spin-button {
      -webkit-appearance: none;
      margin: 0;
    }

    .number-input button {
      width: 30px;
      height: 30px;
      color: black;
      cursor: pointer;
      border: none;
      padding: 0px;
      font-size: 25px;
      display: flex;
      justify-content: center;
      align-items: center;
      background-color: #fff;
      transition: all 0.3s ease;
    }    

    @media (hover: hover) {
      /* Only apply hover effect on devices that support hover */
      .button:hover{
        background-color: #f0f0f0;
      }
      .section button:hover {
        background-color: #f0f0f0;
      }
      .number-input:hover {
        background-color: #f0f0f0;
      }
      .number-input button:hover {
        background-color: #f0f0f0;
      }    
    }
    
  </style>
</head>
<body>
  <div class="navbar">
    <button class="home" onclick="location.reload();">&#8962;</button>
    <span class="title">NCDA Digital Game Controller</span>
    <a href="/settings" class="link"><button class="settings-button">&#9965;</button></a>
  </div>

  <div class="section">
    <label>Shot Clockers:</label>
    <div>    
      <a href="/team1" class="link"><button id="team1-btn">
        <p id="team1-name">Team 1</p>
      </button></a>
      <a href="/team2" class="link">      
      <button id="team2-btn">
        <p id="team2-name">Team 2</p>
      </button></a> 
    </div>
  </div>

  <div class="section">
    <label>Referees:</label>
    <div>    
      <a href="/gameclock" class="link"><button id="ref-btn">
        <p>Game Clock</p>
      </button></a>      
    </div>
  </div>

  <div class="section">
    <label>Other:</label>
    <div>         
      <button id="new-game-btn" onclick="construction()">
        <p>New Game</p>
      </button>
      <a href="/practice" class="link"><button id="practice-btn">
        <p>Practice</p>
      </button></a>  
    </div>
  </div>

  <div class="section">
    <label>Edit Shot Clock Timer:</label>
    <table>
      <tbody>
      <tr>
        <td>Min:</td>
        <td>Max:</td>
      </tr>
      <tr>
        <td>
          <div class="number-input">
            <button onclick="decrement('min')" style="margin-right: -20px">&#706;</button>
            <input type="number" id="min" name="min" min="0" max="15" value="10">
            <button onclick="increment('min')" style="margin-left: -20px">&#707;</button>              
          </div>
        </td>
        <td>
          <div class="number-input">
            <button onclick="decrement('max')" style="margin-right: -20px">&#706;</button>
            <input type="number" id="max" name="max" min="0" max="99" value="15">
            <button onclick="increment('max')" style="margin-left: -20px">&#707;</button>  
          </div>
        </td>
      </tr>
    </tbody>
    </table>
  </div>

  <script>
    var side = "index";
    var source;

    // Function to run on page load
    document.addEventListener('DOMContentLoaded', function() {
      load();
    });

    function load() {
      source = new EventSource(`/events`);
      source.addEventListener('update', events);
      setTimeout(function() {  
        var xhr = new XMLHttpRequest();
        xhr.open("GET", `/updateServer?side=load`, true);    
        xhr.send();       
      }, 100);

      // Get all input elements
      var inputs = document.querySelectorAll('input');

      // Add event listeners to each input element
      inputs.forEach(function(input) {      
        // When the input receives focus, remove event listener
        input.addEventListener('focus', function() {
            source.removeEventListener('update', events);
        });

        // When the input loses focus, add event listener
        input.addEventListener('blur', function() {            
            source.addEventListener('update', events);
            checkMinMax();
        });      
      });
    }

    function increment(inputId) {
      var input = document.getElementById(inputId);
      input.stepUp();
      checkMinMax();
    }

    function decrement(inputId) {
      var input = document.getElementById(inputId);
      input.stepDown();
      checkMinMax();
    }

    function construction() {
      alert("This feature is still under construction!");
    }
    
    // Check min/max 
    function checkMinMax() {
      const minInput = document.getElementById('min');
      const maxInput = document.getElementById('max');   
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
      updateServer(min, max);
    }

    // Update the server when an action occurs 
    function updateServer(min, max) {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", `/updateServer?side=${side}&min=${min}&max=${max}`, true);    
      xhr.send();
    }

    // Add event listeners 
    function events(e) {
      var response = JSON.parse(e.data);
      var team1 = response.team1;
      var team2 = response.team2;

      // Update shot clockers buttons
      if (team1.name === "") { // Team 1
        document.getElementById("team1-name").innerHTML = team1.side;
      } else {
        document.getElementById("team1-name").innerHTML = team1.side + ":\n" + team1.name;
      }

      if (team2.name === "") { // Team 2
        document.getElementById("team2-name").innerHTML = team2.side;        
      } else {
        document.getElementById("team2-name").innerHTML = team2.side + ":\n" + team2.name;
      }

      // Update shot clock timer
      document.getElementById("min").value = response.other.minimumLength;
      document.getElementById("max").value = response.other.maximumLength;      
    }

  </script>
</body>
</html>
)rawliteral";

const char gameclock_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title id="title">Game Clock</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
        font-family: Arial, sans-serif;
        margin: 0;
        padding: 0;
        background-color: #f5f5f5;
    }
    
    .navbar {
        background-color: #3f51b5;
        color: white;
        font-size: 20px;
        height: 50px;
        display: flex;
        align-items: center;
        justify-content: space-between;
        padding: 0px 20px;
        box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);
    }

    .navbar button {
        font-weight: bold;
        font-size: 20px;
        cursor: pointer;
        background: none;
        border: none;
        padding: 0;
        color: white;
    }
    
    .container {
        text-align: center;
        padding: 20px;
    }

    .settings-overlay {
        position: fixed;
        top: 0;
        right: 0;
        bottom: 0;
        left: 100%;
        background-color: rgba(0, 0, 0, 0.5);
        z-index: 999;
        transition: left 0.3s ease-in-out;
      }
  
    .settings-overlay.open {
        left: 0;
    }
  
    .settings-page {
        position: absolute;
        top: 0;
        right: 0;
        bottom: 0;
        width: 300px;
        background-color: #f5f5f5;
        padding: 0px;
        transition: transform 0.3s ease-in-out;
        transform: translateX(100%);
        box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);
    }
  
    .settings-overlay.open .settings-page {
        transform: translateX(0);
    }
  
    .settings-content label,
    .settings-content input {
        margin-bottom: 10px;
        margin-left: 5px;
    }  

    .settings-overlay {
        position: fixed;
        top: 0;
        right: 0;
        bottom: 0;
        left: 100%;
        background-color: rgba(0, 0, 0, 0.5);
        z-index: 999;
        transition: left 0.3s ease-in-out;
    }

    .settings-overlay.open {
        left: 0;
    }

    .settings-page {
        position: absolute;
        top: 0;
        right: 0;
        bottom: 0;
        width: 300px;
        background-color: #f5f5f5;
        padding: 0px;
        transition: transform 0.3s ease-in-out;
        transform: translateX(100%);
        box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);
    }

    .settings-overlay.open .settings-page {
        transform: translateX(0);
    }

    .settings-content label,
    .settings-content input {
        margin-bottom: 10px;
        margin-left: 5px;
    }
    
    .timer {
        background-color: #f5f5f5;
        border: 1px solid #ddd;
        padding: 10px;
        font-size: 70px;        
    }

    .timer span {
        color: #ff0000;
    }
    
    .buttons {
        margin-top: 20px;
    }
    
    .buttons button {
        background-color: #fff;
        border: none;
        color: black;
        padding: 10px 20px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 50px;
        cursor: pointer;
        width: 100%;
        border-radius: 5px;
        box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);
        transition: all 0.3s ease;
        margin-bottom: 20px;
    }    
    
    .info-table {
        width: 100%;
        margin: 0px;
        border-collapse: collapse;
        font-size: 15px;
    }
    
    .info-table td {
        border: 1px solid #ddd;
        padding: 15px;
        text-align: center;
    }
    
    .table-title {
        background-color: #f5f5f5;        
    }
    
    .table-text {
        background-color: #f9f9f9;
    }
    
    .popup {
        display: none;
        position: fixed;
        top: 0;
        left: 0;
        width: 100%;
        height: 100%;
        background-color: rgba(0, 0, 0, 0.5);
    }
    
    .popup-content {
        position: absolute;
        top: 50%;
        left: 50%;
        transform: translate(-50%, -50%);
        background-color: #f5f5f5;
        box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
        border-radius: 4px;
        width: 80%; /* Adjust as needed */
        max-width: 500px; /* Adjust as needed */
    }      
    
    .popup-navbar {
        display: flex;
        justify-content: space-between;
        align-items: center;
        padding-bottom: 10px;
        border-bottom: 1px solid #ddd;
    }   

    .popup-options {
        display: flex;
        flex-direction: column;
        align-items: center;
        justify-content: center;
        padding: 10px;
    }
    
    .popup-options button {
        background-color: #fff;
        border: none;
        color: black;
        padding: 10px 20px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 20px;
        cursor: pointer;
        width: 90%;
        border-radius: 5px;
        box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);
        transition: all 0.3s ease;
        margin-bottom: 10px;
        margin-top: 10px;
    }  
        
    .switch {
        position: relative; 
        display: inline-block; 
        width: 60px; 
        height: 34px;
        margin-top: 5px;
    } 
  
    .switch input {
        opacity: 0;
        width: 0;
        height: 0;
    }
  
    .slider {
        position: absolute;
        cursor: pointer;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #ccc;
        transition: .4s;
        border-radius: 34px;
    }
  
    .slider:before {
        position: absolute;
        content: "";
        height: 26px;
        width: 26px;
        left: 4px;
        bottom: 4px;
        background-color: white;
        transition: .4s;
        border-radius: 50%;
    }
  
    input:checked + .slider {
        background-color: #3f51b5;
    }
  
    input:focus + .slider {
        box-shadow: 0 0 1px #3f51b5;
    }
  
    input:checked + .slider:before {
        transform: translateX(26px);
    }

    .slider:after {
        color: white;
        display: block;
        position: absolute;
        transform: translate(-50%, -50%);
        top: 50%;
        left: 50%;
        font-size: 10px;
        font-family: Verdana, sans-serif;
    }

    @media (hover: hover) {
        /* Only apply hover effect on devices that support hover */
        .button:hover{
            background-color: #f0f0f0;
        }      
        .buttons button:hover {
            background-color: #f0f0f0;
        }
        .popup-options button:hover {
            background-color: #f0f0f0;
        }
    }
      
  </style>
</head>
<body>
  <div class="navbar">
    <a href="/" class="link"><button class="back-button">&#8592;</button></a>
    <span class="title" id="navbar-title">Game Clock</span>
    <button class="settings-button" onclick="openSettings()">&#9965;</button>
  </div>

  <div class="container">    
    <div class="timer">
        <span id="timer-text">25:00</span>
    </div>        
    <div class="buttons">
        <button id="time-button" onclick="button(this)">TIMEOUT</button><br>
        <button id="point-button" onclick="button(this)">START POINT</button>
    </div>
    <div>
        <table class="info-table">
            <tbody>
            <tr>
                <td class="table-text" id="team1-name">Team 1</td>
                <td class="table-text" id="team2-name">Team 2</td>
            </tr>
            <tr>
                <td colspan="2" class="table-title">Timeouts Left</td>
            </tr>
            <tr>
                <td class="table-text" id="team1-timeouts">0</td>
                <td class="table-text" id="team2-timeouts">0</td>
            </tr>
            <tr>
                <td colspan="2" class="table-title">Shot Clock</td>
            </tr>
            <tr>
                <td class="table-text" id="team1-sc">0</td>
                <td class="table-text" id="team2-sc">0</td>
            </tr>
            <tr>
                <td colspan="2" class="table-title">Players</td>
            </tr>
            <tr>
                <td class="table-text" id="team1-players">0</td>
                <td class="table-text" id="team2-players">0</td>
            </tr>
            
            <tr>
                <td colspan="2" class="table-title">Points</td>
            </tr>
            <tr>
                <td class="table-text" id="team1-pts">0</td>
                <td class="table-text" id="team2-pts">0</td>
            </tr>
        </tbody>
        </table>
    </div>
    </div>

    <div id="popup" class="popup">
        <div class="popup-content">
            <div class="navbar" id="popup-navbar"></div>
            <div class="popup-options" id="popup-options"></div>
        </div>
    </div>   
    
  <div class="settings-overlay">
    <div class="settings-page" id="settings">
      <div class="navbar" style="margin-bottom: 20px">
        <span>Settings</span>
        <button class="close-button" onclick="closeSettings()">&#88;</button>
      </div>
      <div class="settings-content">
        <label for="timer">Clock:</label>
        <input type="number" id="minutes" min="0" max="99" value="25" style="width: 40px;"> :
        <input type="number" id="seconds" min="0" max="59" value="0" style="width: 40px;"><br>

        <label for="half">Half:</label>
        <select id="half" onchange="updateServer(this)">
            <option value="1st">1st</option>
            <option value="2nd">2nd</option>
            <option value="OT">OT</option>
        </select><br>
  
        <label for="color">Color:</label>
        <input type="color" id="color" value="#ff0000"><br>

      </div>
    </div>
    </div>

  <script>
    var side = "gameclock";
    var source;

    // Function to run on page load
    document.addEventListener('DOMContentLoaded', function() {
        load();
    });

    
  
    function load() {
        source = new EventSource(`/events`);
        source.addEventListener('update', events);
        setTimeout(function() {  
            var xhr = new XMLHttpRequest();
            xhr.open("GET", `/updateServer?side=load`, true);    
            xhr.send();     
        }, 100);

        // Get all input elements
        var inputs = document.querySelectorAll('input');

        // Add event listeners to each input element
        inputs.forEach(function(input) {
        if (input.type != "checkbox") {       
            // When the input receives focus, remove event listener
            input.addEventListener('focus', function() {
                source.removeEventListener('update', events);
            });

            // When the input loses focus, add event listener
            input.addEventListener('blur', function() {
                checkInput(input);                
                source.addEventListener('update', events);
                updateServer(input);
            });
        } 
        });
    } 

    function button(element) {
        const type = element.innerHTML.toLowerCase().replace(" ", ""); 

        // If the button is resume or startpoint, update server and return
        if (type === "resume") {
            if (document.getElementById('point-button').innerHTML === "START POINT") {
                window.alert("Please start the point before resuming the game.");
                return;
            }
            updateServer(type);
        } else if (type === "startpoint") {
            updateServer(type);
        } else if (type == "endtimeout") {
            updateServer(type);
        }
        else {
            showPopup(element);
        }
    }

    function showPopup(element) {
        updateServer("pause");

        const popupNavbar = document.getElementById('popup-navbar');
        const popupOptions = document.getElementById('popup-options');        
        const type = element.innerHTML.toLowerCase().replace(" ", "");             

        // Popup navbar
        popupNavbar.innerHTML = `<span>${element.innerHTML}</span>`;
        popupNavbar.innerHTML += `<button class="close" id="${type}" value="close" onclick="closePopup(event.target)">&#88;</button>`;
    
        // Popup content
        popupOptions.innerHTML = '';
        var options = [];
        
        if (type === 'timeout') {
            team1Timeouts = document.getElementById('team1-timeouts').innerHTML;
            team2Timeouts = document.getElementById('team2-timeouts').innerHTML;
            if (team1Timeouts > 0) {
                options.push('Team 1');
            }
            if (team2Timeouts > 0) {
                options.push('Team 2');
            }
            options.push('Official');                                           
        } else if (type === 'endpoint') {
            options = ['Team 1', 'Team 2', 'Draw'];            
        }

        options.forEach(option => {                
            const button = document.createElement('button');
            button.innerText = option;
            button.id = type;
            button.value = option.replace(" ", "").toLowerCase();
            button.onclick = () => closePopup(button);
            popupOptions.appendChild(button);
        });        
        document.getElementById('popup').style.display = 'block';
    }    
    
    function closePopup(element) {        
        document.getElementById('popup').style.display = 'none';
        updateServer(element);
    }   
    
    // Settings page functions
    function openSettings() {
        document.querySelector('.settings-overlay').classList.add('open');
    }

    function closeSettings() {
        document.querySelector('.settings-overlay').classList.remove('open');
    }

    const settingsOverlay = document.querySelector('.settings-overlay');
    settingsOverlay.addEventListener('click', (event) => {
        if (event.target === settingsOverlay) {
            closeSettings();
        }
    });

    function checkInput(input) {
        const min = parseInt(input.min);
        const max = parseInt(input.max);
        if (input.value > max) {
            input.value = max;
        } else if (input.value < min) {
            input.value = min;
        }
    }

    // Update the server when an action occurs 
    function updateServer(element) {
        var str = element;
        var value; 

        // Check if element is undefined
        if (typeof element === "object") {
            str = element.id;
            value = element.value;
        }        
        
        if (str === "color") {
            value = value.replace("#", "");
            // Convert the hex color code to RGB
            var r = parseInt(value.substring(0, 2), 16);
            var g = parseInt(value.substring(2, 4), 16);
            var b = parseInt(value.substring(4, 6), 16);
            value = `r=${r}g=${g}b=${b}`;
        }        

        var xhr = new XMLHttpRequest();        
        xhr.open("GET", `/updateServer?side=${side}&type=${str}&value=${value}`, true);    
        xhr.send();
    }

    // Add event listeners 
    function events(e) {
        var response = JSON.parse(e.data);

        var gameClock = response.gameClock;
        var team1 = response.team1;
        var team2 = response.team2;

        // Update game clock   
        document.getElementById('minutes').value = gameClock.minutes;
        document.getElementById('seconds').value = gameClock.seconds;

        // Update display
        var dispMinutes = gameClock.displayedMinutes;
        var dispSeconds = gameClock.displayedSeconds;

        if (dispMinutes < 10) {
            dispMinutes = "0" + String(dispMinutes);
        }
        if (dispSeconds < 10) {
            dispSeconds = "0" + String(dispSeconds);
        }

        document.getElementById('timer-text').innerHTML = dispMinutes + ":" + dispSeconds; 
        
        // Update buttons
        var text = gameClock.paused ? 'RESUME' : 'TIMEOUT';
        if (gameClock.timeout) {
            text = 'END TIMEOUT'
        }
        document.getElementById('time-button').innerHTML = text;

        text = gameClock.middleOfPoint ? 'ENDPOINT' : 'START POINT';
        document.getElementById('point-button').innerHTML = text;

        // Update team names
        if (team1.name === "") { // Team 1
            document.getElementById("team1-name").innerHTML = team1.side;
        } else {
            document.getElementById("team1-name").innerHTML = team1.side + ": " + team1.name;
        }

        if (team2.name === "") { // Team 2
            document.getElementById("team2-name").innerHTML = team2.side;        
        } else {
            document.getElementById("team2-name").innerHTML = team2.side + ": " + team2.name;
        }  

        // Update team points
        document.getElementById('team1-pts').innerHTML = team1.points;
        document.getElementById('team2-pts').innerHTML = team2.points;

        // Update team timeouts
        document.getElementById('team1-timeouts').innerHTML = team1.timeoutsLeft;
        document.getElementById('team2-timeouts').innerHTML = team2.timeoutsLeft;

        // Update team players
        document.getElementById('team1-players').innerHTML = team1.playersLeft;
        document.getElementById('team2-players').innerHTML = team2.playersLeft;

        // Update team shot clockers
        document.getElementById('team1-sc').innerHTML = team1.displayedCount;
        document.getElementById('team2-sc').innerHTML = team2.displayedCount;

        // Update half 
        document.getElementById('half').value = gameClock.half;

        // Update color
        document.getElementById("timer-text").style.color = gameClock.color.hex;
        document.getElementById("color").value = gameClock.color.hex;

    }

  </script>
</body>
</html>
)rawliteral";

const char shotclock_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title id="title">Team _: Shot Clock</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 0;
      background-color: #f5f5f5;
    }

    .navbar {
      background-color: #3f51b5;
      color: white;
      font-size: 20px;
      height: 50px;
      display: flex;
      align-items: center;
      justify-content: space-between;
      padding: 0px 20px;
      box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);
    }

    .navbar button {
      font-weight: bold;
      font-size: 20px;
      cursor: pointer;
      background: none;
      border: none;
      padding: 0;
      color: white;
    }

    .content {
      text-align: center;
      margin-top: 20px;
    }

    .settings-overlay {
      position: fixed;
      top: 0;
      right: 0;
      bottom: 0;
      left: 100%;
      background-color: rgba(0, 0, 0, 0.5);
      z-index: 999;
      transition: left 0.3s ease-in-out;
    }

    .settings-overlay.open {
      left: 0;
    }

    .settings-page {
      position: absolute;
      top: 0;
      right: 0;
      bottom: 0;
      width: 300px;
      background-color: #f5f5f5;
      padding: 0px;
      transition: transform 0.3s ease-in-out;
      transform: translateX(100%);
      box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);
    }

    .settings-overlay.open .settings-page {
      transform: translateX(0);
    }

    .settings-content label,
    .settings-content input {
      margin-bottom: 10px;
      margin-left: 5px;
    }    

    .container {
      display: flex;
      flex-direction: column;
      height: calc(100vh - 50px);
      background-color: #f5f5f5;
    }

    .top-half {
      display: flex;
      flex-basis: 50%;
    }

    .button {
      flex-grow: 1;
      display: block;
      align-items: center;
      justify-content: center;
      box-sizing: border-box;
      border: 1px solid #ddd;
      background-color: #fff;
      font-size: 50px;
      cursor: pointer;
      transition: all 0.3s ease;
      box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);
    }

    .button.left {
      justify-content: flex-start;
      text-align: center;
    }

    .button.right {
      justify-content: flex-end;
    }

    .bottom-half {
      display: flex;
      flex-basis: 50%;
    }

    .button.big {
      flex-grow: 1;
    }    

    .middle-container {
      position: absolute;
      top: 52%;
      left: 49%;
      transform: translate(-50%, -50%);
      width: 150px;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      padding: 5px;
    }   
      
    .timer {
      width: 150px;
      height: 150px;
      background-color: #fff;
      border: 1px solid #ddd;
      border-radius: 5px;
      box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
      display: flex;
      align-items: center;
      justify-content: center;
      /*animation: timerInOut 2s ease-in-out infinite;*/
    }

    .paused {
      animation-play-state: paused;
    }

    .timer p {
      margin: 0;      
      display: flex;
      align-items: center;
      justify-content: center;
      font-size: 130px;
      color: #ff0000;
    }  
    
    .switch {
      position: relative; 
      display: inline-block; 
      width: 60px; 
      height: 34px;
      margin-top: 5px;
    } 

    .switch input {
      opacity: 0;
      width: 0;
      height: 0;
    }

    .slider {
      position: absolute;
      cursor: pointer;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background-color: #ccc;
      transition: .4s;
      border-radius: 34px;
    }

    .slider:before {
      position: absolute;
      content: "";
      height: 26px;
      width: 26px;
      left: 4px;
      bottom: 4px;
      background-color: white;
      transition: .4s;
      border-radius: 50%;
    }

    input:checked + .slider {
      background-color: #3f51b5;
    }

    input:focus + .slider {
      box-shadow: 0 0 1px #3f51b5;
    }

    input:checked + .slider:before {
      transform: translateX(26px);
    }

    .slider:after {
      color: white;
      display: block;
      position: absolute;
      transform: translate(-50%, -50%);
      top: 50%;
      left: 50%;
      font-size: 10px;
      font-family: Verdana, sans-serif;
    }

    /* Add new animation keyframes */
    @keyframes timerInOut {
      0% {
        transform: scale(1);
        opacity: 1;
      }
      50% {
        transform: scale(0.8);
        opacity: 0.8;
      }
      100% {
        transform: scale(1);
        opacity: 1;
      }
    }

    @media (hover: hover) {
      /* Only apply hover effect on devices that support hover */
      .button:hover{
          background-color: #f0f0f0;
      }      
    }
      
  </style>
</head>
<body>
  <div class="navbar">
    <a href="/" class="link"><button class="back-button">&#8592;</button></a>
    <span class="title" id="navbar-title">Team _: Shot Clock</span>
    <button class="settings-button" onclick="openSettings()">&#9965;</button>
  </div>
  <div class="container">

    <div class="top-half">
      <button class="button left" id="undo" onclick="updateServer(this)">UNDO</button>
      <button class="button right" id="pause" onclick="updateServer(this)">PAUSED</button>
    </div>

    <div class="bottom-half">
      <button class="button big" id="reset" onclick="updateServer(this)">RESET</button>
    </div>   

    <div class="middle-container">
      <div class="timer">
        <p id="timer-text">10</p>
      </div>
      <div style="display: flex; flex-direction: column; align-items: center;">
        <div style="display: flex; align-items: center;">
          <span style="margin-right: 7px; margin-top: 7px; font-size: 20px" id="min-length">10</span>
          <label class="switch">
            <input type="checkbox" id="duration" onchange="updateServer(this)">
            <span class="slider"></span>
          </label>
          <span style="margin-left: 7px; margin-top: 7px; font-size: 20px" id="max-length">15</span>
        </div>
      </div>
    </div>       
  </div>

  <div class="settings-overlay">
    <div class="settings-page" id="settings">
      <div class="navbar" style="margin-bottom: 20px">
        <span>Settings</span>
        <button class="close-button" onclick="closeSettings()">&#88;</button>
      </div>
      <div class="settings-content">
        <label for="name">Name:</label>
        <input type="text" id="name" placeholder="Enter name"><br>
  
        <label for="timer">Shot Clock:</label>
        <input type="number" id="timer" min="0" max="99" value="10"><br>
  
        <label for="players">Players:</label>
        <input type="number" id="players" min="0" max="99" value="0"><br>
  
        <label for="timeouts">Timeouts:</label>
        <input type="number" id="timeouts" min="0" max="99" value="0"><br>
  
        <label for="points">Points:</label>
        <input type="number" id="points" min="0" max="99" value="0"><br>
  
        <label for="color">Color:</label>
        <input type="color" id="color" value="#ff0000"><br>
  
        <div class="slider-container" style="display: flex; align-items: center;">
          <label for="direction">Count Direction:</label>
          <div style="display: flex; align-items: center; transform: scale(0.8);">
            <span style="margin-right: 7px; margin-top: 7px; font-size: 20px" id="down">Dw</span>
            <label class="switch">
              <input type="checkbox" id="direction" onchange="updateServer(this)">
              <span class="slider"></span>
            </label>
            <span style="margin-left: 7px; margin-top: 7px; font-size: 20px" id="up">Up</span>
          </div>
          </div>
        </div>
      </div>
    </div>
  </div>

  <script>  
    var side;
    var source;
    function capitalize(str) {
      return str.charAt(0).toUpperCase() + str.slice(1).toLowerCase();
    }

    // Function to run on page load
    document.addEventListener('DOMContentLoaded', function() {
      load();
    });
    
    function load() {
      const path = window.location.pathname;
      side = path.substring(path.lastIndexOf('/') + 1).toLowerCase().replace(/\s+/g, '');
      source = new EventSource(`/events`);
      source.addEventListener('update', events);
      setTimeout(function() {  
        var xhr = new XMLHttpRequest();
        xhr.open("GET", `/updateServer?side=load`, true);    
        xhr.send();        
      }, 100);

      // Get all input elements
      var inputs = document.querySelectorAll('input');

      // Add event listeners to each input element
      inputs.forEach(function(input) {
        if (input.type != "checkbox") {       
          // When the input receives focus, remove event listener
          input.addEventListener('focus', function() {
              source.removeEventListener('update', events);
          });

          // When the input loses focus, add event listener
          input.addEventListener('blur', function() {
              checkInput(input);              
              source.addEventListener('update', events);
              updateServer(input);
          });
        } 
      });
    }                  

    // Settings page functions
    function openSettings() {
      document.querySelector('.settings-overlay').classList.add('open');
    }
    
    function closeSettings() {
      document.querySelector('.settings-overlay').classList.remove('open');
    }

    const settingsOverlay = document.querySelector('.settings-overlay');
    settingsOverlay.addEventListener('click', (event) => {
      if (event.target === settingsOverlay) {
        closeSettings();
      }
    });

    function checkInput(input) {
        const min = parseInt(input.min);
        const max = parseInt(input.max);
        if (input.value > max) {
            input.value = max;
        } else if (input.value < min) {
            input.value = min;
        }
    }

    // Update the server when an action occurs 
    function updateServer(element) { 
      var str = element;
      var value; 

      // Check if element is undefined
      if (typeof element === "object") {
          str = element.id;
          value = element.value;
      } 

      if (str === "color") {
        value = value.replace("#", "");
        // Convert the hex color code to RGB
        var r = parseInt(value.substring(0, 2), 16);
        var g = parseInt(value.substring(2, 4), 16);
        var b = parseInt(value.substring(4, 6), 16);
        value = `r=${r}g=${g}b=${b}`;
      }

      var xhr = new XMLHttpRequest();
      xhr.open("GET", `/updateServer?side=${side}&type=${str}&value=${value}`, true);    
      xhr.send();
    }

    // Add event listeners 
    function events(e) {
        var response = JSON.parse(e.data);
        var team;

        // Determine what team page to update
        if (side === "team1") {
          team = response.team1;
        } else if (side === "team2") {
          team = response.team2;
        }

        // Update title
        var title;
        if (team.name === "") {
          title = team.side + ": Shot Clock";
        } else {
          title = team.side + ": " + team.name + " Shot Clock";
        }
        document.getElementById("title").innerHTML = title;
        document.getElementById("navbar-title").innerHTML = title;   
        
        // Update name
        document.getElementById("name").value = team.name;
        
        // Update pause
        var paused = team.paused;
        var text = paused ? 'RESUME' : 'PAUSE';
        //if (paused) {
        //  timer.classList.add('paused');
        //} else {
        //  timer.classList.remove('paused');
        //}
        document.getElementById("pause").innerHTML = text;               

        // Update shot clock
        document.getElementById("timer-text").innerHTML = team.displayedCount;
        document.getElementById("timer").value = team.displayedCount;

        // Update min length
        document.getElementById("min-length").innerHTML = response.other.minimumLength;

        // Update max length
        document.getElementById("max-length").innerHTML = response.other.maximumLength;

        // Update length
        if (team.countDuration === "minimum") {
          document.getElementById("duration").checked = false; 
        } else if (team.countDuration === "maximum") {
          document.getElementById("duration").checked = true;
        }              
        
        // Update points
        document.getElementById("points").value = team.points;
        
        // Update players
        document.getElementById("players").value = team.playersLeft;

        // Update timeouts
        document.getElementById("timeouts").value = team.timeoutsLeft;

        // Update color
        document.getElementById("timer-text").style.color = team.color.hex;
        document.getElementById("color").value = team.color.hex;

        // Update direction
        if (team.countDirection === "down") {
          document.getElementById("direction").checked = false;
        } else if (team.countDirection === "up") {
          document.getElementById("direction").checked = true;
        }        

      };  

  </script>
</body>
</html>
)rawliteral";

const char settings_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Settings</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 0;
      background-color: #f5f5f5;
    }

    .navbar {
      background-color: #3f51b5;
      color: white;
      font-size: 20px;
      height: 50px;
      display: flex;
      align-items: center;
      justify-content: space-between;
      padding: 0px 20px;
      box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);
    }

    .navbar button {
      font-weight: bold;
      font-size: 20px;
      cursor: pointer;
      background: none;
      border: none;
      padding: 0;
      color: white;
    }

    .settings-page {
      margin-top: 20px;
      transition: all 0.3s ease;
    }

    .category {
      cursor: pointer;
      font-weight: bold;
      display: flex;
      align-items: center;
      padding: 10px;
      border-radius: 5px;
      transition: all 0.3s ease;
      margin-bottom: 10px;
      margin-left: 20px;
      margin-right: 20px;
      background-color: #fff;
      box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);
    }

    .category-arrow {
      margin-left: auto;
      transition: transform 0.3s;
    }

    .category.open .category-arrow {
      transform: rotate(90deg);
    }

    .options {
      display: none;
      margin-top: 10px;
      margin-left: 40px;
      margin-right: 20px;
      transition: all 0.3s ease;
    }

    .category.open + .options {
      display: block;
    }

    .options p {
      margin: 0;
      padding: 10px;
      background-color: #fff;
      border-radius: 5px;
      box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);
      margin-bottom: 10px;
    }    

    .switch {
      position: relative; 
      display: inline-block; 
      width: 60px; 
      height: 34px;
      margin-top: 5px;
    } 

    .switch input {
      opacity: 0;
      width: 0;
      height: 0;
    }

    .slider {
      position: absolute;
      cursor: pointer;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background-color: #ccc;
      transition: .4s;
      border-radius: 34px;
    }

    .slider:before {
      position: absolute;
      content: "";
      height: 26px;
      width: 26px;
      left: 4px;
      bottom: 4px;
      background-color: white;
      transition: .4s;
      border-radius: 50%;
    }

    input:checked + .slider {
      background-color: #3f51b5;
    }

    input:focus + .slider {
      box-shadow: 0 0 1px #3f51b5;
    }

    input:checked + .slider:before {
      transform: translateX(26px);
    }

    .slider:after {
      color: white;
      display: block;
      position: absolute;
      transform: translate(-50%, -50%);
      top: 50%;
      left: 50%;
      font-size: 10px;
      font-family: Verdana, sans-serif;
    }

    label {
      font-size: 20px;
      font-weight: bold;
      margin-bottom: 10px;
    }

    .changebtn {
      background-color: #fff;
      border: none;
      color: black;
      padding: 10px 20px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 20px;
      cursor: pointer;
      width: 200px;
      border-radius: 5px;
      box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);
      transition: all 0.3s ease;
    }

    table {
      display: table;
      text-align: center;
      font-size: 20px;
      font-weight: bold;
      margin-bottom: 10px;
    }

    .number-input {
      display: flex;
      background-color: #fff;
      justify-content: center;
      align-items: center;
      border: none;
      border-radius: 5px;
      padding: 10px;
      box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);
      transition: all 0.3s ease;
    }

    .number-input input {
      width: 50px;
      height: 30px;
      background: none;
      text-align: center;
      border: none;
      font-size: 20px;
      -moz-appearance: textfield;
      appearance: textfield;
      outline: none;
    }

    .number-input input::-webkit-inner-spin-button,
    .number-input input::-webkit-outer-spin-button {
      -webkit-appearance: none;
      margin: 0;
    }

    .number-input button {
      width: 30px;
      height: 30px;
      color: black;
      cursor: pointer;
      border: none;
      padding: 0px;
      font-size: 25px;
      display: flex;
      justify-content: center;
      align-items: center;
      background-color: #fff;
      transition: all 0.3s ease;
    }
    
    input[type=number] {
      width: 50px;
    } 

    .swap {
      background-color: #fff;
      border: none;
      color: black;
      padding: 10px 20px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 20px;
      cursor: pointer;
      width: 100px;
      border-radius: 5px;
      box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);
      transition: all 0.3s ease;
      margin-bottom: 10px;
    }

    .navbar .home {
      font-size: 30px;
    }

    @media (hover: hover) {
      /* Only apply hover effect on devices that support hover */
      .category:hover {
        background-color: #f0f0f0;
      }
      .options p:hover {
        background-color: #f0f0f0;
      }
    }  
   
  </style>
</head>
<body>
  <div class="navbar">
    <button class="back-button" onclick="window.history.back()">&#8592;</button>
    <span class="title">Settings</span>
    <a href="/" class="link"><button class="home">&#8962;</button></a>
  </div>  

  <div class="settings-page">
    <div class="category open" onclick="toggleOptions('main', this)">
      <span>Game Clock / Main</span>
      <span class="category-arrow">&#10148;</span>
    </div>
    <div id="main-options" class="options">
      <label>Game Clock:</label>
      <input type="number" id="gameclock-minutes" min="0" max="99" value="25" style="width: 40px;"> :
      <input type="number" id="gameclock-seconds" min="0" max="59" value="0" style="width: 40px;"><br>

      <label for="gameclock-half">Half:</label>
        <select id="gameclock-half" onchange="updateServer(this)">
            <option value="1st">1st</option>
            <option value="2nd">2nd</option>
            <option value="OT">OT</option>
        </select><br>

      <label for="gameclock-color">Color:</label>
      <input type="color" id="gameclock-color" value="#ff0000"><br>

      <table>
        <tbody>
        <tr>
          <td>Min:</td>     
          <td><div class="number-input">
            <button class="changebtn" onclick="decrement('min')">&#706;</button>
            <input type="number" id="min" name="min" min="0" max="15" value="10">
            <button class="changebtn" onclick="increment('min')">&#707;</button>              
          </div><td>
        </tr> 

        <tr>
          <td>Max:</td>     
          <td><div class="number-input">
            <button class="changebtn" onclick="decrement('max')">&#706;</button>
            <input type="number" id="max" name="max" min="0" max="99" value="15">
            <button class="changebtn" onclick="increment('max')">&#707;</button>              
          </div><td>
        </tr>
      </tbody>
      </table>

      <label for="swap">Change Sides:</label>
      <button class="swap" id="gameclock-swap" onclick="updateServer(this)">Swap</button>
        
    </div>  

    <div class="category open" onclick="toggleOptions('team1', this)">
      <span id="team1-title">Team 1</span>
      <span class="category-arrow">&#10148;</span>
    </div>
    <div id="team1-options" class="options">
      <label for="team1-name">Name:</label>
      <input type="text" id="team1-name" placeholder="Enter name"><br>

      <label for="team1-timer">Shot Clock:</label>
      <input type="number" id="team1-timer" min="0" max="99" value="10"><br>

      <label for="team1-players">Players:</label>
      <input type="number" id="team1-players" min="0" max="99" value="0"><br>

      <label for="team1-timeouts">Timeouts:</label>
      <input type="number" id="team1-timeouts" min="0" max="99" value="0"><br>

      <label for="team1-points">Points:</label>
      <input type="number" id="team1-points" min="0" max="99" value="0"><br>

      <label for="team1-color">Color:</label>
      <input type="color" id="team1-color" value="#ff0000"><br>

      <div class="slider-container" style="display: flex; align-items: center;">
        <label for="team1-direction">Count Direction:</label>
        <div style="display: flex; align-items: center; transform: scale(0.8);">
          <span style="margin-right: 7px; margin-top: 7px; font-size: 20px" id="down">Dw</span>
          <label class="switch">
            <input type="checkbox" id="team1-direction" onchange="updateServer(this)">
            <span class="slider"></span>
          </label>
          <span style="margin-left: 7px; margin-top: 7px; font-size: 20px" id="up">Up</span>
        </div>
        </div>
      </div>
    </div>

    <div class="category open" onclick="toggleOptions('team2', this)">
      <span id="team2-title">Team 2</span>
      <span class="category-arrow">&#10148;</span>
    </div>
    <div id="team2-options" class="options">
      <label for="team2-name">Name:</label>
      <input type="text" id="team2-name" placeholder="Enter name"><br>

      <label for="team2-timer">Shot Clock:</label>
      <input type="number" id="team2-timer" min="0" max="99" value="10"><br>

      <label for="team2-players">Players:</label>
      <input type="number" id="team2-players" min="0" max="99" value="0"><br>

      <label for="team2-timeouts">Timeouts:</label>
      <input type="number" id="team2-timeouts" min="0" max="99" value="0"><br>

      <label for="team2-points">Points:</label>
      <input type="number" id="team2-points" min="0" max="99" value="0"><br>

      <label for="team2-color">Color:</label>
      <input type="color" id="team2-color" value="#ff0000"><br>

      <div class="slider-container" style="display: flex; align-items: center;">
        <label for="team2-direction">Count Direction:</label>
        <div style="display: flex; align-items: center; transform: scale(0.8);">
          <span style="margin-right: 7px; margin-top: 7px; font-size: 20px" id="down">Dw</span>
          <label class="switch">
            <input type="checkbox" id="team2-direction" onchange="updateServer(this)">
            <span class="slider"></span>
          </label>
          <span style="margin-left: 7px; margin-top: 7px; font-size: 20px" id="up">Up</span>
        </div>
        </div>
      </div>
    </div>
    
    <div class="category open" onclick="toggleOptions('other', this)">
      <span>Other</span>
      <span class="category-arrow">&#10148;</span>
    </div>
    <div id="other-options" class="options">
      <div class="category open" onclick="toggleOptions('loadpre', this)">
        <span>Load Previous Matches</span>
        <span class="category-arrow">&#10148;</span>
      </div>
      <div id="loadpre-options" class="options">
        <p>Match 1</p>
        <p>Match 2</p>
        <p>Match 3</p>
      </div>
      <div class="category open" onclick="toggleOptions('wifi', this)">
        <span>Edit WiFi Credentials</span>
        <span class="category-arrow">&#10148;</span>
      </div>
      <div id="wifi-options" class="options">
        <p>Network Name: <span id="network-name">NCDAShotclock</span></p>
        <p>Password: <span id="network-password">Dodgeball</span></p>
      </div>
      <div class="category open" onclick="toggleOptions('update', this)">
        <span>Update</span>
        <span class="category-arrow">&#10148;</span>
      </div>
      <div id="update-options" class="options">
        <p>User Interface</p>
        <p>Software</p>
      </div>
      <div class="category open" onclick="toggleOptions('info', this)">
        <span>Info</span>
        <span class="category-arrow">&#10148;</span>
      </div>
      <div id="info-options" class="options">
        <p>User Interface: V2.1.0-alpha</p>
        <p>Software: V1.1.0-alpha</p>
      </div>
    </div>
  </div>

  <script>
    var source;

    // Function to run on page load
    document.addEventListener('DOMContentLoaded', function() {
      load();
    });

    function load() {
        source = new EventSource(`/events`);
        source.addEventListener('update', events);
        setTimeout(function() {
          var xhr = new XMLHttpRequest();
          xhr.open("GET", `/updateServer?side=load`, true);    
          xhr.send();        
        }, 100);

        // Get all input elements
        var inputs = document.querySelectorAll('input');

        // Add event listeners to each input element
        inputs.forEach(function(input) {
        if (input.type != "checkbox") {       
            // When the input receives focus, remove event listener
            input.addEventListener('focus', function() {
                source.removeEventListener('update', events);
            });

            // When the input loses focus, add event listener
            if (input.id === "min" || input.id === "max") {
              input.addEventListener('blur', function() {            
                source.addEventListener('update', events);
                checkMinMax();
              }); 
            } else {
              input.addEventListener('blur', function() {
                checkInput(input);                
                source.addEventListener('update', events);
                updateServer(input);
              });              
            }
            
        } 
        });
    } 

    function toggleOptions(categoryId, element) {
      var options = document.getElementById(categoryId + '-options');
      var category = element.classList;

      options.style.display = options.style.display === 'none' ? 'block' : 'none';
      category.toggle('open');
    }

    var devicesElement = document.querySelector('.devices');
    var settingsPage = document.querySelector('.settings-page');

    devicesElement.addEventListener('click', function() {
      settingsPage.classList.toggle('open');
    });

    function checkInput(input) {
      const min = parseInt(input.min);
      const max = parseInt(input.max);
      if (input.value > max) {
          input.value = max;
      } else if (input.value < min) {
          input.value = min;
      }
    }

    function increment(inputId) {
      var input = document.getElementById(inputId);
      input.stepUp();
      checkMinMax();
    }

    function decrement(inputId) {
      var input = document.getElementById(inputId);
      input.stepDown();
      checkMinMax();
    }
    
    // Check min/max 
    function checkMinMax() {
      const minInput = document.getElementById('min');
      const maxInput = document.getElementById('max');   
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
      updateMinMax(min, max);
    }

    // Update min max on the server
    function updateMinMax(min, max) {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", `/updateServer?side=index&min=${min}&max=${max}`, true);          
      xhr.send();
    }

    // Update the server when an action occurs 
    function updateServer(element) {
      var str = element;
      var value;
      var side;   

      // Check if element is undefined
      if (typeof element === "object") {
        if (element.id.includes("-")) {
          var beforeWordIndex = element.id.indexOf("-");
          side = element.id.substring(0, beforeWordIndex);
          str = element.id.substring(beforeWordIndex + 1);
        } else {
            str = element.id;
        }            
        value = element.value;
      } 

      if (str === "color") {
        value = value.replace("#", "");
        // Convert the hex color code to RGB
        var r = parseInt(value.substring(0, 2), 16);
        var g = parseInt(value.substring(2, 4), 16);
        var b = parseInt(value.substring(4, 6), 16);
        value = `r=${r}g=${g}b=${b}`;
      }

      var xhr = new XMLHttpRequest();
      xhr.open("GET", `/updateServer?side=${side}&type=${str}&value=${value}`, true);    
      xhr.send();
    }

    // Add event listeners 
    function events(e) {
      var response = JSON.parse(e.data);
      
      var gameClock = response.gameClock;
      var team1 = response.team1;
      var team2 = response.team2;

      // Game clock / main
      // Update timer
      var minutes = gameClock.minutes;
      var seconds = gameClock.seconds;

      document.getElementById('gameclock-minutes').value = minutes;
      document.getElementById('gameclock-seconds').value = seconds;

      // Update half 
      document.getElementById('gameclock-half').value = gameClock.half;

      // Update game clock color
      document.getElementById("gameclock-color").value = gameClock.color.hex;

      // Update min/max
      document.getElementById('min').value = response.other.minimumLength;
      document.getElementById('max').value = response.other.maximumLength;

      // Team 1
      // Update name
      if (team1.name === "") {
        document.getElementById("team1-title").innerHTML = team1.side;
      } else {
        document.getElementById("team1-title").innerHTML = team1.side + ": " + team1.name;
      }
      document.getElementById("team1-name").value = team1.name;

      // Update shot clock
      document.getElementById("team1-timer").value = team1.displayedCount;

      // Update points
      document.getElementById("team1-points").value = team1.points;
        
      // Update players
      document.getElementById("team1-players").value = team1.playersLeft;

      // Update timeouts
      document.getElementById("team1-timeouts").value = team1.timeoutsLeft;

      // Update color
      document.getElementById("team1-color").value = team1.color.hex;

      // Update direction
      if (team1.countDirection === "down") {
        document.getElementById("team1-direction").checked = false;
      } else if (team1.countDirection === "up") {
        document.getElementById("team1-direction").checked = true;
      }

      // Team 2
      // Update name
      if (team2.name === "") {
        document.getElementById("team2-title").innerHTML = team2.side;
      } else {
        document.getElementById("team2-title").innerHTML = team2.side + ": " + team2.name;
      }
      document.getElementById("team2-name").value = team2.name;

      // Update shot clock
      document.getElementById("team2-timer").value = team2.displayedCount;

      // Update points
      document.getElementById("team2-points").value = team2.points;
        
      // Update players
      document.getElementById("team2-players").value = team2.playersLeft;

      // Update timeouts
      document.getElementById("team2-timeouts").value = team2.timeoutsLeft;

      // Update color
      document.getElementById("team2-color").value = team2.color.hex;

      // Update direction
      if (team2.countDirection === "down") {
        document.getElementById("team2-direction").checked = false;
      } else if (team2.countDirection === "up") {
        document.getElementById("team2-direction").checked = true;
      }

    };  

  </script>
</body>
</html>
)rawliteral";

const char practice_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Practice Mode</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 0;
      background-color: #f5f5f5;
    }

    .navbar {
      background-color: #3f51b5;
      color: white;
      font-size: 20px;
      height: 50px;
      display: flex;
      align-items: center;
      justify-content: space-between;
      padding: 0px 20px;
      box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);
    }

    .navbar button {
      font-weight: bold;
      font-size: 20px;
      cursor: pointer;
      background: none;
      border: none;
      padding: 0;
      color: white;
    }

    .container {
        text-align: center;
        padding: 20px;
    }
    
    .table {
        width: 100%;
        margin-bottom: 10px;
        border-collapse: collapse;
    }

    td, th {
      padding: 5px;
      text-align: center;      
    }

    .team-name {
        font-size: 25px;
        padding: 5px;
        width: 100%;
    }
    
    .team-timer {
        font-size: 70px;
        color: #ff0000;
        padding: 5px;
        width: 100%;
    }
    
    .name-container, .timer-container, .slider-container {
        display: flex;
        justify-content: center;
        align-items: center;
        border: 1px solid #ccc;        
    }   

    .table button {
        background-color: #fff;
        border: none;
        color: black;
        padding: 5px 20px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 50px;
        cursor: pointer;
        width: 100%;
        border-radius: 5px;
        box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);
        transition: all 0.3s ease;
    }

    .switch {
        position: relative; 
        display: inline-block; 
        width: 60px; 
        height: 34px;
        margin-top: 5px;
    } 
  
    .switch input {
        opacity: 0;
        width: 0;
        height: 0;
    }
  
    .slider {
        position: absolute;
        cursor: pointer;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #ccc;
        transition: .4s;
        border-radius: 34px;
    }
  
    .slider:before {
        position: absolute;
        content: "";
        height: 26px;
        width: 26px;
        left: 4px;
        bottom: 4px;
        background-color: white;
        transition: .4s;
        border-radius: 50%;
    }
  
    input:checked + .slider {
        background-color: #3f51b5;
    }
  
    input:focus + .slider {
        box-shadow: 0 0 1px #3f51b5;
    }
  
    input:checked + .slider:before {
        transform: translateX(26px);
    }
  
    .slider:after {
        color: white;
        display: block;
        position: absolute;
        transform: translate(-50%, -50%);
        top: 50%;
        left: 50%;
        font-size: 10px;
        font-family: Verdana, sans-serif;
    }

    .timer {
        background-color: #f5f5f5;
        border: 1px solid #ddd;
        padding: 10px;
        font-size: 70px;        
    }

    .timer span {
        color: #ff0000;
    }
    
    .buttons {
        margin-top: 10px;
    }

    .buttons button {
        background-color: #fff;
        border: none;
        color: black;
        padding: 10px 20px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 50px;
        cursor: pointer;
        width: 100%;
        border-radius: 5px;
        box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);
        transition: all 0.3s ease;
        margin-bottom: 10px;
    }

    .popup {
        display: none;
        position: fixed;
        top: 0;
        left: 0;
        width: 100%;
        height: 100%;
        background-color: rgba(0, 0, 0, 0.5);
    }
    
    .popup-content {
        position: absolute;
        top: 50%;
        left: 50%;
        transform: translate(-50%, -50%);
        background-color: #f5f5f5;
        box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
        border-radius: 4px;
        width: 80%; /* Adjust as needed */
        max-width: 500px; /* Adjust as needed */
    }      
    
    .popup-navbar {
        display: flex;
        justify-content: space-between;
        align-items: center;
        padding-bottom: 10px;
        border-bottom: 1px solid #ddd;
    }   

    .popup-options {
        display: flex;
        flex-direction: column;
        align-items: center;
        justify-content: center;
        padding: 10px;
    }
    
    .popup-options button {
        background-color: #fff;
        border: none;
        color: black;
        padding: 10px 20px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 20px;
        cursor: pointer;
        width: 90%;
        border-radius: 5px;
        box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.1);
        transition: all 0.3s ease;
        margin-bottom: 10px;
        margin-top: 10px;
    }

    @media (max-width: 600px) {
        /* Adjust the breakpoint as needed */  
        .table button {
          font-size: 30px;
        }

        .team-name {
            font-size: 15px;
        }
    }

    @media (hover: hover) {
        /* Only apply hover effect on devices that support hover */
        .table button:hover {
            background-color: #f0f0f0;
        }   
        .buttons button:hover {
            background-color: #f0f0f0;
        }
        .popup-options button:hover {
            background-color: #f0f0f0;
        }
    }

  </style>
</head>
<body>
  <div class="navbar">
    <button class="back-button" onclick="window.history.back()">&#8592;</button>
    <span class="title">Practice Mode</span>
    <a href="/settings" class="link"><button class="settings-button">&#9965;</button></a>
  </div>  

  <div class="container">
    <table class="table">
        <tbody>
        <tr>
            <td>
            <div class="name-container">
                <span id="team1-name" class="team-name">Team 1</span>
            </div>
            </td>
            <td>
            <div class="name-container">
                <span id="team2-name" class="team-name">Team 2</span>
            </div>
            </td>
        </tr>
        <tr>
            <td>
            <div class="timer-container">
                <span id="team1-timer" class="team-timer">10</span>
            </div>
            </td>
            <td>
            <div class="timer-container">
                <span id="team2-timer" class="team-timer">10</span>
            </div>
            </td>
        </tr>  
        <tr>
            <td><button id="team1-undo" onclick="updateServer(this)">UNDO</button></td>
            <td><button id="team2-undo" onclick="updateServer(this)">UNDO</button></td>
        </tr>
        <tr>
            <td><button id="team1-pause" onclick="updateServer(this)">PAUSE</button></td>
            <td><button id="team2-pause" onclick="updateServer(this)">PAUSE</button></td>
        </tr>
        <tr>
            <td><button id="team1-reset" onclick="updateServer(this)">RESET</button></td>
            <td><button id="team2-reset" onclick="updateServer(this)">RESET</button></td>
        </tr>
        <tr>
            <td>
            <div  class="slider-container" style="flex-direction: column; padding-bottom: 5px">
                <div style="display: flex; align-items: center;">
                    <span style="margin-right: 7px; margin-top: 7px; font-size: 20px" id="team1-min-length">10</span>
                    <label class="switch">
                    <input type="checkbox" id="team1-duration" onchange="updateServer(this)">
                    <span class="slider"></span>
                    </label>
                    <span style="margin-left: 7px; margin-top: 7px; font-size: 20px" id="team1-max-length">15</span>
                </div>
            </div>
            </td>
            <td>
            <div  class="slider-container" style="flex-direction: column; padding-bottom: 5px">
                <div style="display: flex; align-items: center;">
                    <span style="margin-right: 7px; margin-top: 7px; font-size: 20px" id="team2-min-length">10</span>
                    <label class="switch">
                    <input type="checkbox" id="team2-duration" onchange="updateServer(this)">
                    <span class="slider"></span>
                    </label>
                    <span style="margin-left: 7px; margin-top: 7px; font-size: 20px" id="team2-max-length">15</span>
                </div>
            </div>
            </td>
        </tr>
    </tbody>
    </table>
    <div class="timer">
        <span id="gameclock-text">25:00</span>
    </div>        
    <div class="buttons">
        <button id="time-button" onclick="button(this)">TIMEOUT</button><br>
        <button id="point-button" onclick="button(this)">START POINT</button>
    </div>
    <div>
        <p style="display: none" id="team1-timeouts">0</p>
        <p style="display: none" id="team2-timeouts">0</p>
    </div>

    <div id="popup" class="popup">
        <div class="popup-content">
            <div class="navbar" id="popup-navbar"></div>
            <div class="popup-options" id="popup-options"></div>
        </div>
    </div> 

  </div>

  <script>
    var source;

    // Function to run on page load
    document.addEventListener('DOMContentLoaded', function() {
        load();
    });
  
    function load() {
        source = new EventSource(`/events`);
        source.addEventListener('update', events);
        setTimeout(function() {  
            var xhr = new XMLHttpRequest();
            xhr.open("GET", `/updateServer?side=load`, true);    
            xhr.send();      
        }, 100);        
    } 

    function button(element) {
        const type = element.innerHTML.toLowerCase().replace(" ", ""); 

        // If the button is resume or startpoint, update server and return
        if (type === "resume") {
            if (document.getElementById('point-button').innerHTML === "START POINT") {
                window.alert("Please start the point before resuming the game.");
                return;
            }
            updateServer(type);
        } else if (type === "startpoint") {
            updateServer(type);
        } else if (type == "endtimeout") {
            updateServer(type);
        }
        else {
            showPopup(element);
        }
    }

    function showPopup(element) {
        updateServer("pause");

        const popupNavbar = document.getElementById('popup-navbar');
        const popupOptions = document.getElementById('popup-options');        
        const type = element.innerHTML.toLowerCase().replace(" ", "");             

        // Popup navbar
        popupNavbar.innerHTML = `<span>${element.innerHTML}</span>`;
        popupNavbar.innerHTML += `<button class="close" id="${type}" value="close" onclick="closePopup(event.target)">&#88;</button>`;
    
        // Popup content
        popupOptions.innerHTML = '';
        var options = [];
        
        if (type === 'timeout') {
            team1Timeouts = document.getElementById('team1-timeouts').innerHTML;
            team2Timeouts = document.getElementById('team2-timeouts').innerHTML;
            if (team1Timeouts > 0) {
                options.push('Team 1');
            }
            if (team2Timeouts > 0) {
                options.push('Team 2');
            }
            options.push('Official');                                           
        } else if (type === 'endpoint') {
            options = ['Team 1', 'Team 2', 'Draw'];            
        }

        options.forEach(option => {                
            const button = document.createElement('button');
            button.innerText = option;
            button.id = type;
            button.value = option.replace(" ", "").toLowerCase();
            button.onclick = () => closePopup(button);
            popupOptions.appendChild(button);
        });        
        document.getElementById('popup').style.display = 'block';
    }    
    
    function closePopup(element) {        
        document.getElementById('popup').style.display = 'none';
        updateServer(element);
    }

    // Update the server when an action occurs 
    function updateServer(element) {
        var str = element;
        var value;
        var side = "gameclock";   
  
        // Check if element is undefined
        if (typeof element === "object") {
            if (element.id.includes("-")) {
                var beforeWordIndex = element.id.indexOf("-");
                side = element.id.substring(0, beforeWordIndex);
                str = element.id.substring(beforeWordIndex + 1);
            } else {
                str = element.id;
            }            
            value = element.value;
        } 
    
        var xhr = new XMLHttpRequest();
        xhr.open("GET", `/updateServer?side=${side}&type=${str}&value=${value}`, true);    
        xhr.send();
      }

    function events(e) {
        var response = JSON.parse(e.data);

        var gameClock = response.gameClock;
        var team1 = response.team1;
        var team2 = response.team2;

        // Update team names
        if (team1.name === "") { // Team 1
            document.getElementById("team1-name").innerHTML = team1.side;
        } else {
            document.getElementById("team1-name").innerHTML = team1.side + ": " + team1.name;
        }

        if (team2.name === "") { // Team 2
            document.getElementById("team2-name").innerHTML = team2.side;        
        } else {
            document.getElementById("team2-name").innerHTML = team2.side + ": " + team2.name;
        }

        // Update team shot clockers
        document.getElementById('team1-timer').innerHTML = team1.displayedCount;
        document.getElementById('team2-timer').innerHTML = team2.displayedCount;

        // Update color
        document.getElementById("team1-timer").style.color = team1.color.hex;
        document.getElementById("team2-timer").style.color = team2.color.hex;

        // Update pause
        var paused = team1.paused; // Team 1
        var text = paused ? 'RESUME' : 'PAUSE';
        document.getElementById("team1-pause").innerHTML = text;

        paused = team2.paused; // Team 2
        text = paused ? 'RESUME' : 'PAUSE';
        document.getElementById("team2-pause").innerHTML = text;

        // Update length
        if (team1.countDuration === "minimum") { // Team 1
            document.getElementById("team1-duration").checked = false; 
        } else if (team1.countDuration === "maximum") {
            document.getElementById("team1-duration").checked = true;
        }
        
        if (team2.countDuration === "minimum") { // Team 2
            document.getElementById("team2-duration").checked = false; 
        } else if (team2.countDuration === "maximum") {
            document.getElementById("team2-duration").checked = true;
        }

        // Update min length
        document.getElementById("team1-min-length").innerHTML = response.other.minimumLength;
        document.getElementById("team2-min-length").innerHTML = response.other.minimumLength;

        // Update max length
        document.getElementById("team1-max-length").innerHTML = response.other.maximumLength;
        document.getElementById("team2-max-length").innerHTML = response.other.maximumLength;

        // Update display
        var dispMinutes = gameClock.displayedMinutes;
        var dispSeconds = gameClock.displayedSeconds;

        if (dispMinutes < 10) {
            dispMinutes = "0" + String(dispMinutes);
        }
        if (dispSeconds < 10) {
            dispSeconds = "0" + String(dispSeconds);
        }

        document.getElementById('gameclock-text').innerHTML = dispMinutes + ":" + dispSeconds;        

        // Update color
        document.getElementById("gameclock-text").style.color = gameClock.color.hex;
        
        // Update buttons
        var text = gameClock.paused ? 'RESUME' : 'TIMEOUT';
        if (gameClock.timeout) {
            text = 'END TIMEOUT'
        }
        document.getElementById('time-button').innerHTML = text;

        text = gameClock.middleOfPoint ? 'ENDPOINT' : 'START POINT';
        document.getElementById('point-button').innerHTML = text;
        
        // Update timeouts
        document.getElementById('team1-timeouts').innerHTML = team1.timeoutsLeft;
        document.getElementById('team2-timeouts').innerHTML = team2.timeoutsLeft;     
    }   

  </script>
</body>
</html>

)rawliteral";
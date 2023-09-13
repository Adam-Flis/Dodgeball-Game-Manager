import asyncio
import websockets
import json
import aiofiles
import os

TEAM1 = "team1"
TEAM2 = "team2"
GAMECLOCK = "gameClock"

SHOTCLOCK = "trueCount"
PLAYERS = "playersLeft"
TIMEOUTS = "timeoutsLeft"
POINTS = "points"

MINUTES = "minutes"
SECONDS = "seconds"

path = ""

async def getValue(json, parent, key):
    try:
        return json[parent][key]
    except:
        return None

async def writeToTeam(path, team, type, value):
    try:
        async with aiofiles.open(f"{path}/{team}_{type}.txt", mode="w") as f:
            await f.write(f"{value}")
    except Exception as e:
        print(f"Error writing to file: {e}")

async def writeToClock(path, gameclock, value1, value2):
    try:
        async with aiofiles.open(f"{path}/{gameclock}.txt", mode="w") as file:
            if value2 < 10:
                await file.write(f"{value1}:0{value2}")
            else:
                await file.write(f"{value1}:{value2}")
    except Exception as e:
        print(f"Error writing to file: {e}")

async def receive_updates():
    uri = "ws://192.168.4.1/ws"  # Replace with the WebSocket URL of your server

    async with websockets.connect(uri) as websocket:
        
        while True:
            try:
                # Wait for updates from the server
                update_data = await websocket.recv()
                #print(f"Received update: {update_data}")
                                
                # Convert the update data to a Python dictionary object
                data = json.loads(update_data)
                
                # Game clock
                min = await getValue(data, GAMECLOCK, MINUTES)
                sec = await getValue(data, GAMECLOCK, SECONDS)      
                                
                await writeToClock(path, GAMECLOCK, min, sec)  
                
                # Shot clock
                team1Clock = await getValue(data, TEAM1, SHOTCLOCK)
                team2Clock = await getValue(data, TEAM2, SHOTCLOCK)
                
                await writeToTeam(path, TEAM1, SHOTCLOCK, team1Clock)
                await writeToTeam(path, TEAM2, SHOTCLOCK, team2Clock)
                
                # Points
                team1Points = await getValue(data, TEAM1, POINTS)
                team2Points = await getValue(data, TEAM2, POINTS)
                
                await writeToTeam(path, TEAM1, POINTS, team1Points)
                await writeToTeam(path, TEAM2, POINTS, team2Points)
                
                # Timeouts
                team1Timeouts = await getValue(data, TEAM1, TIMEOUTS)
                team2Timeouts = await getValue(data, TEAM2, TIMEOUTS)
                
                await writeToTeam(path, TEAM1, TIMEOUTS, team1Timeouts)
                await writeToTeam(path, TEAM2, TIMEOUTS, team2Timeouts)
                
                # Players
                team1Players = await getValue(data, TEAM1, PLAYERS)
                team2Players = await getValue(data, TEAM2, PLAYERS)
                
                await writeToTeam(path, TEAM1, PLAYERS, team1Players)
                await writeToTeam(path, TEAM2, PLAYERS, team2Players)
                              
                await asyncio.sleep(0.1)  # Wait for 0.1 seconds before requesting another update                
            except websockets.ConnectionClosed:
                print("Connection to the server closed. Reconnecting...")
                websocket = await websockets.connect(uri)
                
                

if __name__ == "__main__":
    
    #"C://Users//adamf//Desktop//data"
    
    while os.path.exists(path) == False:  
        path = input("Enter the exact path to write files that OBS will read from (or press Enter to use the current directory): ").strip()

    print("Press ctrl-c to stop the program.")
    asyncio.get_event_loop().run_until_complete(receive_updates())
        
        



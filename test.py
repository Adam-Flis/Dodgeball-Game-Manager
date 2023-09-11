import asyncio
import websockets

async def receive_updates():
    uri = "ws://192.168.4.1/ws"  # Replace with the WebSocket URL of your server

    async with websockets.connect(uri) as websocket:
        while True:
            try:
                # Wait for updates from the server
                update_data = await websocket.recv()
                print(f"Received update: {update_data}")
                
                # Process the update data as needed
                # You can update your application's UI or perform other actions here
            except websockets.ConnectionClosed:
                print("Connection to the server closed. Reconnecting...")
                await asyncio.sleep(5)  # Reconnect after 5 seconds

if __name__ == "__main__":
    asyncio.get_event_loop().run_until_complete(receive_updates())



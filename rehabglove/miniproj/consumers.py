import json
from channels.generic.websocket import AsyncWebsocketConsumer


class ESP32Consumer(AsyncWebsocketConsumer):
    async def connect(self):
        await self.accept()

    async def disconnect(self, close_code):
        pass

    async def receive(self, text_data):
        data = json.loads(text_data)
        x = data["x"]
        y = data["y"]
        z = data["z"]

        # Here, add logic to handle the data, such as sending it to the ESP32

        await self.send(
            text_data=json.dumps(
                [{"status": "Data received"}, {"average": (x + y + z) / 3}]
            )
        )

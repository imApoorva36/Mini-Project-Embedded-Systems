# consumers.py

import json
from channels.generic.websocket import AsyncWebsocketConsumer


class ArduinoConsumer(AsyncWebsocketConsumer):
    async def connect(self):
        self.room_name = "arduino"
        self.room_group_name = f"ws_{self.room_name}"

        # Join the group
        await self.channel_layer.group_add(
            self.room_group_name, self.channel_name
        )

        # Accept WebSocket connection
        await self.accept()

    async def disconnect(self, close_code):
        # Leave the group
        await self.channel_layer.group_discard(
            self.room_group_name, self.channel_name
        )

    async def receive(self, text_data):
        data = json.loads(text_data)
        threshold = data["threshold"]  # Get the threshold value

        # Send a message back to the WebSocket
        await self.send(text_data=json.dumps({"threshold": threshold}))

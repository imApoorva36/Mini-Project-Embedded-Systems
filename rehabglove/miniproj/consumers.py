import json
from channels.generic.websocket import AsyncWebsocketConsumer


class ArduinoConsumer(AsyncWebsocketConsumer):
    async def connect(self):
        self.room_group_name = "arduino_control"

        # Join the WebSocket group
        await self.channel_layer.group_add(
            self.room_group_name, self.channel_name
        )

        # Accept the WebSocket connection
        await self.accept()

    async def disconnect(self, close_code):
        # Leave the WebSocket group
        await self.channel_layer.group_discard(
            self.room_group_name, self.channel_name
        )

    # Receive message from WebSocket (from frontend)
    async def receive(self, text_data):
        text_data_json = json.loads(text_data)
        threshold = text_data_json["threshold"]

        # Send the threshold to the WebSocket group (for ESP32)
        await self.channel_layer.group_send(
            self.room_group_name,
            {"type": "send_threshold", "threshold": threshold},
        )

    # Receive message from WebSocket group (ESP32)
    async def send_threshold(self, event):
        threshold = event["threshold"]

        # Send threshold to the WebSocket (frontend)
        await self.send(text_data=json.dumps({"threshold": threshold}))

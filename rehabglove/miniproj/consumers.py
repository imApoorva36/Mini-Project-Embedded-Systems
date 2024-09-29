import json
from channels.generic.websocket import WebsocketConsumer


class ESP32Consumer(WebsocketConsumer):
    def connect(self):
        self.accept()

    def disconnect(self, close_code):
        pass

    def receive(self, text_data):
        data = json.loads(text_data)
        # Handle the data received from the WebSocket (e.g., ESP32 data)
        # Example:
        target_angle = data.get("targetAngle")
        assisted_angle = data.get("assistedAngle")

        # You can send data back to the WebSocket
        self.send(
            text_data=json.dumps(
                {
                    "response": "Data received!",
                    "targetAngle": target_angle,
                    "assistedAngle": assisted_angle,
                }
            )
        )

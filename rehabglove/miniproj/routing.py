from django.urls import path
from miniproj import consumers

websocket_urlpatterns = [
    path("ws/esp32/", consumers.ESP32Consumer.as_asgi()),  # WebSocket route
]

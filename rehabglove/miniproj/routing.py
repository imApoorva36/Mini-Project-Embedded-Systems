from django.urls import re_path
from .consumers import ArduinoConsumer

websocket_urlpatterns = [
    re_path(r"ws/arduino/", ArduinoConsumer.as_asgi()),
]

# asgi.py

import os
from django.core.asgi import get_asgi_application
from channels.routing import ProtocolTypeRouter, URLRouter
from channels.auth import AuthMiddlewareStack
from django.urls import path
from miniproj.consumers import ArduinoConsumer

os.environ.setdefault("DJANGO_SETTINGS_MODULE", "rehabglove.settings")

application = ProtocolTypeRouter(
    {
        "http": get_asgi_application(),  # HTTP requests handled by Django app
        "websocket": AuthMiddlewareStack(  # WebSocket routing
            URLRouter(
                [
                    path(
                        "ws/arduino/", ArduinoConsumer.as_asgi()
                    )  # WebSocket endpoint
                ]
            )
        ),
    }
)

#include <WiFi.h>
#include <WebSocketsServer.h>

const char *ssid = "your_SSID";
const char *password = "your_PASSWORD";

WebSocketsServer webSocket = WebSocketsServer(81);

void setup()
{
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void loop()
{
    webSocket.loop();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    if (type == WStype_TEXT)
    {
        Serial.printf("[%u] Received: %s\n", num, payload);
        // Handle the received data here
    }
}

#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebSocketsClient.h>

const int numFingers = 5;
const int flexSensorPin[numFingers] = {34, 35, 32, 33, 36};
const int servoPin[numFingers][2] = { {25, 26}, {27, 14}, {12, 13}, {16, 17}, {18, 19} };

WebSocketsClient webSocket;
Servo servos[numFingers][2];

float targetAngle[numFingers] = {0};
float assistedAngle[numFingers] = {0};
float thresholdChange = 5;
bool assistActive[numFingers] = {false};

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

const char* webSocketServerHost = "your.server.com";
const int webSocketServerPort = 81;

float previousAngle[numFingers] = {0};
unsigned long lastCheckTime[numFingers] = {0};
unsigned long checkInterval = 1000;

void setupWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi.");
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    String msg = String((char *)payload);
    for (int i = 0; i < numFingers; i++) {
      String fingerID = String(i);
      if (msg.indexOf("targetAngle" + fingerID) >= 0) {
        targetAngle[i] = msg.substring(msg.indexOf(":") + 1).toFloat();
      }
      if (msg.indexOf("assistedAngle" + fingerID) >= 0) {
        assistedAngle[i] = msg.substring(msg.indexOf(":") + 1).toFloat();
      }
    }
  }
}

int calculateServoSpeed(float currentAngle, float targetAngle) {
  float angleDifference = targetAngle - currentAngle;
  if (angleDifference > 0) {
    return map(angleDifference, 0, 180, 90, 180);
  } else if (angleDifference < 0) {
    return map(angleDifference, -180, 0, 0, 90);
  } else {
    return 90;
  }
}

void moveServos(int fingerIndex, float currentAngle, float targetAngle) {
  int speed = calculateServoSpeed(currentAngle, targetAngle);
  servos[fingerIndex][0].write(speed);
  servos[fingerIndex][1].write(180 - speed);
}

void setup() {
  Serial.begin(115200);
  setupWiFi();
  webSocket.begin(webSocketServerHost, webSocketServerPort, "/");
  webSocket.onEvent(webSocketEvent);
  for (int i = 0; i < numFingers; i++) {
    servos[i][0].attach(servoPin[i][0]);
    servos[i][1].attach(servoPin[i][1]);
    servos[i][0].write(90);
    servos[i][1].write(90);
    previousAngle[i] = 0;
    lastCheckTime[i] = millis();
  }
}

void loop() {
  webSocket.loop();
  for (int i = 0; i < numFingers; i++) {
    float flexSensorVal = analogRead(flexSensorPin[i]);
    float currentAngle = map(flexSensorVal, 0, 4095, 0, 180);
    unsigned long currentTime = millis();
    if (currentTime - lastCheckTime[i] >= checkInterval) {
      if (abs(currentAngle - previousAngle[i]) >= thresholdChange) {
        assistActive[i] = true;
      }
      previousAngle[i] = currentAngle;
      lastCheckTime[i] = currentTime;
    }
    if (assistActive[i]) {
      moveServos(i, currentAngle, targetAngle[i]);
      if (currentAngle >= assistedAngle[i]) {
        assistActive[i] = false;
        servos[i][0].write(90);
        servos[i][1].write(90);
      }
    }
  }
  delay(10);
}

#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebSocketsClient.h>

// Configuration
const int numFingers = 5;                                                               // Number of fingers
const int flexSensorPin[numFingers] = {34, 35, 32, 33, 36};                             // Flex sensor pins
const int servoPin[numFingers][2] = {{25, 26}, {27, 14}, {12, 13}, {16, 17}, {18, 19}}; // Servo pins

WebSocketsClient webSocket;  // WebSocket client
Servo servos[numFingers][2]; // Servo objects for two servos per finger

// Angle arrays and parameters
float targetAngle[numFingers] = {0};     // Target angles from the Django app
float assistedAngle[numFingers] = {0};   // Assisted angles
float thresholdChange = 5;               // Change threshold for activation
bool assistActive[numFingers] = {false}; // Whether assist is active for each finger

// WiFi configuration
const char *ssid = "Pixelme";         // Replace with your WiFi SSID
const char *password = "12345678"; // Replace with your WiFi password

// WebSocket server configuration
const char *webSocketServerHost = "https://mini-project-embedded-systems.onrender.com/"; // Replace with your server IP or hostname
const int webSocketServerPort = 8000;                  // WebSocket server port

// State variables for previous angles and check timing
float previousAngle[numFingers] = {0};
unsigned long lastCheckTime[numFingers] = {0};
unsigned long checkInterval = 1000; // Time interval for flex sensor checks

void setupWiFi()
{
  WiFi.begin(ssid, password); // Connect to WiFi
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi.");
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  if (type == WStype_TEXT)
  {
    // Process incoming message to extract angles
    String msg = String((char *)payload);
    for (int i = 0; i < numFingers; i++)
    {
      String targetKey = "targetAngle" + String(i);
      String assistedKey = "assistedAngle" + String(i);

      // Extract target angle
      if (msg.indexOf(targetKey) >= 0)
      {
        targetAngle[i] = msg.substring(msg.indexOf(":") + 1, msg.indexOf(",", msg.indexOf(targetKey))).toFloat();
      }
      // Extract assisted angle
      if (msg.indexOf(assistedKey) >= 0)
      {
        assistedAngle[i] = msg.substring(msg.indexOf(":", msg.indexOf(assistedKey)) + 1).toFloat();
      }
    }
  }
}

int calculateServoSpeed(float currentAngle, float targetAngle)
{
  float angleDifference = targetAngle - currentAngle;
  if (angleDifference > 0)
  {
    return map(angleDifference, 0, 180, 90, 180); // Speed for increasing angle
  }
  else if (angleDifference < 0)
  {
    return map(angleDifference, -180, 0, 0, 90); // Speed for decreasing angle
  }
  else
  {
    return 90; // Neutral position
  }
}

void moveServos(int fingerIndex, float currentAngle)
{
  int speed = calculateServoSpeed(currentAngle, targetAngle[fingerIndex]);
  servos[fingerIndex][0].write(speed);       // Move first servo
  servos[fingerIndex][1].write(180 - speed); // Move second servo
}

void setup()
{
  Serial.begin(115200);
  setupWiFi();                                                    // Connect to WiFi
  webSocket.begin(webSocketServerHost, webSocketServerPort, "/"); // Initialize WebSocket connection
  webSocket.onEvent(webSocketEvent);                              // Set the WebSocket event handler

  for (int i = 0; i < numFingers; i++)
  {
    servos[i][0].attach(servoPin[i][0]); // Attach first servo
    servos[i][1].attach(servoPin[i][1]); // Attach second servo
    servos[i][0].write(90);              // Set initial position
    servos[i][1].write(90);              // Set initial position
    previousAngle[i] = 0;                // Initialize previous angle
    lastCheckTime[i] = millis();         // Initialize check time
  }
}

void loop()
{
  webSocket.loop(); // Handle WebSocket events
  for (int i = 0; i < numFingers; i++)
  {
    float flexSensorVal = analogRead(flexSensorPin[i]);       // Read flex sensor value
    float currentAngle = map(flexSensorVal, 0, 4095, 0, 180); // Map flex sensor value to angle
    unsigned long currentTime = millis();

    // Check if it’s time to evaluate the assist status
    if (currentTime - lastCheckTime[i] >= checkInterval)
    {
      if (abs(currentAngle - previousAngle[i]) >= thresholdChange)
      {
        assistActive[i] = true; // Activate assist if significant flex is detected
      }
      previousAngle[i] = currentAngle; // Update previous angle
      lastCheckTime[i] = currentTime;  // Update check time
    }

    // Move servos if assist is active
    if (assistActive[i])
    {
      moveServos(i, currentAngle);
      // Stop assisting if current angle exceeds assisted angle
      if (currentAngle >= assistedAngle[i])
      {
        assistActive[i] = false; // Deactivate assist
        servos[i][0].write(90);  // Neutral position
        servos[i][1].write(90);  // Neutral position
      }
    }
  }
  delay(10); // Short delay for stability
}

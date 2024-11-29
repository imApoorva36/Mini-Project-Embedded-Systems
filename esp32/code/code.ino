#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ESP32Servo.h>

const char *ssid = "vivo";         // Wi-Fi SSID
const char *password = "12345677"; // Wi-Fi password

// WebSocket server running on port 81
WebSocketsServer webSocket = WebSocketsServer(81);

const int flexSensorPin = 34;     // Flex sensor pin
const int servoPin[2] = {23, 22}; // Servo pins
Servo servos[2];                  // Servo objects

// Initial parameters for the servo and flex sensor logic
int prev_flex = 2900;
int minimum = 2700;
int maximum = 3200;
int refer = 2900;
int threshold = 100; // Default threshold value (initial)
int state = 0;
int start = 0;
int end = 0;
int final = 0;
int change = 0;
int motor = 0;

// WebSocket event handler to receive messages
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  if (type == WStype_TEXT)
  {
    String message = String((char *)payload);

    // Check if the message contains a threshold update
    if (message.startsWith("threshold:"))
    {
      threshold = message.substring(10).toInt(); // Extract and update threshold
      Serial.print("Updated threshold to: ");
      Serial.println(threshold);
    }
  }
}

void setup()
{
  Serial.begin(115200); // Initialize serial communication

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP()); // Print ESP32 IP address

  // Start the WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // Initialize the servos
  servos[0].attach(servoPin[0]);
  servos[1].attach(servoPin[1]);
  servos[0].write(90); // Set initial servo positions to neutral
  servos[1].write(90);
}

void loop()
{
  // Handle incoming WebSocket messages
  webSocket.loop();

  // Main flex sensor reading and servo control logic
  int flexValue = analogRead(flexSensorPin); // Read the flex sensor value
  if (flexValue < minimum && flexValue > maximum)
  {
    refer = prev_flex;
  }
  else if (abs(flexValue - prev_flex) > 50)
  {
    refer = prev_flex;
  }
  else
  {
    refer = flexValue;
    prev_flex = flexValue;
  }

  // Logic to control the servos based on flex sensor value and state
  if (state == 0)
  {
    start = refer;
    state = 1;
    if (start < 3000)
    {
      end = 3100;
      final = 1;
    }
    else if (start > 3000)
    {
      end = 2800;
      final = 0;
    }
  }

  if (state == 1)
  {
    if (final)
    {
      change = refer - start;
      if (refer > end)
      {
        state = 0;
        motor = 0;
        change = 0;
      }
    }
    else
    {
      change = start - refer;
      if (refer < end)
      {
        state = 0;
        motor = 0;
        change = 0;
      }
    }
  }

  // Use the threshold value received from the Django app
  if (change > threshold)
  {
    motor = 1;
  }

  // Control the servos based on the motor state
  if (motor == 1)
  {
    if (final)
    {
      servos[0].write(180); // First servo (fully rotated)
      servos[1].write(88);  // Second servo (adjusted position)
    }
    else
    {
      servos[0].write(0);   // First servo (reset)
      servos[1].write(100); // Second servo (reset)
    }
  }
  else
  {
    servos[0].write(90); // Neutral position
    servos[1].write(90); // Neutral position
  }

  // Debugging output
  Serial.print("Raw Flex Value: ");
  Serial.println(flexValue);
  Serial.print("Threshold: ");
  Serial.println(threshold);
  Serial.print("State: ");
  Serial.println(state);
  Serial.print("Motor: ");
  Serial.println(motor);
  Serial.print("Change: ");
  Serial.println(change);
  delay(1000); // Wait before reading again
}
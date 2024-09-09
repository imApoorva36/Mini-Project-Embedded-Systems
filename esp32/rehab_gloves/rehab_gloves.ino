#include <ESP32Servo.h>
// Define the number of flex sensors and servos
#define NUM_FLEX_SENSORS 5
#define NUM_SERVOS 10
#define change 1
// Analog pins for the flex sensors
int flexPins[NUM_FLEX_SENSORS] = {15, 2, 0, 4, 12}; // Change these pins according to your wiring
int servoAngle[NUM_FLEX_SENSORS] int servoAnglePast[NUM_FLEX_SENSORS] = {0};
// Digital pins for the servos
int servoPins[NUM_SERVOS] = {19, 18, 5, 17, 32, 33, 25, 26, 27, 14}; // Change these pins according to your wiring

// Create Servo objects
Servo servos[NUM_SERVOS];

// Flex sensor readings
int flexValues[NUM_FLEX_SENSORS];

void setup()
{
  // Initialize serial communication
  Serial.begin(115200);

  // Attach servos to pins
  for (int i = 0; i < NUM_SERVOS; i++)
  {
    servos[i].attach(servoPins[i]);
  }
}

void loop()
{
  // Read values from flex sensors
  for (int i = 0; i < NUM_FLEX_SENSORS; i++)
  {
    flexValues[i] = analogRead(flexPins[i]);
    Serial.print("Flex Sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(flexValues[i]);
  }

  // Map flex sensor values to servo positions
  for (int i = 0; i < NUM_FLEX_SENSORS; i++)
  {
    // Map flex sensor value to a servo angle (0 to 180 degrees)
    servoAngle = map(flexValues[i], 0, 4095, 0, 180);

    // Control two servos with the same flex sensor value
  }
  for (int i = 0; i < NUM_FLES_SENSORS; i++)
  {
    if (abs(servoAngle - servoAnglePast) >= change)
    {
      servos[i * 2].write(servoAngle + 5);
      servos[i * 2 + 1].write(175 - servoAngle);
      servoAnglePast = servoAngle;
    }
  }

  // Delay for stability
  delay(100);
}

int flex_map(int k)
{
  const int mapping[] = {180, 165, 150, 135, 128, 120, 113, 105, 90, 85, 80, 75, 68, 60, 55, 50, 45, 40, 35, 30, 26, 23, 19, 15, 11, 8, 4, 0};
  int index = k - 6;
  if (index >= 0 && index < sizeof(mapping) / sizeof(mapping[0]))
  {
    return mapping[index];
  }
  else
  {
    return 0;
  }
}

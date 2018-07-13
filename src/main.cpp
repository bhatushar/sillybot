/*
  Arduino-Mega based line following robot with obstacle detection and color recognition.

  Connections:
  # IR array
  | Pin | Connected to | Type |
  | --- | ------------ | ---- |
  |  1  |       52     | INPUT|
  |  2  |       50     | INPUT|
  |  3  |       48     | INPUT|
  |  4  |       46     | INPUT|
  |  5  |       44     | INPUT|
  |  6  |       42     | INPUT|
  |  7  |       40     | INPUT|
  |  8  |       38     | INPUT|

  # Motor driver
  | Pin | Connected to | Type |
  | --- | ------------ | ---- |
  | OUT1|       4      |OUTPUT|
  | OUT2|       5      |OUTPUT|
  | OUT3|       2      |OUTPUT|
  | OUT4|       3      |OUTPUT|

  # IR sensor
  | Pin | Connected to | Type |
  | --- | ------------ | ---- |
  | OUT |      32      | INPUT|

  # Color sensor
  | Pin | Connected to | Type |
  | --- | ------------ | ---- |
  | S2  |      51      |OUTPUT|
  | S3  |      53      |OUTPUT|
  | OUT |      49      | INPUT|

  #RGB led
  | Pin | Connected to | Type |
  | --- | ------------ | ---- |
  |  R  |      22      |OUTPUT|
  |  G  |      24      |OUTPUT|
  |  B  |      26      |OUTPUT|
*/

// Including required libraries
#include <Arduino.h>
#include <MotorDriver.h>
#include <ColorSensor.h>
#include <LineDetector.h>
#include <PIDController.h>


#define stdVolt 150


void turnOffLed();

// Declaring global pins
int motorPins[] = {2, 3, 4, 5}, // MotorL+, MotorL-, MotorR+, MotorR-
    //colorPins[] = {51, 53, 49}, // S2, S3, OUT
    lfrPins[] = {52, 50, 48, 46, 44, 42, 40, 38}; // Left to right
    //ledPins[] = {22, 24, 26}, // R, G, B
    //irPin = 32;

// Global objects
MotorDriver motor(motorPins);
//ColorSensor cSensor(colorPins);
LineDetector lfr(lfrPins);

int lastDeviation = 0; // Variable to store deviation calculated in previous cycle
    passedCross = 0; // Whether bot has passed a cross section

void setup(){
  // Setting LED pins as output
  for(int i = 0; i < 3; i++)
    pinMode(ledPins[i], OUTPUT);

  // Writing to color sensor pins for 20% power output
  digitalWrite(47, HIGH); // S1
  digitalWrite(45, LOW); // S0
}

void loop() {
  /*
  Steps:
  - Calculate deviation
  - Move the bot depending on the deviation
    - Turn right for negative deviation
    - Turn left for positive deviation
  - If deviation is zero, check if bot is on a cross-section
    - If on cross-section, rotate to the left.
  - Move forward
  - If an object is found, stop the motors and launch the color detection module
    - If cross-section was passed
  - Check for obstacles
    - If obstacle detected, stop motors launch color detection module.
    - Move the bot back to the cross-section and rotate to the right
  - How to stop
  - Store current deviation for future use
  */

  // Calculating error and voltage
  int deviation = lfr.calcDeviation();
  int volt = PID(deviation, lastDeviation);

  // Moving bot
  if(deviation < 0) {
    motor.move('r', volt);
  }
  else if(deviation > 0) {
    motor.move('l', volt);
  }
  else {
    // Deviation is zero
    if(lfr.isCrossSection()) {
      // Bot is at a cross-section
      // Rotate it 90 deg to the left
      passedCross = !passedCross; // Toggling passedCross variable
      
      motor.move('f', stdVolt); // Moving forward to bring cross-section at the axis of rotation
      delay(150);
      
      motor.move('l', stdVolt); // Rotating approx. 90 degrees to the left
      delay(450);
    }
    // Move forward
    motor.move('f', stdVolt);
  }

  // Detecting obstacle
  if(digitalRead(irPin) == HIGH) {
    motor.stop(); // Stop the bot
    delay(1000); // Delay required for color sensor to properly work
    switch(cSensor.getColor()) {
      case 'R':
        digitalWrite(ledPins[0], HIGH); // Glow red LED
        break;
      case 'G':
        digitalWrite(ledPins[1], HIGH); // Glow green LED
        break;
      case 'B':
        digitalWrite(ledPins[2], HIGH); // Glow blue LED
        break;
    }
    delay(2000); // Keep the LED glowing for 2 seconds
    turnOffLed();

    if(passedCross) {
      // If bot has passed a cross section
      motor.move('b', stdVolt); // Move a little back
      delay(200);
      motor.move('r', stdVolt); // Then rotate 180 degrees
      delay(1000);
      motor.stop();
    }
    else
      do {
        // Stop the bot while there's an obstacle
        delay(500);
      }while(digitalRead(irPin) == HIGH);
  }

  lastDeviation = deviation; // Storing current deviation for future use
}

inline void turnOffLed() {
  for(int i = 0; i < 3; i++)
    digitalWrite(ledPins[i], LOW);
}

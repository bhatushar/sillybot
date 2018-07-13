#ifndef LINEDETECTOR_H
#define LINEDETECTOR_H

#include <Arduino.h>

#define MAX_SENSOR 8

struct IRSensor {
  int pin,      // The pin to which the sensor is connected
      weight;   // The weight assigned to each pin
};

class LineDetector {
private:
  IRSensor sensor[MAX_SENSOR];
  int zeroSensor, onSensor;

public:
  LineDetector(int pins[]) {
    zeroSensor = 0;

    // Assigning pins to each sensor
    for(int i = 0; i < MAX_SENSOR; i++) {
      sensor[i].pin = pins[i];
      pinMode(sensor[i].pin, INPUT);
    }

    // Assigning weight to each sensor
    int even = !(MAX_SENSOR % 2); // Checking if total sensors are even or odd
    int value = even ? -(MAX_SENSOR/2 - 1) : -(MAX_SENSOR/2) ; //Weight which is to be assigned
    for(int i = 0; i < MAX_SENSOR; i++, value++) {
      if(even && i == MAX_SENSOR/2)
        // Additional condition for even sensors
        // Two sensors in middle should have weight = 0
        value--;
      sensor[i].weight = value; //Adding weight to each sensor
    }
  }

  int calcDeviation() {
    /*int motorPins[] = {4, 5, 2, 3};
    MotorDriver motor(motorPins);
    motor.stop();
    delay(100);*/
    int err = 0;
    for(int i = 0; i < MAX_SENSOR; i++) {
      int temp = sensor[i].weight;
      if(digitalRead(sensor[i].pin) == LOW) {
        // Current IR sensor is on the black strip
        sensor[i].weight = 0;
        zeroSensor++;
      }
      err += sensor[i].weight;
      sensor[i].weight = temp;
    }

    return err;
  }

  int isCrossSection() {
    /*
    Function to check if the bot is at a cross section using the weight of the sensors
    */
    if(zeroSensor == MAX_SENSOR) {
      // All sensors have zero weight
      zeroSensor = 0;
      return 1;
    }
    else {
      zeroSensor = 0;
      return 0;
    }
  }
};

#undef MAX_SENSOR

#endif

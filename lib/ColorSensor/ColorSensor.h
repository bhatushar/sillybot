#ifndef COLORSENSOR_H
#define COLORSENSOR_H

#include <Arduino.h>

class ColorSensor {
  /*
  Color sensing module.
  Connected to the color sensor, and contains methods to identify the color of the object.
  */

private:
  int sensorPin[2],  // Sets the color for which we need to read pulse
      readPin;    // Used to read pulse

  int strongest(unsigned int strength[]) {
    int value = 0;
    for(int i = 1; i < 3; i++)
      if(strength[value] > strength[i]) // Inverse relation
        value = i;
    return value;
  }

  unsigned int testFor(char color) {
    /*
    Returns the strength for a particular color
    */
    switch(color) {
      case 'R':
        for(int i = 0; i < 2; i++)
          digitalWrite(sensorPin[i], LOW);
        break;
      case 'G':
        for(int i = 0; i < 2; i++)
          digitalWrite(sensorPin[i], HIGH);
        break;
      case 'B':
        digitalWrite(sensorPin[0], LOW);
        digitalWrite(sensorPin[1], HIGH);
        break;
    }

    unsigned int pulseWidth = pulseIn(readPin, LOW);  // pulseWidth has inverse relation with strength of the color

    // Returning the strength of the color
    return pulseWidth;
  }

public:

  ColorSensor(int pins[]) {
    // Contructor
    for(int i = 0; i < 2; i++)
      sensorPin[i] = pins[i];
    readPin = pins[2];
    for(int i = 0; i < 2; i++)
      pinMode(sensorPin[i], OUTPUT);
    pinMode(readPin, INPUT);
  }

  char getColor() {
    /*
    Function to return the color with maximum strength
    */
    char colorCode[3] = {'R', 'G', 'B'};
    unsigned int colorStrength[3];
    for(int i = 0; i < 3; i++)
      colorStrength[i] = testFor(colorCode[i]);

    // Returning the code for the color with maximum strength
    return colorCode[strongest(colorStrength)];
  }

};

#endif

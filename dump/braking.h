/**
 * @file braking.h
 * @author Henrik Kaltenbach and Jakob Stoye (henrik.kaltenbach49@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-04-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <Arduino.h>

const int xPin = A0;
const int yPin = A1;
const int zPin = A2;

void printValues(int x, int y, int z);
void printValues(float x, float y, float z);

inline void detectHeavyBraking() {
  int x = analogRead(xPin);
  int y = analogRead(yPin);
  int z = analogRead(zPin);
  printValues(x, y, z);
  float xAccel = ((float)x - 331.5) / 65 * 9.81;
  float yAccel = ((float)y - 329.5) / 68.5 * 9.81;
  float zAccel = ((float)z - 340) / 68 * 9.81;
  printValues(xAccel, yAccel, zAccel);

  if (zAccel < -9.0) {
    blinkLED2s(2, 300);
  } else {
    delay(100);
  }

  delay(100);
}
#include "braking.h"

void detectHeavyBraking() {
  int x = analogRead(xPin);
  int y = analogRead(yPin);
  int z = analogRead(zPin);
  printValues(x, y, z);
  int xAccel = ((float)x - 331.5) / 65 * 9.81;
  int yAccel = ((float)y - 329.5) / 68.5 * 9.81;
  int zAccel = ((float)z - 340) / 68 * 9.81;
  printValues(xAccel, yAccel, zAccel);
  delay(100);
}

void printValues(int x, int y, int z) {
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(z);
  Serial.println();
}
#include <Arduino.h>

/**
 * @brief Prints the three integers on the serial monitor in a single line.
 * 
 * @param x First integer
 * @param y Second integer
 * @param z Third integer
 */
void printValues(int x, int y, int z) {
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(z);
  Serial.println();
}

/**
 * @brief Prints the three fp numbers on the serial monitor in a single line.
 * 
 * @param x First fp number
 * @param y Second fp number
 * @param z Third fp number
 */
void printValues(float x, float y, float z) {
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(z);
  Serial.println();
}
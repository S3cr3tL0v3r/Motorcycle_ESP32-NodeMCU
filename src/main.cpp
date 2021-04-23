/**
 * @file main.cpp
 * @author Henrik Kaltenbach and Jakob Stoye (henrik.kaltenbach49@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-04-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "main.h"

/**
 * @brief Setup code, runs once.
 * 
 */
void setup() {
  Serial.begin(9600);
}

/**
 * @brief Main code, runs repeatedly forever.
 * 
 */
void loop() {
  detectHeavyBraking();
  meassureOilTemperature();
}
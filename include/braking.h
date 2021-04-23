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
const int yPin = A3;
const int zPin = A4;

void printValues(int x, int y, int z);
void printValues(float x, float y, float z);
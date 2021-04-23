#include "main.h"

void setup() {
  Serial.begin(9600);
}

void loop() {
  detectHeavyBraking();
  meassureOilTemperature();
}
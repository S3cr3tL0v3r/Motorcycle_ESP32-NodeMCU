#include "heavyBraking.h"
#include <Arduino.h>

void setupHeavyBraking() 
{
  pinMode(X_AXIS, INPUT);
  pinMode(Y_AXIS, INPUT);
  pinMode(Z_AXIS, INPUT);
}

bool isBraking()
{
  // TODO: detect braking
  return false;
}

bool detectHeavyBraking()
{
  if (isBraking())
  {
    float xAxis = analogRead(X_AXIS);
    float yAxis = analogRead(Y_AXIS);
    float zAxis = analogRead(Z_AXIS);
    return sqrt(xAxis*xAxis + yAxis*yAxis + zAxis*zAxis) > 1.0;
  }
  else 
  {
    return false;
  }
}
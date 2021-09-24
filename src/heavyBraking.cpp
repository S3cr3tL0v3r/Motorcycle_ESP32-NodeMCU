#include "heavyBraking.h"
#include <Arduino.h>

uint8_t timesAxisRead = 0U;
float xAxis = 0.0F;
float yAxis = 0.0F;
float zAxis = 0.0F;

void setupHeavyBraking() 
{
  pinMode(X_AXIS, INPUT);
  pinMode(Y_AXIS, INPUT);
  pinMode(Z_AXIS, INPUT);
}

void readAxis()
{
    xAxis += analogRead(X_AXIS);
    yAxis += analogRead(Y_AXIS);
    zAxis += analogRead(Z_AXIS);
    timesAxisRead++;
}

void detectHeavyBraking()
{
  xAxis = (xAxis / timesAxisRead - 1849.0F) / 388.0F;
  yAxis = (yAxis / timesAxisRead - 1797.0F) / 408.0F;
  zAxis = (zAxis / timesAxisRead - 1840.0F) / 388.0F;
  heavyBraking = sqrt(xAxis*xAxis + yAxis*yAxis + zAxis*zAxis) > 1.0;
  xAxis = 0.0F;
  yAxis = 0.0F;
  zAxis = 0.0F;
  timesAxisRead = 0U;
}
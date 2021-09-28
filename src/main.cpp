#include <Arduino.h>
#include "BLE.h"
#include "oilTemperature.h"
#include "heavyBraking.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define TENTH 100

int8_t oldOilTemperature;
bool oldHeavyBraking;
uint32_t elapsedTenths = 0;

void setup() 
{
  Serial.begin(9600);     // start serial connection
  setupBLE();             // setup included libraries
  setupOilTemperature();
  setupHeavyBraking();
}

void loop() 
{
  if (isDeviceConnected())
  { // connected: notify changed value
    if (!isOldDeviceConnected())
    { // connecting
      setOldDeviceConnected();
      Serial.println("++++++++++++++++\nCONNECTED\n++++++++++++++++\n");
    }
    bool heavyBraking = detectHeavyBraking();
    if (oldHeavyBraking != heavyBraking)
    {
      oldHeavyBraking = heavyBraking;
      notifyHeavyBraking((uint8_t*)&heavyBraking);
      Serial.println("++++ heavyBraking notified ++++\n");
    }
    if (elapsedTenths % 10 == 0)
    {
      int8_t oilTemperature = meassureOilTemperature();
      if (oldOilTemperature != oilTemperature)
      {
        oldOilTemperature = oilTemperature;
        notifyOilTemperature((uint8_t*)&oilTemperature);
        Serial.println("++++ oilTemperature notified ++++\n");
      }
    }
    delay(TENTH);
    elapsedTenths++;
  }
  else if (isOldDeviceConnected())
  { // disconnecting
    delay(500);         // give the bluetooth stack the chance to get things ready
    startAdvertising(); // restart advertising
    setOldDeviceConnected();
    Serial.println("++++++++++++++++\nDISCONNECTED\n++++++++++++++++\n");
  }
}
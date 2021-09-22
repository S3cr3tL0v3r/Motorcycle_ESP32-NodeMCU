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

/*
  Video: https://www.youtube.com/watch?v=oCMOYS71NIU
  Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
  Ported to Arduino ESP32 by Evandro Copercini
  updated by chegewara

  Create a BLE server that, once we receive a connection, will send periodic notifications.
  The service advertises itself as: 4fafc201-1fb5-459e-8fcc-c5c9c331914b
  And has a characteristic of: beb5483e-36e1-4688-b7f5-ea07361b26a8

  The design of creating the BLE server is:
  1. Create a BLE Server
  2. Create a BLE Service
  3. Create a BLE Characteristic on the Service
  4. Create a BLE Descriptor on the characteristic
  5. Start the service.
  6. Start advertising.

  A connect hander associated with the server starts a background task that performs notification
  every couple of seconds.
*/

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "ADC.h"
#include "BLE.h"

#define OILTEMP       13
#define X_AXIS        34
#define Y_AXIS        39
#define Z_AXIS        36
#define RELAIS        10
#define TEN_MILLIS    10
#define FREQUENCY     5
#define T0            298.15
#define B             3000.0
#define ADC_MAX_VALUE 4095.0
#define DEVICE_NAME                         "Henrik's ESP32"
#define ENVIRONMENTAL_SENSING_SERVICE_UUID  "0000181A-0000-1000-8000-00805F9B34FB"
#define TEMPERATURE_CHARACTERISTIC_UUID     "00002A6E-0000-1000-8000-00805F9B34FB"
#define HEAVY_BRAKING_CHARACTERISTIC_UUID   "01234567-89AB-CDEF-FEDC-BA9876543210"

bool deviceConnected = false;
bool oldDeviceConnected = false;
bool heavyBraking = false;
bool oldHeavyBraking = false;
uint32_t elapsedTenMillis = 0U;
float adcReading = 0U;
float xAxis = 0.0F;
float yAxis = 0.0F;
float zAxis = 0.0F;
double oilTemperature = 0.0;
double oldOilTemperature = 0.0 / 0.0;
BLEServer* pServer/* = nullptr*/;
BLECharacteristic* pTemperatureCharacteristic/* = nullptr*/;
BLECharacteristic* pHeavyBrakingCharacteristic/* = nullptr*/;

class MyServerCallbacks : public BLEServerCallbacks 
{
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

void setup() 
{
  // Start serial connection
  Serial.begin(9600);

  // Set pin modes
  pinMode(OILTEMP, INPUT);
  pinMode(X_AXIS, INPUT);
  pinMode(Y_AXIS, INPUT);
  pinMode(Z_AXIS, INPUT);
  pinMode(RELAIS, OUTPUT);

  // Create the BLE Device
  BLEDevice::init(DEVICE_NAME);

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pEnvironmentalSensingService = pServer->createService(ENVIRONMENTAL_SENSING_SERVICE_UUID);

  // Create the BLE Characteristics
  pTemperatureCharacteristic = pEnvironmentalSensingService->createCharacteristic(
                      TEMPERATURE_CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  pHeavyBrakingCharacteristic = pEnvironmentalSensingService->createCharacteristic(
                      HEAVY_BRAKING_CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create the BLE Descriptors
  pTemperatureCharacteristic->addDescriptor(new BLE2902());
  pHeavyBrakingCharacteristic->addDescriptor(new BLE2902());

  // Start the services
  pEnvironmentalSensingService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(ENVIRONMENTAL_SENSING_SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
}

bool detectHeavyBraking() 
{
  xAxis = (xAxis / 10.0F - 1849.0F) / 388.0F;
  yAxis = (yAxis / 10.0F - 1797.0F) / 408.0F;
  zAxis = (zAxis / 10.0F - 1840.0F) / 388.0F;
  return sqrt(xAxis*xAxis + yAxis*yAxis + zAxis*zAxis) > 1.0;
}

double steinhartHartBParameter(float adcReading)
{
  if (adcReading == 0.0F) 
    return -(1.0 / 0.0);
  else if (adcReading == 4095.0F)
    return 1.0 / 0.0;
  else
    return (1.0 / (1.0/T0 + 1.0/B * log(ADC_MAX_VALUE/adcReading - 1.0))) - 273.15;
}

void loop() 
{
  if (deviceConnected) 
  { // run every 10 milliseconds
    if (!oldDeviceConnected) 
    { // on connecting
      Serial.println("++++++++++++++++\nCONNECTED\n++++++++++++++++\n");
      oldDeviceConnected = deviceConnected;
    }

    xAxis += analogRead(X_AXIS);
    yAxis += analogRead(Y_AXIS);
    zAxis += analogRead(Z_AXIS);

    if (!(elapsedTenMillis % 10))
    { // run every 100 milliseconds
      adcReading += analogRead(OILTEMP);
      heavyBraking = detectHeavyBraking();
      if (oldHeavyBraking != heavyBraking)
      {
        oldHeavyBraking = heavyBraking;
        pHeavyBrakingCharacteristic->setValue((uint8_t*)&heavyBraking, 1);
        pHeavyBrakingCharacteristic->notify();
      }
      xAxis = 0.0F;
      yAxis = 0.0F;
      zAxis = 0.0F;
    }
    
    if (!(elapsedTenMillis % 100)) 
    { // run every second
      oilTemperature = steinhartHartBParameter(adcReading /= 10.0F);
      if (oldOilTemperature != oilTemperature)
      {
        oldOilTemperature = oilTemperature;
        pTemperatureCharacteristic->setValue((uint8_t*)&oilTemperature, 8);
        pTemperatureCharacteristic->notify();
        Serial.print("++++++++++++++++\nNOTIFIED: ");
        Serial.println(oilTemperature);
        Serial.println("++++++++++++++++\n");
      }
      Serial.print("adcReading: ");Serial.println(adcReading);
      Serial.print("oilTemperature: ");Serial.println(oilTemperature);
      Serial.println();
      adcReading = 0U;
    }

    delay(TEN_MILLIS);
    elapsedTenMillis++;
  }
  else if (oldDeviceConnected)
  { // on disconnecting
    Serial.println("++++++++++++++++\nDISCONNECTED\n++++++++++++++++\n");
    delay(50 * TEN_MILLIS);       // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising();  // restart advertising
    oldDeviceConnected = deviceConnected;
  }
}

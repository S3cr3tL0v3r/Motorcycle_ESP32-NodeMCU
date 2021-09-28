/*
  Video: https://www.youtube.com/watch?v=oCMOYS71NIU
  Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
  Ported to Arduino ESP32 by Evandro Copercini
  updated by chegewara

  Create a BLE server that, once we receive a connection, will send periodic notifications.
  The service advertises itself as: 0000181A-0000-1000-8000-00805F9B34FB
  And has a characteristic of: 00002A6E-0000-1000-8000-00805F9B34FB

  The design of creating the BLE server is:
  1. Create a BLE Server
  2. Create a BLE Service
  3. Create a BLE Characteristic on the Service
  4. Create a BLE Descriptor on the characteristic
  5. Start the service.
  6. Start advertising.

  A connect hander associated with the server starts a background task that performs notification
  every couple of seconds.

  https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
*/

#include "BLE.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define DEVICE_NAME                         "Henrik's ESP32"
#define ENVIRONMENTAL_SENSING_SERVICE_UUID  "0000181A-0000-1000-8000-00805F9B34FB"
#define TEMPERATURE_CHARACTERISTIC_UUID     "00002A6E-0000-1000-8000-00805F9B34FB"
#define HEAVY_BRAKING_CHARACTERISTIC_UUID   "01234567-89AB-CDEF-FEDC-BA9876543210"
#define NUMBER_OF_CHARACTERISTICS           2

bool deviceConnected;
bool oldDeviceConnected;
BLEServer* pServer/* = nullptr*/;
BLECharacteristic* pTemperatureCharacteristic/* = nullptr*/;
BLECharacteristic* pHeavyBrakingCharacteristic/* = nullptr*/;
BLECharacteristic* pCharacteristics[NUMBER_OF_CHARACTERISTICS] = { pTemperatureCharacteristic, pHeavyBrakingCharacteristic };

class MyServerCallbacks : public BLEServerCallbacks 
{
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

bool isDeviceConnected()
{
    return deviceConnected;
}

bool isOldDeviceConnected()
{
    return oldDeviceConnected;
}

void setOldDeviceConnected()
{
    oldDeviceConnected = deviceConnected;
}

void startAdvertising()
{
  BLEDevice::startAdvertising();
}

void setupBLE()
{
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

  // Create the BLE Descriptors
  pTemperatureCharacteristic->addDescriptor(new BLE2902());
  pHeavyBrakingCharacteristic->addDescriptor(new BLE2902());

  // Start the services
  pEnvironmentalSensingService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(ENVIRONMENTAL_SENSING_SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x00U);   // set value to 0x00 to not advertise this parameter
  startAdvertising();
}

void notifyOilTemperature(uint8_t* oilTemperature)
{
    pTemperatureCharacteristic->setValue(oilTemperature, 1);
    pTemperatureCharacteristic->notify();
}

void notifyHeavyBraking(uint8_t* heavyBraking)
{
    pHeavyBrakingCharacteristic->setValue(heavyBraking, 1);
    pHeavyBrakingCharacteristic->notify();
}
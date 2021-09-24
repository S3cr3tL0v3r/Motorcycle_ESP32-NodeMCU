#include <Arduino.h>
// #include "BLE.h"
#include "oilTemperature.h"
// #include "heavyBraking.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define DEVICE_NAME                         "Henrik's ESP32"
#define ENVIRONMENTAL_SENSING_SERVICE_UUID  "0000181A-0000-1000-8000-00805F9B34FB"
#define TEMPERATURE_CHARACTERISTIC_UUID     "00002A6E-0000-1000-8000-00805F9B34FB"
#define HEAVY_BRAKING_CHARACTERISTIC_UUID   "01234567-89AB-CDEF-FEDC-BA9876543210"
#define NUMBER_OF_CHARACTERISTICS           2
#define TEMPERATURE_CHARACTERISTIC    0
#define HEAVY_BRAKING_CHARACTERISTIC  1

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

void notifyCharacteristic(uint8_t characteristic, uint8_t* data, uint8_t bytes)
{
  pCharacteristics[characteristic]->setValue(data, bytes);
  pCharacteristics[characteristic]->notify();
}

#define TEN_MILLIS  10

uint32_t elapsedTenMillis = 0U;

void setup() 
{
  // Start serial connection
  Serial.begin(9600);
  // Setup included libraries
  setupBLE();
  setupOilTemperature();
  //setupHeavyBraking();
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
    //readAxis();
    if (!(elapsedTenMillis % 10))
    { // run every 100 milliseconds
      readNTC();
      /*heavyBraking = detectHeavyBraking();
      if (oldHeavyBraking != heavyBraking)
      {
        oldHeavyBraking = heavyBraking;
        notifyCharacteristic(HEAVY_BRAKING_CHARACTERISTIC, (uint8_t*)&heavyBraking, 1);
      }*/
    }
    if (!(elapsedTenMillis % 100)) 
    { // run every second
      int8_t oilTemperature = convertADCToTemperature();
      notifyCharacteristic(TEMPERATURE_CHARACTERISTIC, (uint8_t*)&oilTemperature, 8);
    }
    delay(TEN_MILLIS);
    elapsedTenMillis++;
  }
  else if (oldDeviceConnected)
  { // on disconnecting
    delay(50 * TEN_MILLIS); // give the bluetooth stack the chance to get things ready
    startAdvertising();     // restart advertising
    oldDeviceConnected = deviceConnected;
  }
}

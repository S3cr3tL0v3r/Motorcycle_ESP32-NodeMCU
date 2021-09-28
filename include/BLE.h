#ifndef BLE_H
#define BLE_H

#include <stdint.h>

extern bool isDeviceConnected();
extern bool isOldDeviceConnected();
extern void setOldDeviceConnected();
extern void startAdvertising();
extern void setupBLE();
extern void notifyOilTemperature(uint8_t* oilTemperature);
extern void notifyHeavyBraking(uint8_t* heavyBraking);

#endif /* BLE_H */
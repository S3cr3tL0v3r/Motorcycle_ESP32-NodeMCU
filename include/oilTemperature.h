#ifndef OILTEMPERATURE_H
#define OILTEMPERATURE_H

#include <stdint.h>

#define NTC_PIN       25
#define R0            10080.0
#define ADC_MAX_VALUE 4095.0

extern void setupOilTemperature();
extern void readNTC();
extern int8_t convertADCToTemperature();

#endif /* OILTEMPERATURE_H */
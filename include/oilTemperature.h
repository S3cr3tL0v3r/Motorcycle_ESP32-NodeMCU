#ifndef OILTEMPERATURE_H
#define OILTEMPERATURE_H

#include <stdint.h>

#define NTC_PIN       33
#define R0            10080
#define ADC_MAX_VALUE 4095

extern void setupOilTemperature();
extern int8_t meassureOilTemperature();

#endif /* OILTEMPERATURE_H */
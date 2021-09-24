#include "oilTemperature.h"
#include <Arduino.h>

uint16_t adcReading = 0U;
uint8_t timesTemperatureRead = 0U;
uint32_t resistanceLookUpTable[] = {
  205900U, 194100U, 183000U, 172600U, 163000U, 153900U, 145400U, 137400U, 130000U, 123000U, // -40 – -31 °C
  116400U, 110200U, 104400U, 98630U, 93840U, 89020U, 87310U, 82830U, 78610U, 74630U,        // -30 – -21 °C
  70890U, 67360U, 64030U, 60900U, 57930U, 55140U, 52490U, 50000U, 47640U, 45410U,           // -20 – -11 °C
  43290U, 41300U, 39400U, 37610U, 35910U, 34300U, 32780U, 31330U, 29960U, 28650U,           // -10 –  -1 °C
  27410U, 25110U, 24080U, 23110U, 22180U, 21300U, 20460U, 19650U, 18890U, 18150U,           //   0 –   9 °C
  17450U, 16790U, 16150U, 15540U, 14960U, 14400U, 13870U, 13360U, 12880U, 12410U,           //  10 –  19 °C
  11960U, 11540U, 11130U, 10740U, 10360U, 10000U, 9628U, 9271U, 8931U, 8604U,               //  20 –  29 °C
  8292U, 7993U, 7707U, 7433U, 7170U, 6918U, 6677U, 6445U, 6223U, 6010U,                     //  30 –  39 °C
  5805U, 5609U, 5420U, 5239U, 5065U, 4898U, 4738U, 4583U, 4435U, 4292U,                     //  40 –  49 °C
  4155U, 3969U, 3842U, 3719U, 3601U, 3488U, 3379U, 3274U, 3172U, 3075U,                     //  50 –  59 °C
  2981U, 2890U, 2803U, 2719U, 2638U, 2559U, 2484U, 2411U, 2341U, 2273U,                     //  60 –  69 °C
  2207U, 2144U, 2083U, 2024U, 1967U, 1912U, 1858U, 1807U, 1757U, 1709U,                     //  70 –  79 °C
  1662U, 1617U, 1574U, 1532U, 1491U, 1451U, 1373U, 1337U, 1302U, 1267U,                     //  80 –  89 °C
  1234U, 1202U, 1171U, 1141U, 1112U, 1083U, 1056U, 1029U, 1004U, 979U,                      //  90 –  99 °C
  954U, 931U, 908U, 886U, 864U, 843U, 823U, 804U, 784U, 766U,                               // 100 – 109 °C
  748U, 730U, 713U, 697U, 681U, 665U, 650U, 635U, 621U, 607U,                               // 110 – 119 °C
  593U, 580U, 567U, 555U, 543U, 531                                                         // 120 – 125 °C
};

void setupOilTemperature()
{
  pinMode(NTC_PIN, INPUT);
}

void readNTC()
{
  adcReading += analogRead(NTC_PIN);
  timesTemperatureRead++;
}

uint32_t getResistance(float adcReading) 
{
  if (adcReading == 0)
  {
    return -1;
  }
  else
  {
    return (uint32_t)(R0 * (ADC_MAX_VALUE / adcReading - 1));
  }
}

int8_t getTemperature(uint32_t resistance)
{
  uint8_t i = 0;
  while (resistance < resistanceLookUpTable[i++]);
  return i - 41;
}

int8_t convertADCToTemperature()
{
  int8_t temperature = getTemperature(getResistance(adcReading / timesTemperatureRead));
  timesTemperatureRead = 0U;
  return temperature;
}

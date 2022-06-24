#ifndef DS18B20_H_
#define DS18B20_H_
#include "includes.h"
void Relay_init();
void ds18b20_init();
void printAddress(DeviceAddress deviceAddress);
void printTemperature(DeviceAddress deviceAddress);
void waterTemperatureControl(float _temperature);
void runtime_TemperatureController(void *pvParameters);
#endif
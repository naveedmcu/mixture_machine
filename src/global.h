#ifndef GLOBAL_H_
#define GLOBAL_H_
#include "includes.h"

extern TaskHandle_t Task1;
extern TaskHandle_t Task2;
extern TaskHandle_t Task3;
extern TaskHandle_t Task4;
extern TaskHandle_t Task5;
extern TaskHandle_t Task6;
extern TaskHandle_t Task7;
extern TaskHandle_t Task8;
extern portBASE_TYPE s1, s2, s3, s4, s5, s6, s7, s8;

void printAddress(DeviceAddress deviceAddress);
void printTemperature(DeviceAddress deviceAddress);
#endif
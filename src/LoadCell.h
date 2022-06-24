#ifndef LOADCELL_H_
#define LOADCELL_H_
#include "includes.h"
void readScale_Display(void *pvParameters);
void loadCellCalibration(void *pvParameters);
void init_LCD();
void init_loadCell();
#endif
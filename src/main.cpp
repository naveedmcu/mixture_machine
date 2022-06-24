#include "includes.h"
/******************************************************************************************/
void setup(void)
{
  Serial.begin(115200);
  Relay_init();
  ds18b20_init();
  init_LCD();
  init_loadCell();
  // s1 = xTaskCreate(runtime_TemperatureController, "Task1", 4096, NULL, 4, &Task1);
  // s2 = xTaskCreate(readScale_Display, "Task2", 4096, NULL, 4, &Task2);
  s3 = xTaskCreate(loadCellCalibration, "Task3", 4096, NULL, 4, &Task3);
  if (s1 != pdPASS)
  {
    Serial.println("Task1 not Created");
  }
  if (s2 != pdPASS)
  {
    Serial.println("Task2 not Created");
  }
  if (s3 != pdPASS)
  {
    Serial.println("Task3 not Created");
  }
  // start serial port
  esp_task_wdt_init(3660, 0); // 1 hour
}

void loop(void)
{
}

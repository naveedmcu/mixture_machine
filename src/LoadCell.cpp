#include "LoadCell.h"
// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 32;
const int LOADCELL_SCK_PIN = 33;

HX711 scale;
LiquidCrystal_I2C lcd(0x3F, 16, 2);
void init_loadCell()
{
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.tare();
}
void init_LCD()
{
    lcd.init();
    lcd.backlight();
}

void readScale_Display(void *pvParameters)
{
    esp_task_wdt_add(NULL);
    while (true)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        if (scale.is_ready())
        {
            long reading = scale.read_average(10);
            Serial.print("HX711 reading: ");
            Serial.println(reading);
            vTaskDelay(10 / portTICK_PERIOD_MS);
            lcd.setCursor(0, 0);
            lcd.print("Weight raw value");
            lcd.setCursor(0, 1);
            lcd.print(reading);
        }
        else
        {
            Serial.println("HX711 not found.");
        }
    }
    esp_task_wdt_reset();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void loadCellCalibration(void *pvParameters)
{
    float calibration_factor = -14; // this calibration factor is adjusted according to my load cell
    float units;
    float ounces;
    int16_t rawReading;
    esp_task_wdt_add(NULL);
    Serial.println("HX711 calibration sketch");
    Serial.println("Remove all weight from scale");
    Serial.println("After readings begin, place known weight on scale");
    Serial.println("Press + or a to increase calibration factor");
    Serial.println("Press - or z to decrease calibration factor");
    scale.set_scale();
    scale.tare();                            // Reset the scale to 0
    long zero_factor = scale.read_average(); // Get a baseline reading
    Serial.print("Zero factor: ");           // This can be used to remove the need to tare the scale. Useful in permanent scale projects.
    Serial.println(zero_factor);
    while (true)
    {
        scale.set_scale(calibration_factor); // Adjust to this calibration factor
        Serial.print("Reading: ");
        units = scale.get_units();
        rawReading = scale.read_average();
        if (units < 0)
        {
            units = 0.00;
        }
        ounces = units * 0.035274;
        Serial.print(String(units) + ',' + String(rawReading));
        Serial.print(" grams");
        Serial.print(" calibration_factor: ");
        Serial.print(calibration_factor);
        Serial.println();

        if (Serial.available())
        {
            char temp = Serial.read();
            if (temp == '+' || temp == 'a')
                calibration_factor += 1;
            else if (temp == '-' || temp == 'z')
                calibration_factor -= 1;
        }
    }
    esp_task_wdt_reset();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}
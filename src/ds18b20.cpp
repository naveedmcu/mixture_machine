#include "ds18b20.h"
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 14
#define TEMPERATURE_PRECISION 9 // Lower resolution

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

int numberOfDevices; // Number of temperature devices found

DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address
void Relay_init()
{
    pinMode(heater_pin, OUTPUT);
    pinMode(waterValve_pin, OUTPUT);
    Heater_OFF;
}
void ds18b20_init()
{
    Serial.println("Dallas Temperature IC Control Library Demo");
    // Start up the library
    sensors.begin();
    // Grab a count of devices on the wire
    numberOfDevices = sensors.getDeviceCount();

    // locate devices on the bus
    Serial.print("Locating devices...");

    Serial.print("Found ");
    Serial.print(numberOfDevices, DEC);
    Serial.println(" devices.");

    // report parasite power requirements
    Serial.print("Parasite power is: ");
    if (sensors.isParasitePowerMode())
        Serial.println("ON");
    else
        Serial.println("OFF");

    // Loop through each device, print out address
    for (int i = 0; i < numberOfDevices; i++)
    {
        // Search the wire for address
        if (sensors.getAddress(tempDeviceAddress, i))
        {
            Serial.print("Found device ");
            Serial.print(i, DEC);
            Serial.print(" with address: ");
            printAddress(tempDeviceAddress);
            Serial.println();

            Serial.print("Setting resolution to ");
            Serial.println(TEMPERATURE_PRECISION, DEC);

            // set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
            sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);

            Serial.print("Resolution actually set to: ");
            Serial.print(sensors.getResolution(tempDeviceAddress), DEC);
            Serial.println();
        }
        else
        {
            Serial.print("Found ghost device at ");
            Serial.print(i, DEC);
            Serial.print(" but could not detect address. Check power and cabling");
        }
    }
}
// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (deviceAddress[i] < 16)
            Serial.print("0");
        Serial.print(deviceAddress[i], HEX);
    }
}
// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
    // method 1 - slower
    // Serial.print("Temp C: ");
    // Serial.print(sensors.getTempC(deviceAddress));
    // Serial.print(" Temp F: ");
    // Serial.print(sensors.getTempF(deviceAddress)); // Makes a second call to getTempC and then converts to Fahrenheit

    // method 2 - faster
    float tempC = sensors.getTempC(deviceAddress);
    if (tempC == DEVICE_DISCONNECTED_C)
    {
        Serial.println("Error: Could not read temperature data");
        return;
    }
    Serial.print("Temp C: ");
    Serial.print(tempC);
    Serial.print(" Temp F: ");
    Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
    waterTemperatureControl(tempC);
}
void waterTemperatureControl(float _temperature)
{
    if (_temperature <= lowLimit && _temperature < HighLimit)
    {
        Heater_ON;
    }
    else if (_temperature >= HighLimit)
    {
        Heater_OFF;
    }
}
void runtime_TemperatureController(void *pvParameters)
{
    esp_task_wdt_add(NULL);
    // call sensors.requestTemperatures() to issue a global temperature
    // request to all devices on the bus
    Serial.print("Requesting temperatures...");
    sensors.requestTemperatures(); // Send the command to get temperatures
    Serial.println("DONE");

    int i;
    while (true)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // Search the wire for address
        if (sensors.getAddress(tempDeviceAddress, i))
        {
            // It responds almost immediately. Let's print out the data
            printTemperature(tempDeviceAddress); // Use a simple function to print out the data
        }
    }
    // else ghost device! Check your power requirements and cabling
}

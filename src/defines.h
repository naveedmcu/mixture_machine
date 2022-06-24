#ifndef DEFINES_H_
#define DEFINES_H_

#define heater_pin 2
#define waterValve_pin 5

#define Heater_ON  digitalWrite(heater_pin,HIGH)
#define Heater_OFF digitalWrite(heater_pin,LOW)
#define Valve_ON  digitalWrite(waterValve_pin,HIGH)
#define Valve_OFF digitalWrite(waterValve_pin,LOW)

#define lowLimit 18
#define HighLimit 28
#endif  
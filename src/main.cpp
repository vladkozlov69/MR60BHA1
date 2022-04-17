#include <Arduino.h>

#include "MR60BHA1.h"

#define SERIAL1_RXPIN 15 
#define SERIAL1_TXPIN 16

MR60BHA1 * sensor;

void setup() 
{
    Serial.begin(9600);
    // Serial1.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, SERIAL1_RXPIN, SERIAL1_TXPIN);
    while(!Serial);
    sensor = new MR60BHA1(&Serial1, &Serial);
}

void loop() 
{
    if (sensor->read())
    {

    }
}
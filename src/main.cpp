#include <Arduino.h>

#include "MR60BHA1.h"

MR60BHA1 * sensor;

void setup() 
{
    Serial.begin(9600);
    Serial1.begin(9600);
    while(!Serial);
    sensor = new MR60BHA1(&Serial1, &Serial);
}

void loop() 
{
    if (sensor->read())
    {

    }
}
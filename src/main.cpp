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
        bool known_data = false;
        MR60BHA1_PAYLOAD * payload = sensor->getPayload();
        if(payload->header.control_char == 0x80)
        {
            switch (payload->header.command_char)
            {
                case 1: // human presence: 0x00:Nobody 0x01:occupied
                    Serial.print("Human presence:");
                    Serial.println(sensor->getByteData());
                    known_data = true;
                    break;
                case 2: // Campaign information 0x00:None 0x01:Close 0x02:away 0x03:Disorderly motion
                    // << 53 59 80 02 00 01 00 2F 54 43 
                    // << 53 59 80 02 00 01 03 32 54 43 
                    Serial.print("Motion direction:");
                    Serial.println(sensor->getByteData());
                    known_data = true;
                    break;
                case 3: // Body motion parameters
                    // << 53 59 80 03 00 01 01 31 54 43 
                    // << 53 59 80 03 00 01 02 32 54 43 
                    Serial.print("Motion parameters:");
                    Serial.println(sensor->getByteData());
                    known_data = true;
                    break;
                case 4: // Number of people in the current environment
                    Serial.print("Number of people:");
                    Serial.println(sensor->getByteData());
                    known_data = true;
                    break;
                default:
                    break;
            }
        }

        if(payload->header.control_char == 0x81)
        {
            switch (payload->header.command_char)
            {
                case 1: // 1Byte heart rate information + 1Byte heart rate value + 4Byte heart rate waveform
                    //known_data = true;
                    break;
                case 2: // 1Byte respiratory information + 1Byte respiratory values + 4Byte respiratory waveforms
                    // OR: 1Byte heart rate value
                    // << 53 59 81 02 00 01 00 30 54 43
                    // << 53 59 81 02 00 01 00 30 54 43 
                    // << 53 59 81 02 00 01 3C 6C 54 43 
                    Serial.print("81 02 heart rate value:");
                    Serial.println(sensor->getByteData());
                    known_data = true;
                    break;
                case 3:  // 1Byte position warning + 2Byte standstill distance + 2Byte standstill angle
                    // << 53 59 81 03 00 05 0A 7E 33 1F 63 72 54 43 
                    // << 53 59 81 03 00 05 0B 9A 85 91 B1 A1 54 43 
                    // << 53 59 81 03 00 05 17 53 FE 2B E6 AE 54 43
                    known_data = true;
                    break;
                case 4: // 1 Byte Respiratory information
                    // << 53 59 81 04 00 01 01 33 54 43
                    Serial.print("Respiratory information:");
                    Serial.println(sensor->getByteData());
                    known_data = true;
                    break;
                case 5: // 1 Byte Respiratory values
                    // << 53 59 81 05 00 01 00 33 54 43
                    // << 53 59 81 05 00 01 00 33 54 43 
                    // << 53 59 81 05 00 01 0C 3F 54 43 
                    // << 53 59 81 05 00 01 0D 40 54 43 
                    Serial.print("Respiratory values:");
                    Serial.println(sensor->getByteData());
                    known_data = true;
                    break;
                case 6: // 1 Byte Respiratory waveforms // FIXME
                    // << 53 59 81 06 00 05 3F 3F 41 45 4B 87 54 43 
                    // << 53 59 81 06 00 05 4D 4D 4E 50 54 C4 54 43 
                    // << 53 59 81 06 00 05 5C 64 6D 75 7F 59 54 43 
                    Serial.print("Respiratory waveforms:");
                    Serial.println(sensor->getByteData());
                    known_data = true;
                    break;
                case 7: // 1 Byte Location detection anomalies
                    // << 53 59 81 07 00 01 00 35 54 43 
                    // << 53 59 81 07 00 01 01 36 54 43 
                    Serial.print("Location detection anomalies:");
                    Serial.println(sensor->getByteData());
                    known_data = true;
                    break;
                case 8: // 2 Byte Stationary distance, cm
                    // << 53 59 81 08 00 02 00 06 3D 54 43
                    // << 53 59 81 08 00 02 00 06 3D 54 43 
                    // << 53 59 81 08 00 02 00 36 6D 54 43 
                    // << 53 59 81 08 00 02 00 42 79 54 43 
                    Serial.print("Stationary distance, cm:");
                    Serial.println(sensor->getWordData());
                    known_data = true;
                    break;
                case 9: // 2 Byte Stationary angle, degrees
                    // << 53 59 81 09 00 02 00 66 9E 54 43 
                    // << 53 59 81 09 00 02 00 67 9F 54 43 
                    // << 53 59 81 09 00 02 00 68 A0 54 43 
                    Serial.print("Stationary angle, degrees:");
                    Serial.println(sensor->getWordData());
                    known_data = true;
                    break;
                    
                default:
                    break;
            }
        }

        if (!known_data)
        {
            Serial.println("UNDECODED");
        }
    }
}
#include "Arduino.h"
#include "LoRa_E32.h"
#include <AltSoftSerial.h>

#define DEPin 11

AltSoftSerial mod;

const byte registers[] = {0x01, 0x03, 0x00, 0x13, 0x00, 0x01, 0x75, 0xCF};
int values[12];
int* tempval;


class MultiParameterSoilSensor: public AbstractSensor {  
    public: 
        unsigned __int8 fetchDataDevisor;
        MultiParameterSoilSensor(
            unsigned __int8 sensorDeviceId, 
            unsigned __int8 sensorCode,
            unsigned __int16 sensorSleepCount=100, 
            unsigned __int8 sensorReadCount=10, 
            unsigned __int8 sensorMValue=1, 
            unsigned __int8 sensorCValue=0, 
            unsigned __int8 sensorMinValue=0, 
            unsigned __int8 sensorMaxValue=100,
            unsigned __int8 sensorFetchDataDevisor=1
        ): AbstractSensor(
            sensorDeviceId, 
            sensorCode, 
            sensorLabel, 
            sensorSleepCount, 
            sensorReadCount, 
            sensorMValue, 
            sensorCValue, 
            sensorMinValue, 
            sensorMaxValue
        ){
            mod.begin(9600);

            pinMode(DEPin, OUTPUT);
            digitalWrite(DEPin, LOW);

            fetchDataDevisor = sensorFetchDataDevisor;
        }

        int* RS485soil() {
            // clear the receive buffer
            mod.flushInput();
            digitalWrite(DEPin, HIGH);
            delay(1);
            // write out the message
            for (uint8_t i = 0; i < sizeof(registers); i++ ) mod.write( registers[i] );
            // wait for the transmission to complete
            mod.flush();
            digitalWrite(DEPin, LOW);
            // delay to allow response bytes to be received!
            delay(500);
            // read in the received bytes
            for (int i = 0; i < 7; i++) {
                values[i] = mod.read();
                Serial.print(values[i]);
                Serial.print(' ');
            }
            return values;
        }

        virtual float fetchData(){
        //     tempval = RS485soil();
        //     int val3 = (tempval[3]*256) + tempval[4];
        //     Serial.println("*******************************");
        //     Serial.println(String(val3));
            
            mod.flushInput();

            digitalWrite(DEPin, HIGH);
            delay(1);

            for (uint8_t i = 0; i < sizeof(registers); i++ ) mod.write( registers[i] );
            
            mod.flush();

            digitalWrite(DEPin, LOW);

            delay(100);
            
            for (int i = 0; i < 7; i++) {
                values[i] = mod.read();
                Serial.println(String(values[i]));
            }

            int* tempValues = values;

            Serial.println(String(((tempValues[3]*256) + tempValues[4]) / fetchDataDevisor));
            return ((tempValues[3]*256) + tempValues[4]) / fetchDataDevisor;
        }
};
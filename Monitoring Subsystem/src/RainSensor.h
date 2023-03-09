#include "Arduino.h"
#include "LoRa_E32.h"

#define RainAnalogPin A0


class RainSensor: public AbstractSensor {  
    public: 
        RainSensor(
            byte sensorDeviceId, 
            byte sensorCode=40, 
            word sensorSleepCount=10000, 
            byte sensorReadCount=10, 
            byte sensorMValue=1, 
            byte sensorCValue=0, 
            word sensorMinValue=0, 
            word sensorMaxValue=1024
        ): AbstractSensor(
            sensorDeviceId, 
            sensorCode, 
            sensorSleepCount, 
            sensorReadCount, 
            sensorMValue, 
            sensorCValue, 
            sensorMinValue, 
            sensorMaxValue
        ){}

        long fetchData() {
            return analogRead(RainAnalogPin);
        }
};


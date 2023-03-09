#include "Arduino.h"
#include "LoRa_E32.h"
#include "AbstractSensor.h"

#define echoPin A4
#define trigPin A2


class MetaSensor: public AbstractSensor {  
    public: 
        MetaSensor(
        int sensorDeviceId, 
        float sensorCode, 
        int sensorCycleMax, 
        String sensorLabel="dataSensor", 
        float sensorMValue=1, 
        float sensorCValue=0, 
        float sensorMinValue=0, 
        float sensorMaxValue=100
        ): AbstractSensor(
        sensorDeviceId, 
        sensorCode, 
        sensorCycleMax, 
        sensorLabel, 
        sensorMValue, 
        sensorCValue, 
        sensorMinValue, 
        sensorMaxValue
        ){}

        long fetchData() {
            digitalWrite(trigPin, LOW);
            delayMicroseconds(2);
            digitalWrite (trigPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin, LOW);
            return pulseIn (echoPin, HIGH) * 0.034/2;
        }
};


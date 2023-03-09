#include "Arduino.h"
#include "LoRa_E32.h"

#define echoPin 5
#define trigPin 6


class DistanceSensor: public AbstractSensor {  
    public: 
        DistanceSensor(
            byte sensorDeviceId, 
            byte sensorCode=80, 
            word sensorSleepCount=100, 
            byte sensorReadCount=10, 
            byte sensorMValue=100, 
            byte sensorCValue=0, 
            word sensorMinValue=0, 
            word sensorMaxValue=9999
        ): AbstractSensor(
            sensorDeviceId, 
            sensorCode, 
            sensorSleepCount, 
            sensorReadCount, 
            sensorMValue, 
            sensorCValue, 
            sensorMinValue, 
            sensorMaxValue
        ){
            if(sensorCode == 0) sensorCode=80;
            //if(sensorLabel == "") sensorLabel="distanceSensor";
            if(sensorSleepCount == 0) sensorSleepCount=100;
            pinMode(trigPin, OUTPUT);
            pinMode(echoPin, INPUT);
        }

        long fetchData() {
            digitalWrite(trigPin, LOW);
            delayMicroseconds(2);
            digitalWrite (trigPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin, LOW);
            return pulseIn (echoPin, HIGH) * 0.034/2;
        }
};


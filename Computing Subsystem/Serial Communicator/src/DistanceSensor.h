#include "Arduino.h"
#include "AbstractSensor.h"

class DistanceSensor: AbstractSensor {
    public:
        int trigPin;
        int echoPin;

        DistanceSensor(float sensorCode, int sensorCycleMax, String sensorLabel, int setTrigPin, int setEchoPin):AbstractSensor{
            trigPin = setTrigPin;
            echoPin = setEchoPin;
        }
        
        float fetchData(){
            digitalWrite(trigPin, LOW);
            delayMicroseconds(2);
            digitalWrite (trigPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin, LOW);
            float rs = pulseIn (echoPin, HIGH) * 0.034/2;
            return rs;
        }
};
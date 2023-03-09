#include "Arduino.h"

class AmbientTemperatureSensor: public AbstractSensor {  
    public: 
        AmbientTemperatureSensor(
            byte sensorDeviceId, 
            byte sensorCode=70, 
            word sensorSleepCount=1000, 
            byte sensorReadCount=10, 
            byte sensorMValue=10, 
            byte sensorCValue=200, 
            word sensorMinValue=0, 
            word sensorMaxValue=1000
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
            // label = sensorLabel;
            dht.begin();
        }

        long fetchData() {
            float rs = dht.readTemperature();
            return rs;
        }
};


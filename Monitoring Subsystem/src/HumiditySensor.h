#include "Arduino.h"
#include "LoRa_E32.h"
#include "DHT.h"



class HumiditySensor: public AbstractSensor {  
    public: 
        HumiditySensor(
            byte sensorDeviceId, 
            byte sensorCode=20, 
            word sensorSleepCount=1000, 
            byte sensorReadCount=10, 
            byte sensorMValue=10, 
            byte sensorCValue=0, 
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
            dht.begin();
        }

        long fetchData() {
            return dht.readHumidity();
        }
};


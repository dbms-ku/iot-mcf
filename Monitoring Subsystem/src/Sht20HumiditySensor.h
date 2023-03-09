#include "Arduino.h"
#include <Wire.h>
#include "DFRobot_SHT20.h"

DFRobot_SHT20 sht20Humid;

class Sht20HumiditySensor: public AbstractSensor {  
    public: 
        Sht20HumiditySensor(
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
            
        }

        long fetchData() {
            if(readCount == 0){
                sht20Humid.initSHT20();  
                delay(100);
                sht20Humid.checkSHT20(); 
            }

            long rs = sht20Humid.readHumidity();
            return rs;
        }
};


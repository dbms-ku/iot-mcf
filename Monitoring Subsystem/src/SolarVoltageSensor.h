#include "Arduino.h"
#include "LoRa_E32.h"

#define SolarVoltagePin A2


class SolarVoltageSensor: public AbstractSensor {  
    public: 
        SolarVoltageSensor(
            byte sensorDeviceId, 
            byte sensorCode=52, 
            word sensorSleepCount=10, 
            byte sensorReadCount=10, 
            byte sensorMValue=100, 
            byte sensorCValue=0, 
            word sensorMinValue=0, 
            word sensorMaxValue=2000
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
            if(sensorCode == 0) sensorCode=52;
            // if(sensorLabel == "") sensorLabel="solarVoltageSensor";
            if(sensorSleepCount == 0) sensorSleepCount=100;
        }

        long fetchData() {
            long rs = (0.5 + analogRead(SolarVoltagePin)) * 5.0 / (1024.0 * 0.265);
            return rs;
        }
};


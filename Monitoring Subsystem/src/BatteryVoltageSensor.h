#include "Arduino.h"

#define BatteryVoltagePin A1

class BatteryVoltageSensor: public AbstractSensor {  
    public: 
        BatteryVoltageSensor(
            byte sensorDeviceId, 
            byte sensorCode=51, 
            word sensorSleepCount=1000, 
            byte sensorReadCount=10, 
            byte sensorMValue=100, 
            byte sensorCValue=0, 
            word sensorMinValue=0, 
            word sensorMaxValue=470
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
            if(sensorCode == 0) sensorCode=51;
            // if(sensorLabel == "") sensorLabel="batteryVoltageSensor";
            if(sensorSleepCount == 0) sensorSleepCount=100;
        }

        long fetchData() {
            return (0.5 + analogRead(BatteryVoltagePin)) * 5.0 / 1024.0;
        }
};


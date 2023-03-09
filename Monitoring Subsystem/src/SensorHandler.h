#include "Arduino.h"
#include "AbstractSensor.h"
#include "RadioCommunicator.h"


class SensorHandler { 
  public:
    RadioCommunicator rCommunicator;  
  public:
    SensorHandler(){
        rCommunicator = RadioCommunicator();
    }

    float transformData(AbstractSensor &AbSensor){
        if(isnan(AbSensor.stateData)) return 0;
        Serial.println("\n"+String(AbSensor.code));
        Serial.println("\n"+String(AbSensor.stateData));
        // Serial.println("\n"+String((AbSensor.stateData * AbSensor.mValue) + AbSensor.cValue));
        return (AbSensor.stateData * AbSensor.mValue) + AbSensor.cValue;
    }

    String packageData(AbstractSensor &AbSensor){
        float transformedData = (int) transformData(AbSensor);

        if(transformedData < AbSensor.minValue) return "000000"; 
        
        if(transformedData > AbSensor.maxValue) return "999999";
        
        long packagedData = (int) transformedData;
        String packagedDataStr = String(packagedData);
        int packagedDataStrLength = packagedDataStr.length();

        if(packagedData <= 0) return "000000";

        while(++packagedDataStrLength <= 6) packagedDataStr = "0" + packagedDataStr;

        return packagedDataStr;
    }

    String encodeData(AbstractSensor &AbSensor){
        return String(AbSensor.deviceId) + String(AbSensor.code) + packageData(AbSensor);
    }

    void setSleepingMode(AbstractSensor &AbSensor, bool sleepingValue){
        AbSensor.cycleCount = 0;
        AbSensor.sleeping = sleepingValue;
    }

    float closeCycle(AbstractSensor &AbSensor){
        float meanData = 0;
        if(AbSensor.cycleCount != 0) meanData = AbSensor.sum / AbSensor.cycleCount;
        
        AbSensor.sum = 0;
        AbSensor.cycleCount = 0;
        
        return meanData;
    }

    void processData(AbstractSensor &AbSensor){
        AbSensor.sum += AbSensor.fetchData();
        
        if(AbSensor.cycleCount >= AbSensor.readCount){
            AbSensor.stateData = closeCycle(AbSensor);
            setSleepingMode(AbSensor, true);
            
            LedBlinkElement.updateState(HIGH);
            if(!rCommunicator.sendData(encodeData(AbSensor))) Serial.println("Ack failed");
            LedBlinkElement.updateState(LOW);
        }
    }

    void routine(AbstractSensor &AbSensor){
        Serial.print(".");
        AbSensor.cycleCount++;
        if(AbSensor.sleeping){
            delay(1);
            if (AbSensor.cycleCount >= AbSensor.sleepCount) setSleepingMode(AbSensor, false);
        } else processData(AbSensor);
    }
};


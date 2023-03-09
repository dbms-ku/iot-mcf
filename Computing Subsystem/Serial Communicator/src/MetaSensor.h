#include "Arduino.h"
#include "LoRa_E32.h"

// #define device 10

LoRa_E32 e32ttl100(2,3);

class MetaSensor {   
  public:
    int deviceId;
    int code; 
    float sum; 
    int cycleMax; 
    int cycleCount; 
    float stateData = 0;
    float mValue;
    float cValue;
    float minValue;
    float maxValue;
    String label;

    MetaSensor(
      int sensorDeviceId, 
      float sensorCode, 
      int sensorCycleMax, 
      String sensorLabel="dataSensor", 
      float sensorMValue=1, 
      float sensorCValue=0, 
      float sensorMinValue=0, 
      float sensorMaxValue=100
    ){
      deviceId = sensorDeviceId;
      code = sensorCode;
      sum = 0;
      cycleMax = sensorCycleMax;
      cycleCount = 0;
      stateData = 0;
      mValue = sensorMValue;
      cValue = sensorCValue;
      minValue = sensorMinValue;
      maxValue = sensorMaxValue;
      label = sensorLabel;
    }

    void processData(float data){
      sum += data;
      cycleCount++;
      
      if(cycleCount >= cycleMax){
        stateData = closeCycle();
        
        sendData(encodeData());
      }
    }

    float closeCycle(){
      float meanData = 0;
      if(cycleCount != 0){
        meanData = sum / cycleCount;
      }
      
      sum = 0;
      cycleCount = 0;
      
      return meanData;
    }

    float transformData(){
      if(isnan(stateData)) {
        return 0;
      }
      return (stateData * mValue) + cValue;
    }

    String packageData(){
      float transformedData = transformData();

      if(transformedData < minValue){
         if(label=="soilEcData") Serial.println("stateData");
         if(label=="soilEcData") Serial.println(transformedData);
         if(label=="soilEcData") Serial.println(stateData);
         return "000000"; 
      }
      if(transformedData > maxValue) return "999999"; 
      
      long packagedData = transformedData * 100;
      if(packagedData <= 0) return "000000";
      if(packagedData < 10) return "00000" + String(packagedData);
      if(packagedData < 100) return "0000" + String(packagedData);
      if(packagedData < 1000) return "000" + String(packagedData);
      if(packagedData < 10000) return "00" + String(packagedData);
      if(packagedData < 100000) return "0" + String(packagedData);
      
      return String(packagedData);
    }

    String encodeData(){
      return String(deviceId) + String(code) + packageData();
    }

    bool sendData(String rs){
      Serial.println(rs);
      e32ttl100.sendMessage(rs);
      return true;
    }
};


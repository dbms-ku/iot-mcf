#include "Arduino.h"

class AbstractSensor {
 public:
   int device;
   int code; 
   float sum; 
   int cycleMax; 
   int cycleCount; 
   float stateData = 0;
   float minValue;
   float maxValue;
   String label;

   AbstractSensor(int sensorDevice, float sensorCode, int sensorCycleMax, float sensorMinValue, float sensorMaxValue, String sensorLabel){
     device = sensorDevice;
     code = sensorCode;
     sum = 0;
     cycleMax = sensorCycleMax;
     cycleCount = 0;
     stateData = 0;
     minValue = sensorMinValue;
     maxValue = sensorMaxValue;
     label = sensorLabel;
   }

   virtual float fetchData() = 0;

   void fetchAndProcessData(){
     processData(fetchData());
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
     if(isnan(stateData)) return 0;
     return stateData;
   }

   String packageData(){
     float transformedData = transformData();
     int packagedData = transformedData * 100;
     if(packagedData <= 0) return "000000";
     if(packagedData <= 10) return "00000" + String(packagedData);
     if(packagedData <= 100) return "0000" + String(packagedData);
     if(packagedData <= 1000) return "000" + String(packagedData);
     if(packagedData <= 10000) return "00" + String(packagedData);
     if(packagedData <= 100000) return "0" + String(packagedData);
     if(packagedData >= 100000) return "9999";
     return String(packagedData);
   }

   String encodeData(){
     return String(device) + String(code) + packageData();
   }

   bool sendData(String rs){
     Serial.println(rs);
     e32ttl100.sendMessage(rs);
   }
};
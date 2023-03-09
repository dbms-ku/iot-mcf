#include "Arduino.h"

#define freeAnalogPin A7
#define DHTPIN 4
#define DHTTYPE DHT11
#define LedPin 10

DHT dht(DHTPIN, DHTTYPE);

LedBlinker LedBlinkElement(LedPin, 1, 1, HIGH);

class AbstractSensor {   
  public:
    byte deviceId;
    byte code; 
    long sum; 
    bool sleeping;
    word sleepCount; 
    byte readCount;
    word cycleCount; 
    long stateData = 0;
    byte mValue;
    byte cValue;
    word minValue;
    word maxValue;
    // String label;
  
  public:
    AbstractSensor(
      byte sensorDeviceId, 
      byte sensorCode, 
      word sensorSleepCount=1000, 
      byte sensorReadCount=10, 
      byte sensorMValue=1, 
      byte sensorCValue=0, 
      word sensorMinValue=0, 
      word sensorMaxValue=100
    ){
      deviceId = sensorDeviceId;
      code = sensorCode;
      sum = 0;
      sleeping = false;
      sleepCount = sensorSleepCount;
      readCount = sensorReadCount;
      cycleCount = 0;
      stateData = 0;
      mValue = sensorMValue;
      cValue = sensorCValue;
      minValue = sensorMinValue;
      maxValue = sensorMaxValue;
      sleeping = true;
    }

    virtual long fetchData()=0;
};


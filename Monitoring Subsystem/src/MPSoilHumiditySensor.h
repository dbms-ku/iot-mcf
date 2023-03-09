#include "Arduino.h"
#include "LoRa_E32.h"

#define freeAnalogPin A7
#define DHTPIN 11
#define DHTTYPE DHT11

#define serialCommunicatorHighAdd 13
#define serialCommunicatorLowAdd 21
#define serialCommunicatorChannel 6

AltSoftSerial mPSoilHumidMod;
// DHT dht(DHTPIN, DHTTYPE);

// const byte MPSoilTemperatureRegisters[] = {0x01, 0x03, 0x00, 0x13, 0x00, 0x01, 0x75, 0xCF};
const byte MPSoilHumidityRegistry[] = {0x01, 0x03, 0x00, 0x12, 0x00, 0x01, 0x24, 0x0F};
// const byte RS485SoilEcRegistry[] = {0x01, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xCE};
// const byte RS485SoilPhRegistry[] = {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B};
// const byte RS485SoilNitrogenRegistry[] = {0x01, 0x03, 0x00, 0x1E, 0x00, 0x01, 0xE4, 0x0C};
// const byte RS485SoilPhosphorusRegistry[] = {0x01, 0x03, 0x00, 0x1F, 0x00, 0x01, 0xB5, 0xCC};
// const byte RS485SoilPotassiumRegistry[] = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xC0};

// LoRa_E32 e32ttl100(2, 3, 5, 7, 6);
// LoRa_E32 e32ttl100(2,3);

class MPSoilHumiditySensor {   
  public:
    unsigned __int8 deviceId;
    unsigned __int8 code; 
    unsigned __int8 sum; 
    bool sleeping;
    unsigned __int16 sleepCount; 
    unsigned __int8 readCount;
    unsigned __int8 cycleCount; 
    float stateData = 0;
    unsigned __int8 mValue;
    unsigned __int8 cValue;
    unsigned __int8 minValue;
    unsigned __int8 maxValue;
    // String label;
    unsigned __int8 fetchDataDevisor;
  
  public:
    MPSoilHumiditySensor(
      unsigned __int8 sensorDeviceId, 
      unsigned __int8 sensorCode, 
      // String sensorLabel="dataSensor", 
      unsigned __int16 sensorSleepCount=100, 
      unsigned __int8 sensorReadCount=10, 
      unsigned __int8 sensorMValue=1, 
      unsigned __int8 sensorCValue=0, 
      unsigned __int8 sensorMinValue=0, 
      unsigned __int8 sensorMaxValue=100,
      unsigned __int8 sensorFetchDataDevisor=1
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
      label = sensorLabel;
      fetchDataDevisor = sensorFetchDataDevisor;
    }

    void routine(){
      cycleCount++;
      if(sleeping){
        delay(1);
        // Serial.println("Routine "+label+": sleeping mode "+String(cycleCount));
        if (cycleCount >= sleepCount) setSleepingMode(false);
      } else {
        // Serial.println("Routine "+label+": reading mode "+String(cycleCount));
        processData(fetchData());
      }
    }

    void processData(float data){
      sum += data;
      Serial.println(String(sum));
      Serial.println(String(data));
      
      if(cycleCount >= readCount){
        stateData = closeCycle();
        setSleepingMode(true);
        if(!sendData(encodeData())){
          Serial.println("Sending acknowledgement failed");
        };
      }
    }

    void setSleepingMode(bool sleepingValue){
      cycleCount = 0;
      sleeping = sleepingValue;
    }

    virtual float fetchData(){
        Serial.println("*******************");
        int values[12];
        
        mPSoilHumidMod.flushInput();
        for (uint8_t i = 0; i < sizeof(MPSoilHumidityRegistry); i++ ) mPSoilHumidMod.write( MPSoilHumidityRegistry[i] );
        
        mPSoilHumidMod.flush();
        delay(100);
        
        for (int i = 0; i < 7; i++) {
            values[i] = mPSoilHumidMod.read();
        }
        return ((values[3]*256) + values[4]) / fetchDataDevisor;
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
      Serial.println(String(stateData));
      if(isnan(stateData)) {
        return 0;
      }
      return (stateData * mValue) + cValue;
    }

    String packageData(){
      float transformedData = (int) transformData();

      if(transformedData < minValue){
         return "000000"; 
      }
      if(transformedData > maxValue) {
        return "999999"; 
      }
      
      long packagedData = (int) transformedData;
      String packagedDataStr = String(packagedData);
      int packagedDataStrLength = packagedDataStr.length();

      if(packagedData <= 0) return "000000";
      return ((6 - packagedDataStrLength) * '0') + packagedDataStr;
      
      return String(packagedData);
    }

    String encodeData(){
      return String(deviceId) + String(code) + packageData();
    }

    bool sendData(String rs){
     Serial.println(rs);

     int retry_count = 10;
     String ackCode = generateAckCode();

     do {
        // Serial.println(retry_count);
        String slug = ackCode+rs;

        e32ttl100.sendFixedMessage(
          highAdd, 
          serialCommunicatorLowAdd, 
          serialCommunicatorChannel, 
          slug+"\0"
        );

        // Serial.println(slug);
        
        ResponseContainer rc;
        String message;
        int receiveMarker = 10;
        
        do{
          // Serial.println("E");
          rc = e32ttl100.receiveMessage();
          message = rc.data;
          // Serial.println(rc.data);
          delay(10);
        } while(rc.data == "" && receiveMarker-- > 0);
        

        if(message.indexOf(ackCode) != -1 && message.length() > 0){
          return true;
        }

        retry_count--;
     } while(retry_count > 0);

     return false;
   }

  String generateAckCode(int len = 4) {
    const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    String tmp_s;
    int randomInt;
    String new_char;

    for (int i = 0; i < len; ++i) {
      do { 
        randomInt = (analogRead(freeAnalogPin) * random(26))%26;
        new_char = alphanum[randomInt];
      } while(tmp_s.indexOf(alphanum[randomInt]) != -1);
      
      tmp_s += new_char;
    }
    
    return tmp_s;
  }
};


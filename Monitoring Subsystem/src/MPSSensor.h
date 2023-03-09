#include "Arduino.h"
#include <AltSoftSerial.h>

#define DEPin 11

const byte soiltemp[] = {0x01, 0x03, 0x00, 0x13, 0x00, 0x01, 0x75, 0xCF};
const byte soilhumid[] = {0x01, 0x03, 0x00, 0x12, 0x00, 0x01, 0x24, 0x0F};
const byte soilec[] = {0x01, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xCE};
const byte soilph[] = {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B};
const byte soilnitro[] = {0x01, 0x03, 0x00, 0x1E, 0x00, 0x01, 0xE4, 0x0C};
const byte soilphos[] = {0x01, 0x03, 0x00, 0x1F, 0x00, 0x01, 0xB5, 0xCC};
const byte soilpota[] = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xC0};

class MPSSensor: public AbstractSensor {   
  public:
    AltSoftSerial mod;
    int values[12];
    int val1, val2, val3, val4, val5, val6,val7;

    MPSSensor(
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
      mod.begin(9600);
      pinMode(DEPin, OUTPUT);
      digitalWrite(DEPin, LOW);
    }

  virtual long fetchData(){
      mod.begin(9600);
      // Serial.println("------------");
      // Serial.println(code);
      int* tempval = RS485soil(code);
      return ((tempval[3]*256) + tempval[4]) / 10;
  }

  int* RS485soil(byte option) {
    // clear the receive buffer
    mod.flushInput();
    digitalWrite(DEPin, HIGH);
    delay(1);
    // write out the message
    if(option == 31) for (uint8_t i = 0; i < sizeof(soiltemp); i++ ) mod.write( soiltemp[i] );
    if(option == 32) for (uint8_t i = 0; i < sizeof(soilhumid); i++ ) mod.write( soilhumid[i] );
    if(option == 33) for (uint8_t i = 0; i < sizeof(soilec); i++ ) mod.write( soilec[i] );
    if(option == 34) for (uint8_t i = 0; i < sizeof(soilph); i++ ) mod.write( soilph[i] );
    if(option == 35) for (uint8_t i = 0; i < sizeof(soilnitro); i++ ) mod.write( soilnitro[i] );
    if(option == 36) for (uint8_t i = 0; i < sizeof(soilphos); i++ ) mod.write( soilphos[i] );
    if(option == 37) for (uint8_t i = 0; i < sizeof(soilpota); i++ ) mod.write( soilpota[i] );
    // wait for the transmission to complete
    mod.flush();
    digitalWrite(DEPin, LOW);
    // delay to allow response bytes to be received!
    delay(500);
    // read in the received bytes
    for (int i = 0; i < 7; i++) {
      values[i] = mod.read();
    }
    return values;
  }
};


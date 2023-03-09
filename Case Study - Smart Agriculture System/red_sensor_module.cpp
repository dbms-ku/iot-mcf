
#include "Arduino.h"
#include "LoRa_E32.h"
#include <SPI.h>
#include "DHT.h"
#include "LedBlinker.h"

#include "AbstractSensor.h"
#include "MPSSensor.h"
#include "DistanceSensor.h"
#include "AmbientTemperatureSensor.h"
#include "MPSoilHumiditySensor.h"
#include "MPSoilEcSensor.h"
#include "HumiditySensor.h"
#include "RainSensor.h"
#include "SolarVoltageSensor.h"
#include "BatteryVoltageSensor.h"

using namespace std;

#define default_small_sleep_count 100
#define default_long_sleep_count 10000
#define LedPin 10
#define freeAnalogPin A7

#define DEPin 11
#define device 12

const byte soiltemp[] = {0x01, 0x03, 0x00, 0x13, 0x00, 0x01, 0x75, 0xCF};
const byte soilhumid[] = {0x01, 0x03, 0x00, 0x12, 0x00, 0x01, 0x24, 0x0F};
const byte soilec[] = {0x01, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xCE};
const byte soilph[] = {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B};
const byte soilnitro[] = {0x01, 0x03, 0x00, 0x1E, 0x00, 0x01, 0xE4, 0x0C};
const byte soilphos[] = {0x01, 0x03, 0x00, 0x1F, 0x00, 0x01, 0xB5, 0xCC};
const byte soilpota[] = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xC0};

int values[12];
int* tempval;
int val1, val2, val3, val4, val5, val6,val7;
AltSoftSerial mod;



int* RS485soil(char* option) {
  // clear the receive buffer
  mod.flushInput();
  digitalWrite(DEPin, HIGH);
  delay(1);
  // write out the message
  if(option == "TEMPERATURE") for (uint8_t i = 0; i < sizeof(soiltemp); i++ ) mod.write( soiltemp[i] );
  if(option == "HUMIDITY") for (uint8_t i = 0; i < sizeof(soilhumid); i++ ) mod.write( soilhumid[i] );
  if(option == "SOILEC") for (uint8_t i = 0; i < sizeof(soilec); i++ ) mod.write( soilec[i] );
  if(option == "SOILPH") for (uint8_t i = 0; i < sizeof(soilph); i++ ) mod.write( soilph[i] );
  if(option == "NITRO") for (uint8_t i = 0; i < sizeof(soilnitro); i++ ) mod.write( soilnitro[i] );
  if(option == "PHOS") for (uint8_t i = 0; i < sizeof(soilphos); i++ ) mod.write( soilphos[i] );
  if(option == "POTA") for (uint8_t i = 0; i < sizeof(soilpota); i++ ) mod.write( soilpota[i] );
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

// LedBlinker LedBlinkElement(LedPin, 1, 1, HIGH);

DistanceSensor metaDistanceSensor(device, 80, "waterLevel", default_small_sleep_count);
AmbientTemperatureSensor ambientTemperatureSensor(device, 70, "soilTemperature",default_long_sleep_count);
HumiditySensor metaHumiditySensor(device, 20, "humiditySensor",default_long_sleep_count);
RainSensor metaRainSensor(device, 40, "rainSensor",default_long_sleep_count);
BatteryVoltageSensor metaBatteryVoltageSensor(device, 51, "batteryVoltage", default_small_sleep_count);
SolarVoltageSensor metaSolarVoltageSensor(device, 52, "solarVoltage", default_small_sleep_count);

MPSSensor metaMPSTemperatureSensor(device, 31, "soilTemperature",default_long_sleep_count,10,10,200,0,1000);
MPSSensor metaMPSMoistureSensor(device, 32, "soilMoisture",default_long_sleep_count,10,10,0,0,1000);

void setup() {

  Serial.begin(115200);
  
  e32ttl100.begin();

  pinMode(LedPin, OUTPUT);

  // LedBlinkElement.cycleStep();

  randomSeed(analogRead(freeAnalogPin));

  mod.begin(9600);
  pinMode(DEPin, OUTPUT);
  digitalWrite(DEPin, LOW);

  metaDistanceSensor.cycleCount = 0;
  metaBatteryVoltageSensor.cycleCount = 45;
  metaSolarVoltageSensor.cycleCount = 90;
  ambientTemperatureSensor.cycleCount = 150;
  metaHumiditySensor.cycleCount = 210;
  metaRainSensor.cycleCount = 270;
  metaMPSTemperatureSensor.cycleCount = 330;
  metaMPSMoistureSensor.cycleCount = 390;

}

void loop() {
  // LedBlinkElement.cycleStep();
  
  metaDistanceSensor.routine();
  ambientTemperatureSensor.routine();
  metaHumiditySensor.routine();
  metaRainSensor.routine();
  metaBatteryVoltageSensor.routine();
  metaSolarVoltageSensor.routine();
  
  if(metaMPSTemperatureSensor.routine()){
    int* tempval = RS485soil("TEMPERATURE");
    val1 = ((tempval[3]*256) + tempval[4]) / 10;
    metaMPSTemperatureSensor.processData(val1);
  }
  
  if(metaMPSMoistureSensor.routine()){
    int* tempval = RS485soil("HUMIDITY");
    val1 = ((tempval[3]*256) + tempval[4]) / 10;
    metaMPSMoistureSensor.processData(val1);
  }
  
  delay(500);
}
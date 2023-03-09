#include "Arduino.h"
#include <Wire.h>
#include "DFRobot_SHT20.h"
#include <SPI.h>
#include "DHT.h"
#include "LedBlinker.h"


#include "SensorHandler.h"
#include "MPSSensor.h"
#include "DistanceSensor.h"
#include "AmbientTemperatureSensor.h"
// #include "MPSoilHumiditySensor.h"
// #include "MPSoilEcSensor.h"
#include "HumiditySensor.h"
#include "RainSensor.h"
#include "SolarVoltageSensor.h"
#include "BatteryVoltageSensor.h"
#include "Sht20HumiditySensor.h"
#include "Sht20TemperatureSensor.h"

using namespace std;

#define default_small_sleep_count 100
#define default_long_sleep_count 10000
#define LedPin 10

#define device 16

// LedBlinker LedBlinkElement(LedPin, 1, 1, HIGH);
SensorHandler SHandler;

DistanceSensor metaDistanceSensor(device, 80, default_small_sleep_count);
// AmbientTemperatureSensor ambientTemperatureSensor(device, 70,default_long_sleep_count);
// HumiditySensor metaHumiditySensor(device, 20,default_long_sleep_count);
RainSensor metaRainSensor(device, 40, default_long_sleep_count);
BatteryVoltageSensor metaBatteryVoltageSensor(device, 51, default_small_sleep_count);
SolarVoltageSensor metaSolarVoltageSensor(device, 52, default_small_sleep_count);

Sht20HumiditySensor sht20HumiditySensor(device, 20, default_long_sleep_count);
Sht20TemperatureSensor sht20TemperatureSensor(device, 70, default_long_sleep_count);

MPSSensor metaMPSTemperatureSensor(device, 31, default_long_sleep_count,10,10,200,0,1000);
MPSSensor metaMPSMoistureSensor(device, 32, default_long_sleep_count,10,10,0,0,1000);
MPSSensor metaMPSEcSensor(device, 33, default_long_sleep_count,10,10,0,0,1000);
MPSSensor metaMPSPhSensor(device, 34, default_long_sleep_count,10,10,0,0,1000);
MPSSensor metaMPSNitroSensor(device, 35, default_long_sleep_count,10,10,0,0,1000);
MPSSensor metaMPSPhosSensor(device, 36, default_long_sleep_count,10,10,0,0,1000);
MPSSensor metaMPSPotaSensor(device, 37, default_long_sleep_count,10,10,0,0,1000);
// MPSSensor sht20AmbientTempeSensor(device, 70,10,10,10,0,0,1000);
// MPSSensor sht20AmbientHumideSensor(device, 38,10,10,10,0,0,1000);

void setup() {

  Serial.begin(115200);

  pinMode(LedPin, OUTPUT);

  // LedBlinkElement.cycleStep();


  

  metaDistanceSensor.cycleCount = 0;
  metaBatteryVoltageSensor.cycleCount = 45;
  metaSolarVoltageSensor.cycleCount = 90;
  // ambientTemperatureSensor.cycleCount = 150;
  // metaHumiditySensor.cycleCount = 210;
  metaRainSensor.cycleCount = 270;
  metaMPSTemperatureSensor.cycleCount = 330;
  metaMPSMoistureSensor.cycleCount = 390;

}

void loop() {
  
  SHandler.routine(metaDistanceSensor);
  SHandler.routine(metaRainSensor);
  SHandler.routine(metaBatteryVoltageSensor);
  SHandler.routine(metaSolarVoltageSensor);
  SHandler.routine(sht20HumiditySensor);
  SHandler.routine(sht20TemperatureSensor);
  SHandler.routine(metaMPSTemperatureSensor);
  SHandler.routine(metaMPSMoistureSensor);
  SHandler.routine(metaMPSEcSensor);
  SHandler.routine(metaMPSNitroSensor);
  SHandler.routine(metaMPSPhosSensor);
  SHandler.routine(metaMPSPotaSensor);
  
  delay(500);
}
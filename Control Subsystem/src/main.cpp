#include "Arduino.h"
#include "LoRa_E32.h"

#define RelayPin 6
#define ManualPin A1
#define PumpPin A2
#define LEDPin 10
#define SAMPLES 300 

float High_peak,Low_peak;

int LEDLowCntMax = 12;
int LEDHighCntMax = 3;
int LEDCnt = 0;
bool LEDStatus;

bool ManualSwitchStatus = 0;
int PumpStatusReportCnt = 0;
int PumpStatusReportMax = 1000;



LoRa_E32 e32ttl100(2, 3);


void LORASend( String payload){
  ResponseStatus rs = e32ttl100.sendFixedMessage(9,21,0x06,payload);
  //Serial.println(rs.getResponseDescription());
  delay (500);  
  }

String LORARecieve(){
    if (e32ttl100.available()>1) {
    // read the String message
  ResponseContainer rc = e32ttl100.receiveMessage();
  // Is something goes wrong print error
  if (rc.status.code!=1){
    rc.status.getResponseDescription();
  }else {
    // Print the data received
    //Serial.println(rc.data);
  }
  return rc.data;
  }

  else{
    return "";
    }
  }

bool read_Amps(uint8_t CurrentPin){ 
  float AmpSum =0;
  int AvgCnt = 10;
  
  for (int i = 0; i<AvgCnt; i++){
    int cnt;
    High_peak = 0;
    Low_peak = 1024;
    
    for(cnt=0 ; cnt<SAMPLES ; cnt++){
          float ACS_Value = analogRead(CurrentPin);
   
          if(ACS_Value > High_peak)
              {
                High_peak = ACS_Value;
              }        
          if(ACS_Value < Low_peak)
              {
                Low_peak = ACS_Value;
              }         
    }
    AmpSum += (High_peak - Low_peak)*0.3536*0.06;
    }
  //Serial.println(AmpSum/AvgCnt); 
    if ((AmpSum/AvgCnt) < 0.09){
      return 0;
      }
    return 1;
}

String FetchPumpStatus(){
  ManualSwitchStatus =  read_Amps(ManualPin);
  //Serial.println (ManualSwitchStatus);
  Serial.println("4444444"+ String(ManualSwitchStatus)+ String(read_Amps(PumpPin))+ String(digitalRead(RelayPin)));
  PumpStatusReportCnt = 0;
  return "4444444"+ String(ManualSwitchStatus)+ String(read_Amps(PumpPin))+ String(digitalRead(RelayPin))+"\0";
  }

void LEDStep(){
  LEDCnt++;
  if (LEDStatus ==0 &&LEDCnt >= LEDLowCntMax){
    digitalWrite(LEDPin,HIGH);
    LEDStatus =1;
    LEDCnt = 0;

    return;
    }
  if (LEDStatus ==1 &&LEDCnt >= LEDHighCntMax){
   digitalWrite(LEDPin,LOW);
   LEDStatus = 0;
   LEDCnt = 0;

   return;
    }  
  }

void PumpStatusReportInterval(){
  PumpStatusReportCnt++;
  //Serial.println (PumpStatusReportCnt);
  if (PumpStatusReportCnt>= PumpStatusReportMax){
    LORASend(FetchPumpStatus());
    
  }
}
 

// -------------------------------------
void setup() {
  Serial.begin(9600);
  delay(500);
  // Startup all pins and UART
  e32ttl100.begin();

  Serial.println("Hi, I'm listening!");
  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, LOW);
  pinMode(ManualPin,INPUT); 
  pinMode(PumpPin,INPUT); 
  pinMode(LEDPin,OUTPUT);
  digitalWrite(LEDPin, LOW); 
}

void loop() {
  //Serial.println("---------------Looping------------");
  LEDStep();
  PumpStatusReportInterval();

  bool Manualcheck = true;
  String result = LORARecieve();

  if(result == "10"){
    Manualcheck = false;
    digitalWrite(RelayPin, HIGH);  
    delay(500);
    LORASend(FetchPumpStatus());
    }

  else if(result == "20"){
    Manualcheck = false;
    digitalWrite(RelayPin, LOW); 
    delay(500);
    LORASend(FetchPumpStatus());  
    }

  else if(result == "30"){
    Manualcheck = false;
    delay(500);
    LORASend(FetchPumpStatus());
    }
    
  else delay (500);

  if (Manualcheck && ManualSwitchStatus != read_Amps(ManualPin)){
    LORASend(FetchPumpStatus());
  }

}


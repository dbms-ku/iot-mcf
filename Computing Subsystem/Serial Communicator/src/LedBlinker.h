class LedBlinker {
    public:
      int lowCycleMax;
      int highCycleMax; 
      int cycleCount; 
      int blinkState;
      int ledOutputPin;
  
      LedBlinker(int ledPinValue, int lowBlinkCycleMax, int highBlinkCycleMax, int startBlinkState){
        lowCycleMax = lowBlinkCycleMax;
        highCycleMax = highBlinkCycleMax;
        cycleCount = 0;
        blinkState = startBlinkState;
        ledOutputPin = ledPinValue;
      }

      void cycleStep(){
        cycleCount++;
        
        if(blinkState == HIGH && cycleCount >= highCycleMax){
          updateState(LOW);
          return;
        } 
        
        if(blinkState == LOW && cycleCount >= lowCycleMax){
          updateState(HIGH);
          return;
        } 
      }

      void updateState(int stateValue){
        digitalWrite(ledOutputPin, stateValue);
        blinkState = stateValue;
        cycleCount = 0;
      }
};

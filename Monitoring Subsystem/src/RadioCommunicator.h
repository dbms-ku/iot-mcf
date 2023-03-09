#include "Arduino.h"
#include "LoRa_E32.h"

#define comHighAdd 13
#define comLowAdd 21
#define comChannel 6

#define freeAnalogPin A7
#define alphanum "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

// LoRa_E32 e32ttl100(2, 3, 5, 7, 6);
LoRa_E32 e32ttl100(2,3);


class RadioCommunicator {   
  public:
    RadioCommunicator(){
        randomSeed(analogRead(freeAnalogPin));
    }

    String generateAckCode(int len = 4) {
        // const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        
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

    bool sendData(String rs){

        e32ttl100.begin();
        Serial.println(rs);

        int retry_count = 10;
        String ackCode = generateAckCode();

        do {
            String slug = ackCode+rs;

            e32ttl100.sendFixedMessage(
                comHighAdd, 
                comLowAdd, 
                comChannel, 
                slug+"\0"
            );

            delay(500);
            
            ResponseContainer rc;
            String message;
            int receiveMarker = 10;
            
            Serial.println("S");
            
            do{
            Serial.println("R");
            rc = e32ttl100.receiveMessage();
            message = rc.data;
            } while(rc.data == "" && receiveMarker-- > 0);
            

            if(message.indexOf(ackCode) != -1 && message.length() > 0){
            Serial.println("Message sent successfully");

            return true;
            }

            retry_count--;
        } while(retry_count > 0);

        return false;
    } 
};
// #include "Arduino.h"
// #include "LoRa_E32.h"

// #define messageUnitLenght 14

// LoRa_E32 e32ttl100(2, 3); // e32 TX e32 RX

// int registered_channels[9] = {6,6,6,6,6,6,6,6,6};
// int registered_low_addresses[9] = {21, 21, 21, 21, 21, 21, 21, 21, 21};
// int registered_high_addresses[9] = {7, 5, 5, 5, 5, 5, 5, 5, 5};

// int next_ack_code_ind = 0;
// const int recent_ack_codes_queue_size = 5;
// String recent_ack_codes[recent_ack_codes_queue_size] = {};

// void setup() {
//   Serial.begin(115200);
//   delay(500);

//   // Startup all pins and UART
//   e32ttl100.begin();

//   // Send message
//   ResponseStatus rs = e32ttl100.sendMessage("Hello, world?");
//   // Check If there is some problem of successfully send
//   Serial.println(rs.getResponseDescription());
// }

// bool recordAckCode(String ack_code){
//     bool code_is_familiar = false;
//     for (int i = 0; i < recent_ack_codes_queue_size; i++)
//     {
//         if(recent_ack_codes[i] == ack_code) code_is_familiar = true;
//     }

//     if(!code_is_familiar){
//         recent_ack_codes[next_ack_code_ind] = ack_code;
//         if(next_ack_code_ind >= recent_ack_codes_queue_size - 1){
//             next_ack_code_ind = 0;
//         } else {
//             next_ack_code_ind++;
//         }
//     }
    
//     return !code_is_familiar;
// }

// String fetchRadioMessage(){
//     int retry = 100;
//     while(e32ttl100.available()<=1 && retry-- > 0){
//         // Serial.println("--rc.data");
//         delay(50);
//     }

//     String res = "";

//     // if(retry > 0){
//     ResponseContainer rc = e32ttl100.receiveMessageUntil("\0");
    
//     if (rc.status.code!=1){
//         return "";
//     } else {
//         return rc.data;
//     }
//     // }
// }

// void acknowledgeReceipt(String message){
//     String ack_code = message.substring(0,4);
//     String device_id_str = message.substring(4, 6);
    
//     e32ttl100.sendFixedMessage(
//         registered_high_addresses[atoi(device_id_str.c_str())-10], 
//         registered_low_addresses[atoi(device_id_str.c_str())-10], 
//         registered_channels[atoi(device_id_str.c_str())-10],
//         ack_code
//         );
// }

// bool sendRadioMessage(int HighAddress, int LowAddress, int Channel, String message){
//     e32ttl100.sendFixedMessage(HighAddress, LowAddress, Channel, message+"\0");
//     return true;
// }

// void sendRadioMessageAcknowledgement(String messageSection){
    
//     if(messageSection == "44444444444444") return;
//     String device_id_str = messageSection.substring(4, 6);

//     sendRadioMessage(
//         registered_high_addresses[atoi(device_id_str.c_str())-10], 
//         registered_low_addresses[atoi(device_id_str.c_str())-10], 
//         registered_channels[atoi(device_id_str.c_str())-10], 
//         messageSection.substring(0, 4)
//     );
// }

// void loop() {
//     delay(5);

//     String radioMessage = fetchRadioMessage();
//     String messageSection;
//     int messageLength = radioMessage.length(), startIndex = 0, endIndex;

//     while(startIndex < messageLength){
//         endIndex = startIndex+messageUnitLenght;
//         if(endIndex >= messageLength) endIndex = messageLength;
//         messageSection = radioMessage.substring(startIndex, endIndex);

//         acknowledgeReceipt(messageSection);

//         if(recordAckCode(messageSection.substring(0, 4))){
//             Serial.println(messageSection.substring(4, 14));
//             sendRadioMessageAcknowledgement(messageSection);
//             startIndex += messageUnitLenght;
//         }
//         startIndex += messageUnitLenght;
//     }
// }
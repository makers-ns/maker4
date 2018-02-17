/*
 * Compatible with:
 * SODAQ MBILI
 * SODAQ Autonomo
 * SODAQ ONE
 * SODAQ ONE BETA
 * SODAQ EXPLORER
 */

#include "Arduino.h"


#define debugSerial Serial 
#define loraSerial Serial1
#define LORA_RESET 33

void setup() {
  // Enable LoRa module

  // Hard reset the RN module
  #if defined(LORA_RESET)
  pinMode(LORA_RESET, OUTPUT);
  digitalWrite(LORA_RESET, LOW);
  delay(100);
  digitalWrite(LORA_RESET, HIGH);
  delay(100);
  #endif

  while ((!debugSerial) && (millis() < 10000)){
    // wait 10 seconds for serial monitor
  }
  
  debugSerial.begin(57600);
  loraSerial.begin(57600); 
  
  debugSerial.println("Please send command");
}

void loop() {  

    //send and receive data with serial
     if (debugSerial.available()){
      //debugSerial.print("SEND:    ");
      while (debugSerial.available()) {
        uint8_t inChar = debugSerial.read();
        //debugSerial.write(inChar);
        loraSerial.write(inChar);
      }
     }

     if (loraSerial.available()){
      //debugSerial.print("RECEIVE: ");
      while (loraSerial.available()) {
        uint8_t inChar = loraSerial.read();
        debugSerial.write(inChar);
      }
     }
}

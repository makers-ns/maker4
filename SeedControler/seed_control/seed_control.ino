/*

*/
#include "seeder.h"

unsigned long loopTime = 0;
unsigned long sentTime = 0;
unsigned long sendPeriod = 300; // delay between sending data

SEEDC seedc;

void setup() {
  
  Serial1.begin(9600);
  seedc.init();
  attachInterrupt(seedc.getSeedHWPin(0),sensor0,FALLING);
  attachInterrupt(seedc.getSeedHWPin(1),sensor1,FALLING);
  attachInterrupt(seedc.getSeedHWPin(2),sensor2,FALLING);
  attachInterrupt(seedc.getSeedHWPin(3),sensor3,FALLING);
  attachInterrupt(seedc.getSeedHWPin(4),sensor4,FALLING);
  attachInterrupt(seedc.getSeedHWPin(5),sensor5,FALLING);

  seedc.start(); // OVO POSLE PREBACITI U LOOP NA SLUSANJE KOMANDI

}

void loop() {
  
  loopTime = millis();
  
  if((loopTime - sentTime) > sendPeriod){
    sentTime = loopTime;
    
    char csvPayload[255];
    //SEEDC::payload myCSV;
    seedc.buildCSV();
    /*
    snprintf(csvPayload, sizeof(csvPayload),"%d,%d,%d,%d,%d,%d"
      ,seedc.getSeedCount(0)
      ,seedc.getSeedCount(1)
      ,seedc.getSeedCount(2)
      ,seedc.getSeedCount(3)
      ,seedc.getSeedCount(4)
      ,seedc.getSeedCount(5));  
      
    Serial1.println(csvPayload);
    */
    Serial1.println(seedc.payloadCSV);
    // Resetuj brojace
    seedc.resetCounters();
  }
}

void sensor0(){
  seedc.addImpulse(0);
}
void sensor1(){
  seedc.addImpulse(1);
}

void sensor2(){
  seedc.addImpulse(2);
}
void sensor3(){
  seedc.addImpulse(3);
}
void sensor4(){
  seedc.addImpulse(4);
}
void sensor5(){
  seedc.addImpulse(5);
}

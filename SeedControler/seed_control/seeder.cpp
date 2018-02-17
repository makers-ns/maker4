/*

*/

#include "seeder.h"
#include "Arduino.h"

SEEDC::SEEDC(){
  
};

bool SEEDC::init(){
  
  //SVE PINOVE NA INPUT_PULLUP
  for (int i = 0;i<6;i++)  {
    pinMode(_seedHWPins[i],INPUT_PULLUP);
  };
  
  return true;
};

bool SEEDC::start(){

  _resetCounters();

  modeState = Mode_Work; 
  return true;
}

void SEEDC::addImpulse(uint8_t indeks){
  _seedCounter[indeks]++;
}

uint16_t SEEDC::getSeedCount(uint8_t indeks){
  return _getSeedCount(indeks);
}

uint8_t SEEDC::getSeedHWPin(uint8_t indeks){
  return _seedHWPins[indeks];
}

void SEEDC::resetCounters(){
  _resetCounters();
}

void SEEDC::_resetCounters(){
  for (int i = 0;i<6;i++)  {
    _seedCounter[i] = 0;
  };
}

/*
void SEEDC::buildCSV(payload inBuffer){
    // Problem sa sizeof(inBuffer), trebao bi da vrati 255
     snprintf(inBuffer, 255 ,"%d,%d,%d,%d,%d,%d",_seedCounter[0],_seedCounter[1],_seedCounter[2],_seedCounter[3],_seedCounter[4],_seedCounter[5]);   
}
*/
void SEEDC::buildCSV(){
    // Problem sa sizeof(inBuffer), trebao bi da vrati 255
     snprintf(payloadCSV, 255 ,"%d,%d,%d,%d,%d,%d",_seedCounter[0],_seedCounter[1],_seedCounter[2],_seedCounter[3],_seedCounter[4],_seedCounter[5]);   
}

//PRIVATE + WORKER
uint16_t SEEDC::_getSeedCount(uint8_t indeks){
  return _seedCounter[indeks];
}

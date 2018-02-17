
#ifndef MKNS_SEEDC_H
#define MKNS_SEEDC_H

#include "Arduino.h"


#define REGISTERSIZE 32

class SEEDC
{
  public:
    
    //typedef char payload [255];
    char payloadCSV[255];
    
    enum Mode
    {
      Mode_Sleep = 0,
      Mode_Work = 1
    };
      
    enum SampleTime
    {
      Sample_025 = 0,
      Sample_050 = 1,
      Sample_100 = 2,
      Sample_150 = 3,
      Sample_200 = 4
    };
    
    SEEDC();
    bool init();
    
    bool start();
    bool stop();
    
    void addImpulse(uint8_t);
    uint8_t getSeedHWPin(uint8_t);
    void resetCounters();
    uint16_t getSeedCount(uint8_t);
    void buildCSV();
    
  private:
    Mode modeState;
    
    uint16_t _seedCounter[6] = {0,0,0,0,0,0};
    uint8_t _seedHWPins[6] = {0,1,4,5,6,7};
    
    void _resetCounters();
    uint16_t _getSeedCount(uint8_t);
};

#endif

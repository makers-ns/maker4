#include "flip_click_defs.h"
#include <rn2xx3.h>

/*
union {
    float float_variable;
    byte temp_array[4];
} u;
*/
union {
    int32_t float_variable;
    byte temp_array[4];
} u;


uint8_t txBuffer[12];
//create an instance of the rn2483 library, using the given Serial port
rn2xx3 myLora(Serial1);


// the setup routine runs once when you press reset:
void setup()
{
  //output LED pin
  pinMode(13, OUTPUT);
  led_on();

  // Open serial communications and wait for port to open:
  Serial.begin(57600); //serial port to computer
  Serial1.begin(57600); //serial port to radio

  while ((!Serial) && (millis() < 10000));

  Serial.println("Startup");

  initialize_radio();
  
  //transmit a startup message
  //myLora.tx("TTN Mapper on TTN Uno node");

  led_off();
  delay(2000);
}

// the loop routine runs over and over again forever:
void loop()
{
    led_on();
    Serial.println("Meassuring");

    Serial.println("TXing");
    for( int ii = 64; ii < 76; ii++){
      txBuffer[ii] = ii;
    }
    
    myLora.txBytes(txBuffer,12); 

    led_off();
    delay(15*1000);
}



void led_on()
{
  digitalWrite(13, 1);
}

void led_off()
{
  digitalWrite(13, 0);
}


void initialize_radio()
{
  delay(100); //wait for the RN2xx3's startup message
  Serial1.flush();

  //print out the HWEUI so that we can register it via ttnctl
  String hweui = myLora.hweui();
  while(hweui.length() != 16)
  {
    Serial.println("Communication with RN2xx3 unsuccessful. Power cycle the TTN UNO board.");
    delay(10000);
    hweui = myLora.hweui();
  }
  Serial.println("When using OTAA, register this DevEUI: ");
  Serial.println(hweui);
  Serial.println("RN2xx3 firmware version:");
  Serial.println(myLora.sysver());

  //configure your keys and join the network
  Serial.println("Trying to join TTN");
  bool join_result = false;

  //ABP: initABP(String addr, String AppSKey, String NwkSKey);
  //join_result = myLora.initABP("26011D1B", "C65F05E833BE12DC01D55839FFDBD7CC", "8CDD9DE197841E9AC6C6A12EC5E1C186");

  //OTAA: initOTAA(String AppEUI, String AppKey);
  join_result = myLora.initOTAA("70B3D57ED0008934", "C572E966421E2042893109457270CB53","0004A30B001B96DD");

  while(!join_result)
  {
    Serial.println("Unable to join. Are your keys correct, and do you have TTN coverage?");
    delay(60000); //delay a minute before retry
    join_result = myLora.init();
  }
  Serial.println("Successfully joined TTN");

}


#include <Wire.h>

#define OPT3001_ADDRESS 0x44
uint8_t buf[2];
uint16_t data;

typedef union {
  uint16_t rawData;
  struct {
    uint16_t Result : 12;
    uint8_t Exponent : 4;
  };
} OPT3001_ER;

OPT3001_ER er;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("ClosedCube OPT3001 Arduino Test");
  Wire.begin();
  Wire.beginTransmission(OPT3001_ADDRESS);
  Wire.write(0x01); // Registar 01 je konfiguracioni
  Wire.endTransmission();
  Wire.beginTransmission(OPT3001_ADDRESS);
  Wire.write(0xC6);
  Wire.endTransmission();

}

void loop() {
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(OPT3001_ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission(); 
  readData(&er.rawData);
  int lux = 0.01*pow(2, er.Exponent)*er.Result;
  Serial.println(lux);
  delay(500);
}

void readData(uint16_t* data) {
    uint8_t buf[2];

  Wire.requestFrom(OPT3001_ADDRESS, 2);

  int counter = 0;
  while (Wire.available() < 2)
  {
    counter++;
    delay(10);
    if (counter > 250)
      return;
  }

  Wire.readBytes(buf, 2);
  *data = (buf[0] << 8) | buf[1];

  return;
}


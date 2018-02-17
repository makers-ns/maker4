HardwareSerial Serial2(2);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 18);
  
}

void loop()
{
  while (Serial2.available()) 
  {
    Serial.write((char)Serial2.read());
  }
}

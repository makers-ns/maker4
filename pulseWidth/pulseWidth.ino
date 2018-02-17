uint32_t w = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(12,INPUT_PULLUP);
  SerialUSB.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  w = pulseIn(12,HIGH,20*1000);
  SerialUSB.println(w);
}

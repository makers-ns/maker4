#include "Mouse.h"

void setup() {
  // put your setup code here, to run once:
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  Serial.begin(9600);
  Mouse.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(2) == 0){
    Mouse.move(-5,0);
  }
  if (digitalRead(3) == 0){
    Mouse.move(5,0);
  }
  if (digitalRead(4) == 0){
    Mouse.move(0,-5);
  }
  if (digitalRead(5) == 0){
    Mouse.move(0,5);
  }
  if (digitalRead(6) == 0){
    Mouse.click(MOUSE_LEFT);
    delay(300);
  }
  delay(50);
}

/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Sketch generator:           http://examples.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social networks:            http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 * This example code is in public domain.
 *
 **************************************************************
 *
 * This example shows how to use Arduino 101 CurieBLE
 * to connect your project to Blynk.
 *
 * Note: This requires CurieBLE library
 *   from http://librarymanager/all#CurieBLE
 *
 * NOTE: BLE support is in beta!
 *
 **************************************************************/

//#define BLYNK_USE_DIRECT_CONNECT

#define BLYNK_PRINT Serial

#include <BlynkSimpleCurieBLE.h>
#include <CurieBLE.h>

#include <Servo.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "e24a1b28cff146e3b73fc88ee5a2e1b5";

BLEPeripheral  blePeripheral;

Servo m1,m2,m3,m4,m56,m7;

BLYNK_WRITE(V3)
{
  m1.writeMicroseconds(param.asInt());
}

BLYNK_WRITE(V4)
{
  m2.writeMicroseconds(param.asInt());
}

BLYNK_WRITE(V5)
{
  m3.writeMicroseconds(param.asInt());
}

BLYNK_WRITE(V6)
{
  m4.writeMicroseconds(param.asInt());
}

BLYNK_WRITE(V7)
{
  m56.writeMicroseconds(param.asInt());
}

BLYNK_WRITE(V8)
{
  m7.writeMicroseconds(param.asInt());
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  blePeripheral.setLocalName("doktor");
  blePeripheral.setDeviceName("doktor");
  blePeripheral.setAppearance(384);

  Blynk.begin(blePeripheral, auth);

  blePeripheral.begin();

  /*
   * Prikacivanje motora
   */

   m1.attach(6);
   m2.attach(2);
   m3.attach(4);
   m4.attach(3);
   m56.attach(7);
   m7.attach(5);

  Serial.println("Waiting for connections...");
}

void loop() {
  Blynk.run();
  blePeripheral.poll();
}


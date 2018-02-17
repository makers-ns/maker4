/*
   Copyright 2014-2016 AllThingsTalk

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*/

#include <LTask.h>
#include <LWiFi.h>
#include <LWiFiClient.h>

#include <PubSubClient.h>
#include <ATT_IOT.h>
/*
  AllThingsTalk Makers LinkIt Example 

  ### Instructions

  1. Setup the Arduino hardware
    - Grove kit shield
    - Potentiometer to A0
    - Led light to D8
  2. Add 'allthingstalk_arduino_standard_lib' library to your Arduino Environment. [Try this guide](http://arduino.cc/en/Guide/Libraries)
  3. Fill in the missing strings (deviceId, clientId, clientKey, Wifi details) and optionally change/add the sensor & actuator names, ids, descriptions, types
     For extra actuators, make certain to extend the callback code at the end of the sketch.
  4. Upload the sketch

  ### Troubleshooting

  1. 'Device' type is reported to be missing. 
  - Make sure to properly add the arduino/libraries/ATT_IOT/ library
  2. No data is showing up in the cloudapp
  - Make certain that the data type you used to create the asset is the expected data type. Ex, when you define the asset as 'int', don't send strings or boolean values.
*/

#define WIFI_AP "YOUR_WIFI_SSID"                // replace your WiFi AP SSID
#define WIFI_PASSWORD "YOUR_WIFI_PWD"           // replace your WiFi AP password
#define WIFI_AUTH LWIFI_WPA                     // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP according to your AP

char deviceId[] = "YOUR_DEVICE_ID_HERE";
char clientId[] = "YOUR_CLIENT_ID_HERE";
char clientKey[] = "YOUR_CLIENT_KEY_HERE";

//Warning the LinkIt maps some pin numbers differently compared to the arduino. LinkIt pin numbers can be 
//bigger then 9, which doesn't work well with the AllthingsTalk library: it expects asset id's between 0 and 9, 
//the solution: define a different asset id for each pin, which is in the correct range.
int a0=A0;
int a0Id=0;
int a1=8;
int a1Id=1;

ATTDevice Device(deviceId, clientId, clientKey);                //create the object that provides the connection to the cloud to manager the device.
#define httpServer "api.AllThingsTalk.io"                  // HTTP API Server host                  
#define mqttServer httpServer                				// MQTT Server Address 

//required for the device
void callback(char* topic, byte* payload, unsigned int length);
LWiFiClient c;
PubSubClient pubSub(mqttServer, 1883, callback, c);

void setup()
{
  Serial.begin(115200);
  while(!Serial);                                     //for the linkit, we need to wait until the serial monitor is initialized correctly, if we don't do this, we don't get to see the logging.
                                                      //Warning for battery and mains power usage, comment out the above line, other wise the setup() will continue to wait for the Serial to become available
  pinMode(a1, OUTPUT);

  Serial.println("starting");
  LWiFi.begin();                                                        // Initializes LinkIt ONE WiFi module   
  Serial.print("Connecting to WiFi AP: ");
  Serial.println(WIFI_AP);
  while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD)))
    delay(1000);
  Serial.println("connected");

  while(!Device.Connect(&ethClient, httpServer))                          // connect the device with the IOT platform.
    Serial.println("retrying");
  Device.AddAsset(a0Id, "knob", "rotary switch",false, "integer");
  Device.AddAsset(a1Id, "led", "light emitting diode", true, "boolean");
  while(!Device.Subscribe(pubSub))                                      // make certain that we can receive message from the iot platform (activate mqtt)
    Serial.println("retrying"); 
}

unsigned long time;                                                      //only send every x amount of time.
unsigned int prevVal = 0;
void loop()
{  
  unsigned long curTime = millis();
  if (curTime > (time + 1000))                                          // publish light reading every 5 seconds to sensor 1
  {
    unsigned int lightRead = analogRead(a0);                            // read from light sensor (photocell)
    if(prevVal != lightRead){
      Device.Send(String(lightRead), a0Id);
      prevVal = lightRead;
    }
    time = curTime;
  }
  Device.Process(); 
}

// Callback function: handles messages that were sent from the iot platform to this device.
void callback(char* topic, byte* payload, unsigned int length) 
{ 
  String msgString; 
  {                                                            //put this in a sub block, so any unused memory can be freed as soon as possible, required to save mem while sending data
    char message_buff[length + 1];                        //need to copy over the payload so that we can add a /0 terminator, this can then be wrapped inside a string for easy manipulation.
    strncpy(message_buff, (char*)payload, length);        //copy over the data
    message_buff[length] = '\0';                      //make certain that it ends with a null           
          
    msgString = String(message_buff);
    msgString.toLowerCase();                  //to make certain that our comparison later on works ok (it could be that a 'True' or 'False' was sent)
  }
  int* idOut = NULL;
  {                                                       //put this in a sub block, so any unused memory can be freed as soon as possible, required to save mem while sending data
    int pinNr = Device.GetPinNr(topic, strlen(topic));
    
    Serial.print("Payload: ");                            //show some debugging.
    Serial.println(msgString);
    Serial.print("topic: ");
    Serial.println(topic);
    
    if (pinNr == a1Id)
    {
      if (msgString == "false") {
        Serial.println("LED off");  
            digitalWrite(a1, LOW);           //change the LED status to false
            idOut = &a1Id;                              
      }
      else if (msgString == "true") {
        Serial.println("LED on"); 
        digitalWrite(a1, HIGH);              //change the LED status to true
            idOut = &a1Id;
      }
    }
  }
  if(idOut != NULL)                //also let the iot platform know that the operation was succesful: give it some feedback. This also allows the iot to update the GUI's correctly & run scenarios.
    Device.Send(msgString, *idOut);    
}


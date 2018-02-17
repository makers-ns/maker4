/*    _   _ _ _____ _    _              _____     _ _     ___ ___  _  __
 *   /_\ | | |_   _| |_ (_)_ _  __ _ __|_   _|_ _| | |__ / __|   \| |/ /
 *  / _ \| | | | | | ' \| | ' \/ _` (_-< | |/ _` | | / / \__ \ |) | ' <
 * /_/ \_\_|_| |_| |_||_|_|_||_\__, /__/ |_|\__,_|_|_\_\ |___/___/|_|\_\
 *                             |___/
 *
 * Copyright 2017 AllThingsTalk
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//#include <Ethernet2.h>
//#include <Ethernet.h>
//#include <EthernetClient.h>


#include <ESP8266WiFi.h>

#include <PubSubClient.h>

#include <ATT_IOT.h>  // AllThingsTalk for Makers Arduino Library
#include <SPI.h>      // required to have support for signed/unsigned long type                   

const char* ssid     = "TP-LINK";
const char* password = "Samba2015";

// define device credentials and endpoint
char deviceId[] = "oxeYA0IdzwdYJNC7Wr62AOf0";
char token[] = "maker:4Uaaal4eJTDXW1VeVvBo95s14v0b9yrUVz47Fio";
#define httpServer "api.allthingstalk.io"  // API endpoint

ATTDevice Device(deviceId, token);  // create the object that provides the connection to the cloud to manager the device
#define mqttServer httpServer       // MQTT Server Address

int DOORBELL = 8;

//required for the device
void callback(char* topic, byte* payload, unsigned int length);
WiFiClient ethClient;
PubSubClient pubSub(mqttServer, 1883, callback, ethClient);

void setup()
{
  pinMode(DOORBELL, INPUT);
  
  Serial.begin(9600);  // init serial link for debugging
  Serial.println("Evo me");
  setup_wifi();
  delay(1000);
  
  while(!Device.connect(&ethClient, httpServer))  // connect the device with the IOT platform.
    Serial.println("retrying");
    
  Device.addAsset("Doorbell", "Doorbell", "Doorbell button", "sensor", "boolean");  // create the asset for your device
  
  while(!Device.subscribe(pubSub))  // make certain that we can receive message from the iot platform (activate mqtt)
    Serial.println("retrying");     
}

bool button = false;
void loop()
{
  bool buttonValue = digitalRead(DOORBELL);  // read button status
  if (button != buttonValue)                 // verify if value has changed
  {
    button = buttonValue;
    delay(100);
    if (buttonValue == 1)
       Device.send("true", "Doorbell");
    else
       Device.send("false", "Doorbell");
  }
  Device.process(); 
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// callback function: handles messages that were sent from the iot platform to this device.
void callback(char* topic, byte* payload, unsigned int length) 
{
}

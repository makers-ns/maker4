// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.


// Use Arduino IDE 1.6.8 or later.
#include <AzureIoTHubMQTTClient.h>
#include <SPI.h>

// for ESP8266
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>


static char ssid[] = "TP-LINK";     // your network SSID (name)
static char pass[] = "Samba2015";    // your network password (use for WPA, or use as key for WEP)

#define IOTHUB_HOSTNAME         "dudule.azure-devices.net"
#define DEVICE_ID               "espmqtt"
#define DEVICE_KEY              "0LK1KvH+gJ65DnBudXuQZdynEoRaeg7xH2TVaO9TBdM=" //Primary key of the device

#define USE_BMP180              0 //Set this to 0 if you don't have the sensor and generate random sensor value to publish

WiFiClientSecure tlsClient;
AzureIoTHubMQTTClient client(tlsClient, IOTHUB_HOSTNAME, DEVICE_ID, DEVICE_KEY);
//WiFiEventHandler  e1, e2;
unsigned long lastMillis = 0;

void connectToIoTHub(); // <- predefine connectToIoTHub() for setup()
void onMessageCallback(const MQTT::Publish& msg);

void setup() {
    initSerial();
    initWifi();
    //initTime();
}



void initSerial() {
    // Start serial and initialize stdout
    Serial.begin(115200);
    while(!Serial);
    Serial.setDebugOutput(true);
}

void initWifi() {

    // check for the presence of the shield :
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present");
        // don't continue:
        while (true);
    }

    // attempt to connect to Wifi network:
    Serial.println("");
    Serial.println("InitWiFi");
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    WiFi.begin(ssid, pass);

    Serial.println("Waiting for Wifi connection.");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print("#");
        delay(500);
    }

    Serial.println("Connected to wifi");
    connectToIoTHub();
}

void connectToIoTHub() {

    Serial.print("\nBeginning Azure IoT Hub Client... ");
    if (client.begin()) {
        Serial.println("OK");
    } else {
        Serial.println("Could not connect to MQTT");
    }
}
void readSensor(float *temp, float *press) {

    *temp = 20 + (rand() % 10 + 2);
    *press = 90 + (rand() % 8 + 2);

}

void loop() {
  client.run();
      if (client.connected()) {

        // Publish a message roughly every 3 second. Only after time is retrieved and set properly.
        if(millis() - lastMillis > 3000 && timeStatus() != timeNotSet) {
            lastMillis = millis();

            //Read the actual temperature from sensor
            float temp, press;
            readSensor(&temp, &press);

            //Get current timestamp, using Time lib
            time_t currentTime = now();

            // You can do this to publish payload to IoT Hub
            /*
            String payload = "{\"DeviceId\":\"" + String(DEVICE_ID) + "\", \"MTemperature\":" + String(temp) + ", \"EventTime\":" + String(currentTime) + "}";
            Serial.println(payload);

            //client.publish(MQTT::Publish("devices/" + String(DEVICE_ID) + "/messages/events/", payload).set_qos(1));
            client.sendEvent(payload);
            */

            //Or instead, use this more convenient way
            AzureIoTHubMQTTClient::KeyValueMap keyVal = {{"MTemperature", temp}, {"MPressure", press}, {"DeviceId", DEVICE_ID}, {"EventTime", currentTime}};
            client.sendEventWithKeyVal(keyVal);
        }
    }
    else {
      Serial.println("not connected");
      delay(1000);
    }

  delay(1000);
}

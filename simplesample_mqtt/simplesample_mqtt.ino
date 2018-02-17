// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.


// Use Arduino IDE 1.6.8 or later.

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <SPI.h>

// for ESP8266
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>

#include <AzureIoTHub.h>
#include <AzureIoTUtility.h>
#include <AzureIoTProtocol_MQTT.h>

#include "simplesample_mqtt.h"

static char ssid[] = "TP-LINK";     // your network SSID (name)
static char pass[] = "Samba2015";    // your network password (use for WPA, or use as key for WEP)


void setup() {
    initSerial();
    initWifi();
    initTime();
}

void loop() {
    simplesample_mqtt_run();
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
}

void initTime() {
#if defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_FEATHER_M0)
#ifdef ARDUINO_SAMD_FEATHER_M0
    Adafruit_WINC1500UDP ntpUdp; // for Adafruit WINC1500
#else
    WiFiUDP ntpUdp;
#endif
    NTPClient ntpClient(ntpUdp);

    ntpClient.begin();

    while (!ntpClient.update()) {
        Serial.println("Fetching NTP epoch time failed! Waiting 5 seconds to retry.");
        delay(5000);
    }

    ntpClient.end();

    unsigned long epochTime = ntpClient.getEpochTime();

    Serial.print("Fetched NTP epoch time is: ");
    Serial.println(epochTime);

#elif ARDUINO_ARCH_ESP8266
    time_t epochTime;

    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    while (true) {
        epochTime = time(NULL);

        if (epochTime == 0) {
            Serial.println("Fetching NTP epoch time failed! Waiting 2 seconds to retry.");
            delay(2000);
        } else {
            Serial.print("Fetched NTP epoch time is: ");
            Serial.println(epochTime);
            break;
        }
    }
#endif
}

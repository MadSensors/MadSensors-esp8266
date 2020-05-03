// MadSensors - Send Value Example
//
// Author: Jose Zorrilla <jose@madsensors.com>
// Website: https://madsensors.com
// Description: This example create the variable temperature = 10 and increments it in steps of 10 units until 40
// This sketch is only to show how to use the MadSensors library for ESP8266

#include <MadSensors-esp8266.h>

// MadSensors Token (Must be created using the MadSensors platform)
#ifndef TOKEN
#define TOKEN "replace_with_the_madsensors_token"
#endif

// Your WiFi Credentials (replace with your WiFi credentials)
#ifndef WIFISSID
#define WIFISSID "my_wifi_ssid"
#endif

#ifndef WIFIPASS
#define WIFIPASS "my_wifi_password"
#endif

// Device Name (Must be created using the MadSensors platform)
#ifndef DEVICE_NAME
#define DEVICE_NAME "NodeMCU_device_name"
#endif

// Create the MadSensors object
MadSensor madSensor(DEVICE_NAME, TOKEN);

long lastPing = 0;
double temperature = 10.0;

void setup()
{
  // Connect to the WiFi network
  madSensor.connectWifi(WIFISSID, WIFIPASS);
}

void loop()
{
  // Send a value every 30 seconds
  if ((millis() - lastPing) > 30000)
   {
    // Add temperature variable to the payload
    madSensor.addNewValue("temperature", temperature);

    // Make the increment
    temperature = temperature + 10;

    // Re initialize the temperature value
    if (temperature > 40)
      temperature = 10;

    // Shows the payload
    Serial.print("JSON Payload: ");
    Serial.println(madSensor.getPOSTrequest());

    // Send the value to the MadSensor server
    Serial.print("Server Reply: ");
    Serial.println(madSensor.sendAll());

    lastPing = millis();
   }
}

  /*
 * Register the Temperature measurements to the Telstra IoT Platform. Based on the reference registration
 * design by Telstra.
 * File:        RegisterTemperatureMeasurementDevice.ino
 * Author:      Telstra
 * Edited by:   Farmdamentals
 * Last Edited: 11/05/2017
 *
 * For more information see https://github.com/telstra/TIC2017
 *
 */
 
#include <connection4g.h>
#include <telstraiot.h>
#include <iotshield.h>
#include <shieldinterface.h>

ShieldInterface shieldif;
IoTShield shield(&shieldif);
Connection4G conn(true, &shieldif);

#define DeviceName "Farmdamental"

const char host[] = "tic2017publicteam14.iot.telstra.com";
const char tenant[] = "tic2017publicteam14";
const char username[] = "device";
const char password[] = "KappaPride17";
const char appkey[] = "";


TelstraIoT iotPlatform(host, tenant, username, password, appkey, &conn, &shield);

void setup() {
  Serial.begin(115200);
  delay(5000);

  Serial.print(F("[START] Starting Registration Script using device name: "));
  Serial.println(DeviceName);

  while (!Serial); // Wait for serial character before starting

  Serial.println(F("[    ] ******* Waiting for shield *********"));
  shield.waitUntilShieldIsReady();
  Serial.println(F("[    ] ******* Shield ready *********"));


  // Check if shield is connected to 4G network
  if (shield.isPDPContextActive()) {
    Serial.println(F("[ OK ] ******* Connected to network *********"));

    // Open secure TCP connection to IoT Platform host
    Serial.println("############################ OPENING TCP CONNECTION #########################");
    conn.openTCP(host, 443);

    // Register device on IoT Platform
    Serial.println("############################ REGISTERING DEVICE #############################");
    char id[8];
    const char* supportedMeasurements[1];
    supportedMeasurements[0] = "TemperatureMeasurement";

    int result = iotPlatform.registerDevice(DeviceName, id, 8, supportedMeasurements, 1);

    if (result < 0) {
      Serial.println(F("[FAIL] Registration error. Please retry."));
      while (true);
    } else {
      Serial.print(F("[    ] Arduino registered with id: "));
      Serial.println(id);
    }

    // Close the TCP connection to the server
    Serial.println("############################ REGISTRATION COMPLETE #########################");
    Serial.println("Please wait while connection closes...");
    conn.closeTCP();
    Serial.println("Done!");
    delay(1000);




  } else {
    Serial.println(F("[FAIL] Shield is not connected to 4G network!"));
    while (true);
    {
      delay(5000);
      Serial.println(F("[FAIL] Shield is not connected to 4G network!"));
    }
  }
}

void loop() {

}




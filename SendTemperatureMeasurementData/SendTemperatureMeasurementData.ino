/*
 * Send the Temperature measurements to the Telstra IoT Platform. Based on the reference example by Freetronics.
 * File:        SendTemperatureMeasurementData.ino
 * Author:      Andy Gelme (@geekscape) & Angus Gratton (angus at freetronics. com)
 * Edited By:   Daewin SV Lingam
 * Last Edited: 08/05/2017
 * License: GPLv3
 *
 * For more information see www.freetronics.com/irtemp
 *
 */

#include <connection4g.h>
#include <telstraiot.h>
#include <iotshield.h>
#include <shieldinterface.h>
#include "IRTemp.h"

ShieldInterface shieldif;
IoTShield shield(&shieldif);
Connection4G conn(true,&shieldif);

TelstraIoT iotPlatform(&conn,&shield);

const char host[] = "tic2017publicteam14.iot.telstra.com";

char id[8];
char tenant[32];
char username[32];
char password[32];

static const byte PIN_DATA    = 2; // Choose any pins you like for these
static const byte PIN_CLOCK   = 3;
static const byte PIN_ACQUIRE = 4;

static const TempUnit SCALE=CELSIUS;  // Options are CELSIUS, FAHRENHEIT

IRTemp irTemp(PIN_ACQUIRE, PIN_CLOCK, PIN_DATA);

void setup(void) {
  Serial.begin(9600);

  Serial.println(F("IRTemp for Telstra IoT Team 14"));
  Serial.println(F("=============================="));

  Serial.println(F("===== Waiting for shield ====="));
  shield.waitUntilShieldIsReady();
  Serial.println(F("======== Shield Ready ========"));

  Serial.println(F("Reading credentials..."));
  shield.readCredentials(id, tenant, username, password);
  Serial.print(F("ID: "));
  Serial.println(F(id));
  Serial.print(F("Tenant: "));
  Serial.println(F(tenant)); 
  Serial.print(F("Username: "));
  Serial.println(F(username));
  Serial.print(F("Password: "));
  Serial.println(F(password));
  
  Serial.println(F("Setting credentials..."));
  iotPlatform.setCredentials(id,tenant,username,password,"");
  
  Serial.println(F("Setting host..."));
  iotPlatform.setHost(host,443);
  
  conn.openTCP(host,443);
}

void loop(void) {
  delay(1000);
  
  Serial.println(F("=============================="));
  Serial.println(F("Measuring the Temperature..."));
  float irTemperature = irTemp.getIRTemperature(SCALE);
  printTemperature("IR", irTemperature);

  // Not utilizing ambient light information
  float ambientTemperature = irTemp.getAmbientTemperature(SCALE);

  Serial.println(F("=============================="));
  Serial.println(F("Sending the measurement to the IoT Platform..."));
  iotPlatform.sendMeasurement("TemperatureMeasurement", "TemperatureMeasurement", "Temperature(C)", irTemperature, "Celcius");
}

void printTemperature(
  char  *type,
  float  temperature) {

  Serial.print(type);
  Serial.print(" temperature: ");

  if (isnan(temperature)) {
    Serial.println("Failed");
  }
  else {
    Serial.print(temperature);
    Serial.println(SCALE == FAHRENHEIT  ?  " F"  :  " C");
  }
}

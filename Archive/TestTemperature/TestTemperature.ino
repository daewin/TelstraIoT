/*
 * Send the Temperature measurements to the Telstra IoT Platform. Based on the reference example by Freetronics.
 * File:        SendTemperatureMeasurementData.ino
 * Author:      Andy Gelme (@geekscape) & Angus Gratton (angus at freetronics. com)
 * Edited By:   The Farmdamentals
 * Last Edited: 11/05/2017
 * License: GPLv3
 *
 * For more information see www.freetronics.com/irtemp
 *
 */

#include <connection4g.h>
#include <telstraiot.h>
#include <iotshield.h>
#include <shieldinterface.h>
#include <IRTemp.h>

const char host[] = "tic2017publicteam14.iot.telstra.com";

char id[8];
char tenant[32];
char username[32];
char password[32];

static const byte PIN_DATA    = 4; // Choose any pins you like for these
static const byte PIN_CLOCK   = 5;
static const byte PIN_ACQUIRE = 6;

static const TempUnit SCALE=CELSIUS;  // Options are CELSIUS, FAHRENHEIT

const int buttonPin = 7;


  int count = 0;
  bool sheepone = true;

IRTemp irTemp(PIN_ACQUIRE, PIN_CLOCK, PIN_DATA);
ShieldInterface shieldif;
IoTShield shield(&shieldif);
Connection4G conn(true,&shieldif);
TelstraIoT iotPlatform(&conn,&shield);

void setup(void) {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  delay(500);

  Serial.println(F("IRTemp for Telstra IoT Team 14"));
  Serial.println(F("=============================="));
  
  if(!shield.isShieldReady())
  {
    Serial.println(F("===== Waiting for shield ====="));
    shield.waitUntilShieldIsReady();
  } else {
    Serial.println(F("======== Shield Ready ========"));
  }
  //while(Serial.available()==0); // Wait for serial character before starting

  Serial.println(F("Reading credentials..."));
  shield.readCredentials(id, tenant, username, password);

  // Print debug info
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

  if(count == 10){
    sheepone = !sheepone;
    count = 0;
  }

  Serial.println(F("=============================="));
  Serial.println(F("Measuring the Temperature..."));
  float irTemperature = irTemp.getIRTemperature(SCALE);
  printTemperature("IR", irTemperature);


  Serial.println(F("=============================="));
  Serial.println(F("Sending the measurement to the IoT Platform..."));

    
  if(sheepone){
    // ID set to Sheep One (14738)
    char newId[8] = "14738";
    changePlatformCredentials(newId);
    iotPlatform.sendMeasurement("TemperatureMeasurement", "TemperatureMeasurement", "Temperature(C)", irTemperature, "Celcius");
    resetPlatformCredentials();
     
  } else if (!sheepone){
    // ID set to Sheep Two (14739)
    char newId[8] = "14739";
    changePlatformCredentials(newId);
    iotPlatform.sendMeasurement("TemperatureMeasurement", "TemperatureMeasurement", "Temperature(C)", irTemperature, "Celcius");
    resetPlatformCredentials();
    
  } else {
    // Error!
    Serial.println(F("Error!"));
  }

  count++;
  delay(200);
}

void printTemperature(char *type, float temperature){

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

/*
 * Change the Platform's ID to the parameter ID supplied
 */
void changePlatformCredentials(char *newId){
  
  // Replace the current id with the parameter ID
  iotPlatform.setCredentials(newId,tenant,username,password,"");
 
}

/*
 * Resets the Platform's ID back to the original ID
 */
void resetPlatformCredentials(){
  
  // Gets the credentials from the Shield
  shield.readCredentials(id, tenant, username, password);

  // Replace the current id with the original ID
  iotPlatform.setCredentials(id,tenant,username,password,"");
}


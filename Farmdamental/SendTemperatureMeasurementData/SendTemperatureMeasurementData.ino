/*
   Send the Temperature measurements to the Telstra IoT Platform. Based on the reference example by Freetronics and Telstra.
   File:        SendTemperatureMeasurementData.ino
   Edited By:   The Farmdamentals
   Last Edited: 12/05/2017
   License: GPLv3

   For more information see www.freetronics.com/irtemp

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
const int motionPin = 7;
const int readerPin = 9;
const int heckpin = 10;
const int kpin = 8;

static const TempUnit SCALE = CELSIUS; // Options are CELSIUS, FAHRENHEIT

IRTemp irTemp(PIN_ACQUIRE, PIN_CLOCK, PIN_DATA);
ShieldInterface shieldif;
IoTShield shield(&shieldif);
Connection4G conn(true, &shieldif);
TelstraIoT iotPlatform(&conn, &shield);

void setup(void) {
  Serial.begin(115200);
  pinMode(motionPin, INPUT);
  pinMode(readerPin, INPUT);
  pinMode(heckpin, INPUT);
  pinMode(kpin, INPUT);
  delay(500);

  Serial.println(F("IRTemp for Telstra IoT Team 14"));
  Serial.println(F("=============================="));

  if (!shield.isShieldReady())
  {
    Serial.println(F("===== Waiting for shield ====="));
    shield.waitUntilShieldIsReady();
  } else {
    Serial.println(F("======== Shield Ready ========"));
  }
  //while(Serial.available()==0); // Wait for serial character before starting

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
  iotPlatform.setCredentials(id, tenant, username, password, "");

  Serial.println(F("Setting host..."));
  iotPlatform.setHost(host, 443);

  conn.openTCP(host, 443);
}

/*
   If there is an RFID tag being scanned while there is movement, we send off measurements
   based on that RFIDs ID.
*/
void loop(void) {

  int currentRFID = getRFID();

  if (currentRFID == 1) {
    // ID set to Sheep One (14738)
    char newId[8] = "14738";

    changePlatformCredentials(newId);
    sendMeasurementWhenMovementDetected(newId);
    resetPlatformCredentials();

  } else if (currentRFID == 2) {
    // ID set to Sheep Two (14739)
    char newId[8] = "14739";

    changePlatformCredentials(newId);
    sendMeasurementWhenMovementDetected(newId);
    resetPlatformCredentials();

  } else {
    // Do Nothing
    Serial.println(F("No RFID Read"));
  }

  delay(200);
}

void sendMeasurementWhenMovementDetected(char* newId) {

  if (digitalRead(motionPin) == HIGH) {

    Serial.println(F("Measuring the Temperature..."));
    float irTemperature = irTemp.getIRTemperature(SCALE);
    printTemperature("IR", irTemperature);

    Serial.println(F(" "));
    Serial.print(F("Sending the measurement to the IoT Platform for Sheep with ID: "));
    Serial.println(F(newId));

    iotPlatform.sendMeasurement("TemperatureMeasurement", "TemperatureMeasurement", "Temperature(C)", irTemperature, "Celcius");
  } else {
    // Send nothing
    Serial.println(F("No Movement!"));
  }
}


/*
   Get the RFID tag being scanned, if available. Else, return -1.
*/
int getRFID() {
  if ((digitalRead(readerPin) == LOW) && (digitalRead(heckpin) == HIGH) && (digitalRead(kpin) == LOW)) {
    // Sheep One (14738)
    return 1;
    
  } else if ((digitalRead(readerPin) == HIGH) && (digitalRead(heckpin) == LOW) && (digitalRead(kpin) == LOW)) {
    // Sheep Two (14739)
    return -1;
    
  } else if ((digitalRead(readerPin) == HIGH) && (digitalRead(heckpin) == LOW) && (digitalRead(kpin) == HIGH)) {
    // Sheep Three (15925)
    return -1;
    
  } else if ((digitalRead(readerPin) == LOW) && (digitalRead(heckpin) == HIGH) && (digitalRead(kpin) == HIGH)) {
    // Sheep Four (15764)
    return 2;
    
  } else {
    return -1;
  }
}



/*
   Change the Platform's ID to the parameter ID supplied
*/
void changePlatformCredentials(char *newId) {

  // Replace the current id with the parameter ID
  iotPlatform.setCredentials(newId, tenant, username, password, "");

}

/*
   Resets the Platform's ID back to the original ID
*/
void resetPlatformCredentials() {

  // Gets the credentials from the Shield
  shield.readCredentials(id, tenant, username, password);

  // Replace the current id with the original ID
  iotPlatform.setCredentials(id, tenant, username, password, "");
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





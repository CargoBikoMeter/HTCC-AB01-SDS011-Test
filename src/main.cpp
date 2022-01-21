// testprogram to check basic functions to get data from SDS011 pm sensor
// to work with Heltec softSerial function SDS011 source must be modified
// default SoftwareSerial must be replaced with softSerial

// Heltec Cubecell HTCC AB01: https://heltec.org/project/htcc-ab01/
#include "SdsDustSensor.h" // https://github.com/lewapek/sds-dust-sensors-arduino-library
#define __DEBUG_SDS_DUST_SENSOR__

// pin for StepUp module
// VStepUpPin = GPIO5; // AB01
#define _VStepUpPin GPIO5

// GPIO3: Cubecell AB01 RX - connect with SDS011 TXD
// GPIO2: Cubecell AB01 TX - connect with SDS011 RXD
#define _BoardRxPin GPIO3
#define _BoardTxPin GPIO2

// SDS011 with softSerial based communication for Heltec cubecell boards: 
softSerial softwareSerial(_BoardTxPin,_BoardRxPin); // sds(-> SDS011-RXD, -> SDS011-TXD)
SdsDustSensor  sds(softwareSerial);  

// Pin for switching Step-Up module HW-668 On and Off
// needs 140 mA to drive SDS011 with 5V and 73 mA
// VStepUpPin drives a P-MOSFET which switches the StepUpModul On/Off
int VStepUpPin = _VStepUpPin;


// switch external power ON
void VextON()
{
  Serial.println("VextON");
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, LOW);
  delay(500);
}

// switch external power OFF
void VextOFF() //Vext default OFF
{
  Serial.println("VextOFF");
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, HIGH);
  delay(500);
}

// **********   MAIN PROGRAM   ***********

void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.println("Setup");
  
}

void loop() {
  Serial.println("loop begin");
  VextON();

  Serial.println("initialize sensor");
  sds.begin(); // initalize SDS011 sensor

  Serial.println("wakeup sensor");
  sds.wakeup();
  Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
  Serial.println(sds.setQueryReportingMode().toString()); // ensures sensor is in 'query' reporting mode
  
  Serial.println("clean sensor");
  delay(5000); // working some seconds to clean the internal sensor (recommended 30 seconds= 30000)
  PmResult pm = sds.queryPm();
  if (pm.isOk()) {
    Serial.print("PM2.5 = ");
    Serial.print(pm.pm25);
    Serial.print(", PM10 = ");
    Serial.println(pm.pm10);
    Serial.println(pm.toString());
  } else {
    Serial.print("Could not read values from sensor, reason: ");
    Serial.println(pm.statusToString());
  }

  WorkingStateResult state = sds.sleep();
  if (state.isWorking()) {
    Serial.println("Problem with sleeping the sensor.");
  } else {
    Serial.println("Sensor is sleeping");
    
  }

  VextOFF(); // works only with a backup capacitor after StepUpConverter
  // don't insert a delay() command here - system will hang
}
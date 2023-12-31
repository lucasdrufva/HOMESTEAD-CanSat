#include <Arduino.h>
#include <Wire.h>

#include "pins.h"
#include "radio.h"
#include "flash.h"
#include "time_sensor.h"
#include "battery_sensor.h"
#include "bme_sensor.h"
#include "uv_sensor.h"
#include "bno_sensor.h"
#include "gps_sensor.h"
#include "status_data.h"
#include "camera.h"
#include "buzzer.h"
#include "unit_controller.h"

#define DEBUG 1


Mode deviceMode = MODE_PAD;

Radio radio;
Flash flash;
TimeSensor timeSensor;
BatterySensor batterySensor;
BmeSensor bmeSensor;
UvSensor uvSensor;
BnoSensor bnoSensor;
GpsSensor gpsSensor;
Camera camera;
Buzzer buzzer;

UnitController unitController;

const int sensorCount = 1;
Sensor sensors[sensorCount];

void runModePad()
{
  Serial.println("Run pad");
  digitalWrite(PIN_LED, HIGH);
  //Serial.println("Battery");
  //batterySensor.run();
  Serial.println("bme");
  bmeSensor.run();
  Serial.println("uv");
  uvSensor.run();
  Serial.println("bno");
  bnoSensor.run();
  Serial.println("GPS");
  gpsSensor.run();
  Serial.println("Camera");
  camera.run();
  buzzer.run();
  radio.run();
  delay(200);
  digitalWrite(PIN_LED, LOW);
  delay(100);
}

void runModeFlight(){
  
}

void runModeRetrival(){
  
}

void runModeExportData()
{
  Serial.println("Run export data");
  while(!Serial){delay(10);}
  flash.exportData();
  while(true)
  {
    digitalWrite(PIN_LED, HIGH);
    delay(1000);
    digitalWrite(PIN_LED, LOW);
    delay(1000); 
  }

}

void setDeviceModeFromStatus(StatusType newMode)
{
  Serial.println("Changing device mode");
  switch (newMode)
  {
  case STATUS_TYPE_MODE_PAD:
    deviceMode = MODE_PAD;
    uvSensor.setPeriod(5000);
    bmeSensor.setPeriod(5000);
    bnoSensor.setPeriod(5000);
    gpsSensor.setPeriod(5000);
    break;
  case STATUS_TYPE_MODE_FLIGHT:
    deviceMode = MODE_FLIGHT;
    uvSensor.setPeriod(500);
    bmeSensor.setPeriod(1000);
    bnoSensor.setPeriod(0);
    gpsSensor.setPeriod(1000);
    break;
  case STATUS_TYPE_MODE_DROP:
    deviceMode = MODE_FLIGHT;
    uvSensor.setPeriod(500);
    bmeSensor.setPeriod(1000);
    bnoSensor.setPeriod(0);
    gpsSensor.setPeriod(1000);
    break;
  case STATUS_TYPE_MODE_RETRIEVAL:
    deviceMode = MODE_RETRIEVAL;
    uvSensor.setPeriod(10000);
    bmeSensor.setPeriod(10000);
    bnoSensor.setPeriod(10000);
    gpsSensor.setPeriod(5000);
    break;
  
  default:
    break;
  }
  StatusData *data = new StatusData(newMode);
  flash.logData(data);
  radio.logData(data);
}

void logDataCallback(Data *data)
{
  Serial.println("Got data");
  radio.logData(data);
  flash.logData(data);
  delete data;
}

void radioCallback(Data *data)
{
  Serial.println("radio cb");
  if(data->getType() == DATA_TYPE_STATUS)
  {
    Serial.println("radio cb: status");
    StatusType newMode;
    data->writeData((uint8_t*) &newMode);
    setDeviceModeFromStatus(newMode);
  }
  delete data;
}

void setup() {
  Serial.begin(9600);
  //while(!Serial){delay(10);}
  pinMode(PIN_USER_SWITCH, INPUT);
  pinMode(PIN_LED, OUTPUT);

  Serial.println("Adding units");

  unitController.addUnit(&radio);
  unitController.addUnit(&flash);
  unitController.addUnit(&bmeSensor);
  unitController.addUnit(&uvSensor);
  unitController.addUnit(&bnoSensor);
  unitController.addUnit(&gpsSensor);
  unitController.addUnit(&buzzer);
  unitController.addUnit(&camera);

  radio.setDataCallback(*radioCallback);
  
  bmeSensor.setDataCallback(*logDataCallback);
  uvSensor.setDataCallback(*logDataCallback);
  bnoSensor.setDataCallback(*logDataCallback);
  gpsSensor.setDataCallback(*logDataCallback);

  Serial.println("Starting everything");

  if(unitController.begin())
  {
    buzzer.indicateStatus(UNIT_STATUS_OK);
  }
  else
  {
    // Something is very wrong!
    Serial.println("UNIT STATUS ERROR");
    buzzer.indicateStatus(UNIT_STATUS_ERROR);
    buzzer.run();
  }

  UnitStatus status = unitController.test();
  buzzer.indicateStatus(status); 
  buzzer.run();

  if (digitalRead(PIN_USER_SWITCH) == HIGH){
    deviceMode = MODE_EXPORT;
  }
  else
  {
    StatusData *data = new StatusData(STATUS_TYPE_START);
    flash.logData(data);
    radio.logData(data);
    setDeviceModeFromStatus(STATUS_TYPE_MODE_PAD);
  }
  
}

void loop() {
  switch (deviceMode)
  {
    case MODE_EXPORT:
      runModeExportData();
      break;
    default:
      gpsSensor.run();
      // radio.run();
      // delay(1);
      unitController.run();
      // radio.run();
      // delay(1);
      gpsSensor.run();
      break;
  }
}
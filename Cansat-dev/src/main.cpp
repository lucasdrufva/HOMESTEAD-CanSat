#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#include <RH_RF95.h>
#include <RHSPIDriver.h>

// #include <RHSoftwareSPI.h>
// RHSoftwareSPI spi;

#include <SPIFlash.h>

#include <Adafruit_BNO08x.h>

// #include <SoftwareSerial.h>

#include "wiring_private.h"

#include <xSL01.h>

#include <TinyGPSPlus.h>

#define RFM95_CS 21
// #define RFM95_CS 4
#define RFM95_RST 30
// #define RFM95_RST 2
#define RFM95_INT 20

#define RF95_FREQ 434.0

#define SEALEVELPRESSURE_HPA (1013.25)

RH_RF95 rf95(RFM95_CS, RFM95_INT);

SPIFlash flash(28, 0xEF40);

Adafruit_BME680 bme; // I2C

#define BNO08X_RESET -1

Adafruit_BNO08x bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

xSL01 SL01;

TinyGPSPlus gps;

void spi_radio_mode()
{
  SPI._uc_pinMosi = 10;
  SPI._uc_pinMiso = 8;

  SPI._padTx = SPI_PAD_3_SCK_1;
  SPI._padRx = SERCOM_RX_PAD_0;
  SPI.begin();
}

void spi_flash_mode()
{
  SPI._uc_pinMosi = 8;
  SPI._uc_pinMiso = 10;

  SPI._padTx = SPI_PAD_0_SCK_1;
  SPI._padRx = SERCOM_RX_PAD_3;
  SPI.begin();
}

// Here is where you define the sensor outputs you want to receive
void setReports(void)
{
  Serial.println("Setting desired reports");
  if (!bno08x.enableReport(SH2_ACCELEROMETER))
  {
    Serial.println("Could not enable accelerometer");
  }
  if (!bno08x.enableReport(SH2_GYROSCOPE_CALIBRATED))
  {
    Serial.println("Could not enable gyroscope");
  }
  if (!bno08x.enableReport(SH2_MAGNETIC_FIELD_CALIBRATED))
  {
    Serial.println("Could not enable magnetic field calibrated");
  }
  if (!bno08x.enableReport(SH2_LINEAR_ACCELERATION))
  {
    Serial.println("Could not enable linear acceleration");
  }
  if (!bno08x.enableReport(SH2_GRAVITY))
  {
    Serial.println("Could not enable gravity vector");
  }
  if (!bno08x.enableReport(SH2_ROTATION_VECTOR))
  {
    Serial.println("Could not enable rotation vector");
  }
  if (!bno08x.enableReport(SH2_GEOMAGNETIC_ROTATION_VECTOR))
  {
    Serial.println("Could not enable geomagnetic rotation vector");
  }
  if (!bno08x.enableReport(SH2_GAME_ROTATION_VECTOR))
  {
    Serial.println("Could not enable game rotation vector");
  }
  if (!bno08x.enableReport(SH2_RAW_ACCELEROMETER))
  {
    Serial.println("Could not enable raw accelerometer");
  }
  if (!bno08x.enableReport(SH2_RAW_GYROSCOPE))
  {
    Serial.println("Could not enable raw gyroscope");
  }
  if (!bno08x.enableReport(SH2_RAW_MAGNETOMETER))
  {
    Serial.println("Could not enable raw magnetometer");
  }
  if (!bno08x.enableReport(SH2_STABILITY_CLASSIFIER))
  {
    Serial.println("Could not enable stability classifier");
  }
}

void setup()
{
  GpsSerial.begin(9600);

  // spi.setPins(8, 10, 9);
  //  put your setup code here, to run once:

  pinMode(RFM95_RST, OUTPUT);
  // pinMode(RFM95_CS, OUTPUT);
  pinMode(31, OUTPUT);

  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  while (!Serial)
    ;
  Serial.begin(9600);
  delay(100);

  Wire.begin();

  // SL01.begin();

  // FLASH

  // spi_flash_mode();

  // if (flash.initialize())
  // {
  //   Serial.println("Flash Init OK!");
  // }
  // Serial.println(flash.readDeviceId(), HEX);

  // char text[40] = "Hello dear world\0";

  // flash.writeBytes(1000, text, 40);

  // char buf[40];

  // flash.readBytes(1000, buf, 40);
  // Serial.println(buf);

  spi_radio_mode();

  Serial.println("Lora send test");

  // Manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(100);

  // pinMode(RFM95_CS, OUTPUT);
  // digitalWrite(RFM95_CS, HIGH);

  // SPI.begin();

  // digitalWrite(RFM95_CS, LOW);
  // delay(100);

  // SPI.transfer(0x01 & RH_SPI_WRITE_MASK);
  // SPI.transfer(8);

  // delay(100);
  // digitalWrite(RFM95_CS, HIGH);

  // delay(1000);

  // digitalWrite(RFM95_CS, LOW);
  // delay(100);

  // uint8_t val = 0;
  // SPI.transfer(0x01 & ~RH_SPI_WRITE_MASK);
  // val = SPI.transfer(0);

  // delay(100);
  // digitalWrite(RFM95_CS, HIGH);

  // Serial.println(val);

  // Set sleep mode, so we can also set LORA mode:
  // rf95.spiWrite(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_SLEEP | RH_RF95_LONG_RANGE_MODE);
  // delay(10); // Wait for sleep mode to take over from say, CAD
  // // Check we are in sleep mode, with LORA set
  // if (rf95.spiRead(RH_RF95_REG_01_OP_MODE) != (RH_RF95_MODE_SLEEP | RH_RF95_LONG_RANGE_MODE))
  // {
  //   Serial.println("failish");
  // }

  // Serial.print("val:");
  // Serial.println(rf95.spiRead(RH_RF95_REG_01_OP_MODE), HEX);

  if (!rf95.init())
  {
    Serial.println("Failed init");
    while (1)
      ;
  }

  rf95.setFrequency(RF95_FREQ);

  Serial.println("Suceess");

  // if (!bme.begin(0x76)) {
  //   Serial.println("Could not find a valid BME680 sensor, check wiring!");
  //   while (1);
  // }

  // // Set up oversampling and filter initialization
  // bme.setTemperatureOversampling(BME680_OS_8X);
  // bme.setHumidityOversampling(BME680_OS_2X);
  // bme.setPressureOversampling(BME680_OS_4X);
  // bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  // bme.setGasHeater(320, 150); // 320*C for 150 ms

  // if (!bno08x.begin_I2C(0x4B)) {
  //   // if (!bno08x.begin_UART(&gps)) {  // Requires a device with > 300 byte
  //   // UART buffer! if (!bno08x.begin_SPI(BNO08X_CS, BNO08X_INT)) {
  //   Serial.println("Failed to find BNO08x chip");
  //   while (1) {
  //     delay(10);
  //   }
  // }
  // Serial.println("BNO08x Found!");

  // for (int n = 0; n < bno08x.prodIds.numEntries; n++) {
  //   Serial.print("Part ");
  //   Serial.print(bno08x.prodIds.entry[n].swPartNumber);
  //   Serial.print(": Version :");
  //   Serial.print(bno08x.prodIds.entry[n].swVersionMajor);
  //   Serial.print(".");
  //   Serial.print(bno08x.prodIds.entry[n].swVersionMinor);
  //   Serial.print(".");
  //   Serial.print(bno08x.prodIds.entry[n].swVersionPatch);
  //   Serial.print(" Build ");
  //   Serial.println(bno08x.prodIds.entry[n].swBuildNumber);
  // }

  // setReports();

  // gps
  // gpsSerial.begin(9600);
  // gps.begin(9600);
}

int16_t packetnum = 0;

void loop()
{
  spi_radio_mode();

  if (rf95.available())
  {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {
      //      RH_RF95::printBuffer("request: ", buf, len);
      Serial.print("got request: ");
      Serial.println((char *)buf);
      //      Serial.print("RSSI: ");
      //      Serial.println(rf95.lastRssi(), DEC);

      // Send a reply
      uint8_t data[] = "And hello back to you";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Sent a reply");
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  

  // put your main code here, to run repeatedly:

  // digitalWrite(31, HIGH);

  // spi_radio_mode();

  // char radiopacket[20] = "Hello World #      ";
  // itoa(packetnum++, radiopacket+13, 10);
  // Serial.print("Sending "); Serial.println(radiopacket);
  // radiopacket[19] = 0;

  // Serial.println("Sending..."); delay(10);
  // rf95.send((uint8_t *)radiopacket, 20);

  // Serial.println("Waiting for packet to complete..."); delay(10);
  // rf95.waitPacketSent(100);
  // delay(100);
  // rf95.setModeIdle();

  // digitalWrite(31, LOW);

  // spi_flash_mode();

  // Serial.print("Flash id:");
  // Serial.println(flash.readDeviceId(), HEX);

  // if (! bme.performReading()) {
  //   Serial.println("Failed to perform reading :(");
  //   return;
  // }
  // Serial.print("Temperature = ");
  // Serial.print(bme.temperature);
  // Serial.println(" *C");

  // Serial.print("Pressure = ");
  // Serial.print(bme.pressure / 100.0);
  // Serial.println(" hPa");

  // Serial.print("Humidity = ");
  // Serial.print(bme.humidity);
  // Serial.println(" %");

  // Serial.print("Gas = ");
  // Serial.print(bme.gas_resistance / 1000.0);
  // Serial.println(" KOhms");

  // Serial.print("Approx. Altitude = ");
  // Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  // Serial.println(" m");

  // Serial.println();

  // if (bno08x.wasReset()) {
  //   Serial.print("sensor was reset ");
  //   setReports();
  // }

  // if (!bno08x.getSensorEvent(&sensorValue)) {
  //   return;
  // }

  // switch (sensorValue.sensorId) {

  // case SH2_ACCELEROMETER:
  //   Serial.print("Accelerometer - x: ");
  //   Serial.print(sensorValue.un.accelerometer.x);
  //   Serial.print(" y: ");
  //   Serial.print(sensorValue.un.accelerometer.y);
  //   Serial.print(" z: ");
  //   Serial.println(sensorValue.un.accelerometer.z);
  //   break;
  // case SH2_GYROSCOPE_CALIBRATED:
  //   Serial.print("Gyro - x: ");
  //   Serial.print(sensorValue.un.gyroscope.x);
  //   Serial.print(" y: ");
  //   Serial.print(sensorValue.un.gyroscope.y);
  //   Serial.print(" z: ");
  //   Serial.println(sensorValue.un.gyroscope.z);
  //   break;
  // case SH2_MAGNETIC_FIELD_CALIBRATED:
  //   Serial.print("Magnetic Field - x: ");
  //   Serial.print(sensorValue.un.magneticField.x);
  //   Serial.print(" y: ");
  //   Serial.print(sensorValue.un.magneticField.y);
  //   Serial.print(" z: ");
  //   Serial.println(sensorValue.un.magneticField.z);
  //   break;
  // case SH2_LINEAR_ACCELERATION:
  //   Serial.print("Linear Acceration - x: ");
  //   Serial.print(sensorValue.un.linearAcceleration.x);
  //   Serial.print(" y: ");
  //   Serial.print(sensorValue.un.linearAcceleration.y);
  //   Serial.print(" z: ");
  //   Serial.println(sensorValue.un.linearAcceleration.z);
  //   break;
  // case SH2_GRAVITY:
  //   Serial.print("Gravity - x: ");
  //   Serial.print(sensorValue.un.gravity.x);
  //   Serial.print(" y: ");
  //   Serial.print(sensorValue.un.gravity.y);
  //   Serial.print(" z: ");
  //   Serial.println(sensorValue.un.gravity.z);
  //   break;
  // case SH2_ROTATION_VECTOR:
  //   Serial.print("Rotation Vector - r: ");
  //   Serial.print(sensorValue.un.rotationVector.real);
  //   Serial.print(" i: ");
  //   Serial.print(sensorValue.un.rotationVector.i);
  //   Serial.print(" j: ");
  //   Serial.print(sensorValue.un.rotationVector.j);
  //   Serial.print(" k: ");
  //   Serial.println(sensorValue.un.rotationVector.k);
  //   break;
  // case SH2_GEOMAGNETIC_ROTATION_VECTOR:
  //   Serial.print("Geo-Magnetic Rotation Vector - r: ");
  //   Serial.print(sensorValue.un.geoMagRotationVector.real);
  //   Serial.print(" i: ");
  //   Serial.print(sensorValue.un.geoMagRotationVector.i);
  //   Serial.print(" j: ");
  //   Serial.print(sensorValue.un.geoMagRotationVector.j);
  //   Serial.print(" k: ");
  //   Serial.println(sensorValue.un.geoMagRotationVector.k);
  //   break;

  // case SH2_GAME_ROTATION_VECTOR:
  //   Serial.print("Game Rotation Vector - r: ");
  //   Serial.print(sensorValue.un.gameRotationVector.real);
  //   Serial.print(" i: ");
  //   Serial.print(sensorValue.un.gameRotationVector.i);
  //   Serial.print(" j: ");
  //   Serial.print(sensorValue.un.gameRotationVector.j);
  //   Serial.print(" k: ");
  //   Serial.println(sensorValue.un.gameRotationVector.k);
  //   break;

  // case SH2_STABILITY_CLASSIFIER: {
  //   Serial.print("Stability Classification: ");
  //   sh2_StabilityClassifier_t stability = sensorValue.un.stabilityClassifier;
  //   switch (stability.classification) {
  //   case STABILITY_CLASSIFIER_UNKNOWN:
  //     Serial.println("Unknown");
  //     break;
  //   case STABILITY_CLASSIFIER_ON_TABLE:
  //     Serial.println("On Table");
  //     break;
  //   case STABILITY_CLASSIFIER_STATIONARY:
  //     Serial.println("Stationary");
  //     break;
  //   case STABILITY_CLASSIFIER_STABLE:
  //     Serial.println("Stable");
  //     break;
  //   case STABILITY_CLASSIFIER_MOTION:
  //     Serial.println("In Motion");
  //     break;
  //   }
  //   break;
  // }

  // case SH2_RAW_ACCELEROMETER:
  //   Serial.print("Raw Accelerometer - x: ");
  //   Serial.print(sensorValue.un.rawAccelerometer.x);
  //   Serial.print(" y: ");
  //   Serial.print(sensorValue.un.rawAccelerometer.y);
  //   Serial.print(" z: ");
  //   Serial.println(sensorValue.un.rawAccelerometer.z);
  //   break;
  // case SH2_RAW_GYROSCOPE:
  //   Serial.print("Raw Gyro - x: ");
  //   Serial.print(sensorValue.un.rawGyroscope.x);
  //   Serial.print(" y: ");
  //   Serial.print(sensorValue.un.rawGyroscope.y);
  //   Serial.print(" z: ");
  //   Serial.println(sensorValue.un.rawGyroscope.z);
  //   break;
  // case SH2_RAW_MAGNETOMETER:
  //   Serial.print("Raw Magnetic Field - x: ");
  //   Serial.print(sensorValue.un.rawMagnetometer.x);
  //   Serial.print(" y: ");
  //   Serial.print(sensorValue.un.rawMagnetometer.y);
  //   Serial.print(" z: ");
  //   Serial.println(sensorValue.un.rawMagnetometer.z);
  //   break;

  // }

  // float uv;
  // float lux;
  // uv = 0;

  // // Request SL01 to return calculated UVB intensity
  // lux = SL01.getLUX();
  // uv = SL01.getUVIndex();
  // // Display Data on the Serial monitor
  // Serial.print("UVB Index: ");
  // Serial.println(uv);

  // Serial.println();

  // gps

  // while (GpsSerial.available())
  // {
  //   if (gps.encode(GpsSerial.read()))
  //   {
  //     Serial.print(F("Location: "));
  //     if (gps.location.isValid())
  //     {
  //       Serial.print(gps.location.lat(), 6);
  //       Serial.print(F(","));
  //       Serial.println(gps.location.lng(), 6);
  //     }

  //     if (gps.time.isValid())
  //     {
  //       if (gps.time.hour() < 10)
  //         Serial.print(F("0"));
  //       Serial.print(gps.time.hour());
  //       Serial.print(F(":"));
  //       if (gps.time.minute() < 10)
  //         Serial.print(F("0"));
  //       Serial.print(gps.time.minute());
  //       Serial.print(F(":"));
  //       if (gps.time.second() < 10)
  //         Serial.print(F("0"));
  //       Serial.print(gps.time.second());
  //       Serial.print(F("."));
  //       if (gps.time.centisecond() < 10)
  //         Serial.print(F("0"));
  //       Serial.print(gps.time.centisecond());
  //     }
  //     else
  //     {
  //       Serial.print(F("INVALID"));
  //     }
  //   }
  // }

  delay(100);
}
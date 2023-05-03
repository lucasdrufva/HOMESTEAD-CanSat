#include "bno_sensor.h"

#include "bno_data.h"

bool BnoSensor::begin()
{
    if (!bno08x.begin_I2C(0x4B)) {
        // if (!bno08x.begin_UART(&Serial1)) {  // Requires a device with > 300 byte
        // UART buffer! if (!bno08x.begin_SPI(BNO08X_CS, BNO08X_INT)) {
        Serial.println("Failed to find BNO08x chip");
        return false;
    }
    Serial.println("BNO08x Found!");

    for (int n = 0; n < bno08x.prodIds.numEntries; n++) {
        Serial.print("Part ");
        Serial.print(bno08x.prodIds.entry[n].swPartNumber);
        Serial.print(": Version :");
        Serial.print(bno08x.prodIds.entry[n].swVersionMajor);
        Serial.print(".");
        Serial.print(bno08x.prodIds.entry[n].swVersionMinor);
        Serial.print(".");
        Serial.print(bno08x.prodIds.entry[n].swVersionPatch);
        Serial.print(" Build ");
        Serial.println(bno08x.prodIds.entry[n].swBuildNumber);
    }

    setReports();

    return true;
}

void BnoSensor::run()
{
    if(!(millis()-lastUpdate > period))
        return;

    lastUpdate = millis();

    if (bno08x.wasReset()) {
        Serial.print("sensor was reset ");
        setReports();
    }

    if (!bno08x.getSensorEvent(&sensorValue)) {
        return;
    }

    switch (sensorValue.sensorId) {
        case SH2_GYROSCOPE_CALIBRATED:
            Serial.print("Gyro - x: ");
            Serial.print(sensorValue.un.gyroscope.x);
            Serial.print(" y: ");
            Serial.print(sensorValue.un.gyroscope.y);
            Serial.print(" z: ");
            Serial.println(sensorValue.un.gyroscope.z);
            break;
        case SH2_MAGNETIC_FIELD_CALIBRATED:
            Serial.print("Magnetic Field - x: ");
            Serial.print(sensorValue.un.magneticField.x);
            Serial.print(" y: ");
            Serial.print(sensorValue.un.magneticField.y);
            Serial.print(" z: ");
            Serial.println(sensorValue.un.magneticField.z);
            break;
        case SH2_LINEAR_ACCELERATION:
            Serial.print("Linear Acceration - x: ");
            Serial.print(sensorValue.un.linearAcceleration.x);
            Serial.print(" y: ");
            Serial.print(sensorValue.un.linearAcceleration.y);
            Serial.print(" z: ");
            Serial.println(sensorValue.un.linearAcceleration.z);
            break;
        case SH2_ROTATION_VECTOR:
            Serial.print("Rotation Vector - r: ");
            Serial.print(sensorValue.un.rotationVector.real);
            Serial.print(" i: ");
            Serial.print(sensorValue.un.rotationVector.i);
            Serial.print(" j: ");
            Serial.print(sensorValue.un.rotationVector.j);
            Serial.print(" k: ");
            Serial.println(sensorValue.un.rotationVector.k);
            break;

        case SH2_STABILITY_CLASSIFIER: {
            Serial.print("Stability Classification: ");
            sh2_StabilityClassifier_t stability = sensorValue.un.stabilityClassifier;
            switch (stability.classification) {
            case STABILITY_CLASSIFIER_UNKNOWN:
            Serial.println("Unknown");
            break;
            case STABILITY_CLASSIFIER_ON_TABLE:
            Serial.println("On Table");
            break;
            case STABILITY_CLASSIFIER_STATIONARY:
            Serial.println("Stationary");
            break;
            case STABILITY_CLASSIFIER_STABLE:
            Serial.println("Stable");
            break;
            case STABILITY_CLASSIFIER_MOTION:
            Serial.println("In Motion");
            break;
            }
            break;
        }

        }

    BnoData *data = new BnoData(sensorValue);
    dataCallback(data);
}

void BnoSensor::setReports(void) {
  Serial.println("Setting desired reports");
  // if (!bno08x.enableReport(SH2_GYROSCOPE_CALIBRATED)) {
  //   Serial.println("Could not enable gyroscope");
  // }
  if (!bno08x.enableReport(SH2_MAGNETIC_FIELD_CALIBRATED, 1000000)) {
    Serial.println("Could not enable magnetic field calibrated");
  }
  if (!bno08x.enableReport(SH2_LINEAR_ACCELERATION, 100)) {
    Serial.println("Could not enable linear acceleration");
  }
  if (!bno08x.enableReport(SH2_ROTATION_VECTOR, 100)) {
    Serial.println("Could not enable rotation vector");
  }
  // if (!bno08x.enableReport(SH2_GEOMAGNETIC_ROTATION_VECTOR)) {
  //   Serial.println("Could not enable geomagnetic rotation vector");
  // }
//   if (!bno08x.enableReport(SH2_GAME_ROTATION_VECTOR, 100)) {
//     Serial.println("Could not enable game rotation vector");
//   }
  // if (!bno08x.enableReport(SH2_STABILITY_CLASSIFIER)) {
  //   Serial.println("Could not enable stability classifier");
  // }

}
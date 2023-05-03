#pragma once

#include "data.h"

#include <Adafruit_BNO08x.h>

class BnoData : public Data
{
public:
    BnoData(sh2_SensorValue_t data)
    {

        storedData = data;
    };
    size_t getSize()
    {
        return dataLength[getType()];
    }
    DataType getType()
    {
        switch (storedData.sensorId) {
        case SH2_GYROSCOPE_CALIBRATED:
            return DATA_TYPE_GYRO;
        case SH2_MAGNETIC_FIELD_CALIBRATED:
            return DATA_TYPE_MAGNET;
        case SH2_LINEAR_ACCELERATION:
            return DATA_TYPE_ACCELERATION;
        case SH2_ROTATION_VECTOR:
            return DATA_TYPE_ROTATION; 
        case SH2_STABILITY_CLASSIFIER: {
            return DATA_TYPE_ACCELERATION;
        }

        }
    }

    void writeData(uint8_t *destination)
    {
        switch (storedData.sensorId) {
        case SH2_GYROSCOPE_CALIBRATED:
            Serial.print("Gyro - x: ");
            Serial.print(storedData.un.gyroscope.x);
            Serial.print(" y: ");
            Serial.print(storedData.un.gyroscope.y);
            Serial.print(" z: ");
            Serial.println(storedData.un.gyroscope.z);
            x.flt = storedData.un.gyroscope.x; 
            y.flt = storedData.un.gyroscope.y; 
            z.flt = storedData.un.gyroscope.z; 

            destination[0] = x.u8[0];
            destination[1] = x.u8[1];
            destination[2] = x.u8[2];
            destination[3] = x.u8[3];
            destination[4] = y.u8[0];
            destination[5] = y.u8[1];
            destination[6] = y.u8[2];
            destination[7] = y.u8[3];
            destination[8] = z.u8[0];
            destination[9] = z.u8[1];
            destination[10] = z.u8[2];
            destination[11] = z.u8[3];

            break;
        case SH2_MAGNETIC_FIELD_CALIBRATED:
            Serial.print("Magnetic Field - x: ");
            Serial.print(storedData.un.magneticField.x);
            Serial.print(" y: ");
            Serial.print(storedData.un.magneticField.y);
            Serial.print(" z: ");
            Serial.println(storedData.un.magneticField.z);

            
            x.flt = storedData.un.magneticField.x; 
            y.flt = storedData.un.magneticField.y; 
            z.flt = storedData.un.magneticField.z; 

            destination[0] = x.u8[0];
            destination[1] = x.u8[1];
            destination[2] = x.u8[2];
            destination[3] = x.u8[3];
            destination[4] = y.u8[0];
            destination[5] = y.u8[1];
            destination[6] = y.u8[2];
            destination[7] = y.u8[3];
            destination[8] = z.u8[0];
            destination[9] = z.u8[1];
            destination[10] = z.u8[2];
            destination[11] = z.u8[3];
            break;
        case SH2_LINEAR_ACCELERATION:
            Serial.print("Linear Acceration - x: ");
            Serial.print(storedData.un.linearAcceleration.x);
            Serial.print(" y: ");
            Serial.print(storedData.un.linearAcceleration.y);
            Serial.print(" z: ");
            Serial.println(storedData.un.linearAcceleration.z);

            x.flt = storedData.un.linearAcceleration.x; 
            y.flt = storedData.un.linearAcceleration.y; 
            z.flt = storedData.un.linearAcceleration.z; 

            destination[0] = x.u8[0];
            destination[1] = x.u8[1];
            destination[2] = x.u8[2];
            destination[3] = x.u8[3];
            destination[4] = y.u8[0];
            destination[5] = y.u8[1];
            destination[6] = y.u8[2];
            destination[7] = y.u8[3];
            destination[8] = z.u8[0];
            destination[9] = z.u8[1];
            destination[10] = z.u8[2];
            destination[11] = z.u8[3];
            break;
        case SH2_ROTATION_VECTOR:
            Serial.print("Rotation Vector - r: ");
            Serial.print(storedData.un.rotationVector.real);
            Serial.print(" i: ");
            Serial.print(storedData.un.rotationVector.i);
            Serial.print(" j: ");
            Serial.print(storedData.un.rotationVector.j);
            Serial.print(" k: ");
            Serial.println(storedData.un.rotationVector.k);

            
            real.flt = storedData.un.rotationVector.real; 
            i.flt = storedData.un.rotationVector.i; 
            j.flt = storedData.un.rotationVector.j; 
            k.flt = storedData.un.rotationVector.k; 


            destination[0] = real.u8[0];
            destination[1] = real.u8[1];
            destination[2] = real.u8[2];
            destination[3] = real.u8[3];
            destination[4] = i.u8[0];
            destination[5] = i.u8[1];
            destination[6] = i.u8[2];
            destination[7] = i.u8[3];
            destination[8] = j.u8[0];
            destination[9] = j.u8[1];
            destination[10] = j.u8[2];
            destination[11] = j.u8[3];
            destination[12] = k.u8[0];
            destination[13] = k.u8[1];
            destination[14] = k.u8[2];
            destination[15] = k.u8[3];
            break;

        case SH2_STABILITY_CLASSIFIER: {
            Serial.print("Stability Classification: ");
            sh2_StabilityClassifier_t stability = storedData.un.stabilityClassifier;
            destination[0] = stability.classification;
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
        
    }

private:
    sh2_SensorValue_t storedData;
    float_bytes x;
    float_bytes y;
    float_bytes z;
    float_bytes real;
    float_bytes i;
    float_bytes j;
    float_bytes k;
};
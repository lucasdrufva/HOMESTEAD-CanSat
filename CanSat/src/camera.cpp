#include "camera.h"

#include "pins.h"

extern Mode deviceMode;

bool Camera::begin()
{
    pinMode(PIN_CAMERA_TRIGGER, OUTPUT);
    digitalWrite(PIN_CAMERA_TRIGGER, HIGH);
}

void Camera::run()
{
    switch (deviceMode)
    {
    case MODE_PAD:
        waitTime = 10000;
        break;
    case MODE_FLIGHT:
        waitTime = 1000;
        break;
    case MODE_RETRIEVAL:
    case MODE_EXPORT:
        return;
    default:
        break;
    }
    if(millis() - lastPic > waitTime)
    {
        Serial.println("trigger camera now!!!");
        digitalWrite(PIN_CAMERA_TRIGGER, LOW);
        delay(10);
        digitalWrite(PIN_CAMERA_TRIGGER, HIGH);
        lastPic = millis();
    }
}


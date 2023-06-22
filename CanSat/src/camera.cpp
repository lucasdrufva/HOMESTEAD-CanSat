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
    if (started)
    {
        if (millis() - lastPic > waitTime)
        {
            Serial.println("trigger camera now!!!");
            digitalWrite(PIN_CAMERA_TRIGGER, LOW);
            delay(50);
            digitalWrite(PIN_CAMERA_TRIGGER, HIGH);
            lastPic = millis();
        }
    }
    if(deviceMode == MODE_FLIGHT){
        started = true;
    } 
}

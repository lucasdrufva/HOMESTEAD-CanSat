#include "gps_sensor.h"

#include "gps_data.h"

bool GpsSensor::begin()
{
    GpsSerial.begin(9600);
    return true;
}

void GpsSensor::run()
{
    while (GpsSerial.available())
    {
        if (gps.encode(GpsSerial.read()))
        {
            if(gps.location.lat() == lat && gps.location.lng() == lng)
            {
                continue;
            }

            lat = gps.location.lat();
            lng = gps.location.lng();
            updated = true;
        }
    }

    if(!(millis()-lastUpdate > period) || !updated)
    {
        return;
    }
        

    lastUpdate = millis();
    Serial.print("Sent gps, lat:");
    Serial.print(lat);
    Serial.print(" long: ");
    Serial.println(lng);

    GpsData *data = new GpsData(lat, lng);
    updated = false;
    dataCallback(data);
}
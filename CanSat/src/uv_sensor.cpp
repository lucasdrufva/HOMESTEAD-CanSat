#include "uv_sensor.h"

#include "uv_data.h"

bool UvSensor::begin()
{  
    Wire.begin();
    SL01.begin();
}

void UvSensor::run()
{
    if(!(millis()-lastUpdate > period))
        return;

    lastUpdate = millis();

    float uv;
    float lux;
	uv = 0;

	// Request SL01 to return calculated UVB intensity
    lux = SL01.getLUX();
	uv = SL01.getUVIndex();
	// Display Data on the Serial monitor
	UvData *data = new UvData(uv, lux);
    dataCallback(data);
}
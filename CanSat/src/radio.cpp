#include "radio.h"

#include "status_data.h"

#include <Arduino.h>

#define RF95_FREQ 434.0

extern Mode deviceMode;

bool Radio::begin()
{
    setSpiModeRadio();

    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(100);
    
    if (!rf95.init())
    {
        Serial.println("Failed init radio");
        return false;
    }

    rf95.setFrequency(RF95_FREQ);
    rf95.setPayloadCRC(false);
    return true;
}

UnitStatus Radio::test()
{
    return UNIT_STATUS_OK;
}

void Radio::run()
{
    setSpiModeRadio();

    if(rf95.available())
    {
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        if (!rf95.recv(buf, &len))
        {
            Serial.println("No radio data :(");
            return;
        }

        Serial.println("Got radio data!!!!!");
        for(int i = 0; i < len; i++)
        {
            char tmp[3];
            tmp[2] = 0;
            sprintf(tmp, "%02x", buf[i]);
            Serial.print(tmp);
        }
        Serial.println("");
        delay(1000);
        
        if(buf[0] == DATA_TYPE_STATUS)
        {
            StatusData *data = new StatusData((StatusType)buf[1]);
            dataCallback(data);
        }
    }
}

void Radio::logData(Data *data)
{
    if(deviceMode == MODE_FLIGHT && (data->getType() == DATA_TYPE_ACCELERATION || data->getType() == DATA_TYPE_GYRO)){
        if(!(millis()-lastBno > 5000))
            return;
        lastBno = millis();
    }

    setSpiModeRadio();
    uint8_t packet[100];
    packet[0] = data->getType();
    writeTime(packet+1);
    size_t size = 5 + data->getSize();
    data->writeData(packet+5);
    rf95.send(packet, size);
    for(int i = 0; i < size; i++)
    {
        Serial.print((char)packet[i], HEX);
    }
    Serial.println("....Waiting for packet to complete..."); //delay(10);
    rf95.waitPacketSent(100);
    //rf95.waitPacketSent(100);
    //delay(100);
    rf95.setModeIdle();
}

void Radio::setSpiModeRadio()
{
    // TODO replace with defined pins
    SPI._uc_pinMosi = 10;
    SPI._uc_pinMiso = 8;

    SPI._padTx = SPI_PAD_3_SCK_1;
    SPI._padRx = SERCOM_RX_PAD_0;
    SPI.begin();
}

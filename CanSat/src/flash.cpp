#include "flash.h"

#include <Arduino.h>

#include "pins.h"

union u8_u32
{
    uint32_t u32;
    uint8_t u8[sizeof(float)];
};

bool Flash::begin()
{
    setSpiModeFlash();
    if(!flash.initialize()){
        Serial.println("Failed init flash");
        return false;
    }

    //Setup structure if needed
    //flash.blockErase64K(0);

    uint32_t curr_addr = 0;

    while(true)
    {

        u8_u32 start_symbol;

        flash.readBytes(curr_addr, start_symbol.u8, 4);
        bool found = true;
        if(start_symbol.u32 != START_SYMBOL_U32)
        {
            //break;
            found = false;
            //break;
            for(int i = 0; i < 1024; i++){
                flash.readBytes(curr_addr+i, start_symbol.u8, 4);
                if(start_symbol.u32 == START_SYMBOL_U32)
                {
                    found = true;
                    curr_addr += i;  
                    break;
                }
            }
        }
        if(!found){
            break;
        }
        DataType type = static_cast<DataType>(flash.readByte(curr_addr+4));
        curr_addr += 4 + 1 + 4 + dataLength[type];
    }

    write_addr = curr_addr;

    Serial.print("Start addr:");
    Serial.println(curr_addr);


    return true;
}

UnitStatus Flash::test()
{
    //TODO read flash id
    return UNIT_STATUS_OK;
}

void Flash::logData(Data *data)
{
    Serial.println("Writing at: ");
    Serial.println(write_addr);
    setSpiModeFlash();
    //Serial.print("mod");
    uint8_t packet[100] = {0xde, 0xad, 0xbe, 0xef};
    //Serial.println("Packet created");
    //DataType type = data->getType();
    //DataType type = DATA_TYPE_BATTERY;
    //Serial.println("Type gotten");
    //Serial.println((uint32_t)data);
    packet[4] = data->getType();
    //packet[4] = type;
    //Serial.println("Time to write time");
    writeTime(packet+5);
    //Serial.println("Time written");
    size_t size = 9 + data->getSize();
    //Serial.println("Size gotten");
    data->writeData(packet+9);
    //Serial.println("Time to write to flash");
    flash.writeBytes(write_addr, packet, size);
    //Serial.println("written");
    write_addr += size;

    if(((int) (write_addr / 4096)) > page){
        Serial.println("time to Erase stuff");
        page = write_addr / 4096;
        flash.blockErase4K((page+1)*4096);
        Serial.print("Erased stuff, page: ");
        Serial.println(page);
    }
}

void Flash::setSpiModeFlash()
{
    // TODO replace with defined pins
    SPI._uc_pinMosi = 8;
    SPI._uc_pinMiso = 10;

    SPI._padTx = SPI_PAD_0_SCK_1;
    SPI._padRx = SERCOM_RX_PAD_3;
    SPI.begin();
}

void Flash::printFlightData(uint32_t start_addr){
    char t[9];
    t[8] = 0;
    sprintf(t, "%08lx", start_addr);
    Serial.print("Packet: ");
    Serial.println(t);
    while(true)
    {
        u8_u32 start_symbol;

        flash.readBytes(start_addr, start_symbol.u8, 4);
        Serial.print("Sym:");
        Serial.println(start_symbol.u32);

        if(start_symbol.u32 != START_SYMBOL_U32)
        {
            break;
        }
        DataType type = static_cast<DataType>(flash.readByte(start_addr+4));
        //TODO print data
        for(uint32_t i = start_addr; i < start_addr+4+1+4+dataLength[type]; i++)
        {
            char tmp[3];
            tmp[2] = 0;
            sprintf(tmp, "%02x", flash.readByte(i));
            Serial.print(tmp);
        }

        Serial.println(); 
        start_addr += 4 + 1 + 4 + dataLength[type];
    }
}

void Flash::exportData()
{
    while(!Serial){delay(10);}

    setSpiModeFlash();
    
    uint32_t curr_addr = 0;
    while(true)
    {
        // delay(20);
        // digitalWrite(PIN_LED, LOW);


        u8_u32 start_symbol;

        flash.readBytes(curr_addr, start_symbol.u8, 4);

        if(start_symbol.u32 != START_SYMBOL_U32)
        {
            bool found = false;
            //break;
            for(int i = 0; i < 1024; i++){
                flash.readBytes(curr_addr+i, start_symbol.u8, 4);
                if(start_symbol.u32 == START_SYMBOL_U32)
                {
                    found = true;
                    curr_addr += i;  
                    break;
                }
            }
            if(!found){
                //Serial.print("Ended at");
                //Serial.println(curr_addr);
                return;
            }
            
        }
        DataType type = static_cast<DataType>(flash.readByte(curr_addr+4));

        // delay(20);
        // digitalWrite(PIN_LED, HIGH);

        for(uint32_t i = curr_addr; i < curr_addr+4+1+4+dataLength[type]; i++)
        {
            char tmp[3];
            tmp[2] = 0;
            sprintf(tmp, "%02x", flash.readByte(i));
            Serial.print(tmp);
        }

        Serial.println(); 
        curr_addr += 4 + 1 + 4 + dataLength[type];     
    }

}
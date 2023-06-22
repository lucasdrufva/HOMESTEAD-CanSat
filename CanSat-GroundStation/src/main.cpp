#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include "SSD1306Wire.h"

// #include <RH_RF95.h>
// #include <RHSPIDriver.h>
// // #include <RHSoftwareSPI.h>
// // RHSoftwareSPI spi;

#define RFM95_CS 18
//#define RFM95_CS 4
#define RFM95_RST 23
//#define RFM95_RST 2
#define RFM95_INT 26
//#define RF95_FREQ 434.0

#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     23   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)

SSD1306Wire display(0x3c, SDA, SCL);


//RH_RF95 rf95; //(RFM95_CS, RFM95_INT);

void setup() {
  //TODO change to higher baud
  Serial.begin(9600);

  while (!Serial);

  pinMode(RST, OUTPUT);
  digitalWrite(RST, LOW);
  delay(10);
  digitalWrite(RST, HIGH);
  delay(10);


  Serial.println("Im ready!");
  

  SPI.begin(SCK,MISO,MOSI,SS);

  // if(!rf95.init())
  // {
  //   while (1)
  //   {
  //     Serial.println("Starting rfm failerd!");
  //     delay(1000);
  //   }
  // }

  LoRa.setPins(SS,RST,DI0); 
  //spi.setPins(MISO, MOSI, SCK); //// MISO 12, MOSI 11, SCK  13 

  // ÄNDRA FREKVENS HÄR!!!
  // ex 433.150mhz = 433150000
  // Alltså enheten är i hertz
  if(!LoRa.begin(434000000))
  {
    while (1)
    {
      Serial.println("Starting LoRa failerd!");
      delay(1000);
    }
  }

  

  Serial.println("Started lora");

  //rf95.setFrequency(RF95_FREQ);

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
}

enum StatusType : uint8_t
{
  STATUS_TYPE_START = 1,
  STATUS_TYPE_MODE_PAD = 2,
  STATUS_TYPE_MODE_FLIGHT = 3,
  STATUS_TYPE_MODE_DROP = 4,
  STATUS_TYPE_MODE_RETRIEVAL = 5,
};

void sendCommand(StatusType status)
{
  LoRa.beginPacket();
  // //Set mode
  LoRa.write(0xff);
  LoRa.write(0xff);
  LoRa.write(0x00);
  LoRa.write(0x00);
  LoRa.write(11);
  LoRa.write((uint8_t)status);
  LoRa.endPacket();

  // uint8_t packet[] = {11, status};
  // rf95.send(packet, 2);
  // rf95.waitPacketSent(100);
  // delay(100);
  // rf95.setModeIdle();

  // Serial.print("LORA: Sent packet: ");
  // Serial.println((u_int8_t)status);
}

int packetCount = 0;

void loop() {
  // if(rf95.available())
  // {
  //   Serial.println("Something?????");
  //   // Should be a message for us now
  //   uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  //   uint8_t len = sizeof(buf);
  //   if (rf95.recv(buf, &len))
  //   {
  //     for(int i = 0; i < len; i++)
  //     {
  //       char tmp[3];
  //       tmp[2] = 0;
  //       sprintf(tmp, "%02x", (uint8_t) buf[i]);
  //       //Serial.print((char)LoRa.read(), HEX);
  //       Serial.print(tmp);
  //     }
  //     Serial.println();

  //     //Display
  //     char pText[20] = "Packet:            ";
  //     pText[19] = 0;
  //     itoa(packetCount++, pText+8, 10);
  //     char rText[20] = "RSSI:              ";
  //     rText[19] = 0;
  //     itoa(rf95.lastRssi(), rText+8, 10);

  //     display.clear();
  //     display.drawString(0,0, pText);
  //     display.drawString(0,24, rText);
  //     display.display();
  //   }
  // }

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // read packet
    while (LoRa.available()) {
      char tmp[3];
      tmp[2] = 0;
      sprintf(tmp, "%02x", (uint8_t) LoRa.read());
      //Serial.print((char)LoRa.read(), HEX);
      Serial.print(tmp);
    }

    Serial.println();

    //Display
    char pText[20] = "Packet:            ";
    pText[19] = 0;
    itoa(packetCount++, pText+8, 10);
    char rText[20] = "RSSI:              ";
    rText[19] = 0;
    itoa(LoRa.packetRssi(), rText+8, 10);

    display.clear();
    display.drawString(0,0, pText);
    display.drawString(0,24, rText);
    display.display();

  }

  if(Serial.available())
  {
    String rec = Serial.readStringUntil('\n');
    if(rec.indexOf("PAD") != -1)
    {
      sendCommand(STATUS_TYPE_MODE_PAD);
    }
    else if(rec.indexOf("FLIGHT") != -1)
    {
      sendCommand(STATUS_TYPE_MODE_FLIGHT);
    }
    else if(rec.indexOf("DROP") != -1)
    {
      sendCommand(STATUS_TYPE_MODE_DROP);
    }
    else if(rec.indexOf("RETRIEVAL") != -1)
    {
      sendCommand(STATUS_TYPE_MODE_RETRIEVAL);
    }
  }
}
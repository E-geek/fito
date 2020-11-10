#include <Arduino.h>
#include "User_Setup.h"

#define DEBUG 1

#define ONE_WIRE_ENABLE

#include "clog.cpp"

uint16_t counter = 0;

#ifdef ONE_WIRE_ENABLE
#define TEMP_ENABLE

#include <OneWire/OneWire.cpp>
#include <Arduino-Temperature-Control-Library/DallasTemperature.cpp>

OneWire ds(4);

DallasTemperature td(&ds);

uint8_t countDS18 = 0;

uint8_t countOneWire = 0;

#define DS2438 0x26

void read2438() {
  ds.reset_search();
  DeviceAddress addr;
  uint8_t i = 0;
  uint8_t data[16];
  clog("Search device...");
  while (ds.search(addr)) {
    if (addr[0] != DS2438) {
      continue;
    }
    clog("Found ds2438");


    // Configure chip to read VAD and not VDD
    ds.reset();
    ds.select(addr);
    ds.write(0x4E);
    ds.write(0x00);
    ds.write(0x00);

    // Convert voltage
    ds.reset();
    ds.select(addr);
    ds.write(0xB4);
    delay(20);

    // Start temperature conversion
    ds.reset();
    ds.select(addr);
    ds.write(0x44);
    delay(20);

    // Current readings are updated automatically

    // Recall
    ds.reset();
    ds.select(addr);
    ds.write(0xB8);
    ds.write(0x00);

    // Read Scratchpad
    ds.reset();
    ds.select(addr);
    ds.write(0xBE);
    ds.write(0x00);

    for ( i = 0; i < 9; i++) { // we need 9 bytes, last one for CRC
      data[i] = ds.read();
      // Serial.print(data[i], HEX);
      // Serial.print(" ");
    }

    // CRC
    if ( OneWire::crc8(data, 8) != data[8]) {
      Serial.println("CRC is not valid!");
      clog("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], OneWire::crc8(data, 8));
    }

    Serial.print("Temp=");
    float temp = (((((int16_t) data[2]) << 8) | data[1]) >> 3) * 0.03125;
    Serial.print(temp);

    Serial.print(" VAD=");
    float vad = ((((int16_t)data[4]) << 8) | data[3]) * 0.01;
    Serial.print(vad);

    Serial.print(" IAD=");
    float iad = ((((int16_t)data[6]) << 8) | data[5] << 2) * 0.2441;
    Serial.print(iad);

    ds.reset();
    ds.select(addr);
    ds.write(0x4E);
    ds.write(0x00);
    ds.write(0x0F);

    // Convert voltage
    ds.reset();
    ds.select(addr);
    ds.write(0xB4);
    delay(20);

    // Recall
    ds.reset();
    ds.select(addr);
    ds.write(0xB8);
    ds.write(0x00);

    // Read Scratchpad
    ds.reset();
    ds.select(addr);
    ds.write(0xBE);
    ds.write(0x00);

    Serial.println();
    for ( i = 0; i < 9; i++) { // we need 9 bytes, last one for CRC
      data[i] = ds.read();
    }

    // CRC
    if ( OneWire::crc8(data, 8) != data[8]) {
      Serial.println("CRC is not valid!");
      clog("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], OneWire::crc8(data, 8));
    }

    Serial.print(" VDD=");
    float vdd = ((((int16_t)data[4]) << 8) | data[3]) * 0.01;
    Serial.print(vdd);

    Serial.println();
  }
}

#endif

void setup() {
  Serial.begin(115200);
  Serial.println("Let's go");

#ifdef ONE_WIRE_ENABLE
#ifdef TEMP_ENABLE
  td.begin();
  countOneWire = td.getDeviceCount();
  countDS18 = td.getDS18Count();
  clog("Count devices: %i\nCount ds18: %i\nSet resolution 12bit", countOneWire, countDS18);
  td.setResolution(12);
#endif
#endif
}

void loop() {
  if (counter++ != 0) {
    return;
  }
  delay(2000);
#ifdef ONE_WIRE_ENABLE
#ifdef TEMP_ENABLE
  td.requestTemperatures();
  for (uint8_t i = 0; i < countDS18; i++) {
    float thermal = td.getTempCByIndex(i);
    char str[7];
    dtostrf(thermal, 2, 2, str); // [-]dd.dd\0
    clog("Thermal: %s", str);
  }
#endif
  read2438();
#endif
}
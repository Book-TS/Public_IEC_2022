#include <myDS18B20.h>
#include <Arduino.h>
#include <OneWire.h>

OneWire ds18(temp_pin); 	// Chân Data

myDS18B20::myDS18B20() {
    ;
}

void myDS18B20::searchAddress() {
  if (!ds18.search(addr)) {
    Serial.println("No more addresses.");
    ds18.reset_search();
    delay(250);
    return;
  }
}

void myDS18B20::testCRC() {
  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return;
  }
}

void myDS18B20::searchTypeSensor() {
  switch (addr[0]) {
  case 0x10:		 	// Serial.println("  Chip = DS18S20");
    type_s = 1;
    break;
  case 0x28:			// Serial.println("  Chip = DS18B20");
    type_s = 0;
    break;
  case 0x22:			 // Serial.println("  Chip = DS1822");
    type_s = 0;
    break;
  default:				// Serial.println("Device is not a DS18x20 device.");
    return;
  }
}

void myDS18B20::init() {
	searchAddress();
	testCRC();
	searchTypeSensor();
}

float myDS18B20::readDS18() {
	ds18.reset();
  	ds18.select(addr);
  	ds18.write(0x44, 1);

	delay(1000);
	present = ds18.reset();
	ds18.select(addr);
	ds18.write(0xBE);

	for (i = 0; i < 9; i++) {
		data[i] = ds18.read();
	}

	int16_t raw = (data[1] << 8) | data[0];
	if (type_s) {
		raw = raw << 3;
		if (data[7] == 0x10) 	 raw = (raw & 0xFFF0) + 12 - data[6];
	}
	else {
		byte cfg = (data[4] & 0x60);

		if (cfg == 0x00)		raw = raw & ~7;
		else if (cfg == 0x20)	raw = raw & ~3;
		else if (cfg == 0x40)	raw = raw & ~1;
	}

	celsius = (float)raw / 16.0 + offer;
	fahrenheit = celsius * 1.8 + 32.0;

	return celsius;
}

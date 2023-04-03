#ifndef myDS18B20_h
#define myDS18B20_h

#include <OneWire.h>

class myDS18B20
{

public:
	myDS18B20();
	
	void init();
	void searchAddress();
	void testCRC();
	void searchTypeSensor();
	float readDS18();

private:
	
	#define temp_pin 5

	byte i;
	byte present = 0;
	byte type_s;   		//  Biến lưu giá trị xác định loại cảm biến
	byte data[12]; 		//  Biến lưu giá trị nhiệt độ (HEX)
	byte addr[8]; 	    //  Biến lưu địa chỉ cảm biến (gồm 8 bit)
	float offer = 0;
	float celsius, fahrenheit;
};
#endif
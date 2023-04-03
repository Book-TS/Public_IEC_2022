// #include <Arduino.h>
// #include <SoftwareSerial.h>
// #include <stdio.h>
// #include <myDS18B20.h>

// #define RS485Rx 7
// #define RS485Tx 2
// #define baudRate 9600

// #define address 0x42 // adrres: B
// #define func_1 0X30  // Function Code: 0
// #define lenght 0x33  // Lenght: 3
// #define data 0x30    // data: 0
// #define crc 0x2E     // CRC: '.'

// unsigned long oldtime_1 = 0;
// unsigned long time_1 = 1000;
// int8_t counter = 0x30;
// boolean LED = false;
// int value = 297;

// String rxValue;
// SoftwareSerial RS485(RS485Rx, RS485Tx);
// myDS18B20 mDS18B20;

// void setup()
// {
//   Serial.begin(9600);
//   pinMode(13, OUTPUT);
//   RS485.begin(baudRate);
//   mDS18B20.init();
// }

// void loop()
// {
//   if ((unsigned long)(millis() - oldtime_1) > time_1)
//   {
//     LED = !LED;
//     digitalWrite(13, LED);
//     Serial.println((char)counter);
//     oldtime_1 = millis();
//   }
//   if (RS485.available())
//   {
//     rxValue = RS485.readStringUntil(0x13);
//     Serial.println(rxValue);
//     delay(10);
//     if (counter >= 0x39)
//       counter = 0x30;
//     else
//       counter += 0x01;
//     if ((rxValue[0] == 'B') && (rxValue[1] == '0'))
//     {
//       int temp = mDS18B20.readDS18()*10;
//       RS485.write(address);
//       RS485.write(func_1);
//       RS485.write(lenght);
//       RS485.print(temp);
//       RS485.write(crc);
//       Serial.print("Nhiet do: ");
//       Serial.println(temp);
//       Serial.println("Done!");
//     }

//     int bufferDelay = (baudRate / 10) * 2 + 100;
//     delayMicroseconds(bufferDelay);
//   }
// }
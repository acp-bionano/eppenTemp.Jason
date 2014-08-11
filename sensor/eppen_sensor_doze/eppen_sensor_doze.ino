//eppen_sensor.ino
//by Andrew Payne 
//rev. 06/11/14
//cribs from http://tushev.org/articles/arduino/item/52-how-it-works-ds18b20-and-arduino

#include <SoftwareSerial.h>
#include <OneWire.h>

OneWire ds(3);
SoftwareSerial tinySerial(8, 2); // RX, TX


#define DS18S20_ID 0x10
#define DS18B20_ID 0x28


void setup()  
{
  tinySerial.begin(9600);
  pinMode(0,OUTPUT);
}

void loop(void)
{
  byte i;
  byte data[12];
  byte addr[8];
  float temp;

  //each sensor has a unique ID
  if (!ds.search(addr)) {
      ds.reset_search();
      return;
  }
  
  //check for valid ID
  if (OneWire::crc8(addr,7) != addr[7]){
    return;
  }  
  if (addr[0] != DS18B20_ID){
    return;
  }
  
  ds.reset(); //needed before any communication can occur
  ds.select(addr);
  
  //0x44 is CONVERT command for data conversion for 2-byte measurement 
  ds.write(0x44, 1); //measurement is stored in sensor scratchpad (RAM)
  delay(9995); 
  ds.reset();
  ds.select(addr);
  
  //0xBE is READ SCRATCHPAD -> 9 data bytes expected
  ds.write(0xBE);
  for (i = 0; i < 9; i++) {
    data[i] = ds.read();
  }
  //multiply by sensor resolution
  temp = ( (data[1] << 8) + data[0])*0.0625;
  
  digitalWrite(0, LOW);
  delay(4);
  tinySerial.print("ID=1112 R=");
  tinySerial.print(temp);
  tinySerial.print("\n");
  delay(1);
  digitalWrite(0, HIGH);
}

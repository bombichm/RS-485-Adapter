#include <SoftwareSerial.h>
#include "RS485_protocol.h"
#include <OneWire.h> 
#include <avr/wdt.h>

SoftwareSerial rs485 (2, 3);  // receive pin, transmit pin
const byte myAddress = 1;     //unique number to each device
const byte ENABLE_PIN = 4;
const byte LED1 = 1;
const byte LED2 = 6;
#define ONEWIRE_BUS 7

OneWire TemperatureSensor(ONEWIRE_BUS);  // Dallas one wire data bus pin, a 4.7K resistor pullup is needed @ the master


void setup()
{
  wdt_enable(WDTO_8S);                                            //8 second watchdog timer
  rs485.begin (28800);
  pinMode (ENABLE_PIN, OUTPUT);  // driver output enable
  blink();
  blink();
}


void loop()
{
  wdt_reset();
  int temp = getTemperature();
  byte buf [10];
  
  byte received = recvMsg (fAvailable, fRead, buf, sizeof (buf));
  
  if (received)
    {
    if (buf [0] != myAddress)
      return;  // not my device
      
    if (buf [1] != 2)
      return;  // unknown command
    
    byte msg [] = {
       0,  // device 0 (master)
       temp,  // return the temperature from the sensor
    };
    
    delay (1);  // give the master a moment to prepare to receive
    digitalWrite (ENABLE_PIN, HIGH);  // enable sending
    sendMsg (fWrite, msg, sizeof msg);
    digitalWrite (ENABLE_PIN, LOW);  // disable sending
    blink();    
    
   }  // end if something received
}  // end of loop

void blink()
  {
    digitalWrite(1,HIGH);
    delay (250);
    digitalWrite(1,LOW);
    delay(175);
  }    
void fWrite (const byte what)
  {
  rs485.write (what);  
  }
  
int fAvailable ()
  {
  return rs485.available ();  
  }

int fRead ()
  {
  return rs485.read ();  
  }

int getTemperature ()
{
    byte i;
    byte data[12];
    int16_t raw;
    float fahrenheit;
 
    TemperatureSensor.reset();       // reset one wire bus
    TemperatureSensor.skip();        // select only device
    TemperatureSensor.write(0x44);   // start conversion
 
//    delay(1000);                     // wait for the conversion
 
    TemperatureSensor.reset();
    TemperatureSensor.skip();
    TemperatureSensor.write(0xBE);   // Read Scratchpad
    for ( i = 0; i < 9; i++) {       // 9 bytes
      data[i] = TemperatureSensor.read();
    }
 
    // Convert the data to actual temperature
    raw = (data[1] << 8) | data[0];
    fahrenheit = (float)raw / 8.889 + 32;
    int temp = fahrenheit;
    return temp;
    }
    

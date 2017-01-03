
#include "RS485_protocol.h"
#include <SoftwareSerial.h>

SoftwareSerial rs485 (2, 3);  // receive pin, transmit pin

const byte ENABLE_PIN = 4;
const byte LED_PIN = 13;

//device addresses:
int tempArray [6];
const byte boilerTemp = tempArray[0];
const byte tankReturn = tempArray[1];
const byte tankTop = tempArray[2];
const byte midTank = tempArray[3];
const byte outdoor = tempArray[4];
const byte boilerRoom = tempArray[5];


void setup()
{
  rs485.begin (28800);
  pinMode (ENABLE_PIN, OUTPUT);  // driver output enable
  pinMode (LED_PIN, OUTPUT);  // built-in LED
}  // end of setup
  

void loop()
{
  for (int address = 0; address < 7; address++)
  {
    int reading = getTemperatures(address);
    tempArray[address] = reading;
  }
}  // end of loop

 // assemble message, ask the devices what their temp readings are
int getTemperatures(byte device)
{
  byte msg [] = 

  { 
     device,    // device #
     2,    // getTemperature
  };

  // send to slave  
  digitalWrite (ENABLE_PIN, HIGH);  // enable sending
  sendMsg (fWrite, msg, sizeof msg);
  digitalWrite (ENABLE_PIN, LOW);  // disable sending

  // receive response  
  byte buf [10];
  byte received = recvMsg (fAvailable, fRead, buf, sizeof buf);
  
  digitalWrite (LED_PIN, received == 0);  // turn on LED if error    
  
  // only send once per successful change
  if (received)
  return buf [1];
}

// callback routines
  
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


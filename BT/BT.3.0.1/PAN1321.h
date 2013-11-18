/*
PAN1321.ino: this file is used to control a PAN1321 module. this
requires flow control. We use RTS and CTS to control when we can 
send. These pins can be any pins you dont need in your design. 
*/
#include "Arduino.h"

#ifndef PAN1321_h
#define PAN1321_h

#define PAN1321_VERSION           1     // software version of this library

#define CTS 2 
#define RTS 14

/*
Init_Bluetooth: initializes the bluetooth module so the host
can connect. In this condition it is discoverable and requires
the passweord 1234
*/
void Init_Bluetooth() {

  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  // initialize RTS and CTS respectivley
  pinMode(RTS, OUTPUT);
  pinMode(CTS, INPUT);

  // set RTS high
  digitalwrite(RTS, HIGH);  
  digitalWrite(RTS, LOW);
  
  delay(2000);
  // wait for the clear to send from the BT module
  while(digitalRead(CTS) != LOW) { } 
    Serial.println("AT+JRES" );
  delay(4000);   
  while(digitalRead(CTS) != LOW) { }  
    Serial.println("AT+JSEC=1,2,2,04,1234" );  
  while(digitalRead(CTS) != LOW) { }  
    Serial.println("AT+JDIS=3" );
  while(digitalRead(CTS) != LOW) { }  
    Serial.println("AT+JRLS=1101,11,Serial port,01,000000" );
  while(digitalRead(CTS) != LOW) { } 
    Serial.println("AT+JAAC=1" );  
  while(digitalRead(CTS) != LOW) { }
  
  // wait for a device to connect and then enter streaming mode 
  while(Serial.read() != '+') { }
  while(Serial.read() != 'R') { } 
  while(Serial.read() != 'C') { } 
  while(Serial.read() != 'C') { }
 
  Serial.println("AT+JSCR");
  // this delay allows the BT "OK" checks to be cleared
  delay(1000);
  digitalWrite(RTS, LOW);
}

#endif
'''
PAN1321.ino: this file is used to control a PAN1321 module. this
requires flow control. We use RTS and CTS to control when we can 
send. These pins can be any pins you dont need in your design. 
'''
#include "Arduino.h"

#ifndef PAN1321_h
#define PAN1321_h

#define CTS 2 
#define RTS 14

'''
setup_PAN1321: initializes the pins and serial required
to use the bluetooth module
'''
void setup_PAN1321() {  
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
fv vuo   
  // initialize RTS and CTS respectivley
  pinMode(RTS, OUTPUT);
  pinMode(CTS, INPUT);

  // set RTS high
  digitalwrite(RTS, HIGH) 
 
}

'''
Init_Bluetooth: initializes the bluetooth module so the host
can connect. In this condition it is discoverable and requires
the passweord 1234
'''
void Init_Bluetooth() {

  digitalWrite(RTS, LOW);
  
  //while(digitalRead(CTS) != LOW) { } 
    //Serial.println("AT+JRES" );   
  while(digitalRead(CTS) != LOW) { }  
    Serial.println("AT+JSEC=1,2,2,04,1234" );  
  while(digitalRead(CTS) != LOW) { }  
    Serial.println("AT+JDIS=3" );
  while(digitalRead(CTS) != LOW) { }  
    Serial.println("AT+JRLS=1101,11,Serial port,01,000000" );
  while(digitalRead(CTS) != LOW) { } 
    Serial.println("AT+JAAC=1" );  

  // this delay allows the BT "OK" checks to be cleared
  delay(200);
  digitalWrite(RTS, HIGH);
}

'''
SendChar_Bluetooth: sends a single character via bluetooth
'''
void SendChar_Bluetooth(char currentsend){
  while(digitalRead(CTS) != LOW)   { } 
    Serial.print("AT+JSDA=001,"); 
    Serial.print(currentsend);
    Serial.println();

    // used to clear the RX buffer of the PAN1321 try to get rid
    digitalWrite(RTS, LOW);
    delay(300);
    digitalWrite(RTS, HIGH);
}

'''
SendGet_Bluetooth: get a single character from bluetooth
'''
char GetChar_Bluetooth(){
  // clear the BT module to send last string
  digitalWrite(RTS, LOW)
  while (Serial.available()) {

    // read until the deliminator shows up
    if ((char)Serial.read() == ',') {
      digitalWrite(RTS, HIGH);
      return((char)Serial.read());
      break;
    }
  }
}

#endif
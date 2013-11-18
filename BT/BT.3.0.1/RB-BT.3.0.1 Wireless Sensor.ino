/* This program will read a sensor value on A0 and will send the value over
the bluetooth booster pack. 
  -the device will appear as com port on your computer
  -open the com port with putty at 9600 baud
  -you will then see the sensor values being printed
*/
#include "PAN1321.h"

const int analogInPin = A0; 
int sensorValue = 0;    

// the serial and bluetooth are all intialized in Init_Bluetooth   
void setup() {
  Init_Bluetooth();
}

void loop() {

  // read the analog in value:
  sensorValue = analogRead(analogInPin);           

  // send the results over bluetooth
  Serial.print("sensor = " );                       
  Serial.println(sensorValue);      

  // wait 100 ms until sending the next value - 10Hz
  delay(100);                     
}
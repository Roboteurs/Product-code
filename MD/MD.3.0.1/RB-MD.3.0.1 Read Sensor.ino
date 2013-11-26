/* This progam will read a sensor attached to the RB-MD.3.0.1 booster pack.
the sensor it reads is on pin 2

@Directions: To view the output of this program open device manager and look under COM and LPT
you should see the lauchpad listed as "COMXX" use putty and open this COM port to view the 
values. Check the video tutorial for more info
*/

void setup() {

  Serial.begin(9600);
}

// print the value every second
void loop() {
  
  // read the analog pin and print the value
  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  // wait one second
  delay(1);
}

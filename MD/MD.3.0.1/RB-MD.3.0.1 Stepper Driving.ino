/* This program will drive a 2 phase (4 wire) bipolar stepper motor
with the RB-MD.3.0.1 luachpad booster pack
*/

#include <Stepper.h>

const int stepsToTravel = 200;

// setup stepper motor on correct pins to match the RB-MD.3.0.1
Stepper myStepper(stepsPerRevolution, 9,10,12,13);            

void setup() {
  myStepper.setSpeed(60);
}

void loop() {

  // rotate the stepper in one direct
  myStepper.step(stepsToTravel);
  delay(500);
  
  // rotate in the oposite direction
  myStepper.step(-stepsToTravel);
  delay(500); 
}


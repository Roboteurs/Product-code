/*

This program will actuate a servo motor back and forth from 0 to 180 degrees

*/

#include <Servo.h>                                // add the servo header
 
Servo servo1;                                     // the name of our first servo (more can be added) 
int angle1 = 0;                                   // the angle we want our first servo to move

 
void setup() { 

  servo1.attach(9);                               // setup the servo, it is now ready for use!                
}   


void loop() {                                 

  for(angle1 = 0; angle1 < 180; angle1 += 1) {   // here repeatatley move the servo while increasing angle

    servo1.write(angle1);                        // this is the important part where we write the angle to the servo
    delay(15);                                   // here you can change the speed, try using 50 
  } 
} 
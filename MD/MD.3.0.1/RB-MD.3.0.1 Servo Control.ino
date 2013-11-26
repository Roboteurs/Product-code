/* This program will actuate a servo motor back and forth from 0 to 180 degrees
*/
#include <Servo.h>                                
 
 // define the servo object and the angle variable
Servo servo1;                                   
int angle1 = 0;                                   
 
void setup() { 

  servo1.attach(9);                                             
}   

void loop() {                                 

	// increment the servo through 180 degrees
  	for(angle1 = 0; angle1 < 180; angle1 += 1) {   

  		// update the angle and wait a little
    	servo1.write(angle1);                        
    	delay(15);                                   
  } 
} 
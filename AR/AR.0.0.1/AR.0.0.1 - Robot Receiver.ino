//define robot drive electronic parameters
const int M1ForwardPin  = 9;
const int M1ReversePin  = 10;
const int M2ForwardPin  = 12;
const int M2ReversePin  = 14;

const int EnablePin = 6;

//required for serial event 
String inputString = ""; 
boolean stringComplete = false; 

int counter = 0;
int safteycounter = 0;
  
void setup() {
  // initialize serial:
  Serial.begin(9600);
  inputString.reserve(200);

  // setup the PWM pins for the motor driver
  pinMode(M1ForwardPin, OUTPUT);
  pinMode(M1ReversePin, OUTPUT);
  pinMode(M2ForwardPin, OUTPUT);
  pinMode(M2ReversePin, OUTPUT);
  
  pinMode(EnablePin, OUTPUT);
  
  analogWrite(M1ForwardPin, 0);
  analogWrite(M1ReversePin, 0);
  analogWrite(M2ForwardPin, 0);
  analogWrite(M2ReversePin, 0);

}

void loop() {

  int M1ForwardPwm, M1ReversePwm, M2ReversePwm, M2ForwardPwm;

  // print the string when a newline arrives:
    if (stringComplete) {
      stringComplete = false;
      if(inputString[0] = 'Q'){
        M1ForwardPwm = (inputString[3] - 48) + (((inputString[2])-48) * 10) + (((inputString[1])-48) * 100);
        M1ReversePwm = (inputString[7] - 48) + (((inputString[6])-48) * 10) + (((inputString[5])-48) * 100);
        M2ForwardPwm = (inputString[11] - 48) + (((inputString[10])-48) * 10) + (((inputString[9])-48) * 100);
        M2ReversePwm = (inputString[15] - 48) + (((inputString[14])-48) * 10) + (((inputString[13])-48) * 100);
        
        //write the PWM values to the respective variable
        analogWrite(M1ForwardPin, M1ForwardPwm);
        analogWrite(M1ReversePin, M1ReversePwm);
        analogWrite(M2ForwardPin, M2ForwardPwm);
        analogWrite(M2ReversePin, M2ReversePwm);
        
        //clear the input string
        inputString = "";
        safteycounter = 0;
    }
  }
  
  if (counter == 3000){
    digitalWrite(EnablePin, LOW);
    delay(2);
    digitalWrite(EnablePin, HIGH);
    delay(2);
    counter == 0;
  }

  if (safteycounter == 15000){

    //turn off all motors
    analogWrite(M1ForwardPin, 0);
    analogWrite(M1ReversePin, 0);
    analogWrite(M2ForwardPin, 0);
    analogWrite(M2ReversePin, 0);
    safteycounter = 0;
  }

  counter++;
  safteycounter++;
    
  
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}
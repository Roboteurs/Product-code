// this program is not fully developed. It was created for testing purposes. 


#define CTS 2
#define RTS 14

char inchar = '0';
char valray[4];

void setup()
{ 
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  // initialize RTS and CTS respectivley
  pinMode(RTS, OUTPUT);
  pinMode(CTS, INPUT);

  // set RTS high
  digitalWrite(RTS, LOW); 
  
  pinMode(9, OUTPUT);  
  digitalWrite(9, LOW);
  pinMode(10, OUTPUT);  
  digitalWrite(10, LOW);
  pinMode(12, OUTPUT);  
  digitalWrite(12, LOW);
  pinMode(13, OUTPUT);  
  digitalWrite(13, LOW);
}

char getchar(){
  //digitalWrite(RTS, HIGH);
  while(Serial.available()){
    //digitalWrite(RTS, LOW);
    inchar = Serial.read();
  }
    //digitalWrite(RTS, LOW);
 
    return(inchar);    
}

void SendChar_Bluetooth(char currentsend){
  while(digitalRead(CTS) != LOW)   { } 
    Serial.print("AT+JSDA=001,"); 
    Serial.print(currentsend);
    Serial.println();
    analogWrite(RTS, 127);
    delay(300);
    analogWrite(RTS, 0);
}

void loop()
{
    //delay(100);
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
  while(Serial.read() != '+') { }
  while(Serial.read() != 'R') { } 
  while(Serial.read() != 'C') { } 
  while(Serial.read() != 'C') { }
 
  Serial.println("AT+JSCR");
  // this delay allows the BT "OK" checks to be cleared
  delay(1000);
  digitalWrite(RTS, LOW);
  
  while(1){
    
    //check for the clarification byte
    if(getchar() == 'Q'){
     ///get all the values at once
    while(Serial.available()){  
    if(Serial.read() == '1'){
      digitalWrite(10, HIGH);
      digitalWrite(9, LOW);
    }
    else{
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
    }
    }
    while(Serial.available()){
      inchar = (char)Serial.read();
    }
    
    while(Serial.available()){  
    if(Serial.read() == '1'){
      digitalWrite(12, HIGH);
      digitalWrite(13, LOW);
    }
    else{
      digitalWrite(13, HIGH);
      digitalWrite(12, LOW);
    }
    }


    }
}

}

 

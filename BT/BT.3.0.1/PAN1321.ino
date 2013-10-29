#ifndef PAN1321_h
#define PAN1321_h

#define CTS 1 
#define RTS 2


'''
setup_PAN1321: initializes the pins and serial required
to use the bluetooth module
'''
void setup_PAN1321() {  
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

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
  
  digitalwrite(RTS, LOW)
  // wait for the clear to send from the BT module
  while(digitalRead(CTS) != HIGH) { } 
    Serial.print("AT+JRES\r" ); 
  while(digitalRead(CTS) != HIGH) { }  
    Serial.println("AT+JSEC=1,2,2,04,1234\r" );  
  while(digitalRead(CTS) != HIGH) { }  
    Serial.println("AT+JDIS=3\r" );
  while(digitalRead(CTS) != HIGH) { }  
    Serial.println("AT+JRLS=1101,11,Serial port,01,000000\r" );
  while(digitalRead(CTS) != HIGH) { } 
    Serial.println("AT+JAAC=1\r" );  

  // this delay allows the BT "OK" checks to be cleared
  delay(100);
  digitalwrite(RTS, HIGH);

}

'''
SendChar_Bluetooth: sends a single character via bluetooth
'''
void SendChar_Bluetooth(char currentsend){
  while(digitalRead(CTS) != HIGH) { } 
    Serial.print("AT+JSDA=009,"); 
    Serial.print(currentsend);
    Serial.print("\r");
}


#endif
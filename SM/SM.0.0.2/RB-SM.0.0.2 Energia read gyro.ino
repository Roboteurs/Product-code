/* This program will read only the x axis of the L3GD20 gyro, it will then print
the values to the serial terminal

@Directions: Attached the SCL, SDA, GND and Vcc pins of the sensor module to the launchpad. Open
the serial port with the serial monitor to view the data
*/

// library to handel the i2c functions
#include <Wire.h> 

void setup()
{
  Serial.begin(9600);     //start serial at baud 9600
  Wire.begin();           //start i2c bus as master
  
  i2cset(0x6a, 0x20, 0xff);   //setup the accelermeter before taking readings
}

void loop()
{  
  byte LSB, MSB;
  int RESULT;   
  
  LSB = i2cget(0x6a, 0x28);   //read first byte of z axis
  MSB = i2cget(0x6a, 0x29);   //read second byte
  RESULT = (MSB << 8) + LSB;

  Serial.println(RESULT, DEC);    //print the value of the data as a decimal
  delay(100);
}

// read a single byte from the i2c bus
byte i2cget(byte i2cadr, byte reg)
{
  Wire.beginTransmission(i2cadr);
  Wire.write(reg);                    //set address pointer
  Wire.endTransmission(false);        
  Wire.requestFrom(i2cadr, 1);        //request byte from addresstoread
  while(!Wire.available());             //wait for avalible byte
  return Wire.read();                   //read byte
}

// write a byte to and address on the i2c bus
void i2cset(byte i2cadr, byte reg, byte data)
{
  Wire.beginTransmission(i2cadr);
  Wire.write(reg);                  //set address pointer
  Wire.write(data);                     //write data
  Wire.endTransmission();
}
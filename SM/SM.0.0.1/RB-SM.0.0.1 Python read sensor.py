# smbus is the base package used in this code. It should alread be
# installed on the Beaglbone or Raspberry Pi. if not use
# sudo apt-get install python-smbus (Pi)
# opkg install python-smbus (BB)

import smbus as smbus
import time
import sys

# can be found using i2c detect
i2cadr = 0x1c		

# define the i2c object on i2c bus 1
i2c = smbus.SMBus(1)

#intialize the sensor by writing to its config address
i2c.write_byte_data(i2cadr, 0x2a, 0x01)

#continualy update the value of x-axis on screen
while 1 :
	LSB = i2c.read_byte_data(i2cadr, 0x02)
	MSB = i2c.read_byte_data(i2cadr, 0x01)
	RESULT = ((MSB << 8) + LSB) >> 2
	print (int(RESULT))
	time.sleep(.1)
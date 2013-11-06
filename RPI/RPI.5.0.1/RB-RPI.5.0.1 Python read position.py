#pynmea is an external module. It is included with the Roboteurs PI image
import serial, time, threading
import RPi.GPIO as GPIO
from pynema import nmea

#init the serial object at the GPS default baud rate
s = serial.Serial('/dev/ttyAMA0' , 9600, timeout = 5)

#init the GPS module. this is done by pulsing the on/off pin on the GPS
def start_GPS():
	GPIO.setmode(GPIO.BCM)
	GPIO.setup(4, GPIO.OUT)
	GPIO.output(4, True)
	time.sleep(.01)
	GPIO.output(4, False)

#init the GPS module. this is done by pulsing the on/off pin on the GPS
def stop_GPS():
	GPIO.output(4, True)
	time.sleep(.01)
	GPIO.output(4, False)
	#GPIO.cleanup()

#function to read and filter the NMEA phrases 
def read_GPSNMEA():
	global alt, lng, lat
	while 1:		
		line = s.readline())
		linesplit = str.split(str=',', line)
		if linesplit[0] == '$GPGGA':
			gpgga = nmea.GPGGA()
			gpgga.parse(line)
			alt = gpgga.antenna_altitude
			lat = gpgga.latitude
			lng = gpgga.longitude
			print ("Altitude: " & str(alt) & " Longitude: " & str(lng) & " Latitude " & str(lat))

#set the read function as the first task 
task_1 = threading.Thread(target=read_GPSNMEA)

#turn the GPS on and then start reading for 100 seconds. The GPS must be
#turned off when finished to prevent data loss and cold starts
start_GPS()
task_1.start()
time.sleep(100)
task_1.stop()
stop_GPS()





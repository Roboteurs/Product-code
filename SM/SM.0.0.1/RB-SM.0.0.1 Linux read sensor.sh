# install i2c-tools
sudo apt-get install i2c-tools

# check the i2c buss for a device you should see the device in the output
sudo i2cdetect -r 1

# check the current sensor status, will return 0x00 if device inactive
sudo i2cget -f -y 1 0x1c 0x0b

# enable the sensor at max sampling rate
sudo i2cset -f -y 1 0x1c 0x2a 0x01

while :
do
	# read sensor data from adresses 0x01 - 0x06
	sudo i2cget -f -y 1 0x1c 0x01
done
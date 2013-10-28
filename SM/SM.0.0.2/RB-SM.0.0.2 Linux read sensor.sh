# install i2c-tools
sudo apt-get install i2c-tools

# check the i2c buss for a device you should see the device in the output
sudo i2cdetect -r 1

# check the device ID
sudo i2cget -f -y 1 0x6b 0x20

# enable the sensor at max sampling rate
sudo i2cset -f -y 1 0x6b 0x2a 0xff

# read sensor data from adresses 0x01 - 0x06
sudo i2cget -f -y 1 0x6b 0x28

while :
do
	# read the temprature from adress 0x26
	sudo i2cget -f -y 0x6b 0x26
done
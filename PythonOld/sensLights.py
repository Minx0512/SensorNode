#!/usr/bin/python

import sys
import re
import subprocess
from BluetoothConn import *

prThresh = 550
send_str = "550\n"
#send_str +="\n"

sensorData= ReadSensorData("98:D3:31:FB:31:A6",send_str)
print sensorData
try:
 pr= int(re.search("<NSL19M51>(.*)</NSL19M51>",sensorData)[0])
except IOError as e:
 print "{0}".format(e)

ds18b20 = re.findall("<DS18B20><ID>(.*)</ID><T>(.*)\xc2\xb0C</T></DS18B20>",sensorData)
dht = re.findall("<DHT22><T>(.*)\xc2\xb0C</T><H>(.*)%RH</H></DHT22>",sensorData)

for y in ds18b20:
 print "DS18B20: ID:",y[0]," T:", y[1],"\xc2\xb0C"
 
for d in dht:
 print "DHT22: T: ",d[0], "\xc2\xb0C"
 print "DHT22: Humidity:",d[1],"%RH"
 

movement = int(re.search("<mv>(.*)</mv>",sensorData).group(1))
moveSenseEnabled = int(re.search("<mvSen>(.*)</mvSen>",sensorData).group(1))
lights = False
lightSwitch = True
time=  int(re.search("<time>(.*)</time>",sensorData).group(1))
# time: time in s since movement
print "Photoresistor:",pr
print "Time:",time

if pr<prThresh and movement is 1 and moveSenseEnabled is 1 and lightSwitch is True:
 subprocess.call("./Lights.sh c 0 on > /dev/null",shell=True)
else:
 if pr>prThresh or time>300 or lightSwitch is False:
  subprocess.call("./Lights.sh c 0 off > /dev/null",shell=True)
 else:
  subprocess.call("./Lights.sh c 0 on > /dev/null",shell=True)

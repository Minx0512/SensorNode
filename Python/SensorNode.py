#!/usr/bin/python3


#import os
#import sys
#import thread
#import time


import sensors

# 1. get available sensors per Node


nodeAddresses = ["A0:A0:A0:A0:A0"]
#threadLock = thread.allocate_lock()
port = "/dev/ttyAMA0"
baudrate = 9600



sens = sensors.Sensors(port, baudrate)
sens.SetNodeAddress(nodeAddresses[0])
sens.Update()
sens.InterpretResponse()
print (sens)

ds = sensors.DS18B20(port, baudrate)
ds.SetNodeAddress(nodeAddresses[0])
ds.Update()
ds.InterpretResponse()
print (ds)

mv = sensors.Movement(port,baudrate)
mv.SetNodeAddress(nodeAddresses[0])
mv.Update()
mv.InterpretResponse()
print (mv)


# 
# for addr in nodeAddresses:
#  sensor.append(rs.spawnSensors(addr))
#  
# 
# for s in sensor:
#  i = 0
#  print ("Sensor objects on {1}: {0}".format(s,nodeAddresses[i]))
#  i = i+1
# 






#try:   
# thread.start_new_thread(qDS18B20,(5*60)) # every 5 mins
# thread.start_new_thread(qMovement, (1)) # every second
# thread.start_new_thread(qDHT22,(5))  # every 5 mins
#ReadSensorData(sock)
#except Exception, e:
# print "{0}".format(e)     
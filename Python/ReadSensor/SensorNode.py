#!/usr/bin/python3


#import os
#import sys
#import thread
#import time
import ReadSensor
import Sensors.Sensors as Sensors

# 1. get available sensors per Node



#threadLock = thread.allocate_lock()


rs = ReadSensor.ReadSensor("/dev/ttyAMA0", 9600)

availableSensors = rs.GetAvailableSensors("A0:A0:A0:A0:A0")
print ("Sensors: {0}".format(availableSensors.sens))


ds =  rs.DS18B20("A0:A0:A0:A0:A0")
print ("DS18B20: {0}{1}".format(ds.GetAvgTemperature(), ds.unitT ))
#for idx in range(0,ds.numEl):
 #print (ds.GetValuePair(idx))


mv = rs.Movement("A0:A0:A0:A0:A0")
print ("Mv: {0}".format(mv.move))

light = rs.LightAnalog("A0:A0:A0:A0:A0")
print ("Light: {0}".format(light.value))






#try:   
# thread.start_new_thread(qDS18B20,(5*60)) # every 5 mins
# thread.start_new_thread(qMovement, (1)) # every second
# thread.start_new_thread(qDHT22,(5))  # every 5 mins
#ReadSensorData(sock)
#except Exception, e:
# print "{0}".format(e)     
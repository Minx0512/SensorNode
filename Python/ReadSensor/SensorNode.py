#!/usr/bin/python
#import os
#import sys
#import thread
#import time
import ReadSensor



#threadLock = thread.allocate_lock()





rs = ReadSensor.ReadSensor("/dev/ttyAMA0", 9600)

#rs.GetAvailableSensors("A0:A0:A0:A0:A0")

print rs.DS18B20("A0:A0:A0:A0:A0")







#try:   
# thread.start_new_thread(qDS18B20,(5*60)) # every 5 mins
# thread.start_new_thread(qMovement, (1)) # every second
# thread.start_new_thread(qDHT22,(5))  # every 5 mins
#ReadSensorData(sock)
#except Exception, e:
# print "{0}".format(e)     
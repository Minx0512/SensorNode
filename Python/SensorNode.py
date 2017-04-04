#!/usr/bin/python3


#import os
#import sys
#import thread
#import time

import sys


import sensors

class SensorNode:
 """SensorNode class:
 
     main code to execute
 
 """   
  
 nodeAddresses = ["A0:A0:A0:A0:A0"]
#threadLock = thread.allocate_lock()
 port = "/dev/ttyAMA0"
 baudrate = 9600



 sens = sensors.Sensors(port, baudrate)
 
 for nodeAddr in nodeAddresses:
  #print(nodeAddr)   
  sens.SetNodeAddress(nodeAddr)
  sens.Update()
  sens.InterpretResponse()
  print(sens)
  sens.spawnSensors()

 
 for sob in sens.sensorObjList:
  sob.Update()
  sob.InterpretResponse()
  print (sob)



if __name__ == "__main__":
    app = SensorNode()
    





#try:   
# thread.start_new_thread(qDS18B20,(5*60)) # every 5 mins
# thread.start_new_thread(qMovement, (1)) # every second
# thread.start_new_thread(qDHT22,(5))  # every 5 mins
#ReadSensorData(sock)
#except Exception, e:
# print "{0}".format(e)     
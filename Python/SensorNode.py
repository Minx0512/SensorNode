#!/usr/bin/python3


import os
import sys

import _thread
import time



import sensors

class SensorNode:
 """SensorNode class:
 
     main code to execute
 
 """
  
 threadLock = _thread.allocate_lock()
 nodeAddresses = ["A0:A0:A0:A0:A0"]
 port = "/dev/ttyAMA0"
 baudrate = 9600
    
 def UpdateThreads(self,sensorObj):
  """  """    
  
  while 1:
   threadLock.acquire()
   
   sensorObj.Update()
   threadLock.release()
   sensorObj.InterpretResponse()
       
   if sensorObj.err is not 0:    
    sleep(5)
   else:
    # \todo :  write function for transmit value to openhab REST API 
    print(sensorObj)  
    sleep(sensorObj.updateTime)   
         

  




 sens = sensors.Sensors(port, baudrate)
 
 for nodeAddr in nodeAddresses:
  #print(nodeAddr)   
  sens.SetNodeAddress(nodeAddr)
  sens.Update()
  sens.InterpretResponse()
  print(sens)
  sens.spawnSensors()

 
 for sob in sens.sensorObjList:
  _thread.start_new_thread(UpdateThreads,(sensorObj))   
  #sob.Update()
  #sob.InterpretResponse()
  #print (sob)



if __name__ == "__main__":
    app = SensorNode()
    





#try:   
# thread.start_new_thread(qDS18B20,(5*60)) # every 5 mins
# thread.start_new_thread(qMovement, (1)) # every second
# thread.start_new_thread(qDHT22,(5))  # every 5 mins
#ReadSensorData(sock)
#except Exception, e:
# print "{0}".format(e)     
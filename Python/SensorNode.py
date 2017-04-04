#!/usr/bin/python3


import os
import sys
import threading
import time
import sensors


class Thread(threading.Thread):
    def __init__(self, t, *args):
        threading.Thread.__init__(self, target=t, args=args)
        self.start()

count = 0
lock = threading.Lock()
thr = []
   
def UpdateThreads(sensorObj):
  """  """    
  
  while 1:
      
   with lock:
    sensorObj.Update()
   
   sensorObj.InterpretResponse()
       
   if sensorObj.err is not 0:    
    time.sleep(5)
   else:
    # \todo :  write function for transmit value to openhab REST API 
    print(sensorObj)  
    time.sleep(sensorObj.updateTime)   
    
 
    
class SensorNode:
 """SensorNode class:
 
     main code to execute
 
 """
  
 
 nodeAddresses = ["A0:A0:A0:A0:A0"]
 port = "/dev/ttyAMA0"
 baudrate = 9600
 
 def run(self):
  while 1:
    try:
     pass
    except(KeyboardInterrupt, EOFError):
     pass
 

 sens = sensors.Sensors(port, baudrate)
 
 for nodeAddr in nodeAddresses:
  #print(nodeAddr)   
  sens.SetNodeAddress(nodeAddr)
  sens.Update()
  sens.InterpretResponse()
  print(sens)
  sens.spawnSensors()

 
 
 
 for sob in sens.sensorObjList:
  thr.append(Thread(UpdateThreads,(sob))) 
  
    
  #sob.Update()
  #sob.InterpretResponse()
  #print (sob)



if __name__ == "__main__":
    app = SensorNode()
    app.run()
    


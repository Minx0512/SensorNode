#!/usr/bin/python3


import os
import sys
import threading
import time
import sensors
import termios


class Thread(threading.Thread):
    def __init__(self, t, *args):
        threading.Thread.__init__(self, target=t, args=args)
        self.start()
        self.join()

count = 0
lock = threading.Lock()
thr = []
stp = 0
   
def UpdateThreads(sensorObj):
  """  """    
  
  while 1:
   
    with lock:
     sensorObj.Update()
   
    sensorObj.InterpretResponse()
       
    if sensorObj.err is not 0:
     for i in range(0,5):
      time.sleep(1)
      if stp == 1:
       break
      
    else:
     # \todo :  write function for transmit value to openhab REST API 
     print(sensorObj)
     for i in range(0,sensorObj.updateTime):
      time.sleep(1)
      if stp==1:
       break       
    
 
    
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
     stp = 1          
     sys.exit()
 

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
    


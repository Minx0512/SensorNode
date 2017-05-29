#!/usr/bin/python3


import os
import sys
import threading
import time
import sensors
import termios




def GetTimeString():
  return time.strftime("%B %d, %Y @ %I:%M:%S %p  ",time.localtime())
   

class Thread(threading.Thread):
    def __init__(self, t, *args):
        threading.Thread.__init__(self, target=t, args=args)
        self.start()
        #self.join()

count = 0
lock = threading.Lock()
thr = []


   
def UpdateThreads(sensorObj):
  """  """    
  
  while sensorObj.signal:
   
    with lock:
     sensorObj.Update()
   
    sensorObj.InterpretResponse()
       
    if sensorObj.err is not 0:
     for i in range(0,5):
      time.sleep(1)
      if sensorObj.signal == 0:
       break
      
    else:
      
     print ("{0} : {1}".format(GetTimeString(),sensorObj))
     # \todo :  write function for transmit value to openhab REST API
     
     
     for i in range(0,sensorObj.updateTime):
      time.sleep(1)
      if sensorObj.signal==0:
       break       
    
 
    
class SensorNode:
 """SensorNode class:
 
     main code to execute
 
 """
  
 
 nodeAddresses = ["A0:A0:A0:A0:A0"]
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
  #print(sob)   
  thr.append(Thread(UpdateThreads,(sob))) 
  
    
 while 1:
    
    try:
       pass 
        
    except (KeyboardInterrupt):
     for t in thr:
      t.join()  
     for s in sens.sensorObjList:
      s.signal = 0            
     sleep(1)
     sys.exit() 


if __name__ == "__main__":
    
    app = SensorNode()
    
   
    


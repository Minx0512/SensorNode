#!/usr/bin/python3


import re
import readsensor


class Sensors(readsensor.ReadSensor):
    def __init__(self, prt,bdrate):
      """Initialize Sensors Class """  
      readsensor.ReadSensor.__init__(self,prt,bdrate)  
      self.sensorList = []
      self.sensorObjList = []      
      #print(responsrStr)  
      self.nodeMAC = ""
      self.cmdID = "20"
      self.pString = "Sensors"            
      self.respMask = ".\/\/system\/{0}/\|(.*)\|/\|(.*)\|"
    
    def InterpretResponse(self):
     """Interpret response list """
     self.sensorList =[hex(int(i,0))[2:] for i in self.interpResp[1].rsplit("|")]
       #print(self.sens)
    def __str__(self):
     """Format string for print """   
     return "{0} | {1}".format(self.nodeMAC, self.sensorList)
 
    def spawnSensors(self):
     """ Spawn sensor objects for a given node address """
     #print(self)
     for sensor in self.sensorList:
      #print(sensor)
      if sensor == "31":
          
       mv =  Movement(self.port,self.baudrate)       
       mv.SetNodeAddress(self.nodeMAC)
       mv.updateTime = 1
       #mv.Update()
       #mv.InterpretResponse()
       self.sensorObjList.append(mv)
       #print(mv)
       
      elif sensor =="32":
          
       dht =  DHT22(self.port,self.baudrate)       
       dht.SetNodeAddress(self.nodeMAC)
       dht.updateTime = 300
       #dht.Update()
       #dht.InterpretResponse()
       self.sensorObjList.append(dht) 
       #print(dht)  
        
      elif sensor =="33":
          
       la =  LightAnalog(self.port,self.baudrate)       
       la.SetNodeAddress(self.nodeMAC) 
       la.updateTime = 1
       #la.Update()
       #la.InterpretResponse()
       self.sensorObjList.append(la) 
       #print(la)
       
      elif sensor =="34":
          
       ld =  LightDigital(self.port,self.baudrate)       
       ld.SetNodeAddress(self.nodeMAC) 
       ld.updateTime = 2
       #ld.Update()
       #ld.InterpretResponse()
       self.sensorObjList.append(ld) 
       #print(ld)
            
      elif sensor =="35":
          
       ds = DS18B20(self.port,self.baudrate)
       ds.SetNodeAddress(self.nodeMAC)
       ds.updateTime = 300
       #ds.Update()
       #ds.InterpretResponse() 
       self.sensorObjList.append(ds) 
       #print(ds)
       
      elif sensor =="36":
          
       bmp = BMP180(self.port,self.baudrate)
       bmp.SetNodeAddress(self.nodeMAC)
       bmp.updateTime = 120
       #bmp.Update()
       #bmp.InterpretResponse() 
       self.sensorObjList.append(bmp) 
       #print(bmp)

 
class DS18B20(readsensor.ReadSensor):
    def __init__(self,prt,bdrate):
     readsensor.ReadSensor.__init__(self,prt,bdrate)  
     self.MAC = []
     self.T = []     
     self.unitT = "°C".encode('utf-8').decode('unicode-escape')
     self.nodeMAC = ""
     self.cmdID = "35"
     self.pString = "DST"            
     self.respMask = ".\/\/sensor\/{0}/\|(.*)\|/\|(.*)\|" 
     self.numEl = 0
      
    def InterpretResponse(self):
     """Interpret response list """        
     self.MAC = self.interpResp[0].rsplit("|")     
     self.T = [float(i) for i in self.interpResp[1].replace(u'\xc2\xb0C','').rsplit("|")]
     self.numEl = len(self.T)
     
           
    def GetValuePair(self,idx):
     return [self.MAC[idx], self.T[idx],self.unitT]
     
    def GetAvgTemperature(self):
     avrgTemp = 0.0  
     for ti in range(0,self.numEl):
      avrgTemp= avrgTemp+self.T[ti]
     if self.numEl>0: 
      return round(avrgTemp/self.numEl,3)     
     else: 
      return null
      
      
    def __str__(self):
        return "{0} | T: {1}{2}".format(self.nodeMAC, self.GetAvgTemperature(),self.unitT)     
      
class DHT22(readsensor.ReadSensor):
    def __init__(self,prt,bdrate):
     readsensor.ReadSensor.__init__(self,prt,bdrate)    
     self.H = 0.0
     self.unitH = ""
     self.T = 0.0
     self.unitT = ""
     self.err = 0
     self.cmdID = "32"
     self.pString = "DHT22"            
     self.respMask = ".\/\/sensor\/{0}\/\|(.*)\|\/\|(.*):(.*)\|(.*):(.*)\|(.*)\|" 
     
    def InterpretResponse(self):
     """Interpret response list """ 
     self.interpResp 
     self.H = float(self.interpResp[3])
     self.unitH = self.interpResp[4]
     self.T = float(self.interpResp[1])
     self.unitT = self.interpResp[2]
     self.err = int(self.interpResp[5])
  
    def __str__(self):
        return "{0} | T: {1}{2} | H: {3}{4} | err: {5}".format(self.nodeMAC, self.T,self.unitT,self.H,self.unitH,self.err)  
 
class Movement(readsensor.ReadSensor):
    def __init__(self,prt,bdrate):
     readsensor.ReadSensor.__init__(self,prt,bdrate)
     self.move = 0
     self.nodeMAC = ""
     self.cmdID = "31"
     self.pString = "M"            
     self.respMask = ".\/\/sensor\/{0}\/\|(.*)\|\/\|(.*)\|"
     
    def InterpretResponse(self):
     """Interpret response list """ 
     self.move = int(self.interpResp[1])
     
    def __str__(self):
        return "{0} | M: {1}".format(self.nodeMAC, self.move) 
    
      
  
class LightAnalog(readsensor.ReadSensor):
    def __init__(self,prt,bdrate):
     readsensor.ReadSensor.__init__(self,prt,bdrate)
     self.value = 0.0
     self.nodeMAC = ""
     self.cmdID = "33"
     self.pString = "Lav"            
     self.respMask = ".\/\/sensor\/{0}\/\|(.*)\|\/\|(.*)\|"
     
    def InterpretResponse(self):
     """Interpret response list """ 
     self.value = float(self.interpResp[1])   
    
    def __str__(self):
     return "{0} | Light(analog): {1}".format(self.nodeMAC, self.value)  
 
      
class LightDigital(readsensor.ReadSensor):
    def __init__(self,prt,bdrate):
     readsensor.ReadSensor.__init__(self,prt,bdrate)
     self.value = 0.0
     self.nodeMAC = ""
     self.cmdID = "34"
     self.pString = "Ld"            
     self.respMask = ".\/\/sensor\/{0}\/\|(.*)\|\/\|(.*)\|"
     
    def InterpretResponse(self):
     """Interpret response list """   
     self.value = float(self.interpResp[1])
              
    def __str__(self):
     return "{0} | Light(I2C): {1}".format(self.nodeMAC, self.value) 

         
class BMP180(readsensor.ReadSensor):
    def __init__(self,prt,bdrate):
     readsensor.ReadSensor.__init__(self,prt,bdrate)
     self.P = 0.0
     self.unitP = ""
     self.T = 0.0
     self.unitT = ""
     self.err = 0  
     self.nodeMAC = ""
     self.cmdID = "36"
     self.pString = "PT"            
     self.respMask = ".\/\/sensor\/{0}\/\|(.*)\|\/\|(.*):(.*)\|(.*):(.*)\|(.*)\|"
     
                 
    def InterpretResponse(self):
     """Interpret response list """
     self.T = float(self.interpResp[3])
     self.unitT = self.interpResp[4]
     self.P = float(self.interpResp[1])
     self.unitP = self.interpResp[2]
     self.err = int(self.interpResp[5])
               
    def __str__(self):
     return "{0} | T: {1}{2} | P: {3}{4} | err: {5}".format(self.nodeMAC, self.T,self.unitT,self.P,self.unitP,self.err)  
        
             
     
 
     
     
     
     
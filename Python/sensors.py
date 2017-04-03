#!/usr/bin/python3


import re
import readsensor


class Sensors(readsensor.ReadSensor):
    def __init__(self, prt,bdrate):
      readsensor.ReadSensor.__init__(self,prt,bdrate)  
      self.sens = []      
      #print(responsrStr)  
      self.nodeMAC = ""
      self.cmdID = "20"
      self.pString = "Sensors"            
      self.respMask = ".\/\/system\/{0}/\|(.*)\|/\|(.*)\|"
    
    def InterpretResponse(self):
     """Interpret response list """
     self.sens =[hex(int(i,0))[2:] for i in self.interpResp[1].rsplit("|")]
       #print(self.sens)
    def __str__(self):
     return "{0} | {1}".format(self.nodeMAC, self.sens)
 
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
     self.interpResp     
     self.MAC = self.interpResp[0].rsplit("|")
      #print (self.ds[1].replace(u'\xc2\xb0C','').rsplit("|"))
     self.T = [float(i) for i in self.interpResp[1].replace(u'\xc2\xb0C','').rsplit("|")]
     self.numEl = len(self.T)
     
           
    def GetValuePair(self,idx):
     return [self.MAC[idx], self.T[idx],self.unitT]
     
    def GetAvgTemperature(self):
     avrgTemp = 0.0  
     for ti in range(0,self.numEl):
      avrgTemp= avrgTemp+self.T[ti]
     return round(avrgTemp/self.numEl,3)     
    
    def __str__(self):
        return "{0} | T: {1}{2}".format(self.nodeMAC, self.GetAvgTemperature(),self.unitT)     
#      
# class DHT22:
#     def __init__(self,responseStr,sensStr):
#      self.H = 0.0
#      self.unitH = ""
#      self.T = 0.0
#      self.unitT = ""
#      self.err = 0
# 
#      self.dht = re.findall(".\/\/sensor\/{0}\/\|(.*)\|\/\|(.*):(.*)\|(.*):(.*)\|(.*)\|".format(sensStr),responseStr)
#      if len(self.dht) > 0:
#       self.dht = self.dht[0]   
#       self.H = float(self.dht[3])
#       self.unitH = self.dht[4]
#       self.T = float(self.dht[1])
#       self.unitT = self.dht[2]
#       self.err = int(self.dht[5])
#      
# 
# class Movement:
#     def __init__(self,responseStr,sensStr):
#      self.move = 0     
#      self.mv = re.findall(".\/\/sensor\/{0}\/\|(.*)\|\/\|(.*)\|".format(sensStr),responseStr)
#      if len(self.mv) > 0:
#       self.mv = self.mv[0]
#       self.move = int(self.mv[1])
#  
# class LightAnalog:
#     def __init__(self,responseStr,sensStr):
#      self.value = 0.0    
#      self.la = re.findall(".\/\/sensor\/{0}\/\|(.*)\|\/\|(.*)\|".format(sensStr),responseStr)
#      if len(self.la) > 0:
#       self.la = self.la[0]
#       self.value = float(self.la[1])   
#         
# class LightDigital:
#     def __init__(self,responseStr,sensStr):
#      self.value = 0.0
#      self.ld = re.findall(".\/\/sensor\/{0}\/\|(.*)\|\/\|(.*)\|".format(sensStr),responseStr)
#      if len(self.ld) > 0:
#       self.ld = self.ld[0]
#       self.value = float(self.ld[1])         
#         
# class BMP180:
#     def __init__(self,responseStr,sensStr):
#      self.P = 0.0
#      self.unitP = ""
#      self.T = 0.0
#      self.unitT = ""
#      self.err = 0  
#                 
#      self.bmp = re.findall(".\/\/sensor\/{0}\/\|(.*)\|\/\|(.*):(.*)\|(.*):(.*)\|(.*)\|".format(sensStr),responseStr)
#      if len(self.bmp) > 0:
#       self.bmp = self.bmp[0]   
#       self.T = float(self.bmp[3])
#       self.unitT = self.bmp[4]
#       self.P = float(self.bmp[1])
#       self.unitP = self.bmp[2]
#       self.err = int(self.bmp[5])            
#         
#         
             
     
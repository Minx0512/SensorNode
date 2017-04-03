#!/usr/bin/python3

import re
import serialconnection
#import Sensors.Sensors as Sensors



class ReadSensor:
    """provides functions for sensor reading"""
    def __init__(self, prt,bdrate):
     self.port = prt
     self.baudrate = bdrate   
     self.nodeMAC = ""
     self.cmdID = 0
     self.pString = ""
     self.queryStr = "{0}|{1}|{2}"
     self.serialCon = serialconnection.SerialConnection(self.port,self.baudrate)
     self.resp = ""
     self.respMask = ""
     self.interpResp = []
     self.availableSensors = []
     self.cmdIDs = ["20","31","32","33","34","35","36"]
     self.__InterpretResponse()
    
    
    def InterpretResponse(self):
     """Interpret response list """
     
     
    __InterpretResponse = InterpretResponse
    
    def Update(self):
     """Make new query """ 
     q = self.queryStr.format(self.cmdID,self.nodeMAC,self.pString)
     self.resp = self.serialCon.ReadData(q)
     self.interpResp = re.findall(self.respMask.format(self.pString),self.resp)   
     if len(self.interpResp) > 0:
      self.interpResp = self.interpResp[0] 
     
    def SetNodeAddress(self, ndAddr):
     self.nodeMAC = ndAddr
         
    
#     def GetAvailableSensors(self,MAC):
#      """Get available sensors on a certain node"""   
#      q = self.queryStr.format(self.cmdIDs[0],MAC,"Sensors")
#      self.resp = self.serialCon.ReadData(q)
#      sens = Sensors.Sensors(self.resp,"Sensors")
#      return sens    
#        
    
#     def DS18B20(self,MAC):
#      """Reads all DS18B20 sensors on one node with a certain MAC address"""
#      q = self.queryStr.format(self.cmdIDs[5],MAC,"DST")
#      self.resp = self.serialCon.ReadData(q)
#      ds = Sensors.DS18B20(self.resp,"DST")
#      #vals = ds.GetValuePair(0)
#      return ds
# 
#     def DHT22(self,MAC):
#      """Reads the DHT22 sensor on a node with a certain MAC address"""   
#      q = self.queryStr.format(self.cmdIDs[2],MAC,"DHT22")
#      self.resp = self.serialCon.ReadData(q)
#      dht = Sensors.DHT22(self.resp,"DHT22")
#      return dht
#     
#     def Movement(self,MAC):
#      """Reads the movement detector of a certain node """   
#      q = self.queryStr.format(self.cmdIDs[1],MAC,"M")
#      self.resp = self.serialCon.ReadData(q)
#      mv = Sensors.Movement(self.resp,"M")
#      return mv 
#     
#     def LightAnalog(self,MAC):
#      """ Reads the analog photoresistor on a certain node """   
#      q = self.queryStr.format(self.cmdIDs[3],MAC,"Lav")
#      self.resp = self.serialCon.ReadData(q)
#      la = Sensors.LightAnalog(self.resp,"Lav")
#      return la 
#     
#     def LightDigital(self,MAC):
#      """ Reads the digital light sensor on a certain node """   
#      q = self.queryStr.format(self.cmdIDs[4],MAC,"Ld")
#      self.resp = self.serialCon.ReadData(q)
#      ld = Sensors.LightAnalog(self.resp,"Ld")
#      return ld
#  
#  
#     def BMP180(self,MAC):
#      """ Reads the BMP180 pressure sensor on a certain node"""
#      q = self.queryStr.format(self.cmdIDs[6],MAC,"PT")
#      self.resp = self.serialCon.ReadData(q)
#      bmp = Sensors.BMP180(self.resp,"PT")
#      return bmp
#  
#     def spawnSensors(self, nodeAdress):
#      print ("#######################\r\nAddress: {0}\r\n#######################".format(nodeAdress))   
#      sensrs = []
#      availableSensors = self.GetAvailableSensors(nodeAdress).sens
#      print ("# Sensors: {0}".format(availableSensors))  
#      for s in availableSensors:
#       if s == self.cmdIDs[1]:
#        mv = self.Movement(nodeAdress)
#        sensrs.append(mv)
#        print ("# Mv: {0}".format(mv.move))
#    
#       elif  s == self.cmdIDs[3]:
#        light = self.LightAnalog(nodeAdress)
#        print ("# Light: {0}".format(light.value))
#        sensrs.append(light)
#       elif s == self.cmdIDs[5]:
#        ds = self.DS18B20(nodeAdress)   
#        sensrs.append(ds)
#        print ("# DS18B20: {0}{1}".format(ds.GetAvgTemperature(), ds.unitT ))
# 
#      return sensrs
#      
#      
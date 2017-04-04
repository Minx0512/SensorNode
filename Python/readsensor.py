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
   
    def __str__(self):
        return "{0}".format(self.nodeMAC)
     


#!/usr/bin/python

import re

class Sensors:
    def __init__(self,responsrStr):
      self.sens = []  
      self.s = re.findall(".\/\/system\/Sensors/\|(.*)\|/\|(.*)\|",responsrStr)[0]  
      self.sens = self.s[1].rsplit("|")


class DS18B20:
    def __init__(self,responseStr,sensStr):
     self.MAC = []
     self.T = []     
     
     self.ds = re.findall(".\/\/sensor\/{0}/\|(.*)\|/\|(.*)\|".format(sensStr),responseStr)
     if len(self.ds) > 0:
      self.ds = self.ds[0]
      self.MAC = self.ds[0].rsplit("|")
      self.T = self.ds[1].rsplit("|")
      self.numEl = len(self.T)
         
    def GetValuePair(self,idx):
     return [self.MAC[idx], self.T[idx]]
     
class DHT22:
    def __init__(self,responseStr):
     self.H = 0.0
     self.unitH = ""
     self.T = 0.0
     self.unitT = ""
     self.err = 0


     self.dht = re.findall(".\/\/sensor\/DHT22\/\|(.*)\|\/\|(.*):(.*)\|(.*):(.*)\|(.*)\|",responseStr)
     if len(self.dht) > 0:
      self.dht = self.dht[0]   
      self.H = float(self.dht[3])
      self.unitH = self.dht[4]
      self.T = float(self.dht[1])
      self.unitT = self.dht[2]
      self.err = int(self.dht[5])
     

class Movment:
    def __init__(self,responseStr):
     self.move = 0     
     self.mv = re.findall(".\/\/sensor\/M\/\|(.*)\|\/\|(.*)\|",responseStr)
     if len(self.mv) > 0:
      self.mv = self.mv[0]
      self.move = int(self.mv[1])
     
     
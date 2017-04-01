#!/usr/bin/python

import re

class Sensors:
    def __init__(self,responsrStr):
      self.sens = []  
      self.s = re.findall(".\/\/system\/Sensors/\|(.*)\|/\|(.*)\|",responsrStr)[0]  
      self.sens = self.s[1].rsplit("|")


class DS18B20:
    def __init__(self,responsrStr):
     self.MAC = []
     self.T = []     
     
     self.ds = re.findall(".\/\/sensor\/DST/\|(.*)\|/\|(.*)\|",responsrStr)[0]
     self.MAC = self.ds[0].rsplit("|")
     self.T = self.ds[1].rsplit("|")


    def GetValuePair(self,idx):
     return [self.MAC[idx], self.T[idx]]
     

  
     
#!/usr/bin/python


import SerialConnection

class ReadSensor:
    """provides functions for sensor reading"""
    def __init__(self, port,baudrate):
     self.sensorMAC = ""
     self.cmdID = 0
     self.propertyString = ""
     self.queryStr = "{0}|{1}|{2}"
     self.serialCon = SerialConnection.SerialConnection(port,baudrate)
    
    def DS18B20(self,MAC):
     q = self.queryStr.format(35,MAC,"DST")
     return self.serialCon.ReadData(q)
 
     
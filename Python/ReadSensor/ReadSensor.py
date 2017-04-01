#!/usr/bin/python
# -*- coding: utf-8 -*-

import SerialConnection
import Sensors.Sensors as Sensors


class ReadSensor:
    """provides functions for sensor reading"""
    def __init__(self, port,baudrate):
     self.sensorMAC = ""
     self.cmdID = 0
     self.propertyString = ""
     self.queryStr = "{0}|{1}|{2}"
     self.serialCon = SerialConnection.SerialConnection(port,baudrate)
     self.resp = ""
     self.availableSensors = []
    
    def GetAvailableSensors(self,MAC):
     """Get available sensors on a certain node"""   
     q = self.queryStr.format(20,MAC,"Sensors")
     self.resp = self.serialCon.ReadData(q)
    
       
    
    def DS18B20(self,MAC):
     """Reads all DS18B20 sensors on one node with a certain MAC address"""
     q = self.queryStr.format(35,MAC,"DST")
     self.resp = self.serialCon.ReadData(q)
     ds = Sensors.DS18B20(self.resp,"DST")
     #vals = ds.GetValuePair(0)
     return ds

    def DHT22(self,MAC):
     """Reads the DHT22 sensor on a node with a certain MAC address"""   
     q = self.queryStr.format(32,MAC,"DHT22")
     self.resp = self.serialCon.ReadData(q)
     dht = Sensors.DHT22(self.resp,"DHT22")
     return dht
    
    def Movement(self,MAC):
     """Reads the movement detector of a certain node """   
     q = self.queryStr.format(31,MAC,"M")
     self.resp = self.serialCon.ReadData(q)
     mv = Sensors.Movment(self.resp,"M")
     return mv 
    
    def LightAnalog(self,MAC):
     """ Reads the analog photoresistor on a certain node """   
     q = self.queryStr.format(33,MAC,"Lav")
     self.resp = self.serialCon.ReadData(q) 
    
    def LightDigital(self,MAC):
     """ Reads the digital light sensor on a certain node """   
     q = self.queryStr.format(34,MAC,"Ld")
     self.resp = self.serialCon.ReadData(q)
    
    def BMP180(self,MAC):
     """ Reads the BMP180 pressure sensor on a certain node"""
     q = self.queryStr.format(36,MAC,"PT")
     self.resp = self.serialCon.ReadData(q)
     
     
     
     
     
     
     
     
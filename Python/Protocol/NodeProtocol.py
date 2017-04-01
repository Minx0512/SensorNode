#!/usr/bin/python

import sys
from .. import readSerial
# Get Sensor data
def DHT22():
    return "{0}".format(32)

def DS18B20():
    return "{0}".format(35)


def ReadSensorDS18B20(tt,MAC):
 queryStr = "{0}|{1}|DST".format(35,MAC)
 resp = ''
 resp = ReadData(tt,queryStr)
 return resp
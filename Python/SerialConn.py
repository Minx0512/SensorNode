#!/usr/bin/python
import os
import sys
import re
import thread
import serial, time
from xml.dom.minidom import parse, parseString
from pyasn1.compat.octets import null

from ReadSensor import *
#from Protocol.NodeProtocol import *
from readSerial import *






ser = NewSerialCon("/dev/ttyAMA0", 9600)



resp = ReadSensorDS18B20(ser,"A0:A0:A0:A0:A0")

print("{0}".format(resp))
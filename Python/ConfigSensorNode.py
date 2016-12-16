#!/usr/bin/python

import sys
import re
from BluetoothConn import *
from ReadSensor import *
from xml.dom.minidom import parse, parseString


bt_addr = "98:D3:31:FB:31:A6"

def ConfigMACnRF(sock,pipeNum,MAC):
    # Configure nRF pipe: 20
    # # Pipe: 0
    # => 0
 sendStr = "20|"
 sendStr+=pipeNum
 sendStr+= "|"
 sendStr+=MAC
 sendStr+="\n"
 #print sendStr
 out_str = sendStr
 #SendData(sock,send_str)
 # out_str = ReceiveData(sock,"|end")
 return out_str

def ConfigBluetoothName(sock,bt_name):
    # Configure: 0
    # BT: 1
    # Name: 0
    # => 0+1+0 = 1
 sendStr = "10|0|"
 sendStr+=bt_name
 sendStr+="\n"
  
 #print sendStr
 out_str = sendStr
 #SendData(sock,send_str)
 # out_str = ReceiveData(sock,"|end")
 return out_str


port = 1 
conn=0
out_str = ''
while conn==0:
 try:
  sock = BlueConnect(bt_addr,port,5.0)
  conn=1
   
  print ConfigBluetoothName(sock,"MinxSensorNode")
 
  print ConfigMACnRF(sock,"0","e0:f0:f0:f0:f0") # TX_ADDR and RX_ADDR0 at SensorNodeBase
  print ConfigMACnRF(sock,"1","e1:f0:f0:f0:C0") # SensorNode01
  print ConfigMACnRF(sock,"2","e1:f0:f0:f0:C1") # SensorNode02
  print ConfigMACnRF(sock,"3","e1:f0:f0:f0:C2")
  print ConfigMACnRF(sock,"4","e1:f0:f0:f0:C3")
  print ConfigMACnRF(sock,"5","e1:f0:f0:f0:C4")
  
   
  CloseConn(sock)
 except IOError as e:
  print "{0}".format(e)
 

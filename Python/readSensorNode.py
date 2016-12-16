#!/usr/bin/python

import sys
import re
from BluetoothConn import *
from ReadSensor import *
from xml.dom.minidom import parse, parseString
from pyasn1.compat.octets import null

send_str = 'Help\n'

scriptname = sys.argv[0]
val = sys.argv.pop()

if val is not scriptname:
    send_str = val
    send_str +="\n"
    print val



sensorData= ReadSensorData("98:D3:31:FB:31:A6",send_str)
print sensorData

if val == 'DHT22':
 dht = re.findall("<DHT22><T unit='(.*)'>(.*)</T><H unit='(.*)'>(.*)</H></DHT22>",sensorData)

 for d in dht:
  print "DHT22: T:",d[1],d[0]
  print "DHT22: H:",d[3],d[2]
  
elif val =='Light':
 try:
  l=re.findall("<NSL19M51>(.*)</NSL19M51>",sensorData)
  pr=int(l[0])
  print "L:",pr
 except Error as e:
  print "{0}".format(e)
elif val=='RS':
 rss = re.findall("<RS>(.*)</RS>", sensorData)
 
 for rs in rss:
  print "RS:",rs[0]

elif val=='Move':
 mvs = re.findall("<mv>(.*)</mv>", sensorData)
 m=int(mvs[0])
 print "mv:", m
   
    
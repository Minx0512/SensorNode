#!/usr/bin/python

import sys
from BluetoothConn import *
from time import *
from ReadSensor import *
from NodeProtocol import *


def isNightTime():
 t = int(strftime("%H",localtime()))
 isNight = 1
 if t < 15 or t > 8:
  isNight = 0
 return isNight
  
def SleepMode(slpmd):
 slpmd = 1


port = 1
conn=0
out_str = ''
bd_addr = '98:D3:31:FB:31:A6'
remoteSensorsEnabled = 0
sleepmode = 0

try:
 sock = BlueConnect(bd_addr,port,5.0)
 conn=1
except IOError as e:
 print "{0}".format(e)


# get temp and hum every hour
# get Lightsensors every min
# get RemoteSensors every min
prevmins = 0
thresh = 500
treshOut = 500
humDHT22= ''
tempDHT22=''

while True and conn==1:

 mins = strftime("%M", gmtime())
 light = ''
 remoteSensors = ''
 move = Move(GetMovement(sock))
 # print move 
 light = LightsInside(GetLightsense(sock))
 
 if mins != prevmins: 
  if mins==0:
   # get temp and humidity   
   tempDHT22 = '' # GetTempDHT22(sock)
 #  humDHT22 = GetHumidityDHT22(sock)
 #  remoteSensors = GetRemoteSensors(socks)   
  elif mins%1 == 0 and mins > 0:   
   remoteSensors = GetRemoteSensors(socks)
  prevmins = mins
  
  if move == 1:
   if remoteSensorsEnabled ==1:
    if RemoteLight(remoteSensors,0) < treshOut:
     # Lights on
     LightsOn()
    elif light< thresh:
     LightsOn()
    else:
     #Lights off
     LightsOff()
   elif remoteSensorsEnabled == 0:
    if light < thresh and (isNightTime()==1 or sleepmode==0):
     LightsOn()
    else:
     LightsOff()
    

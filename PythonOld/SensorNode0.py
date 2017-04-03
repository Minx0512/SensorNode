#!/usr/bin/python

import sys
import re
import thread
# import MySQLdb as mdb
from BluetoothConn import *
import time
from ReadSensor import *
from NodeProtocol import *
from xml.dom.minidom import parse, parseString
from pyasn1.compat.octets import null
from bluetooth._bluetooth import btsocket
from time import sleep
from sys import getsizeof



def isNightTime(lightValue):
 t = int(time.strftime("%H",time.localtime()))
 #print "{0}".format(t)
 isNight = 1
 if t < 15 and t > 8:
  isNight = 0
 return isNight
  
def SleepMode(slpmd):
 slpmd = 1

def NoMovement(lastmv,lightVal,tIn,stateOlight):
 noww = int(time.time() )
 prevMovementMins = noww - lastmv
   
 if prevMovementMins >= 5*60 and stateOlight==1:
  print LightsOff(lightVal)
  stateOlight = 0
 #elif stateOlight ==0 and lightVal<tIn:   
 # LightsOn()
 # stateOlight = 1 
 #print "Light: {0} - stateOfLight: {1}".format(lightVal,stateOlight)
 return stateOlight

def OnMovement(lightVal,tIn,tOut,stateOlight,rsEneabled):
 if rsEneabled ==1:
  #if RemoteLight(remoteSensors,0) < tOut:
     # Lights on
     #LightsOn()
  #elif light< tInside:
     #LightsOn()
  #else:
     #Lights off
     print LightsOff(lightVal)
 else:
  if (lightVal < tIn  or isNightTime(lightVal)==1) and stateOlight==0: 
     print LightsOn(lightVal)     
     stateOlight = 1
  elif lightVal > tIn and isNightTime(lightVal)==0 and stateOlight ==1:
     print LightsOff(lightVal)
     stateOlight = 0 
 return stateOlight
    

def DetectMovement(socket,tInside, tOutside,rSensorsEnabled,slpmode):
 
 
 lastmovement = 0
 lightson = 0;
 natLightVal = 0
 light = 0
 while 1:
  threadLock.acquire()
  #SendData(socket,"35|0|Mv\n")  
  recData = GetMovement(socket)
  
  threadLock.release()
  mvs = re.findall("<mv>(.*)\|", recData)
  #print mvs
  if len(mvs) > 0:
   m=int(mvs[0])
  else:
   m = -1 
  
  #print "mv: {0}".format(m)
 
  if m == 1: # movement detected
   lastmovement = int(time.time())
   #print "lastmovement: {0} ".format(lastmovement)
   threadLock.acquire()
   #SendData(socket,"33|0|L\n")  
   sensorData = GetLightsense(socket)
   threadLock.release()
   l=re.findall("<NSL19M51>(.*)\|",sensorData)
   #print l
   if len(l) > 0:
    light=(l[0])
   else:
    light = -1 
   #print "Light: {0} ".format(light)
   #light = LightsInside(GetLightsense(socket))
   
   
   lightson = OnMovement(light,tInside,tOutside,lightson,rSensorsEnabled)
   
  else:  # no movement
   lightson = NoMovement(lastmovement,light,tInside,lightson)
   
  sleep(1)     

def ReadSensorData(socket,tnum):
 while 1:
 #print "In readSensorData: "
 #humDHT22 =  GetHumidityDHT22(socket) 
 #tempDHT22 = GetTempDHT22(socket)
 #lightI = GetLightsense(socket)
 #lightO = GetLightsense(socket)
 
  threadLock.acquire()
  recDataLight = GetLightsense(socket)
  #print recDataLight
  
  #SendData(socket,"32|0|DHT22\n")  
  recDataDHT22 = GetTempHumidDHT22(socket)
  
  threadLock.release()
  
  l=re.findall("<NSL19M51>(.*)\|",recDataLight)
  if len(l) > 0:
   light=(l[0])
  else:
   light = -1 
  
  t = time.strftime("%H:%M:%S %p",time.localtime())
  
  #print recDataDHT22
  dht = re.findall(".*<DHT22><T unit='(.*)'>(.*)\|<H unit='(.*)'>(.*)\|\|",recDataDHT22)

  for d in dht:  
   print "{0} : DHT22: T: {1}{2} | H: {3}{4} | Light: {5}".format(t, d[1],d[0],d[3],d[2],light)
  
  #print recDataDHT22
   
  
  #print "{0} : {1} ".format(t,recDataDHT22,light)
 #print "HumDHT22: {0} | TmpDHT22: {1} | Light: {2}\r\n".format(humDHT22,tempDHT22,lightI)  
  sleep(5*60)  
 #con = mdb.connect('localhost','SensorLog','sensNode059','sensors')
 #with con:
  #cur = con.cursor()
  #cur.execute("INSERT INTO sensorlog_all (SensorID,Value) VALUES (,%s),(,%s),(,%s),(,%s);",(humDHT22,tempDHT22,lightI,lightO))    




port = 1
conn=0
out_str = ''
bd_addr = '98:D3:31:FB:31:A6'
remoteSensorsEnabled = 0
sleepmode = 0

LightsOff(0)

while conn==0:
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


threadLock = thread.allocate_lock()


try:
 thread.start_new_thread(DetectMovement, (sock,thresh,treshOut,remoteSensorsEnabled,sleepmode))
 thread.start_new_thread(ReadSensorData,(sock,2))
 #ReadSensorData(sock)
except Exception, e:
 print "{0}".format(e)        
 

while 1:
 sleep(1)
 pass



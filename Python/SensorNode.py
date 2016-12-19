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



def isNightTime():
 t = int(time.strftime("%H",time.localtime()))
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
  LightsOff()
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
     LightsOff()
 else:
  if (lightVal < tIn  or isNightTime()==1) and stateOlight==0: 
     LightsOn()
     stateOlight = 1
  elif lightVal > tIn and isNightTime()==0 and stateOlight ==1:
     LightsOff()
     stateOlight = 0 
 return stateOlight
    

def DetectMovement(socket,tInside, tOutside,rSensorsEnabled,slpmode):
 
 
 lastmovement = 0
 lightson = 0;
 natLightVal = 0
 light = 0
 while 1:
  threadLock.acquire()
  SendData(socket,"Move\n")  
  recData = ReceiveData(socket,"|end")
  mvs = re.findall("<mv>(.*)</mv>", recData)
  m=int(mvs[0])
  threadLock.release()
  #print "mv: {0}".format(m)
 
  if m == 1: # movement detected
   lastmovement = int(time.time())
   #print "lastmovement: {0} ".format(lastmovement)
   threadLock.acquire()
   SendData(socket,"Light\n")  
   sensorData = ReceiveData(socket,"|end")
   l=re.findall("<NSL19M51>(.*)</NSL19M51>",sensorData)
   light=int(l[0])
   #print "Light: {0} ".format(light)
   #light = LightsInside(GetLightsense(socket))
   threadLock.release()
   
   lightson = OnMovement(light,tInside,tOutside,lightson,rSensorsEnabled)
   
  else:  # no movement
   lightson = NoMovement(lastmovement,light,tInside,lightson)
  sleep(5)     

def ReadSensorData(socket,tnum):
 while 1:
 #print "In readSensorData: "
 #humDHT22 =  GetHumidityDHT22(socket) 
 #tempDHT22 = GetTempDHT22(socket)
 #lightI = GetLightsense(socket)
 #lightO = GetLightsense(socket)
  threadLock.acquire()
  SendData(socket,"DHT22\n")  
  recDataDHT22 = ReceiveData(socket,"|end")
  SendData(socket,"Light\n")  
  recDataLight = ReceiveData(socket,"|end")
  l=re.findall("<NSL19M51>(.*)</NSL19M51>",recDataLight)
  light=int(l[0])
  threadLock.release()
  t = time.strftime("%H:%M %p",time.localtime())
  print "{0} : {1} | Light: {2}".format(t,recDataDHT22,light)
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
 pass



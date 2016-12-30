import sys
import subprocess
import bluetooth
from BluetoothConn import *
import time
from NodeProtocol import *
from xml.dom.minidom import parse, parseString


def getText(nodelist):
 rc = []
 for node in nodelist:
  if node.nodeType == node.TEXT_NODE:
   rc.append(node.data)
 return ''.join(rc)

def handleTags(dom,tag):
 items = dom.getElementsByTagName(tag)
 t = []
 for item in items:
  t.append(u'{0}'.format(getText(item.childNodes)).encode('utf-8'))
 return t
def GetNRFConfig(sock):
 SendData(sock,'{0}\r\n'.format(NPGetNRGConfig()))
 return ReceiveData(sock,"|+end\r\n")

def GetTempDS18B20(sock):
 SendData(sock,'{0}\r\n'.format(NPGetSensorDataTempDS18B20()))
 return ReceiveData(sock,"|+end\r\n")


def GetTempHumidDHT22(sock):
 SendData(sock,'{0}\r\n'.format(NPGetSensorDataTempHumidityDHT22()))
 return ReceiveData(sock,"|+end\r\n")
  
def GetTempDHT22(sock):
 SendData(sock,'{0}\r\n'.format(NPGetSensorDataTempDHT22()))
 return ReceiveData(sock,"|+end\r\n")
 
def GetHumidityDHT22(sock):
 SendData(sock,'{0}\r\n'.format(NPGetSensorDataHumidityDHT22()))
 return ReceiveData(sock,"|+end\r\n") 
def GetLightsense(sock):
 SendData(sock,'{0}\r\n'.format(NPGetSensorDataLightsense()))
 return ReceiveData(sock,"|+end\r\n")

def GetMovement(sock):
 SendData(sock,'{0}\r\n'.format(NPGetSensorDataMove()))
 return ReceiveData(sock,"|+end\r\n")
 
def GetRemoteSensors(socks):
  SendData(sock,'{0}\r\n'.format(NPGetSensorDataRemote()))
  return ReceiveData(sock,"|+end\r\n")
 
##########################################################################  

def Move(moveStr):
 dom = parseString(moveStr)
 return '{0}'.format(handleTags(dom,'mv')[0]).encode('utf-8')

def Temp(tempStr):
 dom = parseString(tempStr)
 return '{0}'.format(handleTags(dom,'T')[0]).encode('utf-8')

def Humidity(humStr):
 dom = parseString(humStr)
 return '{0}'.format(handleTags(dom,'H')[0]).encode('utf-8')
 
def RemoteLight(rsStr,idx):
 dom = parseString(rsStr)
 return '{0}'.format(handleTags(dom,'L')[idx]).encode('utf-8')
 
def LightsInside(lightStr):
 dom = parseString(lightStr)
 return '{0}'.format(handleTags(dom,'NSL19M51')[0]).encode('utf-8')

def LightsOn(lightVal):
 t = time.strftime("%I:%M:%S %p",time.localtime())
 st = subprocess.check_output(['./Lights.sh c 0 on'],shell=True)   
 return "{0} : {1} | {2}".format(t,st.rstrip(),lightVal)
 
def LightsOff(lightVal):
 t = time.strftime("%I:%M:%S %p",time.localtime())
 st = subprocess.check_output(['./Lights.sh c 0 off'],shell=True)   
 return "{0} : {1} | {2}".format(t,st.rstrip(),lightVal)
 
def SetLightColor(color):
 t = time.strftime("%I:%M %p",time.localtime())
 st = subprocess.check_output('./Lights.sh c 0 c %s' % color, shell=1)
 return "{0} : {1}".format(t,st.rstrip())
def SetBrightnes(level):
 t = time.strftime("%I:%M %p",time.localtime())
 st = subprocess.check_output('./Lights.sh c 0 B %s' % level, shell=1)
 return "{0} : {1}".format(t,st.rstrip())
 
 
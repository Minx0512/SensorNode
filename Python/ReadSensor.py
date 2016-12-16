import sys
import subprocess
import bluetooth
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

 
def GetDHT22(sock):
 SendData(sock,'DHT22\n')
 return ReceiveData(sock,"|end")
 
 
def GetRemoteSensors(socks):
  SendData(sock,'RS\n')
  return ReceiveData(sock,"|end")

def GetMovement(sock):
 SendData(sock,'Move\n')
 return ReceiveData(sock,"|end")
  
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

def LightsOn():
 subprocess.call(['./Lights.sh c 0 on'], shell=1)
 
def LightsOff():
 subprocess.call(['./Lights.sh c 0 off'], shell=1)
def SetLightColor(color):
 subprocess.call('./Lights.sh c 0 c %s' % color, shell=1)
def SetBrightnes(level):
 subprocess.call('./Lights.sh c 0 B %s' % level, shell=1)

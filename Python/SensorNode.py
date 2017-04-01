#!/usr/bin/python

from Tkinter import *
import tkMessageBox
import os
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
from gtk._gtk import ANCHOR_WEST
from DesignColors import *

def GetMovement(sock):
 SendData(sock,'{0}\r\n'.format(NPGetSensorDataMove()))
 return ReceiveData(sock,"|+end\r\n")
def GetTempHumidDHT22(sock):
 SendData(sock,'{0}\r\n'.format(NPGetSensorDataTempHumidityDHT22()))
 return ReceiveData(sock,"|+end\r\n")
def GetLightsense(sock):
 SendData(sock,'{0}\r\n'.format(NPGetSensorDataLightsense()))
 return ReceiveData(sock,"|+end\r\n")

def GetTime():
 while 1:   
  ti = time.strftime("%I:%M:%S %p | %A, %B %d, %Y",time.localtime())    
  timeStr.set(ti)   
 #return ti

def isNightTime(lightValue):
 t = int(time.strftime("%H",time.localtime()))
 #print "t: {0}".format(t)
 isNight = 1
 if t < 16 and t > 8:
  isNight = 0
 return isNight
  
def SleepMode(slpmd):
 slpmd = 1

def NoMovement(lastmv,lightV,tIn):
 statement = '/usr/bin/curl --header "Content-Type: text/plain" --request PUT --data "OFF" http://minxecho:8080/rest/items/RoomMatthias_Movement0/state'
 os.system(statement) 
 

def OnMovement(lightV,tIn,tOut,rsEneabled):
 statLight = int(statusLight.get())
 statement = '/usr/bin/curl --header "Content-Type: text/plain" --request PUT --data "ON" http://minxecho:8080/rest/items/RoomMatthias_Movement0/state'
 os.system(statement)
 statement = '/usr/bin/curl --header "Content-Type: text/plain" --request PUT --data "{0}" http://minxecho:8080/rest/items/RoomMatthias_LightVoltage0/state'.format(lightV)
 os.system(statement)
 
def DetectMovement(socket,tInside, tOutside,rSensorsEnabled,slpmode):
 
 
 lastmovement = 0
 lightson = 0;
 natLightVal = 0
 light = 0
 while 1:
     
  if int(autoLights.get())==1:  
   threadLock.acquire()
  #SendData(socket,"35|0|Mv\n")  
   recData = GetMovement(socket)
  
   #print "Move: ",recData
   mvs = re.findall("<mv>(.*)\|", recData)
   
   threadLock.release()
   
   #print mvs
   if len(mvs) > 0:
    m=int(mvs[0])
   else:
    m = -1 
  
   #print "mv: {0}".format(m)
  
 
   if m == 1: # movement detected
    lastmovement = int(time.time())
   #print "lastmovement: {0} ".format(lastmovement)
    while 1:
     threadLock.acquire()
     recData = GetLightsense(socket)
  #sleep(1)
 
  #print "recDataLight: ", recData
     l = re.findall("<NSL19M51>(.*)\|",recData)
     if len(l)>0:
      light=float(l[0])
     else:
      light = -1
 
     threadLock.release()
  
     if light>0:
      lightVal.set(light)
      break 
     else:
      sleep(2)
   #print l
    light = lightVal.get()  
   
    OnMovement(light,tInside,tOutside,rSensorsEnabled)
   
  # else:  # no movement
   # NoMovement(lastmovement,light,tInside,frame)
   
   sleep(1)     

def ReadSystemConfig(socket):
 threadLock.acquire() 
 recData = GetNRFConfig(socket)  
   
 #print "SysConfig: ",recData
 nrf = re.findall("<SYSTEM><VERSION>(.*)\|\|<NRF24L01><STATUS>(.*)\|<CONFIG>(.*)\|<RF_CH>(.*)\|<RF_SETUP>(.*)\|<EN_AA>(.*)\|<EN_RXADDR>(.*)\|<OBSERVE_TX>(.*)\|<RX_ADDR_P0>(.*)\|<RX_ADDR_P1>(.*)\|<RX_ADDR_P2>(.*)\|<RX_ADDR_P3>(.*)\|<RX_ADDR_P4>(.*)\|<RX_ADDR_P5>(.*)\|<TX_ADDR>(.*)\|<RX_PW_P0>(.*)\|<RX_PW_P1>(.*)\|<RX_PW_P2>(.*)\|<RX_PW_P3>(.*)\|<RX_PW_P4>(.*)\|<RX_PW_P5>(.*)\|\|",recData)
 
 threadLock.release()
   
 for n in nrf:
  print "Version: {0}".format(n[0])   
  print "Status: {0} \r\nConfig: {1}".format(n[1],n[2])
 
 return recData  
    

def ReadSensorDataLight(socket,tnum):
 while 1:
  threadLock.acquire()
  recData = GetLightsense(socket)
  #sleep(1)
 
  #print "recDataLight: ", recData
  l = re.findall("<NSL19M51>(.*)\|",recData)
  if len(l)>0:
   light=float(l[0])
  else:
   light = -1
 
  threadLock.release()
  
  if light>0:
   lightVal.set(light)
   statement = '/usr/bin/curl --header "Content-Type: text/plain" --request PUT --data "{0}" http://minxecho:8080/rest/items/RoomMatthias_LightVoltage0/state'.format(light)
   os.system(statement)
   sleep(120) 
  else:
   sleep(2)   
  
   
    

def ReadSensorData(socket,tnum):
 errC = 0   
 while 1: 
 
  threadLock.acquire() 
  recDataDHT22 = GetTempHumidDHT22(socket) 
    
  #print "recDHT: {0}",recDataDHT22
  dht = re.findall(".<DHT22><T unit='(.*)'>(.*)\|<H unit='(.*)'>(.*)\|\|err=(.*)\|",recDataDHT22)
  
  threadLock.release()
  t = time.strftime("%I:%M:%S %p",time.localtime())  
  
  for d in dht:  
   lightsMsg.set("{0} : DHT22: T: {1}{2} | H: {3}{4} | err: {5} | Light: {6}".format(t, d[1],d[0],d[3],d[2],d[4],lightVal.get()))
   try:
    DHT22TempVal.set(float(d[1]))
    DHT22TempUnit.set("{0}".format(d[0]))
    DHT22HumVal.set(float(d[3]))
    DHT22HumUnit.set("{0}".format(d[2]))
    errC = int(d[4])
   except:
    errC = -10 
  
  print lightsMsg.get()
   
  if errC is not 0:
   sleep(5)
  else:
   statement = '/usr/bin/curl --header "Content-Type: text/plain" --request PUT --data "{0}" http://minxecho:8080/rest/items/RoomMatthias_Temperature/state'.format(DHT22TempVal.get())
   os.system(statement)
   statement = '/usr/bin/curl --header "Content-Type: text/plain" --request PUT --data "{0}" http://minxecho:8080/rest/items/RoomMatthias_Humidity/state'.format(DHT22HumVal.get())
   os.system(statement)
       
   sleep(5*60)  

def SwitchON(frame):
# B.config(text="Lights ON/OFF")
 #B.config(command=lambda: SwitchOFF(frame))
 lightsMsg.set(LightsOn(lightVal.get()))
 statusLight.set(1)
 if menuID.get()==1:
  LightsMenu(frame)
 #B.config(bg=BUTTON_BG_COLOR_ACTIVE,activebackground=BUTTON_BG_COLOR_ACTIVE,fg=BUTTON_FONT_COLOR,activeforeground=BUTTON_FONT_COLOR_HOVER_ACTIVE,font="-weight bold")
 #tkMessageBox.showinfo( "Hello Python", "Hello World")

def SwitchOFF(frame):
 #B.config(text="Lights ON")
 #B.config(command= lambda: SwitchON(B))
 lightsMsg.set(LightsOff(lightVal.get()))
 #print lightsMsg
 statusLight.set(0)
 if menuID.get()==1:
  LightsMenu(frame)
 #B.config(bg=BUTTON_BG_COLOR_INACTIVE,activebackground=BUTTON_BG_COLOR_HOVER,activeforeground=BUTTON_FONT_COLOR_HOVER_INACTIVE)

def LightsAutoON(frame):
 #B.config(text="Auto")
 autoLights.set(1)
 if menuID.get()==1:
  LightsMenu(frame)
 #B.config(command= lambda: LightsAutoOFF(B))
 #B.config(bg=BUTTON_BG_COLOR_ACTIVE,activebackground=BUTTON_BG_COLOR_ACTIVE,fg=BUTTON_FONT_COLOR,activeforeground=BUTTON_FONT_COLOR_HOVER_ACTIVE,font="-weight bold")
 
def LightsAutoOFF(frame):
 autoLights.set(0)
 if menuID.get()==1:
  LightsMenu(frame)
 #B.config(command= lambda: LightsAutoON(B))
 #B.config(bg=BUTTON_BG_COLOR_INACTIVE,activebackground=BUTTON_BG_COLOR_HOVER,activeforeground=BUTTON_FONT_COLOR_HOVER_INACTIVE)    
 
def LightsAutoSlider(slider):
    
 if autoLights.get()==0: # auto off
    
  slider.config(bg=BUTTON_BG_COLOR_INACTIVE)
 else:
     
  slider.config(bg=BUTTON_BG_COLOR_ACTIVE)       
 
def SensorsMenu(frame): 
 for widget in frame.winfo_children():
    widget.destroy()      
 #frame.config(bg=APP_BG)
 
 label_sensor = Label(frame,text="Sensors")
 label_sensor.config(bg=APP_BAR,fg=APP_FONT_COLOR,font="-weight bold -size 20")
 label_sensor.place(x=0,y=0,width=512,height=50) 
 
 label_Sens_Tmp = Label(frame,text="Temperature: ")
 label_Sens_Tmp.config(bg=APP_BG,fg=APP_FONT_COLOR,font="-weight bold -size 15",anchor="w")
 label_Sens_Tmp.place(x=20,y=100,width=200,height=20)
 
 label_Sens_Tmp_Val = Label(frame,text="000.00")
 label_Sens_Tmp_Val.config(bg=APP_BG,fg=APP_FONT_COLOR,textvariable=DHT22TempVal,anchor="e",font="-size 12")
 label_Sens_Tmp_Val.place(x=325,y=100,width=100,height=20)
 
 label_Sens_Tmp_Unit = Label(frame,text="C")
 label_Sens_Tmp_Unit.config(bg=APP_BG,fg=APP_FONT_COLOR,textvariable=DHT22TempUnit,anchor="w",font="-size 12")
 label_Sens_Tmp_Unit.place(x=425,y=100,width=40,height=20)
 
 label_Sens_Hum = Label(frame,text="Humidity: ")
 label_Sens_Hum.config(bg=APP_BG,fg=APP_FONT_COLOR,font="-weight bold -size 15",anchor="w")
 label_Sens_Hum.place(x=20,y=150,width=200,height=20)
 
 label_Sens_Hum_Val = Label(frame,text="00.00")
 label_Sens_Hum_Val.config(bg=APP_BG,fg=APP_FONT_COLOR,textvariable=DHT22HumVal,anchor="e",font="-size 12")
 label_Sens_Hum_Val.place(x=325,y=150,width=100,height=20)
 
 label_Sens_Hum_Unit = Label(frame,text="RH")
 label_Sens_Hum_Unit.config(bg=APP_BG,fg=APP_FONT_COLOR,textvariable=DHT22HumUnit,anchor="w",font="-size 12")
 label_Sens_Hum_Unit.place(x=425,y=150,width=40,height=20)
 
 
 label_Sens_Light = Label(frame,text="Lightsensor: ")
 label_Sens_Light.config(bg=APP_BG,fg=APP_FONT_COLOR,font="-weight bold -size 15",anchor="w")
 label_Sens_Light.place(x=20,y=200,width=200,height=20)
 
 label_Sens_Light_Val = Label(frame,text="000.00")
 label_Sens_Light_Val.config(bg=APP_BG,fg=APP_FONT_COLOR,textvariable=lightVal,anchor="e",font="-size 12")
 label_Sens_Light_Val.place(x=325,y=200,width=100,height=20)
 
 Label_bottom = Label(frame, text=" ",padx=5,pady=5)
 Label_bottom.config(bg=APP_DIALOG,fg=APP_FONT_COLOR, textvariable=lightsMsg,font="-size 10",anchor="w")
 Label_bottom.place(relx=0,y=560,width=512,height=40)
 
 menuID.set(2)

def LightsMenu(frame):
 for widget in frame.winfo_children():
    widget.destroy()
 label_sensor = Label(frame,text="Lights")
 label_sensor.config(bg=APP_BAR,fg=APP_FONT_COLOR,font="-weight bold -size 20")
 label_sensor.place(x=0,y=0,width=512,height=50) 
 Button_Lights = Button(frame, text ="Lights ON/OFF", relief=FLAT,padx=5,pady=5)
 Button_AutoLights = Button(frame, text ="Auto",relief=FLAT,padx=5,pady=5)

  
 if statusLight.get() == 0: # Lights are off    
  Button_Lights.config(command =lambda: SwitchON(frame),bg=BUTTON_BG_COLOR_INACTIVE,activebackground=BUTTON_BG_COLOR_INACTIVE,fg=BUTTON_FONT_COLOR,activeforeground=BUTTON_FONT_COLOR_HOVER_INACTIVE,font="-weight bold",highlightthickness=0,bd=0)
 else:
  Button_Lights.config(command =lambda: SwitchOFF(frame),bg=BUTTON_BG_COLOR_ACTIVE,activebackground=BUTTON_BG_COLOR_ACTIVE,fg=BUTTON_FONT_COLOR,activeforeground=BUTTON_FONT_COLOR_HOVER_ACTIVE,font="-weight bold",highlightthickness=0,bd=0)
 
 if autoLights.get()==0: # auto off    
  Button_AutoLights.config(command =lambda: LightsAutoON(frame),bg=BUTTON_BG_COLOR_INACTIVE,activebackground=BUTTON_BG_COLOR_INACTIVE,fg=BUTTON_FONT_COLOR,activeforeground=BUTTON_FONT_COLOR_HOVER_INACTIVE,font="-weight bold",highlightthickness=0,bd=0)
 else:
  Button_AutoLights.config(command =lambda: LightsAutoOFF(frame),bg=BUTTON_BG_COLOR_ACTIVE,activebackground=BUTTON_BG_COLOR_ACTIVE,fg=BUTTON_FONT_COLOR,activeforeground=BUTTON_FONT_COLOR_HOVER_ACTIVE,font="-weight bold",highlightthickness=0,bd=0)
  
    
 Button_Lights.place(x=50, y=100,width=200, height=50) 
    
 Button_AutoLights.place(x=350, y=100,width=120, height=35)
 
 Label_bottom = Label(frame, text=" ",padx=5,pady=5)
 Label_bottom.config(bg=APP_DIALOG,fg=APP_FONT_COLOR, textvariable=lightsMsg,font="-size 10",anchor="w")
 Label_bottom.place(relx=0,y=560,width=512,height=40)
 #Button_AutoLights.invoke()
 
 #lights_Scale = Scale(frame,variable=autoLights)
#lights_Scale.config(command=LightsAutoSlider(lights_Scale),from_=0, to=1,orient=HORIZONTAL,sliderlength=20,showvalue=0,troughcolor=APP_BAR,highlightthickness=0,bd=0)
 
 #if autoLights.get()==0: # auto off
# lights_Scale.config(bg=BUTTON_BG_COLOR_INACTIVE)
 #else:
 # lights_Scale.config(bg=BUTTON_BG_COLOR_ACTIVE)   
 
 #lights_Scale.place(x=350,y=200,width=75)
   
 menuID.set(1) 

def SystemParamsMenu(socket,frame):
 for widget in frame.winfo_children():
  widget.destroy()
 label_sensor = Label(frame,text="System Parameter")
 label_sensor.config(bg=APP_BAR,fg=APP_FONT_COLOR,font="-weight bold -size 20")
 label_sensor.place(x=0,y=0,width=512,height=50)
 
 sysConfig = ReadSystemConfig(socket)
 
      

def ConfigureMenu(frame):
 for widget in frame.winfo_children():
  widget.destroy()
 label_sensor = Label(frame,text="Configure")
 label_sensor.config(bg=APP_BAR,fg=APP_FONT_COLOR,font="-weight bold -size 20")
 label_sensor.place(x=0,y=0,width=512,height=50) 
 


port = 1
conn=0
out_str = ''
bd_addr = '98:D3:31:FB:31:A6'
remoteSensorsEnabled = 0
sleepmode = 0

threadLock = thread.allocate_lock()

top = Tk()
# top.minsize(1024, 600)
# top.maxsize(1024,600)
# top.title("SensorNode")
# top.config(bg=APP_BAR)

timeStr = StringVar()
lightsMsg = StringVar()
lightVal = DoubleVar()
menuID = IntVar()
DHT22TempVal = DoubleVar()
DHT22TempUnit = StringVar()

DHT22HumVal = DoubleVar()
DHT22HumUnit = StringVar()

while conn==0:
 try:
  sock = BlueConnect(bd_addr,port,5.0)
  conn=1
 except IOError as e:
  lightsMsg.set("{0}".format(e))
  print lightsMsg.get()


# get temp and hum every hour
# get Lightsensors every min
# get RemoteSensors every min
prevmins = 0
thresh = 360
treshOut = 360



autoLights = IntVar()
autoLights.set(1)

statusLight = IntVar()
statusLight.set(0)


try:
 thread.start_new_thread(ReadSensorDataLight,(sock,2))
 thread.start_new_thread(DetectMovement, (sock,thresh,treshOut,remoteSensorsEnabled,sleepmode))
 thread.start_new_thread(ReadSensorData,(sock,2))
 thread.start_new_thread(GetTime,())
 #ReadSensorData(sock)
except Exception, e:
 print "{0}".format(e)        
 

#top.mainloop()
top.mainloop()









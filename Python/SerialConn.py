#!/usr/bin/python

import serial, time
from ReadSensor import *
from NodeProtocol import *


SERIALPORT = "/dev/ttyUSB0"
BAUDRATE = 9600

ser = serial.Serial(SERIALPORT, BAUDRATE)
ser.bytesize = serial.EIGHTBITS #number of bits per bytes
ser.parity = serial.PARITY_NONE #set parity check: no parity
ser.stopbits = serial.STOPBITS_ONE #number of stop bits
ser.timeout = None          #block read
    #ser.timeout = 0             #non-block read
    #ser.timeout = 2              #timeout block read
ser.xonxoff = False     #disable software flow control
ser.rtscts = False     #disable hardware (RTS/CTS) flow control
ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
ser.writeTimeout = 0     #timeout for write
print 'Starting Up Serial Monitor'

try:
 ser.open()

except Exception, e:
  print "error open serial port: " + str(e)
  exit()

if ser.isOpen():
 try:
   ser.flushInput() #flush input buffer, discarding all its contents
   ser.flushOutput()#flush output buffer, aborting current output
   ser.write("ATI")
   print("write data: ATI")
   time.sleep(0.5)
   numberOfLine = 0

   while True:

    response = ser.readline()
    print("read data: " + response)

    numberOfLine = numberOfLine + 1
    if (numberOfLine >= 5):
     break

   ser.close()

 except Exception, e:
  print "error communicating...: " + str(e)

else:
 print "cannot open serial port "

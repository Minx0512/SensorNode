#!/usr/bin/python

import serial, time
from cherrypy import response



def NewSerialCon(port, baudrate):
 ser = serial.Serial(port, baudrate)
 ser.bytesize = serial.EIGHTBITS #number of bits per bytes
 ser.parity = serial.PARITY_NONE #set parity check: no parity
 ser.stopbits = serial.STOPBITS_ONE #number of stop bits
 #ser.timeout = None          #block read
 #ser.timeout = 0             #non-block read
 #ser.timeout = 2              #timeout block read
 ser.xonxoff = False     #disable software flow control
 ser.rtscts = False     #disable hardware (RTS/CTS) flow control
 ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
 ser.writeTimeout = 0     #timeout for write
 #print 'Starting Up Serial Monitor'
 
 return ser


def ReadData(ser,queryStr):
 response = ''   
 try:
  ser.open()
 except Exception, e:
  print "error open serial port: " + str(e)
  
 if ser.isOpen():
  try:
   ser.flushInput() #flush input buffer, discarding all its contents
   ser.flushOutput()#flush output buffer, aborting current output
   ser.write("{0}\r\n".format(queryStr))
   #print("write data: {0}".format(queryStr))
   time.sleep(0.5)
   #numberOfLine = 0

  # while True:

   response = ser.read_until("\\\\+end")

    #numberOfLine = numberOfLine + 1
    #if (numberOfLine >= 1):
     #break

   ser.close()

  except Exception, e:
   print "error communicating...: " + str(e)

 else:
  print "cannot open serial port "
 
 return response
 
   
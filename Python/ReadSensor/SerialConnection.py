#!/usr/bin/python

import serial, time


class SerialConnection:
    """Sets up a serial connection"""
    def __init__(self,port,baudrate):
     self.ser = serial.Serial(port, baudrate)
     self.ser.bytesize = serial.EIGHTBITS #number of bits per bytes
     self.ser.parity = serial.PARITY_NONE #set parity check: no parity
     self.ser.stopbits = serial.STOPBITS_ONE #number of stop bits
     #ser.timeout = None          #block read
     #ser.timeout = 0             #non-block read
     #ser.timeout = 2              #timeout block read
     self.ser.xonxoff = False     #disable software flow control
     self.ser.rtscts = False     #disable hardware (RTS/CTS) flow control
     self.ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
     self.ser.writeTimeout = 0     #timeout for write
     #print 'Starting Up Serial Monitor'

    def ReadData(self,queryStr):
     resp = ""   
     try:
      self.ser.open()
     except Exception, e:
      print "error open serial port: " + str(e)
     
     if self.ser.isOpen():
      try:
       self.ser.flushInput() #flush input buffer, discarding all its contents
       self.ser.flushOutput()#flush output buffer, aborting current output
       self.ser.write("{0}\r\n".format(queryStr))
       #print("write data: {0}".format(queryStr))
       time.sleep(0.125)
       #numberOfLine = 0
       # while True:
       resp = self.ser.read_until("\\\\+end")
       self.ser.close()
      except Exception, e:
        print "error communicating...: " + str(e) 
     return resp
  
      
      
      
      
      
      
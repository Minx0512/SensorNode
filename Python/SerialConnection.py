#!/usr/bin/python3

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
     self.ser.timeout = 2              #timeout block read
     self.ser.xonxoff = False     #disable software flow control
     self.ser.rtscts = False     #disable hardware (RTS/CTS) flow control
     self.ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
     self.ser.writeTimeout = 0     #timeout for write
     self.err = ""
     ##print ('Starting Up Serial Monitor')

    def ReadData(self,queryStr):
     resp = ""   
     try:
      self.ser.open()
     except Exception as e:
      self.err = self.err + "error open serial port: " + str(e)
      #print(self.err)
     
     if self.ser.isOpen():
      try:
       self.ser.flushInput() #flush input buffer, discarding all its contents
       self.ser.flushOutput()#flush output buffer, aborting current output
       self.ser.write("{0}\r\n".format(queryStr).encode())
       #print("write data: {0}".format(queryStr))
       time.sleep(0.125)
       #numberOfLine = 0
       # while True:
       resp = self.ser.read_until("+end").decode('unicode-escape')
       #print(resp)
       self.ser.close()
      except Exception as e:
       self.err = self.err + "error communicating...: " + str(e)
       #(self.err) 
     return resp
  
      
      
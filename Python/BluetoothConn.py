import bluetooth


def BlueConnect(MAC,port,timeOut):
 print "Connecting to",MAC
 socket=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
 socket.connect((MAC, port))
 socket.settimeout(timeOut)
 return socket

def CloseConn(s):
 #print("disconnecting...")
 s.close()

def ReceiveData(s,endStr):
 out_str = ''
 lE = len(endStr)
 #print "Receiving data...", endStr
 try:
  while True:
   data = s.recv(8)
   out_str+=data
   #print out_str
   if out_str[len(out_str)-lE:len(out_str)]==endStr:
    break
 except IOError as e:
  return "IOError Receive Data: {0}: {1}".format(e,out_str)
 #print out_str 
 return out_str[0:len(out_str)-lE]

def SendData(s,string):
 #print "Sending: ",string
 try:
  s.send(string)
 except IOError as e:
  return "IOError Send Data: {0}".format(e)


def ReadSensorData(bd_addr,send_str):
 port = 1 
 conn=0
 out_str = ''
 while conn==0:
  try:
   sock = BlueConnect(bd_addr,port,5.0)
   conn=1
   SendData(sock,send_str)
   out_str = ReceiveData(sock,"\n|+end")
   CloseConn(sock)
  except IOError as e:
   print "Error ReadSensorData: {0}".format(e)
 return out_str

















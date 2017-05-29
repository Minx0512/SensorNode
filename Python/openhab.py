#!/usr/bin/python3

import os
import sys
import ssl
import urllib
import urllib.request


class OpenHab:
 def __init__(self, hst):
  """ Initialize Openhab Class """       
  self.host = hst   
  self.https_sslv3_handler = urllib.request.HTTPSHandler(context=ssl.SSLContext(ssl.PROTOCOL_SSLv23))
  self.opener = urllib.request.build_opener(self.https_sslv3_handler)
  urllib.request.install_opener(self.opener)
  
 def checkAllItemsExist(self):
  
  try:
       
   resp = self.opener.open(itemurl)
  except(HTTPError):
       
   pass
   #data = resp.read().decode('utf-8')
  #print(data)
      
  
  
 def putDataInItem(self,data,itemName):
  """ Put data to openhab item """
  
  mask =  '/usr/bin/curl -k --header "Content-Type: text/plain" --request PUT --data "{1}" {0}/rest/items/{2}/state'
  
  statement = mask.format(self.host,data ,itemName)
  
  #print(statement)
  #r = os.system(statement)   
  
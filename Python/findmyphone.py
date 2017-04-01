#!/usr/bin/python
import bluetooth

target_name = "MinxSensorNode01"
target_address = None

nearby_devices = bluetooth.discover_devices()

for bdaddr in nearby_devices:
    print "Address: ", bdaddr
    t = bluetooth.lookup_name( bdaddr,20 )
    print "Name: ", t
    if target_name == t :
        target_address = bdaddr
        break



if target_address is not None:
    print "found target bluetooth device with address ", target_address
else:
    print "could not find target bluetooth device nearby"



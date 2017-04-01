#!/usr/bin/python

import ReadSensor



rs = ReadSensor.ReadSensor("/dev/ttyAMA0", 9600)

rs.DS18B20("A0:A0:A0:A0:A0")

print rs.resp


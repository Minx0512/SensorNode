#!/usr/bin/python

import ReadSensor

rs = ReadSensor("/dev/ttyAMA0", 9600)

print rs.DS18B20("A0")


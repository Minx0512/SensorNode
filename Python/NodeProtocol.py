#!/usr/bin/python

import sys

def NPSetBluetoothName(name):
    return '10|0|{0}'.format(name)
def NPSetBluetoothPIN(pin):
    return '11|{0}|{0}'.format(pin)
def NPSetNRFPipe(pipeNr, mac):
    # PipeNr: 0-5
    # MAC: e.g. e0:f0:f0:f0:f0
    return '20|{0}|{1}'.format(pipeNr,mac)
def NPSetNRFPipeEnable(pipeNr):
    # PipeNr: 0-5
    return '21|{0}|PEn'.format(pipeNr)
def NPSetNRFChannel(chN):
    # chN: 0-125
    return '22|{0}|Chn'.format(chN)
def NPSetNRFPowerLevel(pwrlvl):
    # pwrlvl: 1-4
    return '23|{0}|PwrLvl'.format(pwrlvl)
def NPSetNRFDataRate(datart):
    # datart: 1-3
    return '24|{0}|DR'.format(datart)
def NPSetNRFCRCLevel(crclvl):
    # crclvl: 1-3
    return '25|{0}|CRCLvl'.format(crclvl)
def NPSetNRFPayLoadSize(payloadsze):
    # payloadsze: e.g. 16
    return '26|{0}|PLS'.format(payloadsze)
def NPSetNRFAutoACK(endisack):
    # endisack: 0/1
    return '27|{0}|ACK'.format(endisack)
def NPSetNRFResetModule():
    return '28|0|Reset'
def NPGetNRGConfig():
    return '29|0|GC'
def NPGetSensorDataTempDHT22():
    return '30|0|TmpDHT22'
def NPGetSensorDataHumidityDHT22():
    return '31|0|HumDHT22'
def NPGetSensorDataTempHumidityDHT22():
    return '32|0|TmpHumDHT22'
def NPGetSensorDataLightsense():
    return '33|0|Light'
def NPGetSensorDataTempDS18B20():
    return '34|0|DS'
def NPGetSensorDataMove():
    return '35|0|Mv'

def NPGetSensorDataRemote():
    return '36|0|RS'


    
    
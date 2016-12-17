# SensorNode
A little private project for a sensor node based on the ATmega328P.
It is connected to a HC-06 bluetooth module and a nRF24L01+ Transceiver module for communication, a DHT22 temperatur and humidity sensor, a resistive light sensor and a motion detector. 

> Code is not jet ready <

Through its comm interfaces it communicates with other tiny nodes based on the ATiny85 and the Raspberry Pi which will control everything, log the data into a database and sends control commands to a WLAN light control unit.

The sensor nodes Wireless addresses can be configured via the raspberry pie through a python script.

Configure protocol for SensorNode via Python command:

command id (CmdID) + command property (cmdP) | property id (PID/PVal) | property string (pString)

Bluetooth : 1 *10 + Name: 0 | Name: 0 | Name: eg. "MinxSensorNode01"

Bluetooth : 1 *10 + PIN: 1 | PIN: 01234 | "01234"

nRF24L01 : 2 *10 + Pipe: 0 | pipeID: (0-5)  1 | mac-address: e.g: "e0:f0:f0:f0:f0"

nRF24L01 : 2 *10 + EnablePipe: 1 | Enable PipeID: (0-5)  1 | ""

nRF24L01 : 2 *10 + Channel: 2  | ChannelID: (0-125)  10 | ""

nRF24L01 : 2 *10 + PowerLvl: 3 | PwrLvlVal: (1-4)  4 | ""

nRF24L01 : 2 *10 + DataRate: 4 | DataRteVal: (1-3) 2 | ""

nRF24L01 : 2 *10 + CRCLevel: 5 | CRCLvlVal: (1-3)  3 | ""

nRF24L01 : 2 *10 + PayloadSize: 6 | payloadSzVal: 16 | ""

nRF24L01 : 2 *10 + AutoAck: 7 | En/Dis ACK: (0/1)  1 | ""

nRF24L01 : 2 *10 + ReInitModule: 8 | 0 | ""

GetSensorData : 3 *10 + TempDHT22: 0 | 0 | ""

GetSensorData : 3 *10 + HumidityDHT22: 1 | 0 | ""

GetSensorData : 3 *10 + LightSense: 2 | 0 | ""

GetSensorData : 3 *10 + TempDS18B20: 3 | 0 | ""

GetSensorData : 3 *10 + Movement: 4 | 0 | ""





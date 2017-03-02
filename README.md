# SensorNode
A little private project for a sensor node based on the ATmega328P.
The main node is connected to the serial interface of a Raspberry Pi on which an openHab server is running.
A nRF24L01+ Transceiver module on the SPI-bus of the ATmega328P serves for communication to other nodes.
The main node has 2x DS18B20 temperature sensors, 1 motion sensor and 1 resistive light sensor connected to it.

The sensor nodes Wireless addresses can be configured via a python script.

> Code is not yet ready <

Communications protocoll between the raspberry pi and the main sensor node:


example request:
0x31|A0:A0:A0:A0:A0|DHT22 

|  |HEX(command id (CmdID) + command property (cmdP) )| property id (PID/PVal) | property string (pString)|
|-------- |--------------------------------------|------------------------|--------------------------|
|nRF24L01 Channel | 0x10 + 0x01  | ChannelID: (0-125)  10 | "" |
|nRF24L01 PowerLvl| 0x10 +  0x02 | PwrLvlVal: (1-4)  4 | ""|
|nRF24L01 DataRate| 0x10 +  0x03 | DataRteVal: (1-3) 2 | ""|
|nRF24L01 CRCLevel| 0x10 +  0x04 | CRCLvlVal: (1-3)  3 | ""|
|nRF24L01 PayloadSize| 0x10 + 0x05 | payloadSzVal: 16 | ""|
|nRF24L01 ResetModule| 0x10 + 0x06 | 0 | ""|
|nRF24L01 SystemInfo| 0x10 + 0x07 | 0 | ""|
|GetSensorInfos availableSensors on current node| 0x20 + 0x00 | %MAC | Sensors|
|GetSensorInfos availableSensors on connected node n| 0x20 + 0x01 | %MAC | Sensors|
|GetSensorData Movement| 0x30 + 0x01 | %MAC | Mv|
|GetSensorData DHT22 | 0x30 + 0x02 | %MAC | DHT22|
|GetSensorData LightSense| 0x30 + 0x03 | %MAC | Lap|
|GetSensorData LightSense| 0x30 + 0x04 | %MAC | Ldx|
|GetSensorData TempDS18B20| 0x30 + 0x05 | %MAC | TDS|
|GetSensorData Pressure| 0x30 + 0x06 | %MAC | P|

| | |
|------------| ------ |
|Light on analog Port | Lap |
|Light digital in Lux | Ldx |
|TempDS | TDS|
|TempHumDHT22 | DHT |
|Pressure | P|
|Movement | M|


answers:
```
DS18B20: //sensor/DST/|%MAC0|%MAC1|...|/|%TempVal0|%TempVal1|...|\\
DHT22: //sensor/DHT/|%sensorMAC|/|%tempVal:%unit|%humVal:%unit|%errCode|\\
Movement: //sensor/M/|%sensorMAC|/|(0|1)|\\
LightAnalog: //sensor/Lav/|%sensorMAC|/|%d.%03d|\\
```






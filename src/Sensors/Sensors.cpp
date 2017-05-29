/**
 ****************************************************************************
 *
 * Project: SensorNode
 * 
 * Sensors.cpp
 *
 * Created on: 25 May 2017
 *
 * Author: Matthias Minx
 *
 * Revision: 1
 *
 ****************************************************************************
 License:

 **************************************************************************/
/** \file Sensors.cpp
 * 
 */

#include "Sensors.hpp"

namespace Sensors {

Sensors::Sensors() : movement(0) {
	// TODO Auto-generated constructor stub



	ds18b20 = DS18B20(PORTD,DDRD,PIND, PD6);	//!< Create new DS18B20 object connected to pin PD6

	dht22 = DHT22(PORTD,DDRD,PIND,PD7); //!< Create new DHT22 object connected to pin PD7

	pr = Photoresistor(0x00); 	//!< Create new Photoresitor object connected to the analog output 0x00

// bmp180 = BMP180();



}

void Sensors::InitSensors(){

	pr.Init();



	char str[30];
//	sprintf(str," ");
	DS18B20_GetSensorValueString(str);

	DHT22_GetSensorValueString(str);


}


void Sensors::MV_SetMovementTrue(){

	movement = true;

}

void Sensors::MV_SetMovementFalse(){
	movement = false;

}

bool Sensors::MV_GetMovement(){

	return (movement);
}

void Sensors::PR_GetSensorValueString(char *string){

	pr.GetSensorStringXML(string);

}

void Sensors::DHT22_GetSensorValueString(char* string){

	dht22.GetSensorStringXML(string);
}

void Sensors::DS18B20_GetMACString(char* macOut){

	ds18b20.GetMACString(macOut);

}

void Sensors::DS18B20_GetSensorValueString(char* string){

	ds18b20.GetSensorStringXML(string);
}



} /* namespace Sensors */

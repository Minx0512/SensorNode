/*
 * Configuration.cpp
 *
 *  Created on: 16 Dec 2016
 *      Author: matthias
 */

#include "Configuration.h"

Configuration::Configuration() {
	// TODO Auto-generated constructor stub

	btooth.PIN = 1234;
	btooth.Name = "MinxSensorName01";

	nrfConf.Pipe.PipeID = 0;
	nrfConf.Pipe.MAC[0] = 0xe0;

	//1,10,4,2,3,16,1};



}
Configuration::Configuration(uint8_t cmdID, uint8_t cmdP, uint8_t PID, char* pString){


	switch(cmdID){
		case 0: // config bluetooth
			break;
		case 1: // config nRF24L01
			break;
		case 2: //
			break;
		default:
			break;

	};






}



void Configuration::getBluetoothConfig(uint8_t PID){






}

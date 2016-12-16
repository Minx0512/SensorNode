/*
 * Configuration.h
 *
 *  Created on: 16 Dec 2016
 *      Author: matthias
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct{
	uint16_t PIN;
	char *Name;

}BTConfig;


typedef struct{

	uint8_t PipeID;
	uint8_t MAC[5];


}PipeMac;

typedef struct{

	PipeMac Pipe;
	uint8_t EnablePipe;
	uint8_t Channel;
	uint8_t PwrLvl;
	uint8_t DataRate;
	uint8_t CRCLvl;
	uint8_t PayloadSz;
	uint8_t AutoACK;


}nRFConfig;

class Configuration {

	BTConfig btooth;

	nRFConfig nrfConf;




public:

	Configuration();
	Configuration(uint8_t cmdID, uint8_t cmdP, uint8_t PID, char* pString);

	void getBluetoothConfig(uint8_t );


private:

	//SetArray();



};

#endif /* CONFIGURATION_H_ */

/*
 * Photoresistor.h
 *
 *  Created on: Nov 12, 2016
 *      Author: matthias
 */

#ifndef PHOTORESISTOR_H_
#define PHOTORESISTOR_H_

#include <avr/io.h>

class Photoresistor {

	uint8_t type;

	uint8_t ADPort;

public:

	uint16_t readADValue();
	void SetType(uint8_t t);
	void SetPort(uint8_t port);


	Photoresistor();
	Photoresistor(uint8_t port);

	void GetSensorStringXML(char* string);
private:

	void Init();

};

//extern Photoresistor photoresistor;

#endif /* PHOTORESISTOR_H_ */

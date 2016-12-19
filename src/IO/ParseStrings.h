/*
 * ParseStrings.h
 *
 *  Created on: 15 Dec 2016
 *      Author: matthias
 */

#ifndef PARSESTRINGS_H_
#define PARSESTRINGS_H_

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <avr/pgmspace.h>

#include "USART.h"

class ParseStrings {

		uint8_t cmdID;
		uint8_t cmdP ;
		uint8_t PID;
		char pString[36];
		uint8_t addr[5];


public:


	void Parse(const char* string);
	void ParseMAC();

	uint8_t * getMAC();
	uint8_t getMAC(uint8_t* a);
	void getPropertyString(char *propString);

	uint8_t getCmdID();
	uint8_t getCmdProperty();
	uint8_t getPropertyID();


	void getBTName(char* name);

	void PrintVars(char* string);
	void PrintMAC(char* string);


	void ClearAll();


	ParseStrings();
	ParseStrings(char* string);


private:
	//uint8_t strtoken(char *string, char *separator, char *token[]);

	USART u;

};

#endif /* PARSESTRINGS_H_ */

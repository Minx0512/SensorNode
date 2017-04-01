/**
****************************************************************************
* Project: SensorNode
*
* ParseStrings.h
*
* Created on: 15 Dec 2016
*
* Author: Matthias Minx
*
* Revision: 0.2
*
*
****************************************************************************/
/** \file ParseStrings.h
 *
 */

#ifndef PARSESTRINGS_H_
#define PARSESTRINGS_H_

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <avr/pgmspace.h>
#include <stdint.h>
//#include "USART.h"


//! ParseStrings class definition
/*!
 * Defines methods for accessing  the ParseStrings interface
 *
 */
class ParseStrings {

		//! Command ID
		/*!
		 *
		 */
		uint8_t cmdID;

		//! Command Property
		/*!
		 *
		 */
		uint8_t cmdP ;

		//! Property ID
		/*!
		 *
		 */
		char PID[36];

		//! Property String
		/*!
		 *
		 */
		char pString[36];

		//! 5 Byte address array
		/*!
		 *
		 */
		uint8_t addr[5];


public:

	//! Parse const character string
	/*!
	 *
	 * @param[in] string Character string to parse
	 */
	void Parse(const char* string);

	//! Parse MAC address
	/*!
	 * Parses MAC address from pString into the addr array
	 */
	void ParseMAC();

//	uint8_t * getMAC();
	//uint8_t getMAC(uint8_t* a);
	//! Get MAC address as uint64_t
	/*!
	 *
	 * @return MAC address as a 64 Bit unsigned integer
	 */
	uint64_t getMAC();

	//! Get Property string
	/*!
	 *
	 * @param[out] propString Character array
	 */
	void getPropertyString(char *propString);

	//! get Property ID as a MAC address array
	/*!
	 *
	 * @param[out] propString 8 Byte Address array
	 */
	void getPropertyIDAsMAC(uint8_t propString[8]);

	//! get Command ID
	/*!
	 *
	 * @return Command ID as uint8_t
	 */
	uint8_t getCmdID();

	//! Returns command property
	/*!
	 *
	 * @return Command Property as uint8_t
	 */
	uint8_t getCmdProperty();

	//! Returns Property ID as character array
	/*!
	 *
	 * @param[out] PIDString Character array of Property ID
	 */
	void getPropertyID(char *PIDString);



	//void getBTName(char* name);

	//! Returns Variables as string
	/*!
	 *
	 * @param[out] string Character array of variable values
	 */
	void PrintVars(char* string);

	//! Returns MAC address to string
	/*!
	 *
	 * @param[out] string Character array of MAC address
	 */
	void PrintMAC(char* string);

	//! Clears all variables
	void ClearAll();

	//! ParseStrings standard constructor
	/*!
	 *  new ParseStrings object and initializes variables
	 */
	ParseStrings();

	//! ParseStrings constructor
	/*!
	 *
	 * @param[in] string Character array to parse
	 */
	ParseStrings(char* string);


private:
	//uint8_t strtoken(char *string, char *separator, char *token[]);


//	USART u;

};

#endif /* PARSESTRINGS_H_ */

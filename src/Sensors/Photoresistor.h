/**
****************************************************************************
* Project: SensorNode
*
* Photoresistor.h
*
* Created on: Nov 12, 2016
*
* Author: Matthias Minx
*
* Revision: 0.2
*
*
****************************************************************************/
/** \file Photoresistor.h
 *
 */



#ifndef PHOTORESISTOR_H_
#define PHOTORESISTOR_H_

#include <avr/io.h>

namespace Sensors {

//! Photoresistor class definition
/*!
 * Defines methods for accessing  the Photoresistor interface
 *
 */
class Photoresistor {

	//!
	uint8_t type; //!< Light detector type

	uint8_t ADPort; //!< Analog/Digital converter port

public:




	//! Photoresistor standard constructor
	/*!
	 *  new Photoresistor object and initializes variables
	 */
	Photoresistor();

	//! Photoresistor constructor
	/*!
	 *
	 * @param[in] port Port number of analog/digital converter
	 */
	Photoresistor(uint8_t port);

	//! Initialize analog digital converter
	/*!
	 *
	 */
	void Init();

	//! Get sensor value
	/*!
	 * Returns a character array with the average Sensor value of 1000 measurements
	 * and adds a | to the end of it.
	 * @param[out] string
	 */
	void GetSensorStringXML(char* string);


private:



	//! Set Port of analog/digital converter
	/*!
	 *
	 * @param[in] port Port number
	 */
	void setPort(uint8_t port);

	//! Set Type of light detector
	/*!
	 *
	 * @param[in] t Type number
	 */
	void setType(uint8_t t);

	//! Reads digital value of the AD converter
	/*!
	 *
	 * @return Digital value of one AD conversion
	 */
	uint16_t readADValue();



};

}
//extern Photoresistor photoresistor;

#endif /* PHOTORESISTOR_H_ */

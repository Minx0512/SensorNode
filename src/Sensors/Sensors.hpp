/**
 ****************************************************************************
 *
 * Project: SensorNode
 * 
 * Sensors.hpp
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
/** \file Sensors.hpp
 * 
 */

#ifndef SENSORS_HPP_
#define SENSORS_HPP_

//#include <avr/io.h>



#include "DHT22.h"
#include "DS18B20.h"
#include "Photoresistor.h"

//#include "BMP180.hpp"



namespace Sensors {

class Sensors {



	DHT22 dht22;

	DS18B20 ds18b20;

	Photoresistor pr;

	bool movement;




public:


	Sensors();

	void InitSensors(void);

	//! Set movement flag
	/*!
	 * Set movement flag to true
	 */
	void MV_SetMovementTrue();

	//! Set movement flag
	/*!
	 * Reset movement flag to false..
	 */
	void MV_SetMovementFalse();

	//! Get movement
	/*!
	 * Returns true or false depending if movement was detected or not.
	 * @return bool
	 */
	bool MV_GetMovement();


	//! Get sensor value
	/*!
	 * Returns a character array with the average Sensor value of 1000 measurements
	 * and adds a | to the end of it.
	 * @param[out] string
	 */
	void PR_GetSensorValueString(char *string);

	//! Get sensor value
	/*!
	 * Returns a character array with the average Sensor value of 1000 measurements
	 * and adds a | to the end of it.
	 * @param[out] string
	 */
	void DHT22_GetSensorValueString(char *string);

	//! Get MAC addresses from MACString
	/*!
	 *
	 * @param[out] MACStringOut Character array of MAC addresses
	 */
	void DS18B20_GetMACString(char* MACStringOut);

	// //! Get temperature string from
	//void GetTemperatureString(char* TemperatureString);
	//! Get xml style sensor string
	/*!
	 * Puts out one string with the temperature values
	 * @param[out] string XML style sensor string
	 */
	void DS18B20_GetSensorValueString(char* string);


private:






};

} /* namespace Sensors */

#endif /* SENSORS_HPP_ */

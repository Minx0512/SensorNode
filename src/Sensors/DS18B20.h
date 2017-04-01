/**
****************************************************************************
* Project: SensorNode
*
* DS18B20.h
*
* Created on: Nov 12, 2016
*
* Author: Matthias Minx
*
* Revision: 0.2
*
*
****************************************************************************/
/** \file DS18B20.h
 *
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <ctype.h>
#include <inttypes.h>

//#include "../IO/USART.h"


#ifndef FOSC
#define	FOSC			16000000UL		//!< CPU clock speed
#endif
#define LOOP_CYCLES  8  				//!< Number of cycles that the loop takes
#define us(num) (num/(LOOP_CYCLES*(1/(FOSC/1000000.0))))	//!< Microseconds

#define THERM_CMD_CONVERTTEMP 0x44		//!< Command convert temperature
#define THERM_CMD_RSCRATCHPAD 0xBE		//!< Command read scratchpad
#define THERM_CMD_WSCRATCHPAD 0x4E		//!< Command write scratchpad
#define THERM_CMD_CPYSCRATCHPAD 0x48	//!< Command copy scratchpad
#define THERM_CMD_RECEEPROM 0xb8		//!< Command record eeprom
#define THERM_CMD_RPWRSUPPLY 0xb4		//!< Command reset power supply

#define THERM_CMD_READROM 0x33			//!< Command read ROM
#define THERM_CMD_ALARMSEARCH 0xec		//!< Command alarm search
#define THERM_DECIMAL_STEPS_12BIT 625 	//!< Decimal Steps 12 Bit: .0625

#define MATCH_ROM	0x55				//!< Match Rom
#define SKIP_ROM	0xCC				//!< Skip ROM search
#define	SEARCH_ROM	0xF0				//!< Init ROM search

#define	SEARCH_FIRST	0xFF			//!< start new search
#define	PRESENCE_ERR	0xFF			//!< Presence error
#define	DATA_ERR	0xFE				//!< Data error
#define LAST_DEVICE	0x00				//!< last device found




//! DS18B20 class definition
/*!
 * Defines methods for accessing  the DS18B20 interface
 *
 */
class DS18B20 {




	volatile uint8_t* WIRE_PORT; //!< Thermometer data port
	volatile uint8_t* WIRE_DDR; //!< Thermometer data port
	volatile uint8_t* WIRE_PIN; //!< Thermometer data port



	uint8_t WIRE_DQ;			//!< Thermometer data line
	char MACString[120]; 		//!< MAC string of the sensor


public:

// USART u;



	//! Takes a Measurement per sensor on the bus
	/*!
	 *
	 * @param[out] string String with MAC addresses and temperature values
	 */
	void ReadMeasurement( char* string );

	//! Takes a Measurement per sensor on the bus
	/*!
	 *
	 * @param[out] Macstring MAC addresses of each sensor on the bus
	 * @param[out] tempStr Temperature values in one string
	 */
	void ReadMeasurement2( char* Macstring, char* tempStr );

	//! DS18B20 standard constructor
	/*!
	 *  new DS18B20 object and initializes variables
	 */
	DS18B20();

	//! DS18B20 constructor
	/*!
	 *
	 * @param[in] Wire_PORT
	 * @param[in] Wire_DDR
	 * @param[in] Wire_PIN
	 * @param[in] Wire_DQ Data line
	 */
	DS18B20(volatile uint8_t &Wire_PORT, volatile uint8_t &Wire_DDR, volatile uint8_t &Wire_PIN,uint8_t Wire_DQ);


	//void therm_read_DS18B20(char*buffer);
	//! Get MAC addresses from MACString
	/*!
	 *
	 * @param[out] MACStringOut Character array of MAC addresses
	 */
	void GetMACString(char* MACStringOut);

	// //! Get temperature string from
	//void GetTemperatureString(char* TemperatureString);

	//! Get xml style sensor string
	/*!
	 * Puts out one string with MAC addresses and their temperature values
	 * @param[out] string XML style sensor string
	 */
	void GetSensorStringXML(char* string);

	//! Get temperature values in one string an the MAC addresses in a seperate one
	/*!
	 *
	 * @param[out] MACstring MAC addresses of each sensor on the bus
	 * @param[out] tempString Temperature values in one string
	 */
	void GetTemperature(char* MACstring,char* tempString);


private:




	//! Sensor Initialization
	/*!
	 *  Starts
	 */
	void StartMeasurement( void );
	uint8_t Reset();
	//uint8_t Reset(char* string);

	//! BitIO
	/*!
	 *
	 * @param[in] bit
	 * @return
	 */
	uint8_t BitIO(uint8_t bit);
	uint8_t WriteByte(uint8_t byte);
	uint8_t ReadByte(void);
	unsigned char RomSearch(unsigned char diff, unsigned char *id);
	void Command( unsigned char command, unsigned char *id );


	void PIN_INPUT_MODE(){ *WIRE_DDR&=~(1<<WIRE_DQ);}
	void PIN_OUTPUT_MODE(){ *WIRE_DDR|=(1<<WIRE_DQ);}
	void PIN_LOW(){ *WIRE_PORT&=~(1<<WIRE_DQ);}
	void PIN_HIGH(){*WIRE_PORT|=(1<<WIRE_DQ);}



	inline __attribute__((gnu_inline)) void delay_us(uint16_t delay){
		  	 while(delay--) asm volatile("nop");
		  }

};

#endif /* DS18B20_H_ */

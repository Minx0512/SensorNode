/**
 ****************************************************************************
 *
 * Project: SensorNode
 * 
 * IO.hpp
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
/** \file IO.hpp
 * 
 */

#ifndef IO_HPP_
#define IO_HPP_

#include "I2C.hpp"
#include "USART.h"
#include "SPI.h"

//namespace IO {

//class SPI;
//class USART;

class IO {


	SPI spi;
	USART usart;
	I2C i2c;

	uint8_t uartRX_flag; 	//!< Flag received UART message


public:





	IO();

	void USART_SetRxFlagTrue();

	void USART_SetRxFlagFalse();

	bool USART_GetRxFlag();

	//! Writes String to interface
	/*!
	 *
	 * @param str character array
	 */
	void USART_writeString(char *str);

	//! Reads incoming string
	/*!
	 *
	 * @param[out] myString character array
	 * @param[in] maxLength Maximum length of string to read
	 */
	void USART_readString(char *myString,uint8_t maxLength);

	//! Reads incoming string
	/*!
	 * Reads incoming string until a new line character
	 * @param[out] string character array
	 */
	void USART_readString(char* string);

	//! Flush USART buffer
	/*!
	 *
	 */
	void USART_Flush(void);

	//! Write and Read one byte
	/*!
	 *
	 * @param[in] data Databyte to write
	 * @return Byte
	 */
	uint8_t SPI_WriteReadbyte(uint8_t data);




	void InitInterfaces(void);










};

//}

#endif /* IO_HPP_ */

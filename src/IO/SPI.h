/**
****************************************************************************
* Project: SensorNode
*
* SPI.h
*
* Created on: Nov 12, 2016
*
* Author: Matthias Minx
*
* Revision: 1
*
*
****************************************************************************/
/** \file SPI.h
 *
 */

#ifndef SPI_H_
#define SPI_H_


#include <avr/io.h>

//namespace SPI {

//! SPI class definition
/*!
 * Defines methods for accessing  the SPI interface
 *
 */
class SPI {


	//spi ports
	volatile uint8_t* SPI_DDR;
	//volatile uint8_t* SPI_PORT;
	uint8_t SPI_MISO;
	uint8_t SPI_MOSI;
	uint8_t SPI_SCK;
	uint8_t SPI_SS;







public:

	//! SPI standard constructor
	/*!
	 *  new SPI object where MOSI, MISO, SCK and SS are set according to the pins on the microcontroller
	 *
	 */
	SPI();


	//! SPI constructor
	/*!
	 *
	 * @param[in] DDR
	 * @param[in] MISO Master In Slave Out
	 * @param[in] MOSI Master Out Slave In
	 * @param[in] SCK Signal Clock
	 * @param[in] SS Slave Select
	 */
	SPI(volatile uint8_t &DDR,uint8_t MISO, uint8_t MOSI,uint8_t SCK,uint8_t SS);

	//! Initialize SPI
	/*!
	 *
 	 */
	void Init();

	//! Write and Read one byte
	/*!
	 *
	 * @param[in] data Databyte to write
	 * @return Byte
	 */
	uint8_t WriteReadbyte(uint8_t data);

private:






};

//}

#endif /* SPI_H_ */

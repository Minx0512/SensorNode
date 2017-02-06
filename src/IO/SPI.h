/*
 * SPI.h
 *
 *  Created on: Nov 12, 2016
 *      Author: matthias
 */

#ifndef SPI_H_
#define SPI_H_


#include <avr/io.h>

//SPI spi = SPI();

class SPI {


	//spi ports
	volatile uint8_t* SPI_DDR;
	//volatile uint8_t* SPI_PORT;
	uint8_t SPI_MISO;
	uint8_t SPI_MOSI;
	uint8_t SPI_SCK;
	uint8_t SPI_SS;







public:
	SPI();
	SPI(volatile uint8_t &DDR,uint8_t MISO, uint8_t MOSI,uint8_t SCK,uint8_t SS);

	void Init();
	uint8_t WriteReadbyte(uint8_t data);

private:






};

#endif /* SPI_H_ */

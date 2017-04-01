/**
****************************************************************************
* Project: SensorNode
*
* SPI.cpp
*
* Created on: Nov 12, 2016
*
* Author: Matthias Minx
*
* Revision: 1
*
*
****************************************************************************/
/** \file SPI.cpp
 *
 */

#include "SPI.h"

#include <avr/io.h>
#include <avr/interrupt.h>

/*!
 *
 */
SPI::SPI() {


	//spi ports

#if defined(__AVR_ATmega328P__)

		 SPI_DDR =&DDRB;
		// SPI_PORT = &PORTB;
		 SPI_MISO = PB4;
		 SPI_MOSI = PB3;
		 SPI_SCK = PB5;
		 SPI_SS = PB2;
#endif


}

SPI::SPI(volatile uint8_t &DDR,uint8_t MISO, uint8_t MOSI,uint8_t SCK,uint8_t SS){

	//spi ports
	 SPI_DDR =&DDR;
	// SPI_PORT = &PORT;
	 SPI_MISO = MISO;
	 SPI_MOSI = MOSI;
	 SPI_SCK = SCK;
	 SPI_SS = SS;




}


void SPI::Init() {
  //  SPI_DDR &= ~((1<<SPI_MOSI) | (1<<SPI_MISO) | (1<<SPI_SS) | (1<<SPI_SCK)); //input
    *SPI_DDR |= ((1<<SPI_MOSI) | (1<<SPI_SS) | (1<<SPI_SCK)); //output
    *SPI_DDR &= ~(1<<SPI_MISO);


    SPCR = ((1<<SPE)|               // SPI Enable
            (0<<SPIE)|              // SPI Interupt Enable
            (0<<DORD)|              // Data Order (0:MSB first / 1:LSB first)
            (1<<MSTR)|              // Master/Slave select
            (0<<SPR1)|(1<<SPR0)|    // SPI Clock Rate
            (0<<CPOL)|              // Clock Polarity (0:SCK low / 1:SCK hi when idle)
            (0<<CPHA));             // Clock Phase (0:leading / 1:trailing edge sampling)

    SPSR = (1<<SPI2X); // Double SPI Speed Bit
    sei();
}

uint8_t SPI::WriteReadbyte(uint8_t data) {
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
    return SPDR;
}



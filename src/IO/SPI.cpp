/*
 * SPI.cpp
 *
 *  Created on: Nov 12, 2016
 *      Author: matthias
 */

#include "SPI.h"

#include <avr/io.h>
#include <avr/interrupt.h>


SPI::SPI() {
	// TODO Auto-generated constructor stub

	//spi ports
		 SPI_DDR =&DDRB;
		 SPI_PORT = &PORTB;
		 SPI_MISO = PB4;
		 SPI_MOSI = PB3;
		 SPI_SCK = PB5;
		 SPI_SS = PB2;



}

SPI::SPI(volatile uint8_t &DDR,volatile uint8_t &PORT,uint8_t MISO, uint8_t MOSI,uint8_t SCK,uint8_t SS){

	//spi ports
	 SPI_DDR =&DDR;
	 SPI_PORT = &PORT;
	 SPI_MISO = MISO;
	 SPI_MOSI = MOSI;
	 SPI_SCK = SCK;
	 SPI_SS = SS;




}


/*
 * spi initialize
 */
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

/*
 * spi write one byte and read it back
 */
uint8_t SPI::WriteReadbyte(uint8_t data) {
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
    return SPDR;
}



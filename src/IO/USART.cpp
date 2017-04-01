/**
****************************************************************************
* Project: SensorNode
*
* USART.cpp
*
* Created on: Nov 12, 2016
*
* Author: Matthias Minx
*
* Revision: 0.2
*
*
****************************************************************************/
/** \file USART.cpp
 *
 */


#include "USART.h"
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>


USART::USART(){


	//FOSC =16000000UL;
	_BAUD =	9600;					// Baud rate (9600 is default)
//	_DATA = 0x03;					// Number of data bits in frame = byte tranmission
	_UBRR = (FOSC/16)/_BAUD - 1;	// Used for UBRRL and UBRRH

//	RX_BUFF=10;



}

USART::USART(uint32_t baudrate){

	_BAUD =	baudrate;
	_UBRR = (FOSC/16)/_BAUD - 1;
}


/*! \brief Configures baud rate (refer datasheet) */
void USART::initUART(void){
	// Not necessary; initialize anyway
	DDRD |= _BV(PD1);
	DDRD &= ~_BV(PD0);



	// Set baud rate; lower byte and top nibble
	UBRR0H = ((_UBRR) & 0xF00);
	UBRR0L = (uint8_t) ((_UBRR) & 0xFF);

	cli();

	TX_START();
	RX_START();

	RX_INTEN();



	// Set frame format = 8-N-1
	UCSR0C = (_DATA << UCSZ00);

	sei();

}

/*! \brief Returns a byte from the serial buffer
 * 	Use this function if the RX interrupt is not enabled.
 * 	Returns 0 on empty buffer
 */
uint8_t USART::getByte(void){
	// Check to see if something was received
	while (!(UCSR0A & _BV(RXC0)));
	return (uint8_t) UDR0;
}


/*! \brief Transmits a byte
 * 	Use this function if the TX interrupt is not enabled.
 * 	Blocks the serial port while TX completes
 */
void USART::putByte(unsigned char data){
	// Stay here until data buffer is empty
	while (!(UCSR0A & _BV(UDRE0)));
	UDR0 = (unsigned char) data;

}

/*! \brief Empties the RX buffer */
void USART::USART0_Flush(void){
	unsigned char dummy;
	while(UCSR0A & (1<<RXC0)) dummy = UDR0;
}




/*! \brief Writes an ASCII string to the TX buffer */
void USART::writeString(char *str){
	while (*str != '\0')
	{
		putByte(*str);
		++str;
	}
}

/*! \brief Reads an ASCII string from the RX buffer */
const char* USART::readString(void){
	static char rxstr[RX_BUFF];
	static char* temp;
	temp = rxstr;

	uint8_t num = 0;
	while(((*temp = getByte()) != '\n') && (num<RX_BUFF-2)){

		++temp;
		num++;
	}

	return rxstr;
}

/*! \brief Reads an ASCII string from the RX buffer */
void USART::readString(char myString[],uint8_t maxLength){

	char response;
	uint8_t i;

	i = 0;
	while(i<(maxLength-1)){

		response = getByte();
		putByte(response);
		if(response=='\r'){
			break;
		}else{
			myString[i] = response;
			i++;
		}


	}
	myString[i]=0;



}

/*! \brief Reads an ASCII string from the RX buffer */
void USART::readString(char* string){

//static char rxstr[l_puffer];
	char* temp;
	temp = string;

	while((*temp = getByte()) != '\n'){
			++temp;
	}


	USART0_Flush();


}





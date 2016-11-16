/*
 * USART.h
 *
 *  Created on: Nov 12, 2016
 *      Author: matthias
 */

#ifndef USART_H_
#define USART_H_



#include <avr/io.h>
#include <util/delay.h>
#include <string.h>


/* Useful macros */
#define TX_START()		UCSR0B |= _BV(TXEN0)	// Enable TX
#define TX_STOP()		UCSR0B &= ~_BV(TXEN0)	// Disable TX
#define RX_START()		UCSR0B |= _BV(RXEN0)	// Enable RX
#define RX_STOP()		UCSR0B &= ~_BV(RXEN0)	// Disable RX
#define COMM_START()		TX_START(); RX_START()	// Enable communications

/* Interrupt macros; Remember to set the GIE bit in SREG before using (see datasheet) */
#define RX_INTEN()		UCSR0B |= _BV(RXCIE0)	// Enable interrupt on RX complete
#define RX_INTDIS()		UCSR0B &= ~_BV(RXCIE0)	// Disable RX interrupt
#define TX_INTEN()		UCSR0B |= _BV(TXCIE0)	// Enable interrupt on TX complete
#define TX_INTDIS()		UCSR0B &= ~_BV(TXCIE0)	// Disable TX interrupt


#define	FOSC			16000000UL
/* Settings */
//#define _BAUD			9600					// Baud rate (9600 is default)
#define _DATA			0x03					// Number of data bits in frame = byte tranmission
//#define _UBRR			(FOSC/16)/_BAUD - 1		// Used for UBRRL and UBRRH

#define RX_BUFF			8



class USART {

	/* Settings */
//	long	FOSC;
	uint32_t _BAUD;					// Baud rate (9600 is default)
//	int _DATA;					// Number of data bits in frame = byte tranmission
	uint32_t _UBRR;	// Used for UBRRL and UBRRH

//	uint8_t RX_BUFF;






public:
	USART();
	USART(uint32_t baudrate);


	void writeString(char *str);
	const char* readString(void);
	void readString(char* string);

	void initUART(void);
	void USART0_Flush(void);


private:


	uint8_t getByte(void);
	void putByte(unsigned char data);




	/* Useful macros */
/*
	void TX_START(){UCSR0B |= _BV(TXEN0);} // Enable TX
	void TX_STOP(){UCSR0B &= ~_BV(TXEN0);} // Disable TX
	void RX_START(){UCSR0B |= _BV(RXEN0);}	// Enable RX
	void RX_STOP(){UCSR0B &= ~_BV(RXEN0);}	// Disable RX
	void COMM_START(){TX_START(); RX_START();}	// Enable communications
	*/

	/* Interrupt macros; Remember to set the GIE bit in SREG before using (see datasheet) */
	/*
	void RX_INTEN(){UCSR0B |= _BV(RXCIE0);}	// Enable interrupt on RX complete
	void RX_INTDIS(){UCSR0B &= ~_BV(RXCIE0);}	// Disable RX interrupt
	void TX_INTEN(){UCSR0B |= _BV(TXCIE0);}	// Enable interrupt on TX complete
	void TX_INTDIS(){UCSR0B &= ~_BV(TXCIE0);}	// Disable TX interrupt

*/

};
//extern USART uart;

#endif /* USART_H_ */

/*
 * main.cpp
 *
 *  Created on: Nov 12, 2016
 *      Author: matthias
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IO/USART.h"


#include "Sensors/DS18B20.h"
#include "Sensors/Photoresistor.h"



#include "Wireless/NRF24L01.h"

//#define F_CPU 16000000

#define CE PB1
#define CSN PB0


USART uart = USART(9600);


DS18B20 ds = DS18B20(PORTD,DDRD,PIND, PD7);
Photoresistor pr = Photoresistor(0x00);

NRF24L01 nrf = NRF24L01(DDRB, PORTD, CE, CSN);

void delay_ms(uint16_t ms){
	for(uint16_t t=0; t<=ms; t++){
		_delay_ms(1);
	}
}


void SetupNRF(){

	uint8_t a0[5] = {0xe0, 0xf0, 0xf0, 0xf0, 0xf0};
	uint8_t a1[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xc0};
	uint8_t a2[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xc1};
	uint8_t a3[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xc2};
	uint8_t a4[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xc3};
	uint8_t a5[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xc4};
	uint8_t aT[5] =	{0xe0, 0xf0, 0xf0, 0xf0, 0xf0};

//	uart.writeString("Set addresses\r\n");
	nrf.SetAddresses(a0,a1,a2,a3,a4,a5,aT);



	//	uart.writeString("Set Channel to 10\r\n");
		nrf.SetChannel(10);

	//	uart.writeString("Init... ");

		nrf.spi = SPI(DDRB,PORTB,PB4,PB3,PB5,PB2);
	//nrf24l01_init();
	nrf.Init();

	//	uart.writeString("... done \r\n");

}


void init_interupt(){

	/*
	 * Trigger INT1 on rising edge
	 * and INT0 on falling edge
	 */

	EICRA =  (1<<ISC10) | (1<<ISC11) | (1<<ISC01);

	// Enable INT0 and  INT1
	EIMSK = 1<<INT1 | 1<<INT0;


    // Start it up.
   sei();
}


ISR(INT0_vect){

	PORTD ^= (1<<PD4);
	// nrf receive

	//uart.writeString("Hi bro_0\n\r");
}

ISR(INT1_vect){

	PORTD ^= (1<<PD4);
	//uart.writeString("Hi bro\n\r");


	char dsbuffer[255];
	char prbuffer[255];
		//	ds.therm_read_DS18B20(dsbuffer);
	ds.GetSensorStringXML(dsbuffer);

	ds.GetSensorStringXML(dsbuffer);


	uart.writeString(dsbuffer);
	uart.writeString(prbuffer);




}
/*
ISR(USART_RX_vect){
// fifo_put



}

ISR(USART_UDRE_vect){
// fifo_get -> writeString()



}

*/
int main(){


	DDRD |= (1<<DDD4);

	PORTD |= (1<<PD4);

	// Set the pullup on PD3.
	PORTD = (1<<PORTD3);

	init_interupt();


	uart.initUART();


	delay_ms(500);
	PORTD ^= (1<<PD4);

	char* string = "\r\n############### START ###################\r\n";
	uart.writeString(string);



	SetupNRF();
	delay_ms(500);


	char s[500];
	nrf.PrintInfo(s);
	//nrf24l01_PrintNRFInfo(s);


	uart.writeString(s);
	//uart.USART0_Flush();
	//uart.writeString("");

		while(1){

			PORTD ^= (1<<PD4);

			delay_ms(500);

			uart.writeString(".\r");
			const char* s = uart.readString();

			uart.USART0_Flush();

			char* ss = "";
			sprintf(ss,"Input: %s _\r\n",s);
			uart.writeString(ss);




		}





return (0);
}

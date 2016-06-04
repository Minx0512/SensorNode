/*
 * AM2302.c
 *
 *  Created on: Apr 28, 2016
 *      Author: matthias
 */


#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <ctype.h>

#include "AM2302.h"
#include "usart.h"

int8_t getdata_AM2302(float *temperature, float *humidity){
	uint8_t bits[5];
	uint8_t i,j = 0;

	memset(bits, 0, sizeof(bits));

	//reset port
	reset_AM2302();
	cli();
	//send request
	AM2302_PIN_LOW(); //low
	_delay_ms(1);
	AM2302_PIN_HIGH(); //high
	AM2302_PIN_INPUT_MODE(); //input
	_delay_us(40);

	//check start condition 1
	if((AM2302_HIGH_INPUT())) {
		return -1;
	}
	_delay_us(80);
	//check start condition 2
	if(!(AM2302_HIGH_INPUT())) {
		return -1;
	}
	_delay_us(80);

	//read the data
	uint16_t timeoutcounter = 0;
	for (j=0; j<5; j++) { //read 5 byte
		uint8_t result=0;
		for(i=0; i<8; i++) {//read every bit
			timeoutcounter = 0;
			while(!(AM2302_HIGH_INPUT())) { //wait for an high input (non blocking)
				timeoutcounter++;
				if(timeoutcounter > AM2302_TIMEOUT) {
					return -1; //timeout
				}
			}
			_delay_us(30);
			if(AM2302_HIGH_INPUT()) //if input is high after 30 us, get result
				result |= (1<<(7-i));
			timeoutcounter = 0;
			while(AM2302_HIGH_INPUT()) { //wait until input get low (non blocking)
				timeoutcounter++;
				if(timeoutcounter > AM2302_TIMEOUT) {
					return -1; //timeout
				}
			}
		}
		bits[j] = result;
	}

	//reset port
	reset_AM2302();
sei();
	//check checksum
	if ((uint8_t)(bits[0] + bits[1] + bits[2] + bits[3]) == bits[4]) {
		//return temperature and humidity

		uint16_t rawhumidity = bits[0]<<8 | bits[1];
		uint16_t rawtemperature = bits[2]<<8 | bits[3];
		if(rawtemperature & 0x8000) {
			*temperature = (float)((rawtemperature & 0x7FFF) / 10.0) * -1.0;
		} else {
			*temperature = (float)(rawtemperature)/10.0;
		}
		*humidity = (float)(rawhumidity)/10.0;

		return 0;
	}

	return -1;




}


void reset_AM2302(){

//reset port
	AM2302_PIN_OUTPUT_MODE(); //output
	AM2302_PIN_HIGH(); //high
	_delay_ms(100);


}
uint8_t send_request_AM2302(uint8_t bit){
	//send request
		AM2302_PIN_LOW(); //low
		_delay_ms(1);
		AM2302_PIN_HIGH(); //high
		AM2302_PIN_INPUT_MODE(); //input
		_delay_us(40);

		//check start condition 1
		if((AM2302_HIGH_INPUT())) {
			return -1;
		}
		_delay_us(80);
		//check start condition 2
		if(!(AM2302_HIGH_INPUT())) {
			return -1;
		}
		_delay_us(80);
	return 0;
}

int8_t gettemperaturehumidity(float *temperature, float *humidity) {
	return getdata_AM2302(temperature, humidity);
}

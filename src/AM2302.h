/*
 * AM2302.h
 *
 *  Created on: Apr 28, 2016
 *      Author: matthias
 */

#ifndef AM2302_H_
#define AM2302_H_

#include <stdio.h>
#include <avr/io.h>

#include <stdlib.h>
#include <ctype.h>
#include "usart.h"
#include <inttypes.h>

#ifndef FOSC
#define	FOSC			16000000UL
#endif
#define LOOP_CYCLES  8  //Number of cycles that the loop takes
#define us(num) (num/(LOOP_CYCLES*(1/(FOSC/1000000.0))))


/* Thermometer Connections (At your choice) */
#define AM2302_WIRE_PORT PORTD
#define AM2302_WIRE_DDR DDRD
#define AM2302_WIRE_PIN PIND
#define AM2302_WIRE_DQ PD7
#define AM2302_TIMEOUT 200

/* Utils */
#define AM2302_PIN_INPUT_MODE() AM2302_WIRE_DDR&=~(1<<AM2302_WIRE_DQ)
#define AM2302_PIN_OUTPUT_MODE() AM2302_WIRE_DDR|=(1<<AM2302_WIRE_DQ)
#define AM2302_PIN_LOW() AM2302_WIRE_PORT&=~(1<<AM2302_WIRE_DQ)
#define AM2302_PIN_HIGH() AM2302_WIRE_PORT|=(1<<AM2302_WIRE_DQ)
#define AM2302_HIGH_INPUT() AM2302_WIRE_PIN & (1<<AM2302_WIRE_DQ)



int8_t getdata_AM2302(float *temperature, float *humidity);

extern int8_t gettemperaturehumidity(float *temperature, float *humidity);

void reset_AM2302();
uint8_t send_request_AM2302(uint8_t bit);

#endif /* AM2302_H_ */

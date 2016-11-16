/*
 * DHT22.h
 *
 *  Created on: Nov 12, 2016
 *      Author: matthias
 */

#ifndef DHT22_H_
#define DHT22_H_

#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <ctype.h>
#include <inttypes.h>



#ifndef FOSC
#define	FOSC			16000000UL
#endif
#define LOOP_CYCLES  8  //Number of cycles that the loop takes
#define us(num) (num/(LOOP_CYCLES*(1/(FOSC/1000000.0))))

/* Utils */
#define AM2302_PIN_INPUT_MODE() AM2302_WIRE_DDR&=~(1<<AM2302_WIRE_DQ)
#define AM2302_PIN_OUTPUT_MODE() AM2302_WIRE_DDR|=(1<<AM2302_WIRE_DQ)
#define AM2302_PIN_LOW() AM2302_WIRE_PORT&=~(1<<AM2302_WIRE_DQ)
#define AM2302_PIN_HIGH() AM2302_WIRE_PORT|=(1<<AM2302_WIRE_DQ)
#define AM2302_HIGH_INPUT() AM2302_WIRE_PIN & (1<<AM2302_WIRE_DQ)




class DHT22 {

/* Thermometer Connections (At your choice) */
	uint8_t AM2302_WIRE_PORT;
	uint8_t AM2302_WIRE_DDR;
	uint8_t AM2302_WIRE_PIN;
	uint8_t AM2302_WIRE_DQ;

	uint8_t TIMEOUT;



public:

	DHT22();
	DHT22(uint8_t Wire_PORT,uint8_t Wire_DDR, uint8_t Wire_PIN,uint8_t Wire_DQ);


	int8_t GetTemperatureHumidity(float *temperature, float *humidity);

	int8_t GetTemperature(float *temperature);

	int8_t GetHumidity(float *humidity);


private:




	void Reset();
	uint8_t SendRequest(uint8_t bit);
	int8_t GetData(float *temperature, float *humidity);

};

#endif /* DHT22_H_ */

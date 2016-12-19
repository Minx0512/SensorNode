/*
 * DHT22.cpp
 *
 *  Created on: Nov 12, 2016
 *      Author: matthias
 */

#include "DHT22.h"
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <ctype.h>

DHT22::DHT22() {


	// PORTD , DDRD , PIND , PD7
	AM2302_WIRE_PORT = &PORTD;
	AM2302_WIRE_DDR = &DDRD;
	AM2302_WIRE_PIN = &PIND;
	AM2302_WIRE_DQ = PD7;
	TIMEOUT=200;



}


DHT22::DHT22(volatile uint8_t &Wire_PORT, volatile uint8_t &Wire_DDR, volatile uint8_t &Wire_PIN,uint8_t Wire_DQ){

	/* Thermometer Connections (At your choice) */
	// PORTD , DDRD , PIND , PD7

	AM2302_WIRE_PORT = &Wire_PORT;
	AM2302_WIRE_DDR = &Wire_DDR;
	AM2302_WIRE_PIN = &Wire_PIN;
	AM2302_WIRE_DQ = Wire_DQ;

	TIMEOUT=200;


}

int8_t DHT22::GetTemperatureHumidity(float *temperature, float *humidity){
	return GetData(temperature, humidity);
}

int8_t DHT22::GetTemperature(float *temperature){
	float humidity = 0;
	return GetData(temperature, &humidity);
}

int8_t DHT22::GetHumidity(float *humidity){
	float temperature = 0;
	return GetData(&temperature, humidity);
}

void DHT22::Reset(){

	//reset port
		AM2302_PIN_OUTPUT_MODE(); //output
		AM2302_PIN_HIGH(); //high
		_delay_ms(100);

}
uint8_t DHT22::SendRequest(uint8_t bit){
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
int8_t DHT22::GetData(float *temperature, float *humidity){
	uint8_t bits[5];
	uint8_t i,j = 0;

	memset(bits, 0, sizeof(bits));

		//reset port
		Reset();
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
					if(timeoutcounter > TIMEOUT) {
						return -1; //timeout
					}
				}
				_delay_us(30);
				if(AM2302_HIGH_INPUT()) //if input is high after 30 us, get result
					result |= (1<<(7-i));
				timeoutcounter = 0;
				while(AM2302_HIGH_INPUT()) { //wait until input get low (non blocking)
					timeoutcounter++;
					if(timeoutcounter > TIMEOUT) {
						return -1; //timeout
					}
				}
			}
			bits[j] = result;
		}

		//reset port
		Reset();
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
void DHT22::GetSensorStringXML(char* string){

	float temp = 0;
	float humidity = 0;
	char tempBuff[20];
	char humBuff[20];
	GetTemperatureHumidity(&temp,&humidity);
	dtostrf(temp,3,1,tempBuff);
	dtostrf(humidity,3,1,humBuff);

	sprintf(string,"<DHT22><T unit='°C'>%s</T><H unit='%%RH'>%s</H></DHT22>\r\n",tempBuff,humBuff);


}
void DHT22::GetSensorTemperatureStringXML(char* string){
	float temp = 0;
	float humidity = 0;
	char tempBuff[5];

	GetTemperatureHumidity(&temp,&humidity);
	dtostrf(temp,3,1,tempBuff);


	sprintf(string,"<DHT22><T unit='°C'>%s</T></DHT22>\r\n",tempBuff);




}

void DHT22::GetSensorHumidityStringXML(char* string){

	float temp = 0;
	float humidity = 0;

	char humBuff[5];
	GetTemperatureHumidity(&temp,&humidity);

	dtostrf(humidity,3,1,humBuff);

	sprintf(string,"<DHT22><H unit='%%RH'>%s</H></DHT22>\r\n",humBuff);



}


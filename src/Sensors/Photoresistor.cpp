/*
 * Photoresistor.cpp
 *
 *  Created on: Nov 12, 2016
 *      Author: matthias
 */

#include "Photoresistor.h"
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Photoresistor::Photoresistor() {
	// TODO Auto-generated constructor stub
	SetType(0);
	SetPort(0x00);
	Init();

}

Photoresistor::Photoresistor(uint8_t port) {
	// TODO Auto-generated constructor stub
	SetType(0);
	SetPort(port);
	Init();


}


void Photoresistor::Init(){

		ADCSRA = (1<< ADEN) | (1<<ADPS1) | (1<<ADPS2);
		ADMUX = ADPort; //use Port ADCx
		ADMUX |= (1<<REFS0);


}

void Photoresistor::SetPort(uint8_t port){
	this->ADPort = port;

}

void Photoresistor::SetType(uint8_t t){
	this->type = t;

}


uint16_t Photoresistor::readADValue(){
		uint16_t value = 0;
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		value = ADCW;
		return value;
}


void Photoresistor::GetSensorStringXML(char* string){

	float valueL = 0;

	for(int i=0;i<1000;i++){
		valueL+=(float)readADValue();
	}

	valueL/=1000;

	char str[10];
	dtostrf(valueL,4,3,str);

	sprintf(string,"<NSL19M51>%s</NSL19M51>\n\r",str);




}


/*
 * Photoresistor.cpp
 *
 *  Created on: Nov 12, 2016
 *      Author: matthias
 */

#include "Photoresistor.h"
#include <avr/io.h>
#include <stdio.h>



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

	uint16_t valueL = readADValue();

	sprintf(string,"<NSL19M51>%i</NSL19M51>\n\r",valueL);




}


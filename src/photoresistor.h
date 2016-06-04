/*
 * photoresistor.h
 *
 *  Created on: Apr 24, 2016
 *      Author: matthias
 */

#ifndef PHOTORESISTOR_H_
#define PHOTORESISTOR_H_


	void InitAD(uint8_t ADPort){

		ADCSRA = (1<< ADEN) | (1<<ADPS1) | (1<<ADPS2);
		ADMUX = ADPort; //use Port ADCx
		ADMUX |= (1<<REFS0);


	}


	uint16_t readADValue(){
		uint16_t value = 0;
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		value = ADCW;
		return value;
	}



#endif /* PHOTORESISTOR_H_ */

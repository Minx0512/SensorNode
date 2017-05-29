/**
 ****************************************************************************
 *
 * Project: SensorNode
 * 
 * I2C.cpp
 *
 * Created on: 28 Apr 2017
 *
 * Author: matthias
 *
 * Revision: 
 *
 ****************************************************************************
 License:

 **************************************************************************/
/** \file I2C.cpp
 * 
 */

#include <avr/io.h>
#include <util/twi.h>

#include "I2C.hpp"




I2C::I2C() {
	// TODO Auto-generated constructor stub



}

void I2C::Init(void){

	TWBR = (uint8_t)TWBR_val;
}

uint8_t I2C::Start(uint8_t address){
		// reset TWI control register
		TWCR = 0;
		// transmit START condition
		TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
		// wait for end of transmission
		while( !(TWCR & (1<<TWINT)) );

		// check if the start condition was successfully transmitted
		if((TWSR & 0xF8) != TW_START){ return (1); }

		// load slave address into data register
		TWDR = address;
		// start transmission of address
		TWCR = (1<<TWINT) | (1<<TWEN);
		// wait for end of transmission
		while( !(TWCR & (1<<TWINT)) );

		// check if the device has acknowledged the READ / WRITE mode
		uint8_t twst = TW_STATUS & 0xF8;
		if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return (1);

		return (0);

}

uint8_t I2C::Write(uint8_t data){
	// load data into data register
		TWDR = data;
		// start transmission of data
		TWCR = (1<<TWINT) | (1<<TWEN);
		// wait for end of transmission
		while( !(TWCR & (1<<TWINT)) );

		if( (TWSR & 0xF8) != TW_MT_DATA_ACK ){ return (1); }

		return (0);
}
uint8_t I2C::Read_ack(void){
	// start TWI module and acknowledge data after reception
		TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
		// wait for end of transmission
		while( !(TWCR & (1<<TWINT)) );
		// return received data from TWDR
		return (TWDR);

}

uint8_t I2C::Read_nack(void){
	// start receiving without acknowledging reception
		TWCR = (1<<TWINT) | (1<<TWEN);
		// wait for end of transmission
		while( !(TWCR & (1<<TWINT)) );
		// return received data from TWDR
		return (TWDR);

}

uint8_t I2C::Transmit(uint8_t address, uint8_t* data, uint16_t length){

	if (Start(address | I2C_WRITE)) return (1);

	for (uint16_t i = 0; i < length; i++){
		if (Write(data[i])) return (1);
	}

	Stop();

	return (0);

}

uint8_t I2C::Receive(uint8_t address, uint8_t* data, uint16_t length){

	if (Start(address | I2C_READ)) return (1);

	for (uint16_t i = 0; i < (length-1); i++){
		data[i] = Read_ack();
	}

	data[(length-1)] = Read_nack();

	Stop();

	return (0);

}

uint8_t I2C::WriteReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length){

	if (Start(devaddr | 0x00)) return (1);

	Write(regaddr);

	for (uint16_t i = 0; i < length; i++){
		if (Write(data[i])) return (1);
	}

	Stop();

	return (0);

}

uint8_t I2C::ReadReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length){
	if (Start(devaddr)) return (1);

	Write(regaddr);

	if (Start(devaddr | 0x01)) return (1);

	for (uint16_t i = 0; i < (length-1); i++){
		data[i] = Read_ack();
	}

	data[(length-1)] = Read_nack();

	Stop();

	return (0);

}
void I2C::Stop(void){
	// transmit STOP condition
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

}




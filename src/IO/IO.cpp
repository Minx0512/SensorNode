/**
 ****************************************************************************
 *
 * Project: SensorNode
 * 
 * IO.cpp
 *
 * Created on: 25 May 2017
 *
 * Author: matthias
 *
 * Revision: 
 *
 ****************************************************************************
 License:

 **************************************************************************/
/** \file IO.cpp
 * 
 */

#include "IO.hpp"

#include "I2C.hpp"
#include "USART.h"
#include "SPI.h"

//namespace IO {

IO::IO() : uartRX_flag(0) {
	// TODO Auto-generated constructor stub


	spi = SPI();

	usart = USART();
	i2c = I2C();


}


void IO::USART_SetRxFlagTrue(){
	uartRX_flag = true;
}

void IO::USART_SetRxFlagFalse(){
	uartRX_flag = false;
}

bool IO::USART_GetRxFlag(){
	return (uartRX_flag);
}


void IO::InitInterfaces(void){



	spi.Init();

	usart.initUART();

	i2c.Init();



}


void IO::USART_writeString(char *str){

	usart.writeString(str);
}

void IO::USART_readString(char* str){
	usart.readString(str);
}

void IO::USART_readString(char *myString,uint8_t maxLength){

	char rStr[maxLength+1];

	usart.readString(rStr,maxLength);
	for(uint8_t i=0;i<maxLength;i++){
		myString[i] = rStr[i];
	}
}

void IO::USART_Flush(){
	usart.USART0_Flush();
}

uint8_t IO::SPI_WriteReadbyte(uint8_t data){

	return (spi.WriteReadbyte(data));

}


//}

/*
 * DS18B20.h
 *
 *  Created on: Nov 12, 2016
 *      Author: matthias
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <ctype.h>
#include <inttypes.h>

//#include "../IO/USART.h"


#ifndef FOSC
#define	FOSC			16000000UL
#endif
#define LOOP_CYCLES  8  //Number of cycles that the loop takes
#define us(num) (num/(LOOP_CYCLES*(1/(FOSC/1000000.0))))

#define THERM_CMD_CONVERTTEMP 0x44
#define THERM_CMD_RSCRATCHPAD 0xBE
#define THERM_CMD_WSCRATCHPAD 0x4E
#define THERM_CMD_CPYSCRATCHPAD 0x48
#define THERM_CMD_RECEEPROM 0xb8
#define THERM_CMD_RPWRSUPPLY 0xb4

#define THERM_CMD_READROM 0x33
#define THERM_CMD_ALARMSEARCH 0xec
#define THERM_DECIMAL_STEPS_12BIT 625 //.0625

#define MATCH_ROM	0x55
#define SKIP_ROM	0xCC
#define	SEARCH_ROM	0xF0

#define	SEARCH_FIRST	0xFF		// start new search
#define	PRESENCE_ERR	0xFF
#define	DATA_ERR	0xFE
#define LAST_DEVICE	0x00		// last device found







class DS18B20 {

	/* Thermometer Connections (At your choice) */
	volatile uint8_t* WIRE_PORT;
	volatile uint8_t* WIRE_DDR;
	volatile uint8_t* WIRE_PIN;
	uint8_t WIRE_DQ;
	char MACString[120];


public:

// USART u;




	void ReadMeasurement( char* string );
	void ReadMeasurement2( char* Macstring, char* tempStr );
	DS18B20();
	DS18B20(volatile uint8_t &Wire_PORT, volatile uint8_t &Wire_DDR, volatile uint8_t &Wire_PIN,uint8_t Wire_DQ);


	//void therm_read_DS18B20(char*buffer);
	void GetMACString(char* MACString);
	void GetTemperatureString(char* TemperatureString);
	void GetSensorStringXML(char* string);
	void GetTemperature(char* MACstring,char* tempString);


private:




	void StartMeasurement( void );
	uint8_t Reset();
	//uint8_t Reset(char* string);
	uint8_t BitIO(uint8_t bit);
	uint8_t WriteByte(uint8_t byte);
	uint8_t ReadByte(void);
	unsigned char RomSearch(unsigned char diff, unsigned char *id);
	void Command( unsigned char command, unsigned char *id );


	void PIN_INPUT_MODE(){ *WIRE_DDR&=~(1<<WIRE_DQ);}
	void PIN_OUTPUT_MODE(){ *WIRE_DDR|=(1<<WIRE_DQ);}
	void PIN_LOW(){ *WIRE_PORT&=~(1<<WIRE_DQ);}
	void PIN_HIGH(){*WIRE_PORT|=(1<<WIRE_DQ);}

	inline __attribute__((gnu_inline)) void delay_us(uint16_t delay){
		  	 while(delay--) asm volatile("nop");
		  }

};

#endif /* DS18B20_H_ */

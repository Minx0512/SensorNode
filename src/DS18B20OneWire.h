/*
 * DS18B20OneWire.h
 *
 *  Created on: Apr 28, 2016
 *      Author: matthias
 */

#ifndef DS18B20ONEWIRE_H_
#define DS18B20ONEWIRE_H_


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

/* Thermometer Connections (At your choice) */
#define WIRE_PORT PORTB
#define WIRE_DDR DDRB
#define WIRE_PIN PINB
#define WIRE_DQ PB0
/* Utils */
#define PIN_INPUT_MODE() WIRE_DDR&=~(1<<WIRE_DQ)
#define PIN_OUTPUT_MODE() WIRE_DDR|=(1<<WIRE_DQ)
#define PIN_LOW() WIRE_PORT&=~(1<<WIRE_DQ)
#define PIN_HIGH() WIRE_PORT|=(1<<WIRE_DQ)


//void therm_read_DS18B20(char*buffer);
void start_meas_DS18B20( void );
void read_meas_DS18B20( char* string );

inline __attribute__((gnu_inline)) void delay_us(uint16_t delay);

uint8_t wire_reset_DS18B20();
uint8_t wire_bit_io_DS18B20(uint8_t bit);

uint8_t wire_write_byte_DS18B20(uint8_t byte);

uint8_t wire_read_byte_DS18B20(void);

unsigned char wire_rom_search_DS18B20(unsigned char diff, unsigned char *id);

void wire_commandDS18B20( unsigned char command, unsigned char *id );

#endif /* DS18B20ONEWIRE_H_ */

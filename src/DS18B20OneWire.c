/*
 * DS18B20OneWire.c
 *
 *  Created on: Apr 28, 2016
 *      Author: matthias
 */


#include <inttypes.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "DS18B20OneWire.h"
#include "usart.h"



 inline __attribute__((gnu_inline)) void delay_us(uint16_t delay){
	 while(delay--) asm volatile("nop");
 }

 uint8_t wire_reset_DS18B20(){
 uint8_t i;
 	 //Pull line low and wait for 480uS
 	 PIN_LOW();
 	 PIN_OUTPUT_MODE();
 	 delay_us(us(480));
 	 cli();
 	 //Release line and wait for 60uS
 	 PIN_INPUT_MODE();
 	 delay_us(us(60));
 	 //Store line value and wait until the completion of 480uS period
 	 i=(WIRE_PIN & (1<<WIRE_DQ));
 	 sei();

 	 delay_us(us(420));
 	 //Return the value read from the presence pulse (0=OK, 1=WRONG)
 	if( (WIRE_PIN & (1<<WIRE_DQ)) == 0 )		// short circuit
 	    i = 1;

 return i;

}
 uint8_t wire_bit_io_DS18B20(uint8_t bit){

	 //Pull line low for 1uS
	 cli();
	 PIN_LOW();
	 PIN_OUTPUT_MODE();
	 delay_us(us(1));

	 //If we want to write 1, release the line (if not will keep low)
	 if(bit) PIN_INPUT_MODE();
	 delay_us(15);
	 if( (WIRE_PIN & (1<<WIRE_DQ)) == 0 )
		 bit=0;
	 //Wait for 60uS and release the line
	 delay_us(us(60));
	 PIN_INPUT_MODE();
	sei();
return bit;
}


uint8_t wire_write_byte_DS18B20(uint8_t byte){
 uint8_t i=8, j;

 	 do{
 		//Write actual bit and shift one position right to make the next bit ready
 		j = wire_bit_io_DS18B20(byte&1);
 		 byte>>=1;
 		 if(j)
 			byte |= 0x80;

 	 }while(--i);

return byte;
}

uint8_t wire_read_byte_DS18B20(void){
	 return wire_write_byte_DS18B20( 0xFF );
}

unsigned char wire_rom_search_DS18B20(unsigned char diff, unsigned char *id){

	unsigned char i, j, next_diff;
	unsigned char b;

	  if( wire_reset_DS18B20() )
	    return PRESENCE_ERR;			// error, no device found
	  wire_write_byte_DS18B20( SEARCH_ROM );			// ROM search command
	  next_diff = LAST_DEVICE;			// unchanged on last device
	  i = 8 * 8;					// 8 bytes
	  do{
	    j = 8;					// 8 bits
	    do{
	      b = wire_bit_io_DS18B20( 1 );			// read bit
	      if( wire_bit_io_DS18B20( 1 ) ){			// read complement bit
		if( b )					// 11
		  return DATA_ERR;			// data error
	      }else{
		if( !b ){				// 00 = 2 devices
		  if( diff > i ||
		    ((*id & 1) && diff != i) ){
		    b = 1;				// now 1
		    next_diff = i;			// next pass 0
		  }
		}
	      }
	      wire_bit_io_DS18B20( b );     			// write bit
	      *id >>= 1;
	      if( b )					// store bit
		*id |= 0x80;
	      i--;
	    }while( --j );
	    id++;					// next byte
	  }while( i );
	  return next_diff;				// to continue search

}

void wire_commandDS18B20( unsigned char command, unsigned char *id ){
  unsigned char i;
  wire_reset_DS18B20();
  if( id ){
	  wire_write_byte_DS18B20( MATCH_ROM );			// to a single device
    i = 8;
    do{
    	wire_write_byte_DS18B20( *id );
      id++;
    }while( --i );
  }else{
	  wire_write_byte_DS18B20( SKIP_ROM );			// to all devices
  }
  wire_write_byte_DS18B20( command );
}



void start_meas_DS18B20( void ){
  if( WIRE_PIN & 1<< WIRE_DQ){
	  wire_commandDS18B20( THERM_CMD_CONVERTTEMP, NULL );
	WIRE_PORT |= 1<< WIRE_DQ;
    WIRE_DDR |= 1<< WIRE_DQ;			// parasite power on

  }else{
    ;
  }
}


void read_meas_DS18B20( char* string ){
  unsigned char id[8], diff;
  //unsigned char s[30];
  unsigned char i;
  unsigned int temp;

  for( diff = SEARCH_FIRST; diff != LAST_DEVICE; ){
    diff = wire_rom_search_DS18B20( diff, id );

    if( diff == PRESENCE_ERR ){
      writeString( "No Sensor found" );
      break;
    }
    if( diff == DATA_ERR ){
     writeString( "Bus Error" );
      break;
    }
    if( id[0] == 0x28 || id[0] == 0x10 ){	// temperature sensor
      writeString( "<DS18B20><ID>" );
      for( i = 0; i < 8; i++ ){
	sprintf( string, "%02X", id[i] );
	writeString( string );
      }
      wire_write_byte_DS18B20( THERM_CMD_RSCRATCHPAD );			// read command
      temp = wire_read_byte_DS18B20();			// low byte
      temp |= (unsigned int)wire_read_byte_DS18B20() << 8;		// high byte
     // temp-=0x20;
      if( id[0] == 0x10 )			// 9 -> 12 bit
        temp <<= 3;
   //   sprintf( string, "  %04X = ", temp );	// hex value
    //  writeString( string );
      sprintf( string, "</ID><T>");
      writeString( string );
      sprintf( string, "%d.%01d°C</T></DS18B20>\r\n", temp >> 4, (temp << 12) / 6553 ); // 0.1C
     writeString( string );
    }
  }
//writeString( "" );
}
/*
void therm_read_DS18B20(char*buffer){

	// Buffer length must be at least 12bytes long! ["+XXX.XXXX C"]
	uint8_t temperature[2];
	int8_t digit;
	uint16_t decimal;

	//Reset, skip ROM and start temperature conversion
	wire_reset_DS18B20();
	wire_write_byte_DS18B20(SKIP_ROM);
	wire_write_byte_DS18B20(THERM_CMD_CONVERTTEMP);

	//Wait until conversion is complete

//	while(!wire_read_bit());

	//Reset, skip ROM and send command to read Scratchpad
	wire_reset_DS18B20();
	wire_write_byte_DS18B20(SKIP_ROM);
	wire_write_byte_DS18B20(THERM_CMD_RSCRATCHPAD);

	//Read Scratchpad (only 2 first bytes)
	temperature[0]=wire_read_byte_DS18B20();
	temperature[1]=wire_read_byte_DS18B20();
	wire_reset_DS18B20();

	//Store temperature integer digits and decimal digits
	digit=temperature[0]>>4;
	digit|=(temperature[1]&0x7)<<4;

	//Store decimal digits
	decimal=temperature[0]&0xf;
	decimal*=THERM_DECIMAL_STEPS_12BIT;

	//Format temperature into a string [+XXX.XXXX C]

	sprintf(buffer, "%+d.%04u C", digit, decimal);


}
*/


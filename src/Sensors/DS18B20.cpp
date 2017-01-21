/*
 * DS18B20.cpp
 *
 *  Created on: Nov 12, 2016
 *      Author: matthias
 */

#include "DS18B20.h"
#include <util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//#include "../IO/USART.h"


DS18B20::DS18B20() {


	WIRE_PORT = &PORTB;

	WIRE_DDR = &DDRB;

	WIRE_PIN = &PINB;

	WIRE_DQ  = PB0;




//u = USART();


}


DS18B20::DS18B20(volatile uint8_t &Wire_PORT, volatile uint8_t &Wire_DDR, volatile uint8_t &Wire_PIN,uint8_t Wire_DQ){



	WIRE_PORT = &Wire_PORT;

	WIRE_DDR = &Wire_DDR;

	WIRE_PIN = &Wire_PIN;

	WIRE_DQ  = Wire_DQ;

	// u = USART();





}

void DS18B20::StartMeasurement( void ){
	 if( *WIRE_PIN & 1<< WIRE_DQ){
		 Command( THERM_CMD_CONVERTTEMP, NULL );
		*WIRE_PORT |= 1<< WIRE_DQ;
	    *WIRE_DDR |= 1<< WIRE_DQ;			// parasite power on

	  }else{
	    ;
	  }
}


void DS18B20::ReadMeasurement( char* tempStr ){

	unsigned char id[8], diff;

	sprintf(tempStr,"%s",(char*)"");
	sprintf(MACString,"%s",(char*)"");
	//unsigned char i;
	unsigned int temp;

	for( diff = SEARCH_FIRST; diff != LAST_DEVICE; ){
		diff = RomSearch( diff, id );

	    if( diff == PRESENCE_ERR ){
	    	sprintf(tempStr, "Error 0x00|" ); // No sensor found
	    	break;
	    }
	    if( diff == DATA_ERR ){
	    	sprintf(tempStr, "Error 0x10|" ); // Bus Error
	    	break;
	    }
	    if( id[0] == 0x28 || id[0] == 0x10 ){	// temperature sensor
	    	sprintf( MACString, "%s%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X|",MACString, id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7] );
	    	WriteByte( THERM_CMD_RSCRATCHPAD );			// read command
	    	temp = ReadByte();			// low byte
	    	temp |= (unsigned int)ReadByte() << 8;		// high byte
	    	// temp-=0x20;
	    	if( id[0] == 0x10 )			// 9 -> 12 bit
	    		temp <<= 3;
	    	//   sprintf( string, "  %04X = ", temp );	// hex value

	    	sprintf( tempStr, "%s%d.%01d°C|",tempStr, temp >> 4, (temp << 12) / 6553 ); // 0.1C

	    }

	  }


}

void DS18B20::ReadMeasurement2( char* Macstring, char* tempStr ){

	unsigned char id[8], diff;
	sprintf(Macstring,(char*)" ");
	sprintf(tempStr,(char*)" ");
	//unsigned char i;
	unsigned int temp;

	for( diff = SEARCH_FIRST; diff != LAST_DEVICE; ){
		diff = RomSearch( diff, id );

	    if( diff == PRESENCE_ERR ){
	    	sprintf(Macstring, "No Sensor found." );
	    	break;
	    }
	    if( diff == DATA_ERR ){
	    	sprintf(Macstring, "Bus Error." );
	    	break;
	    }
	    if( id[0] == 0x28 || id[0] == 0x10 ){	// temperature sensor
	    	sprintf( Macstring, "%s%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X|",Macstring, id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7] );
	    	WriteByte( THERM_CMD_RSCRATCHPAD );			// read command
	    	temp = ReadByte();			// low byte
	    	temp |= (unsigned int)ReadByte() << 8;		// high byte
	    	// temp-=0x20;
	    	if( id[0] == 0x10 )			// 9 -> 12 bit
	    		temp <<= 3;
	    	//   sprintf( string, "  %04X = ", temp );	// hex value

	    	sprintf( tempStr, "%s%d.%01d°C|",tempStr, temp >> 4, (temp << 12) / 6553 ); // 0.1C

	    }

	  }


}

uint8_t DS18B20::Reset(){
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
	i=(*WIRE_PIN & (1<<WIRE_DQ));
	sei();

	delay_us(us(420));
	//Return the value read from the presence pulse (0=OK, 1=WRONG)
	if( (*WIRE_PIN & (1<<WIRE_DQ)) == 0 )		// short circuit
	 i = 1;


	return i;

}
uint8_t DS18B20::BitIO(uint8_t bit){
	 //Pull line low for 1uS
		 cli();
		 PIN_LOW();
		 PIN_OUTPUT_MODE();
		 delay_us(us(1));

		 //If we want to write 1, release the line (if not will keep low)
		 if(bit) PIN_INPUT_MODE();
		 delay_us(15);
		 if( (*WIRE_PIN & (1<<WIRE_DQ)) == 0 )
			 bit=0;
		 //Wait for 60uS and release the line
		 delay_us(us(60));
		 PIN_INPUT_MODE();
		sei();
	return bit;
}
uint8_t DS18B20::WriteByte(uint8_t byte){
	uint8_t i=8, j;

	 	 do{
	 		//Write actual bit and shift one position right to make the next bit ready
	 		j = BitIO(byte&1);
	 		 byte>>=1;
	 		 if(j)
	 			byte |= 0x80;

	 	 }while(--i);

	return byte;
}
uint8_t DS18B20::ReadByte(void){
	 return WriteByte( 0xFF );
}
unsigned char DS18B20::RomSearch(unsigned char diff, unsigned char *id){
	unsigned char i, j, next_diff;
	unsigned char b;

	  if( Reset() )
	    return PRESENCE_ERR;			// error, no device found
	  WriteByte( SEARCH_ROM );			// ROM search command
	  next_diff = LAST_DEVICE;			// unchanged on last device
	  i = 8 * 8;					// 8 bytes
	  do{
	    j = 8;					// 8 bits
	    do{
	      b = BitIO( 1 );			// read bit
	      if( BitIO( 1 ) ){			// read complement bit
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
	      BitIO( b );     			// write bit
	      *id >>= 1;
	      if( b )					// store bit
		*id |= 0x80;
	      i--;
	    }while( --j );
	    id++;					// next byte
	  }while( i );
	  return next_diff;				// to continue search


}
void DS18B20::Command( unsigned char command, unsigned char *id ){
	unsigned char i;
	Reset();
	  if( id ){
		  WriteByte( MATCH_ROM );			// to a single device
	    i = 8;
	    do{
	    	WriteByte( *id );
	      id++;
	    }while( --i );
	  }else{
		  WriteByte( SKIP_ROM );			// to all devices
	  }
	  WriteByte( command );
}

void DS18B20::GetMACString(char* MACStringOut){
	sprintf(MACStringOut,"%s",MACString);
}

void DS18B20::GetTemperatureString(char* TemperatureString){


}

void DS18B20::GetSensorStringXML(char* string){

	//char str[255];
	StartMeasurement();
//	ReadMeasurement(str);
//_delay_ms(2000);
	ReadMeasurement(string);

}
void DS18B20::GetTemperature(char* MACstring,char* tempString){

	//char str[255];
	StartMeasurement();
//	ReadMeasurement(str);
//_delay_ms(2000);
	ReadMeasurement2( MACstring, tempString);

}


/*
 * main.cpp
 *
 *  Created on: Nov 12, 2016
 *      Author: matthias
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/eeprom.h>

//#include <avr/wdt.h>

#include "IO/USART.h"

#include "IO/ParseStrings.h"
#include "Sensors/DHT22.h"
#include "Sensors/DS18B20.h"
#include "Sensors/Photoresistor.h"



#include "Wireless/NRF24L01.h"

//#define F_CPU 16000000

#define CE PB1
#define CSN PB0


uint8_t flag1 = 0;


uint8_t firstrun EEMEM;

char btName[21] EEMEM;

//uint8_t pipeEnable EEMEM;
uint8_t nrfChannel EEMEM;
uint8_t nrfACK EEMEM;


uint8_t nRFPipe0[5] EEMEM;
uint8_t nRFPipe1[5] EEMEM;
uint8_t nRFPipe2[5] EEMEM;
uint8_t nRFPipe3[5] EEMEM;
uint8_t nRFPipe4[5] EEMEM;
uint8_t nRFPipe5[5] EEMEM;



USART uart = USART(9600);

char str[RX_BUFF];

DS18B20 ds = DS18B20(PORTD,DDRD,PIND, PD6);
DHT22 dht22 = DHT22(PORTD,DDRD,PIND,PD7);

Photoresistor pr = Photoresistor(0x00);

NRF24L01 nrf = NRF24L01(DDRB, PORTD, CE, CSN);

ParseStrings ps = ParseStrings();


void delay_ms(uint16_t ms){
	for(uint16_t t=0; t<=ms; t++){
		_delay_ms(1);
	}
}

void CpyMAC(uint8_t* src, uint8_t* dest, uint8_t len){

	for(uint8_t i=0;i<len;i++){

		dest[i] = src[i];

	}


}
/*
void SetupBT(){

	// Bluetooth
	uint8_t fr =eeprom_read_byte(&firstrun);

	char btname[] = "MinxSensorNode";

	if(fr == 0){
		// after chip erase

		eeprom_write_block(btname,btName,sizeof(btname));

	}else if(fr==1){

		eeprom_read_block(btname,btName,sizeof(btname));


	}

}
*/

void SetupNRF(){

	// standard mac addresses
	uint8_t  stdMACpipe0[] = {0xe0, 0xf0, 0xf0, 0xf0, 0xf0};
	uint8_t  stdMACpipe1[] = {0xe1, 0xf0, 0xf0, 0xf0, 0xc0};
	uint8_t  stdMACpipe2[] = {0xe1, 0xf0, 0xf0, 0xf0, 0xc1};
	uint8_t  stdMACpipe3[] = {0xe1, 0xf0, 0xf0, 0xf0, 0xc2};
	uint8_t  stdMACpipe4[] = {0xe1, 0xf0, 0xf0, 0xf0, 0xc3};
	uint8_t  stdMACpipe5[] = {0xe1, 0xf0, 0xf0, 0xf0, 0xc4};


	uint8_t nrfChn = 0x0A;
	uint8_t nrfAutoACK = 1;
	//uint8_t nrfPipesEnable = 0b00111111;



	uint8_t fr =eeprom_read_byte(&firstrun);

	if(fr == 0){
		// empty eeprom after chip erase

		// nRF config
		eeprom_update_block(stdMACpipe0,nRFPipe0,sizeof(nRFPipe0));
		eeprom_update_block(stdMACpipe1,nRFPipe1,sizeof(nRFPipe1));
		eeprom_update_block(stdMACpipe2,nRFPipe2,sizeof(nRFPipe2));
		eeprom_update_block(stdMACpipe3,nRFPipe3,sizeof(nRFPipe3));
		eeprom_update_block(stdMACpipe4,nRFPipe4,sizeof(nRFPipe4));
		eeprom_update_block(stdMACpipe5,nRFPipe5,sizeof(nRFPipe5));

		eeprom_write_byte(&firstrun,1);

		eeprom_write_byte(&nrfACK,nrfAutoACK);
		eeprom_write_byte(&nrfChannel,nrfChn);
	//	eeprom_write_byte(&pipeEnable,nrfPipesEnable);


	}else if(fr==1){
		// after restart

		// read nRF config
		eeprom_read_block (stdMACpipe0, nRFPipe0, sizeof(nRFPipe0));
		eeprom_read_block (stdMACpipe1, nRFPipe1, sizeof(nRFPipe1));
		eeprom_read_block (stdMACpipe2, nRFPipe2, sizeof(nRFPipe2));
		eeprom_read_block (stdMACpipe3, nRFPipe3, sizeof(nRFPipe3));
		eeprom_read_block (stdMACpipe4, nRFPipe4, sizeof(nRFPipe4));
		eeprom_read_block (stdMACpipe5, nRFPipe5, sizeof(nRFPipe5));

		nrfChn = eeprom_read_byte(&nrfChannel);
		nrfAutoACK = eeprom_read_byte(&nrfACK);
	//	nrfPipesEnable = eeprom_read_byte(&pipeEnable);




	}
	//	char tr[50];
	//	sprintf(tr,"FR: %i - MAC: %02X:%02X:%02X:%02X:%02X\r\n",fr,stdMACpipe1[0],stdMACpipe1[1],stdMACpipe1[2],stdMACpipe1[3],stdMACpipe1[4]);
	//	uart.writeString(tr);


	nrf.SetAddressPipe0(stdMACpipe0);
	nrf.SetAddressPipe1(stdMACpipe1);
	nrf.SetAddressPipe2(stdMACpipe2);
	nrf.SetAddressPipe3(stdMACpipe3);
	nrf.SetAddressPipe4(stdMACpipe4);
	nrf.SetAddressPipe5(stdMACpipe5);
	nrf.SetAddressPipeT(stdMACpipe0);

//	nrf.SetAutoAck(nrfAutoACK);
	nrf.SetChannel(nrfChn);


	//	uart.writeString("Init... ");

		nrf.spi = SPI(DDRB,PORTB,PB4,PB3,PB5,PB2);
	//nrf24l01_init();
	nrf.Init();

	//	uart.writeString("... done \r\n");

}



void init_interupt(){

	/*
	 * Trigger INT1 on rising edge
	 * and INT0 on falling edge
	 */

	EICRA =  (1<<ISC10) | (1<<ISC11) | (1<<ISC01);

	// Enable INT0 and  INT1
	EIMSK = 1<<INT1 | 1<<INT0;


    // Start it up.
  sei();
}


ISR(INT0_vect){

	PORTD ^= (1<<PD4);
	// nrf receive


}

ISR(INT1_vect){

	flag1 = 1;

}

uint8_t flag2 = 0;

ISR(USART_RX_vect){
// fifo_put
	PORTD ^= (1<<PD4);
	//const char* s = uart.readString();
	uart.readString(str,RX_BUFF);


char s[50];

sprintf(s," ");

sprintf(s,"<input>%s</input>\r\n",str);
//sprintf(str,"%s","");

uart.writeString(s);



flag2 = 1;

uart.USART0_Flush();



}
/*
ISR(USART_UDRE_vect){
    //if index is not at start of buffer

        //disable transmission and UDR0 empty interrupt
  //  TX_STOP();

	UCSR0B &= ~((1<<TXEN0)|(1<<UDRIE0));
        //enable reception and RC complete interrupt

        RX_START();
        RX_INTEN();
      //  UCSR0B |= (1<<RXEN0)|(1<<RXCIE0);

}

ISR(USART_UDRE_vect){
// fifo_get -> writeString()

	uart.writeString(str);

}
*/





int main(){


	DDRD |= (1<<DDD4);

	PORTD |= (1<<PD4);

	// Set the pullup on PD3.
	PORTD = (1<<PORTD3);

	init_interupt();


	uart.initUART();


	delay_ms(500);
	PORTD ^= (1<<PD4);

	char buffer[500];

	sprintf(buffer,"\r\n########### START #############\r\n");

	uart.writeString(buffer);

	sprintf(buffer, " ");

	SetupNRF();
	delay_ms(500);


	//char s[50];
	nrf.PrintInfo(buffer);
	uart.writeString(buffer);

	sprintf(buffer,"");


		while(1){

			PORTD ^= (1<<PD4);

			delay_ms(500);

			if(flag2){

				//char buffer[500];

//				uart.writeString("String: ");
//				uart.writeString(str);
//				uart.writeString("\r\n");

				ps.Parse(str);

			//	ps.PrintVars(buffer);





				if(ps.getCmdID()==1){ // configure Bluetooth





				}else if(ps.getCmdID()==2){ // configure nRF24L01


					if(ps.getCmdProperty()==0){ // get MAC address for pipe


						ps.ParseMAC();

						uint8_t addr[5];
						ps.getMAC(addr);

						if(ps.getPropertyID()==0){ // pipe 0 and T

							eeprom_write_block(addr,nRFPipe0,sizeof(addr));

							nrf.SetAddressPipe0(addr);
							nrf.SetAddressPipeT(addr);
							//CpyMAC(addr,a0,5);
							//CpyMAC(addr,aT,5);

//							char sb[50];
//							sprintf(sb," ");
//							sprintf(sb,"MAC a0: %02X:%02X:%02X:%02X:%02X\r\n",a0[0],a0[1],a0[2],a0[3],a0[4]);
//							strcat(buffer,sb);

							//a0 = ps.getMAC();

						}else if(ps.getPropertyID()==1){ // pipe 1

							eeprom_write_block(addr,nRFPipe1,sizeof(addr));

						//	strcat(buffer,"wrote block\r\n");

							nrf.SetAddressPipe1(addr);
							//CpyMAC(addr,a1,5);

						}else if(ps.getPropertyID()==2){ // pipe 2
							eeprom_update_block(addr,nRFPipe2,sizeof(addr));
							nrf.SetAddressPipe2(addr);
						//	CpyMAC(addr,a2,5);

						}else if(ps.getPropertyID()==3){ // pipe 3
							eeprom_update_block(addr,nRFPipe3,sizeof(addr));
							nrf.SetAddressPipe3(addr);
							//CpyMAC(addr,a3,5);

						}else if(ps.getPropertyID()==4){ // pipe 4
							eeprom_update_block(addr,nRFPipe4,sizeof(addr));
							nrf.SetAddressPipe4(addr);
						//	CpyMAC(addr,a4,5);

						}else if(ps.getPropertyID()==5){ // pipe 5
							eeprom_update_block(addr,nRFPipe5,sizeof(addr));
							nrf.SetAddressPipe5(addr);
							//CpyMAC(addr,a5,5);

						}




					//	ps.PrintMAC(buffer);

					}else if(ps.getCmdProperty()==1){ // Enable pipe


					//	uint8_t c = 0b00111111;

					//	eeprom_update_byte(&pipeEnable,c);


						nrf.EnablePipe(ps.getPropertyID());


					}else if(ps.getCmdProperty()==2){ // Channel

						eeprom_update_byte(&nrfChannel,ps.getPropertyID());

						nrf.SetChannel(ps.getPropertyID());

					}else if(ps.getCmdProperty()==3){ // PowerLevel

						nrf.SetPowerLevel(ps.getPropertyID());

					}else if(ps.getCmdProperty()==4){ // Data rate

						nrf.SetDataRate(ps.getPropertyID());

					}else if(ps.getCmdProperty()==5){ // CRC level

						nrf.SetCRClength(ps.getPropertyID());

					}else if(ps.getCmdProperty()==6){ // Payload size

						nrf.SetPayloadSize(ps.getPropertyID());

					}else if(ps.getCmdProperty()==7){ // Auto ACK

						eeprom_update_byte(&nrfACK,ps.getPropertyID());

						nrf.SetAutoAck(ps.getPropertyID());

					}else if(ps.getCmdProperty()==8){ // Reset

						//nrf.SetAddresses(a0,a1,a2,a3,a4,a5,aT);
						//nrf.Init();
						eeprom_write_byte(&firstrun,0);




					}




				}else if(ps.getCmdID()==3){ // get Sensor data


					if(ps.getCmdProperty()==0){ // TempDHT22
						char dh[50];
						dht22.GetSensorTemperatureStringXML(dh);
						strcat(buffer,dh);


					}else if(ps.getCmdProperty()==1){ // HumidityDHT22

						char dh[50];
						dht22.GetSensorHumidityStringXML(dh);
						strcat(buffer,dh);


					}else if(ps.getCmdProperty()==2){ // TempHumidityDHT22

						char dh[50];
						dht22.GetSensorStringXML(dh);
						strcat(buffer,dh);


					}else if(ps.getCmdProperty()==3){ //Lightsense

						char prbuffer[50];
						pr.GetSensorStringXML(prbuffer);
						strcat(buffer,prbuffer);

					}else if(ps.getCmdProperty()==4){ //TempDS18B20

						char dsb[50];
						ds.GetSensorStringXML(dsb);
						strcat(buffer,dsb);



					}else if(ps.getCmdProperty()==5){ //Movement

						if(flag1){
							strcat(buffer,"<mv>1</mv>\r\n");
						}else{
							strcat(buffer,"<mv>0</mv>\r\n");

						}



					}else if(ps.getCmdProperty()==6){ // Remote Sensors

					}



				}



				strcat(buffer,"|end\r\n");
				uart.writeString(buffer);
				sprintf(buffer,"\r");
				sprintf(str," ");


					//uart.writeString(buffer);

					flag2 = 0;

			}



			if(flag1){


				//strcat(buffer,"<mv>1</mv>|end\r\n");
				uart.writeString("<mv>1</mv>|end\r\n");


				flag1=0;
			}



		}

		PORTD ^= (1<<PD4);



return (0);
}

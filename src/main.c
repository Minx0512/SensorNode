/*
 * main.c
 *
 *  Created on: Apr 16, 2016
 *      Author: matthias
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usart.h"
#include "photoresistor.h"
#include "DS18B20OneWire.h"
#include "AM2302.h"

//include nrf24l01
#include "nrf24l01/nrf24l01.h"
#include "nrf24l01/nrf24l01registers.h"

//role definitions
#define ROLETX 1
#define ROLERX 0

//role button/pin
#define BUTTONROLE_DDR DDRC
#define BUTTONROLE_PORT PORTC
#define BUTTONROLE_PIN PINC
#define BUTTONROLE PC0
//send button
#define BUTTONSEND_DDR DDRC
#define BUTTONSEND_PORT PORTC
#define BUTTONSEND_PIN PINC
#define BUTTONSEND PC1
//output led
#define LEDOUT_DDR DDRC
#define LEDOUT_PORT PORTC
#define LEDOUT PC5
#define LEDOUTPAUSE 300

int enableMoveSens = 1;
int movement = 0;
int prThresh = 550;
int time = 0;

volatile int rf = 0;




void delay_ms(uint16_t ms){
	for(uint16_t t=0; t<=ms; t++){
		_delay_ms(1);
	}
}



void evalLightsensor(){

	int valueL = readADValue();
	char string[256];
	//char st[256];
	itoa(valueL,string,10);
	//strcat(st,"<L>");
	//strcat(st,string);
	//writeString("<L>");
	//writeString("### Photoresistor NSL19M51 ### \r\n");
	writeString("<NSL19M51>");
	writeString(string);
	writeString("</NSL19M51>");



}

void evalTempSensorDS18B20(){

	char buffer[256];
	//writeString("### Temperature DS18B20 ### \r\n");
	//therm_read_temperature(buffer);
	start_meas_DS18B20();
	//writeString("<DS18B20>\r\n");
	read_meas_DS18B20(buffer);
	//writeString("</DS18B20>\r\n");





}

void evalHumidSensor(){

	float temp = 0;
	float humidity = 0;
	char buffer[256];

	if(gettemperaturehumidity(&temp,&humidity)!=-1){

		dtostrf(temp,3,1,buffer);
		//itoa(temp,buffer,10);
	//	writeString("### Temperature and Humidity DHT22/AM2302 ###\r\n");
		writeString("<DHT22>");
		writeString("<T>");
		writeString(buffer);
		writeString("°C</T>");
		dtostrf(humidity,3,1,buffer);
		writeString("<H>");
		writeString(buffer);
		writeString("%RH</H></DHT22>");

	}


}

ISR(INT0_vect){


rf = 1;
	//rf_interrupt = true;
//	writeString("INT0\r\n");



}

ISR(INT1_vect){


	PORTD ^= (1<<PD4);
	char buff[100];
	sprintf(buff,"STATUS: %02X\r\n", nrf24l01_getstatus()); writeString(buff);
	sprintf(buff,"RX_PW_P0: %02X\r\n",nrf24l01_readregister(NRF24L01_REG_RX_PW_P0)); writeString(buff);
	sprintf(buff,"RX_PW_P1: %02X\r\n",nrf24l01_readregister(NRF24L01_REG_RX_PW_P1)); writeString(buff);
	evalHumidSensor();
	writeString("\r\n");
	evalLightsensor();
	writeString("\r\n");

}




void v(int time,int movement, int enableMoveSens){

	char t[128];
	sprintf(t,"<time>%i</time>",time);
	writeString(t);
	char mv[128];
	sprintf(mv,"<mv>%i</mv>\r\n",movement);
	writeString(mv);
	char mvT[128];
	sprintf(mvT,"<mvSen>%i</mvSen>\r\n",enableMoveSens);
	writeString(mvT);

	//	writeString(string);

	evalLightsensor();
	writeString("\r\n");
	evalHumidSensor();
	writeString("\r\n");
	evalTempSensorDS18B20();
	//	writeString("\r\n");
	writeString("|end");
	movement=0;


}

void printNRF24L01(){
	char buff[100];
	writeString("##### NRF24L01+ Info #####\r\n");
	sprintf(buff,"STATUS: %02X\r\n", nrf24l01_getstatus()); writeString(buff);
	sprintf(buff,"CONFIG: %02X\r\n", nrf24l01_readregister(NRF24L01_REG_CONFIG)); writeString(buff);
	sprintf(buff,"RF_CH: %02X\r\n", nrf24l01_readregister(NRF24L01_REG_RF_CH)); writeString(buff);
	sprintf(buff,"RF_SETUP: %02X\r\n", nrf24l01_readregister(NRF24L01_REG_RF_SETUP)); writeString(buff);
	sprintf(buff,"EN_AA: %02X\r\n", nrf24l01_readregister(NRF24L01_REG_EN_AA)); writeString(buff);
	sprintf(buff,"EN_RXADDR: %02X\r\n", nrf24l01_readregister(NRF24L01_REG_EN_RXADDR)); writeString(buff);
	sprintf(buff,"OBSERVE_TX: %02X\r\n", nrf24l01_readregister(NRF24L01_REG_OBSERVE_TX)); writeString(buff);

	uint8_t adr[NRF24L01_ADDRSIZE];
	uint8_t addrrev[NRF24L01_ADDRSIZE];

	nrf24l01_readregisters(NRF24L01_REG_RX_ADDR_P0,adr,NRF24L01_ADDRSIZE);
	nrf24l01_revaddress(adr, (uint8_t *)addrrev);
	sprintf(buff,"RX_ADDR_P0: %02X %02X %02X %02X %02X \r\n",addrrev[0],addrrev[1],addrrev[2],addrrev[3],addrrev[4]); writeString(buff);

	nrf24l01_readregisters(NRF24L01_REG_RX_ADDR_P1,adr,NRF24L01_ADDRSIZE);
	nrf24l01_revaddress(adr, (uint8_t *)addrrev);
	sprintf(buff,"RX_ADDR_P1: %02X %02X %02X %02X %02X \r\n",addrrev[0],addrrev[1],addrrev[2],addrrev[3],addrrev[4]); writeString(buff);


	nrf24l01_readregisters(NRF24L01_REG_TX_ADDR,adr,NRF24L01_ADDRSIZE);
	nrf24l01_revaddress(adr, (uint8_t *)addrrev);
	sprintf(buff,"TX_ADDR: %02X %02X %02X %02X %02X \r\n",addrrev[0],addrrev[1],addrrev[2],addrrev[3],addrrev[4]); writeString(buff);


	sprintf(buff,"RX_PW_P0: %02X\r\n",nrf24l01_readregister(NRF24L01_REG_RX_PW_P0)); writeString(buff);
	sprintf(buff,"RX_PW_P1: %02X\r\n",nrf24l01_readregister(NRF24L01_REG_RX_PW_P1)); writeString(buff);


	writeString("##########################\r\n");
}


int main(){

	DDRD |= (1<<DDD4);

	PORTD |= (1<<PD4);


	EICRA =  (1<<ISC10) | (1<<ISC11) | (1<<ISC01);	// Trigger INT0 on falling edge
	EIMSK = 1<<INT1 | 1<<INT0;				// Enable INT1
	//EIMSK |= 1<<INT0;

	InitAD(0x00);

	// char data;

	initUART();
	delay_ms(1000);
	writeString("\r\n###############START###################\r\n");

	//uint8_t txrxrole = 0; // 1 transmitter 0 receiver


	//nrf24l01 variables
	uint8_t bufferout[NRF24L01_PAYLOAD];
	uint8_t bufferin[NRF24L01_PAYLOAD];
	uint8_t i = 0;

	//init nrf24l01
	nrf24l01_init();



	delay_ms(500);
	//evalTempSensorDS18B20s();
	//setup buffer

	for(i=0; i<sizeof(bufferout); i++)
		bufferout[i] = i+'a';
	for(i=0; i<sizeof(bufferin); i++)
		bufferin[i] = 0;


		printNRF24L01();


			while(1){

			//	writeString("Awaiting Input...\r\n");
			//	const char* string = readString();
				//writeString(string);
				//delay_ms(500);
			//	prThresh = atoi(string);

			//	v(time,movement,enableMoveSens);

				uint8_t pipe = 0;
					if(nrf24l01_readready(&pipe)) { //if data is ready
						rf = 0;
						char pipebuffer[5];
						writeString("getting data, on pipe ");
						sprintf(pipebuffer,"%i",pipe);
						//	itoa(pipe, pipebuffer, 10);
						writeString(pipebuffer);
						writeString("\r\n");

						//read buffer
						nrf24l01_read(bufferin);

						uint8_t samecheck = 1;
						writeString("  data: ");
						for(i=0; i<sizeof(bufferin); i++) {
							if(bufferin[i] != bufferout[i])
								samecheck = 0;
							writeString(bufferin[i]);
						}
						writeString("\r\n");
						if(samecheck)
							writeString("  check ok\r\n");
						else
							writeString("  check fails\r\n");
						for(i=0; i<sizeof(bufferin); i++)
							bufferin[i] = 0;


					}
						_delay_ms(10);




			}

return (1);
}

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

#include "IO/USART.h"

#include "IO/ParseStrings.h"

#include "Sensors/DS18B20.h"
#include "Sensors/Photoresistor.h"



#include "Wireless/NRF24L01.h"

//#define F_CPU 16000000

#define CE PB1
#define CSN PB0


uint8_t flag1 = 0;

uint8_t a0[5] = {0xe0, 0xf0, 0xf0, 0xf0, 0xf0};
uint8_t a1[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xc0};
uint8_t a2[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xc1};
uint8_t a3[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xc2};
uint8_t a4[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xc3};
uint8_t a5[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xc4};
uint8_t aT[5] =	{0xe0, 0xf0, 0xf0, 0xf0, 0xf0};

//char* bt_name = "MinxSensorNode";

USART uart = USART(9600);

char str[RX_BUFF];

DS18B20 ds = DS18B20(PORTD,DDRD,PIND, PD7);
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


void SetupNRF(){



//	uart.writeString("Set addresses\r\n");
	nrf.SetAddresses(a0,a1,a2,a3,a4,a5,aT);



	//	uart.writeString("Set Channel to 10\r\n");
		nrf.SetChannel(10);

	//	uart.writeString("Init... ");

		nrf.spi = SPI(DDRB,PORTB,PB4,PB3,PB5,PB2);
	//nrf24l01_init();
	nrf.Init();

	//	uart.writeString("... done \r\n");

}

void SetBluetoothName(char* name){

//	bt_name = name;

}

void Config(const char* str){





}

/*
void SetNRFMac(uint8_t pipe,uint8_t* add){

	switch(pipe){
		case 0: for(int j=0;j<5;j++){a0[j] = add[j];aT[j] = add[j];}
			break;
		case 1: for(int j=0;j<5;j++){a1[j] = add[j];}
			break;
		case 2:for(int j=0;j<5;j++){a2[j] = add[j];}
			break;
		case 3:for(int j=0;j<5;j++){a3[j] = add[j];}
			break;
		case 4:for(int j=0;j<5;j++){a4[j] = add[j];}
			break;
		case 5:for(int j=0;j<5;j++){a5[j] = add[j];}
			break;
		default:
			break;

	}




}
*/



void GetSensorData(){

	PORTD ^= (1<<PD4);
	//uart.writeString("Hi bro\n\r");

	char dsbuffer[255];
	char prbuffer[255];
	//	ds.therm_read_DS18B20(dsbuffer);
	ds.GetSensorStringXML(dsbuffer);
	pr.GetSensorStringXML(prbuffer);
	uart.writeString(dsbuffer);
	uart.writeString(prbuffer);


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

	//uart.writeString("Hi bro_0\n\r");
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

	char* string = "\r\n########### START #############\r\n";
	uart.writeString(string);


	SetupNRF();
	delay_ms(500);


	char s[50];
	nrf.PrintInfo(s);
	uart.writeString(s);



		while(1){

			PORTD ^= (1<<PD4);

			delay_ms(500);

			if(flag2 !=0){

				char buffer[500];

//				uart.writeString("String: ");
//				uart.writeString(str);
//				uart.writeString("\r\n");

				ps.Parse(str);

			//	ps.PrintVars(buffer);


				if(ps.getCmdID()==1){ // configure Bluetooth




				}else if(ps.getCmdID()==2){ // configure nRF24L01


					if(ps.getCmdProperty()==0){ // get MAC address for pipe


						ps.ParseMAC();

						uint8_t *addr;
						addr = ps.getMAC();

						if(ps.getPropertyID()==0){ // pipe 0 and T

							CpyMAC(addr,a0,5);
							CpyMAC(addr,aT,5);

//							char sb[50];
//							sprintf(sb," ");
//							sprintf(sb,"MAC a0: %02X:%02X:%02X:%02X:%02X\r\n",a0[0],a0[1],a0[2],a0[3],a0[4]);
//							strcat(buffer,sb);

							//a0 = ps.getMAC();

						}else if(ps.getPropertyID()==1){ // pipe 1

							CpyMAC(addr,a1,5);

						}else if(ps.getPropertyID()==2){ // pipe 2

							CpyMAC(addr,a2,5);

						}else if(ps.getPropertyID()==3){ // pipe 3

							CpyMAC(addr,a3,5);

						}else if(ps.getPropertyID()==4){ // pipe 4

							CpyMAC(addr,a4,5);

						}else if(ps.getPropertyID()==5){ // pipe 5

							CpyMAC(addr,a5,5);

						}




					//	ps.PrintMAC(buffer);

					}else if(ps.getCmdProperty()==1){ // Enable pipe

						nrf.EnablePipe(ps.getPropertyID());


					}else if(ps.getCmdProperty()==2){ // Channel

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

						nrf.SetAutoAck(ps.getPropertyID());

					}else if(ps.getCmdProperty()==8){ // ReInitNRF

						nrf.SetAddresses(a0,a1,a2,a3,a4,a5,aT);
						nrf.Init();

					}




				}else if(ps.getCmdID()==3){ // get Sensor data


					if(ps.getCmdProperty()==0){ // TempDHT22



					}else if(ps.getCmdProperty()==1){ // HumidityDHT22




					}else if(ps.getCmdProperty()==2){ //Lightsense

						char prbuffer[50];
						pr.GetSensorStringXML(prbuffer);
						strcat(buffer,prbuffer);

					}else if(ps.getCmdProperty()==3){ //TempDS18B20

						char dsb[50];
						ds.GetSensorStringXML(dsb);
						strcat(buffer,dsb);



					}else if(ps.getCmdProperty()==4){ //Movement

						if(flag1){
							strcat(buffer,"<mv>1</mv>\r\n");
						}else{
							strcat(buffer,"<mv>0</mv>\r\n");

						}



					}



				}




					uart.writeString(buffer);
					sprintf(str," ");
					sprintf(buffer,"");
					uart.writeString(buffer);

					flag2 = 0;

			}



			if(flag1){



				uart.writeString("<mv>1</mv>\r\n");


				flag1=0;
			}



		}

		PORTD ^= (1<<PD4);



return (0);
}

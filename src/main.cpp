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
#include <avr/pgmspace.h>
//#include <avr/wdt.h>

#include "IO/USART.h"
#include "IO/ParseStrings.h"
#include "Wireless/NRF24L01.h"

//#define F_CPU 16000000

#define CE PB1
#define CSN PB0


#define Sensor_DS18B20 1
#define Sensor_DHT22 0
#define Sensor_Move 1
#define Sensor_LightVoltage 1
#define Sensor_LightLux 0
#define Sensor_Pressure 0


#if Sensor_DHT22
#include "Sensors/DHT22.h"
#endif
#if Sensor_DS18B20
#include "Sensors/DS18B20.h"
#endif
#if Sensor_LightVoltage
#include "Sensors/Photoresistor.h"
#endif

uint8_t sent = 0;
uint8_t flag2 = 0;
uint8_t flagMovement = 0;
uint8_t nrfIntFlag = 0;

char bufferNRF[128][5];

//static char* system = "<SYSTEM><VERSION>1.0||";

uint8_t firstrun EEMEM;

const char sendSensorStr[]  = "//sensor/%s/|%s/|%s\\\\";
const char sendSystemStr[]  = "//system/%s/|%s/|%s\\\\";
//uint8_t pipeEnable EEMEM;
uint8_t nrfChannel EEMEM;
uint8_t nrfACK EEMEM;


uint64_t mem_adressTx[25] EEMEM;
uint64_t mem_adressRx[5] EEMEM;



/** standard Rx address to receive data on */
uint64_t adressRx[] = {0xa0a0a0a0a0,0xa0a0a0a0a1,0xa0a0a0a0a2,0xa0a0a0a0a3,0xa0a0a0a0a4};

/**  Tx address to send data to */
uint64_t adressTx[] = {0xb0b0b0b0b1,0xb0b0b0b0c1,0xb0b0b0b0d1,0xb0b0b0b0e1,0xb0b0b0b0f1};

USART uart = USART(9600);
NRF24L01 nrf = NRF24L01(DDRB, PORTB, CE, CSN);
ParseStrings ps = ParseStrings();


#if Sensor_DS18B20
DS18B20 ds = DS18B20(PORTD,DDRD,PIND, PD6);
#endif
#if Sensor_DHT22
DHT22 dht22 = DHT22(PORTD,DDRD,PIND,PD7);
#endif
#if Sensor_LightVoltage
Photoresistor pr = Photoresistor(0x00);
#endif

char str[RX_BUFF];




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
void GetNodeMAC(char* newMAC, uint8_t* nrfMAC){


	sprintf(newMAC,"%02X:%02X:%02X:%02X:%02X",nrfMAC[0],nrfMAC[1],nrfMAC[2],nrfMAC[3],nrfMAC[4]);



}

void CreateSensorMAC(char* newMAC, uint8_t* nrfMAC,char* add){


	sprintf(newMAC,"%02X:%02X:%02X:%02X:%02X%s",nrfMAC[0],nrfMAC[1],nrfMAC[2],nrfMAC[3],nrfMAC[4],add);



}
void CreateSensorMAC(char* newMAC, uint64_t nrfMAC,char* add){


	sprintf(newMAC,"%02X:%02X:%02X:%02X:%02X%s",
			(uint8_t)(nrfMAC & 0x00000000ff),
			(uint8_t)((nrfMAC & 0x000000ff00)>>8),
			(uint8_t)((nrfMAC & 0x0000ff0000)>>16),
			(uint8_t)((nrfMAC & 0x00ff000000)>>24),
			(uint8_t)((nrfMAC & 0xff00000000)>>32),add);



}

void SetupNRF(){




	uint8_t nrfChn = 0x0A;
	uint8_t nrfAutoACK = 1;
	//uint8_t nrfPipesEnable = 0b00111111;



	uint8_t fr =eeprom_read_byte(&firstrun);

	if(fr == 0){
		// empty eeprom after chip erase

		// nRF config
		eeprom_update_block(adressTx,mem_adressTx,sizeof(mem_adressTx));
		eeprom_update_block(adressRx,mem_adressRx,sizeof(mem_adressRx));


		eeprom_write_byte(&firstrun,1);

		eeprom_write_byte(&nrfACK,nrfAutoACK);
		eeprom_write_byte(&nrfChannel,nrfChn);
	//	eeprom_write_byte(&pipeEnable,nrfPipesEnable);


	}else if(fr==1){
		// after restart

		// read nRF config
		eeprom_read_block(adressTx,mem_adressTx,sizeof(mem_adressTx));
		eeprom_read_block(adressRx,mem_adressRx,sizeof(mem_adressRx));


		nrfChn = eeprom_read_byte(&nrfChannel);
		nrfAutoACK = eeprom_read_byte(&nrfACK);
	//	nrfPipesEnable = eeprom_read_byte(&pipeEnable);




	}




	nrf.Init();
	nrf.setChannel(nrfChn);
	nrf.setCRCLength(RF24_CRC_16);
	nrf.setPALevel(RF24_PA_LOW);
	nrf.setDataRate(RF24_1MBPS);
	nrf.setAutoAck(true);
	nrf.enableAckPayload();
	nrf.enableDynamicPayloads();

	nrf.openWritingPipe(adressTx[0]);
	nrf.openReadingPipe(1,adressRx[1]);

	nrf.startListening();




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
	//uart.writeString("INT0\r\n");
	nrfIntFlag = 1;
/*
	uint8_t payloadSize = nrf.getPayloadSize();
	uint8_t bufferin[payloadSize-6];
	char bufferout[payloadSize];
	uint8_t pipe = 0;
		if(nrf.ReadReady(&pipe)) { //if data is ready
			//read buffer
			nrf.read(bufferin,payloadSize);
			//	uint8_t samecheck = 1;


			//evalNRFBuffer(pipe,bufferin,bufferout);
			sprintf(bufferout,"%i: %s|",pipe, (char*)bufferin);
			uart.writeString(bufferout);

			for(uint8_t i=0;i<128;i++){
				bufferNRF[i][pipe] = bufferout[i];

			}



			for(uint8_t i=0; i<sizeof(bufferin); i++)
				bufferin[i] = 0;

		}

*/

}

ISR(INT1_vect){
	// set movement flag
	flagMovement = 1;

}



ISR(USART_RX_vect){
// fifo_put
	PORTD ^= (1<<PD4);
	//const char* s = uart.readString();
	uart.readString(str,RX_BUFF);


char s[50];

sprintf(s," ");

sprintf(s,"//input/%s",str);
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

	char buffer[350];
//	char system[500];


	sprintf(buffer,"\r\n########### START #############\r\n");

	uart.writeString(buffer);

	//snprintf(buffer,1, " ");

	SetupNRF();
	delay_ms(500);


	uint64_t  stdMAC = {0xa0a0a0a0a0};

	char propString[36];


		while(1){
			//uint8_t macAddr[5];
			//eeprom_read_block (macAddr, nRFPipe0, sizeof(nRFPipe0));
			//sprintf(buffer,"<sensor mac=%02X:%02X:%02X:%02X:%02X>",macAddr[0],macAddr[1],macAddr[2],macAddr[3],macAddr[4]);

			PORTD ^= (1<<PD4);


			sprintf(buffer," ");


			if(nrfIntFlag){
				nrfIntFlag = 0;




			}

			if(flag2){


				ps.Parse(str);


				if(ps.getCmdID()==0x10){ // configure nRF24L01

					char pidString[36];
					ps.getPropertyID(pidString);
					uint8_t PID = (uint8_t)strtoul(pidString,NULL,16);

					if(ps.getCmdProperty()==0){ // get MAC address for pipe


						ps.ParseMAC();

						uint64_t addr = ps.getMAC();

					//	sprintf(macadd,"mac: %02X:%02X:%02X:%02X:%02X\r\n",addr[0],addr[1],addr[2],addr[3],addr[4]);

					//	uart.writeString(macadd);
						if(PID==0){ // pipe 0 and T

							eeprom_write_block(&addr,&mem_adressTx[0],sizeof(mem_adressTx[0]));


						//	nrf.SetAddressPipe0(addr);
						//	nrf.SetAddressPipeT(addr);
							//CpyMAC(addr,a0,5);
							//CpyMAC(addr,aT,5);

//							char sb[50];
//							sprintf(sb," ");
//							sprintf(sb,"MAC a0: %02X:%02X:%02X:%02X:%02X\r\n",a0[0],a0[1],a0[2],a0[3],a0[4]);
//							strcat(buffer,sb);

							//a0 = ps.getMAC();

						}else if(PID==1){ // pipe 1

							eeprom_write_block(&addr,&mem_adressRx[0],sizeof(mem_adressRx[0]));

						//	strcat(buffer,"wrote block\r\n");

						//	nrf.SetAddressPipe1(addr);
							//CpyMAC(addr,a1,5);

						}else if(PID==2){ // pipe 2
							eeprom_write_block(&addr,&mem_adressRx[1],sizeof(mem_adressRx[1]));
						//	nrf.SetAddressPipe2(addr);
						//	CpyMAC(addr,a2,5);

						}else if(PID==3){ // pipe 3
							eeprom_write_block(&addr,&mem_adressRx[2],sizeof(mem_adressRx[2]));
						//	nrf.SetAddressPipe3(addr);
							//CpyMAC(addr,a3,5);

						}else if(PID==4){ // pipe 4
							eeprom_write_block(&addr,&mem_adressRx[3],sizeof(mem_adressRx[3]));
						//	nrf.SetAddressPipe4(addr);
						//	CpyMAC(addr,a4,5);

						}else if(PID==5){ // pipe 5
							eeprom_write_block(&addr,&mem_adressRx[4],sizeof(mem_adressRx[4]));
						//	nrf.SetAddressPipe5(addr);
							//CpyMAC(addr,a5,5);

						}




					//	ps.PrintMAC(buffer);

					}else if(ps.getCmdProperty()==1){ // Enable pipe


					//	uint8_t c = 0b00111111;

					//	eeprom_update_byte(&pipeEnable,c);


					//	nrf.EnablePipe(ps.getPropertyID());


					}else if(ps.getCmdProperty()==2){ // Channel

						eeprom_update_byte(&nrfChannel,PID);

					//	nrf.SetChannel(ps.getPropertyID());

					}else if(ps.getCmdProperty()==3){ // PowerLevel

					//	nrf.SetPowerLevel(ps.getPropertyID());

					}else if(ps.getCmdProperty()==4){ // Data rate

					//	nrf.SetDataRate(ps.getPropertyID());

					}else if(ps.getCmdProperty()==5){ // CRC level

					//	nrf.SetCRClength(ps.getPropertyID());

					}else if(ps.getCmdProperty()==6){ // Payload size

					//	nrf.SetPayloadSize(ps.getPropertyID());

					}else if(ps.getCmdProperty()==7){ // Auto ACK

						eeprom_update_byte(&nrfACK,PID);

					//	nrf.SetAutoAck(ps.getPropertyID());

					}else if(ps.getCmdProperty()==8){ // Reset

					//	nrf.SetAddresses(a0,a1,a2,a3,a4,a5,aT);
					//	nrf.Init();
						eeprom_write_byte(&firstrun,0);




					}else if(ps.getCmdProperty()==9){ // System Info


					//	char buf[500];
						//sprintf(buf," ");
					//	uart.USART0_Flush();
					//	uart.writeString("Getting Infos...");
						//nrf.PrintInfo(buf);
					//	sprintf(buf,"%s|%s",system,buf);
					//uart.writeString(system);
					//	delay_ms(100);
					//	uart.writeString(system);
						//	uart.writeString("\r\n");
						//	delay_ms(500);
					//	uart.writeString(buf);



					}




				}else if(ps.getCmdID()==0x20){
					/* Get System Info
					 *
					 */

					char infoString[32];
					char nodeMac[16];
					char valBuffer[35];

					ps.getPropertyString(propString);



					eeprom_read_block (&stdMAC, &mem_adressRx[0], sizeof(mem_adressRx[0]));

					if(ps.getCmdProperty()==0x00){
						/* Get available main node sensors
						 *
						 */

						CreateSensorMAC(nodeMac,stdMAC,(char*)"|");
						sprintf(valBuffer,"%s",(char*)"");

						#if Sensor_Move
							strcat(valBuffer,"0x31|");
						#endif
						#if Sensor_DHT22
							strcat(valBuffer,"0x32|");
						#endif
						#if Sensor_LightVoltage
							strcat(valBuffer,"0x33|");
						#endif
						#if Sensor_LightLux
							strcat(valBuffer,"0x34|");
						#endif
						#if Sensor_DS18B20
							strcat(valBuffer,"0x35|");
						#endif
						#if Sensor_Pressure
							strcat(valBuffer,"0x36|");
						#endif

						sprintf(infoString,sendSystemStr,propString,nodeMac,valBuffer);
						uart.writeString(infoString);


					}else if(ps.getCmdProperty()==0x01){
						/* Get available node 1 sensors
						 *
						 */

						CreateSensorMAC(nodeMac,stdMAC,(char*)"|");
						sprintf(valBuffer,"%s",(char*)"");

						// get node 1 sensors from nRF





					}




				}else if(ps.getCmdID()==0x30){ // get Sensor data

					ps.getPropertyString(propString);

					eeprom_read_block (&stdMAC, &mem_adressRx[0], sizeof(mem_adressRx[0]));
					uint8_t sensorMAC[8];

					ps.getPropertyIDAsMAC(sensorMAC);
					if(ps.getCmdProperty()==0x00){

					}
					#if Sensor_Move
						else if(ps.getCmdProperty()==0x01){
							/* Movement
							 * triggered by interrupt
							 */
							char mvbuffer[50];
							char newMac[25];



							CreateSensorMAC(newMac,stdMAC,(char*)":00:01:00|");

							if(flagMovement){
									// movement detected
								sprintf(mvbuffer,sendSensorStr,propString,newMac,"1|");

								flagMovement = 0;
							}else{
									// no movement detected
								sprintf(mvbuffer,sendSensorStr,propString,newMac,"0|");

							}

							uart.writeString(mvbuffer);

						}
					#endif
					#if Sensor_DHT22
						else if(ps.getCmdProperty()==0x02){ // DHT22

							char dhtBuffer[50];
							char dhtMac[25];
							char valBuffer[18];



							CreateSensorMAC(dhtMac,stdMACpipe0,(char*)":00:02:00|");

							dht22.GetSensorStringXML(valBuffer);

							sprintf(dhtBuffer,sendSensorStr,propString,dhtMac,valBuffer);

							uart.writeString(dhtBuffer);



						}
					#endif
					#if Sensor_LightVoltage
						else if(ps.getCmdProperty()==0x03){ //Lightsense analog

							char prbuffer[80];

							char lightBuffer[15];
							pr.GetSensorStringXML(lightBuffer);
							char lightMac[25];

							CreateSensorMAC(lightMac,stdMAC,(char*)":00:03:00|");

							sprintf(prbuffer,sendSensorStr,propString,lightMac,lightBuffer);

							uart.writeString(prbuffer);


						}
					#endif
					#if Sensor_LightLux
						else if(ps.getCmdProperty()==0x04){
							/* Lightsense digital I2C
							 *
							 * To Do: write Code for reading I2C light sensor
							 */
							char sensorbuffer[80];
							char lightBuffer[15];
							char lightMac[25];

							// get sensorvalue
								//pr.GetSensorStringXML(lightBuffer);


							CreateSensorMAC(lightMac,stdMACpipe0,(char*)":00:04:00|");

							sprintf(sensorbuffer,sendSensorStr,propString,lightMac,lightBuffer);

							uart.writeString(sensorbuffer);



						}
					#endif
					#if Sensor_DS18B20
						else if(ps.getCmdProperty()==0x05){ //TempDS18B20

							char dsb[120];
							char macStr[60];
							char tempStr[30];

							ds.GetSensorStringXML(tempStr);
							ds.GetMACString(macStr);
							//CreateSensorMAC(macStr,stdMACpipe0,(char*)":00:05:00|");
							sprintf(dsb,sendSensorStr,propString,macStr,tempStr);
							uart.writeString(dsb);


						}
					#endif
					#if Sensor_Pressure
						else if(ps.getCmdProperty()==0x06){ //Pressure
							/* Pressure
							 * TO Do: write Code for reading I2C pressure sensor
							 *
							 */





						}
					#endif
						else if(ps.getCmdProperty()==0x07){ // Remote Sensors

							//char buf[100];

							//uart.writeString("<RS>");
							for(int i=0;i<5;i++){
								char stri[128];
								for(int j=0;j<128;j++){
									stri[j] = bufferNRF[j][i];

								}
								uart.writeString(stri);


							}




						}



				}




				uart.writeString((char*)"+end\r\n");

				sprintf(str," ");



					flag2 = 0;

			}


		}



return (0);
}

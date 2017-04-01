/** \mainpage
****************************************************************************
* Project: SensorNode
*
* main.cpp
*
* Created on: Nov 12, 2016
*
* Author: Matthias Minx
*
* Revision: 0.2
*
*
****************************************************************************
*\section License
*
*
*
**************************************************************************/
/** \file main.cpp
 *
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

/*!
 	 \def CE
 	 Chip Enable (CE) for the nRF24L01+ Module
 */
#define CE PB1
#define CSN PB0		//!< Chip Select Not (CSN) for the nRF24L01+ Module


#define Sensor_DS18B20 1 //!< is a DS18B20 temperature Sensor connected to the microcontroller?
#define Sensor_DHT22 0	//!< is a DHT22 temperature and humidity sensor connected?
#define Sensor_Move 1	//!< is a movement detector connected?
#define Sensor_LightVoltage 1 	//!< is a photoresitor connected to a analog input?
#define Sensor_LightLux 0 	//!< is a digital Lux sensor connected (I2C)?
#define Sensor_Pressure 0 	//!< is a pressure sensor connected (I2C)?

#define NRF_MainNode 1	//!< Role of the node
#define NRF_SN 0 	//!< is Slave node?
#define NRF_SNnum 0	//!< number of slave nodes


#if Sensor_DHT22
#include "Sensors/DHT22.h"
#endif
#if Sensor_DS18B20
#include "Sensors/DS18B20.h"
#endif
#if Sensor_LightVoltage
#include "Sensors/Photoresistor.h"
#endif

// uint8_t sent = 0;	//!< Flag
uint8_t uartRX_flag = 0; 	//!< Flag received UART message
uint8_t flagMovement = 0; //!< Flag for movement detected
uint8_t nrfIntFlag = 0;		//!< Flag for incoming message from the RF module

char bufferNRF[128][5]; //!< Buffer for received messages via nRF24L01+ module

//static char* system = "<SYSTEM><VERSION>1.0||";

uint8_t EEMEM firstrun; //!<First run after reset/ reflash?

const char sendSensorStr[]  = "/sensor/%s/|%s/|%s"; //!< Template for sendSensor string
const char sendSystemStr[]  = "/system/%s/|%s/|%s"; //!< Tempplate for sendSystem string
const char getSystemStr[] = "%s|%s|%s"; 	//!< Template for getSystem string


uint8_t EEMEM pipeEnable ;	//!< Store RF enabled pipes in EEPROM
uint8_t EEMEM nrfChannel;	//!< Store RF Channel number in EEPROM
uint8_t EEMEM nrfACK;		//!< Store RF auto acknoledge in EEPROM
uint8_t EEMEM pwrLvl;		//!< Store RF power level in EEPROM
uint8_t EEMEM dataRate;		/*!< Store RF data rate in EEPROM */
uint8_t EEMEM crcLvl;		/*!< Store CRC level in EEPROM */

uint64_t EEMEM mem_adressTx[25];	/*!< Array for transmit addresses */
uint64_t EEMEM mem_adressRx[5];		/*!< Array for receive addresses */

char str[RX_BUFF];		//!< Receive string Buffer

#if NRF_MainNode

/** standard Rx address to receive data on */
uint64_t adressRx[] = {0xA0A0A0A0A0,0xA0A0A0A0A1,0xA0A0A0A0A2,0xA0A0A0A0A3,0xA0A0A0A0A4};

/**  Tx address to send data to */
uint64_t adressTx[] = {0xB0B0B0B0B0,0xB0B0B0B0C0,0xB0B0B0B0D0,0xB0B0B0B0E0,0xB0B0B0B0F0};

#elif NRF_SN
/** standard Rx address to receive data on */
uint64_t adressRx[] = {0xB0B0B0B0B0,0xB0B0B0B0C0,0xB0B0B0B0D0,0xB0B0B0B0E0,0xB0B0B0B0F0};

/**  Tx address to send data to */
uint64_t adressTx[] = {0xA0A0A0A0A0,0xA0A0A0A0A1,0xA0A0A0A0A2,0xA0A0A0A0A3,0xA0A0A0A0A4};

#endif




USART uart = USART(9600); //!< Create new USART object with a baud rate of 9600
NRF24L01 nrf = NRF24L01(DDRB, PORTB, CE, CSN); //!< Create new NRF242L01 object
ParseStrings ps = ParseStrings(); 	//!< Create new ParseString object




#if Sensor_DS18B20
DS18B20 ds = DS18B20(PORTD,DDRD,PIND, PD6);	//!< Create new DS18B20 object connected to pin PD6
#endif
#if Sensor_DHT22
DHT22 dht22 = DHT22(PORTD,DDRD,PIND,PD7); //!< Create new DHT22 object connected to pin PD7
#endif
#if Sensor_LightVoltage
Photoresistor pr = Photoresistor(0x00); 	//!< Create new Photoresitor object connected to the analog output 0x00
#endif



//! Delays a certain amount of milliseconds
/*!
  @param [in] ms Number of milliseconds to delay as unsigned integer 16 argument.
*/
void delay_ms(uint16_t ms){

	for(uint16_t t=0; t<=ms; t++){
		_delay_ms(1);
	}
}


//! Copies a MAC address from an uint8 array to a char array
/*!
  @param [in] nrfMAC MAC address uint8 array of the nRF24L01+ module
  @param [out] newMAC MAC address char array of the nRF24L01+ module
*/
void GetNodeMAC(char* newMAC, uint8_t* nrfMAC){


	sprintf(newMAC,"%02X:%02X:%02X:%02X:%02X",nrfMAC[0],nrfMAC[1],nrfMAC[2],nrfMAC[3],nrfMAC[4]);



}
/*! \fn void CreateSensorMAC(char* newMAC, uint8_t* nrfMAC,char* add)
	\brief Combine new MAC address from an uint8 MAC array and a char array to a char array

  @param [in] nrfMAC MAC address uint8 array of the nRF24L01+ module
  @param [out] newMAC MAC address char array of the sensor
  @param [in] add Address addition
*/
void CreateSensorMAC(char* newMAC, uint8_t* nrfMAC,char* add){


	sprintf(newMAC,"%02X:%02X:%02X:%02X:%02X%s",nrfMAC[0],nrfMAC[1],nrfMAC[2],nrfMAC[3],nrfMAC[4],add);



}
/*! \fn void CreateSensorMAC(char* newMAC, uint64_t nrfMAC,char* add)
  \brief Combine new MAC address from an uint64 MAC address and a char array to a char array

  @param [in] nrfMAC MAC address uint64 of the nRF24L01+ module
  @param [out] newMAC MAC address char array of the sensor
  @param [in] add Address addition
*/
void CreateSensorMAC(char* newMAC, uint64_t nrfMAC,char* add){


	sprintf(newMAC,"%02X:%02X:%02X:%02X:%02X%s",
			(uint8_t)((nrfMAC & 0xff00000000)>>32),
			(uint8_t)((nrfMAC & 0x00ff000000)>>24),
			(uint8_t)((nrfMAC & 0x0000ff0000)>>16),
			(uint8_t)((nrfMAC & 0x000000ff00)>>8),
			(uint8_t)(nrfMAC & 0x00000000ff),add);



}

/*! \fn void SetupNRF()
    \brief Setup RF Module

*/
void SetupNRF(){




	uint8_t nrfChn = 0x0A;
	uint8_t nrfAutoACK = 1;
	uint8_t nrfPipesEnable = 0b00111111;

	uint8_t powerLvl = RF24_PA_LOW;
	uint8_t dataRt = RF24_1MBPS;
	uint8_t crcLevel = RF24_CRC_16;





	uint8_t fr =eeprom_read_byte(&firstrun);

	if(fr == 0){
		// empty eeprom after chip erase

		// nRF config
		eeprom_update_block(adressTx,mem_adressTx,sizeof(mem_adressTx));
		eeprom_update_block(adressRx,mem_adressRx,sizeof(mem_adressRx));


		eeprom_write_byte(&firstrun,1);

		eeprom_write_byte(&nrfACK,nrfAutoACK);
		eeprom_write_byte(&nrfChannel,nrfChn);
		eeprom_write_byte(&pipeEnable,nrfPipesEnable);

		eeprom_write_byte(&pwrLvl,powerLvl);
		eeprom_write_byte(&dataRate,dataRt);
		eeprom_write_byte(&crcLvl,crcLevel);


	}else if(fr==1){
		// after restart

		// read nRF config
		eeprom_read_block(adressTx,mem_adressTx,sizeof(mem_adressTx));
		eeprom_read_block(adressRx,mem_adressRx,sizeof(mem_adressRx));


		nrfChn = eeprom_read_byte(&nrfChannel);
		nrfAutoACK = eeprom_read_byte(&nrfACK);
		nrfPipesEnable = eeprom_read_byte(&pipeEnable);

		powerLvl = eeprom_read_byte(&pwrLvl);
		dataRt = eeprom_read_byte(&dataRate);
		crcLevel = eeprom_read_byte(&crcLvl);




	}




	nrf.Init();
	nrf.setChannel(nrfChn);
	nrf.setCRCLength(crcLevel);
	nrf.setPALevel(powerLvl);
	nrf.setDataRate(dataRt);

	if (nrfAutoACK==1){
		nrf.setAutoAck(true);
	}else{
		nrf.setAutoAck(false);
	}


	nrf.enableAckPayload();
	nrf.enableDynamicPayloads();

	nrf.openWritingPipe(adressTx[0]);
	nrf.openReadingPipe(1,adressRx[0]);

	nrf.startListening();




}
/*! \fn void init_interupt()
    \brief Initialize interrupts

    Trigger INT1 on rising edge and INT0 on falling edge
*/
void init_interupt(){


	EICRA =  (1<<ISC10) | (1<<ISC11) | (1<<ISC01);

	// Enable INT0 and  INT1
	EIMSK = 1<<INT1 | 1<<INT0;


    // Start it up.
  sei();
}

//! Interrupt routine for INT0
/*!
 *	RF module sends an interrupt if there is data in its buffer to read
 *
 */
ISR(INT0_vect){

	PORTD ^= (1<<PD4);
	// nrf receive
	//uart.writeString("INT0\r\n");
	nrfIntFlag = 1;

	bool tx,fail,rx;
	nrf.whatHappened(tx,fail,rx);
	uint8_t pipeNo;
	uint8_t received[128];

	if(rx || nrf.available(&pipeNo) ){

		nrf.read(&received,sizeof(received));

		for(uint8_t i=0;i<sizeof(received);i++){
			bufferNRF[i][pipeNo] = received[i];
		}


	}




}
//! Interrupt routine for INT1
/*!
 *	Sets the flagMovement to 0 if the movment detector fires
 *
 */
ISR(INT1_vect){
	// set movement flag
	flagMovement = 1;

}


//! Interrupt routine for USART_RX
/*!
 *	Reads the incoming message from the UART buffer
 *
 */
ISR(USART_RX_vect){
// fifo_put
	PORTD ^= (1<<PD4);
	//const char* s = uart.readString();
	uart.readString(str,RX_BUFF);


char s[50];

sprintf(s," ");

sprintf(s,"//input/%s/",str);
//sprintf(str,"%s","");
uart.writeString(s);



uartRX_flag = 1;


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

/*! \fn int main()
 *  \brief Main function
 *
 *
 *
 * @return an integer 0 upon exit success
 */
int main(){

	/* Define pull-ups and set output high
	 *	Define directions for port pins
	 */
	DDRD |= (1<<DDD4);
	PORTD |= (1<<PD4| (1<<PD3));

	init_interupt();

	uart.initUART();

	delay_ms(500); /* Delay 500ms */

	PORTD ^= (1<<PD4); /* Toggle PD4*/

	char buffer[350];
//	char system[500];

	//sprintf(buffer,"\r\n########### START #############\r\n");
	//uart.writeString(buffer);

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

			if(uartRX_flag){


				ps.Parse(str);


				if(ps.getCmdID()==0x10){ // configure nRF24L01

					char pidString[36];
					ps.getPropertyID(pidString);
					uint8_t PID = (uint8_t)strtoul(pidString,NULL,16);

					if(ps.getCmdProperty()==0){ // set MAC address for pipe


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

					}else if(ps.getCmdProperty()==1){ // set Channel

						eeprom_update_byte(&nrfChannel,PID);


					}else if(ps.getCmdProperty()==2){ // PowerLevel

						eeprom_update_byte(&pwrLvl,PID);


					}else if(ps.getCmdProperty()==3){ // Data rate

						eeprom_update_byte(&dataRate,PID);


					}else if(ps.getCmdProperty()==4){ // CRC level

						eeprom_update_byte(&crcLvl,PID);


					}else if(ps.getCmdProperty()==5){ // Auto ACK

						eeprom_update_byte(&nrfACK,PID);


					}else if(ps.getCmdProperty()==6){ // Reset

						eeprom_write_byte(&firstrun,0);


					}else if(ps.getCmdProperty()==7){ // System Info

						/*!
						 * \todo System info
						 */

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

					char infoString[80];
					char nodeMac[16];
					char valBuffer[35];

					ps.getPropertyString(propString);



					if(ps.getCmdProperty()==0x00){
						/* Get available node sensors
						 *
						 */
						eeprom_read_block (&stdMAC, &mem_adressRx[0], sizeof(mem_adressRx[0]));
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


					}
#if NRF_MainNode
					else if(ps.getCmdProperty()==0x01  ){
						/* Get available sensors on node n
						 *
						 */

						uint8_t cp = ps.getCmdProperty()-1;
						uint64_t writeto = {0x00};
						eeprom_read_block (&writeto, &mem_adressTx[cp], sizeof(mem_adressTx[cp]));



						nrf.stopListening();
						nrf.openWritingPipe(writeto);
						nrf.openReadingPipe(1,adressRx[0]);

						nrf.startListening();


						CreateSensorMAC(nodeMac,writeto,(char*)"");
						sprintf(valBuffer,"%s",(char*)"Sensors");




						sprintf(infoString,getSystemStr,(char*)"20",nodeMac,valBuffer);

						// get node n sensors via nRF
					//	nrf.write(infoString,sizeof(infoString));
						uart.writeString(infoString);




					}

#endif


				}else if(ps.getCmdID()==0x30){ // get Sensor data

					ps.getPropertyString(propString);

					eeprom_read_block (&stdMAC, &mem_adressRx[0], sizeof(mem_adressRx[0]));
					uint8_t sensorMAC[8];

					ps.getPropertyIDAsMAC(sensorMAC);
					if(ps.getCmdProperty()==0x00){

					}
					#if Sensor_Move
						else if(ps.getCmdProperty()==0x01){
							/*
							 * Movement detector
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
						else if(ps.getCmdProperty()==0x02){
							/*
							 * DHT22
							 */

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
						else if(ps.getCmdProperty()==0x03){
							/*
							 * Lightsense analog
							 *
							 */

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
							/*!
							 * \brief Lightsense digital I2C
							 *
							 * \todo : write Code for reading I2C light sensor
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
							/*
							 * DS18B20
							 *
							 * 	Get temperature from the DS18B20 temperature sensor
							 *
							 */

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
							/*!
							 * \brief Pressure
							 * \todo: write Code for reading I2C BMP180 sensor
							 *
							 * the BMP180 also returns temperature values
							 *
							 */





						}
					#endif




				}




				uart.writeString((char*)"\\\\+end\r\n");

				sprintf(str," ");



				uartRX_flag = 0;

			}


		}



return (0);
}

/*
 * NRF24L01.cpp
 *
 *  Created on: Nov 14, 2016
 *      Author: Matthias Minx
 *
 * 	based on the Code of:
 *
 * nrf24l01 lib 0x02
 * Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * References:
 *   -  This library is based upon nRF24L01 avr lib by Stefan Engelke
 *      http://www.tinkerer.eu/AVRLib/nRF24L01
 *   -  and arduino library 2011 by J. Coliz
 *      http://maniacbug.github.com/RF24
 *
 *      Released under GPLv3.
 *      Please refer to LICENSE file for licensing information.
*/
#include "NRF24L01.h"
#include "NRF24L01registers.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>



NRF24L01::NRF24L01() {


	NRF24L01_DDR = &DDRB;
//	NRF24L01_PORT = &PORTB;
	NRF24L01_CE = PB1;
	NRF24L01_CSN = PB0;

	//transmission channel
	setChannel(124);
		//payload lenght
	setPayloadSize(16);
		//auto ack enabled
	setAutoAck(true);


/*
	//enable / disable pipe
	EnablePipe(0);
	EnablePipe(1);
	EnablePipe(2);
	EnablePipe(3);
	EnablePipe(4);
	EnablePipe(5);
*/
	NRF24L01_ADDRSIZE = 5;


	//uint8_t a0[5] = { 0xe0, 0xf0, 0xf0, 0xf0, 0xf0 };
/*
	SetAddress(nrf24l01_addr0,0xe0, 0xf0, 0xf0, 0xf0, 0xf0);
	SetAddress(nrf24l01_addr1,0xe1, 0xf0, 0xf0, 0xf0, 0xe0);
	SetAddress(nrf24l01_addr2,0xe1, 0xf0, 0xf0, 0xf0, 0xe1);
	SetAddress(nrf24l01_addr3,0xe1, 0xf0, 0xf0, 0xf0, 0xe2);
	SetAddress(nrf24l01_addr4,0xe1, 0xf0, 0xf0, 0xf0, 0xe3);
	SetAddress(nrf24l01_addr5,0xe1, 0xf0, 0xf0, 0xf0, 0xe4);
	SetAddress(nrf24l01_addrtx,0xe0, 0xf0, 0xf0, 0xf0, 0xf0);


	uint8_t a1[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xe0};
	uint8_t a2[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xe1};
	uint8_t a3[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xe2};
	uint8_t a4[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xe3};
	uint8_t a5[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xe4};
	uint8_t aT[5] =	{0xe0, 0xf0, 0xf0, 0xf0, 0xf0};

	SetAddresses(a0,a1,a2,a3,a4,a5,aT);
*/

	//power setup
//	SetPowerLevel(NRF24L01_RF24_PA_MIN);

	//speed setup
//	SetDataRate(NRF24L01_RF24_SPEED_1MBPS);

	//crc setup
//	SetCRClength(NRF24L01_RF24_CRC_16);

//	u = USART();

}


NRF24L01::NRF24L01(IO *iface, volatile uint8_t &DDR, volatile uint8_t &PORT, uint8_t CE, uint8_t CSN):p_variant(true),csDelay(5){
// DDR, PORT, CE,CSN
	//u = USART();


	ioInterface = iface;

	//spi = SPI(DDRB,PORTB,PB4,PB3,PB5,PB2);
	NRF24L01_DDR = &DDR;
//	NRF24L01_PORT = &Wire_PORT;
	NRF24L01_CE = CE;
	NRF24L01_CSN = CSN;

	txDelay = 0;
	spi_speed = 0;
	failureDetected=false;

	//transmission channel
	NRF24L01_CH = 124;
			//payload lenght
	NRF24L01_PAYLOAD = 32;
			//auto ack enabled
	NRF24L01_ACK = 1;

	dynamic_payloads_enabled = false;

			//enable / disable pipe
	NRF24L01_ENABLEDP0 = 1; //pipe 0
	NRF24L01_ENABLEDP1 = 1; //pipe 1
	NRF24L01_ENABLEDP2 = 1; //pipe 2
	NRF24L01_ENABLEDP3 = 1; //pipe 3
	NRF24L01_ENABLEDP4 = 1; //pipe 4
	NRF24L01_ENABLEDP5 = 1; //pipe 5


	NRF24L01_ADDRSIZE = 5;

	//power setup
	NRF24L01_RF24_PA = NRF24L01_RF24_PA_MIN;

	//speed setup
	NRF24L01_RF24_SPEED = NRF24L01_RF24_SPEED_1MBPS;

	//crc setup
	NRF24L01_RF24_CRC = NRF24L01_RF24_CRC_16;

/*
	uint8_t a0[5] = {0xe0, 0xf0, 0xf0, 0xf0, 0xf0};
	uint8_t a1[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xe0};
	uint8_t a2[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xe1};
	uint8_t a3[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xe2};
	uint8_t a4[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xe3};
	uint8_t a5[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xe4};
	uint8_t aT[5] =	{0xe0, 0xf0, 0xf0, 0xf0, 0xf0};
*/
	pipe0_reading_address[0]=0;

	//SetAddresses(a0,a1,a2,a3,a4,a5,aT);




}
/*
NRF24L01::NRF24L01(volatile uint8_t &DDR, volatile uint8_t &PORT, uint8_t CE, uint8_t CSN):p_variant(true),csDelay(5){
// DDR, PORT, CE,CSN
	//u = USART();
	//spi = SPI(DDRB,PORTB,PB4,PB3,PB5,PB2);
	NRF24L01_DDR = &DDR;
//	NRF24L01_PORT = &Wire_PORT;
	NRF24L01_CE = CE;
	NRF24L01_CSN = CSN;

	txDelay = 0;
	spi_speed = 0;
	failureDetected=false;

	//transmission channel
	NRF24L01_CH = 124;
			//payload lenght
	NRF24L01_PAYLOAD = 32;
			//auto ack enabled
	NRF24L01_ACK = 1;

	dynamic_payloads_enabled = false;

			//enable / disable pipe
	NRF24L01_ENABLEDP0 = 1; //pipe 0
	NRF24L01_ENABLEDP1 = 1; //pipe 1
	NRF24L01_ENABLEDP2 = 1; //pipe 2
	NRF24L01_ENABLEDP3 = 1; //pipe 3
	NRF24L01_ENABLEDP4 = 1; //pipe 4
	NRF24L01_ENABLEDP5 = 1; //pipe 5


	NRF24L01_ADDRSIZE = 5;

	//power setup
	NRF24L01_RF24_PA = NRF24L01_RF24_PA_MIN;

	//speed setup
	NRF24L01_RF24_SPEED = NRF24L01_RF24_SPEED_1MBPS;

	//crc setup
	NRF24L01_RF24_CRC = NRF24L01_RF24_CRC_16;


	uint8_t a0[5] = {0xe0, 0xf0, 0xf0, 0xf0, 0xf0};
	uint8_t a1[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xe0};
	uint8_t a2[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xe1};
	uint8_t a3[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xe2};
	uint8_t a4[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xe3};
	uint8_t a5[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xe4};
	uint8_t aT[5] =	{0xe0, 0xf0, 0xf0, 0xf0, 0xf0};

	pipe0_reading_address[0]=0;

	//SetAddresses(a0,a1,a2,a3,a4,a5,aT);




}
*/

bool NRF24L01::Init(){

	 uint8_t setup=0;
	//setup port

		*NRF24L01_DDR |= (1<<NRF24L01_CSN); //output
		*NRF24L01_DDR |= (1<<NRF24L01_CE); //output

		//spi = SPI();
	    //spi.Init(); //   init spi

		//ioInterface->USART_writeString((char*) ("Init nRF\r\n"));

	    CE_LOW(); //low CE
	    CSN_HIGH(); //high CSN


	    _delay_ms(5); //wait for the radio to init


	    // Reset NRF_CONFIG and enable 16-bit CRC.
	     WriteRegister( NRF24L01_REG_CONFIG, 0x0C ) ;

	    setRetries(5,15);

	    setPALevel( RF24_PA_MAX ) ;

	    if( setDataRate( RF24_250KBPS ) ){
	        p_variant = true ;
	    }
	    setup = ReadRegister(NRF24L01_REG_RF_SETUP);

	    // Then set the data rate to the slowest (and most reliable) speed supported by all
	    // hardware.
	    setDataRate( RF24_1MBPS );
	    ToggleFeatures();
	    WriteRegister(NRF24L01_REG_FEATURE,0 );
	    WriteRegister(NRF24L01_REG_DYNPD,0);
	      dynamic_payloads_enabled = false;

	  // Reset current status
	   // Notice reset and flush is the last thing we do
	  WriteRegister(NRF24L01_REG_STATUS,_BV(NRF24L01_REG_MASK_RX_DR) | _BV(NRF24L01_REG_MASK_TX_DS) | _BV(NRF24L01_REG_MASK_MAX_RT) );
	  // Set up default configuration.  Callers can always change it later.
	  // This channel should be universally safe and not bleed over into adjacent
	 // spectrum.
	 setChannel(0x0A);

	 // Flush buffers
	   FlushRx();
	   FlushTx();

	   powerUp(); //Power up by default when begin() is called

	   // Enable PTX, do not write CE high so radio will remain in standby I mode ( 130us max to transition to RX or TX instead of 1500us from powerUp )
		 // PTX should use only 22uA of power
	   WriteRegister(NRF24L01_REG_CONFIG, ( ReadRegister(NRF24L01_REG_CONFIG) ) & ~_BV(NRF24L01_REG_PRIM_RX) );

		 // if setup is 0 or ff then there was no response from module
	 return ( setup != 0 && setup != 0xff );


}

void NRF24L01::startListening(void){

  powerUp();

  WriteRegister(NRF24L01_REG_CONFIG, ReadRegister(NRF24L01_REG_CONFIG) | _BV(NRF24L01_REG_PRIM_RX));
  WriteRegister(NRF24L01_REG_STATUS, _BV(NRF24L01_REG_MASK_RX_DR) | _BV(NRF24L01_REG_MASK_TX_DS) | _BV(NRF24L01_REG_MASK_MAX_RT) );
  CE_HIGH();
  // Restore the pipe0 adddress, if exists
  if (pipe0_reading_address[0] > 0){
	  WriteRegisters(NRF24L01_REG_RX_ADDR_P0, pipe0_reading_address, NRF24L01_ADDRSIZE);
  }else{
	closeReadingPipe(0);
  }

  // Flush buffers
  //flush_rx();
  if(ReadRegister(NRF24L01_REG_FEATURE) & _BV(NRF24L01_REG_EN_AA)){
	FlushTx();
  }

  // Go!
  //delayMicroseconds(100);
}

/****************************************************************************/
static const uint8_t child_pipe_enable[] PROGMEM ={NRF24L01_REG_ERX_P0, NRF24L01_REG_ERX_P1, NRF24L01_REG_ERX_P2, NRF24L01_REG_ERX_P3, NRF24L01_REG_ERX_P4, NRF24L01_REG_ERX_P5};

void NRF24L01::stopListening(void){
  CE_LOW();

  delay_us(txDelay);

  if(ReadRegister(NRF24L01_REG_FEATURE) & _BV(NRF24L01_REG_EN_AA)){
	  delay_us(txDelay); //200
    FlushTx();
  }
  //flush_rx();
  WriteRegister(NRF24L01_REG_CONFIG, ( ReadRegister(NRF24L01_REG_CONFIG) ) & ~_BV(NRF24L01_REG_PRIM_RX) );


  WriteRegister(NRF24L01_REG_EN_RXADDR,ReadRegister(NRF24L01_REG_EN_RXADDR) | _BV(pgm_read_byte(&child_pipe_enable[0]))); // Enable RX on pipe0

  //delayMicroseconds(100);

}

/****************************************************************************/
void NRF24L01::powerDown(void)
{
	CE_LOW(); // Guarantee CE is low on powerDown
  WriteRegister(NRF24L01_REG_CONFIG,ReadRegister(NRF24L01_REG_CONFIG) & ~_BV(NRF24L01_REG_PWR_UP));
}

/****************************************************************************/
//Power up now. Radio will not power down unless instructed by MCU for config changes etc.
void NRF24L01::powerUp(void)
{
   uint8_t cfg = ReadRegister(NRF24L01_REG_CONFIG);

   // if not powered up then power up and wait for the radio to initialize
   if (!(cfg & _BV(NRF24L01_REG_PWR_UP))){
	   WriteRegister(NRF24L01_REG_CONFIG, cfg | _BV(NRF24L01_REG_PWR_UP));

      // For nRF24L01+ to go from power down mode to TX or RX mode it must first pass through stand-by mode.
	  // There must be a delay of Tpd2stby (see Table 16.) after the nRF24L01+ leaves power down mode before
	  // the CEis set high. - Tpd2stby can be up to 5ms per the 1.0 datasheet
      delay_ms(5);
   }
}



/*** GetStatus ********************************************/
uint8_t NRF24L01::getStatus(){

	return (spiTrans(NRF24L01_CMD_NOP)); //get status, send NOP request

}
/*** set channel ***************************************************/
void NRF24L01::setChannel(uint8_t channel){
	const uint8_t max_channel = 125;
	WriteRegister(NRF24L01_REG_RF_CH,rf24_min(channel,max_channel));

}
uint8_t NRF24L01::getChannel(){
  return (ReadRegister(NRF24L01_REG_RF_CH));
}

/****************************************************************************/
void NRF24L01::setPayloadSize(uint8_t size){
	NRF24L01_PAYLOAD = rf24_min(size,32);
}
/****************************************************************************/
uint8_t NRF24L01::getPayloadSize(void){
  return (NRF24L01_PAYLOAD);
}

/*
void NRF24L01::EnableAutoAck(){
	NRF24L01_ACK = 1;
}
void NRF24L01::DisableAutoAck(){
	NRF24L01_ACK = 0;
}
void NRF24L01::SetAutoAck(uint8_t ack){
	NRF24L01_ACK= ack;
}
void NRF24L01::EnablePipe(uint8_t pipe){


	switch (pipe) {
		case 0: NRF24L01_ENABLEDP0 = 1; //pipe 0
			break;
		case 1: NRF24L01_ENABLEDP1 = 1; //pipe 1
			break;
		case 2: NRF24L01_ENABLEDP2 = 1; //pipe 2
			break;
		case 3: NRF24L01_ENABLEDP3 = 1; //pipe 3
			break;
		case 4: NRF24L01_ENABLEDP4 = 1; //pipe 4
			break;
		case 5: NRF24L01_ENABLEDP5 = 1; //pipe 5
			break;
		default:
			break;
	}




}
uint8_t NRF24L01::GetPayloadSize(){
	return NRF24L01_PAYLOAD;
}
void NRF24L01::SetAddresses(uint8_t* pipe0, uint8_t* pipe1,uint8_t* pipe2,uint8_t* pipe3,uint8_t* pipe4,uint8_t* pipe5,uint8_t* pipeT){

	for(int i=0;i<5;i++){

		nrf24l01_addr0[i] = pipe0[i];
		nrf24l01_addr1[i] = pipe1[i];
		nrf24l01_addr2[i] = pipe2[i];
		nrf24l01_addr3[i] = pipe3[i];
		nrf24l01_addr4[i] = pipe4[i];
		nrf24l01_addr5[i] = pipe5[i];
		nrf24l01_addrtx[i] = pipeT[i];


	}


}
void NRF24L01::SetAddress(uint8_t* pipe,uint8_t p0,uint8_t p1,uint8_t p2,uint8_t p3,uint8_t p4){

	pipe[0]= p0;
	pipe[1]= p1;
	pipe[2]= p2;
	pipe[3]= p3;
	pipe[4]= p4;

}
void NRF24L01::SetAddressPipe0(uint8_t p0,uint8_t p1,uint8_t p2,uint8_t p3,uint8_t p4){
	nrf24l01_addr0[0]= p0;
	nrf24l01_addr0[1]= p1;
	nrf24l01_addr0[2]= p2;
	nrf24l01_addr0[3]= p3;
	nrf24l01_addr0[4]= p4;

}
void NRF24L01::SetAddressPipe1(uint8_t p0,uint8_t p1,uint8_t p2,uint8_t p3,uint8_t p4){
	nrf24l01_addr1[0]= p0;
	nrf24l01_addr1[1]= p1;
	nrf24l01_addr1[2]= p2;
	nrf24l01_addr1[3]= p3;
	nrf24l01_addr1[4]= p4;
}
void NRF24L01::SetAddressPipe2(uint8_t p0,uint8_t p1,uint8_t p2,uint8_t p3,uint8_t p4){
	nrf24l01_addr2[0]= p0;
	nrf24l01_addr2[1]= p1;
	nrf24l01_addr2[2]= p2;
	nrf24l01_addr2[3]= p3;
	nrf24l01_addr2[4]= p4;
}
void NRF24L01::SetAddressPipe3(uint8_t p0,uint8_t p1,uint8_t p2,uint8_t p3,uint8_t p4){
	nrf24l01_addr3[0]= p0;
	nrf24l01_addr3[1]= p1;
	nrf24l01_addr3[2]= p2;
	nrf24l01_addr3[3]= p3;
	nrf24l01_addr3[4]= p4;
}
void NRF24L01::SetAddressPipe4(uint8_t p0,uint8_t p1,uint8_t p2,uint8_t p3,uint8_t p4){
	nrf24l01_addr4[0]= p0;
	nrf24l01_addr4[1]= p1;
	nrf24l01_addr4[2]= p2;
	nrf24l01_addr4[3]= p3;
	nrf24l01_addr4[4]= p4;
}
void NRF24L01::SetAddressPipe5(uint8_t p0,uint8_t p1,uint8_t p2,uint8_t p3,uint8_t p4){
	nrf24l01_addr5[0]= p0;
	nrf24l01_addr5[1]= p1;
	nrf24l01_addr5[2]= p2;
	nrf24l01_addr5[3]= p3;
	nrf24l01_addr5[4]= p4;
}
void NRF24L01::SetAddressPipeT(uint8_t p0,uint8_t p1,uint8_t p2,uint8_t p3,uint8_t p4){
	nrf24l01_addrtx[0]= p0;
	nrf24l01_addrtx[1]= p1;
	nrf24l01_addrtx[2]= p2;
	nrf24l01_addrtx[3]= p3;
	nrf24l01_addrtx[4]= p4;
}
void NRF24L01::SetAddressPipe0(uint8_t* addr){
	for(uint8_t i=0;i<5;i++){
		nrf24l01_addr0[i]=addr[i];
	}


}
void NRF24L01::SetAddressPipe1(uint8_t* addr){
	for(uint8_t i=0;i<5;i++){
		nrf24l01_addr1[i]=addr[i];
	}
}
void NRF24L01::SetAddressPipe2(uint8_t* addr){
	for(uint8_t i=0;i<5;i++){
		nrf24l01_addr2[i]=addr[i];
	}
}
void NRF24L01::SetAddressPipe3(uint8_t* addr){
	for(uint8_t i=0;i<5;i++){
		nrf24l01_addr3[i]=addr[i];
	}
}
void NRF24L01::SetAddressPipe4(uint8_t* addr){
	for(uint8_t i=0;i<5;i++){
		nrf24l01_addr4[i]=addr[i];
	}
}
void NRF24L01::SetAddressPipe5(uint8_t* addr){
	for(uint8_t i=0;i<5;i++){
		nrf24l01_addr5[i]=addr[i];
	}
}
void NRF24L01::SetAddressPipeT(uint8_t* addr){
	for(uint8_t i=0;i<5;i++){
		nrf24l01_addrtx[i]=addr[i];
	}
}
void NRF24L01::SetPowerLevel(uint8_t power){
	NRF24L01_RF24_PA = power;
}
void NRF24L01::SetDataRate(uint8_t speed){
	NRF24L01_RF24_SPEED = speed;
}
void NRF24L01::SetCRClength(uint8_t crclvl){
	NRF24L01_RF24_CRC = crclvl;
}
uint8_t NRF24L01::ReadReady(uint8_t* pipe){
	uint8_t status = GetStatus();
	uint8_t ret = status & (1<<NRF24L01_REG_RX_DR);
	if(ret) {
		//get the pipe number
		if(pipe){
			*pipe = (status >> NRF24L01_REG_RX_P_NO) & 0b111;
		}
	}

	return ret;
}
*/
/****************************************************************************/

void NRF24L01::whatHappened(bool& tx_ok,bool& tx_fail,bool& rx_ready){
  // Read the status & reset the status in one easy call
  // Or is that such a good idea?
  uint8_t status = WriteRegister(NRF24L01_REG_STATUS,_BV(NRF24L01_REG_RX_DR) | _BV(NRF24L01_REG_TX_DS) | _BV(NRF24L01_REG_MAX_RT) );

  // Report to the user what happened
  tx_ok = status & _BV(NRF24L01_REG_TX_DS);
  tx_fail = status & _BV(NRF24L01_REG_MAX_RT);
  rx_ready = status & _BV(NRF24L01_REG_RX_DR);
}
/****************************************************************************/

void NRF24L01::openWritingPipe(const uint8_t *address){
  // Note that AVR 8-bit uC's store this LSB first, and the NRF24L01(+)
  // expects it LSB first too, so we're good.

	WriteRegisters(NRF24L01_REG_RX_ADDR_P0, address, NRF24L01_ADDRSIZE);
	WriteRegisters(NRF24L01_REG_TX_ADDR, address, NRF24L01_ADDRSIZE);


  //const uint8_t max_payload_size = 32;
  //write_register(RX_PW_P0,rf24_min(payload_size,max_payload_size));
	WriteRegister(NRF24L01_REG_RX_PW_P0,NRF24L01_PAYLOAD);
}
/****************************************************************************/

void NRF24L01::openWritingPipe(uint64_t value){
  // Note that AVR 8-bit uC's store this LSB first, and the NRF24L01(+)
  // expects it LSB first too, so we're good.

	WriteRegisters(NRF24L01_REG_RX_ADDR_P0, reinterpret_cast<uint8_t*>(&value), NRF24L01_ADDRSIZE);
  WriteRegisters(NRF24L01_REG_TX_ADDR, reinterpret_cast<uint8_t*>(&value), NRF24L01_ADDRSIZE);


  //const uint8_t max_payload_size = 32;
  //write_register(RX_PW_P0,rf24_min(payload_size,max_payload_size));
  WriteRegister(NRF24L01_REG_RX_PW_P0,NRF24L01_PAYLOAD);
}

/****************************************************************************/
static const uint8_t PROGMEM child_pipe[] =
{
		NRF24L01_REG_RX_ADDR_P0, NRF24L01_REG_RX_ADDR_P1, NRF24L01_REG_RX_ADDR_P2, NRF24L01_REG_RX_ADDR_P3, NRF24L01_REG_RX_ADDR_P4, NRF24L01_REG_RX_ADDR_P5
};
static const uint8_t PROGMEM child_payload_size[] =
{
		NRF24L01_REG_RX_PW_P0, NRF24L01_REG_RX_PW_P1, NRF24L01_REG_RX_PW_P2, NRF24L01_REG_RX_PW_P3, NRF24L01_REG_RX_PW_P4, NRF24L01_REG_RX_PW_P5
};
/****************************************************************************/
void NRF24L01::setAddressWidth(uint8_t a_width){

	if(a_width -= 2){
		WriteRegister(NRF24L01_REG_SETUP_AW,a_width%4);
		NRF24L01_ADDRSIZE = (a_width%4) + 2;
	}else{
		WriteRegister(NRF24L01_REG_SETUP_AW,0);
		NRF24L01_ADDRSIZE = 2;
    }

}
/****************************************************************************/
void NRF24L01::openReadingPipe(uint8_t child, const uint8_t *address){
  // If this is pipe 0, cache the address.  This is needed because
  // openWritingPipe() will overwrite the pipe 0 address, so
  // startListening() will have to restore it.
  if (child == 0){
    memcpy(pipe0_reading_address,address,NRF24L01_ADDRSIZE);
  }
  if (child <= 6){
    // For pipes 2-5, only write the LSB
    if ( child < 2 ){
    	WriteRegisters(pgm_read_byte(&child_pipe[child]), address, NRF24L01_ADDRSIZE);
    }else{
    	WriteRegisters(pgm_read_byte(&child_pipe[child]), address, 1);
	}
    WriteRegister(pgm_read_byte(&child_payload_size[child]),NRF24L01_PAYLOAD);

    // Note it would be more efficient to set all of the bits for all open
    // pipes at once.  However, I thought it would make the calling code
    // more simple to do it this way.
    WriteRegister(NRF24L01_REG_EN_RXADDR,ReadRegister(NRF24L01_REG_EN_RXADDR) | _BV(pgm_read_byte(&child_pipe_enable[child])));

  }
}
void NRF24L01::openReadingPipe(uint8_t child, uint64_t address){
  // If this is pipe 0, cache the address.  This is needed because
  // openWritingPipe() will overwrite the pipe 0 address, so
  // startListening() will have to restore it.
  if (child == 0){
    memcpy(pipe0_reading_address,&address,NRF24L01_ADDRSIZE);
  }

  if (child <= 6) {
    // For pipes 2-5, only write the LSB
    if ( child < 2 )
    	WriteRegisters(pgm_read_byte(&child_pipe[child]), reinterpret_cast<const uint8_t*>(&address), NRF24L01_ADDRSIZE);
    else
    	WriteRegisters(pgm_read_byte(&child_pipe[child]), reinterpret_cast<const uint8_t*>(&address), 1);

    WriteRegister(pgm_read_byte(&child_payload_size[child]),NRF24L01_PAYLOAD);

    // Note it would be more efficient to set all of the bits for all open
    // pipes at once.  However, I thought it would make the calling code
    // more simple to do it this way.
    WriteRegister(NRF24L01_REG_EN_RXADDR,ReadRegister(NRF24L01_REG_EN_RXADDR) | _BV(pgm_read_byte(&child_pipe_enable[child])));
  }
}
/****************************************************************************/
void NRF24L01::closeReadingPipe( uint8_t pipe ){
	WriteRegister(NRF24L01_REG_EN_RXADDR,ReadRegister(NRF24L01_REG_EN_RXADDR) & ~_BV(pgm_read_byte(&child_pipe_enable[pipe])));
}
/****************************************************************************/

void NRF24L01::ToggleFeatures(void){
    CSN_LOW();
    ioInterface->SPI_WriteReadbyte(NRF24L01_CMD_ACTIVATE);
    //spi.WriteReadbyte(NRF24L01_CMD_ACTIVATE );
    ioInterface->SPI_WriteReadbyte(0x73 );
   // spi.WriteReadbyte(0x73 );
	CSN_HIGH();
}
/****************************************************************************/
void NRF24L01::enableDynamicPayloads(void){
  // Enable dynamic payload throughout the system

    //toggle_features();
	WriteRegister(NRF24L01_REG_FEATURE,ReadRegister(NRF24L01_REG_FEATURE) | _BV(NRF24L01_REG_EN_DPL) );

  // Enable dynamic payload on all pipes
  //
  // Not sure the use case of only having dynamic payload on certain
  // pipes, so the library does not support it.
	WriteRegister(NRF24L01_REG_DYNPD,ReadRegister(NRF24L01_REG_DYNPD) | _BV(NRF24L01_REG_DPL_P5) | _BV(NRF24L01_REG_DPL_P4) | _BV(NRF24L01_REG_DPL_P3) | _BV(NRF24L01_REG_DPL_P2) | _BV(NRF24L01_REG_DPL_P1) | _BV(NRF24L01_REG_DPL_P0));

  dynamic_payloads_enabled = true;
}
/****************************************************************************/
void NRF24L01::disableDynamicPayloads(void){
  // Disables dynamic payload throughout the system.  Also disables Ack Payloads

  //toggle_features();
	WriteRegister(NRF24L01_REG_FEATURE,ReadRegister(NRF24L01_REG_FEATURE) & ~(_BV(NRF24L01_REG_EN_DPL) | _BV(NRF24L01_REG_EN_ACK_PAY)));

  // Disable dynamic payload on all pipes
  //
  // Not sure the use case of only having dynamic payload on certain
  // pipes, so the library does not support it.
  WriteRegister(NRF24L01_REG_DYNPD,ReadRegister(NRF24L01_REG_DYNPD) & ~( _BV(NRF24L01_REG_DPL_P5) | _BV(NRF24L01_REG_DPL_P4) | _BV(NRF24L01_REG_DPL_P3) | _BV(NRF24L01_REG_DPL_P2) | _BV(NRF24L01_REG_DPL_P1) | _BV(NRF24L01_REG_DPL_P0)));

  dynamic_payloads_enabled = false;
}
/****************************************************************************/
void NRF24L01::enableAckPayload(void){
  //
  // enable ack payload and dynamic payload features
  //

    //toggle_features();
	WriteRegister(NRF24L01_REG_FEATURE,ReadRegister(NRF24L01_REG_FEATURE) | _BV(NRF24L01_REG_EN_ACK_PAY) | _BV(NRF24L01_REG_EN_DPL) );
  //
  // Enable dynamic payload on pipes 0 & 1
  //

	WriteRegister(NRF24L01_REG_DYNPD,ReadRegister(NRF24L01_REG_DYNPD) | _BV(NRF24L01_REG_DPL_P1) | _BV(NRF24L01_REG_DPL_P0));
  dynamic_payloads_enabled = true;
}
/****************************************************************************/
void NRF24L01::enableDynamicAck(void){
  //
  // enable dynamic ack features
  //
    //toggle_features();
	WriteRegister(NRF24L01_REG_FEATURE,ReadRegister(NRF24L01_REG_FEATURE) | _BV(NRF24L01_REG_EN_DYN_ACK) );


}
/****************************************************************************/
void NRF24L01::writeAckPayload(uint8_t pipe, const void* buf, uint8_t len){
  const uint8_t* current = reinterpret_cast<const uint8_t*>(buf);
  uint8_t data_len = rf24_min(len,32);

  CSN_LOW();
  ioInterface->SPI_WriteReadbyte(NRF24L01_CMD_W_ACK_PAYLOAD | ( pipe & 0x07 ) );
  //spi.WriteReadbyte(NRF24L01_CMD_W_ACK_PAYLOAD | ( pipe & 0x07 ) );

  while ( data_len-- ){
	ioInterface->SPI_WriteReadbyte(*current++);
	  //spi.WriteReadbyte(*current++);
  }
  CSN_HIGH();



}
/****************************************************************************/
bool NRF24L01::isAckPayloadAvailable(void){
  return (!(ReadRegister(NRF24L01_REG_FIFO_STATUS) & _BV(NRF24L01_REG_RX_EMPTY)));
}
/****************************************************************************/
void NRF24L01::read(void* buf, uint8_t len){
	// Fetch the payload
	  ReadPayload( buf, len );

	  //Clear the two possible interrupt flags with one command
	  WriteRegister(NRF24L01_REG_STATUS,_BV(NRF24L01_REG_RX_DR) | _BV(NRF24L01_REG_MAX_RT) | _BV(NRF24L01_REG_TX_DS) );


}
/****************************************************************************/
bool NRF24L01::isPVariant(void){
  return (p_variant) ;
}
/****************************************************************************/
void NRF24L01::setAutoAck(bool enable){
  if ( enable )
	  WriteRegister(NRF24L01_REG_EN_AA, 0x3F);
  else
	  WriteRegister(NRF24L01_REG_EN_AA, 0);
}
/****************************************************************************/
void NRF24L01::setAutoAck( uint8_t pipe, bool enable ){
  if ( pipe <= 6 ){
    uint8_t en_aa = ReadRegister( NRF24L01_REG_EN_AA ) ;
    if( enable ){
      en_aa |= _BV(pipe) ;
    } else {
      en_aa &= ~_BV(pipe) ;
    }
    WriteRegister( NRF24L01_REG_EN_AA, en_aa ) ;
  }
}
/****************************************************************************/
bool NRF24L01::testCarrier(void){
  return ( ReadRegister(NRF24L01_REG_CD) & 1 );
}
/****************************************************************************/
bool NRF24L01::testRPD(void){
  return ( ReadRegister(RPD) & 1 ) ;
}
/****************************************************************************/
void NRF24L01::setPALevel(uint8_t level){

  uint8_t setup = ReadRegister(NRF24L01_REG_RF_SETUP) & 0xF8;

  if(level > 3){  						// If invalid level, go to max PA
	  level = (RF24_PA_MAX << 1) + 1;		// +1 to support the SI24R1 chip extra bit
  }else{
	  level = (level << 1) + 1;	 		// Else set level as requested
  }


  WriteRegister( NRF24L01_REG_RF_SETUP, setup |= level ) ;	// Write it to the chip
}
/****************************************************************************/
uint8_t NRF24L01::getPALevel(void){

  return ((ReadRegister(NRF24L01_REG_RF_SETUP) & (_BV(NRF24L01_REG_RF_PWR_LOW) | _BV(NRF24L01_REG_RF_PWR_HIGH))) >> 1 );
}
/****************************************************************************/
bool NRF24L01::setDataRate(uint8_t speed){
  bool result = false;
  uint8_t setup = ReadRegister(NRF24L01_REG_RF_SETUP) ;

  // HIGH and LOW '00' is 1Mbs - our default
  setup &= ~(_BV(NRF24L01_REG_RF_DR_LOW) | _BV(NRF24L01_REG_RF_DR_HIGH)) ;

 //16Mhz Arduino
    txDelay=85;

  if( speed == RF24_250KBPS ){
    // Must set the RF_DR_LOW to 1; RF_DR_HIGH (used to be RF_DR) is already 0
    // Making it '10'.
    setup |= _BV( NRF24L01_REG_RF_DR_LOW ) ;
   //16Mhz Arduino
	txDelay=155;

  } else {
    // Set 2Mbs, RF_DR (RF_DR_HIGH) is set 1
    // Making it '01'
    if ( speed == RF24_2MBPS ){
      setup |= _BV(NRF24L01_REG_RF_DR_HIGH);
       //16Mhz Arduino
	  txDelay=65;
    }
  }
  WriteRegister(NRF24L01_REG_RF_SETUP,setup);

  // Verify our result
  if ( ReadRegister(NRF24L01_REG_RF_SETUP) == setup ){
    result = true;
  }
  return (result);
}

/****************************************************************************/
rf24_datarate_e NRF24L01::getDataRate( void ){
  rf24_datarate_e result ;
  uint8_t dr = ReadRegister(NRF24L01_REG_RF_SETUP) & (_BV(NRF24L01_REG_RF_DR_LOW) | _BV(NRF24L01_REG_RF_DR_HIGH));

  // switch uses RAM (evil!)
  // Order matters in our case below
  if ( dr == _BV(NRF24L01_REG_RF_DR_LOW) ){
    // '10' = 250KBPS
    result = RF24_250KBPS ;
  }else if ( dr == _BV(NRF24L01_REG_RF_DR_HIGH) ) {
    // '01' = 2MBPS
    result = RF24_2MBPS ;
  }else {
    // '00' = 1MBPS
    result = RF24_1MBPS ;
  }
  return (result);
}
/****************************************************************************/
void NRF24L01::setCRCLength(uint8_t length){
  uint8_t config = ReadRegister(NRF24L01_REG_CONFIG) & ~( _BV(NRF24L01_REG_CRCO) | _BV(NRF24L01_REG_EN_CRC)) ;

  // switch uses RAM (evil!)
  if ( length == RF24_CRC_DISABLED ){
    // Do nothing, we turned it off above.
  }else if ( length == RF24_CRC_8 ) {
    config |= _BV(NRF24L01_REG_EN_CRC);
  } else {
    config |= _BV(NRF24L01_REG_EN_CRC);
    config |= _BV( NRF24L01_REG_CRCO );
  }
  WriteRegister( NRF24L01_REG_CONFIG, config ) ;
}
/****************************************************************************/
rf24_crclength_e NRF24L01::getCRCLength(void){
  rf24_crclength_e result = RF24_CRC_DISABLED;

  uint8_t config = ReadRegister(NRF24L01_REG_CONFIG) & ( _BV(NRF24L01_REG_CRCO) | _BV(NRF24L01_REG_EN_CRC)) ;
  uint8_t AA = ReadRegister(NRF24L01_REG_EN_AA);

  if ( (config & _BV(NRF24L01_REG_EN_CRC )) || AA) {
    if ( config & _BV(NRF24L01_REG_CRCO) )
      result = RF24_CRC_16;
    else
      result = RF24_CRC_8;
  }

  return (result);
}
/****************************************************************************/
void NRF24L01::disableCRC( void ){
  uint8_t disable = ReadRegister(NRF24L01_REG_CONFIG) & ~_BV(NRF24L01_REG_EN_CRC) ;
  WriteRegister( NRF24L01_REG_CONFIG, disable ) ;
}
/****************************************************************************/
void NRF24L01::setRetries(uint8_t delay, uint8_t count){
	WriteRegister(NRF24L01_REG_SETUP_RETR,(delay&0xf)<<NRF24L01_REG_ARD | (count&0xf)<<NRF24L01_REG_ARC);
}

/******************************************************************/
//Similar to the previous write, clears the interrupt flags
bool NRF24L01::write(const void* buf, uint8_t len, const bool multicast){
	//Start Writing
		startFastWrite(buf,len,multicast);

		//stop if max_retries reached or send is ok

		while( !(getStatus() & (1<<NRF24L01_REG_MAX_RT | 1<<NRF24L01_REG_TX_DS)) );

		CE_LOW(); //low CE

		uint8_t status = WriteRegister(NRF24L01_REG_STATUS,_BV(NRF24L01_REG_MASK_RX_DR) | _BV(NRF24L01_REG_TX_DS) | _BV(NRF24L01_REG_MAX_RT) );


		//Max retries exceeded
		  if( status & _BV(NRF24L01_REG_MAX_RT)){
		  	FlushTx(); //Only going to be 1 packet int the FIFO at a time using this method, so just flush
		  	return (0);
		  }



		return (1);
}

bool NRF24L01::write( const void* buf, uint8_t len ){
	return (write(buf,len,0));
}
/****************************************************************************/

//For general use, the interrupt flags are not important to clear
bool NRF24L01::writeBlocking( const void* buf, uint8_t len, uint32_t timeout ){
	//Block until the FIFO is NOT full.
	//Keep track of the MAX retries and set auto-retry if seeing failures
	//This way the FIFO will fill up and allow blocking until packets go through
	//The radio will auto-clear everything in the FIFO as long as CE remains high

						  //Get the time that the payload transmission started

	while( ( getStatus()  & ( _BV(NRF24L01_REG_TX_FULL) ))) {		  //Blocking only if FIFO is full. This will loop and block until TX is successful or timeout

		if( getStatus() & _BV(NRF24L01_REG_MAX_RT)){					  //If MAX Retries have been reached
			reUseTX();										  //Set re-transmit and clear the MAX_RT interrupt flag

		}


  	}

  	//Start Writing
	startFastWrite(buf,len,0);								  //Write the payload if a buffer is clear

	return (1);												  //Return 1 to indicate successful transmission
}
/****************************************************************************/

void NRF24L01::reUseTX(){
	WriteRegister(NRF24L01_REG_STATUS,_BV(NRF24L01_REG_MAX_RT) );			  //Clear max retry flag
		spiTrans( NRF24L01_CMD_REUSE_TX_PL );
		CE_LOW();										  //Re-Transfer packet
		CE_HIGH();
}

/****************************************************************************/
bool NRF24L01::writeFast( const void* buf, uint8_t len, const bool multicast ){
	//Block until the FIFO is NOT full.
	//Keep track of the MAX retries and set auto-retry if seeing failures
	//Return 0 so the user can control the retrys and set a timer or failure counter if required
	//The radio will auto-clear everything in the FIFO as long as CE remains high

	while( ( getStatus()  & ( _BV(NRF24L01_REG_TX_FULL) ))) {			  //Blocking only if FIFO is full. This will loop and block until TX is successful or fail

		if( getStatus() & _BV(NRF24L01_REG_MAX_RT)){
			//reUseTX();										  //Set re-transmit
			WriteRegister(NRF24L01_REG_STATUS,_BV(NRF24L01_REG_MAX_RT) );			  //Clear max retry flag
			return (0);										  //Return 0. The previous payload has been retransmitted
															  //From the user perspective, if you get a 0, just keep trying to send the same payload
		}

  	}
		     //Start Writing
	startFastWrite(buf,len,multicast);

	return (1);
}
bool NRF24L01::writeFast( const void* buf, uint8_t len ){
	return (writeFast(buf,len,0));
}
/****************************************************************************/
//Per the documentation, we want to set PTX Mode when not listening. Then all we do is write data and set CE high
//In this mode, if we can keep the FIFO buffers loaded, packets will transmit immediately (no 130us delay)
//Otherwise we enter Standby-II mode, which is still faster than standby mode
//Also, we remove the need to keep writing the config register over and over and delaying for 150 us each time if sending a stream of data
void NRF24L01::startFastWrite( const void* buf, uint8_t len, const bool multicast, bool startTx){ //TMRh20

	//write_payload( buf,len);
	WritePayload(buf, len,multicast ? NRF24L01_CMD_W_TX_PAYLOAD_NO_ACK : NRF24L01_CMD_W_TX_PAYLOAD ) ;
	if(startTx){
		CE_HIGH();
	}

}
/****************************************************************************/
//Added the original startWrite back in so users can still use interrupts, ack payloads, etc
//Allows the library to pass all tests
void NRF24L01::startWrite( const void* buf, uint8_t len, const bool multicast ){

  // Send the payload

  //write_payload( buf, len );
	WritePayload( buf, len,multicast? NRF24L01_CMD_W_TX_PAYLOAD_NO_ACK : NRF24L01_CMD_W_TX_PAYLOAD ) ;
	CE_HIGH();

	CE_LOW();
}
/****************************************************************************/

bool NRF24L01::rxFifoFull(){
	return (ReadRegister(NRF24L01_REG_FIFO_STATUS) & _BV(NRF24L01_REG_RX_FULL));
}
/****************************************************************************/

bool NRF24L01::txStandBy(){


	while( ! (ReadRegister(NRF24L01_REG_FIFO_STATUS) & _BV(NRF24L01_REG_TX_EMPTY)) ){
		if( getStatus() & _BV(NRF24L01_REG_MAX_RT)){
			WriteRegister(NRF24L01_REG_STATUS,_BV(NRF24L01_REG_MAX_RT) );
			CE_LOW();
			FlushTx();    //Non blocking, flush the data
			return (0);
		}

	}

	CE_LOW();			   //Set STANDBY-I mode
	return (1);
}
/****************************************************************************/
/**
bool NRF24L01::txStandBy(uint32_t timeout, bool startTx){

    if(startTx){
	  stopListening();
	  CE_HIGH();
	}
	uint32_t start = millis();

	while( ! (ReadRegister(NRF24L01_REG_FIFO_STATUS) & _BV(NRF24L01_REG_TX_EMPTY)) ){
		if( GetStatus() & _BV(NRF24L01_REG_MAX_RT)){
			WriteRegister(NRF24L01_REG_STATUS,_BV(NRF24L01_REG_MAX_RT) );
			CE_LOW();									  //Set re-transmit
			CE_HIGH();
				if(millis() - start >= timeout){
					CE_LOW(); FlushTx(); return 0;
				}
		}

	}


	CE_LOW();				   //Set STANDBY-I mode
	return 1;

}*/

/****************************************************************************/

void NRF24L01::maskIRQ(bool tx, bool fail, bool rx){

	uint8_t config = ReadRegister(NRF24L01_REG_CONFIG);
	/* clear the interrupt flags */
	config &= ~(1 << NRF24L01_REG_MASK_MAX_RT | 1 << NRF24L01_REG_MASK_TX_DS | 1 << NRF24L01_REG_MASK_RX_DR);
	/* set the specified interrupt flags */
	config |= fail << NRF24L01_REG_MASK_MAX_RT | tx << NRF24L01_REG_MASK_TX_DS | rx << NRF24L01_REG_MASK_RX_DR;
	WriteRegister(NRF24L01_REG_CONFIG, config);
}
/****************************************************************************/

uint8_t NRF24L01::getDynamicPayloadSize(void)
{
  uint8_t result = 0;

  CSN_LOW();
  ioInterface->SPI_WriteReadbyte(NRF24L01_CMD_R_RX_PL_WID );
  //spi.WriteReadbyte( NRF24L01_CMD_R_RX_PL_WID );
  result = ioInterface->SPI_WriteReadbyte(0xff);
  //result = spi.WriteReadbyte(0xff);
  CSN_HIGH();


  if(result > 32) { FlushRx(); delay_ms(2); return (0); }
  return (result);
}
/****************************************************************************/

bool NRF24L01::available(void){
  return (available(NULL));
}

/****************************************************************************/
bool NRF24L01::available(uint8_t* pipe_num){
  if (!( ReadRegister(NRF24L01_REG_FIFO_STATUS) & _BV(NRF24L01_REG_RX_EMPTY) )){

    // If the caller wants the pipe number, include that
    if ( pipe_num ){
	  uint8_t status = getStatus();
      *pipe_num = ( status >> NRF24L01_REG_RX_P_NO ) & 0x07;
  	}
  	return (1);
  }

  return (0);

}

/*** read one register **************************************/
uint8_t NRF24L01::ReadRegister(uint8_t reg) {
	//u.writeString("In read Regs...\r\n");

	CSN_LOW(); //low CSN
	ioInterface->SPI_WriteReadbyte(NRF24L01_CMD_R_REGISTER | (NRF24L01_CMD_REGISTER_MASK & reg));
//	spi.WriteReadbyte(NRF24L01_CMD_R_REGISTER | (NRF24L01_CMD_REGISTER_MASK & reg));
	uint8_t result = ioInterface->SPI_WriteReadbyte(NRF24L01_CMD_NOP); //read write register
	//uint8_t result = spi.WriteReadbyte(NRF24L01_CMD_NOP); //read write register
    CSN_HIGH(); //high CSN
    return (result);
}

/*** read many registers ***********************************/
uint8_t NRF24L01::ReadRegisters(uint8_t reg, uint8_t *value, uint8_t len) {

	uint8_t status;

	CSN_LOW(); //low CSN
	status = ioInterface->SPI_WriteReadbyte(NRF24L01_CMD_R_REGISTER | (NRF24L01_CMD_REGISTER_MASK & reg));
//	status = spi.WriteReadbyte(NRF24L01_CMD_R_REGISTER | (NRF24L01_CMD_REGISTER_MASK & reg));
	while ( len-- ){
		*value++ = ioInterface->SPI_WriteReadbyte(NRF24L01_CMD_NOP); //read write register
	  //*value++ = spi.WriteReadbyte(NRF24L01_CMD_NOP); //read write register
	}
	CSN_HIGH(); //high CSN
	return (status);
}

/*** write one register *************************************/
uint8_t NRF24L01::WriteRegister(uint8_t reg, uint8_t value) {
	uint8_t status;
	CSN_LOW(); //low CSN
	status = ioInterface->SPI_WriteReadbyte(NRF24L01_CMD_W_REGISTER | (NRF24L01_CMD_REGISTER_MASK & reg));
//	status = spi.WriteReadbyte(NRF24L01_CMD_W_REGISTER | (NRF24L01_CMD_REGISTER_MASK & reg));
	ioInterface->SPI_WriteReadbyte(value); //write register
	//spi.WriteReadbyte(value); //write register
	CSN_HIGH(); //high CSN
	return (status);
}

/*** write many registers ***********************************/
uint8_t NRF24L01::WriteRegisters(uint8_t reg, const uint8_t *value, uint8_t len) {
	uint8_t status;
	CSN_LOW(); //low CSN
	status = ioInterface->SPI_WriteReadbyte(NRF24L01_CMD_W_REGISTER | (NRF24L01_CMD_REGISTER_MASK & reg));
    //status = spi.WriteReadbyte(NRF24L01_CMD_W_REGISTER | (NRF24L01_CMD_REGISTER_MASK & reg));
    while ( len-- ){
		ioInterface->SPI_WriteReadbyte(*value++); //write register
		//spi.WriteReadbyte(*value++); //write register
    }
    CSN_HIGH(); //high CSN
	return (status);
}

/*************************************************************/
uint8_t NRF24L01::WritePayload(const void* buf, uint8_t data_len, const uint8_t writeType){

	uint8_t status;
	const uint8_t* current = reinterpret_cast<const uint8_t*>(buf);
	data_len = rf24_min(data_len, NRF24L01_PAYLOAD);
	uint8_t blank_len = dynamic_payloads_enabled ? 0 : NRF24L01_PAYLOAD - data_len;

	CSN_LOW(); //low CSN
	status = ioInterface->SPI_WriteReadbyte( writeType );
	//status = spi.WriteReadbyte( writeType );
	  while ( data_len-- ) {
		  ioInterface->SPI_WriteReadbyte(*current++);
		  //spi.WriteReadbyte(*current++);
	  }
	  while ( blank_len-- ) {
		  ioInterface->SPI_WriteReadbyte(0);
		  //spi.WriteReadbyte(0);
	  }

	CSN_HIGH(); //high CSN
	return (status);
}
/****************************************************************************/
uint8_t NRF24L01::ReadPayload(void* buf, uint8_t data_len){
	 uint8_t status;
	 uint8_t* current = reinterpret_cast<uint8_t*>(buf);
	 if(data_len > NRF24L01_PAYLOAD) data_len = NRF24L01_PAYLOAD;
	 uint8_t blank_len = dynamic_payloads_enabled ? 0 : NRF24L01_PAYLOAD - data_len;


	 CSN_LOW(); //low CSN
		status = ioInterface->SPI_WriteReadbyte( NRF24L01_CMD_R_RX_PAYLOAD );
	 	//status = spi.WriteReadbyte( NRF24L01_CMD_R_RX_PAYLOAD );
	  while ( data_len-- ) {
	    *current++ = ioInterface->SPI_WriteReadbyte(0xFF);
	    //*current++ = spi.WriteReadbyte(0xFF);
	  }
	  while ( blank_len-- ) {
		  ioInterface->SPI_WriteReadbyte(0xff);
		  //spi.WriteReadbyte(0xff);
	  }
	  CSN_HIGH(); //high CSN
	  	return (status);

}


/*** flush RX fifo ******************************************/
uint8_t NRF24L01::FlushRx() {
	return (spiTrans(NRF24L01_CMD_FLUSH_RX));

}
/*** flush TX fifo **************************************************/
uint8_t NRF24L01::FlushTx(){

	return (spiTrans(NRF24L01_CMD_FLUSH_TX));

}

/****************************************************************************/
uint8_t NRF24L01::spiTrans(uint8_t cmd){

  uint8_t status;

  CSN_LOW(); //low CSN
  status = ioInterface->SPI_WriteReadbyte( cmd );
  //status = spi.WriteReadbyte( cmd );
  CSN_HIGH(); //high CSN

  return (status);
}





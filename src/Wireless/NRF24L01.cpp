/*
 * NRF24L01.cpp
 *
 *  Created on: Nov 14, 2016
 *      Author: Matthias Minx
 *
 * 	based on the Code of:
 *
 * nrf24l01 lib 0x02
 * copyright (c) Davide Gironi, 2012
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

#include "../IO/SPI.h"





NRF24L01::NRF24L01() {
	// TODO Auto-generated constructor stub
	//spi = SPI(DDRB,PORTB,PB4,PB3,PB5,PB2);

	NRF24L01_DDR = &DDRB;
//	NRF24L01_PORT = &PORTB;
	NRF24L01_CE = PB1;
	NRF24L01_CSN = PB0;

	//transmission channel
	SetChannel(124);
		//payload lenght
	SetPayloadSize(16);
		//auto ack enabled
	EnableAutoAck();



	//enable / disable pipe
	EnablePipe(0);
	EnablePipe(1);
	EnablePipe(2);
	EnablePipe(3);
	EnablePipe(4);
	EnablePipe(5);

	NRF24L01_ADDRSIZE = 5;





	//uint8_t a0[5] = { 0xe0, 0xf0, 0xf0, 0xf0, 0xf0 };

	SetAddress(nrf24l01_addr0,0xe0, 0xf0, 0xf0, 0xf0, 0xf0);
	SetAddress(nrf24l01_addr1,0xe1, 0xf0, 0xf0, 0xf0, 0xe0);
	SetAddress(nrf24l01_addr2,0xe1, 0xf0, 0xf0, 0xf0, 0xe1);
	SetAddress(nrf24l01_addr3,0xe1, 0xf0, 0xf0, 0xf0, 0xe2);
	SetAddress(nrf24l01_addr4,0xe1, 0xf0, 0xf0, 0xf0, 0xe3);
	SetAddress(nrf24l01_addr5,0xe1, 0xf0, 0xf0, 0xf0, 0xe4);
	SetAddress(nrf24l01_addrtx,0xe0, 0xf0, 0xf0, 0xf0, 0xf0);

/*
	uint8_t a1[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xe0};
	uint8_t a2[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xe1};
	uint8_t a3[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xe2};
	uint8_t a4[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xe3};
	uint8_t a5[5] =	{0xe1, 0xf0, 0xf0, 0xf0, 0xe4};
	uint8_t aT[5] =	{0xe0, 0xf0, 0xf0, 0xf0, 0xf0};

	SetAddresses(a0,a1,a2,a3,a4,a5,aT);
*/

	//power setup
	SetPowerLevel(NRF24L01_RF24_PA_MIN);

	//speed setup
	SetDataRate(NRF24L01_RF24_SPEED_1MBPS);

	//crc setup
	SetCRClength(NRF24L01_RF24_CRC_16);

//	u = USART();

}

NRF24L01::NRF24L01(volatile uint8_t &DDR, volatile uint8_t &PORT, uint8_t CE, uint8_t CSN){
// DDR, PORT, CE,CSN
	//u = USART();
	//spi = SPI(DDRB,PORTB,PB4,PB3,PB5,PB2);
	NRF24L01_DDR = &DDR;
//	NRF24L01_PORT = &Wire_PORT;
	NRF24L01_CE = CE;
	NRF24L01_CSN = CSN;

	//transmission channel
	NRF24L01_CH = 124;
			//payload lenght
	NRF24L01_PAYLOAD = 16;
			//auto ack enabled
	NRF24L01_ACK = 1;



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

	SetAddresses(a0,a1,a2,a3,a4,a5,aT);




}

void NRF24L01::Init(){
	//setup port
		*NRF24L01_DDR |= (1<<NRF24L01_CSN); //output
		*NRF24L01_DDR |= (1<<NRF24L01_CE); //output



	 //   init spi
	    spi.Init();


	    CE_LOW(); //low CE
	    CSN_HIGH(); //high CSN


	    _delay_ms(5); //wait for the radio to init


	    SetPowerLevel(); //set power level

	    SetDataRate(); //set data rate

	    SetCRClength(); //set crc length


	    WriteRegister(NRF24L01_REG_SETUP_RETR, NRF24L01_RETR); // set retries
	    WriteRegister(NRF24L01_REG_DYNPD, 0x03); //enable dynamic payloads
	    WriteRegister(NRF24L01_REG_RF_CH, NRF24L01_CH); //set RF channel

	 //   u.writeString("write regs done\r\n");

		//payload size
		if(NRF24L01_ENABLEDP0){
			WriteRegister(NRF24L01_REG_RX_PW_P0, NRF24L01_PAYLOAD);
		}
		if(NRF24L01_ENABLEDP1){
			WriteRegister(NRF24L01_REG_RX_PW_P1, NRF24L01_PAYLOAD);
		}
		if(NRF24L01_ENABLEDP2){
			WriteRegister(NRF24L01_REG_RX_PW_P2, NRF24L01_PAYLOAD);
		}
		if(NRF24L01_ENABLEDP3){
			WriteRegister(NRF24L01_REG_RX_PW_P3, NRF24L01_PAYLOAD);
		}
		if(NRF24L01_ENABLEDP4){
			WriteRegister(NRF24L01_REG_RX_PW_P4, NRF24L01_PAYLOAD);
		}
		if(NRF24L01_ENABLEDP5){
			WriteRegister(NRF24L01_REG_RX_PW_P5, NRF24L01_PAYLOAD);
		}

		//enable pipe
		WriteRegister(NRF24L01_REG_EN_RXADDR, 0);

		if(NRF24L01_ENABLEDP0){
			WriteRegister(NRF24L01_REG_EN_RXADDR, ReadRegister(NRF24L01_REG_EN_RXADDR) | (1<<NRF24L01_REG_ERX_P0));
		}
		if(NRF24L01_ENABLEDP1) {
			WriteRegister(NRF24L01_REG_EN_RXADDR, ReadRegister(NRF24L01_REG_EN_RXADDR) | (1<<NRF24L01_REG_ERX_P1));
		}
		if(NRF24L01_ENABLEDP2){
			WriteRegister(NRF24L01_REG_EN_RXADDR, ReadRegister(NRF24L01_REG_EN_RXADDR) | (1<<NRF24L01_REG_ERX_P2));
		}
		if(NRF24L01_ENABLEDP3){
			WriteRegister(NRF24L01_REG_EN_RXADDR, ReadRegister(NRF24L01_REG_EN_RXADDR) | (1<<NRF24L01_REG_ERX_P3));
		}
		if(NRF24L01_ENABLEDP4){
			WriteRegister(NRF24L01_REG_EN_RXADDR, ReadRegister(NRF24L01_REG_EN_RXADDR) | (1<<NRF24L01_REG_ERX_P4));
		}
		if(NRF24L01_ENABLEDP5){
			WriteRegister(NRF24L01_REG_EN_RXADDR, ReadRegister(NRF24L01_REG_EN_RXADDR) | (1<<NRF24L01_REG_ERX_P5));
		}

		//auto ack
		if(NRF24L01_ACK ){
			WriteRegister(NRF24L01_REG_EN_AA, ReadRegister(NRF24L01_REG_EN_AA) | (1<<NRF24L01_REG_ENAA_P0));
			WriteRegister(NRF24L01_REG_EN_AA, ReadRegister(NRF24L01_REG_EN_AA) | (1<<NRF24L01_REG_ENAA_P1));
			WriteRegister(NRF24L01_REG_EN_AA, ReadRegister(NRF24L01_REG_EN_AA) | (1<<NRF24L01_REG_ENAA_P2));
			WriteRegister(NRF24L01_REG_EN_AA, ReadRegister(NRF24L01_REG_EN_AA) | (1<<NRF24L01_REG_ENAA_P3));
			WriteRegister(NRF24L01_REG_EN_AA, ReadRegister(NRF24L01_REG_EN_AA) | (1<<NRF24L01_REG_ENAA_P4));
			WriteRegister(NRF24L01_REG_EN_AA, ReadRegister(NRF24L01_REG_EN_AA) | (1<<NRF24L01_REG_ENAA_P5));
		}else{
			WriteRegister(NRF24L01_REG_EN_AA, ReadRegister(NRF24L01_REG_EN_AA) & ~(1<<NRF24L01_REG_ENAA_P0));
			WriteRegister(NRF24L01_REG_EN_AA, ReadRegister(NRF24L01_REG_EN_AA) & ~(1<<NRF24L01_REG_ENAA_P1));
			WriteRegister(NRF24L01_REG_EN_AA, ReadRegister(NRF24L01_REG_EN_AA) & ~(1<<NRF24L01_REG_ENAA_P2));
			WriteRegister(NRF24L01_REG_EN_AA, ReadRegister(NRF24L01_REG_EN_AA) & ~(1<<NRF24L01_REG_ENAA_P3));
			WriteRegister(NRF24L01_REG_EN_AA, ReadRegister(NRF24L01_REG_EN_AA) & ~(1<<NRF24L01_REG_ENAA_P4));
			WriteRegister(NRF24L01_REG_EN_AA, ReadRegister(NRF24L01_REG_EN_AA) & ~(1<<NRF24L01_REG_ENAA_P5));
		}

		//rx address
		SetRxAddr(0, nrf24l01_addr0);
		SetRxAddr(1, nrf24l01_addr1);
		SetRxAddr(2, nrf24l01_addr2);
		SetRxAddr(3, nrf24l01_addr3);
		SetRxAddr(4, nrf24l01_addr4);
		SetRxAddr(5, nrf24l01_addr5);

		//tx address
		SetTxAddr(nrf24l01_addrtx);

		//set rx mode
		SetRX();
}

uint8_t NRF24L01::GetStatus(){
	uint8_t status = 0;
	CSN_LOW(); //low CSN
	status = spi.WriteReadbyte(NRF24L01_CMD_NOP); //get status, send NOP request
	CSN_HIGH(); //high CSN

return status;
}

void NRF24L01::SetChannel(uint8_t channel){
	NRF24L01_CH = channel;
}
void NRF24L01::SetPayloadSize(uint8_t payload){
	NRF24L01_PAYLOAD = payload;
}
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
void NRF24L01::Read(uint8_t *data){
	uint8_t i = 0;
	//read rx register
	CSN_LOW(); //low CSN
	spi.WriteReadbyte(NRF24L01_CMD_R_RX_PAYLOAD);
	for(i=0; i<NRF24L01_PAYLOAD; i++){
	    data[i] = spi.WriteReadbyte(NRF24L01_CMD_NOP);
	}
	CSN_HIGH(); //high CSN
	//reset register
	WriteRegister(NRF24L01_REG_STATUS, (1<<NRF24L01_REG_RX_DR));
	//handle ack payload receipt
	if (GetStatus() & (1<<NRF24L01_REG_TX_DS)){
			WriteRegister(NRF24L01_REG_STATUS, (1<<NRF24L01_REG_TX_DS));
	}

}
uint8_t NRF24L01::Write(uint8_t *data){
	uint8_t i = 0;
		uint8_t ret = 0;

		//set tx mode
		SetTX();

		//write data
		CSN_LOW(); //low CSN
		spi.WriteReadbyte(NRF24L01_CMD_W_TX_PAYLOAD);
		for (i=0; i<NRF24L01_PAYLOAD; i++)
			spi.WriteReadbyte(data[i]);
		CSN_HIGH(); //high CSN

		//start transmission
		CE_HIGH(); //high CE
		_delay_us(15);
		CE_LOW(); //low CE

		//stop if max_retries reached or send is ok
		do {
			_delay_us(10);
		}
		while( !(GetStatus() & (1<<NRF24L01_REG_MAX_RT | 1<<NRF24L01_REG_TX_DS)) );

		if(GetStatus() & 1<<NRF24L01_REG_TX_DS)
			ret = 1;

		//reset PLOS_CNT
		WriteRegister(NRF24L01_REG_RF_CH, NRF24L01_CH);

		//power down
		WriteRegister(NRF24L01_REG_CONFIG, ReadRegister(NRF24L01_REG_CONFIG) & ~(1<<NRF24L01_REG_PWR_UP));

		//set rx mode
		SetRX();

		return ret;
}


/*
 * read one register
 */
uint8_t NRF24L01::ReadRegister(uint8_t reg) {
	//u.writeString("In read Regs...\r\n");
	CSN_LOW(); //low CSN
	spi.WriteReadbyte(NRF24L01_CMD_R_REGISTER | (NRF24L01_CMD_REGISTER_MASK & reg));
    uint8_t result = spi.WriteReadbyte(NRF24L01_CMD_NOP); //read write register
    CSN_HIGH(); //high CSN
    return result;
}

/*
 * read many registers
 */
void NRF24L01::ReadRegisters(uint8_t reg, uint8_t *value, uint8_t len) {
	uint8_t i = 0;
	CSN_LOW(); //low CSN
	spi.WriteReadbyte(NRF24L01_CMD_R_REGISTER | (NRF24L01_CMD_REGISTER_MASK & reg));
	for(i=0; i<len; i++)
		value[i] = spi.WriteReadbyte(NRF24L01_CMD_NOP); //read write register
	CSN_HIGH(); //high CSN
}

/*
 * write one register
 */
void NRF24L01::WriteRegister(uint8_t reg, uint8_t value) {
	CSN_LOW(); //low CSN
	spi.WriteReadbyte(NRF24L01_CMD_W_REGISTER | (NRF24L01_CMD_REGISTER_MASK & reg));
	spi.WriteReadbyte(value); //write register
	CSN_HIGH(); //high CSN
}

/*
 * write many registers
 */
void NRF24L01::WriteRegisters(uint8_t reg, uint8_t *value, uint8_t len) {
	uint8_t i = 0;
	CSN_LOW(); //low CSN
    spi.WriteReadbyte(NRF24L01_CMD_W_REGISTER | (NRF24L01_CMD_REGISTER_MASK & reg));
	for(i=0; i<len; i++)
		spi.WriteReadbyte(value[i]); //write register
	CSN_HIGH(); //high CSN
}


/*
 * reverse an array, NRF24L01 expects LSB first
 */
void NRF24L01::ReverseAddress(uint8_t *addr, uint8_t *addrrev) {
	//reverse address
	uint8_t i = 0;
	for(i=0; i<NRF24L01_ADDRSIZE; i++)
		memcpy(&addrrev[i], &addr[NRF24L01_ADDRSIZE-1-i], 1);
}

void NRF24L01::SetRxAddr(uint8_t pipe, uint8_t *addr){
	if(pipe == 0) {
			memcpy(&nrf24l01_addr0, addr, NRF24L01_ADDRSIZE); //cache address
			uint8_t addrrev[NRF24L01_ADDRSIZE];
			ReverseAddress(addr, (uint8_t *)addrrev);
			WriteRegisters(NRF24L01_REG_RX_ADDR_P0, addrrev, NRF24L01_ADDRSIZE);
		} else if(pipe == 1) {
			memcpy(&nrf24l01_addr1, addr, NRF24L01_ADDRSIZE); //cache address
			uint8_t addrrev[NRF24L01_ADDRSIZE];
			ReverseAddress(addr, (uint8_t *)addrrev);
			WriteRegisters(NRF24L01_REG_RX_ADDR_P1, addrrev, NRF24L01_ADDRSIZE);
		} else if(pipe == 2) {
			memcpy(&nrf24l01_addr2, addr, NRF24L01_ADDRSIZE); //cache address
			WriteRegister(NRF24L01_REG_RX_ADDR_P2, addr[NRF24L01_ADDRSIZE-1]); //write only LSB MSBytes are equal to RX_ADDR_P
		} else if(pipe == 3) {
			memcpy(&nrf24l01_addr3, addr, NRF24L01_ADDRSIZE); //cache address
			WriteRegister(NRF24L01_REG_RX_ADDR_P3, addr[NRF24L01_ADDRSIZE-1]); //write only LSB MSBytes are equal to RX_ADDR_P
		} else if(pipe == 4) {
			memcpy(&nrf24l01_addr4, addr, NRF24L01_ADDRSIZE); //cache address
			WriteRegister(NRF24L01_REG_RX_ADDR_P4, addr[NRF24L01_ADDRSIZE-1]); //write only LSB MSBytes are equal to RX_ADDR_P
		} else if(pipe == 5) {
			memcpy(&nrf24l01_addr5, addr, NRF24L01_ADDRSIZE); //cache address
			WriteRegister(NRF24L01_REG_RX_ADDR_P5, addr[NRF24L01_ADDRSIZE-1]); //write only LSB MSBytes are equal to RX_ADDR_P
		}
}

void NRF24L01::SetTxAddr(uint8_t *addr){
	memcpy(&nrf24l01_addrtx, addr, NRF24L01_ADDRSIZE); //cache address
	uint8_t addrrev[NRF24L01_ADDRSIZE];
	ReverseAddress(addr, (uint8_t *)addrrev);
	WriteRegisters(NRF24L01_REG_RX_ADDR_P0, addrrev, NRF24L01_ADDRSIZE); //set rx address for ack on pipe 0
	WriteRegisters(NRF24L01_REG_TX_ADDR, addrrev, NRF24L01_ADDRSIZE); //set tx address
}

void NRF24L01::SetPowerLevel(){
 //set power level
	uint8_t setup = ReadRegister(NRF24L01_REG_RF_SETUP);
	  setup &= ~((1<<NRF24L01_REG_RF_PWR_LOW) | (1<<NRF24L01_REG_RF_PWR_HIGH));

	  if (NRF24L01_RF24_PA == NRF24L01_RF24_PA_MAX) {
		  setup |= (1<<NRF24L01_REG_RF_PWR_LOW) | (1<<NRF24L01_REG_RF_PWR_HIGH);
	  } else if (NRF24L01_RF24_PA == NRF24L01_RF24_PA_HIGH) {
		  setup |= (1<<NRF24L01_REG_RF_PWR_HIGH) ;
	  } else if (NRF24L01_RF24_PA == NRF24L01_RF24_PA_LOW) {
		  setup |= (1<<NRF24L01_REG_RF_PWR_LOW);
	  } else if (NRF24L01_RF24_PA == NRF24L01_RF24_PA_MIN) {
	  } else {
		  //default is max power
		  setup |= (1<<NRF24L01_REG_RF_PWR_LOW) | (1<<NRF24L01_REG_RF_PWR_HIGH);
	  }

	  WriteRegister(NRF24L01_REG_RF_SETUP, setup);

}
void NRF24L01::SetDataRate(){
//set data rate
	 uint8_t setup = ReadRegister(NRF24L01_REG_RF_SETUP) ;

	  setup &= ~((1<<NRF24L01_REG_RF_DR_LOW) | (1<<NRF24L01_REG_RF_DR_HIGH));
	  if(NRF24L01_RF24_SPEED == NRF24L01_RF24_SPEED_250KBPS) {
	    setup |= (1<<NRF24L01_REG_RF_DR_LOW);
	  } else {
	    if (NRF24L01_RF24_SPEED == NRF24L01_RF24_SPEED_2MBPS) {
	    	setup |= (1<<NRF24L01_REG_RF_DR_HIGH);
	    } else if (NRF24L01_RF24_SPEED == NRF24L01_RF24_SPEED_2MBPS) {
	    } else {
	    	//default is 1Mbps
	    }
	  }

	  WriteRegister(NRF24L01_REG_RF_SETUP, setup);


}
void NRF24L01::SetCRClength(){
//set crc length
	uint8_t config = ReadRegister(NRF24L01_REG_CONFIG) & ~((1<<NRF24L01_REG_CRCO) | (1<<NRF24L01_REG_EN_CRC));

	  if (NRF24L01_RF24_CRC == NRF24L01_RF24_CRC_DISABLED) {
		  //nothing
	  } else if (NRF24L01_RF24_CRC == NRF24L01_RF24_CRC_8) {
		  config |= (1<<NRF24L01_REG_EN_CRC);
	  } else if (NRF24L01_RF24_CRC == NRF24L01_RF24_CRC_16) {
		  config |= (1<<NRF24L01_REG_EN_CRC);
		  config |= (1<<NRF24L01_REG_CRCO);
	  } else {
		  //default is disabled
	  }

	  WriteRegister(NRF24L01_REG_CONFIG, config);

}

/*
 * flush RX fifo
 */
void NRF24L01::FlushRXfifo() {
	CSN_LOW(); //low CSN
	spi.WriteReadbyte(NRF24L01_CMD_FLUSH_RX);
	CSN_HIGH(); //high CSN
}
/*
 * flush RX fifo
 */
void NRF24L01::FlushTXfifo() {
	CSN_LOW(); //low CSN
	spi.WriteReadbyte(NRF24L01_CMD_FLUSH_TX);
	CSN_HIGH(); //high CSN
}
void NRF24L01::SetRX() {
	SetRxAddr(0, nrf24l01_addr0); //restore pipe 0 address
	WriteRegister(NRF24L01_REG_CONFIG, ReadRegister(NRF24L01_REG_CONFIG) | (1<<NRF24L01_REG_PRIM_RX)); //prx mode
	WriteRegister(NRF24L01_REG_CONFIG, ReadRegister(NRF24L01_REG_CONFIG) | (1<<NRF24L01_REG_PWR_UP)); //power up
	WriteRegister(NRF24L01_REG_STATUS, (1<<NRF24L01_REG_RX_DR) | (1<<NRF24L01_REG_TX_DS) | (1<<NRF24L01_REG_MAX_RT)); //reset status
	FlushRXfifo(); //flush rx fifo
	FlushTXfifo(); //flush tx fifo
	CE_HIGH(); //start listening
	_delay_us(150); //wait for the radio to power up
}
void NRF24L01::SetTX() {
	CE_LOW(); //stop listening
	WriteRegister(NRF24L01_REG_CONFIG, ReadRegister(NRF24L01_REG_CONFIG) & ~(1<<NRF24L01_REG_PRIM_RX)); //ptx mode
	WriteRegister(NRF24L01_REG_CONFIG, ReadRegister(NRF24L01_REG_CONFIG) | (1<<NRF24L01_REG_PWR_UP)); //power up
	WriteRegister(NRF24L01_REG_STATUS, (1<<NRF24L01_REG_RX_DR) | (1<<NRF24L01_REG_TX_DS) | (1<<NRF24L01_REG_MAX_RT)); //reset status
	FlushTXfifo(); //flush tx fifo
	_delay_us(150); //wait for the radio to power up
}



void NRF24L01::PrintInfo(char* string){
	char buff[100];


		sprintf(string,"<NRF24L01+>");

		sprintf(buff,"<STATUS>%02X|", GetStatus());  strcat(string,buff);
		sprintf(buff,"<CONFIG>%02X|", ReadRegister(NRF24L01_REG_CONFIG)); strcat(string,buff);
		sprintf(buff,"<RF_CH>%02X|", ReadRegister(NRF24L01_REG_RF_CH)); strcat(string,buff);
		sprintf(buff,"<RF_SETUP>%02X|", ReadRegister(NRF24L01_REG_RF_SETUP)); strcat(string,buff);
		sprintf(buff,"<EN_AA>%02X|", ReadRegister(NRF24L01_REG_EN_AA)); strcat(string,buff);
		sprintf(buff,"<EN_RXADDR>%02X|", ReadRegister(NRF24L01_REG_EN_RXADDR)); strcat(string,buff);
		sprintf(buff,"<OBSERVE_TX>%02X|", ReadRegister(NRF24L01_REG_OBSERVE_TX)); strcat(string,buff);

		uint8_t adr[NRF24L01_ADDRSIZE];
		uint8_t addrrev[NRF24L01_ADDRSIZE];

		ReadRegisters(NRF24L01_REG_RX_ADDR_P0,adr,NRF24L01_ADDRSIZE);
		ReverseAddress(adr, (uint8_t *)addrrev);
		sprintf(buff,"<RX_ADDR_P0>%02X:%02X:%02X:%02X:%02X|",addrrev[0],addrrev[1],addrrev[2],addrrev[3],addrrev[4]); strcat(string,buff);

		ReadRegisters(NRF24L01_REG_RX_ADDR_P1,adr,NRF24L01_ADDRSIZE);
		ReverseAddress(adr, (uint8_t *)addrrev);
		sprintf(buff,"<RX_ADDR_P1>%02X:%02X:%02X:%02X:%02X|",addrrev[0],addrrev[1],addrrev[2],addrrev[3],addrrev[4]); strcat(string,buff);



		ReadRegisters(NRF24L01_REG_RX_ADDR_P2,adr,NRF24L01_ADDRSIZE);
		ReverseAddress(adr, (uint8_t *)addrrev);
		sprintf(buff,"<RX_ADDR_P2>%02X|",addrrev[0]); strcat(string,buff);


		ReadRegisters(NRF24L01_REG_RX_ADDR_P3,adr,NRF24L01_ADDRSIZE);
		ReverseAddress(adr, (uint8_t *)addrrev);
		sprintf(buff,"<RX_ADDR_P3>%02X|",addrrev[0]); strcat(string,buff);

		ReadRegisters(NRF24L01_REG_RX_ADDR_P4,adr,NRF24L01_ADDRSIZE);
		ReverseAddress(adr, (uint8_t *)addrrev);
		sprintf(buff,"<RX_ADDR_P4>%02X|",addrrev[0]); strcat(string,buff);


		ReadRegisters(NRF24L01_REG_RX_ADDR_P5,adr,NRF24L01_ADDRSIZE);
		ReverseAddress(adr, (uint8_t *)addrrev);
		sprintf(buff,"<RX_ADDR_P5>%02X|",addrrev[0]); strcat(string,buff);


		ReadRegisters(NRF24L01_REG_TX_ADDR,adr,NRF24L01_ADDRSIZE);
		ReverseAddress(adr, (uint8_t *)addrrev);
		sprintf(buff,"<TX_ADDR>%02X:%02X:%02X:%02X:%02X|",addrrev[0],addrrev[1],addrrev[2],addrrev[3],addrrev[4]); strcat(string,buff);


		sprintf(buff,"<RX_PW_P0>%02X|",ReadRegister(NRF24L01_REG_RX_PW_P0)); strcat(string,buff);
		sprintf(buff,"<RX_PW_P1>%02X|",ReadRegister(NRF24L01_REG_RX_PW_P1)); strcat(string,buff);
		sprintf(buff,"<RX_PW_P2>%02X|",ReadRegister(NRF24L01_REG_RX_PW_P2)); strcat(string,buff);
		sprintf(buff,"<RX_PW_P3>%02X|",ReadRegister(NRF24L01_REG_RX_PW_P3)); strcat(string,buff);
		sprintf(buff,"<RX_PW_P4>%02X|",ReadRegister(NRF24L01_REG_RX_PW_P4)); strcat(string,buff);
		sprintf(buff,"<RX_PW_P5>%02X|",ReadRegister(NRF24L01_REG_RX_PW_P5)); strcat(string,buff);


		strcat(string,"||+end\r\n");

}




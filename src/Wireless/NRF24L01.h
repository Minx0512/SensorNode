/*
 * NRF24L01.h
 *
 *  Created on: Nov 14, 2016
 *      Author: matthias
 */

#ifndef NRF24L01_H_
#define NRF24L01_H_

#include "avr/io.h"
#include "../IO/SPI.h"
//#include "../IO/USART.h"

//CE and CSN port definitions
//#define NRF24L01_DDR DDRB
#define NRF24L01_PORT PORTB
//#define NRF24L01_CE PB1
//#define NRF24L01_CSN PB0


//power setup
#define NRF24L01_RF24_PA_MIN 1
#define NRF24L01_RF24_PA_LOW 2
#define NRF24L01_RF24_PA_HIGH 3
#define NRF24L01_RF24_PA_MAX 4


//speed setup
#define NRF24L01_RF24_SPEED_250KBPS 1
#define NRF24L01_RF24_SPEED_1MBPS 2
#define NRF24L01_RF24_SPEED_2MBPS 3


//crc setup
#define NRF24L01_RF24_CRC_DISABLED 1
#define NRF24L01_RF24_CRC_8 2
#define NRF24L01_RF24_CRC_16 3


//auto retransmit delay and count
#define NRF24L01_RETR (0b0100 << NRF24L01_REG_ARD) | (0b0111 << NRF24L01_REG_ARC) //1500uS, 15 times


class NRF24L01 {


	//CE and CSN port definitions
	volatile uint8_t* NRF24L01_DDR;
//	volatile uint8_t* NRF24L01_PORT;
	uint8_t NRF24L01_CE;
	uint8_t NRF24L01_CSN;


	//transmission channel
	uint8_t NRF24L01_CH;
	//payload lenght
	uint8_t NRF24L01_PAYLOAD;
	//auto ack enabled
	uint8_t NRF24L01_ACK;


	//enable / disable pipe
	uint8_t NRF24L01_ENABLEDP0; //pipe 0
	uint8_t NRF24L01_ENABLEDP1; //pipe 1
	uint8_t NRF24L01_ENABLEDP2; //pipe 2
	uint8_t NRF24L01_ENABLEDP3; //pipe 3
	uint8_t NRF24L01_ENABLEDP4; //pipe 4
	uint8_t NRF24L01_ENABLEDP5; //pipe 5

	//address size
	uint8_t NRF24L01_ADDRSIZE;

	uint8_t nrf24l01_addr0[5];
	uint8_t nrf24l01_addr1[5];
	uint8_t nrf24l01_addr2[5];
	uint8_t nrf24l01_addr3[5];
	uint8_t nrf24l01_addr4[5];
	uint8_t nrf24l01_addr5[5];
	uint8_t nrf24l01_addrtx[5];

	//power setup
	uint8_t NRF24L01_RF24_PA;

	//speed setup
	uint8_t NRF24L01_RF24_SPEED;


	//crc setup
	uint8_t NRF24L01_RF24_CRC;



public:

	NRF24L01();

	NRF24L01(volatile uint8_t &DDR, volatile uint8_t &PORT, uint8_t CE, uint8_t CSN);


	void Init();

	uint8_t GetStatus();
	uint8_t ReadReady(uint8_t* pipe);
	void Read(uint8_t *data);
	uint8_t Write(uint8_t *data);

	void SetAddresses(uint8_t* pipe0,uint8_t* pipe1,uint8_t* pipe2,uint8_t* pipe3,uint8_t* pipe4,uint8_t* pipe5,uint8_t* pipeT);

	void SetChannel(uint8_t channel);
	void SetPayloadSize(uint8_t payload);

	void SetPowerLevel(uint8_t power);
	void SetDataRate(uint8_t speed);
	void SetCRClength(uint8_t crclvl);

	void EnableAutoAck();
	void DisableAutoAck();
	void SetAutoAck(uint8_t ack);

	void EnablePipe(uint8_t pipe);



	void PrintInfo(char* string);

	SPI spi;

	//USART u;
private:

	//CE and CSN functions
	void CSN_HIGH(){ NRF24L01_PORT |= (1<<NRF24L01_CSN);}
	void CSN_LOW(){ NRF24L01_PORT &= ~(1<<NRF24L01_CSN);}
	void CE_HIGH(){ NRF24L01_PORT |=  (1<<NRF24L01_CE);}
	void CE_LOW(){ NRF24L01_PORT &= ~(1<<NRF24L01_CE);}


	void SetPowerLevel(); //set power level
	void SetDataRate(); //set data rate
	void SetCRClength(); //set crc length
	void WriteRegister(uint8_t reg, uint8_t value);
	void WriteRegisters(uint8_t reg, uint8_t *value, uint8_t len);
	uint8_t ReadRegister(uint8_t reg);
	void ReadRegisters(uint8_t reg, uint8_t *value, uint8_t len);

	void ReverseAddress(uint8_t *addr, uint8_t *addrrev);
	void SetRxAddr(uint8_t pipe, uint8_t *addr);
	void SetTxAddr(uint8_t *addr);


	void FlushRXfifo();
	void FlushTXfifo();
	void SetRX();
	void SetTX();








};

#endif /* NRF24L01_H_ */

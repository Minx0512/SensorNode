/**
 ****************************************************************************
 *
 * Project: SensorNode
 * 
 * I2C.hpp
 *
 * Created on: 28 Apr 2017
 *
 * Author: Matthias Minx
 *
 * Revision: 
 *
 ****************************************************************************
 License:

 **************************************************************************/
/** \file I2C.hpp
 * 
 */

#ifndef I2C_HPP_
#define I2C_HPP_

#include <inttypes.h>

#ifndef  F_CPU
#define F_CPU 16000000UL
#endif

#define I2C_READ 0x01
#define I2C_WRITE 0x00


#define F_SCL 100000UL // SCL frequency
#define Prescaler 1
#define TWBR_val ((((F_CPU / F_SCL) / Prescaler) - 16 ) / 2)




class I2C {




public:

	I2C();


	void Init(void);
	uint8_t Start(uint8_t address);
	uint8_t Write(uint8_t data);
	uint8_t Read_ack(void);
	uint8_t Read_nack(void);
	uint8_t Transmit(uint8_t address, uint8_t* data, uint16_t length);
	uint8_t Receive(uint8_t address, uint8_t* data, uint16_t length);
	uint8_t WriteReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length);
	uint8_t ReadReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length);
	void Stop(void);






};



#endif /* I2C_HPP_ */

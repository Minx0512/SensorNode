/**
 * ****************************************************************************
 *
 * File : BMP180.hpp
 *
 *  Created on: 27 Mar 2017
 *
 *  Author: Matthias Minx
 *
 *
 * Usage: Sensor Driver for BMP180 sensor
 * Class based on Bosch Sensortec BMP180 sensor driver
 *
 ****************************************************************************
 *
****************************************************************************
* Copyright (C) 2008 - 2015 Bosch Sensortec GmbH
*
* File : bmp180.h
*
* Date : 2015/03/27
*
* Revision : 2.2.2
*
* Usage: Sensor Driver for BMP180 sensor
*
****************************************************************************
*
* \section License
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*   Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
*   Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
*   Neither the name of the copyright holder nor the names of the
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER
* OR CONTRIBUTORS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
* OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*
* The information provided is believed to be accurate and reliable.
* The copyright holder assumes no responsibility
* for the consequences of use
* of such information nor for any infringement of patents or
* other rights of third parties which may result from its use.
* No license is granted by implication or otherwise under any patent or
* patent rights of the copyright holder.
**************************************************************************/
 /** \file bmp180.h
    \brief Header file for all define constants and function prototypes

*/

#ifndef BMP180_HPP_
#define BMP180_HPP_

#include <stdint.h>
#include "../IO/I2C.hpp"

/*unsigned integer types*/
typedef	uint8_t u8;/**< used for unsigned 8bit */
typedef	uint16_t u16;/**< used for unsigned 16bit */
typedef	uint32_t u32;/**< used for unsigned 32bit */
typedef	uint64_t u64;/**< used for unsigned 64bit */

/*signed integer types*/
typedef	int8_t s8;/**< used for signed 8bit */
typedef	int16_t s16;/**< used for signed 16bit */
typedef	int32_t s32;/**< used for signed 32bit */
typedef	int64_t s64;/**< used for signed 64bit */

/* register write and read delays */
#define BMP180_MDELAY_DATA_TYPE	u32
/***************************************************************/
/**\name	I2C ADDRESS DEFINITION OF BMP180       */
/***************************************************************/
/*BMP180 I2C Address*/
#define BMP180_I2C_ADDR		(0xEE>>1)

/***************************************************************/
/**\name	ERROR CODE DEFINITIONS    */
/***************************************************************/
#define E_BMP_NULL_PTR				((s8)-127)
#define E_BMP_COMM_RES				((s8)-1)
#define E_BMP_OUT_OF_RANGE			((s8)-2)
/***************************************************************/
/**\name	CONSTANTS       */
/***************************************************************/
#define BMP180_RETURN_FUNCTION_TYPE        s8
#define   BMP180_INIT_VALUE						((u8)0)
#define   BMP180_INITIALIZE_OVERSAMP_SETTING_U8X	((u8)0)
#define   BMP180_INITIALIZE_SW_OVERSAMP_U8X			((u8)0)
#define   BMP180_INITIALIZE_NUMBER_OF_SAMPLES_U8X	((u8)1)
#define   BMP180_GEN_READ_WRITE_DATA_LENGTH			((u8)1)
#define   BMP180_TEMPERATURE_DATA_LENGTH			((u8)2)
#define   BMP180_PRESSURE_DATA_LENGTH				((u8)3)
#define   BMP180_SW_OVERSAMP_U8X					((u8)1)
#define   BMP180_OVERSAMP_SETTING_U8X				((u8)3)
#define   BMP180_2MS_DELAY_U8X			(2)
#define   BMP180_3MS_DELAY_U8X			(3)
#define   BMP180_AVERAGE_U8X			(3)
#define   BMP180_INVALID_DATA			(0)
#define   BMP180_CHECK_DIVISOR			(0)
#define   BMP180_DATA_MEASURE			(3)
#define   BMP180_CALCULATE_TRUE_PRESSURE		(8)
#define   BMP180_CALCULATE_TRUE_TEMPERATURE		(8)
#define BMP180_SHIFT_BIT_POSITION_BY_01_BIT			(1)
#define BMP180_SHIFT_BIT_POSITION_BY_02_BITS		(2)
#define BMP180_SHIFT_BIT_POSITION_BY_04_BITS		(4)
#define BMP180_SHIFT_BIT_POSITION_BY_06_BITS		(6)
#define BMP180_SHIFT_BIT_POSITION_BY_08_BITS		(8)
#define BMP180_SHIFT_BIT_POSITION_BY_11_BITS		(11)
#define BMP180_SHIFT_BIT_POSITION_BY_12_BITS		(12)
#define BMP180_SHIFT_BIT_POSITION_BY_13_BITS		(13)
#define BMP180_SHIFT_BIT_POSITION_BY_15_BITS		(15)
#define BMP180_SHIFT_BIT_POSITION_BY_16_BITS		(16)
/***************************************************************/
/**\name	REGISTER ADDRESS DEFINITION       */
/***************************************************************/
/*register definitions */

#define BMP180_PROM_START__ADDR		(0xAA)
#define BMP180_PROM_DATA__LEN		(22)

#define BMP180_CHIP_ID_REG			(0xD0)
#define BMP180_VERSION_REG			(0xD1)

#define BMP180_CTRL_MEAS_REG		(0xF4)
#define BMP180_ADC_OUT_MSB_REG		(0xF6)
#define BMP180_ADC_OUT_LSB_REG		(0xF7)

#define BMP180_SOFT_RESET_REG		(0xE0)

/* temperature measurement */
#define BMP180_T_MEASURE			(0x2E)
/* pressure measurement*/
#define BMP180_P_MEASURE			(0x34)
/* TO be spec'd by GL or SB*/
#define BMP180_TEMP_CONVERSION_TIME  (5)

#define BMP180_PARAM_MG		(3038)
#define BMP180_PARAM_MH		(-7357)
#define BMP180_PARAM_MI		(3791)

/****************************************************/
/**\name	ARRAY SIZE DEFINITIONS      */
/***************************************************/
#define	BMP180_TEMPERATURE_DATA_BYTES	(2)
#define	BMP180_PRESSURE_DATA_BYTES		(3)
#define	BMP180_TEMPERATURE_LSB_DATA		(1)
#define	BMP180_TEMPERATURE_MSB_DATA		(0)
#define	BMP180_PRESSURE_MSB_DATA		(0)
#define	BMP180_PRESSURE_LSB_DATA		(1)
#define	BMP180_PRESSURE_XLSB_DATA		(2)

#define	BMP180_CALIB_DATA_SIZE			(22)
#define	BMP180_CALIB_PARAM_AC1_MSB		(0)
#define	BMP180_CALIB_PARAM_AC1_LSB		(1)
#define	BMP180_CALIB_PARAM_AC2_MSB		(2)
#define	BMP180_CALIB_PARAM_AC2_LSB		(3)
#define	BMP180_CALIB_PARAM_AC3_MSB		(4)
#define	BMP180_CALIB_PARAM_AC3_LSB		(5)
#define	BMP180_CALIB_PARAM_AC4_MSB		(6)
#define	BMP180_CALIB_PARAM_AC4_LSB		(7)
#define	BMP180_CALIB_PARAM_AC5_MSB		(8)
#define	BMP180_CALIB_PARAM_AC5_LSB		(9)
#define	BMP180_CALIB_PARAM_AC6_MSB		(10)
#define	BMP180_CALIB_PARAM_AC6_LSB		(11)
#define	BMP180_CALIB_PARAM_B1_MSB		(12)
#define	BMP180_CALIB_PARAM_B1_LSB		(13)
#define	BMP180_CALIB_PARAM_B2_MSB		(14)
#define	BMP180_CALIB_PARAM_B2_LSB		(15)
#define	BMP180_CALIB_PARAM_MB_MSB		(16)
#define	BMP180_CALIB_PARAM_MB_LSB		(17)
#define	BMP180_CALIB_PARAM_MC_MSB		(18)
#define	BMP180_CALIB_PARAM_MC_LSB		(19)
#define	BMP180_CALIB_PARAM_MD_MSB		(20)
#define	BMP180_CALIB_PARAM_MD_LSB		(21)

/**************************************************************/
/**\name	STRUCTURE DEFINITIONS                         */
/**************************************************************/
/*!
 * @brief This structure holds all device specific calibration parameters
 */
struct bmp180_calib_param_t {
	s16 ac1;/**<calibration ac1 data*/
	s16 ac2;/**<calibration ac2 data*/
	s16 ac3;/**<calibration ac3 data*/
	u16 ac4;/**<calibration ac4 data*/
	u16 ac5;/**<calibration ac5 data*/
	u16 ac6;/**<calibration ac6 data*/
	s16 b1;/**<calibration b1 data*/
	s16 b2;/**<calibration b2 data*/
	s16 mb;/**<calibration mb data*/
	s16 mc;/**<calibration mc data*/
	s16 md;/**<calibration md data*/
};
/*!
 * @brief This structure holds BMP180 initialization parameters
 */
struct bmp180_t {
	struct bmp180_calib_param_t calib_param;/**<calibration data*/
	u8 mode;/**<power mode*/
	u8 chip_id; /**<chip id*/
	u8 ml_version;/**<ml version*/
	u8 al_version;/**<al version*/
	u8 dev_addr;/**<device address*/
	u8 sensortype;/**< sensor type*/

	s32 param_b5;/**<pram*/
	s32 number_of_samples;/**<sample calculation*/
	s16 oversamp_setting;/**<oversampling setting*/
	s16 sw_oversamp;/**<software oversampling*/


	//BMP180_WR_FUNC_PTR;/**< bus write function pointer*/
	//BMP180_RD_FUNC_PTR;/**< bus read function pointer*/
	void (*delay_msec)(BMP180_MDELAY_DATA_TYPE);/**< delay function pointer*/
};
/**************************************************************/
/**\name	BIT MASK, LENGTH AND POSITION FOR REGISTERS     */
/**************************************************************/
/**************************************************************/
/**\name	BIT MASK, LENGTH AND POSITION FOR
 CHIP ID REGISTERS     */
/**************************************************************/
#define BMP180_CHIP_ID__POS             (0)
#define BMP180_CHIP_ID__MSK             (0xFF)
#define BMP180_CHIP_ID__LEN             (8)
#define BMP180_CHIP_ID__REG             (BMP180_CHIP_ID_REG)
/**************************************************************/
/**\name	BIT MASK, LENGTH AND POSITION FOR
   ML VERSION  */
/**************************************************************/
#define BMP180_ML_VERSION__POS          (0)
#define BMP180_ML_VERSION__LEN          (4)
#define BMP180_ML_VERSION__MSK          (0x0F)
#define BMP180_ML_VERSION__REG          (BMP180_VERSION_REG)
/**************************************************************/
/**\name	BIT MASK, LENGTH AND POSITION FOR
   AL VERSION  */
/**************************************************************/
#define BMP180_AL_VERSION__POS          (4)
#define BMP180_AL_VERSION__LEN          (4)
#define BMP180_AL_VERSION__MSK          (0xF0)
#define BMP180_AL_VERSION__REG          (BMP180_VERSION_REG)
/**************************************************************/
/**\name	GET AND SET BITSLICE FUNCTIONS*/
/**************************************************************/

/* DATA REGISTERS */
/* LG/HG thresholds are in LSB and depend on RANGE setting */
/* no range check on threshold calculation */
#define BMP180_GET_BITSLICE(regvar, bitname) ((regvar & bitname##__MSK) >> (bitname##__POS))


#define BMP180_SET_BITSLICE(regvar, bitname, val) ((regvar & ~bitname##__MSK) | ((val<<bitname##__POS)&bitname##__MSK))

//#include "../../3rdParty/I2C-master-lib/i2c_master.h"


class BMP180 {


		I2C i2c;
		static struct bmp180_t *p_bmp180;

		int32_t temperature, pressure;


		BMP180_RETURN_FUNCTION_TYPE GetCalibrationParam();

		s16 GetTemperature(u32 val_uncomp_temp_u32);
		s32 GetPressure(u32 val_uncomp_pressure_u32);
		u16 GetuncompTemperature(void);
		u32 GetuncompPressure(void);

public:


	BMP180();
	BMP180(I2C&);

	BMP180_RETURN_FUNCTION_TYPE Init(); /** init and calibrate*/

	void GetTemperature();
	void GetPressure();
	void GetTempPressure();
	void GetTempPressureString(char* tempPresStr);







};

#endif /* BMP180_HPP_ */

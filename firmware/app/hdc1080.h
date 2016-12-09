/******************************************************************************
*	Written by Yilmaz Eyidogan
*	HDC1080 Temperature and Humidity Sensor functions
*	Created date: 2016.12.08
*******************************************************************************/
#ifndef __HDC1080_H_
#define __HDC1080_H_

#include "cmsis_os2.h"
#include "i2c.h"
#include "main.h"
#include <stdbool.h>

/* Private define ----------------------------------------------------*/
#define HDC1080_I2C_SLAVE_ADD 0x40
#define HDC1080_WR_CONF_FRAME_SIZE 0x03
#define HDC1080_RD_CONF_FRAME_SIZE 0x03
#define HDC1080_RD_WR_CONF_POINTER_SIZE 0x01

#define TEMP_REG 0x00
#define HUMIDITY_REG 0x01
#define CONFIGURATION_REG 0x02
#define SERIAL_H_REG 0xFB
#define SERIAL_M_REG 0xFC
#define SERIAL_L_REG 0xFD
#define MANUFACTURER_ID_REG 0xFE
#define DEVICE_ID_REG 0xFF

#define HDC1080_CONF_REGISTER_VAL 0x1000
	//bit15:0 normal operation. 1=software reset
	//bit14:0 reserved
	//bit13:0 Heat disabled
	//bit12:1 Mode: Temperature and humidity are acquired sequentially
	//bit11:0 Battery voltage > 2.8V
	//bit10:0 Temperature register is 14 bit
	//bit9-8:00 Humidity register is 14 bit
	//bit7-0:0 reserved

/* macro -------------------------------------------------------------*/
/* typedef -----------------------------------------------------------*/
typedef struct{
	uint8_t ucRegister;
	uint16_t uiData;
}__attribute__((packed)) ST_HDC1080_WR_CONF_FRAME_TYPE;
typedef struct{
	uint8_t ucDataH;
	uint8_t ucDataL;
}ST_HDC1080_RD_CONF_FRAME_TYPE;
typedef struct{
	uint16_t uiTemperature;
	uint16_t uiHumidity;	
}ST_HDC1080_RD_TEMP_HUM_TYPE;

#define PTR_HDC1080_CONF_FRAME_RD ((ST_HDC1080_RD_CONF_FRAME_TYPE *)ucI2CRxBuf)
#define PTR_HDC1080_CONF_FRAME_WR ((ST_HDC1080_WR_CONF_FRAME_TYPE *)ucI2CTxBuf)
#define PTR_HDC1080_TEMP_HUM_RD ((ST_HDC1080_RD_TEMP_HUM_TYPE *)ucI2CRxBuf)

/* Functions ---------------------------------------------------------*/
extern __NO_RETURN void get_temp_hum_from_HDC1080(void *argument);
/* variables ---------------------------------------------------------*/

#endif
/* * * END OF FILE * * */

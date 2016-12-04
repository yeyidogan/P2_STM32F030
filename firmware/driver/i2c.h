/******************************************************************************
*	Written by Yilmaz Eyidogan
*	I2C functions
*	Created date: 2016.12.03
*******************************************************************************/
#ifndef __I2C_H
#define __I2C_H

#include "stm32f0xx.h"
#include "stm32f0xx_i2c.h"
#include "stm32f0xx_rcc.h"

/* Private define ------------------------------------------------------------*/
#define I2C_OWN_ADDR 0x0A
#define I2C_TX_BUF_SIZE 32
#define I2C_RX_BUF_SIZE 32

enum {
	I2C_MASTER_WRITE,
	I2C_MASTER_READ
};
/* macro -------------------------------------------------------------*/

/* typedef -----------------------------------------------------------*/
typedef struct{
	uint32_t mode:1;
	uint32_t arbloss_flag:1;
}I2C_STATUS_TYPE;

typedef struct{
	uint8_t slaveAddress;
	uint8_t length;
}I2C_MSG_TX_TYPE;

/* Functions ---------------------------------------------------------*/
extern void i2c_init(void);
extern void i2c_master_process(uint8_t rw);
/* variables ---------------------------------------------------------*/
extern uint8_t * i2cDataPtr;
extern I2C_MSG_TX_TYPE stI2cMsgTx;
extern I2C_STATUS_TYPE stI2cStatus;

#endif

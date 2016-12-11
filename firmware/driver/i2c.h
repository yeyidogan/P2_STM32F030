/******************************************************************************
*	Written by Yilmaz Eyidogan
*	I2C functions
*	Created date: 2016.12.03
*******************************************************************************/
#ifndef __I2C_H_
#define __I2C_H_

#include "stm32f0xx.h"
#include "stm32f0xx_i2c.h"
#include "stm32f0xx_rcc.h"
#include <stdbool.h>

/* Private define ------------------------------------------------------------*/
#define I2C_OWN_ADDR 0x0A
#define I2C_TX_BUF_SIZE 32
#define I2C_RX_BUF_SIZE 32

enum {
	I2C_MASTER_WRITE,
	I2C_MASTER_READ
};

enum {
	I2C_MASTER_MODE,
	I2C_SLAVE_MODE
};

enum {
	I2C_MASTER_SEND_STOP,
	I2C_MASTER_SEND_START,
	I2C_MASTER_SEND_RESTART,
	I2C_MASTER_SEND_MAX = I2C_MASTER_SEND_RESTART
};
/* macro -------------------------------------------------------------*/

/* typedef -----------------------------------------------------------*/
typedef struct{
	uint32_t mode:1;
	uint32_t bus_busy_flag:1;
	uint32_t bus_error_flag:1;
	uint32_t over_run_flag:1;
	uint32_t arblost_flag:1;
	uint32_t completed_flag:1;
}I2C_STATUS_TYPE;

typedef struct{
	uint8_t slaveAddress;
	uint8_t length;
}I2C_MSG_TX_TYPE;

/* Functions ---------------------------------------------------------*/
extern void i2c_init(void);
extern uint8_t i2c_master_process(uint8_t rw);
/* variables ---------------------------------------------------------*/
extern uint8_t ucI2CTxBuf[];
extern uint8_t ucI2CRxBuf[];
//extern uint8_t * i2cDataPtr;
extern I2C_MSG_TX_TYPE stI2cMsgTx;
extern I2C_STATUS_TYPE stI2cStatus;
extern uint8_t ucI2CMasterSendStartStop;
extern uint16_t uiTimerI2C;

#endif
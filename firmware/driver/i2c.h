/******************************************************************************
*	Written by Yilmaz Eyidogan
*	I2C functions
*	Created date: 2016.12.03
*******************************************************************************/
#ifndef __I2C_H_
#define __I2C_H_

#include "stm32f0xx.h"
#include "stm32f0xx_i2c.h"
#include <stdbool.h>
#include "cmsis_os2.h"
#include "pub_var.h"
#include "main.h"

/* define ------------------------------------------------------------*/
#define I2C_OWN_ADDR 0x0A
#define I2C_TX_BUF_SIZE 34
#define I2C_RX_BUF_SIZE 34

/* macro -------------------------------------------------------------*/
#define START_I2C_TIMER uiTimerI2C=(uint16_t)0x00
#define CHECK_I2C_TIMER_REACH_TO(x) uiTimerI2C>(uint16_t)x

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
typedef union{
	uint8_t byte;
	struct {
		uint8_t bus_busy_flag:1;
		uint8_t bus_error_flag:1;
		uint8_t over_run_flag:1;
		uint8_t arblost_flag:1;
		uint8_t nack_flag:1;
		uint8_t completed_flag:1;
	}bits;
}I2C_STATUS_TYPE;

typedef struct{
	uint8_t chip_add;
	uint8_t length_wr;
	uint8_t length_rd;
}I2C_MSG_TX_TYPE;

/* Functions ---------------------------------------------------------*/
extern void init_i2c(void);
extern uint8_t i2c_master_process(uint8_t rw);
/* variables ---------------------------------------------------------*/
extern uint8_t i2c_tx_buf[];
extern uint8_t i2c_rx_buf[];
extern I2C_MSG_TX_TYPE i2c_msg_tx_s;
extern I2C_STATUS_TYPE i2c_status_s;
extern uint8_t i2c_master_send_start_stop;
extern uint16_t uiTimerI2C;

#endif
/* * * END OF FILE * * */

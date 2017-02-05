/******************************************************************************
*	Written by Yilmaz Eyidogan
*	Uart declarations
*	Created date: 2014.12.22
*******************************************************************************/
#ifndef __UART_H
#define __UART_H

#include <stdint.h>
#include "stm32f0xx.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_misc.h"
#include "stm32f0xx_dma.h"
#if defined(__CC_ARM)
	#include "cmsis_os2.h"
#elif defined(__GNUC__)
	#include "CoOs.h"
#endif
#include "main.h"
#include "plt_free_os_def.h"
#include "uart.h"
#include "par_enums.h"
#include "modbus.h"
#include "util.h"
#include "pub_var.h"

/* Private define */
#define UART_RX_BUFFER_SIZE 0x40 //use power of 2
#define UART_TX_BUFFER_SIZE 0x40
#define UART1_RX_TIMEOUT 0x23 //38 bit = 3.5 character
#define UART2_RX_TIMEOUT 0x23 //38 bit = 3.5 character

enum {
	UART_TX_READY = 0x00, //no data is transmitting
	UART_TX_BUSY //transmitting data
};

#define UART_RX_BUFFER1_WAITING TRUE
#define UART_RX_BUFFER2_WAITING FALSE
#define UART_RX_BUFFER_IS_EMPTY 0x00

/* Private macro */
#define START_UART1_TIMER uiTimerUart1=(uint16_t)0x00
#define CHECK_UART1_TIMER_REACH_TO(x) uiTimerUart1>(uint16_t)x

#define START_UART2_TIMER uiTimerUart2=(uint16_t)0x00
#define CHECK_UART2_TIMER_REACH_TO(x) uiTimerUart2>(uint16_t)x

/* Private function declarations */
extern void initUart1(uint32_t baudRate);
extern void initUart2(uint32_t baudRate);
extern void uartNvicConfig(void);
extern void initUartDma(void);
extern void uart1TxCmd(uint8_t *ptr, uint8_t length);
extern uint8_t uart1CheckRxBuf(void);
extern __NO_RETURN void task_Uart1(void *pdata);
extern __NO_RETURN void task_Uart2(void *pdata);

/* Private typedef */
typedef struct {
	uint8_t length1; //length of data to be read of buffer1
	uint8_t length2; //length of data to be read of buffer1
	uint8_t buffer1[UART_RX_BUFFER_SIZE];
	uint8_t buffer2[UART_RX_BUFFER_SIZE];
	uint8_t * ptrBuffer; //will be used for active buffer
}UART_RX_BUFFER_TYPE;

typedef struct {
	uint8_t bufferId;
	uint8_t length; //length of data to be transferred
	uint8_t buffer[UART_TX_BUFFER_SIZE];
}UART_TX_BUFFER_TYPE;

typedef struct {
	uint8_t
		rxTimeOut:1,
		rxError:1,
		rxOnlineBuffer,
		rxBuffer1Received,
		rxBuffer2Received,
		txBusy:1,
		txError:1;
}UART_STATUS_TYPE;

/* Private variables */
extern UART_RX_BUFFER_TYPE uart1Rx;
extern UART_TX_BUFFER_TYPE uart1Tx;
extern UART_STATUS_TYPE uart1Flags;
extern uint16_t uiTimerUart1;

#endif
/* * * END OF FILE * * */

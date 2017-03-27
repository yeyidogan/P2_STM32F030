/******************************************************************************
*	Written by Yilmaz Eyidogan
*	main.c functions
*	Created date: 2016.12.09
*******************************************************************************/
#ifndef _MAIN_H_
#define _MAIN_H_

#include "cmsis_os2.h"
#include <stdio.h>
#include "pub_var.h"
#include "gpio.h"
#include "uart.h"
#include "i2c.h"
#include "timer.h"
#include "hdc1080.h"
#include "hc0x_bluetooth.h"
#include "stepper_motor.h"

/* define ------------------------------------------------------------*/
/* macro -------------------------------------------------------------*/
/* typedef -----------------------------------------------------------*/
/* Functions ---------------------------------------------------------*/
/* variables ---------------------------------------------------------*/
/* mutexes ---------------------------------------------------------*/
extern osMutexId_t mutex_I2C;
/* flags -----------------------------------------------------------*/
	#define EVENT_MASK_UART1_TIMEOUT 0x00000001ul
	#define EVENT_MASK_UART2_TIMEOUT 0x00000002ul
	#define EVENT_MASK_STEPPER_RUN 0x00000010ul
	extern osEventFlagsId_t event_general;

#endif
/* * * END OF FILE * * */

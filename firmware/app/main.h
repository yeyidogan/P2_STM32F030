/******************************************************************************
*	Written by Yilmaz Eyidogan
*	main.c functions
*	Created date: 2016.12.09
*******************************************************************************/
#ifndef _MAIN_H_
#define _MAIN_H_

#include "cmsis_os2.h"
#include "stm32f0xx_rcc.h"
#include "gpio.h"
#include "i2c.h"
#include "timer.h"
#include "hdc1080.h"

/* define ------------------------------------------------------------*/
/* macro -------------------------------------------------------------*/
/* typedef -----------------------------------------------------------*/
/* Functions ---------------------------------------------------------*/
extern void rccConfig(void);
extern void taskC (void *argument);
/* variables ---------------------------------------------------------*/
extern /* mutexes ---------------------------------------------------------*/
osMutexId_t mutex_I2C;

#endif
/******************************************************************************
*	Written by Yilmaz Eyidogan
*	main.c functions
*	Created date: 2016.12.09
*******************************************************************************/
#ifndef _MAIN_H_
#define _MAIN_H_

#if defined(__CC_ARM)
	#include "cmsis_os2.h"
#elif defined(__GNUC__)
	#include "CoOs.h"
#endif
#include <stdio.h>
#include "plt_free_os_def.h"
#include "pub_var.h"
#include "gpio.h"
#include "uart.h"
#include "i2c.h"
#include "timer.h"
#include "hdc1080.h"

/* define ------------------------------------------------------------*/
/* macro -------------------------------------------------------------*/
/* typedef -----------------------------------------------------------*/
/* Functions ---------------------------------------------------------*/
/* variables ---------------------------------------------------------*/
/* mutexes ---------------------------------------------------------*/
extern PLT_FREE_OS_MUTEX_ID mutex_I2C;

#endif

/******************************************************************************
*	Written by Yilmaz Eyidogan
*	main application functions
*	Created date: 2017.04.09
*******************************************************************************/
#ifndef _MAIN_TASK_H_
#define _MAIN_TASK_H_

#include "cmsis_os2.h"
#include <stdbool.h>
#include <stdint.h>
#include "main.h"
#include "stepper_motor.h"
#include "uart.h"

/* define ------------------------------------------------------------*/
#define INFRARED_SENSOR_PIN ((uint16_t)0x0004) //GPIO_Pin_2
#define INFRARED_SENSOR_CAT_DETECT_CNT 10
#define MOTOR_HOLE_GATE MOTOR_B
#define MOTOR_LITTER_GATE MOTOR_A

enum{
	NO_CAT = (uint8_t)0x00,
	CAT_DETECTED,
	CAT_WAITED_FOR_A_WHILE,
	CAT_GONE_OUT
};
/* macro -------------------------------------------------------------*/
/* typedef -----------------------------------------------------------*/
/* variables ---------------------------------------------------------*/
/* functions ---------------------------------------------------------*/
extern __NO_RETURN void task_main(void *argument);

#endif
/* * * END OF FILE * * */

/******************************************************************************
*	Written by Yilmaz Eyidogan
*	stepper motor functions
*	Created date: 2016.12.15
*******************************************************************************/
#ifndef __STEPPER_MOTOR_H_
#define __STEPPER_MOTOR_H_

#include <stdbool.h>
#include <stdint.h>
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"

/* typedef -----------------------------------------------------------*/
typedef struct{
	uint8_t ucIndexA;
	uint8_t ucIndexB;
	uint8_t ucCmdForA;
	uint8_t ucCmdForB;
}STEPPER_MOTOR_CONTROL_TYPE;

/* define ------------------------------------------------------------*/
#define MAX_INDEX_OF_STEPPER_MOTOR 0x03

enum{
	STEPPER_STOP = (uint8_t)0x00,
	STEPPER_FORWARD,
	STEPPER_BACKWARD,
	STEPPER_TO_ZERO_POINT,
	STEPPER_TO_END_POINT
}; //command list

/* macro -------------------------------------------------------------*/
/* variables ---------------------------------------------------------*/
extern const uint8_t stepperMotorFullStepArray[];

#endif
/* * * END OF FILE * * */

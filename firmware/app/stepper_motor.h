/******************************************************************************
*	Written by Yilmaz Eyidogan
*	stepper motor functions
*	Created date: 2016.12.15
*******************************************************************************/
#ifndef __STEPPER_MOTOR_H_
#define __STEPPER_MOTOR_H_

#if defined(__CC_ARM)
	#include "cmsis_os2.h"
#elif defined(__GNUC__)
	#include "CoOs.h"
#endif
#include <stdbool.h>
#include <stdint.h>
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "gpio.h"
#include "plt_free_os_def.h"

/* typedef -----------------------------------------------------------*/
typedef struct{
	uint8_t ucIndexA;
	uint8_t ucIndexB;
	uint8_t ucCmdForA;
	uint8_t ucCmdForB;
	uint8_t ucSwitchACase;
	uint8_t ucSwitchBCase;
	uint8_t ucStepASize;
	uint8_t ucStepBSize;
	uint16_t ucStepAPoint; //position of step. max = MAX_STEPPER_PULSE
	uint16_t ucStepBPoint; //position of step. max = MAX_STEPPER_PULSE
}STEPPER_MOTOR_CONTROL_TYPE;

/* define ------------------------------------------------------------*/
#define MAX_INDEX_OF_STEPPER_MOTOR 0x03
#define MAX_STEPPER_PULSE 0xFFF0
#define STEPPER_A_EN 0x01ul
#define STEPPER_B_EN 0x08ul
#define STEPPER_A_MASK 0x0006ul
#define STEPPER_B_MASK 0x0030ul
#define SWITCH_A_MASK 0x0001ul
#define SWITCH_B_MASK 0x0002ul
#define SWITCH_DETECT_CNT 0x04

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
extern STEPPER_MOTOR_CONTROL_TYPE stStepperMotorControl;

#endif
/* * * END OF FILE * * */

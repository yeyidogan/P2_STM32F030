/******************************************************************************
*	Written by Yilmaz Eyidogan
*	stepper motor functions
*	Created date: 2016.12.15
*******************************************************************************/
#ifndef __STEPPER_MOTOR_H_
#define __STEPPER_MOTOR_H_

#include "cmsis_os2.h"
#include <stdbool.h>
#include <stdint.h>
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "main.h"
#include "gpio.h"

/* typedef -----------------------------------------------------------*/
typedef struct{
	uint16_t step_size; //how many steps will be moved
	uint16_t step_point; //position of step. max = MAX_STEPPER_PULSE
	uint8_t index;
	uint8_t cmd;
	uint8_t switch_case; //switch data on zero point
	struct{
		uint8_t slow:1;
	}bit;
}STEPPER_MOTOR_CONTROL_TYPE;

/* define ------------------------------------------------------------*/
#define MAX_INDEX_OF_STEPPER_MOTOR 0x03
#define MAX_STEPPER_A_PULSE 1600
#define MAX_STEPPER_B_PULSE 0x0D3F
#define STEPPER_A_EN 0x0001ul
#define STEPPER_B_EN 0x0008ul
#define STEPPER_A_MASK 0x0006ul
#define STEPPER_B_MASK 0x0030ul
#define SWITCH_A_MASK 0x0001ul
#define SWITCH_B_MASK 0x0002ul
#define SWITCH_DETECT_CNT 0x04
#define STEPPER_ZERO_OFFSET 200
#define STEPPER_SLOW_SPEED_PULSE (STEPPER_ZERO_OFFSET + 200)
#define STEPPER_ZERO_POINT_TO_BACK 80

enum{
	STEPPER_STOP = (uint8_t)0x00,
	STEPPER_FORWARD,
	STEPPER_BACKWARD,
	STEPPER_TO_ZERO_POINT,
	STEPPER_TO_END_POINT
}; //command list

enum{
	MOTOR_A,
	MOTOR_B
};

/* macro -------------------------------------------------------------*/
/* variables ---------------------------------------------------------*/
extern STEPPER_MOTOR_CONTROL_TYPE motor_s[];
extern __NO_RETURN void task_stepper_motor(void *argument);

#endif
/* * * END OF FILE * * */

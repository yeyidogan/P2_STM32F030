/******************************************************************************
*	Written by Yilmaz Eyidogan
*	stepper motor functions
*	Created date: 2016.12.15
*******************************************************************************/
#include "stepper_motor.h"
/* typedef -----------------------------------------------------------*/
/* define ------------------------------------------------------------*/
/* macro -------------------------------------------------------------*/
/* variables ---------------------------------------------------------*/

//truth table for Onsemi LBC1848MC
const uint8_t step_array_full[] = {0x00, 0x01, 0x03, 0x02};
STEPPER_MOTOR_CONTROL_TYPE motor_s[2] = {0x00};

const uint8_t ucShift[2] = {0x01, 0x04};
const uint32_t ulStepperEn[2] = {STEPPER_A_EN, STEPPER_B_EN};
const uint32_t ulStepperMask[2] = {STEPPER_A_MASK, STEPPER_B_MASK};
const uint32_t ulSwitchMask[2] = {SWITCH_A_MASK, SWITCH_B_MASK};
uint8_t active_motor = MOTOR_B;

/* functions ---------------------------------------------------------*/
/**
  * @brief drive stepper motor B
  * @param  None
  * @retval None
  */

__NO_RETURN void task_stepper_motor(void *argument){
	
	while (1){
		++active_motor;
		active_motor &= 0x01; //max val is 1
		if (active_motor > 1){
			active_motor = 0;
		}
		
		switch (motor_s[active_motor].cmd){
			case STEPPER_FORWARD:
				if (motor_s[active_motor].step_size == 0x00 || \
						motor_s[active_motor].step_point >= MAX_STEPPER_PULSE){
					motor_s[active_motor].cmd = STEPPER_STOP;
					break;
				}
				ulOutputs |= ulStepperEn[active_motor]; //enable stepper coils
				ulOutputs &= ~ulStepperMask[active_motor]; //reset 2 bit step info
				motor_s[active_motor].index &= 0x03; //limit to max index
				ulOutputs |= step_array_full[motor_s[active_motor].index++] << ucShift[active_motor];
				++motor_s[active_motor].step_point;
				--motor_s[active_motor].step_size;
				setGpioOutputs();
				break;
			case STEPPER_BACKWARD:
			case STEPPER_TO_ZERO_POINT:
				if (motor_s[active_motor].cmd == STEPPER_BACKWARD){
					if (motor_s[active_motor].step_size == 0x00){
						motor_s[active_motor].cmd = STEPPER_STOP;
						break;
					}
				}
				if (motor_s[active_motor].step_point < 10){
					motor_s[active_motor].cmd = STEPPER_STOP;
					//error msg must be defined here: couldnt detect zero point switch
					break;
				}
				ulOutputs |= ulStepperEn[active_motor]; //enable stepper coils
				ulOutputs &= ~ulStepperMask[active_motor]; //reset 2 bit step info
				motor_s[active_motor].index &= 0x03; //limit to max index
				ulOutputs |= step_array_full[motor_s[active_motor].index--] << ucShift[active_motor];
				--motor_s[active_motor].step_point;
				if (motor_s[active_motor].cmd == STEPPER_BACKWARD)
					--motor_s[active_motor].step_size;
				setGpioOutputs();
				
				readGpioInputs();
				if (ulInputs && ulSwitchMask[active_motor] == 0x00){ //switch detected
					if (motor_s[active_motor].switch_case < SWITCH_DETECT_CNT)
						++motor_s[active_motor].switch_case;
				}
				else {//switch not detected
					if (motor_s[active_motor].switch_case > 0x00)
						--motor_s[active_motor].switch_case;
				}
				if (motor_s[active_motor].switch_case >= SWITCH_DETECT_CNT){
					motor_s[active_motor].cmd = STEPPER_STOP;
					motor_s[active_motor].step_point = STEPPER_ZERO_OFFSET;
				}
				break;
			default:
				motor_s[active_motor].cmd = STEPPER_STOP;
				break;
		}
		
		if (motor_s[active_motor].cmd == STEPPER_STOP){
			ulOutputs &= ~ulStepperEn[active_motor];
			setGpioOutputs();
		}
		if (motor_s[MOTOR_A].cmd == STEPPER_STOP || motor_s[MOTOR_B].cmd == STEPPER_STOP){
			osEventFlagsWait(event_general, EVENT_MASK_STEPPER_RUN, osFlagsWaitAny, osWaitForever);
		}
		osDelay(10);
	}
}
/* * * END OF FILE * * */

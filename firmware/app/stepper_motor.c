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
const uint8_t stepperMotorFullStepArray[] = {0x00, 0x01, 0x03, 0x02};
STEPPER_MOTOR_CONTROL_TYPE stStepperMotorControl = {0x00};
/* functions ---------------------------------------------------------*/
/**
  * @brief drive stepper motor
  * @param  None
  * @retval None
  */
__NO_RETURN void task_StepperA(void *argument){
	while (1){
		switch (stStepperMotorControl.ucCmdForA){
			case STEPPER_FORWARD:
				ulOutputs |= STEPPER_A_EN;
				ulOutputs &= ~STEPPER_A_MASK;
				ulOutputs |= stepperMotorFullStepArray[stStepperMotorControl.ucIndexA++] << 1;
				setGpioOutputs();
				PLT_FREE_OS_DELAY(20);
				++stStepperMotorControl.ucStepAPoint;
				--stStepperMotorControl.ucStepASize;
				if (stStepperMotorControl.ucStepASize == 0x00 || \
						stStepperMotorControl.ucStepAPoint >= MAX_STEPPER_PULSE){
					stStepperMotorControl.ucCmdForA = STEPPER_STOP;
					ulOutputs &= ~STEPPER_A_EN;
					setGpioOutputs();
				}
				break;
			case STEPPER_BACKWARD:
				
				readGpioInputs();
				if (ulInputs && SWITCH_A_MASK == 0x00) //switch detected
					++stStepperMotorControl.ucSwitchACase;
				else //switch not detected
					stStepperMotorControl.ucSwitchACase = 0x00;
				
				if (stStepperMotorControl.ucSwitchACase > SWITCH_DETECT_CNT){
					stStepperMotorControl.ucCmdForA = STEPPER_STOP;
					break;
				}
				break;
			case STEPPER_STOP:
				ulOutputs &= ~STEPPER_A_EN;
				setGpioOutputs();
				break;
			default:
				break;
		}
	}
}




/* * * END OF FILE * * */

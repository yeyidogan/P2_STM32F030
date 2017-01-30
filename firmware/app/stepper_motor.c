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
STEPPER_MOTOR_CONTROL_TYPE stMotorA = {0x00}, stMotorB = {0x00};
#if defined(__CC_ARM)
	osStatus_t statusT2;
#endif
/* functions ---------------------------------------------------------*/
/**
  * @brief drive stepper motor A
  * @param  None
  * @retval None
  */
__NO_RETURN void task_StepperA(void *argument){
#if defined(__CC_ARM)
	osThreadId_t threadId;

	threadId = osThreadGetId();
	if (threadId != NULL) {
		statusT2 = osThreadSetPriority (threadId, osPriorityBelowNormal);
		//if (statusT == osOK) {
		//}
	}
#endif
	
	while (1){
		switch (stMotorA.ucCmd){
			case STEPPER_FORWARD:
				ulOutputs |= STEPPER_A_EN;
				ulOutputs &= ~STEPPER_A_MASK;
				ulOutputs |= stepperMotorFullStepArray[stMotorA.ucIndex++] << 1;
				stMotorB.ucIndex &= 0x03; //limit to max index
				setGpioOutputs();
				PLT_FREE_OS_DELAY(20);
				++stMotorA.uiStepPoint;
				--stMotorA.uiStepSize;
				if (stMotorA.uiStepSize == 0x00 || \
						stMotorA.uiStepPoint >= MAX_STEPPER_PULSE){
					stMotorA.ucCmd = STEPPER_STOP;
					ulOutputs &= ~STEPPER_A_EN;
					setGpioOutputs();
				}
				break;
			case STEPPER_BACKWARD:
				readGpioInputs();
				if (ulInputs && SWITCH_A_MASK == 0x00) //switch detected
					++stMotorA.ucSwitchCase;
				else //switch not detected
					stMotorA.ucSwitchCase = 0x00;
				
				if (stMotorA.ucSwitchCase > SWITCH_DETECT_CNT){
					stMotorA.ucCmd = STEPPER_STOP;
					break;
				}
				break;
			case STEPPER_STOP:
				ulOutputs &= ~STEPPER_A_EN;
				setGpioOutputs();
				#if defined(__CC_ARM)
				osEventFlagsWait(event_General, EVENT_MASK_STEPPER_A_RUN, osFlagsWaitAny, osWaitForever);
				#elif defined(__GNUC__)
				CoWaitForSingleFlag(flag_StepperA, 0); //no time-out
				#endif
				break;
			default:
				break;
		}
	}
}

/**
  * @brief drive stepper motor B
  * @param  None
  * @retval None
  */
__NO_RETURN void task_StepperB(void *argument){
	while (1){
		switch (stMotorB.ucCmd){
			case STEPPER_FORWARD:
				ulOutputs |= STEPPER_B_EN;
				ulOutputs &= ~STEPPER_B_MASK;
				ulOutputs |= stepperMotorFullStepArray[stMotorB.ucIndex++] << 4;
				stMotorB.ucIndex &= 0x03; //limit to max index
				setGpioOutputs();
				PLT_FREE_OS_DELAY(20);
				++stMotorB.uiStepPoint;
				--stMotorB.uiStepSize;
				if (stMotorB.uiStepSize == 0x00 || \
						stMotorB.uiStepPoint >= MAX_STEPPER_PULSE){
					stMotorB.ucCmd = STEPPER_STOP;
					ulOutputs &= ~STEPPER_B_EN;
					setGpioOutputs();
				}
				break;
			case STEPPER_BACKWARD:
			case STEPPER_TO_ZERO_POINT:
				ulOutputs |= STEPPER_B_EN;
				ulOutputs &= ~STEPPER_B_MASK;
				ulOutputs |= stepperMotorFullStepArray[stMotorB.ucIndex--] << 4;
				stMotorB.ucIndex &= 0x03; //limit to max index
				setGpioOutputs();
				PLT_FREE_OS_DELAY(20);
				--stMotorB.uiStepPoint;
				if (stMotorB.ucCmd == STEPPER_BACKWARD)
					--stMotorB.uiStepSize;
				
				readGpioInputs();
				if (ulInputs && SWITCH_B_MASK == 0x00) //switch detected
					++stMotorB.ucSwitchCase;
				else //switch not detected
					stMotorB.ucSwitchCase = 0x00;
				if (stMotorB.ucSwitchCase > SWITCH_DETECT_CNT){
					stMotorB.ucCmd = STEPPER_STOP;
					stMotorB.uiStepPoint = STEPPER_ZERO_OFFSET;
					break;
				}
				
				if (stMotorB.ucCmd == STEPPER_BACKWARD)
					if (stMotorB.uiStepSize == 0x00)
						stMotorB.ucCmd = STEPPER_STOP;
				if (stMotorB.uiStepPoint < 10){
					stMotorB.ucCmd = STEPPER_STOP;
					//error msg must be defined here: couldnt detect zero point switch
				}
				break;
			case STEPPER_STOP:
				ulOutputs &= ~STEPPER_B_EN;
				setGpioOutputs();
				#if defined(__CC_ARM)
				osEventFlagsWait(event_General, EVENT_MASK_STEPPER_B_RUN, osFlagsWaitAny, osWaitForever);
				#elif defined(__GNUC__)
				CoWaitForSingleFlag(flag_StepperB, 0); //no time-out
				#endif
				break;
			default:
				stMotorB.ucCmd = STEPPER_STOP;
				PLT_FREE_OS_DELAY(10);
				break;
		}
	}
}



/* * * END OF FILE * * */

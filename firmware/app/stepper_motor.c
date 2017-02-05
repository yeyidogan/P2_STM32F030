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
  * @brief drive stepper motor B
  * @param  None
  * @retval None
  */
enum {
	MOTOR_A_IS_ACTIVE,
	MOTOR_B_IS_ACTIVE
};

__NO_RETURN void task_StepperMotor(void *argument){
	STEPPER_MOTOR_CONTROL_TYPE * stMotor[2];
	const uint8_t ucShift[2] = {0x01, 0x04};
	const uint32_t ulStepperEn[2] = {STEPPER_A_EN, STEPPER_B_EN};
	const uint32_t ulStepperMask[2] = {STEPPER_A_MASK, STEPPER_B_MASK};
	const uint32_t ulSwitchMask[2] = {SWITCH_A_MASK, SWITCH_B_MASK};
	uint8_t ucActiveMotor = MOTOR_B_IS_ACTIVE;
	
	while (1){
		++ucActiveMotor;
		ucActiveMotor &= 0x01; //max val is 1
		
		switch (stMotor[ucActiveMotor]->ucCmd){
			case STEPPER_FORWARD:
				if (stMotor[ucActiveMotor]->uiStepSize == 0x00 || \
						stMotor[ucActiveMotor]->uiStepPoint >= MAX_STEPPER_PULSE){
					stMotor[ucActiveMotor]->ucCmd = STEPPER_STOP;
					break;
				}
				ulOutputs |= ulStepperEn[ucActiveMotor]; //enable stepper coils
				ulOutputs &= ~ulStepperMask[ucActiveMotor]; //reset 2 bit step info
				stMotor[ucActiveMotor]->ucIndex &= 0x03; //limit to max index
				ulOutputs |= stepperMotorFullStepArray[stMotor[ucActiveMotor]->ucIndex++] << ucShift[ucActiveMotor];
				++stMotor[ucActiveMotor]->uiStepPoint;
				--stMotor[ucActiveMotor]->uiStepSize;
				setGpioOutputs();
				break;
			case STEPPER_BACKWARD:
			case STEPPER_TO_ZERO_POINT:
				if (stMotor[ucActiveMotor]->ucCmd == STEPPER_BACKWARD){
					if (stMotor[ucActiveMotor]->uiStepSize == 0x00){
						stMotor[ucActiveMotor]->ucCmd = STEPPER_STOP;
						break;
					}
				}
				if (stMotor[ucActiveMotor]->uiStepPoint < 10){
					stMotor[ucActiveMotor]->ucCmd = STEPPER_STOP;
					//error msg must be defined here: couldnt detect zero point switch
					break;
				}
				ulOutputs |= ulStepperEn[ucActiveMotor]; //enable stepper coils
				ulOutputs &= ~ulStepperMask[ucActiveMotor]; //reset 2 bit step info
				stMotor[ucActiveMotor]->ucIndex &= 0x03; //limit to max index
				ulOutputs |= stepperMotorFullStepArray[stMotor[ucActiveMotor]->ucIndex--] << ucShift[ucActiveMotor];
				--stMotor[ucActiveMotor]->uiStepPoint;
				if (stMotor[ucActiveMotor]->ucCmd == STEPPER_BACKWARD)
					--stMotor[ucActiveMotor]->uiStepSize;
				setGpioOutputs();
				
				readGpioInputs();
				if (ulInputs && ulSwitchMask[ucActiveMotor] == 0x00){ //switch detected
					if (stMotor[ucActiveMotor]->ucSwitchCase < SWITCH_DETECT_CNT)
						++stMotor[ucActiveMotor]->ucSwitchCase;
				}
				else {//switch not detected
					if (stMotor[ucActiveMotor]->ucSwitchCase > 0x00)
						--stMotor[ucActiveMotor]->ucSwitchCase;
				}
				if (stMotor[ucActiveMotor]->ucSwitchCase >= SWITCH_DETECT_CNT){
					stMotor[ucActiveMotor]->ucCmd = STEPPER_STOP;
					stMotor[ucActiveMotor]->uiStepPoint = STEPPER_ZERO_OFFSET;
				}
				break;
			default:
				stMotor[ucActiveMotor]->ucCmd = STEPPER_STOP;
				break;
		}
		
		if (stMotor[ucActiveMotor]->ucCmd == STEPPER_STOP){
			ulOutputs &= ~ulStepperEn[ucActiveMotor];
			setGpioOutputs();
		}
		if (stMotorA.ucCmd == STEPPER_STOP || stMotorA.ucCmd == STEPPER_STOP){
			#if defined(__CC_ARM)
			osEventFlagsWait(event_General, EVENT_MASK_STEPPER_B_RUN, osFlagsWaitAny, osWaitForever);
			#elif defined(__GNUC__)
			CoWaitForSingleFlag(flag_StepperB, 0); //no time-out
			#endif
		}
		PLT_FREE_OS_DELAY(10);
	}
}
/* * * END OF FILE * * */

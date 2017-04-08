/******************************************************************************
*	Written by Yilmaz Eyidogan
*	main application functions
*	Created date: 2017.04.09
*******************************************************************************/
#include "main_task.h"

/* define ------------------------------------------------------------*/
/* macro -------------------------------------------------------------*/
/* typedef -----------------------------------------------------------*/
/* variables ---------------------------------------------------------*/
uint16_t infrared_sensor_cnt = 0;
uint8_t cat_detection = NO_CAT;
/* functions ---------------------------------------------------------*/

/**
*******************************************************************************
* @brief main task that controls the device
* @param[in] 
* @param[out] 
*******************************************************************************
*/
__NO_RETURN void task_main(void *argument){
	while (1){
		readGpioInputs();
		if ((ulInputs & INFRARED_SENSOR_PIN) == 0x00){ //sensor detected
			++infrared_sensor_cnt;
		}
		else if (infrared_sensor_cnt > 0){
			--infrared_sensor_cnt;
		}
		if (infrared_sensor_cnt > INFRARED_SENSOR_CAT_DETECT_CNT){ //cat detected
			cat_detection = CAT_DETECTED;
		}
		if ((infrared_sensor_cnt == 0) && (cat_detection == CAT_DETECTED)){
			cat_detection = CAT_GONE_OUT;
			motor_s[MOTOR_HOLE_GATE].cmd = STEPPER_TO_END_POINT; //open gate
			osEventFlagsSet(event_general, EVENT_MASK_STEPPER_RUN);
			while (motor_s[MOTOR_HOLE_GATE].cmd == STEPPER_TO_END_POINT){
				osDelay(500);
			}
			motor_s[MOTOR_HOLE_GATE].cmd = STEPPER_TO_ZERO_POINT; //close gate
			osEventFlagsSet(event_general, EVENT_MASK_STEPPER_RUN);
			while (motor_s[MOTOR_HOLE_GATE].cmd == STEPPER_TO_ZERO_POINT){
				osDelay(500);
			}
			motor_s[MOTOR_LITTER_GATE].cmd = STEPPER_TO_END_POINT; //open gate
			osEventFlagsSet(event_general, EVENT_MASK_STEPPER_RUN);
			while (motor_s[MOTOR_LITTER_GATE].cmd == STEPPER_TO_END_POINT){
				osDelay(500);
			}
			motor_s[MOTOR_LITTER_GATE].cmd = STEPPER_TO_ZERO_POINT; //close gate
			osEventFlagsSet(event_general, EVENT_MASK_STEPPER_RUN);
			while (motor_s[MOTOR_LITTER_GATE].cmd == STEPPER_TO_ZERO_POINT){
				osDelay(500);
			}
		}
		osDelay(1000);
	}
}

/* * * END OF FILE * * */

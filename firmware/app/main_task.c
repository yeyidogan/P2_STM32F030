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
uint32_t task_cnt = 0x00ul;
uint16_t send_msg_cnt = 0x00;
uint8_t i;
/* functions ---------------------------------------------------------*/

/**
*******************************************************************************
* @brief main task that controls the device
* @param[in] 
* @param[out] 
*******************************************************************************
*/
__NO_RETURN void task_main(void *argument){
	
	//if (sys_par.uart1_protocol == PROTOCOL_MOBILE_APP){
	//	change_device_name((uint8_t *)"CatFx_Toilet");
	//}

	while (1){
		readGpioInputs();
		if ((ulInputs & INFRARED_SENSOR_PIN) == 0x00){ //sensor detected
			if (infrared_sensor_cnt <= INFRARED_SENSOR_CAT_DETECT_CNT){
				++infrared_sensor_cnt;
			}
			send_msg_cnt = 250;
		}
		else if (infrared_sensor_cnt > 1){
			infrared_sensor_cnt -= 2;
		}
		if (infrared_sensor_cnt >= INFRARED_SENSOR_CAT_DETECT_CNT){ //cat detected
			cat_detection = CAT_DETECTED;
		}
		if ((infrared_sensor_cnt < 2) && (cat_detection == CAT_DETECTED)){
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
				readGpioInputs();
				if ((ulInputs & INFRARED_SENSOR_PIN) == 0x00){ //sensor detected
					motor_s[MOTOR_HOLE_GATE].cmd = STEPPER_STOP;
					while (1){
						osDelay(1000);
						readGpioInputs();
						if ((ulInputs & INFRARED_SENSOR_PIN) != 0x00){ //sensor not detected
							motor_s[MOTOR_HOLE_GATE].cmd = STEPPER_TO_ZERO_POINT; //open gate
							osEventFlagsSet(event_general, EVENT_MASK_STEPPER_RUN);
							break;
						}
					}
				}
			}
			motor_s[MOTOR_LITTER_GATE].cmd = STEPPER_TO_END_POINT; //open gate
			osEventFlagsSet(event_general, EVENT_MASK_STEPPER_RUN);
			while (motor_s[MOTOR_LITTER_GATE].cmd == STEPPER_TO_END_POINT){
				osDelay(500);
			}
			//osDelay(2000);
			motor_s[MOTOR_LITTER_GATE].cmd = STEPPER_TO_ZERO_POINT; //close gate
			osEventFlagsSet(event_general, EVENT_MASK_STEPPER_RUN);
			while (motor_s[MOTOR_LITTER_GATE].cmd == STEPPER_TO_ZERO_POINT){
				osDelay(500);
			}
		}
		osDelay(1000);
		
		++task_cnt;
		if (send_msg_cnt){
			--send_msg_cnt;
			sprintf((char *)uart1Tx.buffer, "T:%d H:%d IS:%d C:%d\r\n", stTempHum.uiTemperature, stTempHum.uiHumidity, infrared_sensor_cnt, task_cnt);
			i = count_string(uart1Tx.buffer, UART_TX_BUFFER_SIZE);
			uart1TxCmd(uart1Tx.buffer, i);
		}
	}
}

/* * * END OF FILE * * */

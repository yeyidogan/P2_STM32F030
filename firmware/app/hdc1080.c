/******************************************************************************
*	Written by Yilmaz Eyidogan
*	HDC1080 Temperature and Humidity Sensor functions
*	Created date: 2016.12.08
*******************************************************************************/
#include "hdc1080.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ST_HDC1080_RD_TEMP_HUM_TYPE stTempHum = {0x00};
ST_HDC1080_STATUS_TYPE stHDC1080Status = {0x00};
uint16_t uiTimerHDC1080 = 0x00; //to control loops
osStatus_t statusT;
/* Private functions ---------------------------------------------------------*/
/**
  * @brief write configuration to HDC1080
  * @param  None
  * @retval None
  */
void write_HDC1080_configuration(void){
	statusT = osMutexAcquire(mutex_I2C, 1000); //wait for 1 second
	if (statusT == osOK){
		i2c_master_send_start_stop = I2C_MASTER_SEND_START;
		i2c_msg_tx_s.length_wr = HDC1080_WR_CONF_FRAME_SIZE;
		i2c_msg_tx_s.chip_add = HDC1080_I2C_SLAVE_ADD;
		i2c_master_process(I2C_MASTER_WRITE);
		START_HDC1080_TIMER;
		while (i2c_status_s.byte == 0x00){
			if (CHECK_HDC1080_TIMER_REACH_TO(HDC1080_COMM_TIMEOUT)){
				stHDC1080Status.ok = false;
				break;
			}
		}
		if (i2c_status_s.bits.completed_flag == false){
			stHDC1080Status.ok = false;
		}
	}
	else{
		stHDC1080Status.ok = false;
	}
	osMutexRelease(mutex_I2C);
}

/**
  * @brief read configuration from HDC1080
  * @param  None
  * @retval None
  */
void read_HDC1080_configuration(void){
	statusT = osMutexAcquire(mutex_I2C, 1000); //wait for 1 second
	if (statusT == osOK){
		i2c_master_send_start_stop = I2C_MASTER_SEND_RESTART;
		i2c_msg_tx_s.length_wr = HDC1080_RD_WR_CONF_POINTER_SIZE; //1 byte
		i2c_msg_tx_s.length_rd = HDC1080_RD_CONF_FRAME_SIZE; //3 bytes
		i2c_msg_tx_s.chip_add = HDC1080_I2C_SLAVE_ADD;
		i2c_master_process(I2C_MASTER_WRITE);
		
		START_HDC1080_TIMER;
		while (i2c_status_s.byte == 0x00){
			if (CHECK_HDC1080_TIMER_REACH_TO(HDC1080_COMM_TIMEOUT)){
				stHDC1080Status.ok = false;
				break;
			}
			osDelay(2);
		}
		if (i2c_status_s.bits.completed_flag == false){
			stHDC1080Status.ok = false;
		}
	}
	else{
		stHDC1080Status.ok = false;
	}
	osMutexRelease(mutex_I2C);
}
/**
  * @brief trig convertion of HDC1080
  * @param  None
  * @retval None
  */
void trig_HDC1080(void){
	statusT = osMutexAcquire(mutex_I2C, 1000); //wait for 1 second
	if (statusT == osOK){
		i2c_master_send_start_stop = I2C_MASTER_SEND_START;
		i2c_msg_tx_s.length_wr = 0x01;
		i2c_msg_tx_s.chip_add = HDC1080_I2C_SLAVE_ADD;
		PTR_HDC1080_CONF_FRAME_WR->ucRegister = TEMP_REG;
		//PTR_HDC1080_CONF_FRAME_WR->uiData = (uint16_t)0x0102;
		i2c_master_process(I2C_MASTER_WRITE);
		
		START_HDC1080_TIMER;
		while (i2c_status_s.byte == 0x00){
			if (CHECK_HDC1080_TIMER_REACH_TO(HDC1080_COMM_TIMEOUT)){
				stHDC1080Status.ok = false;
				break;
			}
		}
		if (i2c_status_s.bits.completed_flag == false){
			stHDC1080Status.ok = false;
		}
	}
	else{
		stHDC1080Status.ok = false;
	}
	osMutexRelease(mutex_I2C);
}
/**
  * @brief init HDC1080
  * @param  None
  * @retval None
  */
void init_HDC1080(void){
	stHDC1080Status.ok = true;
	PTR_HDC1080_CONF_FRAME_WR->ucRegister = CONFIGURATION_REG;
	PTR_HDC1080_CONF_FRAME_WR->uiData = HDC1080_CONF_REGISTER_VAL;
	write_HDC1080_configuration();
	osDelay(200);
}
/**
  * @brief get temperature and humidity from HDC1080
  * @param  None
  * @retval None
  */
__NO_RETURN void task_HDC1080(void *argument){
	uint32_t ulTemp;
	osThreadId_t threadId;

	threadId = osThreadGetId();
	if (threadId != NULL) {
		statusT = osThreadSetPriority (threadId, osPriorityBelowNormal);
		//if (statusT == osOK) {
		//}
	}

	while (1){
		if (stHDC1080Status.ok == true){
			trig_HDC1080();
			osDelay(100); //delay 0.1 sec
			if (stHDC1080Status.ok == false){
				continue;
			}

			statusT = osMutexAcquire(mutex_I2C, 1000); //wait for 1 second
			if (statusT == osOK){
				i2c_master_send_start_stop = I2C_MASTER_SEND_START;
				i2c_msg_tx_s.length_rd = sizeof(ST_HDC1080_RD_TEMP_HUM_TYPE);
				i2c_msg_tx_s.chip_add = HDC1080_I2C_SLAVE_ADD;
				i2c_master_process(I2C_MASTER_READ);
				
				START_HDC1080_TIMER;
				while (i2c_status_s.byte == 0x00){
					if (CHECK_HDC1080_TIMER_REACH_TO(1000)){
						stHDC1080Status.ok = false;
						break;
					}
				}
			}
			osMutexRelease(mutex_I2C);

			if (stHDC1080Status.ok == true){
				ulTemp = wordEndianer(PTR_HDC1080_TEMP_HUM_RD->uiTemperature);
				stTempHum.uiTemperature = ((uint32_t)(ulTemp * (uint32_t)165) / 0x10000) - 40;
				
				ulTemp = wordEndianer(PTR_HDC1080_TEMP_HUM_RD->uiHumidity);
				stTempHum.uiHumidity = (uint32_t)(ulTemp * (uint32_t)100) / 0x10000;
			}
		}
		else{
			init_HDC1080();
		}
		osDelay(1000); //1000 milisecond delay
	}
}
/* * * END OF FILE * * */

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
/* Private functions ---------------------------------------------------------*/
/**
  * @brief write configuration to HDC1080
  * @param  None
  * @retval None
  */
void write_HDC1080_configuration(void){
	stI2cMsgTx.length = HDC1080_WR_CONF_FRAME_SIZE;
	stI2cMsgTx.slaveAddress = HDC1080_I2C_SLAVE_ADD;
	i2c_master_process(I2C_MASTER_WRITE);
	while (stI2cStatus.completed_flag == false);
}

/**
  * @brief read configuration from HDC1080
  * @param  None
  * @retval None
  */
void read_HDC1080_configuration(void){
	ucI2CMasterSendStartStop = I2C_MASTER_SEND_RESTART;
	stI2cMsgTx.length = HDC1080_RD_WR_CONF_POINTER_SIZE; //1 byte
	stI2cMsgTx.slaveAddress = HDC1080_I2C_SLAVE_ADD;
	i2c_master_process(I2C_MASTER_WRITE);
	
	stI2cMsgTx.length = HDC1080_RD_CONF_FRAME_SIZE; //3 bytes
	stI2cMsgTx.slaveAddress = HDC1080_I2C_SLAVE_ADD;
		
	while (stI2cStatus.completed_flag == false);
}
/**
  * @brief trig convertion of HDC1080
  * @param  None
  * @retval None
  */
void start_HDC1080(void){
	stI2cMsgTx.length = sizeof(PTR_HDC1080_CONF_FRAME_WR);
	stI2cMsgTx.slaveAddress = HDC1080_I2C_SLAVE_ADD;
	PTR_HDC1080_CONF_FRAME_WR->ucRegister = TEMP_REG;
	PTR_HDC1080_CONF_FRAME_WR->uiData = (uint16_t)0x00;
	i2c_master_process(I2C_MASTER_WRITE);
	while (stI2cStatus.completed_flag == false);
}
/**
  * @brief init HDC1080
  * @param  None
  * @retval None
  */
void init_HDC1080(void){
	PTR_HDC1080_CONF_FRAME_WR->ucRegister = CONFIGURATION_REG;
	PTR_HDC1080_CONF_FRAME_WR->uiData = HDC1080_CONF_REGISTER_VAL;
	write_HDC1080_configuration();
	start_HDC1080();
	osDelay(2);
}
/**
  * @brief get temperature and humidity from HDC1080
  * @param  None
  * @retval None
  */
__NO_RETURN void get_temp_hum_from_HDC1080(void *argument){
	uint32_t ulTemp;
	osThreadId_t threadId;
	osStatus_t statusT;
	
	threadId = osThreadGetId();
	if (threadId != NULL) {
		//statusT = osThreadSetPriority (threadId, osPriorityBelowNormal);
		statusT = osThreadSetPriority (threadId, osPriorityBelowNormal);
		//if (statusT == osOK) {
		//}
	}
	while (1){
		statusT = osMutexAcquire(mutex_I2C, 1000); //wait for 1 second
		if (statusT == osOK){
			ucI2CMasterSendStartStop = I2C_MASTER_SEND_START;
			stI2cMsgTx.length = sizeof(ST_HDC1080_RD_TEMP_HUM_TYPE);
			stI2cMsgTx.slaveAddress = HDC1080_I2C_SLAVE_ADD;
			i2c_master_process(I2C_MASTER_READ);
			while (stI2cStatus.completed_flag == false);
			stTempHum.uiTemperature = PTR_HDC1080_TEMP_HUM_RD->uiTemperature;
			stTempHum.uiHumidity = PTR_HDC1080_TEMP_HUM_RD->uiHumidity;
			
			ulTemp = ((uint32_t)(stTempHum.uiTemperature * (uint32_t)165) / 0x10000) - 40;
			stTempHum.uiTemperature = (uint16_t)ulTemp;
			
			ulTemp = (uint32_t)(stTempHum.uiHumidity * (uint32_t)100) / 0x10000;
			stTempHum.uiHumidity = (uint16_t)ulTemp;
			
			osMutexRelease(mutex_I2C);
			osDelay(100); //1 second delay
			continue;
		}
		osDelay(2); //20 milisecond delay if I2C mutex cannot be acquired
	}
}
/* * * END OF FILE * * */
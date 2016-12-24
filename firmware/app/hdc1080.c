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
#if defined(__CC_ARM)
	osStatus_t statusT;
#endif
/* Private functions ---------------------------------------------------------*/
/**
  * @brief write configuration to HDC1080
  * @param  None
  * @retval None
  */
void write_HDC1080_configuration(void){
#if defined(__CC_ARM)
	statusT = osMutexAcquire(mutex_I2C, 1000); //wait for 1 second
	if (statusT == osOK){
#elif defined(__GNUC__)
		CoEnterMutexSection(mutex_I2C);
#endif
		ucI2CMasterSendStartStop = I2C_MASTER_SEND_START;
		stI2cMsgTx.length = HDC1080_WR_CONF_FRAME_SIZE;
		stI2cMsgTx.slaveAddress = HDC1080_I2C_SLAVE_ADD;
		i2c_master_process(I2C_MASTER_WRITE);
		START_HDC1080_TIMER;
		while (stI2cStatus.byte == 0x00){
			if (CHECK_HDC1080_TIMER_REACH_TO(HDC1080_COMM_TIMEOUT)){
				stHDC1080Status.ok = false;
				break;
			}
		}
		if (stI2cStatus.bits.completed_flag == false)
			stHDC1080Status.ok = false;
	}
#if defined(__CC_ARM)
	else
		stHDC1080Status.ok = false;
	osMutexRelease(mutex_I2C);
#elif defined(__GNUC__)
		CoLeaveMutexSection(mutex_I2C);
#endif
}

/**
  * @brief read configuration from HDC1080
  * @param  None
  * @retval None
  */
void read_HDC1080_configuration(void){
#if defined(__CC_ARM)
	statusT = osMutexAcquire(mutex_I2C, 1000); //wait for 1 second
	if (statusT == osOK){
#elif defined(__GNUC__)
		CoEnterMutexSection(mutex_I2C);
#endif
		ucI2CMasterSendStartStop = I2C_MASTER_SEND_RESTART;
		stI2cMsgTx.length = HDC1080_RD_WR_CONF_POINTER_SIZE; //1 byte
		stI2cMsgTx.slaveAddress = HDC1080_I2C_SLAVE_ADD;
		i2c_master_process(I2C_MASTER_WRITE);
		
		stI2cMsgTx.length = HDC1080_RD_CONF_FRAME_SIZE; //3 bytes
		stI2cMsgTx.slaveAddress = HDC1080_I2C_SLAVE_ADD;
			
		START_HDC1080_TIMER;
		while (stI2cStatus.byte == 0x00){
			if (CHECK_HDC1080_TIMER_REACH_TO(HDC1080_COMM_TIMEOUT)){
				stHDC1080Status.ok = false;
				break;
			}
		}
		if (stI2cStatus.bits.completed_flag == false)
			stHDC1080Status.ok = false;
	}
#if defined(__CC_ARM)
	else
		stHDC1080Status.ok = false;
	osMutexRelease(mutex_I2C);
#elif defined(__GNUC__)
		CoLeaveMutexSection(mutex_I2C);
#endif
}
/**
  * @brief trig convertion of HDC1080
  * @param  None
  * @retval None
  */
void trig_HDC1080(void){
#if defined(__CC_ARM)
	statusT = osMutexAcquire(mutex_I2C, 1000); //wait for 1 second
	if (statusT == osOK){
#elif defined(__GNUC__)
		CoEnterMutexSection(mutex_I2C);
#endif
		ucI2CMasterSendStartStop = I2C_MASTER_SEND_START;
		stI2cMsgTx.length = sizeof(ST_HDC1080_WR_CONF_FRAME_TYPE);
		stI2cMsgTx.slaveAddress = HDC1080_I2C_SLAVE_ADD;
		PTR_HDC1080_CONF_FRAME_WR->ucRegister = TEMP_REG;
		PTR_HDC1080_CONF_FRAME_WR->uiData = (uint16_t)0x0102;
		i2c_master_process(I2C_MASTER_WRITE);
		
		START_HDC1080_TIMER;
		while (stI2cStatus.byte == 0x00){
			if (CHECK_HDC1080_TIMER_REACH_TO(HDC1080_COMM_TIMEOUT)){
				stHDC1080Status.ok = false;
				break;
			}
		}
		if (stI2cStatus.bits.completed_flag == false)
			stHDC1080Status.ok = false;
	}
#if defined(__CC_ARM)
	else
		stHDC1080Status.ok = false;
	osMutexRelease(mutex_I2C);
#elif defined(__GNUC__)
		CoLeaveMutexSection(mutex_I2C);
#endif
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
	pltFreeOsDelay(200);
}
/**
  * @brief get temperature and humidity from HDC1080
  * @param  None
  * @retval None
  */
__NO_RETURN void task_HDC1080(void *argument){
	uint32_t ulTemp;
#if defined(__CC_ARM)
	osThreadId_t threadId;

	threadId = osThreadGetId();
	if (threadId != NULL) {
		statusT = osThreadSetPriority (threadId, osPriorityBelowNormal);
		//if (statusT == osOK) {
		//}
	}
#endif

	//ulTemp = (uint32_t)0x03;
	//while (ulTemp--){
	//	init_HDC1080();
	//	if (stHDC1080Status.ok == true){
	//		break;
	//	}
	//	pltFreeOsDelay(200); //delay 0.2 sec
	//}
	while (1){
		if (stHDC1080Status.ok == true){
			trig_HDC1080();
			if (stHDC1080Status.ok == false){
				pltFreeOsDelay(200); //delay 0.2 sec
				continue;
			}
			pltFreeOsDelay(200); //delay 0.2 sec

		#if defined(__CC_ARM)
			statusT = osMutexAcquire(mutex_I2C, 1000); //wait for 1 second
			if (statusT == osOK){
		#elif defined(__GNUC__)
				CoEnterMutexSection(mutex_I2C);
		#endif
				ucI2CMasterSendStartStop = I2C_MASTER_SEND_RESTART;
				stI2cMsgTx.length = sizeof(ST_HDC1080_RD_TEMP_HUM_TYPE);
				stI2cMsgTx.slaveAddress = HDC1080_I2C_SLAVE_ADD;
				i2c_master_process(I2C_MASTER_READ);
				
				START_HDC1080_TIMER;
				while (stI2cStatus.byte == 0x00){
					if (CHECK_HDC1080_TIMER_REACH_TO(1000)){
						stHDC1080Status.ok = false;
						break;
					}
				}
		#if defined(__CC_ARM)	
			}
		#endif
		#if defined(__CC_ARM)
			osMutexRelease(mutex_I2C);
		#elif defined(__GNUC__)
			CoLeaveMutexSection(mutex_I2C);
		#endif		

			if (stHDC1080Status.ok == true){
				stTempHum.uiTemperature = PTR_HDC1080_TEMP_HUM_RD->uiTemperature;
				stTempHum.uiHumidity = PTR_HDC1080_TEMP_HUM_RD->uiHumidity;

				ulTemp = ((uint32_t)(stTempHum.uiTemperature * (uint32_t)165) / 0x10000) - 40;
				stTempHum.uiTemperature = (uint16_t)ulTemp;

				ulTemp = (uint32_t)(stTempHum.uiHumidity * (uint32_t)100) / 0x10000;
				stTempHum.uiHumidity = (uint16_t)ulTemp;
			}
		}
		else{
			init_HDC1080();
		}
		pltFreeOsDelay(1000); //1000 milisecond delay
	}
}
/* * * END OF FILE * * */

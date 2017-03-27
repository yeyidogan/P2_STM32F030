/******************************************************************************
*	Written by Yilmaz Eyidogan
*	main functions
*	Created date: 2016.12.09
*******************************************************************************/
#include "main.h"

/* mutexes ---------------------------------------------------------*/
osMutexId_t mutex_I2C;
/* flags -----------------------------------------------------------*/
osEventFlagsId_t event_general; 

/**
 *******************************************************************************
 * @brief       "led" task code
 * @param[in]   None
 * @param[out]  None
 * @retval      None
 * @par Description
 * @details    This function use to blink led,and set flag for "taskA" and "taskB".
 *******************************************************************************
 */
#define BSRR_VAL 0x0300
__NO_RETURN void task_Led(void *argument) {
	while (1) {
		/* Set PC8 and PC9 */
		GPIOC->BSRR = BSRR_VAL;
		/* Delay some time */
		//for(times = 0; times < 500000; times++);
		osDelay(100);
		/* Reset PC8 and PC9 */
		GPIOC->BRR = BSRR_VAL;
		/* Delay some time */
		//for(times = 0; times < 500000; times++);
		osDelay(400);
	}
}

extern void SystemCoreClockUpdate (void);
/**
 *******************************************************************************
 * @brief       main function
 * @param[in]   None
 * @param[out]  None
 * @retval      int
 * @par Description
 * @details    This is main function
 *******************************************************************************
 */
int main(void){
	SystemCoreClockUpdate();
	initGpio();
	setParameters();
	initUart1(sys_par.uart1_baudrate);
	//initUart2(sys_par.uart2_baudrate);
	RCC->AHBENR |= RCC_AHBENR_DMAEN; /* Enable DMA1 clock */
	initUartDma();
	uartNvicConfig();
	initTimers();
	initI2C();

	//if (sys_par.uart1_protocol == PROTOCOL_MOBILE_APP)
	//	change_device_name((uint8_t *)"CatFx_Toilet");
	
	if(osKernelGetState() == osKernelInactive)
    osKernelInitialize();
	
	mutex_I2C = osMutexNew(NULL);
	event_general = osEventFlagsNew(NULL);
	
	osThreadNew(task_Led, NULL, NULL);
	osThreadNew(task_HDC1080, NULL, NULL);
	osThreadNew(task_Uart1, NULL, NULL);
	osThreadNew(task_stepper_motor, NULL, NULL);
	
	if (osKernelGetState() == osKernelReady){ // If kernel is ready to run...
    osKernelStart(); // ... start thread execution
  }
  while(1); // only reached in case of error
}
/**
 *******************************************************************************
 * @brief       Check parameters of STM32 Std drivers
 *******************************************************************************
 */
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line){
	char msg[32] = "";
	sprintf(msg, "line %uld", line);
	uart1TxCmd((uint8_t *)file, 6);
	uart1TxCmd((uint8_t *)msg, 8);
	while (1);
}
#endif
/* * * END OF FILE * * */

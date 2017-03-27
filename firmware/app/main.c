/******************************************************************************
*	Written by Yilmaz Eyidogan
*	main functions
*	Created date: 2016.12.09
*******************************************************************************/
#include "main.h"

#if defined(__GNUC__)
#define STACK_SIZE_TASK_LED 128 /*!< Define "taskA" task size */
#define STACK_SIZE_TASK_UART 128 /*!< Define "taskA" task size */
#define STACK_SIZE_TASK_HDC1080 128 /*!< Define "taskA" task size */

OS_STK taskLed_stk[STACK_SIZE_TASK_LED];	/*!< Define "led" task stack   */
OS_STK taskUart_stk[STACK_SIZE_TASK_UART];	/*!< Define "uart" task stack   */
OS_STK taskHDC1080_stk[STACK_SIZE_TASK_HDC1080];	/*!< Define "uart" task stack   */
#endif

/* mutexes ---------------------------------------------------------*/
PLT_FREE_OS_MUTEX_ID mutex_I2C;
/* flags -----------------------------------------------------------*/
#if defined(__CC_ARM)
osEventFlagsId_t event_General; 
#elif defined(__GNUC__)
OS_FlagID flag_UartTimeout, flag_StepperA, flag_StepperB;
#endif

/*
 *******************************************************************************
 * @brief       Enable peripheral clocks
 *******************************************************************************
 */
void rccConfig(void){
	//RCC_HCLKConfig(RCC_SYSCLK_Div1);
	//RCC->CFGR |= RCC_CFGR_PPRE_DIV4;
	//RCC_PCLKConfig(RCC_HCLK_Div4);
	/* GPIOA Peripheral clock enable */
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	/* GPIOB Peripheral clock enable */
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	/* GPIOC Peripheral clock enable */
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	/* Enable USART1 APB clock */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	/* Enable DMA1 clock */
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* Configure I2C Clock Source*/
//	RCC_I2CCLKConfig(RCC_I2C1CLK_HSI); //8 MHz clock source selected
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
}

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
		PLT_FREE_OS_DELAY(100);
		/* Reset PC8 and PC9 */
		GPIOC->BRR = BSRR_VAL;
		/* Delay some time */
		//for(times = 0; times < 500000; times++);
		PLT_FREE_OS_DELAY(400);
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
#if defined(__CC_ARM)
	//osThreadId_t osID;
#endif
	SystemCoreClockUpdate();
	//rccConfig();
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
	
#if defined(__CC_ARM)
	if(osKernelGetState() == osKernelInactive)
    osKernelInitialize();
	
	mutex_I2C = osMutexNew(NULL);
	event_General = osEventFlagsNew(NULL);
	
	osThreadNew(task_Led, NULL, NULL);
	osThreadNew(task_HDC1080, NULL, NULL);
	osThreadNew(task_Uart1, NULL, NULL);
	//osThreadNew(task_StepperMotor, NULL, NULL);
	
	if (osKernelGetState() == osKernelReady){ // If kernel is ready to run...
    osKernelStart(); // ... start thread execution
  }
#elif defined(__GNUC__)
	mutex_I2C = CoCreateMutex();
	flag_UartTimeout = CoCreateFlag(FLAG_RESET_AUTO, FLAG_NON_READY_STATE);
	CoInitOS(); /*!< Initial CooCox CoOS */
	CoCreateTask((FUNCPtr)task_Led,(void *)0,2,&taskLed_stk[STACK_SIZE_TASK_LED-1],STACK_SIZE_TASK_LED);
	CoCreateTask((FUNCPtr)task_Uart1,(void *)0,2,&taskUart_stk[STACK_SIZE_TASK_UART-1],STACK_SIZE_TASK_UART);
	CoCreateTask((FUNCPtr)task_HDC1080,(void *)0,2,&taskHDC1080_stk[STACK_SIZE_TASK_HDC1080-1],STACK_SIZE_TASK_HDC1080);
	CoStartOS(); /*!< Start multitask*/
#endif
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

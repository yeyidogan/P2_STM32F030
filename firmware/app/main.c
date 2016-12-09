/******************************************************************************
*	Written by Yilmaz Eyidogan
*	main functions
*	Created date: 2016.12.09
*******************************************************************************/
#include "main.h"

/* mutexes ---------------------------------------------------------*/
osMutexId_t mutex_I2C;

__NO_RETURN void thread1 (void *argument) {     
    //; // work a lot on data[] 
        osDelay(1000);       
        osThreadExit();
}

//osThreadAttr_t thread1_attr = {0};
extern void SystemCoreClockUpdate (void);
int main(void){
	SystemCoreClockUpdate();
	rccConfig();
	initGpio();
	
	if(osKernelGetState() == osKernelInactive)
    osKernelInitialize();
	
	mutex_I2C = osMutexNew(NULL);
	//osThreadNew(thread1, NULL, NULL);
	osThreadNew(taskC, NULL, NULL);
	//osThreadNew(get_temp_hum_from_HDC1080, NULL, NULL);

	if (osKernelGetState() == osKernelReady){ // If kernel is ready to run...
    osKernelStart(); // ... start thread execution
  }
  while(1); // only reached in case of error
	//return 0;
}


/*
 *******************************************************************************
 * @brief       Enable peripheral clocks
 *******************************************************************************
 */
void rccConfig(void){
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLKConfig(RCC_HCLK_Div1);
	/* GPIOA Peripheral clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	/* GPIOC Peripheral clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	/* Enable USART1 APB clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	/* Enable DMA1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* Configure I2C Clock Source*/
	RCC_I2CCLKConfig(RCC_I2C1CLK_HSI); //8 MHz clock source selected
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
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
__NO_RETURN void taskC (void *argument) {
	//unsigned int led_num;
	while (1) {
		/* Set PC8 and PC9 */
		GPIOC->BSRR = BSRR_VAL;
		/* Delay some time */
		//for(times = 0; times < 500000; times++);
		osDelay(10);
		/* Reset PC8 and PC9 */
		GPIOC->BRR = BSRR_VAL;
		/* Delay some time */
		//for(times = 0; times < 500000; times++);
		osDelay(50);
	}
}

/**
 *******************************************************************************
 * @brief       Check parameters of STM32 Std drivers
 *******************************************************************************
 */
void assert_failed(uint8_t* file, uint32_t line){
	while (1);
}
/* * * END OF FILE * * */

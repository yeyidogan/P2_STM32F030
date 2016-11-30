#include "cmsis_os2.h"
#include "main.h"

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
	//osThreadNew(thread1, NULL, NULL);
	osThreadNew(taskC, NULL, NULL);

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
	/* GPIOA Peripheral clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	/* GPIOC Peripheral clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	/* Enable USART1 APB clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	/* Enable DMA1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
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
void taskC (void *argument) {
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

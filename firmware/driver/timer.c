/******************************************************************************
*	Written by Yilmaz Eyidogan
*	Timer functions
*	Created date: 2016.12.10
*******************************************************************************/
#include "timer.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
  * @brief timer14 interrupt
  * @param  None
  * @retval None
  */
void TIM6_IRQHandler(void){
	if (uiTimerI2C < 20000)
		++uiTimerI2C;
	else
		__NOP;
	/* Clear the IT pending Bit */
	TIM6->SR = (uint16_t)~0x0001;
}
/**
  * @brief init timers
  * @param  None
  * @retval None
  */
void init_timers(void){
	//     TIMER 6              //
	/* Enable Timer 6 reset state */
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM6, ENABLE);
	/* Release Timer 6 from reset state */
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM6, DISABLE);

	TIM6->CR1 = (uint16_t)0x0084;
	//bit15-8:0 reserved
	//bit7:1 ARPE auto reload preload enabled
	//bit6-4:0 Reserved
	//bit3:0 One pulse mode disabled
	//bit2:1 Update request source is only overflow/underflow or DMA
	//bit1:0 UEV update event is generated
	//bit0:0 Counter disabled

	/* Enable the Interrupt sources */
	TIM6->DIER = (uint16_t)0x0001;
	//TIM6->EGR = (uint16_t)0x0001;
	TIM6->PSC = (uint16_t)1199; //0.1 milisecond CNT counter
	TIM6->ARR = (uint16_t)9;  //1 milisecond timer
	
	TIM6->CR1 |= (uint16_t)0x0001; //counter enabled
	NVIC_EnableIRQ(TIM6_IRQn);
	NVIC_SetPriority(TIM6_IRQn,0);
}

/* * * END OF FILE * * */

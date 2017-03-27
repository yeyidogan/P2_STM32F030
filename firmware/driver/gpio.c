/******************************************************************************
*	Written by Yilmaz Eyidogan
*	GPIO functions
*	Created date: 2014.12.22
*******************************************************************************/
#include "gpio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t ulInputs, ulOutputs, ulTmp;

/**
  * @brief Configure the GPIO
  * @param  None
  * @retval None
  */
void initGpio(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOA Peripheral clock enable */
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	//GPIO DeInit
	RCC->AHBRSTR |= (RCC_AHBENR_GPIOAEN|RCC_AHBENR_GPIOBEN|RCC_AHBENR_GPIOCEN);
	RCC->AHBRSTR &= ~(RCC_AHBENR_GPIOAEN|RCC_AHBENR_GPIOBEN|RCC_AHBENR_GPIOCEN);

	/* USART1 Pins configuration */
	/* Connect pin to Peripherals */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
	/* USART2 Pins configuration */
	/* Connect pin to Peripherals */
	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource14, GPIO_AF_1);
	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_1);

	/* Configure pins as AF push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure PA0 to PA7 in input mode */
	GPIO_InitStructure.GPIO_Pin = ((uint16_t)0x00FF); //PA0..PA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/***************************************************/

	/* GPIOB Peripheral clock enable */
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	/* I2C1 Pins Configuration */
	/* Connect pin to Peripherals */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_1);
	//PB6 = I2C1_SCL, PB7 = I2C1_SDA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/***************************************************/

	/* GPIOC Peripheral clock enable */
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	/* Configure PC0 to PC9 in output push-pull mode */
	GPIO_InitStructure.GPIO_Pin = ((uint16_t)0x03FF); //PC0..PC9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/***************************************************/
}
/**
  * @brief set outputs
  */
void setGpioOutputs(void){
	ulOutputs &= 0x00FFUL;
	ulTmp = OUTPUT_PORT_07->ODR;
	ulTmp &= 0xFFFFFF00;
	ulTmp += ulOutputs;
	OUTPUT_PORT_07->ODR = (uint16_t)ulTmp;
}
/**
  * @brief read outputs
  */
void readGpioOutputs(void){
	ulOutputs = OUTPUT_PORT_07->ODR & 0xFFUL;
}
/**
  * @brief read inputs
  */
void readGpioInputs(void){
	ulInputs = INPUT_PORT_07->IDR & 0x00FFUL;
}
/* * * END OF FILE * * */

/******************************************************************************
*	Written by Yilmaz Eyidogan
*	UART functions
*	Created date: 2014.12.24
*******************************************************************************/
#include "uart.h"

/* Private typedef */
/* Private define */

/* Private macro */
/* Private variables */

UART_RX_BUFFER_TYPE uart1Rx = {0x00, 0x00, {0x00}};
UART_TX_BUFFER_TYPE uart1Tx = {0x00, 0x00, "uart1 transmit data\r\n"};
UART_STATUS_TYPE uart1Flags = {0x00};
uint8_t uiTmp = 0x00;
uint16_t uiTimerUart1 = 0x00; //to control loops

UART_RX_BUFFER_TYPE uart2Rx = {0x00, 0x00, {0x00}};
UART_TX_BUFFER_TYPE uart2Tx = {0x00, 0x00, "uart2 transmit data\r\n"};
UART_STATUS_TYPE uart2Flags = {0x00};
uint16_t uiTimerUart2 = 0x00; //to control loops


/**
  * @brief  This function handles USART1 global interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void){

#if defined(__GNUC__)
	CoEnterISR();
#endif
	if (USART_GetITStatus(USART1, USART_IT_RTO) != RESET) { //timeout flag
		uart1Flags.rxTimeOut = TRUE;
	}
	USART_ClearITPendingBit(USART1, USART_IT_RTO);

	DMA_Cmd(DMA1_Channel3, DISABLE); //disable uart1 rx dma
	if (uart1Flags.rxOnlineBuffer == UART_RX_BUFFER1_WAITING) {
		DMA1_Channel3->CMAR = (uint32_t)uart1Rx.buffer2;
		uart1Flags.rxOnlineBuffer = UART_RX_BUFFER2_WAITING;
		uart1Rx.length1 = UART_RX_BUFFER_SIZE - (uint8_t)DMA1_Channel3->CNDTR;
	}
	else {
		DMA1_Channel3->CMAR = (uint32_t)uart1Rx.buffer1;
		uart1Flags.rxOnlineBuffer = UART_RX_BUFFER1_WAITING;
		uart1Rx.length2 = UART_RX_BUFFER_SIZE - (uint8_t)DMA1_Channel3->CNDTR;
	}
	DMA1_Channel3->CNDTR = UART_RX_BUFFER_SIZE;
	DMA_Cmd(DMA1_Channel3, ENABLE); //uart1 rx dma
#if defined(__CC_ARM)
	osEventFlagsSet(event_General, EVENT_MASK_UART1_TIMEOUT);
#elif defined(__GNUC__)
	isr_SetFlag(flag_UartTimeout);
#endif
	#if 0
	//TX and RX interrupts
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		/* Read one byte from the receive data register */
		uart1Rx.buffer[uart1Rx.bufferId++] = (uint8_t)USART_ReceiveData(USART1);

		if (uart1Rx.bufferId >= UART1_RX_BUFFER_SIZE)
		{
			/* Disable the USART1 Receive interrupt */
			//USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
			uart1Rx.bufferId = UART1_RX_BUFFER_SIZE - 0x01;
		}
	}

	if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{
		/* Write one byte to the transmit data register */
		USART_SendData(USART1, uart1Tx.buffer[uart1Tx.bufferId++]);
	    if(uart1Tx.bufferId >= uart1Tx.length)
	    {
	      /* Disable the USART1 Transmit interrupt */
	      USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	    }
	}
	#endif

#if defined(__GNUC__)
	CoExitISR();
#endif
}
/**
  * @brief  This function handles USART2 global interrupt request.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void){

#if defined(__GNUC__)
	CoEnterISR();
#endif
	if (USART_GetITStatus(USART2, USART_IT_RTO) != RESET) { //timeout flag
		uart2Flags.rxTimeOut = TRUE;
	}
	USART_ClearITPendingBit(USART2, USART_IT_RTO);

	DMA_Cmd(DMA1_Channel5, DISABLE); //disable uart2 rx dma
	if (uart2Flags.rxOnlineBuffer == UART_RX_BUFFER1_WAITING) {
		DMA1_Channel5->CMAR = (uint32_t)uart2Rx.buffer2;
		uart2Flags.rxOnlineBuffer = UART_RX_BUFFER2_WAITING;
		uart2Rx.length1 = UART_RX_BUFFER_SIZE - (uint8_t)DMA1_Channel5->CNDTR;
	}
	else {
		DMA1_Channel5->CMAR = (uint32_t)uart2Rx.buffer1;
		uart2Flags.rxOnlineBuffer = UART_RX_BUFFER1_WAITING;
		uart2Rx.length2 = UART_RX_BUFFER_SIZE - (uint8_t)DMA1_Channel5->CNDTR;
	}
	DMA1_Channel3->CNDTR = UART_RX_BUFFER_SIZE;
	DMA_Cmd(DMA1_Channel5, ENABLE); //uart2 rx dma
#if defined(__CC_ARM)
	osEventFlagsSet(event_General, EVENT_MASK_UART2_TIMEOUT);
#elif defined(__GNUC__)
	isr_SetFlag(flag_UartTimeout);
#endif
	
#if defined(__GNUC__)
	CoExitISR();
#endif
}

/**
  * @brief  This function handles DMA Transfer Complete interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel2_3_IRQHandler(void) {
#if defined(__GNUC__)
	CoEnterISR();
#endif
	DMA_Cmd(DMA1_Channel2, DISABLE); //disable uart1 tx dma
	DMA_ClearFlag(DMA_ISR_GIF2);
	DMA_ClearITPendingBit(DMA_ISR_GIF2);
	uart1Flags.txBusy = UART_TX_READY;

	DMA_Cmd(DMA1_Channel2, ENABLE); //uart1 tx dma
#if defined(__GNUC__)
	CoExitISR();
#endif
}

/**
  * @brief  This function handles DMA Transfer Complete interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel4_5_IRQHandler(void) {
#if defined(__GNUC__)
	CoEnterISR();
#endif
	DMA_Cmd(DMA1_Channel4, DISABLE); //disable uart2 tx dma
	DMA_ClearFlag(DMA_ISR_GIF4);
	DMA_ClearITPendingBit(DMA1_FLAG_GL4);
	uart2Flags.txBusy = UART_TX_READY;

	DMA_Cmd(DMA1_Channel4, ENABLE); //uart1 tx dma
#if defined(__GNUC__)
	CoExitISR();
#endif
}

/**
  * @brief Configure the USART1 Device
  * @param  baudRate
  * @retval None
  */
void initUart1(uint32_t baudRate) {
	/* USARTx configured as follow:
	- BaudRate = 9600 baud
	- Word Length = 8 Bits
	- Stop Bit = 1 Stop Bit
	- Parity = No Parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
	*/
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; //Enable Usart1 clock
	USART_InitTypeDef USART_InitStructure;

	//USART DeInit
	RCC->APB2RSTR |= RCC_APB2ENR_USART1EN;
	RCC->APB2RSTR &= ~RCC_APB2ENR_USART1EN;
	
	USART_InitStructure.USART_BaudRate = baudRate; //9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	USART_SetReceiverTimeOut(USART1, UART1_RX_TIMEOUT);
	USART_ReceiverTimeOutCmd(USART1, ENABLE);
	USART_ITConfig(USART1, USART_IT_RTO, ENABLE); //enable usart1 timeout, required for modbus

	USART1->CR1 |= USART_CR1_UE; /* Enable USART */
}

/**
  * @brief Configure the USART2 Device
  * @param  baudRate
  * @retval None
  */
void initUart2(uint32_t baudRate) {
	/* USARTx configured as follow:
	- BaudRate = 9600 baud
	- Word Length = 8 Bits
	- Stop Bit = 1 Stop Bit
	- Parity = No Parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
	*/
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //Enable Usart2 clock
	USART_InitTypeDef USART_InitStructure;

	//USART DeInit
	RCC->APB1RSTR |= RCC_APB1ENR_USART2EN;
	RCC->APB1RSTR &= ~RCC_APB1ENR_USART2EN;
	
	USART_InitStructure.USART_BaudRate = baudRate; //9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	USART_SetReceiverTimeOut(USART2, UART2_RX_TIMEOUT);
	USART_ReceiverTimeOutCmd(USART2, ENABLE);
	USART_ITConfig(USART2, USART_IT_RTO, ENABLE); //enable usart2 timeout, required for modbus

	USART2->CR1 |= USART_CR1_UE; /* Enable USART */
}

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void uartNvicConfig(void) {
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USART1 DMA Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the USART1 (TimeOut) Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the USART2 DMA Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the USART2 (TimeOut) Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  This function handles USART1 DMA.
  * @param  None
  * @retval None
  */
void initUartDma(void) {
	DMA_InitTypeDef DMA_InitStructure;

//USART1 configuration
	/* DMA1 Channel3 Config: Uart1 Rx data */
	DMA_DeInit(DMA1_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->RDR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)uart1Rx.buffer1;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = UART_RX_BUFFER_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	/* DMA1 Channel3 enable */
	DMA_Cmd(DMA1_Channel3, ENABLE);
	uart1Flags.rxOnlineBuffer = UART_RX_BUFFER1_WAITING;

	/* DMA1 Channel2 Config: Uart1 Tx data */
	uart1Flags.txBusy = UART_TX_BUSY;
	DMA_DeInit(DMA1_Channel2);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->TDR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)uart1Tx.buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = uart1Tx.length;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel2, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
	/* DMA1 Channel2 enable */
	DMA_Cmd(DMA1_Channel2, ENABLE);

	USART_DMACmd(USART1, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);

//USART2 configuration
	/* DMA1 Channel5 Config: Uart2 Rx data */
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->RDR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)uart2Rx.buffer1;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = UART_RX_BUFFER_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	/* DMA1 Channel5 enable */
	DMA_Cmd(DMA1_Channel5, ENABLE);
	uart2Flags.rxOnlineBuffer = UART_RX_BUFFER1_WAITING;

	/* DMA1 Channel4 Config: Uart2 Tx data */
	uart2Flags.txBusy = UART_TX_BUSY;
	DMA_DeInit(DMA1_Channel4);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->TDR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)uart2Tx.buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = uart2Tx.length;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
	/* DMA1 Channel4 enable */
	DMA_Cmd(DMA1_Channel4, ENABLE);

	USART_DMACmd(USART2, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);
}

/**
*******************************************************************************
* @brief       uart1 transmitter using dma
* @param[in]   *ptr: pointer of data to be transmitted
* @param[in]   length: length of data
* @details    none.
*******************************************************************************
*/
void uart1TxCmd(uint8_t *ptr, uint8_t length){
	//wait while bus is not free
	START_UART1_TIMER;
	while (uart1Flags.txBusy == UART_TX_BUSY){
		if (CHECK_UART1_TIMER_REACH_TO(1000))
			break;
		osDelay(2);
	}

	uart1Flags.txBusy = UART_TX_BUSY;
	DMA_Cmd(DMA1_Channel2, DISABLE); //disable uart1 tx dma
	if (DMA_GetFlagStatus(DMA1_FLAG_TC2)) {
		DMA_ClearFlag(DMA1_FLAG_TC2);
		USART_ClearFlag(USART1, USART_FLAG_TC);
	}
	DMA1_Channel2->CMAR = (uint32_t)ptr;

	DMA1_Channel2->CNDTR = length;

	DMA_Cmd(DMA1_Channel2, ENABLE);
}
/**
*******************************************************************************
* @brief       uart2 transmitter using dma
* @param[in]   *ptr: pointer of data to be transmitted
* @param[in]   length: length of data
* @details    none.
*******************************************************************************
*/
void uart2TxCmd(uint8_t *ptr, uint8_t length) {
	//wait while bus is not free
	START_UART2_TIMER;
	while (uart2Flags.txBusy == UART_TX_BUSY){
		if (CHECK_UART2_TIMER_REACH_TO(1000))
			break;
		osDelay(2);
	}

	uart2Flags.txBusy = UART_TX_BUSY;
	DMA_Cmd(DMA1_Channel4, DISABLE); //disable uart2 tx dma
	if (DMA_GetFlagStatus(DMA1_FLAG_TC4)) {
		DMA_ClearFlag(DMA1_FLAG_TC4);
		USART_ClearFlag(USART2, USART_FLAG_TC);
	}
	DMA1_Channel4->CMAR = (uint32_t)ptr;

	DMA1_Channel4->CNDTR = length;

	DMA_Cmd(DMA1_Channel4, ENABLE);
}

/**
*******************************************************************************
* @brief       uart1 check rx buffers
* @param[in]   
* @param[out]   
* @details    none.
*******************************************************************************
*/
uint8_t uart1CheckRxBuf(void){
	uint8_t ucLength = UART_RX_BUFFER_IS_EMPTY;
	
	if (uart1Rx.length1){
		ucLength = uart1Rx.length1;
		uart1Rx.length1 = 0x00;
		uart1Rx.ptrBuffer = uart1Rx.buffer1;
	}
	else if (uart1Rx.length2){
		ucLength = uart1Rx.length2;
		uart1Rx.length2 = 0x00;
		uart1Rx.ptrBuffer = uart1Rx.buffer2;
	}
	return ucLength;
}

/**
*******************************************************************************
* @brief       uart2 check rx buffers
* @param[in]   
* @param[out]   
* @details    none.
*******************************************************************************
*/
uint8_t uart2CheckRxBuf(void){
	uint8_t ucLength = UART_RX_BUFFER_IS_EMPTY;
	
	if (uart2Rx.length1){
		ucLength = uart2Rx.length1;
		uart2Rx.length1 = 0x00;
		uart2Rx.ptrBuffer = uart2Rx.buffer1;
	}
	else if (uart2Rx.length2){
		ucLength = uart2Rx.length2;
		uart2Rx.length2 = 0x00;
		uart2Rx.ptrBuffer = uart2Rx.buffer2;
	}
	return ucLength;
}

/**
*******************************************************************************
* @brief       uart1 task code
* @param[in]   None
* @param[out]  None
* @retval      None
* @par Description
* @details    none.
*******************************************************************************
*/
__NO_RETURN void task_Uart1(void* pdata) {

	while (1) {
		#if defined(__CC_ARM)
		osEventFlagsWait(event_General, EVENT_MASK_UART1_TIMEOUT, osFlagsWaitAny, osWaitForever);
		#elif defined(__GNUC__)
		CoWaitForSingleFlag(flag_UartTimeout, 0); //no time-out
		#endif
		switch (sys_par.uart1_protocol) {
			case PROTOCOL_MODBUS:
				uart1Tx.length = 0x00; //clear transmit data
				mbTxRxData.slaveAdd = sys_par.uart1_address;

				if (uart1Rx.length1) {
					mbTxRxData.ptrRxData = uart1Rx.buffer1;
					mbTxRxData.rxLength = uart1Rx.length1;
					mbTxRxData.ptrTxData = uart1Tx.buffer;
					modbusRTU();
					uart1Tx.length = mbTxRxData.txLength;
					uart1Rx.length1 = 0;
				}
				else if (uart1Rx.length2) {
					mbTxRxData.ptrRxData = uart1Rx.buffer2;
					mbTxRxData.rxLength = uart1Rx.length2;
					mbTxRxData.ptrTxData = uart1Tx.buffer;
					modbusRTU();
					uart1Tx.length = mbTxRxData.txLength;
					uart1Rx.length2 = 0;
				}

				if (uart1Tx.length) {
					PLT_FREE_OS_DELAY(5);
					uart1TxCmd(uart1Tx.buffer, uart1Tx.length);
				}

				break;
			case PROTOCOL_DSI:
				break;
			case PROTOCOL_MOBILE_APP:
				break;
			default:
				break;
		}
		
		//PLT_FREE_OS_DELAY(5);
	}
}

/**
*******************************************************************************
* @brief       uart2 task code
* @param[in]   None
* @param[out]  None
* @retval      None
* @par Description
* @details    none.
*******************************************************************************
*/
__NO_RETURN void task_Uart2(void* pdata) {

	while (1) {
		#if defined(__CC_ARM)
		osEventFlagsWait(event_General, EVENT_MASK_UART2_TIMEOUT, osFlagsWaitAny, osWaitForever);
		#elif defined(__GNUC__)
		CoWaitForSingleFlag(flag_UartTimeout, 0); //no time-out
		#endif
		switch (sys_par.uart2_protocol) {
			case PROTOCOL_MODBUS:
				uart2Tx.length = 0x00; //clear transmit data
				mbTxRxData.slaveAdd = sys_par.uart2_address;

				if (uart2Rx.length1) {
					mbTxRxData.ptrRxData = uart2Rx.buffer1;
					mbTxRxData.rxLength = uart2Rx.length1;
					mbTxRxData.ptrTxData = uart2Tx.buffer;
					modbusRTU();
					uart2Tx.length = mbTxRxData.txLength;
					uart2Rx.length1 = 0;
				}
				else if (uart2Rx.length2) {
					mbTxRxData.ptrRxData = uart2Rx.buffer2;
					mbTxRxData.rxLength = uart2Rx.length2;
					mbTxRxData.ptrTxData = uart2Tx.buffer;
					modbusRTU();
					uart2Tx.length = mbTxRxData.txLength;
					uart2Rx.length2 = 0;
				}

				if (uart2Tx.length) {
					PLT_FREE_OS_DELAY(5);
					uart2TxCmd(uart2Tx.buffer, uart2Tx.length);
				}
				break;
			case PROTOCOL_DSI:
				break;
			case PROTOCOL_MOBILE_APP:
				mobile_app_interface(CHANNEL_UART2);
				break;
			default:
				break;
		}
	}
}

/* * * END OF FILE * * */

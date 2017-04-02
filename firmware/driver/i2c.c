/******************************************************************************
*	Written by Yilmaz Eyidogan
*	I2C functions
*	Created date: 2016.12.02
*******************************************************************************/
#include "i2c.h"

/* typedef -----------------------------------------------------------*/
/* define ------------------------------------------------------------*/

/* macro -------------------------------------------------------------*/
/* variables ---------------------------------------------------------*/
uint8_t i2c_tx_buf[I2C_TX_BUF_SIZE] = {0x00};
uint8_t i2c_rx_buf[I2C_RX_BUF_SIZE] = {0x00};
uint8_t * ptr_i2c_tx;
uint8_t * ptr_i2c_rx;
I2C_MSG_TX_TYPE i2c_msg_tx_s = {0x00};
I2C_STATUS_TYPE i2c_status_s = {0x00};
uint8_t i2c_master_send_start_stop = 0x00;

uint32_t ulTempI2C = 0x00;
uint16_t uiTimerI2C = 0x00; //to control loops

/**
  * @brief I2C1 interrupt handler
  * @param  None
  * @retval None
  */
void I2C1_IRQHandler(void){
	ulTempI2C = I2C1->ISR;
	//Transfer direction (slave mode) 0: wr, 1: rd
	//I2C1->ISR & I2C_ISR_DIR
	if (ulTempI2C & I2C_ISR_ADDR){ //address matched in slave mode
		I2C1->ICR = I2C_ICR_ADDRCF; //clear flag
		ptr_i2c_rx = i2c_rx_buf;
		ptr_i2c_tx = i2c_tx_buf;
		return;
	}
	if (ulTempI2C & I2C_ISR_STOPF){ //stop detected
		I2C1->ICR = I2C_ICR_STOPCF; //clear flag
		i2c_status_s.bits.completed_flag = true;
		return;
	}
	if (ulTempI2C & I2C_ISR_TC){ //transfer completed in master mode
		/*This flag is set by hardware when RELOAD=0, AUTOEND=0 
		and NBYTES data have been transferred. 
		It is cleared by software when START bit or STOP bit is set.*/
		if (i2c_master_send_start_stop > I2C_MASTER_SEND_STOP){
			ulTempI2C = (uint32_t)(((uint32_t)i2c_msg_tx_s.chip_add << 1) & I2C_CR2_SADD);
			ulTempI2C |= I2C_CR2_RD_WRN; //read
			ptr_i2c_rx = i2c_rx_buf;
			ulTempI2C |= (uint32_t)(((uint32_t)i2c_msg_tx_s.length_rd << 16 ) & I2C_CR2_NBYTES);
			I2C1->CR2 = ulTempI2C;
			/* Generate a START condition */
			I2C1->CR2 |= I2C_CR2_START;
			--i2c_master_send_start_stop;
		}
		else{
			/* Generate a STOP condition */
			I2C1->CR2 |= I2C_CR2_STOP;
			if (i2c_status_s.bits.nack_flag == false)
				i2c_status_s.bits.completed_flag = true;
			osEventFlagsSet(event_general, EVENT_MASK_I2C1_TRANSFER_COMPLETED);
		}
		return;
	}
	if (ulTempI2C & I2C_ISR_TXIS){ //Transmit interrupt status
		/* Write in the DR register the data to be sent */
		I2C1->TXDR = (uint8_t)*ptr_i2c_tx;
		++ptr_i2c_tx;
		return;
	}
	if (ulTempI2C & I2C_ISR_TXE){ //Transmit data register empty
		/* Write in the DR register the data to be sent */
		I2C1->TXDR = (uint8_t)*ptr_i2c_tx;
		++ptr_i2c_tx;
		return;
	}
	if (ulTempI2C & I2C_ISR_RXNE){ //Receive data register not empty
		/* Return the data in the DR register */
		*ptr_i2c_rx = (uint8_t)I2C1->RXDR;
		++ptr_i2c_rx;
	}
	if (ulTempI2C & I2C_ISR_NACKF){ //NACK received flag
		I2C1->ICR = I2C_ICR_NACKCF; //clear flag
		i2c_status_s.bits.nack_flag = true;
	}
	
	//Error interrupts
	if (ulTempI2C & I2C_ISR_OVR){ //Overrun/Underrun in slave mode
		I2C1->ICR = I2C_ICR_OVRCF; //clear flag
		i2c_status_s.bits.over_run_flag = true;
	}
	if (ulTempI2C & I2C_ISR_ARLO){ //Arbitration lost
		I2C1->ICR = I2C_ICR_ARLOCF; //clear flag
		i2c_status_s.bits.arblost_flag = true;
	}
	if (ulTempI2C & I2C_ISR_BERR){ //Bus error
		I2C1->ICR = I2C_ICR_BERRCF; //clear flag
		i2c_status_s.bits.bus_error_flag = true;
	}
}

/**
  * @brief init i2c
  * @param  None
  * @retval None
  */
void init_i2c(void){
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; //Enable I2C1 clock
	/* Enable I2C1 reset state */
	RCC->APB1RSTR |= RCC_APB1ENR_I2C1EN;
	/* Release I2C1 from reset state */
	RCC->APB1RSTR &= ~RCC_APB1ENR_I2C1EN;
	
	/* Disable I2Cx Peripheral */
	I2C1->CR1 &= (uint32_t)~((uint32_t)I2C_CR1_PE);
	//I2C1->TIMINGR = 0x00200E1C;
	//I2C1->TIMINGR = 0x10200E1C;
	I2C1->TIMINGR = 0x20100A10;
	/* Set analog and digital filters*/
	I2C1->CR1 = 0x080FFE;
	//bit23:0 PEC calculation disabled
	//bit22:0 SMBUS alert
	//bit21:0 SMBUS device address
	//bit20:0 SMBUS host address enabled
	//bit19:1 General call enabled
	//bit18:0 reserved
	//bit17:0 Clock stretch enabled
	//bit16:0 Slave byte control disabled
	//bit15:0 RXDMA disabled
	//bit14:0 TXDMA disabled
	//bit13:0 reserved
	//bit12:0 Analog filter enabled
	//bit11-8:1111 Digital filter
	//bit7:1 Error interrupt enabled
	//bit6:1 Transfer complete interrupt enabled
	//bit5:1 Stop detection interrupt enabled
	//bit4:1 NACK received interrupt enabled
	//bit3:1 Address match interrupt enabled
	//bit2:1 RX interrupt enabled
	//bit1:1 TX interrupt enabled
	//bit0:0 Peripheral disabled
	
	I2C1->OAR1 = 0x00;
	I2C1->OAR1 = (uint32_t)I2C_OWN_ADDR << 1;
	I2C1->OAR1 |= (uint32_t)0x8000; //own address 1 enabled
	
	/* Enable I2Cx Peripheral */
	I2C1->CR1 |= I2C_CR1_PE;
	
	NVIC_EnableIRQ(I2C1_IRQn); //defined in core_cm0.h
	NVIC_SetPriority(I2C1_IRQn,0);
}

/**
  * @brief i2c master transmit
  * @param  None
  * @retval None
  */
uint8_t i2c_master_process(uint8_t rw){
	/* Disable I2Cx Peripheral */
	I2C1->CR1 &= ~I2C_CR1_PE;
	osDelay(2);
	/* Enable I2Cx Peripheral */
	I2C1->CR1 |= I2C_CR1_PE;
	osDelay(2);
	
	if (i2c_master_send_start_stop == 0x00 || i2c_master_send_start_stop > I2C_MASTER_SEND_MAX)
		return false;
	i2c_status_s.byte = (uint8_t)0x00;
	ptr_i2c_tx = i2c_tx_buf;
	ulTempI2C = (uint32_t)(((uint32_t)i2c_msg_tx_s.chip_add << 1) & I2C_CR2_SADD);
	if (rw == I2C_MASTER_READ){
		ulTempI2C |= I2C_CR2_RD_WRN;
		ptr_i2c_rx = i2c_rx_buf;
		if (i2c_msg_tx_s.length_rd == 0x00)
			return false;
		ulTempI2C |= (uint32_t)(((uint32_t)i2c_msg_tx_s.length_rd << 16 ) & I2C_CR2_NBYTES);
	}
	else{ //write
		if (i2c_msg_tx_s.length_wr == 0x00)
			return false;
		ulTempI2C |= (uint32_t)(((uint32_t)i2c_msg_tx_s.length_wr << 16 ) & I2C_CR2_NBYTES);
	}

	I2C1->CR2 = ulTempI2C;
	
	//wait while bus is not free
	START_I2C_TIMER;
	while (I2C1->ISR & I2C_ISR_BUSY){
		osDelay(2);
		if (CHECK_I2C_TIMER_REACH_TO(150))
			return false;
	}
	/* Generate a START condition */
	I2C1->CR2 |= I2C_CR2_START;
	--i2c_master_send_start_stop;
	
	return true;
}
/* * * END OF FILE * * */

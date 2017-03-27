/******************************************************************************
*	Written by Yilmaz Eyidogan
*	HC-06 bluetooth application functions
*	Created date: 2017.01.29
*******************************************************************************/
#include "hc0x_bluetooth.h"
/* Private typedef */
/* Private define */
/* Private macro */
/* Private variables */

/**
*******************************************************************************
* @brief change device name of bluetooth module hc-0x
* @param[in] pointer of string
* @param[out] application status true or false
*******************************************************************************
*/
uint8_t change_device_name(uint8_t * ptrData){
	
	uart1Tx.length = copy_string(ptrData, uart1Tx.buffer, 0x14);
	uart1TxCmd(uart1Tx.buffer, uart1Tx.length);
	osDelay(100);
	
	if (uart1CheckRxBuf() == UART_RX_BUFFER_IS_EMPTY)
		return false;
	
	return compare_string((uint8_t *)"OK", uart1Rx.ptrBuffer, 0x14);
}

/**
*******************************************************************************
* @brief 
* @param[in] 
* @param[out] 
*******************************************************************************
*/
#define UARTx_TX_CMD(x) if(x==CHANNEL_UART1) \
													uart1TxCmd(ptrTx, x); \
												else \
													uart2TxCmd(ptrTx, x);
												
void mobile_app_interface(uint8_t ucChannel){
	uint8_t ucLength, ucReturn = MOBILE_APP_RETURN_ERR;
	uint8_t *ptrRx, *ptrTx;
	
	ucLength = uart1CheckRxBuf();
	if (ucLength == UART_RX_BUFFER_IS_EMPTY)
		return;
	
	if (ucChannel == CHANNEL_UART1){
		ptrRx = uart1Rx.ptrBuffer;
		ptrTx = uart1Tx.buffer;
	}
	else{
		ptrRx = uart2Rx.ptrBuffer;
		ptrTx = uart2Tx.buffer;
	}
	//read status msg
	if (compare_string((uint8_t *)"RS\r\n", ptrRx, 0x14)){
		if (ucLength == 0x04){
			ucReturn = MOBILE_APP_RETURN_MSG;
			if (stHDC1080Status.ok == true){
				sprintf((char *)ptrTx, "Temperature: %d\r\n", stTempHum.uiTemperature);
				ucLength = count_string(ptrTx, UART_TX_BUFFER_SIZE);
				UARTx_TX_CMD(ucLength);
				sprintf((char *)ptrTx, "Humidity: %d\r\n", stTempHum.uiHumidity);
				ucLength = count_string(ptrTx, UART_TX_BUFFER_SIZE);
				UARTx_TX_CMD(ucLength);
			}
			else{
				sprintf((char *)ptrTx, "Temperature sensor error\r\n");
				ucLength = count_string(ptrTx, UART_TX_BUFFER_SIZE);
				UARTx_TX_CMD(ucLength);
			}
			
		}
	}
	
	//read report msg
	if (compare_string((uint8_t *)"RR\r\n", uart1Rx.ptrBuffer, 0x14)){
		if (ucLength == 0x04){
			ucReturn = MOBILE_APP_RETURN_MSG;
			sprintf((char *)ptrTx, "Daily Report:\r\n");
			ucLength = count_string(ptrTx, UART_TX_BUFFER_SIZE);
			UARTx_TX_CMD(ucLength);
			sprintf((char *)ptrTx, "Under construction\r\n");
			ucLength = count_string(ptrTx, UART_TX_BUFFER_SIZE);
			UARTx_TX_CMD(ucLength);
		}
	}

	//open gate 1 (cat litter hopper)
	if (compare_string((uint8_t *)"COG1\r\n", uart1Rx.ptrBuffer, 0x14)){
		if (ucLength == 0x06){
			motor_s[MOTOR_A].cmd = STEPPER_TO_END_POINT;
			ucReturn = MOBILE_APP_RETURN_OK;
			osEventFlagsSet(event_general, EVENT_MASK_STEPPER_RUN);
		}
	}

	//close gate 1 (cat litter hopper)
	if (compare_string((uint8_t *)"CCG1\r\n", uart1Rx.ptrBuffer, 0x14)){
		if (ucLength == 0x06){
			motor_s[MOTOR_A].cmd = STEPPER_TO_ZERO_POINT;
			ucReturn = MOBILE_APP_RETURN_OK;
			osEventFlagsSet(event_general, EVENT_MASK_STEPPER_RUN);
		}
	}

	//open gate 2 (cat litter hopper)
	if (compare_string((uint8_t *)"COG2\r\n", uart1Rx.ptrBuffer, 0x14)){
		if (ucLength == 0x06){
			motor_s[MOTOR_B].cmd = STEPPER_TO_END_POINT;
			ucReturn = MOBILE_APP_RETURN_OK;
			osEventFlagsSet(event_general, EVENT_MASK_STEPPER_RUN);
		}
	}

	//close gate 2 (cat litter hopper)
	if (compare_string((uint8_t *)"CCG2\r\n", uart1Rx.ptrBuffer, 0x14)){
		if (ucLength == 0x06){
			motor_s[MOTOR_B].cmd = STEPPER_TO_ZERO_POINT;
			ucReturn = MOBILE_APP_RETURN_OK;
			osEventFlagsSet(event_general, EVENT_MASK_STEPPER_RUN);
		}
	}
	
	if (ucReturn == MOBILE_APP_RETURN_OK){
		sprintf((char *)ptrTx, "OK\r\n");
		UARTx_TX_CMD(0x04);
	}
	else if (ucReturn == MOBILE_APP_RETURN_ERR){
		sprintf((char *)ptrTx, "ERR\r\n");
		UARTx_TX_CMD(0x05);
	}
}
/* * * END OF FILE * * */

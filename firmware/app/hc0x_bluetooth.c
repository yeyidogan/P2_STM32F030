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
	
	uart1Tx.length = copy_string((uint8_t *)"AT+NAME", uart1Tx.buffer, 0x14);
	uart1Tx.length += copy_string(ptrData, uart1Tx.buffer+uart1Tx.length, 0x14);
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
void mobile_app_interface(void){
	uint8_t ucLength, ucReturn = MOBILE_APP_RETURN_ERR;
	uint8_t *ptrRx, *ptrTx;
	
	ucLength = uart1CheckRxBuf();
	if (ucLength == UART_RX_BUFFER_IS_EMPTY)
		return;
	
	ptrRx = uart1Rx.ptrBuffer;
	ptrTx = uart1Tx.buffer;
	//read status msg
	if (compare_string((uint8_t *)"RS\r\n", ptrRx, 0x14)){
		if (ucLength == 0x04){
			ucReturn = MOBILE_APP_RETURN_MSG;
			if (stHDC1080Status.ok == true){
				osEventFlagsWait(event_general, EVENT_MASK_UART1_TX_TRANSFERED, osFlagsWaitAny, 1000);
				sprintf((char *)ptrTx, "Temperature: %d\r\n", stTempHum.uiTemperature);
				ucLength = count_string(ptrTx, UART_TX_BUFFER_SIZE);
				uart1TxCmd(ptrTx, ucLength);
				osEventFlagsWait(event_general, EVENT_MASK_UART1_TX_TRANSFERED, osFlagsWaitAny, 1000);
				sprintf((char *)ptrTx, "Humidity: %d\r\n", stTempHum.uiHumidity);
				ucLength = count_string(ptrTx, UART_TX_BUFFER_SIZE);
				uart1TxCmd(ptrTx, ucLength);
			}
			else{
				osEventFlagsWait(event_general, EVENT_MASK_UART1_TX_TRANSFERED, osFlagsWaitAny, 1000);
				sprintf((char *)ptrTx, "Temperature sensor error\r\n");
				ucLength = count_string(ptrTx, UART_TX_BUFFER_SIZE);
				uart1TxCmd(ptrTx, ucLength);
			}
			
		}
	}
	
	//read report msg
	if (compare_string((uint8_t *)"RR\r\n", uart1Rx.ptrBuffer, 0x14)){
		if (ucLength == 0x04){
			ucReturn = MOBILE_APP_RETURN_MSG;
			osEventFlagsWait(event_general, EVENT_MASK_UART1_TX_TRANSFERED, osFlagsWaitAny, 1000);
			sprintf((char *)ptrTx, "Daily Report:\r\n");
			ucLength = count_string(ptrTx, UART_TX_BUFFER_SIZE);
			uart1TxCmd(ptrTx, ucLength);
			osEventFlagsWait(event_general, EVENT_MASK_UART1_TX_TRANSFERED, osFlagsWaitAny, 1000);
			sprintf((char *)ptrTx, "Under construction\r\n");
			ucLength = count_string(ptrTx, UART_TX_BUFFER_SIZE);
			uart1TxCmd(ptrTx, ucLength);
		}
	}

	//open gate 1 (cat hole gate)
	if (compare_string((uint8_t *)"COG1\r\n", uart1Rx.ptrBuffer, 0x14)){
		if (ucLength == 0x06){
			motor_s[MOTOR_HOLE_GATE].cmd = STEPPER_TO_END_POINT;
			ucReturn = MOBILE_APP_RETURN_OK;
			osEventFlagsSet(event_general, EVENT_MASK_STEPPER_RUN);
		}
	}

	//close gate 1 (cat litter hopper)
	if (compare_string((uint8_t *)"CCG1\r\n", uart1Rx.ptrBuffer, 0x14)){
		if (ucLength == 0x06){
			motor_s[MOTOR_HOLE_GATE].cmd = STEPPER_TO_ZERO_POINT;
			ucReturn = MOBILE_APP_RETURN_OK;
			osEventFlagsSet(event_general, EVENT_MASK_STEPPER_RUN);
		}
	}

	//open gate 2 (cat litter hopper)
	if (compare_string((uint8_t *)"COG2\r\n", uart1Rx.ptrBuffer, 0x14)){
		if (ucLength == 0x06){
			motor_s[MOTOR_LITTER_GATE].cmd = STEPPER_TO_END_POINT;
			ucReturn = MOBILE_APP_RETURN_OK;
			osEventFlagsSet(event_general, EVENT_MASK_STEPPER_RUN);
		}
	}

	//close gate 2 (cat litter hopper)
	if (compare_string((uint8_t *)"CCG2\r\n", uart1Rx.ptrBuffer, 0x14)){
		if (ucLength == 0x06){
			motor_s[MOTOR_LITTER_GATE].cmd = STEPPER_TO_ZERO_POINT;
			ucReturn = MOBILE_APP_RETURN_OK;
			osEventFlagsSet(event_general, EVENT_MASK_STEPPER_RUN);
		}
	}
	
	if (compare_string((uint8_t *)"RST\r\n", ptrRx, 0x05)){
		NVIC_SystemReset();
	}
	
	if (ucReturn == MOBILE_APP_RETURN_OK){
		osEventFlagsWait(event_general, EVENT_MASK_UART1_TX_TRANSFERED, osFlagsWaitAny, 1000);
		sprintf((char *)ptrTx, "OK\r\n");
		uart1TxCmd(ptrTx, 0x04);
	}
	else if (ucReturn == MOBILE_APP_RETURN_ERR){
		osEventFlagsWait(event_general, EVENT_MASK_UART1_TX_TRANSFERED, osFlagsWaitAny, 1000);
		sprintf((char *)ptrTx, "ERR\r\n");
		uart1TxCmd(ptrTx, 0x05);
	}
}
/* * * END OF FILE * * */

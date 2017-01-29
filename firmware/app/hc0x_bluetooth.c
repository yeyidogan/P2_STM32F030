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
	PLT_FREE_OS_DELAY(100);
	
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
	
	ucLength = uart1CheckRxBuf();
	if (ucLength == UART_RX_BUFFER_IS_EMPTY)
		return;
	
	//read status msg
	if (compare_string((uint8_t *)"RS\r\n", uart1Rx.ptrBuffer, 0x14)){
		if (ucLength == 0x04){
			ucReturn = MOBILE_APP_RETURN_MSG;
			if (stHDC1080Status.ok == true){
				sprintf((char *)uart1Tx.buffer, "Temperature: %d\r\n", stTempHum.uiTemperature);
				ucLength = count_string(uart1Tx.buffer, UART_TX_BUFFER_SIZE);
				uart1TxCmd(uart1Tx.buffer, ucLength);
				sprintf((char *)uart1Tx.buffer, "Humidity: %d\r\n", stTempHum.uiHumidity);
				ucLength = count_string(uart1Tx.buffer, UART_TX_BUFFER_SIZE);
				uart1TxCmd(uart1Tx.buffer, ucLength);
			}
			else{
				sprintf((char *)uart1Tx.buffer, "Temperature sensor error\r\n");
				ucLength = count_string(uart1Tx.buffer, UART_TX_BUFFER_SIZE);
				uart1TxCmd(uart1Tx.buffer, ucLength);
			}
			
		}
	}
	
	//read report msg
	if (compare_string((uint8_t *)"RR\r\n", uart1Rx.ptrBuffer, 0x14)){
		if (ucLength == 0x04){
			ucReturn = MOBILE_APP_RETURN_MSG;
			sprintf((char *)uart1Tx.buffer, "Daily Report:\r\n");
			ucLength = count_string(uart1Tx.buffer, UART_TX_BUFFER_SIZE);
			uart1TxCmd(uart1Tx.buffer, ucLength);
			sprintf((char *)uart1Tx.buffer, "Under construction\r\n");
			ucLength = count_string(uart1Tx.buffer, UART_TX_BUFFER_SIZE);
			uart1TxCmd(uart1Tx.buffer, ucLength);
		}
	}

	//open gate 1 (cat litter hopper)
	if (compare_string((uint8_t *)"COG1\r\n", uart1Rx.ptrBuffer, 0x14)){
		if (ucLength == 0x06){
			stStepperMotorControl.ucCmdForA = STEPPER_TO_END_POINT;
			ucReturn = MOBILE_APP_RETURN_OK;
		}
	}

	//close gate 1 (cat litter hopper)
	if (compare_string((uint8_t *)"CCG1\r\n", uart1Rx.ptrBuffer, 0x14)){
		if (ucLength == 0x06){
			stStepperMotorControl.ucCmdForA = STEPPER_TO_ZERO_POINT;
			ucReturn = MOBILE_APP_RETURN_OK;
		}
	}

	//open gate 2 (cat litter hopper)
	if (compare_string((uint8_t *)"COG2\r\n", uart1Rx.ptrBuffer, 0x14)){
		if (ucLength == 0x06){
			stStepperMotorControl.ucCmdForB = STEPPER_TO_END_POINT;
			ucReturn = MOBILE_APP_RETURN_OK;
		}
	}

	//close gate 2 (cat litter hopper)
	if (compare_string((uint8_t *)"CCG2\r\n", uart1Rx.ptrBuffer, 0x14)){
		if (ucLength == 0x06){
			stStepperMotorControl.ucCmdForB = STEPPER_TO_ZERO_POINT;
			ucReturn = MOBILE_APP_RETURN_OK;
		}
	}
	
	if (ucReturn == MOBILE_APP_RETURN_OK){
		sprintf((char *)uart1Tx.buffer, "OK\r\n");
		uart1TxCmd(uart1Tx.buffer, 0x04);
	}
	else if (ucReturn == MOBILE_APP_RETURN_ERR){
		sprintf((char *)uart1Tx.buffer, "ERR\r\n");
		uart1TxCmd(uart1Tx.buffer, 0x05);
	}
}
/* * * END OF FILE * * */

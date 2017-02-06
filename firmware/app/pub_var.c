/******************************************************************************
*	Written by Yilmaz Eyidogan
*	public variables
*	Created date: 2015.01.05
*******************************************************************************/

#include "pub_var.h"

/*---------------------------- Variable Define ------------------------------*/
PARAMETER_TYPE sys_par;

/*---------------------------- Function Define ------------------------------*/
/**
 *******************************************************************************
 * @brief       variable initialization function
 * @param[in]   None
 * @param[out]  None
 * @retval      None
 * @par Description
 * @details    none.
 *******************************************************************************
 */
void setParameters(void) {
	sys_par.uart1_protocol = PROTOCOL_MODBUS;
	sys_par.uart1_address = 0x01;
	sys_par.uart1_baudrate = 9600;
	
	sys_par.uart2_protocol = PROTOCOL_MOBILE_APP;
	sys_par.uart2_address = 0x01;
	sys_par.uart2_baudrate = 9600;
}

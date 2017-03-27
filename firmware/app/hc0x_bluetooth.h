/******************************************************************************
*	Written by Yilmaz Eyidogan
*	HC-06 bluetooth application functions
*	Created date: 2017.01.29
*******************************************************************************/
#ifndef _HC0X_BLUETOOTH_H_
#define _HC0X_BLUETOOTH_H_

#include "cmsis_os2.h"
#include <stdint.h>
#include <stdbool.h>
#include "uart.h"
#include "pub_var.h"
#include "pub_s.h"
#include "util.h"
#include "hdc1080.h"
#include "stepper_motor.h"

/* Private macro */
enum {
	MOBILE_APP_RETURN_OK,
	MOBILE_APP_RETURN_ERR,
	MOBILE_APP_RETURN_MSG
};
/* Private variables */
/* Private functions */
extern uint8_t change_device_name(uint8_t * ptrData);
extern void mobile_app_interface(uint8_t ucChannel);


#endif
/* * * END OF FILE * * */

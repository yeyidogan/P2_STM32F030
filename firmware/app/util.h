/******************************************************************************
*	Written by Yilmaz Eyidogan
*	public structures
*	Created date: 2015.01.04
*******************************************************************************/
#ifndef __UTIL_H
#define __UTIL_H

#include <stdint.h>
#include <stdbool.h>
#include "pub_s.h"

/* Public typedef */
#define TRUE 0x01
#define FALSE 0x00
/* Public define */

/* Public macro */
/* Public variables */

/* Public functions */
extern uint16_t wordEndianer(uint16_t wordIn);
extern uint8_t copy_string(uint8_t * ptrSource, uint8_t * ptrTarget, uint8_t ucMaxByte);
extern uint8_t compare_string(uint8_t * ptrSource, uint8_t * ptrTarget, uint8_t ucMaxByte);
extern uint8_t count_string(uint8_t * ptrData, uint8_t ucMaxByte);

#endif
/* * * END OF FILE * * */

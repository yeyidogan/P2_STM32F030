/******************************************************************************
*	Written by Yilmaz Eyidogan
*	util functions
*	Created date: 2016.02.22
*******************************************************************************/
#include "util.h"

/**
 *******************************************************************************
 * @brief       endian changer
 * @param[in]   None
 * @retval      None
 * @par Description little endian to big endian
 *******************************************************************************
 */
uint16_t wordEndianer(uint16_t wordIn) {
	uint16_t tmp;

	tmp = wordIn;
	tmp <<= 8;
	wordIn >>= 8;
	tmp += wordIn;
	return tmp;
}

/**
 *******************************************************************************
 * @brief       string copier
 * @param[in]   ptr_source, ptr_destination
 * @retval      length of string
* @par Description: copy string to any buffer
 *******************************************************************************
 */
uint8_t copy_string(uint8_t * ptrSource, uint8_t * ptrTarget, uint8_t ucMaxByte){
	uint8_t ucCount = 0x00;
	while (ucMaxByte--){
		if (*ptrSource == 0x00){
			*ptrTarget = 0x00;
			break;
		}
		*ptrTarget = *ptrSource;
		++ptrTarget;
		++ptrSource;
		++ucCount;
	}
	return ucCount;
}

/**
 *******************************************************************************
 * @brief       string compare
 * @param[in]   ptr_source, ptr_destination
 * @retval      None
* @par Description: compare any string
 *******************************************************************************
 */
uint8_t compare_string(uint8_t * ptrSource, uint8_t * ptrTarget, uint8_t ucMaxByte){
	while (ucMaxByte--){
		if (*ptrSource != *ptrTarget)
			return false;
			
		if (*ptrSource == 0x00)
			return true;

		++ptrTarget;
		++ptrSource;
	}
	return true;
}

/**
 *******************************************************************************
 * @brief       string compare
 * @param[in]   ptr_source, ptr_destination
 * @retval      None
* @par Description: compare any string
 *******************************************************************************
 */
uint8_t count_string(uint8_t * ptrData, uint8_t ucMaxByte){
	uint8_t ucCnt = 0x00;
	
	while (ucMaxByte--){
		if (*ptrData == 0x00)
			break;
		++ucCnt;
		++ptrData;
	}
	return ucCnt;
}
/* * * END OF FILE * * */

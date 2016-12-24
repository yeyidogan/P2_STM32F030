/******************************************************************************
*	Written by Yilmaz Eyidogan
*	platform free operating system definitions
*	Created date: 2016.12.20
*******************************************************************************/
#ifndef __PLT_FREE_OS_DEF_H_
#define __PLT_FREE_OS_DEF_H_

#if defined(__CC_ARM)
	#define pltFreeOsDelay(x) osDelay(x)
#elif defined(__GNUC__)
	#define pltFreeOsDelay(x) CoTickDelay(x)
#endif		



#endif
/* * * END OF FILE * * */

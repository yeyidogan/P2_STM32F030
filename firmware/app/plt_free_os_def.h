/******************************************************************************
*	Written by Yilmaz Eyidogan
*	platform free operating system definitions
*	Created date: 2016.12.20
*******************************************************************************/
#ifndef __PLT_FREE_OS_DEF_H_
#define __PLT_FREE_OS_DEF_H_

#if defined(__CC_ARM)
	#define PLT_FREE_OS_DELAY(x) osDelay(x)
	#define PLT_FREE_OS_MUTEX_ID osMutexId_t
#elif defined(__GNUC__)
	#define PLT_FREE_OS_DELAY(x) CoTickDelay(x)
	#define PLT_FREE_OS_MUTEX_ID OS_MutexID
#endif		


#ifndef __NO_RETURN
#if   defined(__CC_ARM)
	#define __NO_RETURN __declspec(noreturn)
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
	#define __NO_RETURN __attribute__((noreturn))
#elif defined(__GNUC__)
	#define __NO_RETURN __attribute__((noreturn))
#elif defined(__ICCARM__)
	#define __NO_RETURN __noreturn
#else
	#define __NO_RETURN
#endif
#endif


#endif
/* * * END OF FILE * * */

#ifndef __NUT_LOGGING_H
#define __NUT_LOGGING_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_Logging.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include <stdio.h>
#include <stdarg.h>
#include "../Core/NLimits.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _NUT_LOGGING
#define NUT_InitLogging(setup_flags,plogfilename)	
#define NUT_Logging(a)						
#endif

#ifdef _NUT_LOGGING
#define CONSTANT_NUT_LOGGING_BUFFER_SIZE		256
#define	CONSTANT_NUT_LOGGING_PATH_SIZE_MAX		128
#define CONSTANT_NUT_LOGGING_FILENAME_SIZE_MAX	64

#define CONSTANT_NUT_LOGGING_OVERIDE_CHANNEL	NU8_MAX
void NUT_EnableLoggingChannel(Nu8 c, Nbool b);
void NUT_InitLogging(const Nu32 setup_flags, const Nchar* plogfilename);
void NUT_Logging(const Nu8 channel, const Nchar* format, ...);
#endif

#ifndef _NUT_LOGGING
#define NUT_EnableLoggingChannel(c,b)
#define NUT_InitLogging(setup_flags,plogfilename)	
#define NUT_Logging(a)								
#endif


// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NUT_LOGGING_H


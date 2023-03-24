#ifndef __NUT_CORE_H
#define __NUT_CORE_H

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_Core.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NFlags.h"

// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************
// **																																							**
// **																			FUNCTIONS																		**
// **																																							**
// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************
#ifdef _NUT_LOGGING
#define FLAG_NUT_CORE_LOGGING_STREAM			BIT_0
#define FLAG_NUT_CORE_LOGGING_STREAM_FILEOK		BIT_1
#define FLAG_NUT_CORE_LOGGING_STDOUT			BIT_2

#define MASK_NUT_CORE_LOGGING					(BIT_0|BIT_1|BIT_2)
#define MASK_NUT_CORE_LOGGING_STREAM			(BIT_0|BIT_1)
#endif

void NUT_Init();
void NUT_Disable();

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NUT_CORE_H 


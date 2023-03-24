#ifndef __NELAPSED_TIMER_H
#define __NELAPSED_TIMER_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NElapsedTimer.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NType.h"
#include "NTimer.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **						ElapsedTimer Structures/Define & flags						**
// ***************************************************************************************
typedef Nu32					NELAPSED_TIMER_RESULT;
typedef struct					NELAPSED_TIMER		NELAPSED_TIMER;
typedef NELAPSED_TIMER_RESULT	(*NELAPSED_TIMER_HANDLE)(Nu32 user32);

#define NELAPSED_TIMER_RESULT_STOP_AND_DELETE		0	// for EndHandle Only
#define NELAPSED_TIMER_RESULT_RESTART				1	// for EndHandle Only

struct  NELAPSED_TIMER
{
	NTIMER					Timer;
	NELAPSED_TIMER_HANDLE	EndHandle;
	NELAPSED_TIMER_HANDLE	TimeHandle;
	Nf32					ElapsedTime;
	Nf32					TimeLimit;
	Nu32					User_32;
};

// ***************************************************************************************
// **						 ElapsedTimer Functions										**
// ***************************************************************************************
NELAPSED_TIMER*	NSetupElapsedTimer(NELAPSED_TIMER *pelapstimer,const NELAPSED_TIMER_HANDLE ptimehandle,const NELAPSED_TIMER_HANDLE pendhandle, const Nf32 time_limit, const Nu32 user_32);
NELAPSED_TIMER*	NCreateElapsedTimer( const NELAPSED_TIMER_HANDLE ptimehandle,const NELAPSED_TIMER_HANDLE pendhandle, const Nf32 time_limit, const Nu32 user_32);
void			NClearElapsedTimer(NELAPSED_TIMER *pelapstimer);
void			NDeleteElapsedTimer(NELAPSED_TIMER *pelapstimer);

// LOW LEVEL
void			NBasicClearElapsedTimer(NELAPSED_TIMER *pelapstimer);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NELAPSED_TIMER_H 


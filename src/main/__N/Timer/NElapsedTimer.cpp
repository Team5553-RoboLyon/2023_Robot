// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NTimer.cpp										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NErrorHandling.h"
#include "NElapsedTimer.h"

static void NElapsedTimerA_TimerHandle(NTIMER *ptimer, const NTIME *ptime)
{
	((NELAPSED_TIMER*)ptimer)->ElapsedTime += ptime->Nf32_Delay;
	if( ((NELAPSED_TIMER*)ptimer)->ElapsedTime >= ((NELAPSED_TIMER*)ptimer)->TimeLimit )
	{
		if( ((NELAPSED_TIMER*)ptimer)->EndHandle(((NELAPSED_TIMER*)ptimer)->User_32) == NELAPSED_TIMER_RESULT_STOP_AND_DELETE )
		{
			NDeleteElapsedTimer((NELAPSED_TIMER*)ptimer);
		}
		else // NELAPSED_TIMER_RESULT_RESTART
		{
			((NELAPSED_TIMER*)ptimer)->ElapsedTime-=((NELAPSED_TIMER*)ptimer)->TimeLimit; // Reset timer in the right way ( here it's much more better than 'ptimer->ElapsedTime = 0';)
		}
	}
	else
	{
		((NELAPSED_TIMER*)ptimer)->TimeHandle(((NELAPSED_TIMER*)ptimer)->User_32);
	}
}

static void NElapsedTimerB_TimerHandle(NTIMER *ptimer, const NTIME *ptime)
{
	((NELAPSED_TIMER*)ptimer)->ElapsedTime += ptime->Nf32_Delay;
	if( ((NELAPSED_TIMER*)ptimer)->ElapsedTime >= ((NELAPSED_TIMER*)ptimer)->TimeLimit )
	{
		if( ((NELAPSED_TIMER*)ptimer)->EndHandle(((NELAPSED_TIMER*)ptimer)->User_32) == NELAPSED_TIMER_RESULT_STOP_AND_DELETE )
		{
			NDeleteElapsedTimer((NELAPSED_TIMER*)ptimer);
		}
		else // NELAPSED_TIMER_RESULT_RESTART
		{
			((NELAPSED_TIMER*)ptimer)->ElapsedTime-=((NELAPSED_TIMER*)ptimer)->TimeLimit; // Reset timer in the right way ( here it's much more better than 'ptimer->ElapsedTime = 0';)
		}
	}
}

NELAPSED_TIMER*	NSetupElapsedTimer(NELAPSED_TIMER *pelapstimer,const NELAPSED_TIMER_HANDLE ptimehandle,const NELAPSED_TIMER_HANDLE pendhandle, const Nf32 time_limit, const Nu32 user_32)
{
	NErrorIf(!pendhandle, NERROR_NULL_POINTER); // pendhandle must be defined

	Nmem0(pelapstimer,NELAPSED_TIMER);
	if(ptimehandle)
		NSetupTimer((NTIMER*)pelapstimer,NElapsedTimerA_TimerHandle);
	else
		NSetupTimer((NTIMER*)pelapstimer,NElapsedTimerB_TimerHandle);

	pelapstimer->Timer.ID			= NTIMER_ID_ELAPSED_TIMER;		
	// pelapstimer->ElapsedTime		= 0.0f; useless due to Nmem0
	pelapstimer->TimeLimit			= time_limit;
	pelapstimer->EndHandle			= pendhandle;
	pelapstimer->TimeHandle			= ptimehandle;
	pelapstimer->User_32			= user_32;

	return pelapstimer;
}

NELAPSED_TIMER*	NCreateElapsedTimer(const NELAPSED_TIMER_HANDLE ptimehandle,const NELAPSED_TIMER_HANDLE pendhandle, const Nf32 time_limit, const Nu32 user_32)
{
	return NSetupElapsedTimer(NEW(NELAPSED_TIMER),ptimehandle,pendhandle,time_limit,user_32);
}

void NClearElapsedTimer(NELAPSED_TIMER *pelapstimer)
{
	NErrorIf(pelapstimer->Timer.ID != NTIMER_ID_ELAPSED_TIMER, NERROR_TIMER_WRONG_ID );
	pelapstimer->Timer.ID			= NTIMER_ID_TIMER; // To authorized 'NClearTimer'
	NClearTimer((NTIMER*)pelapstimer);
	pelapstimer->ElapsedTime		= 0.0f;
	pelapstimer->TimeLimit			= 0.0f;
	pelapstimer->EndHandle			= NULL;
	pelapstimer->TimeHandle			= NULL;
	pelapstimer->User_32			= 0;
}

void NDeleteElapsedTimer(NELAPSED_TIMER *pelapstimer)
{
	NClearElapsedTimer(pelapstimer);
	Nfree(pelapstimer);
}


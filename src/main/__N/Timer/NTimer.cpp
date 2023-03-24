// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NTimer.cpp										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NCore.h"
#include "../Containers/NArray.h"
#include "../Core/NSafeConversion.h"
#include "NUIThnAnimationPlayer.h"
#include "NThnAnimationPlayer.h"
#include "NElapsedTimer.h"
#include "NTimer.h"

extern Nu32	NEngineCoreFlags;

NARRAY		NTimerPtrArray;

NTIMER*	NSetupTimer(NTIMER *ptimer,const NTIMER_HANDLE timer_handle )
{
	if( NTimerPtrArray.Size < NU16_MAX )
	{
		ptimer->TimerHandle	= timer_handle;
		ptimer->ID			= NTIMER_ID_TIMER;
		ptimer->Index		= _SafeNu32ToNu16(NTimerPtrArray.Size);
		NArrayPushBack(&NTimerPtrArray,(NBYTE*)&ptimer);
	}
	else
	{
		//	It should never happen but, any way ... in that hypothetic case, setup is not possible ...
		//	so, ... just fill it up with ZEROs ...
		NErrorIf(1,NERROR_SYSTEM_CHECK);
		Nmem0(ptimer,NTIMER);
	}
	return ptimer;
}

NTIMER*	NCreateTimer(const NTIMER_HANDLE timer_handle)
{
	return NSetupTimer(NEW(NTIMER),timer_handle);
}

void	NClearTimer(NTIMER *ptimer)
{
	NErrorIf(ptimer->ID != NTIMER_ID_TIMER, NERROR_TIMER_WRONG_ID);								// In debug Mode, check ptimer is truly an NTIMER.
	NErrorIf(((NTIMER**)NTimerPtrArray.pFirst)[ptimer->Index] != ptimer , NERROR_SYSTEM_CHECK);	// In debug Mode, check NTimerPtrArray consistency.
	
	FLAG_ON(NEngineCoreFlags,FLAG_NENGINE_CORE_TIMERSTACK_TO_BE_CLEAN);
	((NTIMER**)NTimerPtrArray.pFirst)[ptimer->Index] = NULL;
	Nmem0(ptimer,NTIMER);
}

void	NDeleteTimer(NTIMER *ptimer)
{
	NClearTimer(ptimer);
	Nfree(ptimer);
}

void NInitializeTimerPtrArray()
{
	NSetupArray( &NTimerPtrArray, DEFAULT_NTIMER_PTR_ARRAY_CAPACITY,sizeof(NTIMER*) );
}

// ------------------------------------------------------------------------------------------
// static void NTimerPtrArray_Destructor
// ------------------------------------------------------------------------------------------
// Description :
//	Internal use only... by the 'NClearTimerPtrArray' function.
//	To call the right 'BasicClear' function in accordance with the Timer Type.
//	Usual NDeleteXXX timer object, call the   
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
static void NTimerPtrArray_Destructor(void* ptr)
{
	NTIMER *pt = *((NTIMER**)ptr); 
	if(pt)
	{
		switch( pt->ID )
		{
			case NTIMER_ID_TIMER:
				NDeleteTimer(pt);
				break;
			case NTIMER_ID_ELAPSED_TIMER:
				NDeleteElapsedTimer((NELAPSED_TIMER*)pt);
				break;
			case NTIMER_ID_THN_ANIMATION:
				NDeleteThnAnimationPlayer((NTHNANIM_PLAYER*)pt);
				break;
			case NTIMER_ID_UITHNANIM:
				NDeleteUIThnAnimationPlayer((NUITHNANIM_PLAYER*)pt);
				break;
			default:
				NErrorIf(1,NERROR_TIMER_UNKNOWN_ID);
				break;
		}
	}
}
void NClearTimerPtrArray()
{
	NClearArray(&NTimerPtrArray,NTimerPtrArray_Destructor );
}

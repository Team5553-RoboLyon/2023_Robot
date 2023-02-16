#ifndef __NTIMER_H
#define __NTIMER_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NTimer.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NType.h"
#include "../Core/NTime.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **								Timer Structures/Define & flags						**
// ***************************************************************************************
#define	DEFAULT_NTIMER_PTR_ARRAY_CAPACITY		4

#define NTIMER_ID_NULL				0	// Unused actually
#define NTIMER_ID_TIMER				1
#define NTIMER_ID_ELAPSED_TIMER		2
#define NTIMER_ID_THN_ANIMATION		3
#define NTIMER_ID_UITHNANIM			4

typedef struct NTIMER	NTIMER;
typedef void			(*NTIMER_HANDLE)(NTIMER *ptimer, const NTIME *ptime);

struct NTIMER  
{
	NTIMER_HANDLE			TimerHandle;
	Nu16					Index;
	Nu16					ID;
};

// ***************************************************************************************
// **							  Timer Functions										**
// ***************************************************************************************
NTIMER*		NSetupTimer(NTIMER *ptimer,const NTIMER_HANDLE timer_handle );
NTIMER*		NCreateTimer(const NTIMER_HANDLE timer_handle);
void		NClearTimer(NTIMER *ptimer);
void		NDeleteTimer(NTIMER *ptimer);

// LOW LEVEL
void		NInitializeTimerPtrArray();
void		NClearTimerPtrArray();


// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NTIMER_H 


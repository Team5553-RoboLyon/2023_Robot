#ifndef __NTIME_H
#define __NTIME_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NTime.h											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#ifdef _NIOS
#import <QuartzCore/QuartzCore.h>
#endif
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

	// ***************************************************************************************
	// **							Structures & Defines									**
	// ***************************************************************************************
	typedef struct
	{
		Nu64 Nu64_Frequency;

		Nu64 Nu64_Start;
		Nu64 Nu64_Previous;
		Nu64 Nu64_Current;
		Nu64 Nu64_Delay;

		Nf64 Nf64_InvFrequency;

		Nf64 Nf64_Start;	// Time value at start (sec) ( =(float)dwStart / 1000.0f )
		Nf64 Nf64_Previous; // Previous Time Value ( =(float)dwPrecedent / 1000.0f )
		Nf64 Nf64_Current;	// Current Time Value ( =(float)dwCurrent / 1000.0f )

		// Nu32	Nu32_Tick;			// Tick counter ( One Tick = 1 frame. Setup to 0, first frame = 1, second frame = 2, etc ... )
		Nf32 Nf32_Delay; // Delta time between Current and Previous time.
	} NTIME;

	NTIME *NCreateTime();
	NTIME *NSetupTime(NTIME *ptime);

	void NClearTime(NTIME *ptime);
	void NDeleteTime(NTIME *ptime);

	void NUpdateTime(NTIME *ptime);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NTIME_H

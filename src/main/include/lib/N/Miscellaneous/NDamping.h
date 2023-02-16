#ifndef __NDAMPING_H
#define __NDAMPING_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NDamping.h											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NCStandard.h"
#include "../NType.h"
#include "../NErrorHandling.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------
// Nf32 NEvaluateDampingValue
// ----------------------------------------------------------------------------------------------------------------------
// Description :
//	Purpose of this function is calculate the damping value to reach targetvalue from initialvalue in n iteration number.
//							x = initialvalue
//							for(iteration) x = x*damping
//							x == targetvalue
// ----------------------------------------------------------------------------------------------------------------------
// In	:	const Nf32 initialvalue		the supposed start value 
//			const Nf32 targetvalue		the value we want to reach
//			const Nu32 iteration		the number of iterations to reach targetvalue from initial value
// Out :	damping value
// ----------------------------------------------------------------------------------------------------------------------
inline Nf32 NEvaluateDampingValue(const Nf32 initialvalue, const Nf32 targetvalue, const Nu32 iteration)
{
	NErrorIf(!initialvalue,NERROR_NULL_VALUE);  // It's a damping, 0 x ... = 0 !!! Always ; so initialvalue should be != 0
	NErrorIf(!targetvalue,NERROR_NULL_VALUE);	// 0 doesn't Work. USer should use a small value instead of 0. Something like 0.0001 or  smaller. And it will work
	NErrorIf(!iteration,NERROR_NULL_VALUE);		// No iteration means NO CHANGE ... so iteration should be != 0
	#ifdef _NWINDOWS
		return powf(targetvalue/initialvalue,1.0f/(Nf32)iteration);
    #endif

    #ifdef _NANDROID
		return powf(targetvalue/initialvalue,1.0f/(Nf32)iteration);
	#endif

	#ifdef _NIOS
			// It seems there is no pow function in objective c, but we are in c so ...
			// lets check and try using  'NSDecimalNumber' instead if pow doesn't work ...
			NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION); // not an error, just a reminder
			return powf(targetvalue/initialvalue,1.0f/(Nf32)iteration);
	#endif
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NDAMPING_H 


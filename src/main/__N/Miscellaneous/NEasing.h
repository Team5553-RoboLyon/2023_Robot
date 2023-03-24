// ==========================================================================================
// NEasing.h
// ------------------------------------------------------------------------------------------
// Author  : JJTerazan
// Created : 05/04/2017
// Updated : 
// ------------------------------------------------------------------------------------------
// 
// ==========================================================================================
#ifndef __NEASING_H
#define __NEASING_H

#include "../NCStandard.h"
#include "../NType.h"
#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef enum
{
	NEASING_LINEAR = 0,		// 0   +   +   +   +   +   1 ---> 0   +   +   +   +   +   1
	NEASING_IN,				// 0   +   +   +   +   +   1 ---> 0      +     +    +  + +1
	NEASING_OUT,			// 0   +   +   +   +   +   1 ---> 0+ +  +    +     +      1
	NEASING_INOUT,			// 0   +   +   +   +   +   1 ---> 0+  +       +       +  +1
	NEASING_OVER,			// 0   +   +   +   +   +   1 ---> 0    +   + 3/2 +   +    1
	NEASING_UNDER,			// 0   +   +   +   +   +   1 ---> 0    +   +-1/2 +   +    1
	NEASING_SMOOTHOVER,		// 0   +   +   +   +   +   1 ---> 0+  +    + 3/2 +    +  +1
	NEASING_SMOOTHUNDER,	// 0   +   +   +   +   +   1 ---> 0+  +    +-1/2 +    +  +1
	// -----------------------------
	NEASING_HANDLE_ID_ENUM_SIZE
}NEASING_HANDLE_ID_ENUM;
typedef Nf32	(*NEASING_HANDLE)(const Nf32 normalized_ratio);


Nf32					NEasing_Linear(const Nf32 normalized_ratio);
Nf32					NEasing_In(const Nf32 normalized_ratio);
Nf32					NEasing_Out(const Nf32 normalized_ratio);
Nf32					NEasing_InOut(const Nf32 normalized_ratio);
Nf32					NEasing_Over(const Nf32 normalized_ratio);
Nf32					NEasing_Under(const Nf32 normalized_ratio);
Nf32					NEasing_SmoothOver(const Nf32 normalized_ratio);
Nf32					NEasing_SmoothUnder(const Nf32 normalized_ratio);

inline NEASING_HANDLE	NGetEasingHandle(NEASING_HANDLE_ID_ENUM id)
{
	switch(id)
	{
		case NEASING_LINEAR:
			return NEasing_Linear;
		case NEASING_IN:
			return NEasing_In;
		case NEASING_OUT:
			return NEasing_Out;
		case NEASING_INOUT:
			return NEasing_InOut;
		case NEASING_OVER:
			return NEasing_Over;
		case NEASING_UNDER:
			return NEasing_Under;
		case NEASING_SMOOTHOVER:
			return NEasing_SmoothOver;
		case NEASING_SMOOTHUNDER:
			return NEasing_SmoothUnder;

		default:
			return NEasing_Linear;
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NEASING_H 


#ifndef __NCYLINDRIC_H
#define __NCYLINDRIC_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NCylindric.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NType.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// Cylindric Coordinates
typedef struct
{
	union
	{
		struct  
		{
			Nf32 Radius;
			Nf32 Height;
			Nf32 Azimuth; // radian
		};
		Nf32	coord[3]; // See "NUsualCoordinates.h"
	};
}NCYLINDRIC;

// Fast Cylindric Coordinates
typedef struct
{
	Nf32 Radius;
	Nf32 Height;
	Nu32 Azimuth; // NDEGtoANGLE
}NFASTCYLINDRIC;

inline void NSetCylindricf(NCYLINDRIC *pc, Nf32 radius, Nf32 height, Nf32 azimuth){pc->Radius = radius; pc->Height = height; pc->Azimuth = azimuth;}
inline void NSetFastCylindricf(NFASTCYLINDRIC *pfc, Nf32 radius, Nf32 height, Nu32 azimuth){pfc->Radius = radius; pfc->Height = height; pfc->Azimuth = azimuth;}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NCYLINDRIC_H 


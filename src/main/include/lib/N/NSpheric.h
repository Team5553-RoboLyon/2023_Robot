#ifndef __NSPHERIC_H
#define __NSPHERIC_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NSpheric.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NType.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// Spherical Coordinates (ISO Format)
typedef struct
{
	union
	{
		struct  
		{
			Nf32 Radius;		// Radial distance from Origin to the Point ( == Radius of the sphere )
			Nf32 Inclination;	// radian: 0 <= Inclination <= PI. 0 means Aligned with Y direction, PI means Aligned with -Y direction
			Nf32 Azimuth;		// radian: 0 <=    Azimuth	<= 2PI. 	
		};
		Nf32	coord[3];		// See "NUsualCoordinates.h"
	};
}NSPHERIC;

// Fast Spherical Coordinates (ISO Format)
typedef struct
{
	Nf32 Radius;		// Radial distance from Origin to the Point ( == Radius of the sphere )
	Nu32 Inclination;	// NDEGtoANGLE: 0 <= Inclination <= NANGLE180. 0 means Aligned with Y direction, NANGLE180 means Aligned with -Y direction
	Nu32 Azimuth;		// NDEGtoANGLE: 0 <=    Azimuth	<= NANGLEMAX. 	
}NFASTSPHERIC;

inline void NSetSphericf(NSPHERIC *ps, Nf32 radius, Nf32 inclination, Nf32 azimuth){ps->Radius = radius; ps->Inclination = inclination;ps->Azimuth = azimuth;}
inline void NSetFastSphericf(NFASTSPHERIC *pfs, Nf32 radius, Nu32 inclination, Nu32 azimuth){pfs->Radius = radius; pfs->Inclination = inclination;pfs->Azimuth = azimuth;}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NSPHERIC_H 


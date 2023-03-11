#ifndef __NRANDOMIZE_H
#define __NRANDOMIZE_H

#include "NType.h"
#include "NMath.h"
#include "lib/N/Maths/NVec2f32.h"
#include "lib/N/Maths/NVec3f32.h"
#include "NSpheric.h"
#include "NCylindric.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							   NFastRandExtend.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
//
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Some HiLevel Extended Randomize Functions ...
//
inline	Nf32		NFastRandEx(const Nf32 min, const Nf32 range){return ( min + range*NFastRand() );}									// min < value < min + range 
inline	Nf32		NFastRandExMM(const Nf32 min, const Nf32 max){return ( min + (max-min)*NFastRand() );}								// min < value < max 
inline	Nf32		NFastRandExX(const Nf32 base, const Nf32 extent){return ( base + ( 1.0f-2.0f*NFastRand() )*extent );}				// (base-extent) < value < (base + extent)
inline	Nf32		NFastRandExC(const Nf32 base, const Nf32 base_coef){return ( base*(1.0f + (1.0f-2.0f*NFastRand())*base_coef) );}	// base*(1-base_coef) < value < base*(1+base_coef) 

inline void NFastRandEx_NVec2(NVEC2 *pv, const NVEC2*pv0,const NVEC2 *pvrange )
{
	pv->x	= pv0->x + pvrange->x*NFastRand();
	pv->y	= pv0->y + pvrange->y*NFastRand();
}
inline void	NFastRandExX_NVec2(NVEC2 *pv, const NVEC2 *pv0,const NVEC2 *pvextent )
{
	pv->x	= pv0->x + ( 1.0f-2.0f*NFastRand() )*pvextent->x;
	pv->y	= pv0->y + ( 1.0f-2.0f*NFastRand() )*pvextent->y;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + FastRandEx -> Apply to 3DCoord ...
inline void			NFastRandEx_Cartesian(NVEC3 *pv, const NVEC3 *pv0,const NVEC3 *pvrange )
{
	pv->x	= pv0->x + pvrange->x*NFastRand();
	pv->y	= pv0->y + pvrange->y*NFastRand();
	pv->z	= pv0->z + pvrange->z*NFastRand();
}
inline void			NFastRandEx_FastSpheric(NFASTSPHERIC *psph, const NFASTSPHERIC *psph0,const NFASTSPHERIC *psphrange )
{
	psph->Radius		= psph0->Radius + psphrange->Radius*NFastRand();	 
	psph->Inclination	= psph0->Inclination+(Nu32)((Nf32)psphrange->Inclination*NFastRand());	  
	psph->Azimuth		= psph0->Azimuth + (Nu32)( (Nf32)psphrange->Azimuth*NFastRand() );
}
static inline void NFastRandEx_FastCylindric(NFASTCYLINDRIC *pcyl, const NFASTCYLINDRIC *pcyl0,const NFASTCYLINDRIC *pcylrange )
{
	pcyl->Radius	= pcyl0->Radius + pcylrange->Radius*NFastRand();	 
	pcyl->Height	= pcyl0->Height + pcylrange->Height*NFastRand();	 
	pcyl->Azimuth	= pcyl0->Azimuth + (Nu32)( (Nf32)pcylrange->Azimuth*NFastRand() );
}
inline void			NFastRandEx_Spheric(NSPHERIC *psph, const NSPHERIC *psph0,const NSPHERIC *psphrange )
{
	psph->Radius		= psph0->Radius		+ psphrange->Radius*NFastRand();	 
	psph->Inclination	= psph0->Inclination+ psphrange->Inclination*NFastRand();	  
	psph->Azimuth		= psph0->Azimuth	+ psphrange->Azimuth*NFastRand();
}
static inline void NFastRandEx_Cylindric(NCYLINDRIC *pcyl, const NCYLINDRIC *pcyl0,const NCYLINDRIC *pcylrange )
{
	pcyl->Radius	= pcyl0->Radius + pcylrange->Radius*NFastRand();	 
	pcyl->Height	= pcyl0->Height	+ pcylrange->Height*NFastRand();	 
	pcyl->Azimuth	= pcyl0->Azimuth+ pcylrange->Azimuth*NFastRand();
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + FastRandExX -> Apply to 3DCoord ...
inline void	NFastRandExX_Cartesian(NVEC3 *pv, const NVEC3 *pv0,const NVEC3 *pvextent )
{
	pv->x	= pv0->x + ( 1.0f-2.0f*NFastRand() )*pvextent->x;
	pv->y	= pv0->y + ( 1.0f-2.0f*NFastRand() )*pvextent->y;
	pv->z	= pv0->z + ( 1.0f-2.0f*NFastRand() )*pvextent->z;
}

inline void	NFastRandExX_Cylindric(NCYLINDRIC *pcyl, const NCYLINDRIC *pcyl0,const NCYLINDRIC *pcylextent )
{
	pcyl->Radius	= pcyl0->Radius + ( 1.0f-2.0f*NFastRand() )*pcylextent->Radius ;
	pcyl->Height	= pcyl0->Height	+ ( 1.0f-2.0f*NFastRand() )*pcylextent->Height	;
	pcyl->Azimuth	= pcyl0->Azimuth+ ( 1.0f-2.0f*NFastRand() )*pcylextent->Azimuth;
}

inline void	NFastRandExX_Spheric(NSPHERIC *psph, const NSPHERIC *psph0,const NSPHERIC *psphextent )
{
	psph->Radius		= psph0->Radius		+ ( 1.0f-2.0f*NFastRand() )*psphextent->Radius ;
	psph->Inclination	= psph0->Inclination+ ( 1.0f-2.0f*NFastRand() )*psphextent->Inclination;
	psph->Azimuth		= psph0->Azimuth	+ ( 1.0f-2.0f*NFastRand() )*psphextent->Azimuth;
}
inline void	NFastRandExX_FastCylindric(NFASTCYLINDRIC *pcyl, const NFASTCYLINDRIC *pcyl0,const NFASTCYLINDRIC *pcylextent )
{
	pcyl->Radius	= pcyl0->Radius + ( 1.0f-2.0f*NFastRand() )*pcylextent->Radius ;
	pcyl->Height	= pcyl0->Height	+ ( 1.0f-2.0f*NFastRand() )*pcylextent->Height	;
	pcyl->Azimuth	= pcyl0->Azimuth+ (Nu32)(( 1.0f-2.0f*NFastRand() )*(Nf32)pcylextent->Azimuth);
}

inline void	NFastRandExX_FastSpheric(NFASTSPHERIC *psph, const NFASTSPHERIC *psph0,const NFASTSPHERIC *psphextent )
{
	psph->Radius		= psph0->Radius		+ ( 1.0f-2.0f*NFastRand() )*psphextent->Radius ;
	psph->Inclination	= psph0->Inclination+ (Nu32)(( 1.0f-2.0f*NFastRand() )*(Nf32)psphextent->Inclination);
	psph->Azimuth		= psph0->Azimuth	+ (Nu32)(( 1.0f-2.0f*NFastRand() )*(Nf32)psphextent->Azimuth);
}


// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NRANDOMIZE_H 


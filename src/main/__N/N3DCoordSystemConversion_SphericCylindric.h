#ifndef __N3D_COORDSYSTEM_CONVERSION_SPHERICCYLINDRIC_H
#define __N3D_COORDSYSTEM_CONVERSION_SPHERICCYLINDRIC_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **					N3DCoordSystemConversion_SphericCylindric.h						**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NType.h"
#include "./Maths/NVec3f32.h"
#include "NSpheric.h"
#include "NCylindric.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
//	All of these functions are inline ...
//	Inclination Reference Axis is forced to be the vertical one into these 3 schemes.
//	The base still Direct of course ...
//	
//			 X					Y					Z					
//			 |					|					|					
//			 | 					|					|					
//			 O---Z 				O--- X				O--- Y				
//			/	 			   /				   /					
//		   Y				  Z					  X						
//

// Spheric coordinates system needs a ZENITH DIRECTION REFERENCE ORIENTED AXIS to define
// the inclination angle with OP. Where O is the origin and P the point to describe.
//			Cartesian	P(x,y,z) 
//			Spherical	P(r,inclination,azimuth) 
//			Cylindrical P(r,azimuth,Height) 

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + CYLINDRIQUE COORDINATES <<< >>> SPHERIC COORDINATES
// +
inline void NCylindricToSpheric(NSPHERIC *psph, const NCYLINDRIC *pcyl)
{
	psph->Azimuth		= pcyl->Azimuth;
	psph->Radius		= sqrt( pcyl->Radius*pcyl->Radius + pcyl->Height*pcyl->Height );
	if(psph->Radius<NF32_EPSILON_VECTOR_LENGTH)
	{
		psph->Radius = psph->Inclination = 0.0f;
	}
	else
	{
		psph->Inclination	= acos(pcyl->Height/psph->Radius); 
	//	If using atan2 for calculation ....
	//	-------------------------------------
	//	psph->Inclination	= atan2(pcyl->Radius,pcyl->Height);
	// 	if(psph->Inclination < 0.0f)
	// 	{
	// 		psph->Inclination +=NF32_2PI;
	// 	}
	}
}
inline void NFastCylindricToFastSpheric(NFASTSPHERIC *psph, const NFASTCYLINDRIC *pcyl)
{
	psph->Azimuth		= pcyl->Azimuth;
	psph->Radius		= NFastSqrt( pcyl->Radius*pcyl->Radius + pcyl->Height*pcyl->Height );
	if(psph->Radius<NF32_EPSILON_VECTOR_LENGTH)
	{
		psph->Radius = 0.0f; psph->Inclination = 0;
	}
	else
	{
		psph->Inclination	= NRADtoANGLE( acos(pcyl->Height/psph->Radius) );
		//	If using atan2 for calculation ....
		//	-------------------------------------
		//	Nf32 inc = atan2(pcyl->Radius,pcyl->Height);
		// 	if(inc < 0.0f)
		// 	{
		// 		inc +=NF32_2PI;
		// 	}
		// psph->Inclination = NRADtoANGLE(inc);
	}
}
inline void NSphericToCylindric(NCYLINDRIC *pcyl, const NSPHERIC *psph)
{
	Nf32	cos_inclination,sin_inclination;

	NCosSin(psph->Inclination,&cos_inclination,&sin_inclination);
	pcyl->Azimuth	= psph->Azimuth;
	pcyl->Height	= psph->Radius*cos_inclination;
	pcyl->Radius	= psph->Radius*sin_inclination;
}
inline void NFastSphericToFastCylindric(NFASTCYLINDRIC *pcyl, const NFASTSPHERIC *psph)
{
	Nf32	cos_inclination,sin_inclination;

	NFastCosSin(psph->Inclination,&cos_inclination,&sin_inclination);
	pcyl->Azimuth	= psph->Azimuth;
	pcyl->Height	= psph->Radius*cos_inclination;
	pcyl->Radius	= psph->Radius*sin_inclination;
}
/*

inline void N3DCoord_SphericToCylindric(NVEC3 *pv, const NSPHERIC *psph){}
inline void N3DCoord_CylindricToSpheric(NSPHERIC *psph, const NVEC3 *pv){}

*/
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __N3D_COORDSYSTEM_CONVERSION_SPHERICCYLINDRIC_H 


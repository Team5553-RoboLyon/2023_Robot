#ifndef __N3D_COORDSYSTEM_CONVERSION_CARTESIANCYLINDRIC_H
#define __N3D_COORDSYSTEM_CONVERSION_CARTESIANCYLINDRIC_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **			N3DCoordSystemConversion_CartesianCylindric.h							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NType.h"
#include "./Maths/NVec3f32.h"
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
// + FROM CYLINDRIC COORDINATES TO CARTESIAN COORDINATES
// +
// Height Axis Reference is Cartesian Axe X
inline void NCylindricXToCartesian(NVEC3 *pv, const NCYLINDRIC *pcyl)
{
	Nf32 cos_azimuth,sin_azimuth; 
	NCosSin(pcyl->Azimuth,&cos_azimuth,&sin_azimuth); 
	pv->x = pcyl->Height;
	pv->y = pcyl->Radius*cos_azimuth;
	pv->z =	pcyl->Radius*sin_azimuth;
}
inline void NFastCylindricXToCartesian(NVEC3 *pv, const NFASTCYLINDRIC *pcyl)
{
	Nf32 cos_azimuth,sin_azimuth; 
	NFastCosSin(pcyl->Azimuth,&cos_azimuth,&sin_azimuth); 
	pv->x = pcyl->Height;
	pv->y = pcyl->Radius*cos_azimuth;
	pv->z =	pcyl->Radius*sin_azimuth;
}
// +
// Height Axis Reference is Cartesian Axe Y
inline void NCylindricYToCartesian(NVEC3 *pv, const NCYLINDRIC *pcyl)
{
	Nf32 cos_azimuth,sin_azimuth; 
	NCosSin(pcyl->Azimuth,&cos_azimuth,&sin_azimuth); 
	pv->x =	pcyl->Radius*sin_azimuth;
	pv->y = pcyl->Height;
	pv->z = pcyl->Radius*cos_azimuth;
}
inline void NFastCylindricYToCartesian(NVEC3 *pv, const NFASTCYLINDRIC *pcyl)
{
	Nf32 cos_azimuth,sin_azimuth; 
	NFastCosSin(pcyl->Azimuth,&cos_azimuth,&sin_azimuth); 
	pv->x =	pcyl->Radius*sin_azimuth;
	pv->y = pcyl->Height;
	pv->z = pcyl->Radius*cos_azimuth;
}
// +
// Height Axis Reference is Cartesian Axe Z
inline void NCylindricZToCartesian(NVEC3 *pv, const NCYLINDRIC *pcyl)
{
	Nf32 cos_azimuth,sin_azimuth; 
	NCosSin(pcyl->Azimuth,&cos_azimuth,&sin_azimuth); 
	pv->x = pcyl->Radius*cos_azimuth;
	pv->y =	pcyl->Radius*sin_azimuth;
	pv->z = pcyl->Height;
}
inline void NFastCylindricZToCartesian(NVEC3 *pv, const NFASTCYLINDRIC *pcyl)
{
	Nf32 cos_azimuth,sin_azimuth; 
	NFastCosSin(pcyl->Azimuth,&cos_azimuth,&sin_azimuth); 
	pv->x = pcyl->Radius*cos_azimuth;
	pv->y =	pcyl->Radius*sin_azimuth;
	pv->z = pcyl->Height;
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + FROM CARTESIAN COORDINATES TO CYLINDRIC COORDINATES
// +
// Height Axis Reference is Cartesian Axe X
inline void NCartesianToCylindricX(NCYLINDRIC *pcyl, const NVEC3 *pv)
{
	pcyl->Height = pv->x;

	if(pv->y == 0.0f && pv->z == 0.0f)
	{
		pcyl->Radius	= 0.0f;
		pcyl->Azimuth	= 0.0f;
		return;
	}
	else
	{
		pcyl->Radius = sqrt(pv->y*pv->y + pv->z*pv->z);
		pcyl->Azimuth= atan2( pv->z,pv->y );
		if(pcyl->Azimuth < 0.0f)
		{
			pcyl->Azimuth+=NF32_2PI;
		}
	}
}
inline void NCartesianToFastCylindricX(NFASTCYLINDRIC *pcyl, const NVEC3 *pv)
{
	pcyl->Height = pv->x;

	if(pv->y == 0.0f && pv->z == 0.0f)
	{
		pcyl->Radius	= 0.0f;
		pcyl->Azimuth	= 0;
		return;
	}
	else
	{
		pcyl->Radius = NFastSqrt(pv->y*pv->y + pv->z*pv->z);
		Nf32 azim = atan2( pv->z,pv->y );
		if(azim < 0.0f)
		{
			azim += NF32_2PI;
		}
		pcyl->Azimuth = NRADtoANGLE( azim );
	}
}

// Height Axis Reference is Cartesian Axe Y
inline void NCartesianToCylindricY(NCYLINDRIC *pcyl, const NVEC3 *pv)
{
	pcyl->Height = pv->y;
	if(pv->z == 0.0f && pv->x == 0.0f)
	{
		pcyl->Radius	= 0.0f;
		pcyl->Azimuth	= 0.0f;
		return;
	}
	else
	{
		pcyl->Radius = sqrt(pv->z*pv->z + pv->x*pv->x);
		pcyl->Azimuth= atan2( pv->x,pv->z );
		if(pcyl->Azimuth < 0.0f)
		{
			pcyl->Azimuth+=NF32_2PI;
		}
	}
}
inline void NCartesianToFastCylindricY(NFASTCYLINDRIC *pcyl, const NVEC3 *pv)
{
	pcyl->Height = pv->y;
	if(pv->z == 0.0f && pv->x == 0.0f)
	{
		pcyl->Radius	= 0.0f;
		pcyl->Azimuth	= 0;
		return;
	}
	else
	{
		pcyl->Radius = NFastSqrt(pv->z*pv->z + pv->x*pv->x);
		Nf32 azim = atan2( pv->x,pv->z );
		if(azim < 0.0f)
		{
			azim+=NF32_2PI;
		}
		pcyl->Azimuth = NRADtoANGLE( azim );
	}
}
// Height Axis Reference is Cartesian Axe Z
inline void NCartesianToCylindricZ(NCYLINDRIC *pcyl, const NVEC3 *pv)
{
	pcyl->Height = pv->z;
	if(pv->x == 0.0f && pv->y == 0.0f)
	{
		pcyl->Radius	= 0.0f;
		pcyl->Azimuth	= 0.0f;
		return;
	}
	else
	{
		pcyl->Radius = sqrt(pv->x*pv->x + pv->y*pv->y);
		pcyl->Azimuth= atan2( pv->y,pv->x );
		if(pcyl->Azimuth < 0.0f)
		{
			pcyl->Azimuth+=NF32_2PI;
		}
	}
}
inline void NCartesianToFastCylindricZ(NFASTCYLINDRIC *pcyl, const NVEC3 *pv)
{
	pcyl->Height = pv->z;
	if(pv->x == 0.0f && pv->y == 0.0f)
	{
		pcyl->Radius	= 0.0f;
		pcyl->Azimuth	= 0;
		return;
	}
	else
	{
		pcyl->Radius = NFastSqrt(pv->x*pv->x + pv->y*pv->y);
		Nf32 azim = atan2( pv->y,pv->x );
		if(azim < 0.0f)
		{
			azim +=NF32_2PI;
		}
		pcyl->Azimuth = NRADtoANGLE( azim );
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __N3D_COORDSYSTEM_CONVERSIONS_CARTESIANCYLINDRIC_H 


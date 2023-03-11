#ifndef __N3D_COORDSYSTEM_CONVERSION_CARTESIANSPHERIC_H
#define __N3D_COORDSYSTEM_CONVERSION_CARTESIANSPHERIC_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **					N3DCoordSystemConversion_CartesianSpheric.h						**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NType.h"
#include "lib/N/Maths/NVec3f32.h"
#include "NSpheric.h"

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
// + FROM SPHERIC COORDINATES TO CARTESIAN COORDINATES
// +
// Inclination Axis Reference is Cartesian Axe X
inline void NSphericXToCartesian(NVEC3 *pv, const NSPHERIC *psph)
{
	Nf32 cos_inclination,sin_inclination,cos_azimuth,sin_azimuth; 
	NCosSin(psph->Inclination,&cos_inclination,&sin_inclination); NCosSin(psph->Azimuth,&cos_azimuth,&sin_azimuth); 
	pv->y =	psph->Radius*sin_inclination*cos_azimuth;
	pv->z = psph->Radius*sin_inclination*sin_azimuth; 
	pv->x = psph->Radius*cos_inclination;
}
inline void NFastSphericXToCartesian(NVEC3 *pv, const NFASTSPHERIC *psph)
{
	Nf32 cos_inclination,sin_inclination,cos_azimuth,sin_azimuth; 
	NFastCosSin(psph->Inclination,&cos_inclination,&sin_inclination); NFastCosSin(psph->Azimuth,&cos_azimuth,&sin_azimuth); 
	pv->y =	psph->Radius*sin_inclination*cos_azimuth;
	pv->z = psph->Radius*sin_inclination*sin_azimuth; 
	pv->x = psph->Radius*cos_inclination;
}
// +
// Inclination Axis Reference is Cartesian Axe Y
inline void NSphericYToCartesian(NVEC3 *pv, const NSPHERIC *psph)
{
	Nf32 cos_inclination,sin_inclination,cos_azimuth,sin_azimuth; 
	NCosSin(psph->Inclination,&cos_inclination,&sin_inclination); NCosSin(psph->Azimuth,&cos_azimuth,&sin_azimuth); 
	pv->z = psph->Radius*sin_inclination*cos_azimuth;
	pv->x = psph->Radius*sin_inclination*sin_azimuth; 
	pv->y = psph->Radius*cos_inclination;
}
inline void NFastSphericYToCartesian(NVEC3 *pv, const NFASTSPHERIC *psph)
{
	Nf32 cos_inclination,sin_inclination,cos_azimuth,sin_azimuth; 
	NFastCosSin(psph->Inclination,&cos_inclination,&sin_inclination); NFastCosSin(psph->Azimuth,&cos_azimuth,&sin_azimuth); 
	pv->z = psph->Radius*sin_inclination*cos_azimuth;
	pv->x = psph->Radius*sin_inclination*sin_azimuth; 
	pv->y = psph->Radius*cos_inclination;
}
// +
// Inclination Axis Reference is Cartesian Axe Z 
inline void NSphericZToCartesian(NVEC3 *pv, const NSPHERIC *psph)
{
	Nf32 cos_inclination,sin_inclination,cos_azimuth,sin_azimuth; 
	NCosSin(psph->Inclination,&cos_inclination,&sin_inclination); NCosSin(psph->Azimuth,&cos_azimuth,&sin_azimuth); 
	pv->x = psph->Radius*sin_inclination*cos_azimuth;
	pv->y = psph->Radius*sin_inclination*sin_azimuth; 
	pv->z = psph->Radius*cos_inclination;
}
inline void NFastSphericZToCartesian(NVEC3 *pv, const NFASTSPHERIC *psph)
{
	Nf32 cos_inclination,sin_inclination,cos_azimuth,sin_azimuth; 
	NFastCosSin(psph->Inclination,&cos_inclination,&sin_inclination); NFastCosSin(psph->Azimuth,&cos_azimuth,&sin_azimuth); 
	pv->x = psph->Radius*sin_inclination*cos_azimuth;
	pv->y = psph->Radius*sin_inclination*sin_azimuth; 
	pv->z = psph->Radius*cos_inclination;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + FROM CARTESIAN COORDINATES TO SPHERIC COORDINATES
// +
// Inclination Axis Reference is Cartesian Axe X 
inline void NCartesianToSphericX(NSPHERIC *psph, const NVEC3 *pv)
{
	if(pv->y == 0.0f && pv->z == 0.0f)
	{
		psph->Radius = pv->x; psph->Azimuth = psph->Inclination = 0.0f;
		return;
	}
	psph->Radius = NVec3Length(pv);
	if(psph->Radius<NF32_EPSILON_VECTOR_LENGTH)
	{
		psph->Radius = psph->Azimuth = psph->Inclination = 0.0f;
	}
	else
	{
		psph->Inclination	= acos( pv->x/psph->Radius );
		psph->Azimuth		= atan2( pv->z,pv->y );
		if(psph->Azimuth < 0.0f)
		{
			psph->Azimuth+=NF32_2PI;
		}
	}
}
inline void NCartesianToFastSphericX(NFASTSPHERIC *psph, const NVEC3 *pv)
{
	if(pv->y == 0.0f && pv->z == 0.0f)
	{
		psph->Radius = pv->x; psph->Azimuth = psph->Inclination = 0;
		return;
	}
	psph->Radius = NVec3FastLength(pv);
	if(psph->Radius<NF32_EPSILON_VECTOR_LENGTH)
	{
		psph->Radius = 0.0f; psph->Azimuth = psph->Inclination = 0;
	}
	else
	{
		psph->Inclination	= NRADtoANGLE( acos( pv->x/psph->Radius ) );
		Nf32 az = atan2( pv->z,pv->y );
		if(az < 0.0f)
		{
			az+=NF32_2PI;
		}
		psph->Azimuth = NRADtoANGLE(az);
	}
}
// +
// Inclination Axis Reference is Cartesian Axe Y 
inline void NCartesianToSphericY(NSPHERIC *psph, const NVEC3 *pv)
{
	if(pv->x == 0.0f && pv->z == 0.0f)
	{
		psph->Radius = pv->y; psph->Azimuth = psph->Inclination = 0.0f;
		return;
	}
	psph->Radius = NVec3Length(pv);
	if(psph->Radius<NF32_EPSILON_VECTOR_LENGTH)
	{
		psph->Radius = psph->Azimuth = psph->Inclination = 0.0f;
	}
	else
	{
		psph->Inclination	= acos( pv->y/psph->Radius );
		psph->Azimuth		= atan2( pv->x,pv->y );
		if(psph->Azimuth < 0.0f)
		{
			psph->Azimuth+=NF32_2PI;
		}
	}
}
inline void NCartesianToFastSphericY(NFASTSPHERIC *psph, const NVEC3 *pv)
{
	if(pv->x == 0.0f && pv->z == 0.0f)
	{
		psph->Radius = pv->y; psph->Azimuth = psph->Inclination = 0;
		return;
	}
	psph->Radius = NVec3FastLength(pv);
	if(psph->Radius<NF32_EPSILON_VECTOR_LENGTH)
	{
		psph->Radius = 0.0f; psph->Azimuth = psph->Inclination = 0;
	}
	else
	{
		psph->Inclination	= NRADtoANGLE(acos( pv->y/psph->Radius ));
		Nf32 az				= atan2( pv->x,pv->y );
		if(az < 0.0f)
		{
			az+=NF32_2PI;
		}
		psph->Azimuth		= NRADtoANGLE(az);
	}
}
// +
// Inclination Axis Reference is Cartesian Axe Z 
inline void NCartesianToSphericZ(NSPHERIC *psph, const NVEC3 *pv)
{
	if(pv->x == 0.0f && pv->y == 0.0f)
	{
		psph->Radius = pv->z; psph->Azimuth = psph->Inclination = 0.0f;
		return;
	}
	psph->Radius = NVec3Length(pv);
	if(psph->Radius<NF32_EPSILON_VECTOR_LENGTH)
	{
		psph->Radius = psph->Azimuth = psph->Inclination = 0.0f;
	}
	else
	{
		psph->Inclination	= acos( pv->z/psph->Radius );
		psph->Azimuth		= atan2( pv->y,pv->x );
		if(psph->Azimuth < 0.0f)
		{
			psph->Azimuth+=NF32_2PI;
		}
	}
}
inline void NCartesianToFastSphericZ(NFASTSPHERIC *psph, const NVEC3 *pv)
{
	if(pv->x == 0.0f && pv->y == 0.0f)
	{
		psph->Radius = pv->z; psph->Azimuth = psph->Inclination = 0;
		return;
	}
	psph->Radius = NVec3FastLength(pv);
	if(psph->Radius<NF32_EPSILON_VECTOR_LENGTH)
	{
		psph->Radius = 0.0f; psph->Azimuth = psph->Inclination = 0;
	}
	else
	{
		psph->Inclination	= NRADtoANGLE( acos( pv->z/psph->Radius ) );
		Nf32 az				= atan2( pv->y,pv->x );
		if(az < 0.0f)
		{
			az+=NF32_2PI;
		}
		psph->Azimuth		= NRADtoANGLE(az);
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __N3D_COORDSYSTEM_CONVERSION_CARTESIANSPHERIC_H 


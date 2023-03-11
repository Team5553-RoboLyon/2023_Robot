#ifndef __NBOUNDINGVOLUMES_H
#define __NBOUNDINGVOLUMES_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NBoundingVolumes.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NType.h"
#include "lib/N/Maths/NVec3f32.h"
#include "lib/N/Core/NLimits.h"
//#include "NGeometry.h"
//#include "NArray.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **							Bounding Volume: AABB									**
// **									---												**
// **								Structures											**
// ***************************************************************************************
typedef struct
{
	union
	{
		struct  
		{
			Nf32	fXMin;		// XMin = x position (bottom left corner)
			Nf32	fYMin;		// YMin = y position (bottom left corner)
			Nf32	fZMin;		// ZMin = z position (bottom left corner)
		};
		NVEC3	VMin;		
	};

	union
	{
		struct  
		{
			Nf32	fXMax;		// XMax = XMin + Width
			Nf32	fYMax;		// YMax = YMin + Height
			Nf32	fZMax;		// ZMax = ZMin + Depth
		};
		NVEC3	VMax;
	};
}NAABB;

// ***************************************************************************************
// **																					**
// **							Bounding Volume: SPHERE									**
// **									---												**
// **								Structures											**
// ***************************************************************************************
typedef struct
{
	NVEC3	Center;		// Sphere Center position
	Nf32		fRadius;	// Sphere Radius
}NBOUNDINGSPHERE;

// ***************************************************************************************
// **							  Bounding Volume										**
// **									---												**
// **							Union of all of them									**
// ***************************************************************************************
typedef union
{
	NBOUNDINGSPHERE		BoundingSphere;
	NAABB				BoundingAABB;
} NBOUNDING_VOLUME;

inline NVEC3* NGetAABBSize(NVEC3 *pv,const NAABB *paabb)
{
	pv->x = paabb->fXMax - paabb->fXMin;
	pv->y = paabb->fYMax - paabb->fYMin;
	pv->z = paabb->fZMax - paabb->fZMin;
	return pv;
}

inline NVEC3* NGetAABBCenter(NVEC3 *pcenter, const NAABB *paabb)
{
	pcenter->x = ( paabb->fXMax + paabb->fXMin )*0.5f;
	pcenter->y = ( paabb->fYMax + paabb->fYMin )*0.5f;
	pcenter->z = ( paabb->fZMax + paabb->fZMin )*0.5f;
	return pcenter;
}

inline	NVEC3* NGetAABBExtents(NVEC3 *pextents, const NAABB *paabb)
{
	pextents->x = (paabb->fXMax - paabb->fXMin)*0.5f;
	pextents->y = (paabb->fYMax - paabb->fYMin)*0.5f;
	pextents->z = (paabb->fZMax - paabb->fZMin)*0.5f;
	return pextents;
}

inline void NAABB_IncludeVector3Into(NAABB *pdst,const NVEC3 *pv)
{
	if( pv->x < pdst->fXMin )
		pdst->fXMin = pv->x;
	else if(pv->x > pdst->fXMax)
		pdst->fXMax = pv->x;

	if( pv->y < pdst->fYMin )
		pdst->fYMin = pv->y;
	else if(pv->y > pdst->fYMax)
		pdst->fYMax = pv->y;

	if( pv->z < pdst->fZMin )
		pdst->fZMin = pv->z;
	else if(pv->z > pdst->fZMax)
		pdst->fZMax = pv->z;
}

inline void NAABB_ClampVector3(NVEC3 *pv, const NAABB *paabb)
{
	pv->x = NCLAMP(paabb->fXMin,pv->x,paabb->fXMax);
	pv->y = NCLAMP(paabb->fYMin,pv->y,paabb->fYMax);
	pv->z = NCLAMP(paabb->fZMin,pv->z,paabb->fZMax);
}

inline void NAABB_IncludeAABBInto(NAABB *pdst,const NAABB *psrc)
{
	if( psrc->fXMin < pdst->fXMin )
		pdst->fXMin = psrc->fXMin;
	if( psrc->fYMin < pdst->fYMin )
		pdst->fYMin = psrc->fYMin;
	if( psrc->fZMin < pdst->fZMin )
		pdst->fZMin = psrc->fZMin;

	if( psrc->fXMax > pdst->fXMax )
		pdst->fXMax = psrc->fXMax;
	if( psrc->fYMax > pdst->fYMax )
		pdst->fYMax = psrc->fYMax;
	if( psrc->fZMax > pdst->fZMax )
		pdst->fZMax = psrc->fZMax;
}

inline void NSetMinMaxAABBWithMaxMinNf32(NAABB *paabb)
{
	paabb->fXMin = paabb->fYMin = paabb->fZMin = NF32_MAX;
	paabb->fXMax = paabb->fYMax = paabb->fZMax = -NF32_MAX;
}

inline Nbool NIsVector3IncludedIntoAABB(const NVEC3 *ppos, const NAABB *paabb)
{
	if(ppos->x < paabb->fXMin)
		return NFALSE;
	if(ppos->x > paabb->fXMax)
		return NFALSE;

	if(ppos->y < paabb->fYMin)
		return NFALSE;
	if(ppos->y > paabb->fYMax)
		return NFALSE;
	
	if(ppos->z < paabb->fZMin)
		return NFALSE;
	if(ppos->z > paabb->fZMax)
		return NFALSE;

	return NTRUE;
}

inline NBOUNDINGSPHERE* NGetAABBBoundingSphere(NBOUNDINGSPHERE *pbs, const NAABB *paabb)
{
	NVEC3	v;

	// Center
	pbs->Center.x	= (paabb->fXMin+ paabb->fXMax)*0.5f;
	pbs->Center.y	= (paabb->fYMin+ paabb->fYMax)*0.5f;
	pbs->Center.z	= (paabb->fZMin+ paabb->fZMax)*0.5f;

	// Radius
	NVec3Sub(&v,&paabb->VMax,&paabb->VMin);
	pbs->fRadius	= 0.5f*NVec3Length(&v);
	return pbs;
}


// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NBOUNDINGVOLUMES_H 


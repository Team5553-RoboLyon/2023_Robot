#ifndef __NUT_PICK_H
#define __NUT_PICK_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_Pick.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "../Maths/NVec3f32.h"
#include "../NSpline.h"
#include "../NScreenPoint.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// ***************************************************************************************************************************************************************
	// ***************************************************************************************************************************************************************
	// **																																							**
	// **														GLOBAL and GENERIC DEFINEs																			**
	// **																																							**
	// ***************************************************************************************************************************************************************
	// ***************************************************************************************************************************************************************

	// PICK RESULT STRUCTURES
	// Spline
	// faire une union pickspline point (comme ici) avec pickspline splinepoint
	typedef struct
	{
		NVEC3 PickedPosition;
		NVEC3 EstimatedTA;
		NVEC3 EstimatedTB;
		Nu32 BSplinePointIndex; // Nu32	ASplinePointIndex; ... is  not necessary because it's obvious: A = B-1. Always !
		Nf32 LerpFactor;
		Nf32 fDot; // ZfromCam indeed ...
	} NUT_PICKSPLINE;

	typedef struct
	{
		NVEC3 *pPickedV3Element;
		Nu32 PickedElementIndex;
		Nf32 PickingDistance;
	} NUT_PICKV3ARRAY;

	// ***************************************************************************************
	// ***************************************************************************************
	// **																					**
	// **									 												**
	// **							 Pick and Detection										**
	// **									---												**
	// **							     Functions											**
	// **																					**
	// ***************************************************************************************
	// ***************************************************************************************
	/*
	Nf32	NUT_Pick3DPoint(const NVEC2s16 *pmouse2d,const Nf32 pickradius,const NVEC3 *pv);
	Nf32	NUT_Pick3DPointEx(const NVEC3 *prayorigin,const NVEC3 *prayvdir,const Nf32 pickradius,const NVEC3 *pv);
	Nf32	NUT_Pick3DPointEx_SquareDist(const NVEC3 *prayorigin,const NVEC3 *prayvdir,const Nf32 picksquareradius,const NVEC3 *pv);
	*/

	Nbool NUT_Pick3DSpline(NUT_PICKSPLINE *presult, const NVEC2s16 *pmouse2d, const Nf32 pickradius, const NSPLINE *pspline);
	Nu32 NUT_PickVector3Array(NUT_PICKV3ARRAY *presult, const NVEC2s16 *pmouse2d, const Nf32 pickradius, const NARRAY *pv3array);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NUT_PICK_H

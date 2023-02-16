#ifndef __NFRUSTUM_CULLING_H
#define __NFRUSTUM_CULLING_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NFrustum_Culling.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "./Renderable/NRenderable.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **							 Structures & Defines									**
// ***************************************************************************************
typedef enum
{
	NFRUSTUM_CULLING_OUTSIDE	= 0,
	NFRUSTUM_CULLING_INSIDE,	
	NFRUSTUM_CULLING_INTERSECT,
	//----------------------
	NFRUSTUM_CULLING_RESULT_ENUMSIZE
}NFRUSTUM_CULLING_RESULT;

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
NFRUSTUM_CULLING_RESULT	NFrustumCulling_BSphere_vs_NearFarFrustum(const NBOUNDINGSPHERE *psphere, const NMATRIX *pmatrix, Nf32 *preturn_spherecamcoord_z);
NFRUSTUM_CULLING_RESULT	NFrustumCulling_BSphere_vs_3DFrustum(const NBOUNDINGSPHERE *psphere, const NMATRIX *pmatrix, Nf32 *preturn_spherecamcoord_z);

NFRUSTUM_CULLING_RESULT	NRenderableCulling_vs_3DFrustum(const NRENDERABLE *prenderable, Nf32 *preturn_spherecamcoord_z);
NFRUSTUM_CULLING_RESULT	NRenderableCulling_vs_NearFarFrustum(const NRENDERABLE *prenderable, Nf32 *preturn_spherecamcoord_z);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NFRUSTUM_CULLING_H 


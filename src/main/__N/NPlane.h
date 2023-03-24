#ifndef __NPLANE_H
#define __NPLANE_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NPlane.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NType.h"
#include "./Maths/NVec3f32.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef struct
{
	NVEC3	Origin;
	NVEC3	DirectorVector;
}NPLANE;

inline void NSetPlanef(NPLANE *ppl, const Nf32 ox,const Nf32 oy,const Nf32 oz,const Nf32 vdir_x,const Nf32 vdir_y,const Nf32 vdir_z)
{
	ppl->Origin.x = ox;
	ppl->Origin.y = oy;
	ppl->Origin.z = oz;

	ppl->DirectorVector.x = vdir_x;
	ppl->DirectorVector.y = vdir_y;
	ppl->DirectorVector.z = vdir_z;
}

inline void NSetPlane(NPLANE *ppl, const NVEC3 *porigin,const NVEC3 *pvdir)
{
	ppl->Origin = *porigin;
	ppl->DirectorVector = *pvdir;
}

inline void NSetPlaneOrigin(NPLANE *ppl, const NVEC3 *porigin)
{
	ppl->Origin = *porigin;
}

inline void NSetPlaneOriginf(NPLANE *ppl, const Nf32 ox,const Nf32 oy,const Nf32 oz)
{
	ppl->Origin.x = ox;
	ppl->Origin.y = oy;
	ppl->Origin.z = oz;
}

inline void NSetPlaneDirectorVector(NPLANE *ppl, const NVEC3 *pvdir)
{
	ppl->DirectorVector = *pvdir;
}

inline void NSetPlaneDirectorVectorf(NPLANE *ppl, const Nf32 vdir_x,const Nf32 vdir_y,const Nf32 vdir_z)
{
	ppl->DirectorVector.x = vdir_x;
	ppl->DirectorVector.y = vdir_y;
	ppl->DirectorVector.z = vdir_z;
}




// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NBOUNDINGVOLUMES_H 


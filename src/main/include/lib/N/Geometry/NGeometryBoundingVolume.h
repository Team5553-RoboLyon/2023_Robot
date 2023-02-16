#ifndef __NGEOMETRY_BOUNDINGVOLUME_H
#define __NGEOMETRY_BOUNDINGVOLUME_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NGeometryBoundingVolume.h							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NType.h"
#include "../NMatrix.h"
#include "../Containers/NArray.h"
#include "../Render/Accumulator/NAccumulator.h"
#include "../Geometry/NGeometry.h"
#include "../NBoundingVolumes.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

void			NGetGeometryAABB(const NGEOMETRY *pgeom, NAABB *paabb);
void			NGetGeometryArrayAABB(NAABB *paabb,const NARRAY *pgeomarray);
inline	void	NGetGeometryAABBSize(const NGEOMETRY *pgeom, NVEC3 *psize){NAABB aabb;NGetGeometryAABB(pgeom,&aabb);NGetAABBSize(psize,&aabb);}
void			NGetBoundingSphereFromGeometryArray( NBOUNDINGSPHERE *pbs, const NARRAY *pgeometryarray );
void			NGetBoundingSphereCenterFromGeometryArray( NVEC3	*pcenter, const NARRAY *pgeometryarray );
void			NGetBoundingSphereFromGeometry( NBOUNDINGSPHERE *pbs, const NGEOMETRY *pgeom);
void			NGetBoundingSphereCenterFromGeometry(NVEC3	*pcenter, const NGEOMETRY *pgeom);
// void			NSetGeometryAABBSize( NGEOMETRY *pgeom,const NVEC3 *pnewsize,const NVEC3 *panchor );
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NGEOMETRY_H 


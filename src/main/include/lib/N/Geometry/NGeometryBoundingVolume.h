#ifndef __NGEOMETRY_BOUNDINGVOLUME_H
#define __NGEOMETRY_BOUNDINGVOLUME_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NGeometryBoundingVolume.h							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/NMatrix.h"
#include "lib/N/Containers/NArray.h"
#include "lib/N/Render/Accumulator/NAccumulator.h"
#include "lib/N/Geometry/NGeometry.h"
#include "lib/N/NBoundingVolumes.h"

#ifdef __cplusplus
extern "C"
{
#endif
    // --------------------------------------------------------------------------------------------------------------------------------------------------------------

    void NGetGeometryAABB(const NGEOMETRY *pgeom, NAABB *paabb);
    void NGetGeometryArrayAABB(NAABB *paabb, const NARRAY *pgeomarray);
    inline void NGetGeometryAABBSize(const NGEOMETRY *pgeom, NVEC3 *psize)
    {
        NAABB aabb;
        NGetGeometryAABB(pgeom, &aabb);
        NGetAABBSize(psize, &aabb);
    }
    void NGetBoundingSphereFromGeometryArray(NBOUNDINGSPHERE *pbs, const NARRAY *pgeometryarray);
    void NGetBoundingSphereCenterFromGeometryArray(NVEC3 *pcenter, const NARRAY *pgeometryarray);
    void NGetBoundingSphereFromGeometry(NBOUNDINGSPHERE *pbs, const NGEOMETRY *pgeom);
    void NGetBoundingSphereCenterFromGeometry(NVEC3 *pcenter, const NGEOMETRY *pgeom);
// void			NSetGeometryAABBSize( NGEOMETRY *pgeom,const NVEC3 *pnewsize,const NVEC3 *panchor );
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NGEOMETRY_H

#ifndef __NGEOMETRY_VERTEXTRANSFORMATIONS_H
#define __NGEOMETRY_VERTEXTRANSFORMATIONS_H

#include "NGeometry.h"

#ifdef __cplusplus
extern "C"
{
#endif

// --------------------------------------------------------------------------------------------------------------------------------------------------------------
void	NMulGeometryVertexByMatrix3x3(NGEOMETRY *pgeom,const NMATRIX *pmatrix);
void	NMulGeometryVertexByMatrix(NGEOMETRY *pgeom,const NMATRIX *pmatrix);
void	NShiftGeometry(NGEOMETRY *pgeom,const NVEC3 *pshift);
void	NShiftGeometryf(NGEOMETRY *pgeom,const Nf32 x, const Nf32 y, const Nf32 z);
void	NResetGeometryCenterf(NGEOMETRY *pgeom, const Nf32 anchorx, const Nf32 anchory, const Nf32 anchorz );
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NGEOMETRY_VERTEXTRANSFORMATIONS_H 


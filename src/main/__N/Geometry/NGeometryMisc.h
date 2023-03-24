#ifndef __NGEOMETRY_MISC_H
#define __NGEOMETRY_MISC_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **										NGeometryMisc.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NType.h"
#include "../NRectf32.h"
#include "../Geometry/NGeometry.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

void NSetGeometryQuadUVs( NGEOMETRY *pgeom, const Nu32 first_quadvertex_index,const NVERTEX_DATA_ENUM vertex_data_texcoordset, const NRECTf32 *pUVrect, const Nbool cw_building );

void NSetGeometryVertexRangeColor(NGEOMETRY *pgeom,const NVERTEX_DATA_ENUM vertex_data_colorset, const Nu32 first_vertex, const Nu32 vertex_number,const NCOLOR *pcolor);
void NSetGeometryVertexRangeColorf(NGEOMETRY *pgeom,const NVERTEX_DATA_ENUM vertex_data_colorset, const Nu32 first_vertex, const Nu32 vertex_number,const Nf32 r, const Nf32 g, const Nf32 b, const Nf32 a);
void NSetGeometryVertexRangeColorRange(NGEOMETRY *pgeom,const NVERTEX_DATA_ENUM vertex_data_colorset, const Nu32 first_vertex, const Nu32 vertex_number,const NCOLOR *pfirstcolor, const Nu32 color_number );

void NModulateGeometryVertexRangeColor(NGEOMETRY *pgeom,const NVERTEX_DATA_ENUM vertex_data_colorset, const Nu32 first_vertex, const Nu32 vertex_number,const NCOLOR *pmodcolor);
void NModulateGeometryVertexRangeAlpha(NGEOMETRY *pgeom,const NVERTEX_DATA_ENUM vertex_data_colorset, const Nu32 first_vertex, const Nu32 vertex_number,const Nf32 modalpha);

Nu32 NComputeTriangleNormal(NVEC3 *pnormal, const NVEC3 *p0,const NVEC3 *p1,const NVEC3 *p2 );
inline Nu32 NComputeGeometryTriangleNormal(NVEC3 *pnormal, const NGEOMETRY *pgeom, const Nu32 triangle_index )
{
	NErrorIf(!pnormal ||!pgeom ,NERROR_NULL_POINTER);
	NErrorIf(triangle_index >= pgeom->Mesh.PrimitiveArray.Size, NERROR_GEOMETRY_OUTOFRANGE_PRIMITIVEID);
	NTRIANGLE	*ptri	= (NTRIANGLE*)NGetArrayPtr(&pgeom->Mesh.PrimitiveArray,triangle_index);
	return NComputeTriangleNormal( pnormal,(NVEC3*)NGetArrayPtr(&pgeom->Mesh.VertexArray,ptri->i0),(NVEC3*)NGetArrayPtr(&pgeom->Mesh.VertexArray,ptri->i1),(NVEC3*)NGetArrayPtr(&pgeom->Mesh.VertexArray,ptri->i2) );
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif  // __NGEOMETRY_MISC_H 


#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Containers/NArray.h"
#include "NgeometryVertexTransformations.h"
#include "NGeometryBoundingVolume.h"

extern NGEOMETRY_SPECS NGeometrySpecs[NGEOMETRY_FORMAT_ENUM_SIZE];

// Geometry shape manipulations
// Be aware thats all these transformations are definitive
// GEOMETRY LOCAL BUILDING TRANSFORMATIONS
// "Local Building ?" that means these transformations are absolute and definitive.
// They change permanently Vertex positions !!

void NShiftGeometry(NGEOMETRY *pgeom, const NVEC3 *pshift)
{
	Nu32 stride = NGetArrayElementSize(&pgeom->Mesh.VertexArray);
	NVEC3 *pv = (NVEC3 *)NGetFirstArrayPtr(&pgeom->Mesh.VertexArray);
	for (Nu32 i = NGetArraySize(&pgeom->Mesh.VertexArray); i != 0; i--, pv = (NVEC3 *)((NBYTE *)pv + stride))
	{
		NVec3AddTo(pv, pshift);
	}
}

void NShiftGeometryf(NGEOMETRY *pgeom, const Nf32 x, const Nf32 y, const Nf32 z)
{
	Nu32 stride = NGetArrayElementSize(&pgeom->Mesh.VertexArray);
	NVEC3 *pv = (NVEC3 *)NGetFirstArrayPtr(&pgeom->Mesh.VertexArray);
	for (Nu32 i = NGetArraySize(&pgeom->Mesh.VertexArray); i != 0; i--, pv = (NVEC3 *)((NBYTE *)pv + stride))
	{
		pv->x += x;
		pv->y += y;
		pv->z += z;
	}
}

void NMulGeometryVertexByMatrix(NGEOMETRY *pgeom, const NMATRIX *pmatrix)
{
	Nu32 stride = NGetArrayElementSize(&pgeom->Mesh.VertexArray);
	NVEC3 *pv = (NVEC3 *)NGetFirstArrayPtr(&pgeom->Mesh.VertexArray);
	for (Nu32 i = NGetArraySize(&pgeom->Mesh.VertexArray); i != 0; i--, pv = (NVEC3 *)((NBYTE *)pv + stride))
	{
		NMulVector3ByMatrix(pv, pmatrix, pv);
	}
}

void NMulGeometryVertexByMatrix3x3(NGEOMETRY *pgeom, const NMATRIX *pmatrix)
{
	Nu32 stride = NGetArrayElementSize(&pgeom->Mesh.VertexArray);
	NVEC3 *pv = (NVEC3 *)NGetFirstArrayPtr(&pgeom->Mesh.VertexArray);
	for (Nu32 i = NGetArraySize(&pgeom->Mesh.VertexArray); i != 0; i--, pv = (NVEC3 *)((NBYTE *)pv + stride))
	{
		NMulVector3ByMatrix3(pv, pmatrix, pv);
	}
}

// ------------------------------------------------------------------------------------------
// NResetGeometryCenterf
// ------------------------------------------------------------------------------------------
// Description :
// To replace a geometry around 0,0,0. According to the anchorx,anchory,anchorz parameters
// ... object will be anchored by this point ( which is relative to the object AABB )to be place at the 0,0,0 origin.)
// ... and all its vertex will be recalculated !
//	value for anchor x !
//		ANCHOR_LEFT
//		ANCHOR_MIDWIDTH
//		ANCHOR_RIGHT
//	value for anchor y !
//		ANCHOR_TOP
//		ANCHOR_MIDHEIGHT
//		ANCHOR_BOTTOM
//	value for anchor z !
//		ANCHOR_BACK
//		ANCHOR_MIDDEPTH
//		ANCHOR_FRONT

// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NResetGeometryCenterf(NGEOMETRY *pgeom, const Nf32 anchorx, const Nf32 anchory, const Nf32 anchorz)
{
	NAABB aabb;
	NVEC3 size;
	NVEC3 origin;

	NGetGeometryAABB(pgeom, &aabb);
	NGetAABBSize(&size, &aabb);
	origin.x = aabb.fXMin + size.x * anchorx;
	origin.y = aabb.fYMax + size.y * anchory;
	origin.z = aabb.fZMin + size.z * anchorz;

	Nu32 stride = NGetArrayElementSize(&pgeom->Mesh.VertexArray);
	NVEC3 *pv = (NVEC3 *)NGetFirstArrayPtr(&pgeom->Mesh.VertexArray);
	for (Nu32 i = NGetArraySize(&pgeom->Mesh.VertexArray); i != 0; i--, pv = (NVEC3 *)((NBYTE *)pv + stride))
	{
		pv->x -= origin.x;
		pv->y -= origin.y;
		pv->z -= origin.z;
	}
}

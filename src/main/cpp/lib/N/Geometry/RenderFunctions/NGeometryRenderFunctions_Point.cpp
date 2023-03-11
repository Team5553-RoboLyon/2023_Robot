#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Geometry/NGeometry.h"

#include "NGeometryRenderFunctions.h"

// ------------------------------------------------------------------------------------------
// NRenderGeometry_Point_TU0_V3_POINT
// ------------------------------------------------------------------------------------------
// Description :
//	Specific Rendering function, linked with a geometry.
//	This function works only with:
//			POINT based geometry WITHOUT TEXTURE and Without COLOR.
// ------------------------------------------------------------------------------------------
// In  :
//				pgeom		a valid pointer on a geometry.
//							Usually 'pgeom' points to the geometry which the function
//							is linked with.
// Out :
//				nothing
// ------------------------------------------------------------------------------------------
void NRenderGeometry_Point_TU0_V3_POINT(const NGEOMETRY* pgeom)
{
	// First Vertex
	NVERTEX_V3		*pvertex	= (NVERTEX_V3*)pgeom->Mesh.VertexArray.pFirst;
	Nu32			stride		= pgeom->Mesh.VertexArray.ElementSize;

	// OPEN GL DRAW
	glVertexPointer(3,GL_FLOAT,stride,(Nf32*)&pvertex->Position_3f );

	// 16 bits Version for IOS and 32 bits one for WINDOWS
#ifdef _NWINDOWS
	glDrawElements(GL_POINTS,pgeom->Mesh.PrimitiveArray.Size,GL_UNSIGNED_INT,(Nu32*)pgeom->Mesh.PrimitiveArray.pFirst );
#endif

#if defined  _NIOS || defined _NANDROID
	glDrawElements(GL_POINTS,pgeom->Mesh.PrimitiveArray.Size,GL_UNSIGNED_SHORT,(Nu16*)pgeom->Mesh.PrimitiveArray.pFirst );
#endif
}

// ------------------------------------------------------------------------------------------
// NRenderGeometry_ColorPoint_TU0_V3C4_POINT
// ------------------------------------------------------------------------------------------
// Description :
//	Specific Rendering function, linked with a geometry.
//	This function works only with:
//			POINT based geometry WITHOUT TEXTURE and COLOR.
// ------------------------------------------------------------------------------------------
// In  :
//				pgeom		a valid pointer on a geometry.
//							Usually 'pgeom' points to the geometry which the function
//							is linked with.
// Out :
//				nothing
// ------------------------------------------------------------------------------------------
void NRenderGeometry_ColorPoint_TU0_V3C4_POINT(const NGEOMETRY* pgeom)
{
	// First Vertex
	NVERTEX_V3C4	*pvertex	= (NVERTEX_V3C4*)pgeom->Mesh.VertexArray.pFirst;
	Nu32			stride		= pgeom->Mesh.VertexArray.ElementSize;

	// OPEN GL DRAW
	glVertexPointer(3,GL_FLOAT,stride,(Nf32*)&pvertex->Position_3f );
	glColorPointer(4,GL_FLOAT,stride,(Nf32*)&pvertex->Color0_4f);

	// 16 bits Version for IOS and 32 bits one for WINDOWS
#ifdef _NWINDOWS
	glDrawElements(GL_POINTS,pgeom->Mesh.PrimitiveArray.Size,GL_UNSIGNED_INT,(Nu32*)pgeom->Mesh.PrimitiveArray.pFirst );
#endif

#if defined _NIOS || defined _NANDROID
	glDrawElements(GL_POINTS,pgeom->Mesh.PrimitiveArray.Size,GL_UNSIGNED_SHORT,(Nu16*)pgeom->Mesh.PrimitiveArray.pFirst );
#endif
}

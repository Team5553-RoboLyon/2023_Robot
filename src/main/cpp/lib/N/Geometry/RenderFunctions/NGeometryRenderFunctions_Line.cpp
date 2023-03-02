#include "../../NCStandard.h"
#include "../../NType.h"
#include "../NGeometry.h"
#include "NGeometryRenderFunctions.h"

// ------------------------------------------------------------------------------------------
// NRenderGeometry_TexturedLine_TUT1_V3C4T2_LINE
// ------------------------------------------------------------------------------------------
// Description :
//	Specific Rendering function, linked with a geometry.
//	This function works only with:
//			LINE based geometry WITH A TEXTURE.
// ------------------------------------------------------------------------------------------
// In  :
//				pgeom		a valid pointer on a geometry.
//							Usually 'pgeom' points to the geometry which the function
//							is linked with.
// Out :
//				nothing
// ------------------------------------------------------------------------------------------
void NRenderGeometry_TexturedLine_TUT1_V3C4T2_LINE(const NGEOMETRY* pgeom)
{
	// First Vertex
	NVERTEX_V3C4T2	*pvertex	= (NVERTEX_V3C4T2 *)pgeom->Mesh.VertexArray.pFirst;
	Nu32			stride		= pgeom->Mesh.VertexArray.ElementSize;

	// Attach texture
	glTexCoordPointer(2,GL_FLOAT,stride,(Nf32*)&pvertex->TexCoord0_2f);
	glBindTexture(GL_TEXTURE_2D,pgeom->Appearance.TT.TextureUnitT0.pTexture->GL_Texture);

	// OPEN GL DRAW
	glVertexPointer(3,GL_FLOAT,stride,(Nf32*)&pvertex->Position_3f );
	glColorPointer(4,GL_FLOAT,stride,(Nf32*)&pvertex->Color0_4f);

	// 16 bits Version for IOS and 32 bits one for WINDOWS
#ifdef _NWINDOWS
	glDrawElements(GL_LINES,pgeom->Mesh.PrimitiveArray.Size*2,GL_UNSIGNED_INT,(Nu32*)pgeom->Mesh.PrimitiveArray.pFirst );
#endif

#if defined  _NIOS || defined _NANDROID
	glDrawElements(GL_LINES,pgeom->Mesh.PrimitiveArray.Size*2,GL_UNSIGNED_SHORT,(Nu16*)pgeom->Mesh.PrimitiveArray.pFirst );
#endif
}

// ------------------------------------------------------------------------------------------
// NRenderGeometry_ColorLine_TU0_V3C4_LINE
// ------------------------------------------------------------------------------------------
// Description :
//	Specific Rendering function, linked with a geometry.
//	This function works only with:
//			LINES based geometry WITHOUT TEXTURE.
// ------------------------------------------------------------------------------------------
// In  :
//				pgeom		a valid pointer on a geometry.
//							Usually 'pgeom' points to the geometry which the function
//							is linked with.
// Out :
//				nothing
// ------------------------------------------------------------------------------------------
void NRenderGeometry_ColorLine_TU0_V3C4_LINE(const NGEOMETRY* pgeom)
{
	// First Vertex
	NVERTEX_V3C4	*pvertex	= (NVERTEX_V3C4 *)pgeom->Mesh.VertexArray.pFirst;
	Nu32			stride		= pgeom->Mesh.VertexArray.ElementSize;

	// OPEN GL DRAW
	glVertexPointer(3,GL_FLOAT,stride,(Nf32*)&pvertex->Position_3f );
	glColorPointer(4,GL_FLOAT,stride,(float*)&pvertex->Color0_4f );

	// version 16 bits pour IOS et 32 bits pour WINDOWS
#ifdef _NWINDOWS
	glDrawElements(GL_LINES,pgeom->Mesh.PrimitiveArray.Size*2,GL_UNSIGNED_INT,(Nu32*)pgeom->Mesh.PrimitiveArray.pFirst );
#endif

#if defined  _NIOS || defined _NANDROID
	glDrawElements(GL_LINES,pgeom->Mesh.PrimitiveArray.Size*2,GL_UNSIGNED_SHORT,(Nu16*)pgeom->Mesh.PrimitiveArray.pFirst );
#endif
}

// ------------------------------------------------------------------------------------------
// NRenderGeometry_Line_TU0_V3_LINE
// ------------------------------------------------------------------------------------------
// Description :
//	Specific Rendering function, linked with a geometry.
//	This function works only with:
//			LINES based geometry WITHOUT TEXTURE.
// ------------------------------------------------------------------------------------------
// In  :
//				pgeom		a valid pointer on a geometry.
//							Usually 'pgeom' points to the geometry which the function
//							is linked with.
// Out :
//				nothing
// ------------------------------------------------------------------------------------------
void NRenderGeometry_Line_TU0_V3_LINE(const NGEOMETRY* pgeom)
{
	// First Vertex
	NVERTEX_V3		*pvertex	= (NVERTEX_V3 *)pgeom->Mesh.VertexArray.pFirst;
	Nu32			stride		= pgeom->Mesh.VertexArray.ElementSize;

	// OPEN GL DRAW
	glVertexPointer(3,GL_FLOAT,stride,(Nf32*)&pvertex->Position_3f );

	// version 16 bits pour IOS et 32 bits pour WINDOWS
#ifdef _NWINDOWS
	glDrawElements(GL_LINES,pgeom->Mesh.PrimitiveArray.Size*2,GL_UNSIGNED_INT,(Nu32*)pgeom->Mesh.PrimitiveArray.pFirst );
#endif

#if defined  _NIOS || defined _NANDROID
	glDrawElements(GL_LINES,pgeom->Mesh.PrimitiveArray.Size*2,GL_UNSIGNED_SHORT,(Nu16*)pgeom->Mesh.PrimitiveArray.pFirst );
#endif
}




#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Geometry/NGeometry.h"

#include "NGeometryRenderFunctions.h"


// ------------------------------------------------------------------------------------------
// NRenderGeometry_Add_Combine_TUT2_V3C4T2_TRIANGLE
// ------------------------------------------------------------------------------------------
// Description :
//	Specific Rendering function, linked with a geometry.
//	This function works only with:
//			TRIANGLE based geometry WITH TWO TEXTURES.
//	
//	The second texture is going to be combine with the first one by adding its color components
//	... simple actually ! 
// ------------------------------------------------------------------------------------------
// In  :
//				pgeom		a valid pointer on a geometry.
//							Usually 'pgeom' points to the geometry which the function
//							is linked with.
// Out :
//				nothing
// ------------------------------------------------------------------------------------------
void NRenderGeometry_Add_Combine_TUT2_V3C4T2_TRIANGLE(const NGEOMETRY* pgeom)
{
	// First Vertex
	NVERTEX_V3C4T2	*pvertex	= (NVERTEX_V3C4T2 *)pgeom->Mesh.VertexArray.pFirst;
	GLsizei			stride		= pgeom->Mesh.VertexArray.ElementSize;

	// Vertex position and Vertex colors
	glVertexPointer(3,GL_FLOAT,stride,(Nf32*)&pvertex->Position_3f );
	glColorPointer(4,GL_FLOAT,stride,(Nf32*)&pvertex->Color0_4f);

	// Attach the first texture and define it's texture coordinates
	glBindTexture(GL_TEXTURE_2D,pgeom->Appearance.TT.TextureUnitT0.pTexture->GL_Texture);
	glTexCoordPointer(2,GL_FLOAT,stride,(Nf32*)&pvertex->TexCoord0_2f);

	// Attach the second texture and define it's texture coordinates
	//glActiveTextureARB(GL_TEXTURE1_ARB);
	glActiveTexture(GL_TEXTURE1);

	glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D,pgeom->Appearance.TT.TextureUnitT1.pTexture->GL_Texture);

	//glClientActiveTextureARB ( GL_TEXTURE1_ARB );
	glClientActiveTexture( GL_TEXTURE1 );

	glEnableClientState ( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer(2,GL_FLOAT,stride,(Nf32*)&pvertex->TexCoord0_2f);

	// Define the way to combine the 2 textures
	glTexEnvi(GL_TEXTURE_ENV,	GL_TEXTURE_ENV_MODE,	GL_COMBINE		);
	glTexEnvi(GL_TEXTURE_ENV,	GL_COMBINE_RGB,			GL_ADD			); // instead of "GL_ADD_SIGNED"

	glTexEnvi(GL_TEXTURE_ENV,	GL_SRC0_RGB,			GL_PREVIOUS		);
	glTexEnvi(GL_TEXTURE_ENV,	GL_OPERAND0_RGB,		GL_SRC_COLOR	);

	glTexEnvi(GL_TEXTURE_ENV,	GL_SRC1_RGB,			GL_TEXTURE1		);
	glTexEnvi(GL_TEXTURE_ENV,	GL_OPERAND1_RGB,		GL_SRC_COLOR	);

	glTexEnvi(GL_TEXTURE_ENV,	GL_COMBINE_ALPHA,		GL_REPLACE		);
	glTexEnvi(GL_TEXTURE_ENV,	GL_SRC0_ALPHA,			GL_TEXTURE1		);
	glTexEnvi(GL_TEXTURE_ENV,	GL_OPERAND0_ALPHA,		GL_SRC_ALPHA	);

	// OPEN GL DRAW
	// 16 bits Version for IOS and 32 bits one for WINDOWS and ??? for ANDROID
#ifdef _NWINDOWS
	glDrawElements(GL_TRIANGLES,pgeom->Mesh.PrimitiveArray.Size*3,GL_UNSIGNED_INT,(Nu32*)pgeom->Mesh.PrimitiveArray.pFirst );
#endif

#if defined  _NIOS || defined _NANDROID
	glDrawElements(GL_TRIANGLES,pgeom->Mesh.PrimitiveArray.Size*3,GL_UNSIGNED_SHORT,(Nu16*)pgeom->Mesh.PrimitiveArray.pFirst);
#endif

	// Stop using 2 textures ( because the next geometry may use only one !!!)
	// glActiveTextureARB(GL_TEXTURE1_ARB); ... not necessary because it was already done some lines above
	glDisable ( GL_TEXTURE_2D );
	glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
	//glDisableClientState ( GL_COLOR_ARRAY );

	// To define Texture Unit 0 as the default One for Future Texture Manipulation ...
// 	glActiveTextureARB ( GL_TEXTURE0_ARB );
// 	glClientActiveTextureARB ( GL_TEXTURE0_ARB );
	glActiveTexture( GL_TEXTURE0 );
	glClientActiveTexture( GL_TEXTURE0 );

}

// ------------------------------------------------------------------------------------------
// NRenderGeometry_Add_Signed_Combine_TUT2_V3C4T2_TRIANGLE
// ------------------------------------------------------------------------------------------
// Description :
//	Specific Rendering function, linked with a geometry.
//	This function works only with:
//			TRIANGLE based geometry WITH TWO TEXTURES.
//	
//	The second texture is going to be combine with the first one by adding or subtracting its color components ( from -0.5f to + 0.5f )
//	... simple actually ! 
// ------------------------------------------------------------------------------------------
// In  :
//				pgeom		a valid pointer on a geometry.
//							Usually 'pgeom' points to the geometry which the function
//							is linked with.
// Out :
//				nothing
// ------------------------------------------------------------------------------------------
void NRenderGeometry_Add_Signed_Combine_TUT2_V3C4T2_TRIANGLE(const NGEOMETRY* pgeom)
{
	// First Vertex
	NVERTEX_V3C4T2	*pvertex	= (NVERTEX_V3C4T2 *)pgeom->Mesh.VertexArray.pFirst;
	GLsizei			stride		= pgeom->Mesh.VertexArray.ElementSize;

	// Vertex position and Vertex colors
	glVertexPointer(3,GL_FLOAT,stride,(Nf32*)&pvertex->Position_3f );
	glColorPointer(4,GL_FLOAT,stride,(Nf32*)&pvertex->Color0_4f);

	// Attach the first texture and define it's texture coordinates
	glBindTexture(GL_TEXTURE_2D,pgeom->Appearance.TT.TextureUnitT0.pTexture->GL_Texture);
	glTexCoordPointer(2,GL_FLOAT,stride,(Nf32*)&pvertex->TexCoord0_2f);
	
	// Attach the second texture and define it's texture coordinates
	
	//glActiveTextureARB(GL_TEXTURE1_ARB);
	glActiveTexture(GL_TEXTURE1);
	
	glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D,pgeom->Appearance.TT.TextureUnitT1.pTexture->GL_Texture);

	//glClientActiveTextureARB ( GL_TEXTURE1_ARB );
	glClientActiveTexture( GL_TEXTURE1 );
	
	glEnableClientState ( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer(2,GL_FLOAT,stride,(Nf32*)&pvertex->TexCoord0_2f);

	// Define the way to combine the 2 textures
	glTexEnvi(GL_TEXTURE_ENV,	GL_TEXTURE_ENV_MODE,	GL_COMBINE		);
	glTexEnvi(GL_TEXTURE_ENV,	GL_COMBINE_RGB,			GL_ADD_SIGNED	); // instead of "GL_ADD"
	
	glTexEnvi(GL_TEXTURE_ENV,	GL_SRC0_RGB,			GL_PREVIOUS		);
	glTexEnvi(GL_TEXTURE_ENV,	GL_OPERAND0_RGB,		GL_SRC_COLOR	);
	
	glTexEnvi(GL_TEXTURE_ENV,	GL_SRC1_RGB,			GL_TEXTURE1		);
	glTexEnvi(GL_TEXTURE_ENV,	GL_OPERAND1_RGB,		GL_SRC_COLOR	);


	glTexEnvi(GL_TEXTURE_ENV,	GL_COMBINE_ALPHA,		GL_REPLACE		);
	glTexEnvi(GL_TEXTURE_ENV,	GL_SRC0_ALPHA,		GL_TEXTURE1		);
	glTexEnvi(GL_TEXTURE_ENV,	GL_OPERAND0_ALPHA,		GL_SRC_ALPHA	);

	// OPEN GL DRAW
	// 16 bits Version for IOS and 32 bits one for WINDOWS and ??? for ANDROID
#ifdef _NWINDOWS
	glDrawElements(GL_TRIANGLES,pgeom->Mesh.PrimitiveArray.Size*3,GL_UNSIGNED_INT,(Nu32*)pgeom->Mesh.PrimitiveArray.pFirst );
#endif
#if defined  _NIOS || defined _NANDROID
	glDrawElements(GL_TRIANGLES,pgeom->Mesh.PrimitiveArray.Size*3,GL_UNSIGNED_SHORT,(Nu16*)pgeom->Mesh.PrimitiveArray.pFirst);
#endif

	// Stop using 2 textures ( because the next geometry may use only one !!!)
	// glActiveTextureARB(GL_TEXTURE1_ARB); ... not necessary because it was already done some lines above
	glDisable ( GL_TEXTURE_2D );
	glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
	//glDisableClientState ( GL_COLOR_ARRAY );
	
	// To define Texture Unit 0 as the default One for Future Texture Manipulation ...
	
	//glActiveTextureARB ( GL_TEXTURE0_ARB );
	//glClientActiveTextureARB ( GL_TEXTURE0_ARB );
	glActiveTexture( GL_TEXTURE0 );
	glClientActiveTexture( GL_TEXTURE0 );
}

// ------------------------------------------------------------------------------------------
// NRenderGeometry_TexturedTriangle_TUT1_V3C4T2_TRIANGLE
// ------------------------------------------------------------------------------------------
// Description :
//	Specific Rendering function, linked with a geometry.
//	This function works only with:
//			TRIANGLE based geometry WITH A TEXTURE.
// ------------------------------------------------------------------------------------------
// In  :
//				pgeom		a valid pointer on a geometry.
//							Usually 'pgeom' points to the geometry which the function
//							is linked with.
// Out :
//				nothing
// ------------------------------------------------------------------------------------------
void NRenderGeometry_TexturedTriangle_TUT1_V3C4T2_TRIANGLE(const NGEOMETRY* pgeom)
{
	// First Vertex
	NVERTEX_V3C4T2	*pvertex	= (NVERTEX_V3C4T2 *)pgeom->Mesh.VertexArray.pFirst;
	GLsizei			stride		= pgeom->Mesh.VertexArray.ElementSize;

	// Attach texture
	glBindTexture(GL_TEXTURE_2D,pgeom->Appearance.TT.TextureUnitT0.pTexture->GL_Texture);
	glTexCoordPointer(2,GL_FLOAT,stride,(Nf32*)&pvertex->TexCoord0_2f);
	glVertexPointer(3,GL_FLOAT,stride,(Nf32*)&pvertex->Position_3f );
	glColorPointer(4,GL_FLOAT,stride,(Nf32*)&pvertex->Color0_4f);

	// OPEN GL DRAW
    // 16 bits Version for IOS and 32 bits one for WINDOWS
#ifdef _NWINDOWS
	glDrawElements(GL_TRIANGLES,pgeom->Mesh.PrimitiveArray.Size*3,GL_UNSIGNED_INT,(Nu32*)pgeom->Mesh.PrimitiveArray.pFirst );
#endif
#if defined  _NIOS || defined _NANDROID
	glDrawElements(GL_TRIANGLES,pgeom->Mesh.PrimitiveArray.Size*3,GL_UNSIGNED_SHORT,(Nu16*)pgeom->Mesh.PrimitiveArray.pFirst);
#endif
}

// ------------------------------------------------------------------------------------------
// NRender_GeometryFlatTriangles
// ------------------------------------------------------------------------------------------
// Description :
//	Specific Rendering function, linked with a geometry.
//	This function works only with:
//			TRIANGLE based geometry WITHOUT TEXTURE.
// ------------------------------------------------------------------------------------------
// In  :
//				pgeom		a valid pointer on a geometry.
//							Usually 'pgeom' points to the geometry which the function
//							is linked with.
// Out :
//				nothing
// ------------------------------------------------------------------------------------------
void NRenderGeometry_ColorFlatTriangle_TU0_V3C4_TRIANGLE(const NGEOMETRY* pgeom)
{
	// First Vertex
	NVERTEX_V3C4	*pvertex	= (NVERTEX_V3C4 *)pgeom->Mesh.VertexArray.pFirst;
	GLsizei			stride		= pgeom->Mesh.VertexArray.ElementSize;

	// OPEN GL DRAW
	glVertexPointer(3,GL_FLOAT,stride,(Nf32*)&pvertex->Position_3f );
	glColorPointer(4,GL_FLOAT,stride,(Nf32*)&pvertex->Color0_4f);

    // version 16 bits pour IOS et 32 bits pour WINDOWS
#ifdef _NWINDOWS
	glDrawElements(GL_TRIANGLES,pgeom->Mesh.PrimitiveArray.Size*3,GL_UNSIGNED_INT,(Nu32*)pgeom->Mesh.PrimitiveArray.pFirst );
#endif

#if defined  _NIOS || defined _NANDROID
	glDrawElements(GL_TRIANGLES,pgeom->Mesh.PrimitiveArray.Size*3,GL_UNSIGNED_SHORT,(Nu16*)pgeom->Mesh.PrimitiveArray.pFirst);
#endif
}

// ------------------------------------------------------------------------------------------
// NRender_GeometryFlatTriangles (without color vertex)
// ------------------------------------------------------------------------------------------
// Description :
//	Specific Rendering function, linked with a geometry.
//	This function works only with:
//			TRIANGLE based geometry WITHOUT TEXTURE.
// ------------------------------------------------------------------------------------------
// In  :
//				pgeom		a valid pointer on a geometry.
//							Usually 'pgeom' points to the geometry which the function
//							is linked with.
// Out :
//				nothing
// ------------------------------------------------------------------------------------------
void NRenderGeometry_FlatTriangle_TU0_V3_TRIANGLE(const NGEOMETRY* pgeom)
{
	// First Vertex
	NVERTEX_V3		*pvertex	= (NVERTEX_V3 *)pgeom->Mesh.VertexArray.pFirst;
	GLsizei			stride		= pgeom->Mesh.VertexArray.ElementSize;

	// OPEN GL DRAW
	glVertexPointer(3,GL_FLOAT,stride,(Nf32*)&pvertex->Position_3f );

	// version 16 bits pour IOS et 32 bits pour WINDOWS
#ifdef _NWINDOWS
	glDrawElements(GL_TRIANGLES,pgeom->Mesh.PrimitiveArray.Size*3,GL_UNSIGNED_INT,(Nu32*)pgeom->Mesh.PrimitiveArray.pFirst );
#endif

#if defined  _NIOS || defined _NANDROID
	glDrawElements(GL_TRIANGLES,pgeom->Mesh.PrimitiveArray.Size*3,GL_UNSIGNED_SHORT,(Nu16*)pgeom->Mesh.PrimitiveArray.pFirst);
#endif
}

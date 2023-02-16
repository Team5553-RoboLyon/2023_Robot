#ifdef _DEBUG
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "NGeometryDebugChecking.h"
#include "../Render/Accumulator/NAccumulator.h"
// ------------------------------------------------------------------------------------------
// NDebugChecking_ ...
// ------------------------------------------------------------------------------------------
// Description :
//		Because there is no test inside geometry rendering function. It's important to check,
//		at the right time (i.e. just before accumulator insert), the geometry structure is the right one,
//		and all the needed geometry data are here, into the geometry...
//		It's really important to perform this test before accumulator push. Thanks to this early test,
//		its easy to identify quickly the geometry owner and fix potential problems !
//		Note: these tests could  be done inside the geometry rendering function, but, in that way it would be very difficult 
//		to debug and identify the geometry owner ... So, let's do like this !
// ------------------------------------------------------------------------------------------
// In	:
//			NGEOMETRY	*pgeom					The geometry to check
// Out :
//
// ------------------------------------------------------------------------------------------
void NDebugCheckingGeometry_Point_TU0_V3_POINT(const NGEOMETRY *pgeom)
{
	// ! Format consistency checking !
	NErrorIf( NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)		!= NVERTEX_FORMAT_V3,		NERROR_GEOMETRY_UNEXPECTED_VERTEX_FORMAT );
	NErrorIf( NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(pgeom->Core.IDS)	!= NPRIMITIVE_FORMAT_POINT,	NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_FORMAT );
	NErrorIf( NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS)	!= NAPPEARANCE_FORMAT_NULL,	NERROR_GEOMETRY_UNEXPECTED_APPEARANCE_FORMAT );
	NErrorIf( NGetGeometryTextureUnitNumber(pgeom)				!=0,						NERROR_GEOMETRY_UNEXPECTED_TEXTUREUNIT_NUMBER );

	// ! Array Size checking !
	NErrorIf(pgeom->Mesh.VertexArray.ElementSize != sizeof(NVERTEX_V3),		NERROR_GEOMETRY_UNEXPECTED_VERTEX_STRUCTURE_SIZE);
	NErrorIf(pgeom->Mesh.PrimitiveArray.ElementSize != sizeof(NPOINT),		NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_STRUCTURE_SIZE);

	// ! Accumulator Consistency Checking !
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_TEXTURES), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_BLEND), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_LIGHT), NERROR_ACCUMULATOR_INCONSISTENCY );
}
void NDebugCheckingGeometry_ColorPoint_TU0_V3C4_POINT(const NGEOMETRY *pgeom)
{
	// ! Format consistency checking !
	NErrorIf( NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)		!= NVERTEX_FORMAT_V3C4,		NERROR_GEOMETRY_UNEXPECTED_VERTEX_FORMAT );
	NErrorIf( NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(pgeom->Core.IDS)	!= NPRIMITIVE_FORMAT_POINT,	NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_FORMAT );
	NErrorIf( NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS)	!= NAPPEARANCE_FORMAT_NULL,	NERROR_GEOMETRY_UNEXPECTED_APPEARANCE_FORMAT );
	NErrorIf( NGetGeometryTextureUnitNumber(pgeom)				!=0,						NERROR_GEOMETRY_UNEXPECTED_TEXTUREUNIT_NUMBER );
	
	// ! Array Size checking !
	NErrorIf(pgeom->Mesh.VertexArray.ElementSize != sizeof(NVERTEX_V3C4),	NERROR_GEOMETRY_UNEXPECTED_VERTEX_STRUCTURE_SIZE);
	NErrorIf(pgeom->Mesh.PrimitiveArray.ElementSize != sizeof(NPOINT),		NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_STRUCTURE_SIZE);

	// ! Accumulator Consistency Checking !
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_TEXTURES), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_BLEND), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_LIGHT), NERROR_ACCUMULATOR_INCONSISTENCY );

}
void NDebugCheckingGeometry_Line_TU0_V3_LINE(const NGEOMETRY *pgeom)
{
	// !Format consistency checking !
	NErrorIf(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)		!= NVERTEX_FORMAT_V3,		NERROR_GEOMETRY_UNEXPECTED_VERTEX_FORMAT);
	NErrorIf(NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(pgeom->Core.IDS)	!= NPRIMITIVE_FORMAT_LINE,	NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_FORMAT);
	NErrorIf(NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS)	!= NAPPEARANCE_FORMAT_NULL, NERROR_GEOMETRY_UNEXPECTED_APPEARANCE_FORMAT);
	NErrorIf(NGET_GEOMETRY_IDS_TEXTUREUNIT_FORMAT(pgeom->Core.IDS) != NTEXTUREUNIT_FORMAT_NULL,NERROR_GEOMETRY_UNEXPECTED_TEXTUREUNIT_FORMAT);
	NErrorIf(NGetGeometryTextureUnitNumber(pgeom)			!=0,						NERROR_GEOMETRY_UNEXPECTED_TEXTUREUNIT_NUMBER);

	// ! Array Size checking !
	NErrorIf(pgeom->Mesh.VertexArray.ElementSize != sizeof(NVERTEX_V3),				NERROR_GEOMETRY_UNEXPECTED_VERTEX_STRUCTURE_SIZE);
	NErrorIf(pgeom->Mesh.PrimitiveArray.ElementSize != sizeof(NLINE),				NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_STRUCTURE_SIZE);

	// ! Accumulator Consistency Checking !
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_TEXTURES), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_BLEND), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_LIGHT), NERROR_ACCUMULATOR_INCONSISTENCY );
}
void NDebugCheckingGeometry_ColorLine_TU0_V3C4_LINE(const NGEOMETRY *pgeom)
{
	// !Format consistency checking !
	NErrorIf(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)		!= NVERTEX_FORMAT_V3C4,		NERROR_GEOMETRY_UNEXPECTED_VERTEX_FORMAT);
	NErrorIf(NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(pgeom->Core.IDS)	!= NPRIMITIVE_FORMAT_LINE,	NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_FORMAT);
	NErrorIf(NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS)	!= NAPPEARANCE_FORMAT_NULL, NERROR_GEOMETRY_UNEXPECTED_APPEARANCE_FORMAT);
	NErrorIf(NGET_GEOMETRY_IDS_TEXTUREUNIT_FORMAT(pgeom->Core.IDS) != NTEXTUREUNIT_FORMAT_NULL,NERROR_GEOMETRY_UNEXPECTED_TEXTUREUNIT_FORMAT);
	NErrorIf(NGetGeometryTextureUnitNumber(pgeom)			!=0,						NERROR_GEOMETRY_UNEXPECTED_TEXTUREUNIT_NUMBER);

	// ! Array Size checking !
	NErrorIf(pgeom->Mesh.VertexArray.ElementSize != sizeof(NVERTEX_V3C4),				NERROR_GEOMETRY_UNEXPECTED_VERTEX_STRUCTURE_SIZE);
	NErrorIf(pgeom->Mesh.PrimitiveArray.ElementSize != sizeof(NLINE),					NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_STRUCTURE_SIZE);

	// ! Accumulator Consistency Checking !
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_TEXTURES), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_BLEND), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_LIGHT), NERROR_ACCUMULATOR_INCONSISTENCY );
}
void NDebugCheckingGeometry_TexturedLine_TUT1_V3C4T2_LINE(const NGEOMETRY *pgeom)
{
	// !Format consistency checking !
	NErrorIf(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)		!= NVERTEX_FORMAT_V3C4T2,	NERROR_GEOMETRY_UNEXPECTED_VERTEX_FORMAT );
	NErrorIf(NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(pgeom->Core.IDS)	!= NPRIMITIVE_FORMAT_LINE,	NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_FORMAT );
	NErrorIf(NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS)	!= NAPPEARANCE_FORMAT_TT,	NERROR_GEOMETRY_UNEXPECTED_APPEARANCE_FORMAT );
	NErrorIf(NGET_GEOMETRY_IDS_TEXTUREUNIT_FORMAT(pgeom->Core.IDS) != NTEXTUREUNIT_FORMAT_T,	NERROR_GEOMETRY_UNEXPECTED_TEXTUREUNIT_FORMAT );
	NErrorIf(NGetGeometryTextureUnitNumber(pgeom)			!=1,						NERROR_GEOMETRY_UNEXPECTED_TEXTUREUNIT_NUMBER );

	// ! Array Size checking !
	NErrorIf(pgeom->Mesh.VertexArray.ElementSize != sizeof(NVERTEX_V3C4T2),				NERROR_GEOMETRY_UNEXPECTED_VERTEX_STRUCTURE_SIZE);
	NErrorIf(pgeom->Mesh.PrimitiveArray.ElementSize != sizeof(NLINE),					NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_STRUCTURE_SIZE);

	// ! Accumulator Consistency Checking !
	NErrorIf(ISFLAG_OFF(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_TEXTURES), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_BLEND) && (!pgeom->Appearance.TT.pBlend), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_LIGHT) && (!pgeom->Appearance.TT.pMaterial), NERROR_ACCUMULATOR_INCONSISTENCY );

	// !A valid Texture is absolutely necessary !
	NErrorIf(!pgeom->Appearance.TT.TextureUnitT0.pTexture, NERROR_GEOMETRY_MISSING_TEXTURE);
	NErrorIf(pgeom->Appearance.TT.TextureUnitT1.pTexture, NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_FORMAT);
}
void NDebugCheckingGeometry_FlatTriangle_TU0_V3_TRIANGLE(const NGEOMETRY *pgeom)
{
	// !Format consistency checking !
	NErrorIf(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)		!= NVERTEX_FORMAT_V3,			NERROR_GEOMETRY_UNEXPECTED_VERTEX_FORMAT );
	NErrorIf(NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(pgeom->Core.IDS)	!= NPRIMITIVE_FORMAT_TRIANGLE,	NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_FORMAT );
	NErrorIf(NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS)	!= NAPPEARANCE_FORMAT_NULL,		NERROR_GEOMETRY_UNEXPECTED_APPEARANCE_FORMAT );
	NErrorIf(NGET_GEOMETRY_IDS_TEXTUREUNIT_FORMAT(pgeom->Core.IDS) != NTEXTUREUNIT_FORMAT_NULL,	NERROR_GEOMETRY_UNEXPECTED_TEXTUREUNIT_FORMAT );
	NErrorIf(NGetGeometryTextureUnitNumber(pgeom)			!= 0,							NERROR_GEOMETRY_UNEXPECTED_TEXTUREUNIT_NUMBER );

	// ! Array Size checking !
	NErrorIf(pgeom->Mesh.VertexArray.ElementSize != sizeof(NVERTEX_V3),					NERROR_GEOMETRY_UNEXPECTED_VERTEX_STRUCTURE_SIZE);
	NErrorIf(pgeom->Mesh.PrimitiveArray.ElementSize != sizeof(NTRIANGLE),				NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_STRUCTURE_SIZE);

	// ! Accumulator Consistency Checking !
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_TEXTURES), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_BLEND), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_LIGHT), NERROR_ACCUMULATOR_INCONSISTENCY );
}
void NDebugCheckingGeometry_ColorFlatTriangle_TU0_V3C4_TRIANGLE(const NGEOMETRY *pgeom)
{
	// !Format consistency checking !
	NErrorIf(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)		!= NVERTEX_FORMAT_V3C4,			NERROR_GEOMETRY_UNEXPECTED_VERTEX_FORMAT );
	NErrorIf(NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(pgeom->Core.IDS)	!= NPRIMITIVE_FORMAT_TRIANGLE,	NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_FORMAT );
	NErrorIf(NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS)	!= NAPPEARANCE_FORMAT_NULL,		NERROR_GEOMETRY_UNEXPECTED_APPEARANCE_FORMAT );
	NErrorIf(NGET_GEOMETRY_IDS_TEXTUREUNIT_FORMAT(pgeom->Core.IDS) != NTEXTUREUNIT_FORMAT_NULL,	NERROR_GEOMETRY_UNEXPECTED_TEXTUREUNIT_FORMAT );
	NErrorIf(NGetGeometryTextureUnitNumber(pgeom)			!= 0,							NERROR_GEOMETRY_UNEXPECTED_TEXTUREUNIT_NUMBER );

	// ! Array Size checking !
	NErrorIf(pgeom->Mesh.VertexArray.ElementSize != sizeof(NVERTEX_V3C4),					NERROR_GEOMETRY_UNEXPECTED_VERTEX_STRUCTURE_SIZE);
	NErrorIf(pgeom->Mesh.PrimitiveArray.ElementSize != sizeof(NTRIANGLE),					NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_STRUCTURE_SIZE);

	// ! Accumulator Consistency Checking !
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_TEXTURES), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_BLEND), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_LIGHT), NERROR_ACCUMULATOR_INCONSISTENCY );
}
void NDebugCheckingGeometry_TexturedTriangle_TUT1_V3C4T2_TRIANGLE(const NGEOMETRY *pgeom)
{
	// ! Format consistency Checking !
	NErrorIf(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)		!= NVERTEX_FORMAT_V3C4T2,		NERROR_GEOMETRY_UNEXPECTED_VERTEX_FORMAT);
	NErrorIf(NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(pgeom->Core.IDS)	!= NPRIMITIVE_FORMAT_TRIANGLE,	NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_FORMAT);
	NErrorIf(NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS)	!= NAPPEARANCE_FORMAT_TT,		NERROR_GEOMETRY_UNEXPECTED_APPEARANCE_FORMAT);
	NErrorIf(NGET_GEOMETRY_IDS_TEXTUREUNIT_FORMAT(pgeom->Core.IDS) != NTEXTUREUNIT_FORMAT_T,		NERROR_GEOMETRY_UNEXPECTED_TEXTUREUNIT_FORMAT);
	NErrorIf(NGetGeometryTextureUnitNumber(pgeom)			!=1,							NERROR_GEOMETRY_UNEXPECTED_TEXTUREUNIT_NUMBER);

	// ! Array Size checking !
	NErrorIf(pgeom->Mesh.VertexArray.ElementSize	!= sizeof(NVERTEX_V3C4T2),				NERROR_GEOMETRY_UNEXPECTED_VERTEX_STRUCTURE_SIZE);
	NErrorIf(pgeom->Mesh.PrimitiveArray.ElementSize != sizeof(NTRIANGLE),					NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_STRUCTURE_SIZE);

	// ! Accumulator Consistency Checking !
	NErrorIf(ISFLAG_OFF(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_TEXTURES), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_BLEND) && (!pgeom->Appearance.TT.pBlend), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_LIGHT) && (!pgeom->Appearance.TT.pMaterial), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_OFF(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_BLEND) && pgeom->Appearance.TT.pBlend, NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_OFF(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_LIGHT) && pgeom->Appearance.TT.pMaterial, NERROR_ACCUMULATOR_INCONSISTENCY );
	
	// ! A valid Texture is absolutely necessary !
	NErrorIf(!pgeom->Appearance.TT.TextureUnitT0.pTexture, NERROR_GEOMETRY_MISSING_TEXTURE);
	NErrorIf(pgeom->Appearance.TT.TextureUnitT1.pTexture, NERROR_GEOMETRY_UNEXPECTED_TEXTURE);
}
void NDebugCheckingGeometry_Add_Signed_Combine_TUT2_V3C4T2_TRIANGLE(const NGEOMETRY* pgeom)
{
	// ! Format consistency Checking !
	NErrorIf(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)		!= NVERTEX_FORMAT_V3C4T2,		NERROR_GEOMETRY_UNEXPECTED_VERTEX_FORMAT);
	NErrorIf(NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(pgeom->Core.IDS)	!= NPRIMITIVE_FORMAT_TRIANGLE,	NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_FORMAT);
	NErrorIf(NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS)	!= NAPPEARANCE_FORMAT_TT,		NERROR_GEOMETRY_UNEXPECTED_APPEARANCE_FORMAT);
	NErrorIf(NGET_GEOMETRY_IDS_TEXTUREUNIT_FORMAT(pgeom->Core.IDS) != NTEXTUREUNIT_FORMAT_T,		NERROR_GEOMETRY_UNEXPECTED_TEXTUREUNIT_FORMAT);
	NErrorIf(NGetGeometryTextureUnitNumber(pgeom)			!=2,								NERROR_GEOMETRY_UNEXPECTED_TEXTUREUNIT_NUMBER);

	// ! Array Size checking !
	NErrorIf(pgeom->Mesh.VertexArray.ElementSize	!= sizeof(NVERTEX_V3C4T2),				NERROR_GEOMETRY_UNEXPECTED_VERTEX_STRUCTURE_SIZE);
	NErrorIf(pgeom->Mesh.PrimitiveArray.ElementSize != sizeof(NTRIANGLE),					NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_STRUCTURE_SIZE);

	// ! Accumulator Consistency Checking !
	NErrorIf(ISFLAG_OFF(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_TEXTURES), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_BLEND) && (!pgeom->Appearance.TT.pBlend), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_LIGHT) && (!pgeom->Appearance.TT.pMaterial), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_OFF(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_BLEND) && pgeom->Appearance.TT.pBlend, NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_OFF(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_LIGHT) && pgeom->Appearance.TT.pMaterial, NERROR_ACCUMULATOR_INCONSISTENCY );

	// ! 2 valid Textures are absolutely necessary !
	NErrorIf(!pgeom->Appearance.TT.TextureUnitT0.pTexture, NERROR_GEOMETRY_MISSING_TEXTURE);
	NErrorIf(!pgeom->Appearance.TT.TextureUnitT1.pTexture, NERROR_GEOMETRY_MISSING_TEXTURE);
}

void NDebugCheckingGeometry_Add_Combine_TUT2_V3C4T2_TRIANGLE(const NGEOMETRY* pgeom)
{
	// ! Format consistency Checking !
	NErrorIf(NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)		!= NVERTEX_FORMAT_V3C4T2,		NERROR_GEOMETRY_UNEXPECTED_VERTEX_FORMAT);
	NErrorIf(NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(pgeom->Core.IDS)	!= NPRIMITIVE_FORMAT_TRIANGLE,	NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_FORMAT);
	NErrorIf(NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS)	!= NAPPEARANCE_FORMAT_TT,		NERROR_GEOMETRY_UNEXPECTED_APPEARANCE_FORMAT);
	NErrorIf(NGET_GEOMETRY_IDS_TEXTUREUNIT_FORMAT(pgeom->Core.IDS) != NTEXTUREUNIT_FORMAT_T,		NERROR_GEOMETRY_UNEXPECTED_TEXTUREUNIT_FORMAT);
	NErrorIf(NGetGeometryTextureUnitNumber(pgeom)			!=2,								NERROR_GEOMETRY_UNEXPECTED_TEXTUREUNIT_NUMBER);

	// ! Array Size checking !
	NErrorIf(pgeom->Mesh.VertexArray.ElementSize	!= sizeof(NVERTEX_V3C4T2),				NERROR_GEOMETRY_UNEXPECTED_VERTEX_STRUCTURE_SIZE);
	NErrorIf(pgeom->Mesh.PrimitiveArray.ElementSize != sizeof(NTRIANGLE),					NERROR_GEOMETRY_UNEXPECTED_PRIMITIVE_STRUCTURE_SIZE);

	// ! Accumulator Consistency Checking !
	NErrorIf(ISFLAG_OFF(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_TEXTURES), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_BLEND) && (!pgeom->Appearance.TT.pBlend), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_ON(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_LIGHT) && (!pgeom->Appearance.TT.pMaterial), NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_OFF(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_BLEND) && pgeom->Appearance.TT.pBlend, NERROR_ACCUMULATOR_INCONSISTENCY );
	NErrorIf(ISFLAG_OFF(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet,FLAG_STATESET_LIGHT) && pgeom->Appearance.TT.pMaterial, NERROR_ACCUMULATOR_INCONSISTENCY );

	// ! 2 valid Textures are absolutely necessary !
	NErrorIf(!pgeom->Appearance.TT.TextureUnitT0.pTexture, NERROR_GEOMETRY_MISSING_TEXTURE);
	NErrorIf(!pgeom->Appearance.TT.TextureUnitT1.pTexture, NERROR_GEOMETRY_MISSING_TEXTURE);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif	//_DEBUG
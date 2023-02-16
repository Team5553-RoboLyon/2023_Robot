#include "NGeometry.h"
#include "NGeometryDebugChecking.h"
#include "../NErrorHandling.h"
#include "./RenderFunctions/NGeometryRenderFunctions.h"

NGEOMETRY_SPECS			NGeometrySpecs[NGEOMETRY_FORMAT_ENUM_SIZE];

NGEOMETRY_SPECS*	NGetGeometrySpecs(NGEOMETRY_FORMAT_ENUM geometry_format)
{
	return &NGeometrySpecs[geometry_format];
}


// ------------------------------------------------------------------------------------------
// NInitializeGeometrySpecs
// ------------------------------------------------------------------------------------------
// Description :
//	Update all the N entities
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
// Because Specifications are really important and subtle there are some debug function to test
// consistency of all the Specifications parameters which may contain some errors especially 
// when a new one is created. So these debug functions ( which are working only in debug mode)
// are here to help Engine programming itself !!
#ifdef _DEBUG
#define _SetGeometry_Debug_CheckFunction(ps,fct)	(ps->Debug_CheckFunction = fct)
#endif
#ifndef _DEBUG
#define _SetGeometry_Debug_CheckFunction(ps,fct)
#endif

void NInitializeGeometrySpecs()
{
	NGEOMETRY_SPECS	*pgeomspecs;
	
	// All to ZERO (it should be done because NGeometrySpecs is a Global variable, but any away ...
	//				like this everything is done, for sure !)
	for(Nu32 i=0;i<NGEOMETRY_FORMAT_ENUM_SIZE;i++)
	{
		Nmem0(&NGeometrySpecs[i],NGEOMETRY_SPECS);
	}

	// SPECS LUT
	// ---------------------------------------------------------------------------------------------------------------------------------------
	// NGEOMETRY_SPEC_POINT_TU0_V3
	pgeomspecs = &NGeometrySpecs[NGEOMETRY_FORMAT_POINT_TU0_V3_POINT];
	pgeomspecs->RenderFunction				= NRenderGeometry_Point_TU0_V3_POINT;
	pgeomspecs->IDS							= NMAKE_GEOMETRY_IDS(	NGEOMETRY_FORMAT_POINT_TU0_V3_POINT,NACCUM_FLAT,
																	NMESH_FORMAT_ARRAYS,NPRIMITIVE_FORMAT_POINT, NVERTEX_FORMAT_V3,
																	NAPPEARANCE_FORMAT_NULL, NTEXTUREUNIT_FORMAT_NULL	);
	pgeomspecs->UsedTextureUnitNumber		= 0;

/*
	pgeomspecs->MeshSpecs					= 0;
	pgeomspecs->VertexSpecs					= NVERTEX_SPECS_V3;
	pgeomspecs->PrimitiveSpecs				= NPRIMITIVE_SPECS_POINT;
	pgeomspecs->AppearanceSpecs				= NMAKE_APPEARANCE_SPECS(FLAGS_NAPPEARANCE_SPECS_NULL,0);
	pgeomspecs->PrimitiveStructureSize		= _SafeNu32ToNu8( sizeof(NPOINT) );
	pgeomspecs->TextureUnitStructureSize	= _SafeNu32ToNu8( 0 );
*/
	_SetGeometry_Debug_CheckFunction(pgeomspecs,NDebugCheckingGeometry_Point_TU0_V3_POINT);
	// ---------------------------------------------------------------------------------------------------------------------------------------
	// NGEOMETRY_SPEC_POINT_TU0_V3C4
	pgeomspecs = &NGeometrySpecs[NGEOMETRY_FORMAT_COLORPOINT_TU0_V3C4_POINT];
	pgeomspecs->RenderFunction				= NRenderGeometry_ColorPoint_TU0_V3C4_POINT;
	pgeomspecs->IDS							= NMAKE_GEOMETRY_IDS(	NGEOMETRY_FORMAT_COLORPOINT_TU0_V3C4_POINT, NACCUM_FLAT,
																	NMESH_FORMAT_ARRAYS,NPRIMITIVE_FORMAT_POINT, NVERTEX_FORMAT_V3C4,
																	NAPPEARANCE_FORMAT_NULL, NTEXTUREUNIT_FORMAT_NULL	);
	pgeomspecs->UsedTextureUnitNumber		= 0;
/*
	pgeomspecs->MeshSpecs					= 0;
	pgeomspecs->VertexSpecs					= NVERTEX_SPECS_V3C4;
	pgeomspecs->PrimitiveSpecs				= NPRIMITIVE_SPECS_POINT;
	pgeomspecs->AppearanceSpecs				= NMAKE_APPEARANCE_SPECS(FLAGS_NAPPEARANCE_SPECS_NULL,0);
	pgeomspecs->PrimitiveStructureSize		= _SafeNu32ToNu8( sizeof(NPOINT) );
	pgeomspecs->TextureUnitStructureSize	= _SafeNu32ToNu8( 0 );
*/
	_SetGeometry_Debug_CheckFunction(pgeomspecs,NDebugCheckingGeometry_ColorPoint_TU0_V3C4_POINT);
	// ---------------------------------------------------------------------------------------------------------------------------------------
	// NGEOMETRY_SPEC_FLATLINE_TU0_V3
	pgeomspecs = &NGeometrySpecs[NGEOMETRY_FORMAT_LINE_TU0_V3_LINE];
	pgeomspecs->RenderFunction				= NRenderGeometry_Line_TU0_V3_LINE;
	pgeomspecs->IDS							= NMAKE_GEOMETRY_IDS(	NGEOMETRY_FORMAT_LINE_TU0_V3_LINE,NACCUM_FLAT,
																	NMESH_FORMAT_ARRAYS,NPRIMITIVE_FORMAT_LINE, NVERTEX_FORMAT_V3,
																	NAPPEARANCE_FORMAT_NULL,NTEXTUREUNIT_FORMAT_NULL	);
	pgeomspecs->UsedTextureUnitNumber		= 0;

/*
	pgeomspecs->MeshSpecs					= 0;
	pgeomspecs->VertexSpecs					= NVERTEX_SPECS_V3;
	pgeomspecs->PrimitiveSpecs				= NPRIMITIVE_SPECS_LINE;
	
	pgeomspecs->AppearanceSpecs				= NMAKE_APPEARANCE_SPECS(FLAGS_NAPPEARANCE_SPECS_NULL,0,0);

	pgeomspecs->PrimitiveStructureSize		= _SafeNu32ToNu8( sizeof(NLINE) );
	pgeomspecs->TextureUnitStructureSize	= _SafeNu32ToNu8( 0 );
*/

	_SetGeometry_Debug_CheckFunction(pgeomspecs,NDebugCheckingGeometry_Line_TU0_V3_LINE);

	// ---------------------------------------------------------------------------------------------------------------------------------------
	// NGEOMETRY_SPEC_FLATLINE_TU0_V3C4
	pgeomspecs = &NGeometrySpecs[NGEOMETRY_FORMAT_COLORLINE_TU0_V3C4_LINE];
	pgeomspecs->RenderFunction				= NRenderGeometry_ColorLine_TU0_V3C4_LINE;
	pgeomspecs->IDS							= NMAKE_GEOMETRY_IDS(	NGEOMETRY_FORMAT_COLORLINE_TU0_V3C4_LINE, NACCUM_FLAT,
																	NMESH_FORMAT_ARRAYS,NPRIMITIVE_FORMAT_LINE, NVERTEX_FORMAT_V3C4,
																	NAPPEARANCE_FORMAT_NULL,NTEXTUREUNIT_FORMAT_NULL	);

	pgeomspecs->UsedTextureUnitNumber		= 0;

/*
	pgeomspecs->VertexSpecs		= NVertexSpecs[NVERTEX_FORMAT_V3C4];
	pgeomspecs->PrimitiveSpecs		= NMAKE_PRIMITIVE_SPECS( FLAGS_NPRIMITIVE_SPECS_LINE, _SafeNu32ToNu8( sizeof(NLINE) ) );
	pgeomspecs->MeshSpecs			= NMAKE_MESH_SPECS(0);
	pgeomspecs->AppearanceSpecs	= NMAKE_APPEARANCE_SPECS(FLAGS_NAPPEARANCE_SPECS_NULL,0,0);
*/

	_SetGeometry_Debug_CheckFunction(pgeomspecs,NDebugCheckingGeometry_ColorLine_TU0_V3C4_LINE);

	// ---------------------------------------------------------------------------------------------------------------------------------------
	// NGEOMETRY_RENDERSETTINGS_TEXLINE_TUT1_V3C4T2	
	pgeomspecs = &NGeometrySpecs[NGEOMETRY_FORMAT_TEXLINE_TUT1_V3C4T2_LINE];
	pgeomspecs->RenderFunction				= NRenderGeometry_TexturedLine_TUT1_V3C4T2_LINE;
	pgeomspecs->IDS							= NMAKE_GEOMETRY_IDS(	NGEOMETRY_FORMAT_TEXLINE_TUT1_V3C4T2_LINE, NACCUM_TEXTURES,
																	NMESH_FORMAT_ARRAYS,NPRIMITIVE_FORMAT_LINE, NVERTEX_FORMAT_V3C4T2,
																	NAPPEARANCE_FORMAT_TT, NTEXTUREUNIT_FORMAT_T	);

	pgeomspecs->UsedTextureUnitNumber		= 1;

/*
	pgeomspecs->VertexSpecs		= NVertexSpecs[NVERTEX_FORMAT_V3C4T2];
	pgeomspecs->PrimitiveSpecs		= NMAKE_PRIMITIVE_SPECS( FLAGS_NPRIMITIVE_SPECS_LINE, _SafeNu32ToNu8( sizeof(NLINE) ) );
	pgeomspecs->MeshSpecs			= NMAKE_MESH_SPECS(0);
	pgeomspecs->AppearanceSpecs	= NMAKE_APPEARANCE_SPECS(FLAGS_NAPPEARANCE_SPECS_TT,1,_SafeNu32ToNu8( sizeof(NTEXTUREUNIT_T) ));
*/
	
	_SetGeometry_Debug_CheckFunction(pgeomspecs,NDebugCheckingGeometry_TexturedLine_TUT1_V3C4T2_LINE);
	
	// ---------------------------------------------------------------------------------------------------------------------------------------
	// NGEOMETRY_FORMAT_FLATTRIANGLE_TU0_V3_TRIANGLE
	pgeomspecs = &NGeometrySpecs[NGEOMETRY_FORMAT_FLATTRIANGLE_TU0_V3_TRIANGLE];
	pgeomspecs->RenderFunction				= NRenderGeometry_FlatTriangle_TU0_V3_TRIANGLE;
	pgeomspecs->IDS							= NMAKE_GEOMETRY_IDS(	NGEOMETRY_FORMAT_FLATTRIANGLE_TU0_V3_TRIANGLE, NACCUM_FLAT,
																	NMESH_FORMAT_ARRAYS,NPRIMITIVE_FORMAT_TRIANGLE, NVERTEX_FORMAT_V3,
																	NAPPEARANCE_FORMAT_NULL,NTEXTUREUNIT_FORMAT_NULL	);
															
	pgeomspecs->UsedTextureUnitNumber		= 0;

/*
	pgeomspecs->VertexSpecs		= NVertexSpecs[NVERTEX_FORMAT_V3];
	pgeomspecs->PrimitiveSpecs		= NMAKE_PRIMITIVE_SPECS( FLAGS_NPRIMITIVE_SPECS_TRIANGLE, _SafeNu32ToNu8( sizeof(NTRIANGLE) ) );
	pgeomspecs->MeshSpecs			= NMAKE_MESH_SPECS(0);
	pgeomspecs->AppearanceSpecs	= NMAKE_APPEARANCE_SPECS(FLAGS_NAPPEARANCE_SPECS_NULL,0,0);
*/

	_SetGeometry_Debug_CheckFunction(pgeomspecs,NDebugCheckingGeometry_FlatTriangle_TU0_V3_TRIANGLE);

	// ---------------------------------------------------------------------------------------------------------------------------------------
	// NGEOMETRY_FORMAT_COLORFLATTRIANGLE_TU0_V3C4_TRIANGLE
	pgeomspecs = &NGeometrySpecs[NGEOMETRY_FORMAT_COLORFLATTRIANGLE_TU0_V3C4_TRIANGLE];
	pgeomspecs->RenderFunction				= NRenderGeometry_ColorFlatTriangle_TU0_V3C4_TRIANGLE;
	pgeomspecs->IDS							= NMAKE_GEOMETRY_IDS(	NGEOMETRY_FORMAT_COLORFLATTRIANGLE_TU0_V3C4_TRIANGLE,NACCUM_FLAT,
																	NMESH_FORMAT_ARRAYS,NPRIMITIVE_FORMAT_TRIANGLE, NVERTEX_FORMAT_V3C4,
																	NAPPEARANCE_FORMAT_NULL,NTEXTUREUNIT_FORMAT_NULL	);

	pgeomspecs->UsedTextureUnitNumber		= 0;

/*
	pgeomspecs->VertexSpecs		= NVertexSpecs[NVERTEX_FORMAT_V3C4];
	pgeomspecs->PrimitiveSpecs		= NMAKE_PRIMITIVE_SPECS( FLAGS_NPRIMITIVE_SPECS_TRIANGLE, _SafeNu32ToNu8( sizeof(NTRIANGLE) ) );
	pgeomspecs->MeshSpecs			= NMAKE_MESH_SPECS(0);
	pgeomspecs->AppearanceSpecs	= NMAKE_APPEARANCE_SPECS(FLAGS_NAPPEARANCE_SPECS_NULL,0,0);
*/

	_SetGeometry_Debug_CheckFunction(pgeomspecs,NDebugCheckingGeometry_ColorFlatTriangle_TU0_V3C4_TRIANGLE);

	// ---------------------------------------------------------------------------------------------------------------------------------------
	// NGEOMETRY_FORMAT_TEXTRIANGLE_TUT1_V3C4T2_TRIANGLE	
	pgeomspecs = &NGeometrySpecs[NGEOMETRY_FORMAT_TEXTRIANGLE_TUT1_V3C4T2_TRIANGLE];
	pgeomspecs->RenderFunction			= NRenderGeometry_TexturedTriangle_TUT1_V3C4T2_TRIANGLE;
	pgeomspecs->IDS						= NMAKE_GEOMETRY_IDS(	NGEOMETRY_FORMAT_TEXTRIANGLE_TUT1_V3C4T2_TRIANGLE,NACCUM_TEXTURES,
																NMESH_FORMAT_ARRAYS,NPRIMITIVE_FORMAT_TRIANGLE, NVERTEX_FORMAT_V3C4T2,
																NAPPEARANCE_FORMAT_TT, NTEXTUREUNIT_FORMAT_T	);
	pgeomspecs->UsedTextureUnitNumber	= 1;

/*
	pgeomspecs->VertexSpecs		= NVertexSpecs[NVERTEX_FORMAT_V3C4T2];
	pgeomspecs->PrimitiveSpecs		= NMAKE_PRIMITIVE_SPECS( FLAGS_NPRIMITIVE_SPECS_TRIANGLE, _SafeNu32ToNu8( sizeof(NTRIANGLE) ) );
	pgeomspecs->MeshSpecs			= NMAKE_MESH_SPECS(0);
	pgeomspecs->AppearanceSpecs	= NMAKE_APPEARANCE_SPECS(FLAGS_NAPPEARANCE_SPECS_TT,1,_SafeNu32ToNu8( sizeof(NTEXTUREUNIT_T) ));
*/

	_SetGeometry_Debug_CheckFunction(pgeomspecs,NDebugCheckingGeometry_TexturedTriangle_TUT1_V3C4T2_TRIANGLE);

	// ---------------------------------------------------------------------------------------------------------------------------------------
	// NGEOMETRY_FORMAT_ADD_SIGNED_COMBINE_TUT2_V3C4T2_TRIANGLE	
	pgeomspecs = &NGeometrySpecs[NGEOMETRY_FORMAT_ADD_SIGNED_COMBINE_TUT2_V3C4T2_TRIANGLE];
	pgeomspecs->RenderFunction			= NRenderGeometry_Add_Signed_Combine_TUT2_V3C4T2_TRIANGLE;
	pgeomspecs->IDS						= NMAKE_GEOMETRY_IDS(	NGEOMETRY_FORMAT_ADD_SIGNED_COMBINE_TUT2_V3C4T2_TRIANGLE, NACCUM_TEXTURES,
																NMESH_FORMAT_ARRAYS,NPRIMITIVE_FORMAT_TRIANGLE, NVERTEX_FORMAT_V3C4T2,
																NAPPEARANCE_FORMAT_TT, NTEXTUREUNIT_FORMAT_T	);
	pgeomspecs->UsedTextureUnitNumber	= 2;

/*

	pgeomspecs->VertexSpecs		= NVertexSpecs[NVERTEX_FORMAT_V3C4T2];
	pgeomspecs->PrimitiveSpecs		= NMAKE_PRIMITIVE_SPECS( FLAGS_NPRIMITIVE_SPECS_TRIANGLE, _SafeNu32ToNu8( sizeof(NTRIANGLE) ) );
	pgeomspecs->MeshSpecs			= NMAKE_MESH_SPECS(0);
	pgeomspecs->AppearanceSpecs	= NMAKE_APPEARANCE_SPECS(FLAGS_NAPPEARANCE_SPECS_TT,2,_SafeNu32ToNu8( sizeof(NTEXTUREUNIT_T) ));
*/

	_SetGeometry_Debug_CheckFunction(pgeomspecs,NDebugCheckingGeometry_Add_Signed_Combine_TUT2_V3C4T2_TRIANGLE);

	// ---------------------------------------------------------------------------------------------------------------------------------------
	// NGEOMETRY_FORMAT_ADD_COMBINE_TUT2_V3C4T2_TRIANGLE	
	pgeomspecs = &NGeometrySpecs[NGEOMETRY_FORMAT_ADD_COMBINE_TUT2_V3C4T2_TRIANGLE];
	pgeomspecs->RenderFunction			= NRenderGeometry_Add_Combine_TUT2_V3C4T2_TRIANGLE;
	pgeomspecs->IDS						= NMAKE_GEOMETRY_IDS(	NGEOMETRY_FORMAT_ADD_COMBINE_TUT2_V3C4T2_TRIANGLE, NACCUM_TEXTURES,
																NMESH_FORMAT_ARRAYS,NPRIMITIVE_FORMAT_TRIANGLE, NVERTEX_FORMAT_V3C4T2,
																NAPPEARANCE_FORMAT_TT, NTEXTUREUNIT_FORMAT_T	);
	pgeomspecs->UsedTextureUnitNumber	= 2;

/*

	pgeomspecs->VertexSpecs		= NVertexSpecs[NVERTEX_FORMAT_V3C4T2];
	pgeomspecs->PrimitiveSpecs		= NMAKE_PRIMITIVE_SPECS( FLAGS_NPRIMITIVE_SPECS_TRIANGLE, _SafeNu32ToNu8( sizeof(NTRIANGLE) ) );
	pgeomspecs->MeshSpecs			= NMAKE_MESH_SPECS(0);
	pgeomspecs->AppearanceSpecs	= NMAKE_APPEARANCE_SPECS(FLAGS_NAPPEARANCE_SPECS_TT,2,_SafeNu32ToNu8( sizeof(NTEXTUREUNIT_T) ));
*/

	_SetGeometry_Debug_CheckFunction(pgeomspecs,NDebugCheckingGeometry_Add_Combine_TUT2_V3C4T2_TRIANGLE);
	// NGEOMETRY_FORMAT_xx



// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + END CHECKING IN DEBUG MODE TO PREVENT USUAL MISTAKES
// +
#ifdef _DEBUG 
	for(Nu32 i=0;i<NGEOMETRY_FORMAT_ENUM_SIZE;i++)
	{
		NErrorIf( NGET_GEOMETRY_IDS_GEOMETRY_FORMAT( NGeometrySpecs[i].IDS ) != 0 && NGET_GEOMETRY_IDS_GEOMETRY_FORMAT( NGeometrySpecs[i].IDS ) != i, NERROR_SYSTEM_GURU_MEDITATION );
	}
#endif
// +
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

void NDisableGeometrySpecs()
{
	for(Nu32 i=0;i<NGEOMETRY_FORMAT_ENUM_SIZE;i++)
	{
		Nmem0(&NGeometrySpecs[i],NGEOMETRY_SPECS);
	}
}
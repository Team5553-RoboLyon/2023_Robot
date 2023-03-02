#ifndef __NGEOMETRY_H
#define __NGEOMETRY_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NGeometry.h											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "../NMatrix.h"
#include "../NTransformationTree.h"
#include "../Containers/NArray.h"
#include "../NTexture.h"
#include "../Miscellaneous/NColor.h"
#include "../NBoundingVolumes.h"
#include "./Components/NGeometryMesh.h"
#include "./Components/NGeometryAppearance.h"
#include "../Render/Accumulator/NAccumulator.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

	// ***************************************************************************************
	// **					Geometry Format Enum											**
	// ***************************************************************************************
	// NGEOMETRY_FORMAT Nomenclature understanding:
	//
	//			NGEOMETRY_FORMAT_#ExplicitName#_#TextureUnitFormat_and_Number#_#VertexFormat#_#PrimitiveFormat#
	//							 |			  | |							 | |			| |				  |
	//			NGEOMETRY_FORMAT_|   Part I   |_|         Part II            |_|  Part III  |_|   Part IV     |
	//							 |			  | | TU0  : No texture Unit	 | |			| |				  |
	//							 |			  | | TUTn : n TextureUnit T	 | |			| |				  |
	//							 |			  | | TUTMn: n TextureUnit TM	 | |			| |				  |
	//
	// Notice RenderFunction Name and NGEOMETRY_FORMAT Name are close together. It's normal because there is a specific Render function
	// for each Geometry Format...

	typedef enum
	{
		NGEOMETRY_FORMAT_POINT_TU0_V3_POINT = 0,				  // Vertex position ONLY
		NGEOMETRY_FORMAT_COLORPOINT_TU0_V3C4_POINT,				  // Vertex AND Color
		NGEOMETRY_FORMAT_LINE_TU0_V3_LINE,						  // Line Only
		NGEOMETRY_FORMAT_COLORLINE_TU0_V3C4_LINE,				  // Colored Line
		NGEOMETRY_FORMAT_TEXLINE_TUT1_V3C4T2_LINE,				  // Colored Textured Line
		NGEOMETRY_FORMAT_FLATTRIANGLE_TU0_V3_TRIANGLE,			  // Flat Triangle
		NGEOMETRY_FORMAT_COLORFLATTRIANGLE_TU0_V3C4_TRIANGLE,	  // Flat Colored triangle
		NGEOMETRY_FORMAT_TEXTRIANGLE_TUT1_V3C4T2_TRIANGLE,		  // Texture Triangle (with color) Texture "Diffuse" Only
		NGEOMETRY_FORMAT_ADD_SIGNED_COMBINE_TUT2_V3C4T2_TRIANGLE, // Texture 1 is Added to Texture 0
		NGEOMETRY_FORMAT_ADD_COMBINE_TUT2_V3C4T2_TRIANGLE,
		// NGEOMETRY_FORMAT_???_TUT3_V3C4T2_TRIANGLE,
		// NGEOMETRY_FORMAT_???_TUT?_V3C4T2_TRIANGLE,
		// NGEOMETRY_FORMAT_???_TUT0_V3N3C4T2_TRIANGLE,
		NGEOMETRY_FORMAT_CUBEREFLEX_TUT1_V3N3C4T2_TRIANGLE,		  // Triangle (with color and Normal) Textured with a CubeMap
		NGEOMETRY_FORMAT_TEXANDCUBEREFLEX_TUT2_V3N3C4T2_TRIANGLE, // Textured Triangle (with color and Normal) combined with a Cube Map
		NGEOMETRY_FORMAT_TEXANDCUBEREFLEX_TUT3_V3N3C4T2_TRIANGLE, // Textured Triangle (with color and Normal) combined with a Cube Map trough a Normal Map

		// NGEOMETRY_FORMAT_???_TU?_V??????,
		// NGEOMETRY_FORMAT_???_TU?_V??????,
		// NGEOMETRY_FORMAT_???_TU?_V??????,
		// NGEOMETRY_FORMAT_???_TU?_V??????,
		// NGEOMETRY_FORMAT_???_TU?_V??????,
		// NGEOMETRY_FORMAT_???_TU?_V??????,
		// NGEOMETRY_FORMAT_???_TU?_V??????,
		// NGEOMETRY_FORMAT_???_TU?_V??????,
		// NGEOMETRY_FORMAT_???_TU?_V??????,
		// NGEOMETRY_FORMAT_???_TU?_V??????,

		//------------------------------------
		NGEOMETRY_FORMAT_ENUM_SIZE // MAX size is 256 !!! (and max FORMAT ID is 255) This GEOMETRY FORMAT ID is going to be stored on a 8 bits value !!!
	} NGEOMETRY_FORMAT_ENUM;

// ***************************************************************************************
// **					Geometry IDS													**
// ***************************************************************************************
//
//	Geometry IDS is a packed 32 bits value with all the geometry Components Formats and
//	the geometry associated accumulator ID packed inside.
//
//		- Accumulator ID				8 Bits
//		- MESH FORMAT					3 Bits (NOT REALLY USED YET ... BUT FOR A FUTURE INTEGRATION OF VBO)
//		- PRIMITIVE FORMAT				3 Bits
//		- VERTEX FORMAT					6 Bits
//		- APPEARANCE FORMAT				3 Bits
//		- NGEOMETRY FORMAT				9 Bits
// |___________|___________|___________|___________|___________|___________|___________|___________|
// |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
// |	   				      |		   |				 |		  |		   |					   |
// |    NGEOMETRY FORMAT      | APPEAR.|	 VERTEX		 |PRIMITIV|	 MESH  |	  Accumulator 	   |
// |	   				      | FORMAT |	 FORMAT		 | FORMAT | FORMAT |	       ID	 	   |
// |		(9 bits)	      | 3 bits |	 6 bits		 | 3 bits |	3 bits |		(8 bits)	   |
// |		0 to 511	      | 0 to 7 |	0 to 63		 | 0 to 7 | 0 to 7 |		0 to 255	   |
// ------------------------------------|-----------------|--------|--------------------------------|
// |    NGEOMETRY FORMAT	  |APP.FORM|			MESH FORMATS		   |	Accumulator ID	   |
// -------------------------------------------------------------------------------------------------

// MAKE
#define NMAKE_GEOMETRY_IDS(specid, accum, mesh, prim, vert, appear, tu) (Nu32)((((specid)&0xFF) << 24) | (((tu)&0x3) << 22) | (((appear)&0x3) << 20) | (((vert)&0x3F) << 14) | (((prim)&0x7) << 11) | (((mesh)&0x7) << 8) | ((accum)&0xFF))
#define NUPDATE_GEOMETRY_IDS__ACCUMULATOR_ID(geomid, accumid) (Nu32)(((geomid)&0xFFFFFF00) | ((accumid)&0xFF))
// GET
#define NGET_GEOMETRY_IDS_GEOMETRY_FORMAT(geomid) ((NGEOMETRY_FORMAT_ENUM)(((geomid)&0xFF000000) >> 24))
#define NGET_GEOMETRY_IDS_MESH_ALL_FORMATS(geomid) (((geomid)&0xFFF00) >> 8)
#define NGET_GEOMETRY_IDS_MESH_FORMAT(geomid) ((NMESH_FORMAT_ENUM)(((geomid)&0x700) >> 8))
#define NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(geomid) ((NPRIMITIVE_FORMAT_ENUM)(((geomid)&0x3800) >> 11))
#define NGET_GEOMETRY_IDS_VERTEX_FORMAT(geomid) ((NVERTEX_FORMAT_ENUM)(((geomid)&0xFC000) >> 14))
#define NGET_GEOMETRY_IDS_APPEARANCE_ALL_FORMATS(geomid) (((geomid)&0xF00000) >> 20)
#define NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(geomid) ((NAPPEARANCE_FORMAT_ENUM)(((geomid)&0x300000) >> 20))
#define NGET_GEOMETRY_IDS_TEXTUREUNIT_FORMAT(geomid) ((NTEXTUREUNIT_FORMAT_ENUM)(((geomid)&0xC00000) >> 22))

#define NGET_GEOMETRY_IDS_ACCUMULATOR_ID(geomid) ((NACCUM_ID_ENUM)((geomid)&0xFF))

	// ***************************************************************************************
	// **					Geometry Structures												**
	// ***************************************************************************************
	typedef struct NGEOMETRY NGEOMETRY;
	typedef void (*NGEOMETRY_RENDERFCT)(const NGEOMETRY *pgeom /*, const NTRANSFORM_HNODE *ptransform_node*/);

	typedef struct
	{
		NGEOMETRY_RENDERFCT RenderFunction; // "THE" Render function called by NEngine to Render the geometry
		Nu32 IDS;							// a 32 bits value where all the geometry component IDS are packed together
											// thanks to the Macro NMAKEGEOMETRY_IDS
	} NGEOMETRY_COMPONENT_CORE;

	typedef struct
	{
		NGEOMETRY_FORMAT_ENUM GeometryFormat;
		NSTATESET UserStateSet; // Which are included into NSTATESET as flags BIT_8 to BIT_31
		NMESH_CAPACITIES MeshCapacities;
	} NGEOMETRY_DESC;

	struct NGEOMETRY
	{
		NGEOMETRY_COMPONENT_CORE Core;
		NMESH Mesh;
		NAPPEARANCE Appearance;
	};

// ***************************************************************************************
// **					Geometry "Specs" Structure										**
// ***************************************************************************************
#ifdef _DEBUG
	typedef void (*NGEOMETRY_DEBUGCHECKFCT)(const NGEOMETRY *pgeom);
#endif

	typedef struct
	{
		NGEOMETRY_RENDERFCT RenderFunction;
		Nu32 IDS;
		Nu8 UsedTextureUnitNumber;
		Nu8 Available_Nu8;
		Nu16 Available_Nu16;

// ---------- DEBUG ONLY ----------
#ifdef _DEBUG
		NGEOMETRY_DEBUGCHECKFCT Debug_CheckFunction;
#endif
		// ---------- DEBUG ONLY ----------
	} NGEOMETRY_SPECS;

	// ***************************************************************************************
	// **					Geometry Functions												**
	// ***************************************************************************************
	inline void NFillupGeometryDesc(NGEOMETRY_DESC *pgdesc, const NGEOMETRY_FORMAT_ENUM rsid, const NSTATESET userstateset, const Nu32 prim_capacity, const Nu32 vert_capacity)
	{
		pgdesc->GeometryFormat = (NGEOMETRY_FORMAT_ENUM)rsid;
		pgdesc->UserStateSet = userstateset;
		NFillupMeshCapacities(&pgdesc->MeshCapacities, prim_capacity, vert_capacity);
	}
	inline void NFillupGeometryDescEx(NGEOMETRY_DESC *pgdesc, const NGEOMETRY_FORMAT_ENUM rsid, const NSTATESET userstateset, const Nu32 prim_capacity, const Nu32 prim_size, const Nu32 vert_capacity, const Nu32 vert_size)
	{
		pgdesc->GeometryFormat = (NGEOMETRY_FORMAT_ENUM)rsid;
		pgdesc->UserStateSet = userstateset;
		NFillupMeshCapacitiesEx(&pgdesc->MeshCapacities, prim_capacity, prim_size, vert_capacity, vert_size);
	}

	NGEOMETRY *NSetupGeometry(NGEOMETRY *pgeom, const NGEOMETRY_DESC *pdesc);
	NGEOMETRY *NCreateGeometry(const NGEOMETRY_DESC *pdesc);
	void NClearGeometry(void *pgeom);
	void NDeleteGeometry(void *pgeom);

	inline Nbool NCheckGeometryAppearance_vs_AppearanceRaw(const NGEOMETRY *pgeom, const NAPPEARANCE_RAW *praw, const NAPPEARANCE_CHECKMASK *pmask) { return NCheckAppearanceRaw_vs_Appearance(praw, &pgeom->Appearance, NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS), pmask); }
	NGEOMETRY *NCopyGeometry(NGEOMETRY *pgeodst, const NGEOMETRY *pgeosrc);
	NGEOMETRY *NDuplicateGeometry(const NGEOMETRY *pgeosrc);

	inline void NSetGeometryAccumulator(NGEOMETRY *pgeom, const NACCUM_ID_ENUM accum_id) { pgeom->Core.IDS = NUPDATE_GEOMETRY_IDS__ACCUMULATOR_ID(pgeom->Core.IDS, accum_id); }
	void NSetGeometryAppearance(NGEOMETRY *pgeom, const NAPPEARANCE_RAW *pdesc);
	void NUnpackGeometryAppearance(NAPPEARANCE_RAW *praw, const NGEOMETRY *pgeom);
	void NUpdateGeometryAccumulatorID(NGEOMETRY *pgeom);

	void NSetGeometryTexture(NGEOMETRY *pgeom, const Nu8 texture_unit, const NTEXTURE *ptexture);
	void NSetGeometryMaterial(NGEOMETRY *pgeom, const NMATERIAL *pmaterial);
	void NSetGeometryBlend(NGEOMETRY *pgeom, const NBLEND *pblend);

	Nu8 NGetGeometryTextureUnitNumber(const NGEOMETRY *pgeom);
	Nu8 NGetGeometryTexCoordSetNumber(const NGEOMETRY *pgeom);
	Nu8 NGetGeometryColorSetNumber(const NGEOMETRY *pgeom);

	NTEXTURE *NGetGeometryTexture(const NGEOMETRY *pgeom, const Nu8 texture_unit);
	inline NMATERIAL *NGetGeometryMaterial(NGEOMETRY *pgeom)
	{
		NErrorIf((NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS) == NAPPEARANCE_FORMAT_NULL), NERROR_GEOMETRY_APPEARANCE_NOT_INCLUDED);
		return pgeom->Appearance.TT.pMaterial;
	}
	inline NBLEND *NGetGeometryBlend(NGEOMETRY *pgeom)
	{
		NErrorIf((NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS) == NAPPEARANCE_FORMAT_NULL), NERROR_GEOMETRY_APPEARANCE_NOT_INCLUDED);
		return pgeom->Appearance.TT.pBlend;
	}

	void *NGetGeometryVertexPtr(const NVERTEX_DATA_ENUM addr_id, const NGEOMETRY *pgeom, const Nu32 index);
	void NEraseGeometryMesh(NGEOMETRY *pgeom);

	// ***************************************************************************************
	// **					Geometry "Component-Core" Functions								**
	// ***************************************************************************************
	// Setup
	inline void NSetupGeometryComponent_Core(NGEOMETRY_COMPONENT_CORE *pgeomcore, const NGEOMETRY_RENDERFCT renderfct, const Nu32 geomid)
	{
		pgeomcore->RenderFunction = (NGEOMETRY_RENDERFCT)renderfct;
		pgeomcore->IDS = geomid;
	}
	// Clear
	inline void NClearGeometryComponent_Core(NGEOMETRY_COMPONENT_CORE *pgeomcbase)
	{
		// pgeomcbase->NRenderFunction	= NULL;		// keep it for future structure using ?
		// pgeomcbase->IDS				= 0;		// keep it for future structure identification ?
	}
	// Copy
	inline void NCopyGeometryComponent_Core(NGEOMETRY_COMPONENT_CORE *pgeodst, const NGEOMETRY_COMPONENT_CORE *pgeosrc)
	{
		pgeodst->IDS = pgeosrc->IDS;
		pgeodst->RenderFunction = pgeosrc->RenderFunction;
	}

	// ***************************************************************************************
	// **					Geometry "Appearance Check Mask" Functions						**
	// ***************************************************************************************
	NAPPEARANCE_CHECKMASK *NGetGeometryAppearanceCheckMask(NAPPEARANCE_CHECKMASK *pcheckmask, const Nu8 blend_maskvalue, const Nu8 material_maskvalue, const Nu8 texture_maskvalue, const Nu8 txcmatrixupdater_maskvalue, const NGEOMETRY *pgeom);
	inline NAPPEARANCE_CHECKMASK *NGetGeometryAppearanceCheckMask_MASKED(NAPPEARANCE_CHECKMASK *pcheckmask, NGEOMETRY *pgeom) { NGetGeometryAppearanceCheckMask(pcheckmask, NAPPEARANCE_CHECKMASK_MASKED, NAPPEARANCE_CHECKMASK_MASKED, NAPPEARANCE_CHECKMASK_MASKED, NAPPEARANCE_CHECKMASK_MASKED, pgeom); }
	inline NAPPEARANCE_CHECKMASK *NGetGeometryAppearanceCheckMask_EQUAL(NAPPEARANCE_CHECKMASK *pcheckmask, NGEOMETRY *pgeom) { NGetGeometryAppearanceCheckMask(pcheckmask, NAPPEARANCE_CHECKMASK_EQUAL, NAPPEARANCE_CHECKMASK_EQUAL, NAPPEARANCE_CHECKMASK_EQUAL, NAPPEARANCE_CHECKMASK_EQUAL, pgeom); }
	inline NAPPEARANCE_CHECKMASK *NGetGeometryAppearanceCheckMask_DIFFERENT(NAPPEARANCE_CHECKMASK *pcheckmask, NGEOMETRY *pgeom) { NGetGeometryAppearanceCheckMask(pcheckmask, NAPPEARANCE_CHECKMASK_DIFFERENT, NAPPEARANCE_CHECKMASK_DIFFERENT, NAPPEARANCE_CHECKMASK_DIFFERENT, NAPPEARANCE_CHECKMASK_DIFFERENT, pgeom); }

	// ***************************************************************************************
	// **					Geometry "Specs" Functions										**
	// ***************************************************************************************
	NGEOMETRY_SPECS *NGetGeometrySpecs(NGEOMETRY_FORMAT_ENUM rendersettings_id);

	// ***************************************************************************************
	// **					Geometry "PRIVATE" Functions									**
	// ***************************************************************************************
	NGEOMETRY_SPECS *NGetGeometrySpecs(NGEOMETRY_FORMAT_ENUM rendersettings_id);
	void NInitializeGeometrySpecs();
	void NDisableGeometrySpecs();

// ------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NGEOMETRY_H

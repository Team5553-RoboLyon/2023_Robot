#ifndef __NGEOMETRYCOMPONENT_CORE_H_
#define __NGEOMETRYCOMPONENT_CORE_H_

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NGeometryComponent_Core.h							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/NTransformationTree.h"
#include "lib/N/Geometry/RenderFunctions/NGeometryRenderFunctionsID.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// #include "../NGeometryLUTS.h"
// #include "../NAccumulator.h"
// ------------------------------------------------------------------------------------------------------------------------------------------
// The "smallest" possible Geometry structure :Just the render function, the associated statesetID (=accumulator ID) and the geometryID
// Actually this kind of structure is not used. There is no vertex and no primitive inside...
// BUT, like this it will be possible to create all the geometry we want, with all the possible structure of data ...
// Each derivative GEOMETRY structure must have the NGEOMETRY_BASE component has it first component !
// Like this
// 				typedef struct
// 				{
// 					NGEOMETRY_BASE	Base;
// 					Additional GEOMETRY Component 1
// 					Additional GEOMETRY Component 2
// 					Additional GEOMETRY Component 3
//					Additional GEOMETRY Component n
// 				}NGEOMETRY_mygeometry;

// Notes: We are going to use "NGEOMETRY" instead of "NGEOMETRY_BASE" in the whole program excepting inside the core geometry code.
//		 The main reason is to mark a difference between the component "NGEOMETRY_COMPONENT_CORE" and the generic name of all the
//		 derivative NGEOMETRY structures.
/*
typedef struct NGEOMETRY		NGEOMETRY;
typedef void (*NGEOMETRY_RENDERFCT)(const NGEOMETRY *pgeom, const NTRANSFORM_HNODE *ptransform_node);
typedef struct
{
	NGEOMETRY_RENDERFCT	RenderFunction;	// "THE" Render function called by NEngine to Render the geometry
	Nu16				StateSetID;		// store a NSTATESET_ID_ENUM on 16 bits
	Nu16				GeometryID;		// store a NGEOMETRY_FORMAT_ENUM on 16 bits
}NGEOMETRY_COMPONENT_CORE;

// ------------------------------------------------------------------------------------------------------------------------------------------
// Description Structure to Create NGEOMETRY_BASE
#define FLAG_NGEOMETRY_BASE_DESC_RENDER_BLEND				BIT_0
#define FLAG_NGEOMETRY_BASE_DESC_RENDER_ADDITIVE			BIT_1
typedef struct
{
	Nu32								Flags;
	NGEOMETRY_RENDER_FUNCTION_ID_ENUM	RenderFunctionID;
}NGEOMETRY_COMPONENT_BASE_DESC;*/
// ------------------------------------------------------------------------------------------------------------------------------------------
/*
inline NSTATESET_ID_ENUM NExtractStateSetID(const NGEOMETRY_COMPONENT_BASE_DESC *pdesc)
{

	// From RenderFunctionID we will get number of Texture Units
	Nu16 nbtexunit = NGET_GEOMETRYSPEC_TEXUNITNB( NGeometryStructureInfos_LUT[pdesc->RenderFunctionID] );
	if( nbtexunit )
	{
		if( ISFLAG_ON(pdesc->Flags,FLAG_NGEOMETRY_BASE_DESC_RENDER_BLEND) )
		{
			if( ISFLAG_ON(pdesc->Flags,FLAG_NGEOMETRY_BASE_DESC_RENDER_ADDITIVE) )
			{
				return NSTATESET_TEXTURE_ADDITIVEALPHABLENDING;
			}
			else
			{
				return NSTATESET_TEXTURE_ALPHABLENDING;
			}
		}
		else
		{
			if( ISFLAG_ON(pdesc->Flags,FLAG_NGEOMETRY_BASE_DESC_RENDER_ADDITIVE) )
			{
				return NSTATESET_TEXTURE_ADDITIVEBLENDING;
			}
			else
			{
				return NSTATESET_TEXTURE_NOBLENDING;
			}
		}
	}
	else
	{
		return NSTATESET_NOTEXTURE_NOBLENDING;
	}

	return 0; // just temporary to achieve a first compilation
}*/
/*
// ------------------------------------------------------------------------------------------------------------------------------------------
// Setup
inline void NSetupGeometryComponent_Base(NGEOMETRY_COMPONENT_CORE *pgeomcbase, const NGEOMETRY_FORMAT_ENUM created_geometry_id,const NGEOMETRY_COMPONENT_BASE_DESC *pdesc  )
{
	pgeomcbase->RenderFunction	= NGeometryRenderFunctions_LUT[pdesc->RenderFunctionID];
	pgeomcbase->GeometryID		= created_geometry_id;
	pgeomcbase->StateSetID		= NExtractStateSetID(pdesc);
}
// Clear
inline void NClearGeometryComponent_Base(NGEOMETRY_COMPONENT_CORE *pgeomcbase)
{
	//pgeomcbase->GeometryID	= 0;		// keep it for future structure identification ?
	//pgeomcbase->NRender		= NULL;		// kepp it for future structure using ?
	//pgeomcbase->StateSetID	= 0;		// kepp it for future structure using ?
}
// Copy
inline void NCopyGeometryComponent_Base(NGEOMETRY_COMPONENT_CORE *pgeodst,const NGEOMETRY_COMPONENT_CORE *pgeosrc)
{
	// Source and destination MUST have the same ID !!!
	NErrorIf(pgeosrc->GeometryID != pgeodst->GeometryID, NERROR_GEOMETRY_INCOMPATIBLE_TYPE);

	// pgeodst->GeometryID = pgeosrc->GeometryID; // Useless, they are the same !
	pgeodst->StateSetID			= pgeosrc->StateSetID;
	pgeodst->RenderFunction		= pgeosrc->RenderFunction;
}
// ------------------------------------------------------------------------------------------------------------------------------------------
// Fill up Geometry Component description
inline void NFillupGeometryComponent_BaseDesc(NGEOMETRY_COMPONENT_BASE_DESC *pdesc, const NGEOMETRY_RENDER_FUNCTION_ID_ENUM basedesc_render_fct_id, const Nu32 basedesc_flags )
{
	pdesc->RenderFunctionID	= basedesc_render_fct_id;
	pdesc->Flags			= basedesc_flags;
}
*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NGEOMETRYCOMPONENT_BASE_H

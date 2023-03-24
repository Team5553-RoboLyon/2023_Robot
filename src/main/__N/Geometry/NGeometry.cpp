#include "../NCStandard.h"
#include "../NType.h"
#include "../Containers/NArray.h"
#include "./Components/NGeometryAppearance.h"
#include "Ngeometry.h"
#include "./Components/NGeometryVertex.h"


extern NGEOMETRY_SPECS		NGeometrySpecs[];
extern NPRIMITIVE_SPECS		NPrimitiveSpecs[];
extern NVERTEX_SPECS		NVertexSpecs[];
extern NAPPEARANCE_SPECS	NAppearanceSpecs[];

// ------------------------------------------------------------------------------------------
// NSetupGeometry
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
NGEOMETRY* NSetupGeometry(NGEOMETRY *pgeom, const NGEOMETRY_DESC *pdesc)
{
	// Retrieve the geometry specifications with the incoming GeometryFormat, which is an index into the NGeometrySpecs[] table
	NGEOMETRY_SPECS		*prs = &NGeometrySpecs[pdesc->GeometryFormat];
	
	NACCUM_ID_ENUM		default_accum;
	Nu32				geom_ids;

	if(pdesc->UserStateSet)
	{
		// Specific User StateSet Flags have to be ... user flags and no Engine Flags !!!
		NErrorIf( ((pdesc->UserStateSet) & MASK_STATESET_ENGINE ) != 0, NERROR_STATESET_UNAUTHORIZED_FLAGS_USING );
		// Re-Define the Default Accumulator ID 
		// 1/ Retrieve Default associated Engine Accumulator StateSet and combine it with the user specific ones.
		// 2/ Look up for the Accumulator which owns this StateSet ...
		// 3/ Integrate the AccumumulatorID into the Geometry ID.
		geom_ids = NGeometrySpecs[pdesc->GeometryFormat].IDS;
		default_accum = NGetAccumulatorID( NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(geom_ids)].StateSet|pdesc->UserStateSet );
		geom_ids = NUPDATE_GEOMETRY_IDS__ACCUMULATOR_ID(geom_ids,default_accum);
	}
	else
	{
		geom_ids = prs->IDS;
	}
	// This function is going to setup a NGEOMETRY Structure, this structure has 3 components:
	// Core Mesh and Appearance...
	NSetupGeometryComponent_Core( &pgeom->Core, prs->RenderFunction, geom_ids );
	NSetupMesh(&pgeom->Mesh,NPrimitiveSpecs[NGET_GEOMETRY_IDS_PRIMITIVE_FORMAT(geom_ids)].SizeOfPrimitive,NVertexSpecs[NGET_GEOMETRY_IDS_VERTEX_FORMAT(geom_ids)].DirectAccess.SizeOfVertex,&pdesc->MeshCapacities);
	switch( NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(geom_ids) )
	{
		case NAPPEARANCE_FORMAT_NULL:
			Nmem0(&pgeom->Appearance,NAPPEARANCE);// Just fill up Appearance with ZEROs
			break;
		case NAPPEARANCE_FORMAT_TT:
			NSetupAppearance_TT( &pgeom->Appearance.TT );
			break;
		case NAPPEARANCE_FORMAT_TM:
			NSetupAppearance_TM( &pgeom->Appearance.TM );
			break;
		case NAPPEARANCE_FORMAT_ULTD_T:
			NSetupAppearance_ULTD_T( &pgeom->Appearance.ULTD_T,NGeometrySpecs[NGET_GEOMETRY_IDS_GEOMETRY_FORMAT(geom_ids)].UsedTextureUnitNumber );
			break;
		case NAPPEARANCE_FORMAT_ULTD_TM:
			NSetupAppearance_ULTD_TM( &pgeom->Appearance.ULTD_TM,NGeometrySpecs[NGET_GEOMETRY_IDS_GEOMETRY_FORMAT(geom_ids)].UsedTextureUnitNumber );
			break;
		default:
			NErrorIf(1,NERROR_GEOMETRY_UNSUPPORTED_APPEARANCE_FORMAT);
			Nmem0(&pgeom->Appearance,NAPPEARANCE);// Just fill up Appearance with ZEROs
			break;
	}
	// NUpdateGeometryAccumulatorID(pgeom); Not usefull here because all geometry structure is "empty" yet, with NULL blend and material pointers

	return pgeom;
}
// ------------------------------------------------------------------------------------------
// NCreateGeometry
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
NGEOMETRY* NCreateGeometry(const NGEOMETRY_DESC *pdesc)
{
	return NSetupGeometry(NEW(NGEOMETRY), pdesc);
}
// ------------------------------------------------------------------------------------------
// NClearGeometry
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NClearGeometry(void *pgeom)
{
	switch( NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(((NGEOMETRY*)pgeom)->Core.IDS) )
	{
		case NAPPEARANCE_FORMAT_NULL:
			Nmem0(&((NGEOMETRY*)pgeom)->Appearance,NAPPEARANCE);// Just fill up Appearance with ZEROs
			break;
		case NAPPEARANCE_FORMAT_TT:
			NClearAppearance_TT( &((NGEOMETRY*)pgeom)->Appearance.TT );
			break;
		case NAPPEARANCE_FORMAT_TM:
			NClearAppearance_TM( &((NGEOMETRY*)pgeom)->Appearance.TM );
			break;
		case NAPPEARANCE_FORMAT_ULTD_T:
			NClearAppearance_ULTD_T( &((NGEOMETRY*)pgeom)->Appearance.ULTD_T );
			break;
		case NAPPEARANCE_FORMAT_ULTD_TM:
			NClearAppearance_ULTD_TM( &((NGEOMETRY*)pgeom)->Appearance.ULTD_TM );
			break;
		default:
			NErrorIf(1,NERROR_GEOMETRY_UNSUPPORTED_APPEARANCE_FORMAT);
			Nmem0(&((NGEOMETRY*)pgeom)->Appearance,NAPPEARANCE);// Just fill up Appearance with ZEROs
			break;
	}
	NClearMesh(&((NGEOMETRY*)pgeom)->Mesh);
	NClearGeometryComponent_Core(&((NGEOMETRY*)pgeom)->Core);
}
// ------------------------------------------------------------------------------------------
// NDeleteGeometry
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
void NDeleteGeometry(void *pgeom)
{
	NClearGeometry(pgeom);
	Nfree(pgeom);
}
// ------------------------------------------------------------------------------------------
// NCopyGeometry
// ------------------------------------------------------------------------------------------
// Description :
//	Copy a geometry into an another one !
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
NGEOMETRY* NCopyGeometry(NGEOMETRY *pgeodst,const NGEOMETRY *pgeosrc)
{
	// Copy Core
	NCopyGeometryComponent_Core( &pgeodst->Core, &pgeosrc->Core);
	// Copy Mesh
	NCopyMesh( &pgeodst->Mesh, &pgeosrc->Mesh );
	// Copy Appearance
	// --->1:Destination Appearance Clear Process
	switch( NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeodst->Core.IDS) )
	{
		case NAPPEARANCE_FORMAT_NULL:
			// DST already cleared, Nothing to do ........
			break;
		case NAPPEARANCE_FORMAT_TT:
			NClearAppearance_TT(&pgeodst->Appearance.TT);
			break;
		case NAPPEARANCE_FORMAT_TM:
			NClearAppearance_TM(&pgeodst->Appearance.TM);
			break;
		case NAPPEARANCE_FORMAT_ULTD_T:
			NClearAppearance_ULTD_T(&pgeodst->Appearance.ULTD_T);
			break;
		case NAPPEARANCE_FORMAT_ULTD_TM:
			NClearAppearance_ULTD_TM(&pgeodst->Appearance.ULTD_TM);
			break;
		default:
			NErrorIf(1,NERROR_GEOMETRY_UNSUPPORTED_APPEARANCE_FORMAT);
			break;
	}
	// --->2:Source Appearance Setup & copy Process
	switch( NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeosrc->Core.IDS) )
	{
		case NAPPEARANCE_FORMAT_NULL:
			// SRC == DST == NULL ... Nothing to do ........
			pgeodst->Appearance = pgeosrc->Appearance; //...But just because it's a copy, we make a copy, even if we are copying nothing !
			break;
		case NAPPEARANCE_FORMAT_TT:
			NSetupAppearance_TT(&pgeodst->Appearance.TT);
			NCopyAppearance_TT( &pgeodst->Appearance.TT,&pgeosrc->Appearance.TT );
			break;
		case NAPPEARANCE_FORMAT_TM:
			NSetupAppearance_TM(&pgeodst->Appearance.TM);
			NCopyAppearance_TM( &pgeodst->Appearance.TM,&pgeosrc->Appearance.TM );
			break;
		case NAPPEARANCE_FORMAT_ULTD_T:
			NSetupAppearance_ULTD_T(&pgeodst->Appearance.ULTD_T,pgeosrc->Appearance.ULTD_T.TextureUnitTNumber);
			NCopyAppearance_ULTD_T( &pgeodst->Appearance.ULTD_T,&pgeosrc->Appearance.ULTD_T );
			break;
		case NAPPEARANCE_FORMAT_ULTD_TM:
			NSetupAppearance_ULTD_TM(&pgeodst->Appearance.ULTD_TM,pgeosrc->Appearance.ULTD_TM.TextureUnitTMNumber);
			NCopyAppearance_ULTD_TM( &pgeodst->Appearance.ULTD_TM,&pgeosrc->Appearance.ULTD_TM );
			break;
		default:
			NErrorIf(1,NERROR_GEOMETRY_UNSUPPORTED_APPEARANCE_FORMAT);
			break;
	}
	return pgeodst;
}

// ------------------------------------------------------------------------------------------
// NDuplicateGeometry
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
// 
// Out :
//
// ------------------------------------------------------------------------------------------
NGEOMETRY* NDuplicateGeometry(const NGEOMETRY *pgeosrc)
{
	NGEOMETRY_DESC	gdesc;

	
	NFillupGeometryDescEx(&gdesc,NGET_GEOMETRY_IDS_GEOMETRY_FORMAT(pgeosrc->Core.IDS), NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeosrc->Core.IDS)].StateSet & MASK_STATESET_USER,
								 pgeosrc->Mesh.PrimitiveArray.Capacity, pgeosrc->Mesh.PrimitiveArray.Size,
								 pgeosrc->Mesh.VertexArray.Capacity, pgeosrc->Mesh.VertexArray.Size);
	return NCopyGeometry( NSetupGeometry(NEW(NGEOMETRY),&gdesc), pgeosrc);
}

Nu8 NGetGeometryTextureUnitNumber(const NGEOMETRY *pgeom)
{
#ifdef _DEBUG
	// Check UsedTextureUnitNumber against flags when it's possible
#endif
	return NGeometrySpecs[NGET_GEOMETRY_IDS_GEOMETRY_FORMAT(pgeom->Core.IDS)].UsedTextureUnitNumber;
}

Nu8 NGetGeometryTexCoordSetNumber(const NGEOMETRY *pgeom)
{
	return NGET_VERTEX_SPECS_TEXCOORD_NUMBER( NVertexSpecs[NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)].DirectAccess.Flags );
}

Nu8 NGetGeometryColorSetNumber(const NGEOMETRY *pgeom)
{
	return NGET_VERTEX_SPECS_COLOR_NUMBER( NVertexSpecs[NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)].DirectAccess.Flags );
}

void NEraseGeometryMesh(NGEOMETRY *pgeom)
{
	NEraseMesh(&pgeom->Mesh);
}

void NSetGeometryAppearance(NGEOMETRY *pgeom, const NAPPEARANCE_RAW *praw)
{
	NErrorIf(!pgeom, NERROR_NULL_POINTER);
	NErrorIf( NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS)==NAPPEARANCE_FORMAT_NULL, NERROR_GEOMETRY_APPEARANCE_NOT_INCLUDED );
	NSetAppearance( &pgeom->Appearance, NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS),praw );
	NUpdateGeometryAccumulatorID(pgeom);
}

void NUnpackGeometryAppearance(NAPPEARANCE_RAW *praw, const NGEOMETRY *pgeom)
{
	NErrorIf(!pgeom, NERROR_NULL_POINTER);
	NErrorIf( NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS)==NAPPEARANCE_FORMAT_NULL, NERROR_GEOMETRY_APPEARANCE_NOT_INCLUDED );
	NUnpackAppearance(praw,NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS),&pgeom->Appearance);
}

void* NGetGeometryVertexPtr( const NVERTEX_DATA_ENUM vertex_component_id, const NGEOMETRY *pgeom, const Nu32 vertex_index )
{
	// TODO: Manage Mesh Format !!!! 
	// ... and generate an Error if Geometry Mesh are entirely on VBO...
	// ... So, instead of this first fake test it would be better to have something like NGetMeshSpecsFlags( NGET_GEOMETRY_IDS_MESH_FORMAT(pgeom->Core.IDS) ) to test
	// and look if the ARRAY flag is to on, because we will have NMESH_FORMAT_ARRAYS, NMESH_FORMAT_VBOS ... AND !!! NMESH_FORMAT_ARRAYS_VBO
	// So it seems that all the formats with valid ARRAYS are going to be fine !!! Lets thing about that and do it ....later !
	NErrorIf(NGET_GEOMETRY_IDS_MESH_FORMAT(pgeom->Core.IDS)!=NMESH_FORMAT_ARRAYS, NERROR_SYSTEM_GURU_MEDITATION);


	if( NVertexSpecs[NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)].TableAccess.SizeOf[vertex_component_id] )
	{
		return NGetArrayPtr(&pgeom->Mesh.VertexArray,vertex_index) + NVertexSpecs[NGET_GEOMETRY_IDS_VERTEX_FORMAT(pgeom->Core.IDS)].TableAccess.Offset[vertex_component_id];
	}
	else
	{
		return NULL;
	}
}

NTEXTURE *NGetGeometryTexture(const NGEOMETRY *pgeom,const Nu8 texture_unit)
{
	NErrorIf(!pgeom, NERROR_NULL_POINTER);
	//NErrorIf( NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS)==NAPPEARANCE_FORMAT_NULL, NERROR_GEOMETRY_APPEARANCE_NOT_INCLUDED );
	return NGetAppearance_Texture(&pgeom->Appearance, NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS),texture_unit );
}

void NSetGeometryTexture(NGEOMETRY *pgeom, const Nu8 texture_unit, const NTEXTURE *ptexture)
{
	NErrorIf(!pgeom, NERROR_NULL_POINTER);
	NErrorIf(NGET_GEOMETRY_IDS_TEXTUREUNIT_FORMAT(pgeom->Core.IDS)==NTEXTUREUNIT_FORMAT_NULL, NERROR_GEOMETRY_UNEXPECTED_TEXTUREUNIT_FORMAT);
	NSetAppearance_Texture(&pgeom->Appearance,NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS),texture_unit,ptexture);
}

void NSetGeometryMaterial(NGEOMETRY *pgeom, const NMATERIAL *pmaterial)
{
	NErrorIf(!pgeom, NERROR_NULL_POINTER);
	NErrorIf( (NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS)==NAPPEARANCE_FORMAT_NULL),NERROR_GEOMETRY_APPEARANCE_NOT_INCLUDED);
	NErrorIf(! NCHECK_APPEARANCE_SPECS_MATERIAL(NAppearanceSpecs[NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS)].Flags),NERROR_GEOMETRY_MATERIAL_NOT_SUPPORTED);
	pgeom->Appearance.TT.pMaterial = (NMATERIAL*)pmaterial;
	NUpdateGeometryAccumulatorID(pgeom);
}

void NSetGeometryBlend(NGEOMETRY *pgeom, const NBLEND *pblend)
{
	NErrorIf(!pgeom, NERROR_NULL_POINTER);
	NErrorIf( (NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS)==NAPPEARANCE_FORMAT_NULL),NERROR_GEOMETRY_APPEARANCE_NOT_INCLUDED);
	NErrorIf( !NCHECK_APPEARANCE_SPECS_BLEND(NAppearanceSpecs[NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS)].Flags),NERROR_GEOMETRY_BLENDPARAMS_NOT_SUPPORTED);
	pgeom->Appearance.Common.pBlend = (NBLEND*)pblend;
	NUpdateGeometryAccumulatorID(pgeom);
}



void NUpdateGeometryAccumulatorID(NGEOMETRY *pgeom)
{
	NErrorIf( !pgeom , NERROR_NULL_POINTER );
	// Re-Define the Geometry Accumulator ID in relation with geometry content
	// 1/ Retrieve current associated Accumulator StateSet 
	// 2/ Look up for the Blend and Material content and setup a new state set
	// 3/ get a new accumulator ID in relation with this appearance content if the new state set is different than the old one
	// 3/ Integrate the updated AccumumulatorID into the Geometry ID.
	NSTATESET newstateset = NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet;
	if(pgeom->Appearance.Common.pBlend)
	{
		FLAG_ON(newstateset,FLAG_STATESET_ZREAD|FLAG_STATESET_BLEND);
		FLAG_OFF(newstateset,FLAG_STATESET_ZWRITE);
	}
	else
	{
		FLAG_OFF(newstateset,FLAG_STATESET_BLEND);
		FLAG_ON(newstateset,FLAG_STATESET_ZREAD|FLAG_STATESET_ZWRITE);
	}

	// TODO ??? maybe we have to check if GEOMETRY VERTEX NORMALS are included too ????
	// Something like test that in the function "NSetGeometryMaterial" and CRASH if vertex don't have any normals ???
	if(pgeom->Appearance.Common.pMaterial)
		FLAG_ON(newstateset,FLAG_STATESET_LIGHT); 
	else
		FLAG_OFF(newstateset,FLAG_STATESET_LIGHT);
	if(NAccumulator[NGET_GEOMETRY_IDS_ACCUMULATOR_ID(pgeom->Core.IDS)].StateSet != newstateset)
		pgeom->Core.IDS = NUPDATE_GEOMETRY_IDS__ACCUMULATOR_ID(pgeom->Core.IDS,NGetAccumulatorID( newstateset ));
}

// ------------------------------------------------------------------------------------------
// NAPPEARANCE_CHECKMASK* NGetAppearanceCheckMask
// ------------------------------------------------------------------------------------------
// Description :
//	Get the appropriate Appearance Check Mask from an appearance 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
NAPPEARANCE_CHECKMASK* NGetGeometryAppearanceCheckMask(NAPPEARANCE_CHECKMASK *pcheckmask, const Nu8 blend_maskvalue, const Nu8 material_maskvalue, const Nu8 texture_maskvalue, const Nu8 txcmatrixupdater_maskvalue,const NGEOMETRY *pgeom )
{
	Nmem0(pcheckmask,NAPPEARANCE_CHECKMASK);

	switch( NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(pgeom->Core.IDS) )
	{
	case NAPPEARANCE_FORMAT_NULL:
		break;
	case NAPPEARANCE_FORMAT_TT:
		NSetupAppearanceCheckMask( pcheckmask, blend_maskvalue, material_maskvalue, texture_maskvalue, txcmatrixupdater_maskvalue, 2 );
		break;
	case NAPPEARANCE_FORMAT_TM:
		NSetupAppearanceCheckMask( pcheckmask, blend_maskvalue, material_maskvalue, texture_maskvalue, txcmatrixupdater_maskvalue, 1 );
		break;
	case NAPPEARANCE_FORMAT_ULTD_T:
		NSetupAppearanceCheckMask( pcheckmask, blend_maskvalue, material_maskvalue, texture_maskvalue, txcmatrixupdater_maskvalue, pgeom->Appearance.ULTD_T.TextureUnitTNumber );
		break;
	case NAPPEARANCE_FORMAT_ULTD_TM:
		NSetupAppearanceCheckMask( pcheckmask, blend_maskvalue, material_maskvalue, texture_maskvalue, txcmatrixupdater_maskvalue, pgeom->Appearance.ULTD_TM.TextureUnitTMNumber );
		break;
	default:
		NErrorIf(1,NERROR_GEOMETRY_UNSUPPORTED_APPEARANCE_FORMAT);
		break;
	}
	return pcheckmask;
}
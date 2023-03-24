#include "../NCStandard.h"
#include "../NCore.h"

#include "../Render/Renderable/NRenderable.h"
#include "../Render/Renderable/NRenderable_Extract.h"
#include "../Render/Renderable/NRenderable_ExtractSet.h"
#include "../Render/Renderable/NRenderable_UpdateAndExtract_LUT.h"

#include "../Render/NFrustum_Culling.h"
#include "../NViewport.h"
#include "../NErrorHandling.h"
#include "../NString.h"
#include "../Geometry/NGeometryBoundingVolume.h"
#include "../Geometry/NGeometryVertexTransformations.h"
#include "../Geometry/Components/NGeometryVertex.h"
#include "../Geometry/Components/NGeometryTextureCoordinate.h"
#include "../Timer/NUIThnAnimationPlayer.h"
#include "../Utilities/NUT_Shape.h"

#include "NUIClearWatchStack.h"
#include "NUIRenderableUpdate.h"
#include "NUI.h"

// #include "../NUT.h"

// GLobales
extern NARRAY					ActiveUIRootsList;	
// NARRAY						ActiveUIRootsList_LockedCopy;
// Nbool						bActiveUIRootsListUpdated;
extern NUI						*pFocusedUI;
extern Nu32						NEngineCoreFlags;


void NEnableGameStatesTouchEventBlocking(NUI *pui)
{
	NErrorIf(!pui,NERROR_NULL_POINTER);
	FLAG_ON(pui->Flags,FLAG_NUI_BLOCK_TOUCH_EVENT_FOR_GAMESTATES);
	// In that way, it's possible for user to change the used Blocking state from an Event_handle during the Event handling loop.
	// But changes are not going to work on the current started touch sequences which are not going to be canceled.
	// the changes are going to work only on the new touch sequence which are not going to "START" for Game State
}

void NDisableGameStatesTouchEventBlocking(NUI *pui)
{
	NErrorIf(!pui,NERROR_NULL_POINTER);
	FLAG_OFF(pui->Flags,FLAG_NUI_BLOCK_TOUCH_EVENT_FOR_GAMESTATES);
	// In that way, it's possible for user to change the used Blocking state from an Event_handle during the Event handling loop.
	// But changes are not going to work on the current started touch sequences which are not going to be canceled.
	// the changes are going to work only on the new touch sequence which are not going to "START" for Game State

}

void NBuildUITextMeshFromString(NUI *pui, const Nchar *pstring)
{
	NErrorIf(!pui->pText,NERROR_UI_FRAME_TEXT_DISPLAY_DEVICE_MISSING);
	NBuildTextMeshFromString(pui->pText,pstring);
	if( ISFLAG_ON(pui->FlagsXtd,FLAG_NUI_XTD_TEXT_PICKING_MESH ))
	{
		FLAG_ON( pui->Flags, FLAG_NUI_BBOX_UPDATE_ONCE );
	}
}


/*
void NGetUIGeometryDescTextureSize(NVEC3 *psize, const NUIGEOMETRY_DESC* pgd, const Nu8 texture_unit)
{
	NErrorIf( ISFLAG_OFF(pgd->Flags,FLAG_NUIGD_USEPARAM_APPEARANCE) || (!NGetAppearance_Raw_Texture(&((NUIGEOMETRY_DESC*)pgd)->Appearance_Raw,texture_unit)), NERROR_UIGEOMETRYDESC_TEXTURE_REQUESTED);
	NTEXTURE *ptexture = NGetAppearance_Raw_Texture(&((NUIGEOMETRY_DESC*)pgd)->Appearance_Raw,texture_unit);
	psize->x = NSize_PixToNorm_W(ptexture->size.x);
	psize->y = NSize_PixToNorm_H(ptexture->size.y);
	psize->z = 0.0f; // TODO: check maybe 1.0f is more convenient ...
}
*/



void NGetUITextDescStringSize( NVEC3 *psize, const NUITEXT_DESC* ptd, const NFONT *pfont, const Nchar *pstr, const NUISTRING_MAXSIZE_ENUM maxsize )
{
	NErrorIf(!pstr,NERROR_STRING_NULL);

	Nchar str[NUI_STRING_MAX_MAXSIZE]={0};

	if( (Nu32)strlen(pstr) >= (Nu32)maxsize )
	{
		NStrCopy(str,pstr,maxsize);
		NGetStringSize(psize,str,pfont);
	}
	else
	{
		NGetStringSize(psize,pstr,pfont);
	}
}

void NSetUITextDescSizeFromString( NUITEXT_DESC* ptd, const NFONT *pfont, const Nchar *pstr, const NUISTRING_MAXSIZE_ENUM maxsize )
{
	NGetUITextDescStringSize( &ptd->Placement.Size, ptd, pfont, pstr, maxsize );
	FLAG_ON(ptd->Flags,FLAG_NUIGD_USEPARAM_PLACEMENT_SIZE);
}

NTEXT*	NCreateUIText(const NUI *pui, const NEXTRACTSET highlevel_extractset, const NUITEXT_DESC *ptdesc)
{
	NTEXT_DESC						text_desc;

	NErrorIf(ISFLAG_OFF(ptdesc->Flags,FLAG_NUIGD_USEPARAM_PLACEMENT_SIZE),				NERROR_UI_PLACEMENTDESC_HAS_TO_BE_FULLY_DEFINED );
	NErrorIf(ISFLAG_OFF(ptdesc->Flags,FLAG_NUIGD_USEPARAM_PLACEMENT_RELATIVE_POSITION),	NERROR_UI_PLACEMENTDESC_HAS_TO_BE_FULLY_DEFINED );
	NErrorIf(ISFLAG_OFF(ptdesc->Flags,FLAG_NUIGD_USEPARAM_PLACEMENT_ANCHOR),			NERROR_UI_PLACEMENTDESC_HAS_TO_BE_FULLY_DEFINED );
	NErrorIf(ISFLAG_OFF(ptdesc->Flags,FLAG_NUIGD_USEPARAMS_PLACEMENT_PARENT),			NERROR_UI_PLACEMENTDESC_HAS_TO_BE_FULLY_DEFINED );


	memset(&text_desc,0,sizeof(NTEXT_DESC));
	
	// Extract set ... 
	// UIText doesn't use the same Renderable than all the other UI geometries. It has its own renderable,
	// with its own update and EXTRACT process.
	// N wants to stay consistent between UI and text ( UI3D means TEXT 3D and HUD UI means HUD text ...)
	// So, we are going to recover the HighLevel extract set flags we are interested in, to setup the text description HighLevel ExtractSet in
	// the same way ...
	text_desc.HighLevelExtractSet = highlevel_extractset;
/*
	if(	ISFLAG_ON(pui->Flags,FLAG_NUI_3D) )
	{
		FLAG_ON(text_desc.HighLevelExtractSet,FLAG_NEXTRACTSET_3D); 
		// Its not necessary to enable Culling for UI 3D text, UI Culling will be enough if it exists !
		FLAG_OFF(text_desc.HighLevelExtractSet,FLAG_NEXTRACTSET_FRUSTUMCULLING);
	}
	else // The default UI UI setting: HUD
	{
		FLAG_ON(text_desc.HighLevelExtractSet,FLAG_NEXTRACTSET_HUD);
		FLAG_OFF(text_desc.HighLevelExtractSet,FLAG_NEXTRACTSET_FRUSTUMCULLING);
	}
*/

	if( ISFLAG_ON(ptdesc->Flags,FLAG_NUIGD_USEPARAM_GEOMETRY_FORMAT) )
	{
		text_desc.GeometryFormat	= ptdesc->GeometryFormat;
		FLAG_ON(text_desc.Flags,FLAG_NTEXT_DESC_USEPARAM_GEOMETRY_FORMAT);
	}
	else
	{
		FLAG_OFF(text_desc.Flags,FLAG_NTEXT_DESC_USEPARAM_GEOMETRY_FORMAT);
		// In that way, default text GEOMETRY_FORMAT will be used !
	}
	
	if( ISFLAG_ON(ptdesc->Flags,FLAG_NUIGD_USEPARAM_APPEARANCE) )
	{
		text_desc.Appearance_Raw	= ptdesc->Appearance_Raw;
		FLAG_ON(text_desc.Flags,FLAG_NTEXT_DESC_USEPARAM_APPEARANCE);
	}
	else
	{
		FLAG_OFF(text_desc.Flags,FLAG_NTEXT_DESC_USEPARAM_APPEARANCE);
	}
	
	if( ISFLAG_ON(ptdesc->Flags,FLAG_NUIGD_USEPARAM_TEXT_ALIGNMENT) )
	{
		FLAG_ON(text_desc.Flags,FLAG_NTEXT_DESC_USEPARAM_ALIGNMENT);
		text_desc.Alignment	= ptdesc->Alignment;
	}


	FLAG_ON(text_desc.Flags,FLAG_NTEXT_DESC_USEPARAM_USEREXTRACTSET);
	FLAG_ON(text_desc.Flags,FLAG_NTEXT_DESC_USEPARAM_TRANSFORMHNODE);
	FLAG_ON(text_desc.Flags,FLAG_NTEXT_DESC_USEPARAM_FONT);
	FLAG_ON(text_desc.Flags,FLAG_NTEXT_DESC_USEPARAM_ANCHOR);
	FLAG_ON(text_desc.Flags,FLAG_NTEXT_DESC_USEPARAM_CONSTRUCTION_ORIGIN);

	if( ISFLAG_ON(ptdesc->Flags,FLAG_NUIGD_TEXT_USE_UI_RENDERABLE_TRANSFORMHNODE) )
		text_desc.pThn				= pui->pRenderable->pTransformHNode;
	else
		text_desc.pThn				= pui->pUITransformHNode;

	text_desc.ColorBuild			= ptdesc->ColorBuild;
	text_desc.pFont					= pui->pStyle->pFont;
	text_desc.Anchor				= ptdesc->Placement.Anchor;
	NGetUIPlacementAbsoluteComputedPosition( &text_desc.ConstructionOrigin,&ptdesc->Placement );
	
	return NCreateText(&text_desc);
}

NGEOMETRY*	NCreateUIGeometry(NUI *pui, const NUIGEOMETRY_DESC *pgdesc)
{
	NGEOMETRY_DESC		gdesc;
	NGEOMETRY			*pgeom;
	Nu32				i;
	Nbool				bupdatepickingmesh;


	// Incoming Description structure has to be fully setup !
	// So, Checking 
	NErrorIf(ISFLAG_OFF(pgdesc->Flags,FLAG_NUIGD_USEPARAM_PLACEMENT_SIZE),				NERROR_UI_PLACEMENTDESC_HAS_TO_BE_FULLY_DEFINED );
	NErrorIf(ISFLAG_OFF(pgdesc->Flags,FLAG_NUIGD_USEPARAM_PLACEMENT_RELATIVE_POSITION),	NERROR_UI_PLACEMENTDESC_HAS_TO_BE_FULLY_DEFINED );
	NErrorIf(ISFLAG_OFF(pgdesc->Flags,FLAG_NUIGD_USEPARAM_PLACEMENT_ANCHOR),			NERROR_UI_PLACEMENTDESC_HAS_TO_BE_FULLY_DEFINED );
	NErrorIf(ISFLAG_OFF(pgdesc->Flags,FLAG_NUIGD_USEPARAMS_PLACEMENT_PARENT),			NERROR_UI_PLACEMENTDESC_HAS_TO_BE_FULLY_DEFINED );

	// To optimize memory AND to avoid Reallocation, the new created geometry has to be preallocated 
	// at the beginning of the SetUpUI Process ... If not ... CRASH HERE ... in debug mode ONLY.
	// ... because it's not a BUG !!! Just a crash to force User managing geometries allocation in a better Way ...
	NErrorIf( NGetArraySize(&pui->pRenderable->GeomArray)==NGetArrayCapacity(&pui->pRenderable->GeomArray),NERROR_UI_OVERLOADED_GEOMETRY_CAPACITY );

	// ... but in release, manage the potential reallocation ...
	// Actually there is only one problem, "pPickingGeometryMesh" can target the mesh of one of the geometries of the Renderable 
	bupdatepickingmesh = NFALSE;
	if(pui->pPickingGeometryMesh && pui->pRenderable->GeomArray.Size == pui->pRenderable->GeomArray.Capacity )
	{
		// Check if "pPickingGeometryMesh" belongs to a renderable geometry
		pgeom = (NGEOMETRY *)pui->pRenderable->GeomArray.pFirst;
		for(i=0;i<pui->pRenderable->GeomArray.Size;i++,pgeom++)
		{
			if(&pgeom->Mesh == pui->pPickingGeometryMesh)
			{
				bupdatepickingmesh = NTRUE;
				pui->pPickingGeometryMesh = (NMESH*)i;
				break;
			}
		}
	}

	// First of All : Check/Finalize NUIGEOMETRY_DESC.Positioning member setup.
	// It means setup with default values all the param not setup by user.
	if( ISFLAG_ON(pgdesc->Flags,FLAG_NUIGD_USEPARAM_CUSTOM_GEOMETRY) )
	{
		NVEC3			v;
// 		NSetUpGeometryDesc_Triangle(&gdesc,NSTATESET_NOTEXTURE_NOBLENDING, NGetGeometryPrimitiveArraySize(pgdesc->pCustomGeometry),NGetGeometryVertexArraySize(pgdesc->pCustomGeometry),NULL);
// 		pgeom = NRenderableGeometryPushBack(pui->pRenderable,NSetUpGeometry(&geom,&gdesc));
//		NCopyGeometry(pgeom,pgdesc->pCustomGeometry);

		Nmem0(&gdesc,NGEOMETRY_DESC);
		NFillupGeometryDescEx(&gdesc,(NGEOMETRY_FORMAT_ENUM)NGET_GEOMETRY_IDS_GEOMETRY_FORMAT(pgdesc->pCustomGeometry->Core.IDS),0,
								pgdesc->pCustomGeometry->Mesh.PrimitiveArray.Capacity, pgdesc->pCustomGeometry->Mesh.PrimitiveArray.Size,
								pgdesc->pCustomGeometry->Mesh.VertexArray.Capacity, pgdesc->pCustomGeometry->Mesh.VertexArray.Size );
		pgeom = NGeometryEmplaceBack( pui->pRenderable,&gdesc);
		NCopyGeometry(pgeom,pgdesc->pCustomGeometry);

		// Positioning Custom geometry 
		// 1/	Custom geometry is supposed to be build in the right way ( right way for UI used) that means with the LEFT TOP BACK corner
		//		of its BoundingBox at 0,0,0. !!! TODO: it must be LEFT BOTTOM BACK !!! so change it with change ANCHOR convention ( ANCHOR_MIDHEIGHT should be 0.5f and not -0.5f as it is now !) 
		// 2/	Set Geometry Position using the UIPlacement System.

		NAABB	aabb;
		NGetGeometryAABB(pgeom,&aabb);
		NGetUIPlacementShifting(&v, &pgdesc->Placement);
		v.x -= aabb.fXMin; // Left
		v.y -= aabb.fYMax; // Top ( TO DO: change to Bottom, so this line will be: v.y -= aabb.fYMin;
		v.z -= aabb.fZMin; // Back
		NShiftGeometry(pgeom,&v);
	}
	else // A simple rectangle is requested !
	{
		NUT_QUAD_DESC		quad_desc;

		Nmem0(&gdesc,NGEOMETRY_DESC);
		if( ISFLAG_ON(pgdesc->Flags,FLAG_NUIGD_USEPARAM_GEOMETRY_FORMAT) )
		{
			gdesc.GeometryFormat = pgdesc->GeometryFormat;
		}
		else
		{
			gdesc.GeometryFormat = DEFAULT_NUI_GEOMETRY_FORMAT;
		}
		
		if( ISFLAG_ON(pgdesc->Flags,FLAG_NUIGD_USEPARAM_USERSTATESET) )
		{
			gdesc.UserStateSet = pgdesc->UserStateSet;
		}
		else
		{
			gdesc.UserStateSet = DEFAULT_NUI_GEOMETRY_USERSTATESET;
		}

		// At minimum, a UIGeometry needs these vertex attrib: "Vertex position 3f" and "color 4f"
#ifdef _DEBUG
		Nu32 dbg_ids = NGetGeometrySpecs( gdesc.GeometryFormat )->IDS;
		NErrorIf( !NCHECK_VERTEX_SPECS_POSITION_3f( NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(dbg_ids))->DirectAccess.Flags ), NERROR_GEOMETRY_UNSUPPORTED_MESH_DESCRIPTION );
		NErrorIf( !NCHECK_VERTEX_SPECS_COLOR0_4f( NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(dbg_ids))->DirectAccess.Flags ),	NERROR_GEOMETRY_UNSUPPORTED_MESH_DESCRIPTION );
#endif
		NFillupMeshCapacities(&gdesc.MeshCapacities,2,4);
		pgeom = NGeometryEmplaceBack( pui->pRenderable,&gdesc );
		// A texture ?	Why not, but if it's requested ... Created geometry	needs NMAPS component and, of course, TexCoordinates !
		// Note: It's important to define this texture right now, before creating and inserting the Quad...(it will use this texture !)
/*
		if( ISFLAG_ON(pgdesc->Flags,FLAG_NUIGD_USEPARAM_TEXTURE) )
		{
			NErrorIf( !NGETMESHDESC__VERTEX_TEXCOORD0_2f(NGetGeometrySpecs( gdesc.GeometryFormat )->MeshDescription), NERROR_GEOMETRY_UNSUPPORTED_MESH_DESCRIPTION );
			//NErrorIf( NGetGeometrySpecs( gdesc.GeometryFormat )->AppearanceSpecs & FLAG_NAPPEARANCE_DESC_TUTEXTURE != FLAG_NAPPEARANCE_DESC_TUTEXTURE, NERROR_GEOMETRY_UNSUPPORTED_APPEARANCE_DESCRIPTION );
			//NErrorIf( NGetGeometrySpecs( gdesc.GeometryFormat )->TextureUnitNumber < 1, NERROR_GEOMETRY_UNSUPPORTED_TEXTUREUNIT_NUMBER );
			NSetGeometryTexture(pgeom,0,pgdesc->pTexture);
		}
*/
		if( ISFLAG_ON(pgdesc->Flags,FLAG_NUIGD_USEPARAM_APPEARANCE) )
		{
			NSetGeometryAppearance(pgeom,&pgdesc->Appearance_Raw);
		}

		// Insert the Quad into the created geometry:
		/*
		#define FLAG_NUTSCD_FIGURESIZE_FROM_TEXTURESIZE						BIT_0
				#define FLAG_NUTSCD_USEPARAM_SCRUV_RECT						BIT_1
				#define FLAG_NUTSCD_USEPARAM_UV_RECT						BIT_2
				#define FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE				BIT_3
		#define FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE_DIRECTORVECTOR		BIT_4
				#define FLAG_NUTSCD_USEPARAM_CONSTRUCTION_ORIGIN			BIT_5
		#define FLAG_NUTSCD_USEPARAM_ANCHOR									BIT_6
		#define FLAG_NUTSCD_USEPARAM_SCALE									BIT_7
				#define FLAG_QUAD_DESC_USEPARAM_COLORRANGE					BIT_8
		#define FLAG_NUTSCD_USEPARAM_TEXTUREUNIT_ID							BIT_9
				#define FLAG_NUTSCD_USEPARAM_SIZE							BIT_10
		*/

		memset(&quad_desc,0,sizeof(NUT_QUAD_DESC));
		FLAG_ON(quad_desc.CoreDesc.Flags,FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE);
		quad_desc.CoreDesc.Construction_Plane = _PLANE_XY;
		
		// Texture Coordinate and color Building: just copy data
		quad_desc.CoreDesc.TexCoordBuild	= pgdesc->TexCoordBuild;
		quad_desc.CoreDesc.ColorBuild		= pgdesc->ColorBuild;
		
		// Size
		FLAG_ON(quad_desc.CoreDesc.Flags,FLAG_NUTSCD_USEPARAM_SIZE);
		quad_desc.CoreDesc.Size.x	= pgdesc->Placement.Size.x;
		quad_desc.CoreDesc.Size.y	= pgdesc->Placement.Size.y;

		// Position/Anchor
		// Because Quad Size is already knew it's possible to define 'quad_desc.CoreDesc.ConstructionOrigin'
		// By calculation from the whole "PlacementDesc" chain, anchoring included and so, it's not necessary
		// to pass Anchoring params to the  "Quad_desc" ( by opposition, it will be necessary to keep building Anchoring outside the 
		// PlacementDesc calculation process for NTEXT, because final TEXT object size is not known in advance...). 
		FLAG_ON(quad_desc.CoreDesc.Flags,FLAG_NUTSCD_USEPARAM_CONSTRUCTION_ORIGIN);
		NGetUIPlacementShifting( &quad_desc.CoreDesc.ConstructionOrigin,&pgdesc->Placement );
		NUT_InsertQuad(pgeom,&quad_desc);
	}


	// In case of Geometry array reallocation.
	if(	bupdatepickingmesh )
	{
		pui->pPickingGeometryMesh = &((NGEOMETRY*)NGetArrayPtr(&pui->pRenderable->GeomArray,(Nu32)pui->pPickingGeometryMesh))->Mesh;	
	}

	return pgeom;
}

static inline NMESH* _create_custom_pickingmesh(const NMESH* psrc_mesh)
{
	NErrorIf(!psrc_mesh,NERROR_NULL_POINTER);
	NErrorIf(psrc_mesh->PrimitiveArray.Size < 1, NERROR_GEOMETRY_NOT_ENOUGH_PRIMITIVE);
	NErrorIf(psrc_mesh->VertexArray.Size < 3, NERROR_GEOMETRY_NOT_ENOUGH_VERTEX);

	NMESH_CAPACITIES gmdesc;
	memset(&gmdesc,0,sizeof(NMESH_CAPACITIES));
	gmdesc.PrimitiveArray_Capacity	= psrc_mesh->PrimitiveArray.Capacity;
	gmdesc.VertexArray_Capacity		= psrc_mesh->VertexArray.Capacity;
	gmdesc.PrimitiveArray_Size		= 0;
	gmdesc.VertexArray_Size			= 0;
	NMESH *ppickingmesh				= NCreateMesh( sizeof(NTRIANGLE), sizeof(NVERTEX_V3) , &gmdesc);
	// Impossible to use "NCopyGeometryMesh()" here because we don't have any details
	// So, by hand copy ! to copy only what we need, Vertex POSITION and primitive.
	// For NPRIMITIVE we already check that "pCustomPickingMesh->PrimitiveArray" is about NTRIANGLE
	NCopyArray(&ppickingmesh->PrimitiveArray,&psrc_mesh->PrimitiveArray);
	// For Vertex, we are just going to copy Vertex position in "blind mode" without knowing incoming vertex format.
	// Because NVERTEX_V3  is just a NVEC3 we are going to fill up the Vertex array ...
	NVEC3 *psrcv = (NVEC3*)psrc_mesh->VertexArray.pFirst;
	for(Nu32 i=psrc_mesh->VertexArray.Size;i!=0;i--,psrcv =(NVEC3*)((NBYTE*)psrcv+psrc_mesh->VertexArray.ElementSize) )
	{
		NPushBackMeshVertex(ppickingmesh,(NBYTE*)psrcv,sizeof(NVERTEX_V3) );		
	}
	return ppickingmesh;
}
// ------------------------------------------------------------------------------------------
// NCreateUI
// ------------------------------------------------------------------------------------------
// Description :
//	
// ------------------------------------------------------------------------------------------
// In  :
//		NUI		*parent		Parent UI ( a menu, a layer, ... all NUI* are working
//		NUI_DESC	*uidesc	a pointer on uidesc structure which contains all the things
//									necessary t build in the UI.
//									see bellow some details about 'Flags' parameter of 'uidesc'
//		Nu32 user32					User define value
// Out :
//		
// ------------------------------------------------------------------------------------------
// Understand 'Flags' for UI Creation:
/*
	1)
	Usually a UI has a visible GFX part on screen. This GFX part is a composition of several geometries (from one, the primary, to n...)
	By setting on FLAG_NUI_NO_GEOMETRY_EXTRACTION 'NCreateUI' is going to bind with the UI a specific UpdateAndExtract function which
	doesn't extract geometries for rendering. So, this kind of UI is INVISIBLE. All others things are performed normally as requested by user.
	It's just at the end, there is no geometries extraction.
	By default, the UpdateAndExtract function bind with the UI extract all geometries, obviously.
			FLAG_ON(uidesc.Flags,FLAG_NUI_NO_GEOMETRY_EXTRACTION);
			or
			Nothing.
	
	3)
	Usually, UI's graphic part is a rectangle with or without texture.
	But sometimes graphic part can be a complex 3D object.
	Or simply nothing with none of this 2 flags set to ON.
	By setting on FLAG_NUI_SIMPLERECT 'NCreateUI' is going to create a simple quad as primary geometry.
	By setting on FLAG_NUI_COMPLEX3DOBJECT 'NCreateUI' is going to use ... TODO 
			FLAG_ON(uidesc.Flags,FLAG_NUI_SIMPLERECT);
			or
			FLAG_ON(uidesc.Flags,FLAG_NUI_COMPLEX3DOBJECT);
			or
			Nothing.
	
	4)
	Default Focus research process uses UI BoundingBox to test if Touch are On the UI or not.
	By setting on FLAG_NUI_FINE_TESTING_METHOD an accurate test is performed after the bounding box test.
	For sure its only working with a simple rect or a complex 3d object and doesn't work with "nothing".
			FLAG_ON(uidesc.Flags,FLAG_NUI_FINE_TESTING_METHOD);
			or
			Nothing.

	5a)
	! ONLY If FLAG_NUI_SIMPLERECT is set to ON !
	By setting on FLAG_NUI_USEPARAM_BACKTEXTURE 'NCreateUI' is going to use NUI_DESC.pBackTexture as a texture to map the primary quad
	created previously thanks to FLAG_NUI_SIMPLERECT .
	Without FLAG_NUI_USEPARAM_BACKTEXTURE, the quad is going to be untextured.
			FLAG_ON(uidesc.Flags,FLAG_NUI_USEPARAM_BACKTEXTURE);
			or
			Nothing.

	5b)
	! ONLY If FLAG_NUI_USEPARAM_BACKTEXTURE is set to ON !
	By setting on FLAG_NUI_USEPARAM_BACK_UV 'NCreateUI' is going to use NUI_DESC.pBackUV as texture UV for the Back Texture.
	By setting on FLAG_NUI_USEPARAM_BACK_SCRUV 'NCreateUI' is going to use NUI_DESC.pBackScrUV as texture UV for the Back Texture.
	Without one of these 2 flags, 'NCreateUI' is going to use default UV coordinates (0,0,1,1) as texture UV for the Back Texture.
			FLAG_ON(uidesc.Flags,FLAG_NUI_USEPARAM_BACK_UV);
			or
			FLAG_ON(uidesc.Flags,FLAG_NUI_USEPARAM_BACK_SCRUV);
			or
			Nothing.

	6a)
	By setting on FLAG_NUI_USEPARAM_SECONDARYTEXTURE 'NCreateUI' is going to create a secondary quad 
	and use NUI_DESC.pSecondaryTexture as a texture to map this secondary quad.
	Without FLAG_NUI_USEPARAM_SECONDARYTEXTURE, there is no secondary textured quad.
	Notice that its not necessary to have a Simple Rect created to specify a secondary texture.
			FLAG_ON(uidesc.Flags,FLAG_NUI_USEPARAM_SECONDARYTEXTURE);
			or
			Nothing.

	6b)
	! ONLY If FLAG_NUI_USEPARAM_SECONDARYTEXTURE is set to ON !
	By setting on FLAG_NUI_USEPARAM_SECONDARY_UV 'NCreateUI' is going to use NUI_DESC.pSecondaryUV as texture UV for the Secondary Texture.
	By setting on FLAG_NUI_USEPARAM_SECONDARY_SCRUV 'NCreateUI' is going to use NUI_DESC.pSecondaryUV as texture UV for the Secondary Texture.
	Without one of these 2 flags, 'NCreateUI' is going to use default UV coordinates (0,0,1,1) as texture UV for the Secondary Texture.
			FLAG_ON(uidesc.Flags,FLAG_NUI_USEPARAM_SECONDARY_UV);
			or
			FLAG_ON(uidesc.Flags,FLAG_NUI_USEPARAM_SECONDARY_SCRUV);
			or
			Nothing.
	
	7a)
	When 'NCreateUI' creates quad geometries ( primary and secondary. Have a look on 5a & 6a ) it needs to know how determine their sizes.
	-
	By setting on FLAG_NUI_USEPARAM_FRAMESIZE 'NCreateUI' is going to use NUI_DESC.FrameSize to create the first quad (even if this quad is textured with the 'Back Texture').
	By setting on FLAG_NUI_USEPARAM_SECONDARYFRAMESIZE 'NCreateUI' is going to use NUI_DESC.SecondaryFrameSize to create the second quad (even if this quad is textured with the 'Secondary Texture').
	-
	By setting on FLAG_NUI_USE_TEXTURESIZE_AS_FRAMESIZE 'NCreateUI' is going to use current back texture size to create the primary quad.
	By setting on FLAG_NUI_USE_SECONDARYTEXTURESIZE_AS_SECONDARYFRAMESIZE 'NCreateUI' is going to use current secondary texture size to create the second quad.
	-
	By setting on FLAG_NUI_USE_CAPTIONSIZE_AS_FRAMESIZE 'NCreateUI' is going to use current caption size to create the primary quad.
	By setting on FLAG_NUI_USE_CAPTIONSIZE_AS_SECONDARYFRAMESIZE 'NCreateUI' is going to use current caption size to create the second quad.
	For those 2 last cases, a valid caption has to be set up. ( FLAG_NUI_USEPARAM_CAPTION set to ON with a valid caption into NUI_DESC.Caption)
	-
	By default, without one of these 2 flags, 'NCreateUI' uses the 2 default values 'DEFAULT_UISIZE_X' and 'DEFAULT_UISIZE_Y'
			FLAG_ON(uidesc.Flags,FLAG_NUI_USE_TEXTURESIZE_AS_FRAMESIZE);
			or
			FLAG_ON(uidesc.Flags,FLAG_NUI_USEPARAM_FRAMESIZE);
			or
			Nothing.

	7b)
	When 'NCreateUI' creates quad geometries ( primary and secondary. Have a look on 5a & 6a ) AND Caption (text) its possible to shift them.
	-
	By setting on FLAG_NUI_USEPARAM_FRAME_SHIFTING 'NCreateUI' is going to use NUI_DESC.FrameShifting to create the first quad.
	-
	By setting on FLAG_NUI_USEPARAM_SECONDARYFRAME_SHIFTING 'NCreateUI' is going to use NUI_DESC.SecondaryFrameShifting to create the Second quad.
	-
	By setting on FLAG_NUI_USEPARAM_CAPTION_SHIFTING 'NCreateUI' is going to use NUI_DESC.CaptionShifting to create the caption text.
	-
	By default, (FLAG set to OFF) 'shifting vectors' are set like this:
		for caption:			NVec3Set(&text_desc.CursorShifting,0,0,DEFAULT_NUI_Z_CAPTION);
		for UI:				NVec3Set(&quad_desc.CenterShifting,0,0,DEFAULT_NUI_Z_FRAME); 
		for secondary UI:	NVec3Set(&quad_desc.CenterShifting,0,0,DEFAULT_NUI_Z_SECONDARYFRAME);

	8a)
	Each UI needs a BoundingBox to be detected by the UIFocus Research Process. ( Actually, the UI with _ENABLE || _SHOW flags set to OFF doesn't ).
	By setting on FLAG_NUI_USEPARAM_BOUNDINGBOX 'NCreateUI' is going to use NUI_DESC.BoundingBox as UI's BBOX values (at least at the beginning).
	By default, BBOX values are automatically calculated with the primary geometry (SIMPLE RECT or COMPLEX 3D OBJECT) or set to default values (screen size) if there is no primary geometry.
			FLAG_ON(uidesc.Flags,FLAG_NUI_USEPARAM_BOUNDINGBOX);
			or
			Nothing.

	8b)
	At Each update UI can change (position, rotation, etc ... ). And it's important too update properly it's BoundingBox.
	By setting on FLAG_NUI_UPDATE_BBOX_FROM_CHILDBBOX UI is going to update its BBOX from all its Children BBOX instead from its own primary geometry (SIMPLE RECT or COMPLEX 3D OBJECT).
	So, it will be updated each time a child BBox is updated.
	By setting on FLAG_NUI_UPDATE_BBOX_ASNECESSARY 'NCreateUI' is going to update the UI BBOX each time its necessary from its primary geometry.
	By setting on FLAG_NUI_UPDATE_BBOX_ISUPONLY 'NCreateUI' is going to update the UI BBOX from its primary geometry only when UI go back to its 'UP' state.
	By default, UI BBOX is NEVER updated.
			FLAG_ON(uidesc.Flags,FLAG_NUI_UPDATE_BBOX_FROM_CHILDBBOX);
			or
			FLAG_ON(uidesc.Flags,FLAG_NUI_UPDATE_BBOX_ASNECESSARY);
			or
			FLAG_ON(uidesc.Flags,FLAG_NUI_UPDATE_BBOX_ISUPONLY);
			or
			Nothing.
	
	9)
	Each UI can have a caption on it ( with a maximum size of "NUI_FRAMECAPTION_SIZE" characters ).
	By setting on FLAG_NUI_USEPARAM_CAPTION 'NCreateUI' is going to use NUI_DESC.Caption as UI's caption.
	By default a UI has no caption.
			FLAG_ON(uidesc.Flags,FLAG_NUI_USEPARAM_CAPTION);
			or
			Nothing.

	10)
	Each time a Touch starts the Focus research starts to give focus to the right branch (...->parent->child->...) of the menu tree.
	Usually a UI with the focus can have all its children without the focus:
	when the touch position is inside the UI BBox but outside of all the children frames Bbox.
	By setting on FLAG_NUI_FOCUSED_AS_ITS_CHILDREN', a UI can not get and keep the focus if no child has it.
	By setting on FLAG_NUI_REFUSE_TOUCH_FOCUS the UI will never get the focus, same for children. 
	By default a UI can have the focus if touch starts on it.
			FLAG_ON(uidesc.Flags,FLAG_NUI_FOCUSED_AS_ITS_CHILDREN);
			or
			FLAG_ON(uidesc.Flags,FLAG_NUI_REFUSE_TOUCH_FOCUS);
			or
			Nothing.

	11)
	Some types of UI are animated. They change (animation) in relation with their internal status, like push button for example.
	By setting on FLAG_NUI_ANIMATED_FRAME 
	By setting on FLAG_NUI_SMOOTH_ANIMATED_FRAME_TRANSITION ( with FLAG_NUI_ANIMATED_FRAME set to ON) all the UI's animations are going to be smoother
	with intermediate steps calculated between each key.
	By default these flags are set to OFF and the created UI is not animated. 
	That doesn't mean UI states are not updated, it's just there is no visible change on screen.
	For sure these flags have an effect only with animated UI types.
			FLAG_ON(uidesc.Flags,FLAG_NUI_ANIMATED_FRAME);
			and (or without)
			FLAG_ON(uidesc.Flags,FLAG_NUI_SMOOTH_ANIMATED_FRAME_TRANSITION);
			or
			Nothing.

			! ANIMATED FRAME TYPES !:
					NUI_FRAME_TYPE_BUTTON
					... TODO: complete this list.
	
*/
NUI* NCreateUI(NUI *parent, NUI_DESC *uidesc, const Nu32 user32)
{
	return NSetupUI(parent,NEW(NUI),uidesc,user32);
}

NUI_COLORSET_STATE_ENUM NExtractUIDescColorState(const NUI_DESC *puidesc)
{
	// which ColorSet State ? ENABLE or DISABLE ?( Obviously, it can not be FOCUS )
	if( ISFLAG_ON(puidesc->Flags_Style,FLAG_NUIDS_COLOR_UPDATE) && ISFLAG_ON(puidesc->Flags_Core, FLAG_NUIDC_DISABLE)  )
	{
			return NUI_COLORSET_DISABLE;
	}
	else
	{
			return NUI_COLORSET_ENABLE;
	}
}
NUI* NSetupUI(NUI *parent, NUI *pui, NUI_DESC *uidesc, const Nu32 user32)
{
	NRENDERABLE_DESC	rdesc;
	NGEOMETRY			*pbackground_geometry;
	NGEOMETRY			*picon_geometry;
// 	Nf32				zfront;							// for Z spread 
// 	NAABB				aabb;							// for z spread

	NPushUISetUpProcess( pui, uidesc);
	
	// + -------------------------------------------------------------------------------------------------------------------------
	// + FIRST STEPS ...
	memset(pui,0,sizeof(NUI));
	NHNodeChildrenListInitialization((NHIERARCHY_NODE*)pui);
	// + -------------------------------------------------------------------------------------------------------------------------
	// + Setup HNODE at the end or at the beginning of the parent children list ...
	// + SetupUI doesn't use 'NSetUIParent_BeginningInsert/NSetUIParent_EndInsert'
	// + Because these 2 functions check and update some Flags that are not setup yet ...
	if( ISFLAG_ON( uidesc->Flags_Core, FLAG_NUIDC_PARENT_CHILDRENLIST_BEGINNING_INSERTION ) )
	{
		NHNodeSetParent_BeginningInsert((NHIERARCHY_NODE*)parent,(NHIERARCHY_NODE*)pui );
	}
	else
	{
		NHNodeSetParent_EndInsert((NHIERARCHY_NODE*)parent,(NHIERARCHY_NODE*)pui );
	}

	// + -------------------------------------------------------------------------------------------------------------------------
	// * BASIC SETUP ( all the first 'easy' value setups )
	pui->Flags			= 0;	//(... temporary for sure, next processes will change this first set up )
	pui->pStyle			= uidesc->pStyle;
	pui->User_Nu32		= user32;
	
	// System data & functions ( could be overwritten )
	pui->CurrentState			= NUI_STATE_NULL; 
	pui->UIEventToState_Proc	= NDefaultUIEventToState_Handle;
	pui->UIState_Proc			= NDefaultUIState_Handle;
	pui->ClearUI_Proc			= NClearUI_Basics;

	// + -------------------------------------------------------------------------------------------------------------------------
	// * HANDLING SETUP ( Events )
	
	// Event handling Function
	if(uidesc->Event_Proc)
		pui->Event_Proc = uidesc->Event_Proc;
	else
		pui->Event_Proc = NDefaultUIEvent_Handle;


	// + -------------------------------------------------------------------------------------------------------------------------
	// + UI RENDERABLE 
	//		A UI has one RENDERABLE where all the GFX are stored
	//		and one TEXT ( with is own RENDERABLE) to store text.
	//
	//		A UI can have several appearance (i mean on screen)
	//		It can be an invisible UI, like "Menu UI"  which is a screen size test zone.
	//		It can be a visible UI with several graphics inside: a "back button", an "icon".
	//		And for sure it can have a text inside, a simple caption or bigger text.
	//
	//		The number of geometries included into is in direct relationships with the creation flags.
	//		Even if the number of geometries needed is 0, we need to create a RENDERABLE.
	//		First, because we need to call its "update and extract function", without this call, the recursive call process 
	//		of all the current UIframes tree would be broken (BAD).
	//		Second, because we need its ptransformHNode pointer. Each UIFRame point its TransformHNode to store all its transformation.
	//
	// ...	So, let's describe this RENDERABLE ...
	memset(&rdesc,0,sizeof(NRENDERABLE_DESC));

	NTRANSFORM_HNODE *pthn = NULL;
	if(parent)
	{
		if( ISFLAG_ON( uidesc->Flags_Core, FLAG_NUIDC_USE_PARENT_RENDERABLE_TRANSFORMHNODE ) )
			pthn = parent->pRenderable->pTransformHNode;
		else
			pthn = parent->pUITransformHNode;
	}
	
	// ! An UI create its own THN for it's own Renderable ... Always !
	// This Thn will be Registered by UI Renderable at its creation. 
	rdesc.pTransformHNode = NCreateTransformHNode(pthn);

	//		How many preallocated geometries for this RENDERABLE ? 0,1 or 2 (2 is a maximum for a simple UI, but some other UIS need more)
	rdesc.IncludedGeometryPreallocatedCapacity = uidesc->GeometryPreAllocatedCapacity;
	
	//		The "update (and extract)" function of UI UI. 
	//		It will take care of the UI visual aspect depending it's status.
	//		A UI Can be fully updated (with its BBOX) but not rendered thanks to the flag FLAG_NUIDS_NO_GEOMETRY_EXTRACTION
	//		! WARNING ! it's for a permanent use !
	//		A UI build in with this flag on, is going to have a specific UpdateAndExtract function which doesn't extract geometries
	//		for rendering. So, this kind of UI is INVISIBLE.
	
	NEXTRACTSET	highlevelextractset = 0;
	if( ISFLAG_ON( uidesc->Flags_Style, FLAG_NUIDS_NO_GEOMETRY_EXTRACTION ) )
	{
		rdesc.Update_FCT	= NRenderableUpdateWithoutExtract_UI_Hidden; // Hidden at creation !!! NUIShow will replace it by 'NRenderableUpdateWithoutExtract_UI'
	}
	else
	{
		rdesc.Update_FCT	= NRenderableUpdate_UI_Hidden;				 // Hidden at creation !!! NUIShow will replace it by 'NRenderableUpdate_UI'
		
		// If a UI has geometries to update (and extract), this will be possible in 2 ways:
		//		3D (with or without culling)
		//		HUD (by default always without culling)
		//		On the top of this number of geometry included helps to define single or multi geometries extraction...
		if( ISFLAG_ON( uidesc->Flags_Core, FLAG_NUIDC_USEPARAM_HIGHLEVEL_EXTRACTSET ) )
		{
			highlevelextractset = uidesc->HighLevelExtractSet;
		}
		else
		{
			highlevelextractset = DEFAULT_NUI_RENDERABLE_HIGHLEVEL_EXTRACTSET;
		}

/*
		// 3D UI ?
		if( ISFLAG_ON(highlevelextractset,FLAG_NEXTRACTSET_3D) )
			FLAG_ON( pui->Flags,FLAG_NUI_3D );
*/
	}
	//		... and after the description, the RENDERABLE CREATION itself !
	NEXTRACTSET	lowlevelextractset;
	switch(rdesc.IncludedGeometryPreallocatedCapacity)
	{
		case 0:
			lowlevelextractset = NEXTRACTSET_SECTION_VOID;	// Associated ExtractSet Function will extract nothing !
			// NGetRenderableExtractFunctionByExtractSet( NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UI,0) ); could works too 
			// but NMAKE_NEXTRACTSET_2P(NEXTRACTSET_SECTION_UI,0) -> NRENDERABLE_EXTRACT_FCT_VOID has to be defined... let's see
			break;

		case 1:
			lowlevelextractset = NEXTRACTSET_SECTION_UI;	// Associated ExtractSet Function will extract the unique geometry !
			break;

		default:
			lowlevelextractset = (NEXTRACTSET_SECTION_UI|FLAG_NEXTRACTSET_MULTIGEOMETRIES); 	// Associated ExtractSet Function will extract all the array geometries !
			break;
	}

	rdesc.Extract_FCT				= NGetRenderableExtractFunctionByExtractSet( NMAKE_NEXTRACTSET_2P(lowlevelextractset,highlevelextractset) );
	rdesc.BoundingSphere.fRadius	= DEFAULT_NUI_RENDERABLE_BOUNDINGSPHERE_RADIUS;
	pui->pRenderable				= NCreateRenderable(&rdesc);

// + -------------------------------------------------------------------------------------------------------------------------
// + AdditionnalransformHNode ? To manage all "reactive" transformations like "Push animation" for a push button.
// + Obviously, this TransformHnode Parent is "NUI.pRenderable.pTransformHNode"
	if( ISFLAG_ON(uidesc->Flags_Core,FLAG_NUIDC_UI_TRANSFORMHNODE) )
	{
		pui->pUITransformHNode = NCreateTransformHNode(rdesc.pTransformHNode);
	}
	else
	{
		// If not, pui->pUITransformHNode will be set to "NUI.pRenderable.pTransformHNode"
		// In this way, UI Extract function will always use "pui->pUITransformHNode" as extract TransformHNode.
		// ( ANd it has to, because it has to use the last node of the hierarchic Chain )
		pui->pUITransformHNode = rdesc.pTransformHNode;
	}
	// !!! pui->pUITransformHNode may be 'rdesc.pTransformHNode' itself or a child of it.
	//		if its 'rdesc.pTransformHNode' this one will be referenced TWICE ! One time by the Renderable of UI
	//		and one time here !
	//		if its a child of 'rdesc.pTransformHNode' this one will be referenced only Once HERE by the UI

	NRegisterTransformHNodeReference(pui->pUITransformHNode);

	
	// + -------------------------------------------------------------------------------------------------------------------------
	// + Color Update Setup
	//		UI Colors may be updated each time the state ENABLE/DISABLE/TOUCHFOCUS of the UI changes.
	//		This update is done accordingly with linked UIStyle ColorSet.
	if(ISFLAG_ON(uidesc->Flags_Style,FLAG_NUIDS_COLOR_UPDATE))
		FLAG_ON(pui->Flags,FLAG_NUI_COLOR_UPDATE);
	else
		FLAG_OFF(pui->Flags,FLAG_NUI_COLOR_UPDATE);
	
	// + -------------------------------------------------------------------------------------------------------------------------
	// + CAPTION-TEXT Set up
	//		Notes: a UI can have a caption to setup with out a caption string to copy. that means user want to have a TEXT object setup into
	//		the UI (so setup right now) but no UI caption string ready ( it will be ready later ...).
	//		OR,
	//		User give a caption to the UI, but don't want to print it. He prefers to build a UI without text inside...
	// -----------------------------------------------------------------------
	//		If user wants a text object into its UI.
	if( ISFLAG_ON(uidesc->Flags_Style,FLAG_NUIDS_TEXT_DISPLAY_DEVICE) )
	{
		// Manage Default Text Color (which is given by associated UI Style )
		if( (uidesc->UITextDesc.ColorBuild.Flags & MASK_NBUILDBASE_COLOR_COLORRANGE_AND_GRADIENT) == 0 )// That means 'ColorBuild' don't use its Color params
		{
			// 'uidesc->UITextDesc.ColorBuild' is supoose to be full of ZERO ... check that !
			NMemoryCheck(&uidesc->UITextDesc.ColorBuild,sizeof(NBUILDBASE_COLOR),0);
			// N Is going to overwrite 'uidesc->UITextDesc.ColorBuild' with specific parameters  to use
			// NUI_STYLE range of color as default color build data
			FLAG_ON(uidesc->UITextDesc.ColorBuild.Flags,FLAG_NBUILDBASE_COLOR_USEPARAMS_COLORRANGE);
			
			uidesc->UITextDesc.ColorBuild.pColorRange		= (NCOLOR*)NGetUIColorSetPtr(&uidesc->pStyle->ColorSet, NUI_COLORSET_TEXT, NExtractUIDescColorState(uidesc) );
			uidesc->UITextDesc.ColorBuild.ColorRangeSize	= CONSTANT_NUI_COLORSET_COLORRANGE_SIZE;
		}	
		
		// There are 3 possible HighLevel ExtractSet Flags 3D,HUD,and FRUSTUM CULLING. The FRUSTUM CULLING FLAG has not to be set to ON ...
		// ... for UI Text because already done (or not) by NUI itself ...
		pui->pText = NCreateUIText( pui,(highlevelextractset&(FLAG_NEXTRACTSET_3D|FLAG_NEXTRACTSET_HUD)),&uidesc->UITextDesc );
// 		FLAG_ON(pui->Flags,FLAG_NUI_FRAME_TEXT_DISPLAY);
	}
	else // User doesn't ask for a text display device ... so no one is created !!!
	{
		pui->pText = NULL;	// Useless but just to have a Cristal clear Code !!!
	}
	// + -------------------------------------------------------------------------------------------------------------------------
	// + FRAME BACKGROUND Set up
	//		Notes: a UI can have a Background, or not.
	//		This Background can be a simple quad with or without a texture.
	//		or,
	//		it can be a custom geometry (so only one texture !), given by user.
	//		Exactly the same thing for Icon...
	pbackground_geometry = NULL;
	if( ISFLAG_ON(uidesc->Flags_Style,FLAG_NUIDS_BACKGROUND) )
	{
		if( (uidesc->UIBackgroundDesc.ColorBuild.Flags & MASK_NBUILDBASE_COLOR_COLORRANGE_AND_GRADIENT) == 0 )
		{
			// 'uidesc->UIBackgroundDesc.ColorBuild' is supoose to be full of ZERO ... check that !
			NMemoryCheck(&uidesc->UIBackgroundDesc.ColorBuild,sizeof(NBUILDBASE_COLOR),0);
			// N Is going to overwrite 'uidesc->UIBackgroundDesc.ColorBuild' with specific parameters  to use
			// NUI_STYLE range of color as default color build data
			FLAG_ON(uidesc->UIBackgroundDesc.ColorBuild.Flags,FLAG_NBUILDBASE_COLOR_USEPARAMS_COLORRANGE);
			uidesc->UIBackgroundDesc.ColorBuild.pColorRange		= (NCOLOR*)NGetUIColorSetPtr(&uidesc->pStyle->ColorSet, NUI_COLORSET_BKG, NExtractUIDescColorState(uidesc) );
			uidesc->UIBackgroundDesc.ColorBuild.ColorRangeSize	= CONSTANT_NUI_COLORSET_COLORRANGE_SIZE;
		}														  
		pbackground_geometry = NCreateUIGeometry(pui, &uidesc->UIBackgroundDesc);
		// Flags Setup
		FLAG_ON(pui->FlagsXtd,FLAG_NUI_XTD_BACKGROUND);
	}

	// + -------------------------------------------------------------------------------------------------------------------------
	// + FRAME ICON Set up AND NAME/TEXT Set up PART II
	//		Notes: a UI can have an Icon, or not.
	//		This Icon can be a simple quad with or without a texture.
	//		or,
	//		it can be a custom geometry (so only one texture !), given by user.
	picon_geometry = NULL;
	if( ISFLAG_ON(uidesc->Flags_Style,FLAG_NUIDS_ICON) )
	{
		// Second:	ICON
		if( (uidesc->UIIconDesc.ColorBuild.Flags&MASK_NBUILDBASE_COLOR_COLORRANGE_AND_GRADIENT)==0 )
		{
			// 'uidesc->UIIconDesc.ColorBuild' is supoose to be full of ZERO ... check that !
			NMemoryCheck(&uidesc->UIIconDesc.ColorBuild,sizeof(NBUILDBASE_COLOR),0);
			// N Is going to overwrite 'uidesc->UIIconDesc.ColorBuild' with specific parameters  to use
			// NUI_STYLE range of color as default color build data
			FLAG_ON(uidesc->UIIconDesc.ColorBuild.Flags,FLAG_NBUILDBASE_COLOR_USEPARAMS_COLORRANGE);
			uidesc->UIIconDesc.ColorBuild.pColorRange		= (NCOLOR*)NGetUIColorSetPtr(&uidesc->pStyle->ColorSet, NUI_COLORSET_ICON, NExtractUIDescColorState(uidesc) );
			uidesc->UIIconDesc.ColorBuild.ColorRangeSize	= CONSTANT_NUI_COLORSET_COLORRANGE_SIZE;
		}													  
		picon_geometry = NCreateUIGeometry(pui, &uidesc->UIIconDesc);
		// Flags Setup
		FLAG_ON(pui->FlagsXtd,FLAG_NUI_XTD_ICON);
	}

	// + -------------------------------------------------------------------------------------------------------------------------
	// + UI AUTOMATIC Z SPREADING
	//		Until now, all of the 3 geometries created (potentially) should have the same Z.
	//		Maybe user tell explicitly each UI graphic Z elements...
	//		Anyway by set to ON FLAG_NUIDS_AUTO_Z_SPREAD, the 3 basics UI Elements are automaticly 
	//		Z shifted to be visible without any Z fighting in that precise order
	//		( from back to front)
	//			BACKGROUND
	//			ICON
	//			TEXT
/*
	if( ISFLAG_ON(uidesc->Flags_Style,FLAG_NUIDS_Z_SPREAD) )
	{
		zfront = 0.0f;
		if( pbackground_geometry )
		{
			NGetGeometryAABB(pbackground_geometry,&aabb);
			NShiftGeometryf(pbackground_geometry,0,0,zfront - aabb.fZMin  );
			zfront+=(aabb.fZMax-aabb.fZMin) + CONSTANT_NUI_ZSPREADING;
		}

		if( picon_geometry )
		{
			NGetGeometryAABB(picon_geometry,&aabb);
			NShiftGeometryf(picon_geometry,0,0,zfront - aabb.fZMin );
			zfront+=(aabb.fZMax-aabb.fZMin) + CONSTANT_NUI_ZSPREADING;
		}
		
		// for text its a little bit different.
		if( pui->pText )
		{
			pui->pText->ConstructionOrigin.z = zfront;
		}
	}

*/
	// + -------------------------------------------------------------------------------------------------------------------------
	// + UI PICKING GEOMETRY
	//		To work in the right way, a UI needs a geometry to pick. This is its interactive shape, if user/player touch it,
	//		the UI will have the Focus.
	//		This picking geometry is a 3D geometry.
	//		This picking geometry can be:
	//			a)The UI Background geometry. (in that case, "NUI.pPickingGeometry" member will point on the Background geometry itself).
	//			b)The UI Icon geometry. (in that case, "NUI.pPickingGeometry" member will point on the icon geometry itself).
	//			c)A custom geometry, given by user at the creation. This incoming geometry will be copied into a geometry created by the UI itself.
	//			  And the "NUI.pPickingGeometry" will point on it. This created geometry will be automatically destroyed by the UI 
	//			  at the UI destruction process.
	//			d)Nothing. In that case, impossible to have a Precise picking process, but using BBox still possible.
	//
	//		Its not possible to have several PickingMesh, its only possible to have one.
	
	if(ISFLAG_ON( uidesc->Flags_Core, FLAG_NUIDC_PICKING_MESH_FROM_BACKGROUND ))
	{
		NErrorIf(!pbackground_geometry,NERROR_UI_FRAME_BACKGROUND_GEOMETRY_REQUESTED);
		pui->pPickingGeometryMesh = &pbackground_geometry->Mesh;
		// SetUp Array to calculate and store picking mesh transformed vertex !
		// There are as many transformed positions as Picking geometry mesh vertex.
		NSetupArray(&pui->PickingGeometryTransformedVertexArray,pui->pPickingGeometryMesh->VertexArray.Size,sizeof(NVEC3) );
		NResizeArray(&pui->PickingGeometryTransformedVertexArray,pui->pPickingGeometryMesh->VertexArray.Size,NULL,NULL );
	}
	else if(ISFLAG_ON( uidesc->Flags_Core, FLAG_NUIDC_PICKING_MESH_FROM_ICON ))
	{
		NErrorIf(!picon_geometry,NERROR_UI_FRAME_ICON_GEOMETRY_REQUESTED);
		pui->pPickingGeometryMesh = &picon_geometry->Mesh;
		// SetUp Array to calculate and store picking mesh transformed vertex !
		// There are as many transformed positions as Picking geometry mesh vertex.
		NSetupArray(&pui->PickingGeometryTransformedVertexArray,pui->pPickingGeometryMesh->VertexArray.Size,sizeof(NVEC3) );
		NResizeArray(&pui->PickingGeometryTransformedVertexArray,pui->pPickingGeometryMesh->VertexArray.Size,NULL,NULL );
	}
	else if(ISFLAG_ON( uidesc->Flags_Core, FLAG_NUIDC_PICKING_MESH_FROM_TEXT ))
	{
		NErrorIf( !pui->pText, NERROR_UI_FRAME_TEXT_DISPLAY_DEVICE_MISSING );
		// For text its a little bit special, there is no need of a specific PickingGeometryMesh, because ALL text GEOMETRIES
		// are going to be used. So, let's just set pPickingGeometry to NULL for the moment.
		pui->pPickingGeometryMesh = NULL;
		// SetUp Array to calculate and store picking mesh transformed vertex !
		NSetupArray(&pui->PickingGeometryTransformedVertexArray,DEFAULT_NUI_PICKING_GEOMETRY_TRANSFORMED_VERTEX_ARRAY_CAPACITY,sizeof(NVEC3) );
		
		// Keep in mind ! (for UI Text mesh building actually)
		FLAG_ON(pui->FlagsXtd,FLAG_NUI_XTD_TEXT_PICKING_MESH);
	}
	else if(ISFLAG_ON( uidesc->Flags_Core, FLAG_NUIDC_USEPARAM_CUSTOM_PICKING_MESH ))
	{
		NErrorIf(!uidesc->pCustomPickingMesh,NERROR_UI_FRAME_CUSTOM_PICKING_GEOMETRYMESH_REQUESTED);
		NErrorIf( NGetArrayElementSize(&uidesc->pCustomPickingMesh->PrimitiveArray)!=sizeof(NTRIANGLE),NERROR_UI_FRAME_CUSTOM_PICKING_GEOMETRYMESH_TRIANGLE_ONLY );
		NErrorIf( sizeof(NVERTEX_V3)!=sizeof(NVEC3),NERROR_SYSTEM_NENGINEVERSION_CONFLICT ); // made 2015/09/01. At this time sizeof(NVERTEX_V3)==sizeof(NVEC3) !

		// Here we are going to setup geometry Mesh as small as possible ... we are just interesting by Vertex Position and Primitive.
		// Of course, we prefer avoiding allocate ( waste of memory ) memory space for vertex color, uvs and texture ...
		// So it's why we are going to use NVERTEX_V3 as vertex format !
/*
		NMESH_CAPACITIES gmdesc;
		memset(&gmdesc,0,sizeof(NMESH_CAPACITIES));
		gmdesc.PrimitiveArray_Capacity	= uidesc->pCustomPickingMesh->PrimitiveArray.Capacity;
		gmdesc.VertexArray_Capacity		= uidesc->pCustomPickingMesh->VertexArray.Capacity;
		gmdesc.PrimitiveArray_Size		= 0;
		gmdesc.VertexArray_Size			= 0;
		pui->pPickingGeometryMesh	= NCreateMesh( sizeof(NTRIANGLE), sizeof(NVERTEX_V3) , &gmdesc);
		// Impossible to use "NCopyGeometryMesh()" here because we don't have any details
		// So, by hand copy ! to copy only what we need, Vertex POSITION and primitive.
		// For NPRIMITIVE we already check that "pCustomPickingMesh->PrimitiveArray" is about NTRIANGLE
		NCopyArray(&pui->pPickingGeometryMesh->PrimitiveArray,&uidesc->pCustomPickingMesh->PrimitiveArray);
		// For Vertex, we are just going to copy Vertex position in "blind mode" without knowing incoming vertex format.
		// Because NVERTEX_V3  is just a NVEC3 we are going to fill up the Vertex array ...
		NVEC3 *psrcv = (NVEC3*)uidesc->pCustomPickingMesh->VertexArray.pFirst;
		for(Nu32 i=uidesc->pCustomPickingMesh->VertexArray.Size;i!=0;i--,psrcv =(NVEC3*)((NBYTE*)psrcv+uidesc->pCustomPickingMesh->VertexArray.ElementSize) )
		{
			NPushBackMeshVertex(pui->pPickingGeometryMesh,(NBYTE*)psrcv,sizeof(NVERTEX_V3) );		
		}
*/		
		pui->pPickingGeometryMesh	= _create_custom_pickingmesh(uidesc->pCustomPickingMesh);
		// Keep in mind ! (for UI clearing actually)
		FLAG_ON(pui->FlagsXtd,FLAG_NUI_XTD_CUSTOM_PICKING_MESH);
		
		// SetUp Array to calculate and store picking mesh transformed vertex !
		NSetupArray(&pui->PickingGeometryTransformedVertexArray,pui->pPickingGeometryMesh->VertexArray.Size,sizeof(NVEC3) );
		NResizeArray(&pui->PickingGeometryTransformedVertexArray,pui->pPickingGeometryMesh->VertexArray.Size,NULL,NULL );
	}
	else
	{
		pui->pPickingGeometryMesh = NULL;
		// SetUp Array to calculate and store picking mesh transformed vertex !
		NSetupArray(&pui->PickingGeometryTransformedVertexArray,0,sizeof(NVEC3) );
	}
	
	// + -------------------------------------------------------------------------------------------------------------------------
	// + UI BOUNDING BOX
	//		Default UI's BoundingBox parameters
	//		By default bounding box is set as the screen size
	//		But it's possible to define specific parameters by using FLAG_NUI_USEPARAM_BOUNDINGBOX
	if(ISFLAG_ON( uidesc->Flags_Core, FLAG_NUIDC_USEPARAM_BOUNDINGBOX ))
	{
		memcpy(&pui->BoundingBox,&uidesc->BoundingBox,sizeof(NRECTf32));
	}
	else// by default BoundingBox = Full screen ! And it's very important for Root and layer.
	{
		pui->BoundingBox.upper_left.x	= 0.0f;
		pui->BoundingBox.upper_left.y	= 0.0f;
		pui->BoundingBox.lower_right.x	= NGetViewPort_AspectRatio(); //(Nf32)viewport[2]/(Nf32)viewport[3]; // TODO: precalc
		pui->BoundingBox.lower_right.y	= -1.0f;
	}
	
	// HOW updating the BoundingBox ?
	//		Two ways: 
	//			a)From UI picking geometry.
	//			b)From UI child BBoxes.
	//
	//		From UI picking Geometry:
	//			For BoundingBox Updating, engine needs to have a picking geometry. 
	//			It uses this picking geometry with UI associated THN to calculate the projected 2D picking shape AND, from that, the BBox.
	//			Picking geometry can be one of the 2 GFX geometries of the UI (background or icon) or a specific one give by user at creation.
	//			When a UI is INVISIBLE it doesn't means it's graphic part is NOTHING. It may still something, a simple rectangle or a complex 3D object.
	//			When a UI is INVISIBLE it  continues to be be detected thanks to its picking shape (which is can be one of its hidden geometry or a custom one !).

	if( ISFLAG_ON( uidesc->Flags_Core, FLAG_NUIDC_BBOX_UPDATE_FROM_CHILDREN_BBOX ) )
	{
// 		NErrorIf( ISFLAG_ON( uidesc->Flags_Core, FLAG_NUIDC_PICKING_MESH_FROM_TEXT ),	NERROR_INCONSISTENT_FLAGS);		
// 		NErrorIf( pui->pPickingGeometryMesh,											NERROR_INCONSISTENT_PARAMETERS);

		FLAG_ON( pui->Flags, FLAG_NUI_BBOX_UPDATE_FROM_CHILDREN_BBOX );
		pui->BoundingBoxUpdating_Proc = NUpdateUIBBox_From_ChildrenBBox;

		// IMPORTANT:
		//	If FLAG_NUIDC_PICKING_MESH_FROM_TEXT is also set to ON that means, BBOX is going to update by ChildBoxes ( and will fit them) ...
		//	... but precise picking (if it's ACTIVE) is going to be done with UI text. So be aware that's a strange (but effective ) combination.
		//	Same thing with 'pPickingGeometryMesh' if precise picking is set to ON, is going to be done with  'pPickingGeometryMesh' but after
		//	a first test against UIBBOX which was updated by UI children !
	}
	else
	{
		FLAG_OFF( pui->Flags, FLAG_NUI_BBOX_UPDATE_FROM_CHILDREN_BBOX );
		if(pui->pPickingGeometryMesh)
		{
			pui->BoundingBoxUpdating_Proc = NUpdateUIBBox_From_PickingMesh;
		}
		else if( ISFLAG_ON( uidesc->Flags_Core, FLAG_NUIDC_PICKING_MESH_FROM_TEXT ) )
		{
			pui->BoundingBoxUpdating_Proc = NUpdateUIBBox_From_TextMesh;
		}
		else
		{
			pui->BoundingBoxUpdating_Proc = NUpdateUIBBox_Void;
		}
	}

	// Precise or not ? (Only possible with a valid PickingGeometry)
	if(pui->pPickingGeometryMesh)
	{
		if( ISFLAG_ON( uidesc->Flags_Core, FLAG_NUIDC_PRECISE_PICKING ) )
			pui->Pick_Proc = NPickUIPickingMesh;
		else
			pui->Pick_Proc = NPickUIBBox;
	}
	else if( ISFLAG_ON( uidesc->Flags_Core, FLAG_NUIDC_PICKING_MESH_FROM_TEXT ) )
	{
		if( ISFLAG_ON( uidesc->Flags_Core, FLAG_NUIDC_PRECISE_PICKING ) )
			pui->Pick_Proc = NPickUIPickingTextMesh;
		else
			pui->Pick_Proc = NPickUIBBox;
	}
	else
	{
		pui->Pick_Proc = NPickUIBBox; // By default (without any other specifications N is going to pick the UIBBOX)
	}
	
	// Picking refinement
	// NEngine may verify that an UI is fully ON SCreen to try picking it.
	// If the flag is set to ON and UI not fully on screen, all picking methods are going to return 0 ( NON picked !)
	// By default is set to OFF. But it could be very useful to use it, especially
	// for Slide children UI ... Those ones may be partially out of screen and in that case NON INTERACTIVE ( with the flag set to ON )
	if( ISFLAG_ON( uidesc->Flags_Style, FLAG_NUIDS_CAPTURE_TOUCH_ONLY_IF_FULLY_ON_SCREEN ) )
		FLAG_ON( pui->Flags, FLAG_NUI_CAPTURE_TOUCH_ONLY_IF_FULLY_ON_SCREEN );
	else
		FLAG_OFF( pui->Flags, FLAG_NUI_CAPTURE_TOUCH_ONLY_IF_FULLY_ON_SCREEN );

	// Store Picking Mode Flag
	// Notice that's possible to store FLAG_NUI_PRECISE_PICKING set to ON and having 'NPickUIBBox' as pick_Proc due to a lake of Picking Geometry Mesh.
	// (pPickingGeometryMesh = NULL) !! That's Normal, FLAG_NUI_PRECISE_PICKING is just here to keep in mind what user asked for.

	// WHEN updating UI BoundingBox ?
	//		Because there are two ways to update BBbox, thinks are a little bit complex (but still easy to understand). 
	//		
	//		First case: The flag  FLAG_NUIDC_BBOX_UPDATE_FROM_CHILDREN_BBOX is set to ON.
	//		Here, UI BBox will be updated ONLY when one (or several) of its children BBox will change. That's it.
	//		So if FLAG_NUIDC_BBOX_UPDATE_FROM_CHILDREN_BBOX is set to On, the other BBox update control flag is ignored !
	//												FLAG_NUIDC_BBOX_UPDATE_IF_THN_CHANGED is ignored !	
	//
	//		Second case:  The flag  FLAG_NUIDC_BBOX_UPDATE_FROM_CHILDREN_BBOX is set to OFF.
	//		Here, the other BBox update control flag, which is FLAG_NUIDC_BBOX_UPDATE_IF_THN_CHANGED can be set to ON.
	//		In that case, BBox will be updated each time the linked TransformHNode will be updated.
	//
	//		Remind: Engine will set to off these two flags if there is no picking geometry !
	if( ISFLAG_OFF( uidesc->Flags_Core, FLAG_NUIDC_BBOX_UPDATE_FROM_CHILDREN_BBOX ) )
	{
		if(pui->pPickingGeometryMesh || ISFLAG_ON( uidesc->Flags_Core, FLAG_NUIDC_PICKING_MESH_FROM_TEXT ) )
		{
			if( ISFLAG_ON( uidesc->Flags_Core, FLAG_NUIDC_BBOX_UPDATE_IF_THN_CHANGED ) )
			{
				FLAG_ON( pui->Flags, FLAG_NUI_BBOX_UPDATE_IF_THN_CHANGED );
			}
			
			if( ISFLAG_ON(uidesc->Flags_Core,FLAG_NUIDC_BBOX_UPDATE_AT_PUSH_KEY_MOMENTS) )
				FLAG_ON(pui->Flags,FLAG_NUI_BBOX_UPDATE_AT_PUSH_KEY_INSTANTS);

			if( ISFLAG_ON(uidesc->Flags_Core,FLAG_NUIDC_BBOX_UPDATE_DURING_PUSH) )
				FLAG_ON(pui->Flags,FLAG_NUI_BBOX_UPDATE_DURING_PUSH);
		}
	}

	// USER Ask for a first BBox update at setup (to be precise and crystal clear... at the first NUI::Nupdate&Extract call )?
	// It must be performed even if "pui->BoundingBoxUpdating_Proc == NUpdateUIBBox_Void"
	// Because this BBOX update will 'automatically' generate a BBOX update of the UI Parent 
	// ( if this parent has the flag FLAG_NUIDC_BBOX_UPDATE_FROM_CHILDREN_BBOX set to ON ).
	// Excepted !!! ...
	// ... In the special case where User doesn't asked for any kind of BBOX Update and where, at the same time, UI parent
	// has the flag 'FLAG_NUIDC_BBOX_UPDATE_FROM_CHILDREN_BBOX' set to ON ... 
	// So, to deal with this case (certainly rare), we will enforce parent BBOX update directly from here !
	if( ISFLAG_ON( uidesc->Flags_Core, FLAG_NUIDC_BBOX_UPDATE_ONCE_AT_SETUP ) )
	{
		FLAG_ON( pui->Flags, FLAG_NUI_BBOX_UPDATE_ONCE );
	}
	else if( parent && ISFLAG_ON(parent->Flags,FLAG_NUIDC_BBOX_UPDATE_FROM_CHILDREN_BBOX) )
	{
		FLAG_ON( parent->Flags, FLAG_NUI_BBOX_UPDATE_ONCE );
	}
	// + -------------------------------------------------------------------------------------------------------------------------
	// + FRAME NAME 
	//		because we want pui Picking and BBox updating right setup ...
	//		Actually, function 'NBuildUITextMeshFromString' called, need to know if
	//		user wants to use text as picking mesh, or not !
	// 
	//	Store Name
	if( ISFLAG_ON(uidesc->Flags_Style,FLAG_NUIDS_COPY_NAME_STRING) )
	{
		NErrorIf(!uidesc->pName,NERROR_STRING_NULL);
		NErrorIf(!strlen(uidesc->pName),NERROR_STRING_NULL);
		NErrorIf(strlen(uidesc->pName)+1>CONSTANT_NUI_NAME_SIZEMAX, NERROR_UI_FRAME_NAME_TOO_LONG);
		
		pui->pName = (Nchar*)Nmalloc(NMIN(strlen(uidesc->pName)+1,CONSTANT_NUI_NAME_SIZEMAX)*sizeof(Nchar));
		NStrCopy(pui->pName,uidesc->pName,CONSTANT_NUI_NAME_SIZEMAX);
	}
	else // Useless, but help staying Cristal Clear !
	{
		pui->pName = NULL; // !!! No allocated memory to store UiFrame name !!!
	}

	// Display Name
	// It's possible to ask for name displaying without asking to store this name !!!
	// Reminders:
	// ----------
	//	FLAG_NUIDS_USEPARAM_NAME		Allocate the right memory amount and Store the "UIdesc->pName" into "UI->pName" member.
	//	FLAG_NUIDS_DISPLAY_NAME		Display the "UIdesc->pName" even if its not stored. This Flag is not going to be stored.
	//								So, it's effective only here, at UI Setup (Creation)!
	if( ISFLAG_ON(uidesc->Flags_Style,FLAG_NUIDS_DISPLAY_NAME) )
	{
		NErrorIf(!pui->pText, NERROR_NULL_POINTER);
		NErrorIf(!uidesc->pName, NERROR_STRING_NULL);
		NBuildUITextMeshFromString(pui, uidesc->pName);
	}
	// LOCK NAME ?
	// It's possible to lock the name, and make impossible to change it (trough function "NSetUIName")
	// It's just a flag tested each time an UI function wants to change the UI name...
	if(ISFLAG_ON(uidesc->Flags_Style,FLAG_NUIDS_NAME_LOCKED))
		FLAG_ON(pui->FlagsXtd,FLAG_NUI_XTD_NAME_LOCKED);
	else
		FLAG_OFF(pui->FlagsXtd,FLAG_NUI_XTD_NAME_LOCKED);

	// DISABLE at START ?
	//		This kind of UI doesn't want to get the "focus"
	//		Its something like a "non-interactive UI".
	//		WARNING ! If it can't have the focus, its children don't too !
	//		BUT ... That doesn't mean NO BOUNDING BOX. (maybe UI parent needs it !)
	//		It's just that the UI will 'break' the FOCUS LOOKING FOR process on it's branch ( == no focus for me and my children !).
	//
	//		Event_Handle will receive NUI_ENABLE Event after NUI_SETUP event at the end of setup 
	//		So Set Up of  'FLAG_NUI_ENABLE' is done inside 'NPopUISetUpProcess' with ENABLE event sending ( if UI is enable )
	//
	//		So, the code bellow is not here but in 'NPopUISetUpProcess' ( have a lock some lines below )
	// 		if( ISFLAG_ON( uidesc->Flags_Core, FLAG_NUIDC_DISABLE ) )
	// 			FLAG_OFF( pui->Flags, FLAG_NUI_ENABLE );
	// 		else
	// 			FLAG_ON( pui->Flags, FLAG_NUI_ENABLE ); 

	// + -------------------------------------------------------------------------------------------------------------------------
	// + UI FRAME EVENT LISTEN/INTERCEPTION OPTION
	// +
	// + UI is going to receive several different EVENTs.
	// + There are two ways receive event 'LISTEN' and 'INTERCEPT'.
	// +	1)'LISTEN':		means an UI accepts to receive events addressed to it. ( == UI is the Event Recipient)
	// +	2)'INTERCEPT':	means UI wants to intercept events addressed to its children. 
	// +	
	// +		
	// +
	// + UICORE EVENT ('Listen or not' AND 'Intercept or not' )
	// + UI Frame may or may not listen UICORE Event, this is an option, activated by default. (So by default UI Listen !)
	// + Interception ( Receive UICORE EVENT of children before them ) is optional and, by default, is not activate.
	if( ISFLAG_ON( uidesc->Flags_Style, FLAG_NUIDS_LISTEN_UICORE_EVENT ) )
	{
		FLAG_ON( pui->Flags, FLAG_NUI_LISTEN_UICORE_EVENT ); // default behavior

		if( ISFLAG_ON( uidesc->Flags_Style, FLAG_NUIDS_INTERCEPT_UICORE_EVENT ) )
			FLAG_ON( pui->Flags, FLAG_NUI_INTERCEPT_UICORE_EVENT );
	}
	else
	{
		// To intercept uicore event, 'ui' must listen uicore event !
		NErrorIf(ISFLAG_ON( uidesc->Flags_Style, FLAG_NUIDS_INTERCEPT_UICORE_EVENT ), NERROR_INCONSISTENT_FLAGS);
		FLAG_OFF( pui->Flags, FLAG_NUI_LISTEN_UICORE_EVENT );
		FLAG_OFF( pui->Flags, FLAG_NUI_INTERCEPT_UICORE_EVENT );
	}

	// + UICOMMAND EVENT ('Listen or not' AND 'Intercept or not' )
	// + UI Frame may or may not listen UICOMMAND Event, this is an option, activated by default. (So by default UI Listen !)  
	// + Interception ( Receive UICOMMAND EVENT of children before them ) is optional and, by default, is not activate.
	if( ISFLAG_ON( uidesc->Flags_Style, FLAG_NUIDS_LISTEN_UICOMMAND_EVENT ) )
	{
		FLAG_ON( pui->Flags, FLAG_NUI_LISTEN_UICOMMAND_EVENT ); // default behavior

		if( ISFLAG_ON( uidesc->Flags_Style, FLAG_NUIDS_INTERCEPT_UICOMMAND_EVENT ) )
			FLAG_ON( pui->Flags, FLAG_NUI_INTERCEPT_UICOMMAND_EVENT );
	}
	else
	{
		// To intercept command event, 'ui' must listen command event !
		NErrorIf(ISFLAG_ON( uidesc->Flags_Style, FLAG_NUIDS_INTERCEPT_UICOMMAND_EVENT ), NERROR_INCONSISTENT_FLAGS);
		FLAG_OFF( pui->Flags, FLAG_NUI_LISTEN_UICOMMAND_EVENT );
		FLAG_OFF( pui->Flags, FLAG_NUI_INTERCEPT_UICOMMAND_EVENT );
	}

	// + UINOTIFY EVENT ('Listen or not' AND 'Intercept or not' )
	// + UI Frame may or may not listen UINOTIFY Event, this is an option, activated by default. (So by default UI Listen !)  
	// + Interception ( Receive UINOTIFY EVENT of children before them ) is optional and, by default, is not activate.
	if( ISFLAG_ON( uidesc->Flags_Style, FLAG_NUIDS_LISTEN_UINOTIFY_EVENT ) )
	{
		FLAG_ON( pui->Flags, FLAG_NUI_LISTEN_UINOTIFY_EVENT ); // default behavior

		if( ISFLAG_ON( uidesc->Flags_Style, FLAG_NUIDS_INTERCEPT_UINOTIFY_EVENT ) )
			FLAG_ON( pui->Flags, FLAG_NUI_INTERCEPT_UINOTIFY_EVENT );
	}
	else
	{
		// To intercept Notify event, 'ui' must listen Notify event !
		NErrorIf(ISFLAG_ON( uidesc->Flags_Style, FLAG_NUIDS_INTERCEPT_UINOTIFY_EVENT ), NERROR_INCONSISTENT_FLAGS);
		FLAG_OFF( pui->Flags, FLAG_NUI_LISTEN_UINOTIFY_EVENT );
		FLAG_OFF( pui->Flags, FLAG_NUI_INTERCEPT_UINOTIFY_EVENT );
	}

	// + TOUCH EVENT ('Listen or not' AND 'Intercept or not' )
	// + UI Frame may or may not listen TOUCH Event, this is an option, activated by default. (So by default UI Listen !)  
	// + Interception ( Receive TOUCH EVENT even if UI is not the terminated leaf of touch event hierarchical dispatch ) is optional and, by default, is not activate.
/*
	if( ISFLAG_ON( uidesc->Flags_Style, FLAG_NUIDS_DO_NOT_LISTEN_TOUCH_EVENT ) )
	{
		// To intercept touch event, 'ui' must listen touch event !
		NErrorIf(ISFLAG_ON( uidesc->Flags_Style, FLAG_NUIDS_INTERCEPT_TOUCH_EVENT ), NERROR_INCONSISTENT_FLAGS);

		FLAG_OFF( pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT );
		FLAG_OFF( pui->Flags, FLAG_NUI_INTERCEPT_TOUCH_EVENT );

		// About Parent Flag "FLAG_NUI_SOME_CHILDREN_CAPTURE_TOUCH_ON_THE_FLY"
		if(pui->pParent)
		{
			FLAG_ON(((NUI*)pui->pParent)->Flags, FLAG_NUI_ADVANCED_CHILDREN_TOUCH_MOVE_CAPTURE ); // Activate this flag because its a process optimizer flag and in this special case [FLAG_OFF( pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT )] it has to be set !
		}

	}
	else
	{
		FLAG_ON( pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT ); // default behavior

		if( ISFLAG_ON( uidesc->Flags_Style, FLAG_NUIDS_INTERCEPT_TOUCH_EVENT ) )
			FLAG_ON( pui->Flags, FLAG_NUI_INTERCEPT_TOUCH_EVENT );
	}
*/

	// + -------------------------------------------------------------------------------------------------------------------------
	// +
	// + TOUCH management
	// +
	// + !!! Has to be done after "FLAG_NUI_LISTEN_TOUCH_EVENT" setup !!!
	// + Because,  to optimize Touch Dispatching 'NSetUIListenTouchMode' mays call:
	// +							NUpdateUI_FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN_State((NUI*)pui->pParent);
	// +							NUpdateUI_FLAG_NENGINE_CORE_ADVANCED_ACTIVE_UI_TOUCH_MOVE_CAPTURE_State();
	// +							...	
	// +							... And this 2 function look for "FLAG_NUI_LISTEN_TOUCH_EVENT" in their process !!!
	// +
	//
	NErrorIf(!FLAGS_TEST(pui->Flags,MASK_NUI_LISTEN_TOUCH_EVENT,0), NERROR_SYSTEM_CHECK); // just in case ...
	if( ISFLAG_ON(uidesc->Flags_Style,FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE) )
	{
		NSetUIListenTouchMode(pui,uidesc->TouchMode);
	}
	else
	{
		// By Default UI set it's Touch Mode to NUI_LISTEN_TOUCH_MODE_NULL
		// But, this setup may be overwritten by specific UI after the setupUI call... 
		// ... for example: UISlider will modify the default touch mode to BASIC ( only if user doesn't ask explicitly to use a Touch Mode )
		NSetUIListenTouchMode(pui,NUI_LISTEN_TOUCH_MODE_NULL);
	}
	
	if( ISFLAG_ON( uidesc->Flags_Style, FLAG_NUIDS_INTERCEPT_TOUCH_EVENT ) )
	{
		FLAG_ON( pui->Flags, FLAG_NUI_INTERCEPT_TOUCH_EVENT );
	}


	// + -------------------------------------------------------------------------------------------------------------------------
	// + UI FRAME EVENT BLOCKING (for GameStates)
	// +
	// + Only the UI with the TOUCH FOCUS will receive event notification.
	// + And Only the UI Frame with Touch Focus can decide if the current Event is going to pass "trough" the UI to the GAME STATES.
	// + That's the purpose of the flag FLAG_NUI_BLOCK_TOUCH_EVENT_FOR_GAMESTATES. By default N Set it to ON, but user can ask  
	// + to set it to OFF at creation by using the creation flag FLAG_NUIDC_DO_NOT_BLOCK_TOUCH_EVENT_FOR_GAMESTATES
	// + 
	// + After Creation it stills possible to change by using the Functions 'NEnableGameStatesTouchEventBlocking' / 'NDisableGameStatesTouchEventBlocking'
	// +
	if( ISFLAG_ON( uidesc->Flags_Style, FLAG_NUIDS_DO_NOT_BLOCK_TOUCH_EVENT_FOR_GAMESTATES ) )
		FLAG_OFF( pui->Flags, FLAG_NUI_BLOCK_TOUCH_EVENT_FOR_GAMESTATES );
	else
		FLAG_ON( pui->Flags, FLAG_NUI_BLOCK_TOUCH_EVENT_FOR_GAMESTATES );//which is the default behavior of all UI !

	
	pui->TouchCountMax = 1;

	// + -------------------------------------------------------------------------------------------------------------------------
	// + ACTIVATION 
	// +
	//
	// DISABLE at START ?
	//		FLAG_NUIDC_DISABLE is going to be manage inside 'NPopUISetUpProcess'
	//
	// Deactivate HIERARCHICAL_ENABLE ?
	if( ISFLAG_ON( uidesc->Flags_Core, FLAG_NUIDC_HIERARCHICAL_ENABLE_CONTROL_OFF )  )
	{
		FLAG_OFF(pui->FlagsXtd, FLAG_NUI_XTD_HIERARCHICAL_ENABLE_CONTROL); // Useless but to still Cristal clear !!!
	}
	else
	{
		FLAG_ON(pui->FlagsXtd, FLAG_NUI_XTD_HIERARCHICAL_ENABLE_CONTROL); // Which is the default state
	}
	//
	// Deactivate HIERARCHICAL_SHOW ?
	if( ISFLAG_ON( uidesc->Flags_Core, FLAG_NUIDC_HIERARCHICAL_SHOW_CONTROL_OFF )  )
	{
		FLAG_OFF(pui->FlagsXtd, FLAG_NUI_XTD_HIERARCHICAL_SHOW_CONTROL); // Useless but to still Cristal clear !!!
	}
	else
	{
		FLAG_ON(pui->FlagsXtd, FLAG_NUI_XTD_HIERARCHICAL_SHOW_CONTROL); // Which is the default state
	}

	// + -------------------------------------------------------------------------------------------------------------------------
	// + END
	// +
	return NPopUISetUpProcess(pui,uidesc);
}

NUI* NPopUISetUpProcess(NUI *pui, NUI_DESC *pframedesc)
{
	NEVENT nevent;


	pframedesc->Private_SetUpStack--;
	if(!pframedesc->Private_SetUpStack)
	{
		// --------------------------------------------------------------------------------------------------------------------------------
		// End Setup Checks
		// This is the right place to do them, because some setups are 'composite', done in the regular UI Setup and re-done for some part  
		// in the specific UI setup with potential changes... 
		// This is the case, for example, for DEFAULT LISTEN TOUCH MODE :
		//		 Set to NULL by regular UI setup but reset to BASIC by UISlider ...
		NErrorIf(!NIsUIListenTouchEvent(pui) && NIsUIInterceptTouchEvent(pui), NERROR_INCONSISTENT_FLAGS);
		// --------------------------------------------------------------------------------------------------------------------------------

		Nmem0(&nevent,NEVENT);
		NFillupEventHead(nevent,NUI_SETUP,0,NEVENT_RECIPIENT_CAST_UI,pui);

		NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 
		//NSendUIEvent(&nevent,NULL);
		NSendUICoreEvent(&nevent);
		if(NUIClearWatchStackPop()!=pui)
			return NULL;
		
		if( ISFLAG_ON( pframedesc->Flags_Core, FLAG_NUIDC_DISABLE ) || ( pui->pParent && NIsUIDisable((NUI*)pui->pParent ) ) )
		{
			// if we are here, that means ... FLAG_NUIDC_DISABLE is ON, or UI has a Disabled parent !
			// --->
			// In those cases, UI is automatically set as Disable.
			FLAG_OFF( pui->Flags, FLAG_NUI_ENABLE ); // useless but just to be Cristal clear
		}
		else
		{
			// if we are here, that means ... FLAG_NUIDC_DISABLE is OFF, and UI doesn't have any parent or has an Enable parent !
			// ---> 	
			// In those cases, UI is automatically set as Enable.
			// Event_Handle will receive NUI_ENABLE Event after NUI_SETUP event at the end of setup
			
			FLAG_ON( pui->Flags, FLAG_NUI_ENABLE ); 
			Nmem0(&nevent,NEVENT);
			NFillupEventHead(nevent,NUI_ENABLE,0,NEVENT_RECIPIENT_CAST_UI,pui);
			NUIClearWatchStackPush(pui);// "keep an eye" on "pui" 
			//NSendUIEvent(&nevent,NULL);
			NSendUICoreEvent(&nevent);
			if(NUIClearWatchStackPop()!=pui)
				return NULL;
		}
	}
	return pui;
}


void NClearUI_Basics(NUI *pui)
{

	if(!pui)
	{
		NErrorIf(1,NERROR_NULL_POINTER);
		return;
	}

	// Children ?
	NHIERARCHY_NODE		*pa,*pb;
	pa=(NHIERARCHY_NODE*)pui->ChildrenList.pFirst;
	while(NNodeIsValid((NNODE*)pa,&pui->ChildrenList))
	{
		pb = (NHIERARCHY_NODE*)pa->pNext;
		NDeleteUI((NUI*)pa);
		pa=pb;
	} 

	// Delete Text before delete / purge UI Thn. Because this Text registered 'pui->pTransformHNode' at creation
	if(pui->pText)
	{
		NDeleteText(pui->pText);
		pui->pText = NULL;
	}

	// From Here pui doesn't have any children ...
	// There is always a renderable ... but pUITransformHNode is not necessary equal to pRenderable->pTransformHNode...
	// It may be specific ( created by UI itself at setup for PUSH ANIMATION for instance) 
	NTRANSFORM_HNODE *pthn = pui->pRenderable->pTransformHNode;
	NUnregisterTransformHNodeReference(pui->pUITransformHNode);
	NDeleteRenderable(pui->pRenderable); // Renderable will unregister the pUITransformHNode it registered by itself at creation.
	NPurgeTransformHNode(pthn);			 // May delete "pRenderable->pTransformHNode" AND "pui->pUITransformHNode" which is child of "pRenderable->pTransformHNode"
	pui->pUITransformHNode	= NULL;
	pui->pRenderable		= NULL;


	if(pui->pName)
	{
		Nfree(pui->pName);
		pui->pName = NULL;
	}

	if( ISFLAG_ON(pui->FlagsXtd,FLAG_NUI_XTD_CUSTOM_PICKING_MESH) )
	{
		NDeleteMesh(pui->pPickingGeometryMesh);
	}
	pui->pPickingGeometryMesh = NULL;

	NClearArray(&pui->PickingGeometryTransformedVertexArray,NULL);

	// Node Part
	NErrorIf(pui->pParent	&& !pui->pNext,		NERROR_NODE_POINTER_INCONSISTENCY );
	NErrorIf(pui->pParent	&& !pui->pPrevious,	NERROR_NODE_POINTER_INCONSISTENCY );
	NErrorIf(pui->pNext		&& !pui->pPrevious,	NERROR_NODE_POINTER_INCONSISTENCY );
	NErrorIf(!pui->pNext	&& pui->pPrevious,	NERROR_NODE_POINTER_INCONSISTENCY );

	// Optimization Flags removing
	// Here we are sure pui is not visible ! ( hidden at NClearUI(pui) call )
	// But, in DEBUG, just double-check ...
	NErrorIf(NIsUIVisible(pui),NERROR_SYSTEM_CHECK);
	// Because of all children delete ... it's not possible for pui to have FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN set to ON !
	NErrorIf(ISFLAG_ON(pui->Flags,FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN), NERROR_SYSTEM_CHECK);
	
	//  Turn Off FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN if necessary ...
	if( ISFLAG_ON(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN) )
	{	
		NDisableUIListenTouch_CaptureMoveIn(pui);
	}
	
	// Hierarchy Management
	pui->pParent	= NULL; 
	if( pui->pNext ) //  || pui->pPrevious
		NNodeRemove((NNODE*)pui);

	pui->Flags						= 0;
	pui->pStyle						= NULL;
	pui->CurrentState				= NUI_STATE_NULL;
	pui->fPushUpDownRatio			= 0.0f;
	pui->BoundingBoxUpdating_Proc	= NULL;
	pui->Pick_Proc					= NULL;
	pui->Event_Proc					= NULL;
	pui->UIEventToState_Proc		= NULL;
	pui->UIState_Proc				= NULL;
	pui->ClearUI_Proc				= NULL;
	pui->User_Nu32					= 0;
	Nmem0(&pui->BoundingBox,NRECTf32);

	// !!! 
	// pui->Flags_Debug Still the same !!! To keep its FLAGS as they ARE !!!
}

void NClearUI(NUI *pui)
{
	if(!pui)
	{
		NErrorIf(1,NERROR_NULL_POINTER);
		return;
	}

	// 0)	Set the FLAG_NUI_DEBUG_CLEAR_PROCESS_ENGAGED to ON to protect 'pui'
	//		against "In Event Processing" Clear or Delete nested calls.
	#ifdef _DEBUG
	NErrorIf(ISFLAG_ON(pui->Flags_Debug,FLAG_NUI_DEBUG_CLEAR_PROCESS_ENGAGED), NERROR_UI_NESTED_CLEAR );
	FLAG_ON(pui->Flags_Debug,FLAG_NUI_DEBUG_CLEAR_PROCESS_ENGAGED);
	#endif

	NEVENT nevent;

	// 1a)	Find all the 'pui' value in the stack and set them to NULL !
	NUIClearWatchStack_ClearNotification(pui);
	// 1b)	All the UI Transform HNode Animation Players need to be destroyed (if they are some ...)
	NDeleteAllUIThnAnimationPlayer(pui);

	// 2) Be sure UI is not 'Active'
	// No need to call a 'NUITouchCancelAll' because 'NHideUI' will do it if necessary !
	NHideUI(pui); // All the visible 'pui' children are going to be hidden by this call
	NErrorIf(pui->pTouchUIListener, NERROR_NON_NULL_POINTER); // It has to be NULL !!!
	NDelayedUIEventArray_UIClear_Notification(pui);


	// Send the UI Struct Clear Event to the Cleared UI
	// Note: If parent are Intercepting UICORE event they are going to receive it before Clearder UI !
	//		 
	Nmem0(&nevent,NEVENT);
	NFillupEventHead(nevent,NUI_CLEAR,0,NEVENT_RECIPIENT_CAST_UI,pui);
	//NSendUIEvent(&nevent,NULL);
	NSendUICoreEvent(&nevent);

	pui->ClearUI_Proc(pui);
}

void NDeleteUI( NUI *pui )
{
	if(!pui)
	{
		NErrorIf(1,NERROR_NULL_POINTER);
		return;
	}
	
	#ifdef _DEBUG
	NErrorIf(ISFLAG_ON(pui->Flags_Debug,FLAG_NUI_DEBUG_CLEAR_PROCESS_ENGAGED), NERROR_UI_NESTED_DELETE );
	#endif
	
	NClearUI(pui);
	Nfree(pui);
}


NGEOMETRY	*NGetUIBackgroundGeometry(const NUI *pui)
{
	if( ISFLAG_ON(pui->FlagsXtd,FLAG_NUI_XTD_BACKGROUND) )
		return (NGEOMETRY*)pui->pRenderable->GeomArray.pFirst;
	else
		return NULL;
}

NGEOMETRY	*NGetUIIconGeometry(const NUI *pui)
{
	if( ISFLAG_ON(pui->FlagsXtd,FLAG_NUI_XTD_ICON) )
	{
		// If UI has a background, ICON geometry is the second one !
		if( ISFLAG_ON(pui->FlagsXtd,FLAG_NUI_XTD_BACKGROUND) )
			return (NGEOMETRY*)NGetArrayPtr(&pui->pRenderable->GeomArray,1);
		else // ... but without UI Background, ICON geometry is the first one !
			return (NGEOMETRY*)pui->pRenderable->GeomArray.pFirst;
	}
	else
		return NULL;
}
// ------------------------------------------------------------------------------------------
// void NSetUIName
// ------------------------------------------------------------------------------------------
// Description :
//	Change the name of an UI. Be aware that's only possible if the UI flag FLAG_NUI_NAME_LOCKED 
//	is set to OFF. If is set to ON, name remains unchanged !!!
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetUIName(NUI *pui, const Nchar *pname)
{
	if(ISFLAG_OFF(pui->FlagsXtd,FLAG_NUI_XTD_NAME_LOCKED))
	{
		// First remove current Name
		if(pui->pName)
		{
			Nfree(pui->pName);
			pui->pName = NULL;
		}
		// a new name ?
		if(pname)
		{
			NErrorIf(!strlen(pname),NERROR_STRING_NULL);
			NErrorIf(strlen(pname)+1>CONSTANT_NUI_NAME_SIZEMAX, NERROR_UI_FRAME_NAME_TOO_LONG);
			
			pui->pName = (Nchar*)Nmalloc(NMIN(strlen(pname)+1,CONSTANT_NUI_NAME_SIZEMAX)*sizeof(Nchar));
			NStrCopy(pui->pName,pname,CONSTANT_NUI_NAME_SIZEMAX);
		}

	}
	else
	{
		NErrorIf(1,NERROR_UI_NAME_IS_LOCKED);
		return;
	}
}

NUI *NGetUIRoot(const NUI *pui)
{
	NHIERARCHY_NODE *proot;

	if(!pui)
		return NULL;
	
	proot = (NHIERARCHY_NODE *)pui;
	while(proot->pParent)
	{
		proot = (NHIERARCHY_NODE *)proot->pParent;
	}
	return (NUI *)proot;
}

// ------------------------------------------------------------------------------------------
// NUI* NSetUIParent_EndInsert
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
NUI* NSetUIParent_EndInsert( NUI *pnewparent, NUI *pui, const Nbool bupdate_Renderable_transformnode_parent )
{
	NErrorIf( pui == pnewparent, NERROR_UI_CYCLIC_HIERARCHY );
	NErrorIf( pui == pnewparent->pParent, NERROR_UI_CYCLIC_HIERARCHY );
	NErrorIf( NIsUIHidden(pnewparent) && NIsUIVisible(pui), NERROR_UI_INCOMPATIBLE_STATES); // Parent of visible UI MUST BE VISIBLE !!!
	NErrorIf( NIsUIDisable(pnewparent) && NIsUIEnable(pui), NERROR_UI_INCOMPATIBLE_STATES); // Parent of Enable UI MUST BE Enable !!!

	#ifdef _DEBUG
	Nbool	dbg_bfound;
	Nu32	dbg_i;
	NUI		**dbg_pptr;	
	#endif


	NUI *pprevparent = (NUI*)pui->pParent;

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// +---+ 0)	Remove 'pui' from it's current hierarchy ...
	// +		And Update, if necessary this current hierarchy ...touch event capture move in ... flags
	// +
	if( pprevparent )
	{
		// Remove 'pui' from its parent children list ...
		NNodeRemove((NNODE*)pui); 
		pui->pParent = NULL;

		// Manage ...TOUCH_EVENT_CAPTURE_MOVE_IN
		if( (pnewparent!= pprevparent) && ( NIsUICaptureTouchMoveIn(pui)||NIsUICaptureTouchMoveInDummy(pui) ))
		{
			// pParent FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN was obviously set to ON ( because 'pui' capture touch move in or is a dummy for that )
			// So, if, after removing pui, pprevparent doesn't have any more children that capture touch move In or that are dummy for that ...
			// ... update pprevparent to OFF !
			// otherwise (if it stills having children with those flags set to ON ... there is nothing to do actually ...
			if( !NAreUIChildrenInterestedByTouchMoveIn(pprevparent) )
			{
				if(ISFLAG_ON(pprevparent->Flags,FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN))
					NSetOFF_FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN(pprevparent);
			}
		}
		//else
		//{
		// Nothing special to do, new parent and previous parent are the same one !
		//}
	}
	else if( NIsUIVisible(pui) ) // ! No Parent + Visible = UI is Into Active UI Root List !!!!
	{

		// Remove temporary 'pui' from  ActiveUIRootList !!!!
		NUI		**ptr;
		ptr = (NUI**)NGetFirstArrayPtr(&ActiveUIRootsList);
		#ifdef _DEBUG
		dbg_bfound = NFALSE;
		#endif
		for( Nu32 i=0;i<ActiveUIRootsList.Size;i++,ptr++)
		{	
			if(*ptr == pui)
			{
				// Remove UI from ActiveUIRootList
				NEraseArrayElement(&ActiveUIRootsList,i,NULL);
				#ifdef _DEBUG
				dbg_bfound = NTRUE;
				#endif
				break;
			}
		}
		#ifdef _DEBUG
		NErrorIf(!dbg_bfound, NERROR_SYSTEM_CHECK);
		#endif

		// Manage ...TOUCH_EVENT_CAPTURE_MOVE_IN
		if( pnewparent && ( NIsUICaptureTouchMoveIn(pui) || NIsUICaptureTouchMoveInDummy(pui) ) )
		{
			// Then check ActiveUIRootList without 'pui' inside... And update NEngineCoreFlags if necessary
			if( !NAreActiveUIRootsInterestedByTouchMoveIn() )
				FLAG_OFF( NEngineCoreFlags,FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN );
		}
		//else
		//{
		// Nothing special to do
		//}
	}
	//else //// ! No Parent + Visible = Into Active UI Root List !!!!
	//{
	// Nothing special to do
	//}

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// +---+ 1) Insert UI in it's new Hierarchy
	// +	
	if( pnewparent )
	{
		NHNodeSetParent_EndInsert( (NHIERARCHY_NODE*)pnewparent, (NHIERARCHY_NODE*)pui );
		if( bupdate_Renderable_transformnode_parent )
		{
			// Be sure 'pui' ..._UPDATE_TAG are synchronized !
			NErrorIf(!NIsUISynchronizedWithTransformHNode(pui), NERROR_SYSTEM_CHECK);

			NSetTransformHNodeParent_EndInsert( pnewparent->pRenderable->pTransformHNode, pui->pRenderable->pTransformHNode );
		}
	}
	else if( NIsUIVisible(pui) ) // pui is Visible and is going to be without any parent ...
	{
		// Insert It into the Active UI Root List ...
		// So maybe pui was just removed from ActiveUIRootList, but it will be re-inserted at the End ...
		NArrayPushBack(&ActiveUIRootsList,(NBYTE*)&pui);
	}

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// +---+ 2) Update New Hierarchy ...CAPTURE_TOUCH_MOVE_IN... flags only if necessary
	// +	
	if( pnewparent!=pprevparent ) // if( pnewparent == pprevparent ) ... that means not necessary to update !
	{
		// There is a new parent and it's different than the previous one !
		if( pnewparent )
		{
			// pui has 1 or 2 Touch Event Capture Move In flags set to ON !
			if( NIsUICaptureTouchMoveIn(pui) || NIsUICaptureTouchMoveInDummy(pui) )
			{
				// pnewparent must have FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN set to ON !
				if( ISFLAG_OFF(pnewparent->Flags, FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN) )
					NSetON_FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN(pnewparent);
			}
			// pui has none of those 2 flags set to ON (both are OFF ) !
			else
			{
				// pnewparent must have FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN set to OFF !
				if( ISFLAG_ON(pnewparent->Flags, FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN) )
					NSetOFF_FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN(pnewparent);
			}
		}
		// The new parent is NULL, and it's different than the previous one ( NON-NULL) !
		// ... and UI is Visible !!! That means it is into ActiveUIRootList
		else if( NIsUIVisible(pui) )
		{
			// --------------------------------------------------------------
			// Check Visible UI is into ActiveUIRootList !
			#ifdef _DEBUG
			dbg_bfound = NFALSE;
			dbg_pptr = (NUI**)ActiveUIRootsList.pFirst;
			for( dbg_i = ActiveUIRootsList.Size; dbg_i!=0; dbg_i--, dbg_pptr ++ )
			{
				if(*dbg_pptr == pui)
				{
					dbg_bfound = NTRUE;
					break;
				}
			}
			NErrorIf(!dbg_bfound, NERROR_SYSTEM_CHECK);
			#endif
			// --------------------------------------------------------------

			// pui has 1 or 2 Touch Event Capture Move In flags set to ON !
			if( NIsUICaptureTouchMoveIn(pui) || NIsUICaptureTouchMoveInDummy(pui) )
			{
				// NEngineCoreFLags must have FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN set to ON !
				if( ISFLAG_OFF(NEngineCoreFlags, FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN) )
					FLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN); 
			}
			// pui has none of those 2 flags set to ON (both are OFF ) !
			else
			{
				// NEngineCoreFLags must have FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN set to OFF !
				if( ISFLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN) )
					FLAG_OFF(NEngineCoreFlags, FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN); 
			}
		}
		// The new parent is NULL, and it's different than the previous one ( NON-NULL) !
		// ... and UI is NOT Visible ...
		//else
		//{
		// Nothing special to do
		//}
	}
	return pui;
}

NUI* NSetUIParent_BeginningInsert( NUI *pnewparent, NUI *pui, const Nbool bupdate_Renderable_transformnode_parent )
{
	NErrorIf( pui == pnewparent, NERROR_UI_CYCLIC_HIERARCHY );
	NErrorIf( pui == pnewparent->pParent, NERROR_UI_CYCLIC_HIERARCHY );
	NErrorIf( NIsUIHidden(pnewparent) && NIsUIVisible(pui), NERROR_UI_INCOMPATIBLE_STATES); // Parent of visible UI MUST BE VISIBLE !!!
	NErrorIf( NIsUIDisable(pnewparent) && NIsUIEnable(pui), NERROR_UI_INCOMPATIBLE_STATES); // Parent of Enable UI MUST BE Enable !!!

	#ifdef _DEBUG
	Nbool	dbg_bfound;
	Nu32	dbg_i;
	NUI		**dbg_pptr;	
	#endif

	NUI *pprevparent = (NUI*)pui->pParent;

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// +---+ 0)	Remove 'pui' from it's current hierarchy ...
	// +		And Update, if necessary this current hierarchy ...touch event capture move in ... flags
	// +
	if( pprevparent )
	{
		// Remove 'pui' from its parent children list ...
		NNodeRemove((NNODE*)pui); 
		pui->pParent = NULL;

		// Manage ...TOUCH_EVENT_CAPTURE_MOVE_IN
		if( (pnewparent!= pprevparent) && ( NIsUICaptureTouchMoveIn(pui)||NIsUICaptureTouchMoveInDummy(pui) ))
		{
			// pParent FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN was obviously set to ON ( because pui FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN or/and FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN is/are ON ! )
			// So, if, after removing pui pprevparent doesn't have any more children with FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN or FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN set to ON ...
			// ... update it to OFF !
			// otherwise (if it stills having children with those flags set to ON ... there is nothing to do actually ...
			if( !NAreUIChildrenInterestedByTouchMoveIn(pprevparent) )
			{
				if(ISFLAG_ON(pprevparent->Flags,FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN))
					NSetOFF_FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN(pprevparent);
			}
		}
		//else
		//{
		// Nothing special to do
		//}
	}
	else if( NIsUIVisible(pui) ) // No Parent + Visible = Into Active UI Root List !!!!
	{

		// Remove 'pui' from  ActiveUIRootList !!!!
		NUI		**ptr;
		ptr = (NUI**)NGetFirstArrayPtr(&ActiveUIRootsList);
		#ifdef _DEBUG
		dbg_bfound = NFALSE;
		#endif
		for( Nu32 i=0;i<ActiveUIRootsList.Size;i++,ptr++)
		{	
			if(*ptr == pui)
			{
				// Remove UI from ActiveUIRootList
				NEraseArrayElement(&ActiveUIRootsList,i,NULL);
				#ifdef _DEBUG
				dbg_bfound = NTRUE;
				#endif
				break;
			}
		}
		#ifdef _DEBUG
		NErrorIf(!dbg_bfound, NERROR_SYSTEM_CHECK);
		#endif

		// Manage ...TOUCH_EVENT_CAPTURE_MOVE_IN
		if( pnewparent && ( NIsUICaptureTouchMoveIn(pui) ||NIsUICaptureTouchMoveInDummy(pui) ) )
		{
			// Then check ActiveUIRootList without 'pui' inside... And update NEngineCoreFlags if necessary
			if( !NAreActiveUIRootsInterestedByTouchMoveIn() )
				FLAG_OFF( NEngineCoreFlags,FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN );
		}
		//else // No Parent + Visible = Into Active UI Root List !!!!
		//{
		// Nothing special to do
		//}
	}
	//else //// ! No Parent + No Visible !!!!
	//{
	// Nothing special to do
	//}

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// +---+ 1) Insert UI in it's new Hierarchy
	// +	
	if( pnewparent )
	{
		NHNodeSetParent_BeginningInsert( (NHIERARCHY_NODE*)pnewparent, (NHIERARCHY_NODE*)pui );
		if( bupdate_Renderable_transformnode_parent )
		{
			// Be sure 'pui' ..._UPDATE_TAG are synchronized !
			NErrorIf(!NIsUISynchronizedWithTransformHNode(pui), NERROR_SYSTEM_CHECK);
			
			NSetTransformHNodeParent_BeginningInsert( pnewparent->pRenderable->pTransformHNode, pui->pRenderable->pTransformHNode );
		}
	}
	else if( NIsUIVisible(pui) ) // pui is Visible and is going to be without any parent ...
	{
		// Insert It into the Active UI Root List ...
		// So maybe pui was just removed from ActiveUIRootList, but it will be re-inserted at the Beginning ...
		NInsertArrayElement(&ActiveUIRootsList,0,(NBYTE*)&pui);
	}

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// +---+ 2) Update New Hierarchy ...CAPTURE_TOUCH_MOVE_IN... flags only if necessary
	// +	
	if( pnewparent!=pprevparent ) // if( pnewparent == pprevparent ) ... that means not necessary to update !
	{
		// There is a new parent and it's different than the previous one !
		if( pnewparent )
		{
			// pui has 1 or 2 Touch Event Capture Move In flags set to ON !
			if( NIsUICaptureTouchMoveIn(pui) || NIsUICaptureTouchMoveInDummy(pui) )
			{
				// pnewparent must have FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN set to ON !
				if( ISFLAG_OFF(pnewparent->Flags, FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN) )
					NSetON_FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN(pnewparent);
			}
			// pui has none of those 2 flags set to ON (both are OFF ) !
			else
			{
				// pnewparent must have FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN set to OFF !
				if( ISFLAG_ON(pnewparent->Flags, FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN) )
					NSetOFF_FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN(pnewparent);
			}
		}
		// The new parent is NULL, and it's different than the previous one ( NON-NULL) !
		// ... and UI is Visible !!! That means it is into ActiveUIRootList
		else if( NIsUIVisible(pui) )
		{
			// --------------------------------------------------------------
			// Check Visible UI is into ActiveUIRootList !
			#ifdef _DEBUG
			dbg_bfound = NFALSE;
			dbg_pptr = (NUI**)ActiveUIRootsList.pFirst;
			for( dbg_i = ActiveUIRootsList.Size; dbg_i!=0; dbg_i--, dbg_pptr ++ )
			{
				if(*dbg_pptr == pui)
				{
					dbg_bfound = NTRUE;
					break;
				}
			}
			NErrorIf(!dbg_bfound, NERROR_SYSTEM_CHECK);
			#endif
				// --------------------------------------------------------------

				// pui has 1 or 2 Touch Event Capture Move In flags set to ON !
				if( NIsUICaptureTouchMoveIn(pui) || NIsUICaptureTouchMoveInDummy(pui) )
				{
					// NEngineCoreFLags must have FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN set to ON !
					if( ISFLAG_OFF(NEngineCoreFlags, FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN) )
						FLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN); 
				}
				// pui has none of those 2 flags set to ON (both are OFF ) !
				else
				{
					// NEngineCoreFLags must have FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN set to OFF !
					if( ISFLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN) )
						FLAG_OFF(NEngineCoreFlags, FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN); 
				}
		}
		// The new parent is NULL, and it's different than the previous one ( NON-NULL) !
		// ... and UI is NOT Visible ...
		//else
		//{
		// Nothing special to do
		//}
	}
	return pui;
}


// ------------------------------------------------------------------------------------------
// static inline void set_ui_pickingmesh__clear_current_ui_picking_mesh_data
// ------------------------------------------------------------------------------------------
// Description :
//	This local inline function reset what it has to be reset, only !!!
//	It's not a full reset function, only usable by the 3 functions:
//												NSetUIPickingGeometryMesh
//												NSetUICustomPickingGeometryMesh
//												NSetUITextAsPickingMesh
//
//	NOTICE this function perform some consistency checks at the same time !
// ------------------------------------------------------------------------------------------
// In	:
//			NUI *pui		a valid pointer to the UI in update
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
static inline void set_ui_pickingmesh__clear_current_ui_picking_mesh_data( NUI *pui )
{
	// Check current picking method to perform some clearings ...
	// There are 3 possibilities for Picking mesh (and functions linked with )...
	//		case 1- Picking Mesh is a Custom Mesh
	//		case 2- Picking Mesh is all meshes of all UI.Text geometries.
	//		case 3- Picking Mesh is one of the "UI.pRenderable.GeometryArray" geometry.Mesh.
	// 

	// 1/ Current Picking Method uses CUSTOM PICKING MESH
	if(ISFLAG_ON(pui->FlagsXtd,FLAG_NUI_XTD_CUSTOM_PICKING_MESH))
	{
		// It seems there is no change to make for BBox Update function and for picking function...
		// ... just some consistency checks in DEBUG MODE.
		// Notice that here, 'pui->BoundingBoxUpdating_Proc' may have 2 values:
		//								NUpdateUIBBox_From_PickingMesh
		//								NUpdateUIBBox_From_ChildrenBBox
		//
		// Notice that here, 'pui->Pick_Proc' may have 2 values:
		//								NPickUIPickingMesh
		//								NPickUIBBox
		NErrorIf((pui->BoundingBoxUpdating_Proc != NUpdateUIBBox_From_PickingMesh)&&(pui->BoundingBoxUpdating_Proc != NUpdateUIBBox_From_ChildrenBBox),	NERROR_INCONSISTENT_VALUES);
		NErrorIf((pui->Pick_Proc != NPickUIPickingMesh)&&(pui->Pick_Proc != NPickUIBBox),NERROR_INCONSISTENT_VALUES);
		// ... and some other inconsistency checking
		NErrorIf(ISFLAG_ON(pui->FlagsXtd,FLAG_NUI_XTD_TEXT_PICKING_MESH),NERROR_INCONSISTENT_FLAGS);
		NErrorIf(!pui->pPickingGeometryMesh,NERROR_NULL_POINTER);

		// So ... Clear !
		FLAG_OFF(pui->FlagsXtd,FLAG_NUI_XTD_CUSTOM_PICKING_MESH);
		NDeleteMesh(pui->pPickingGeometryMesh);
		pui->pPickingGeometryMesh		= NULL;
	}
	// 2/ Current Picking Method uses TEXT MESHES
	else if(ISFLAG_ON(pui->FlagsXtd,FLAG_NUI_XTD_TEXT_PICKING_MESH))
	{
		// Notice that here, 'pui->BoundingBoxUpdating_Proc' may have 2 values:
		//								NUpdateUIBBox_From_TextMesh
		//								NUpdateUIBBox_From_ChildrenBBox
		// If it's "NUpdateUIBBox_From_ChildrenBBox" we will keep it, but change it if it's "NUpdateUIBBox_From_TextMesh"
		//
		NErrorIf((pui->BoundingBoxUpdating_Proc != NUpdateUIBBox_From_TextMesh)&&(pui->BoundingBoxUpdating_Proc != NUpdateUIBBox_From_ChildrenBBox),	NERROR_INCONSISTENT_VALUES);
		// Notice that here, 'pui->Pick_Proc' may have 2 values:
		//								NPickUIPickingTextMesh
		//								NPickUIBBox
		// If it's "NPickUIBBox" we will keep it, but change it if it's "NPickUIPickingTextMesh"
		//
		NErrorIf((pui->Pick_Proc != NPickUIPickingTextMesh)&&(pui->Pick_Proc != NPickUIBBox),NERROR_INCONSISTENT_VALUES);
		
		NErrorIf((pui->Pick_Proc == NPickUIPickingTextMesh) && ISFLAG_OFF( pui->FlagsXtd, FLAG_NUI_XTD_PRECISE_PICKING ), NERROR_INCONSISTENT_FLAGS);// it should set to OFF Because Pick_Proc is 'NPickUIBBox'
		NErrorIf((pui->Pick_Proc == NPickUIBBox) && ISFLAG_ON( pui->FlagsXtd, FLAG_NUI_XTD_PRECISE_PICKING ), NERROR_INCONSISTENT_FLAGS);// it should set to OFF Because Pick_Proc is 'NPickUIBBox'
		
		NErrorIf(pui->pPickingGeometryMesh != NULL, NERROR_INCONSISTENT_VALUES);

		// So ... Clear !
		FLAG_OFF(pui->FlagsXtd,FLAG_NUI_XTD_TEXT_PICKING_MESH);
	}
	// 3/	Current Picking Method uses a PICKING MESH included into UI Renderable geometries array
	//		... or maybe is just 'void' (no picking mesh specified at creation)
	else
	{
		// Notice that here, 'pui->BoundingBoxUpdating_Proc' may have 3 values:
		//								NUpdateUIBBox_From_PickingMesh
		//								NUpdateUIBBox_From_ChildrenBBox
		//								NUpdateUIBBox_Void
		// If it's "NUpdateUIBBox_From_ChildrenBBox" or "NUpdateUIBBox_From_PickingMesh" we will keep it, but change it if it's "NUpdateUIBBox_Void"
		//
		NErrorIf((pui->BoundingBoxUpdating_Proc != NUpdateUIBBox_From_PickingMesh)&&(pui->BoundingBoxUpdating_Proc != NUpdateUIBBox_From_ChildrenBBox)&&(pui->BoundingBoxUpdating_Proc != NUpdateUIBBox_Void),	NERROR_INCONSISTENT_VALUES);
		// Notice that here, 'pui->Pick_Proc' may have 2 values:
		//								NPickUIPickingMesh
		//								NPickUIBBox
		//
		NErrorIf((pui->Pick_Proc != NPickUIPickingMesh)&&(pui->Pick_Proc != NPickUIBBox),NERROR_INCONSISTENT_VALUES);
		
		// So ... Clear !
		pui->pPickingGeometryMesh		= NULL;
	}

	// ... to enforce a reset of these 2 functions after using 'set_ui_pickingmesh__clear_current_ui_picking_mesh_data'
	pui->Pick_Proc					= NULL;
	pui->BoundingBoxUpdating_Proc	= NULL;
}

// ------------------------------------------------------------------------------------------
// void NResetUIPickingMesh
// ------------------------------------------------------------------------------------------
// Description :
//	Reset current UI picking method to the basic one which is PICKING a static BBOX (static = no update).
//	If current UI picking method uses a custom mesh, this one will be deleted.
//	Notice that is not the purpose of this function to "STOP" picking process. 
//	This one can not be stopped actually. BUT it can be asleep ... thanks to the Enable/Disable functions
//
// ------------------------------------------------------------------------------------------
// In	:
//			NUI				*pui				a valid pointer on UI to update 
//			const NRECTf32	*pboundingbox		a valid pointer on the NEW static BBOX ( will be copied into UI data)
//												or a NULL pointer to specify full screen BBOX. 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NResetUIPickingMesh(NUI *pui, const NRECTf32 *pboundingbox)
{
	// 0)Reset current status
	set_ui_pickingmesh__clear_current_ui_picking_mesh_data(pui);
	// 1)FLAGS: no flags to set ON

	// 2)Pick_Proc: Precise or Not ... doesn't matter here, because there is no Mesh for Precise Picking
	pui->Pick_Proc = NPickUIBBox;
	// 3)BoundingBoxUpdating_Proc:
	pui->BoundingBoxUpdating_Proc = NUpdateUIBBox_Void;
	// 4)Store the NEW picking Mesh
	pui->pPickingGeometryMesh = NULL;
	// 5)... and Update transformed vertex Array according with
	NResizeArray(&pui->PickingGeometryTransformedVertexArray,0,NULL,NULL);
	// 6)BBox Update:
	if(pboundingbox)
	{
		pui->BoundingBox = *pboundingbox;
	}
	else // By default Reset function is going to setup BoundingBox as full screen. 
	{
 		pui->BoundingBox.upper_left.x	=  0.0f;
 		pui->BoundingBox.upper_left.y	=  0.0f;
 		pui->BoundingBox.lower_right.x	=  NGetViewPort_AspectRatio();
 		pui->BoundingBox.lower_right.y	= -1.0f;
	}
}
void NSetUIPickingGeometryMesh(NUI *pui, const Nu32 internal_ui_geometry_id)
{
	// Get Incoming Picking Mesh
	NMESH *pmesh = &((NGEOMETRY*)NGetArrayPtr(&pui->pRenderable->GeomArray,internal_ui_geometry_id))->Mesh;
	NErrorIf(pmesh->PrimitiveArray.Size < 1,NERROR_GEOMETRY_NOT_ENOUGH_PRIMITIVE );
	NErrorIf(pmesh->VertexArray.Size < 3,	NERROR_GEOMETRY_NOT_ENOUGH_VERTEX );

	// 0)Reset current status
	set_ui_pickingmesh__clear_current_ui_picking_mesh_data(pui);
	// 1)FLAGS: no flags to set ON
	
	// 2)Pick_Proc: Precise or Not ...
	if( ISFLAG_ON( pui->FlagsXtd, FLAG_NUI_XTD_PRECISE_PICKING ) )
	{
		pui->Pick_Proc = NPickUIPickingMesh;
	}
	else
	{
		pui->Pick_Proc = NPickUIBBox;
	}
	// 3)BoundingBoxUpdating_Proc:
	pui->BoundingBoxUpdating_Proc = NUpdateUIBBox_From_PickingMesh;
	// 4)Store the NEW picking Mesh
	pui->pPickingGeometryMesh = pmesh;
	// 5)... and Update transformed vertex Array according with
	NResizeArray(&pui->PickingGeometryTransformedVertexArray,pmesh->VertexArray.Size,NULL,NULL);
}

void NSetUICustomPickingMesh(NUI *pui, const NMESH *pcustommesh)
{
	NErrorIf(!pcustommesh,							NERROR_NULL_POINTER);
	NErrorIf(pcustommesh->PrimitiveArray.Size < 1,	NERROR_GEOMETRY_NOT_ENOUGH_PRIMITIVE );
	NErrorIf(pcustommesh->VertexArray.Size < 3,		NERROR_GEOMETRY_NOT_ENOUGH_VERTEX );

	// 0)Reset current status
	set_ui_pickingmesh__clear_current_ui_picking_mesh_data(pui);
	// 1)FLAGS: 
	FLAG_ON(pui->FlagsXtd,FLAG_NUI_XTD_CUSTOM_PICKING_MESH);
	// 2)Pick_Proc: Precise or Not ...
	if( ISFLAG_ON( pui->FlagsXtd, FLAG_NUI_XTD_PRECISE_PICKING ) )
	{
		pui->Pick_Proc = NPickUIPickingMesh;
	}
	else
	{
		pui->Pick_Proc = NPickUIBBox;
	}
	// 3)BoundingBoxUpdating_Proc:
	pui->BoundingBoxUpdating_Proc = NUpdateUIBBox_From_PickingMesh;
	// 4)Store the NEW picking Mesh
	pui->pPickingGeometryMesh	= _create_custom_pickingmesh(pcustommesh);
	// 5)... and Update transformed vertex Array according with
	NResizeArray(&pui->PickingGeometryTransformedVertexArray,pui->pPickingGeometryMesh->VertexArray.Size,NULL,NULL);
}

void NSetUITextAsPickingMesh(NUI *pui)
{
	// Check if UI has a valid Text inside, if not, just return in release mode but CRASH in debug Mode
	if(!pui->pText)
	{
		NErrorIf(1,NERROR_NULL_POINTER);
		return;
	}

	// 0)Reset current status
	set_ui_pickingmesh__clear_current_ui_picking_mesh_data(pui);
	// 1)FLAGS: 
	FLAG_ON(pui->FlagsXtd,FLAG_NUI_XTD_TEXT_PICKING_MESH);
	// 2)Pick_Proc: Precise or Not ...
	if( ISFLAG_ON( pui->FlagsXtd, FLAG_NUI_XTD_PRECISE_PICKING ) )
	{
		pui->Pick_Proc = NPickUIPickingTextMesh;
	}
	else
	{
		pui->Pick_Proc = NPickUIBBox;
	}
	// 3)BoundingBoxUpdating_Proc:
	pui->BoundingBoxUpdating_Proc = NUpdateUIBBox_From_TextMesh;
	// 4)Store the NEW picking Mesh
	pui->pPickingGeometryMesh	= NULL;
	// 5)... and Update transformed vertex Array according with
	// No needs to resize "pui->PickingGeometryTransformedVertexArray" because for Text picking Mesh it will be done Dynamicly
}
// ------------------------------------------------------------------------------------------
// void	NSetUIListenTouchMode
// ------------------------------------------------------------------------------------------
// Description :
//	Setup all the Touch mode Flags in one call, thanks to predefine Touch Mode.
//	( all the available touch mode flags combinations have their specific predefined touch mode )
//	User can change that but only if UI is not listening ( that means without any bound listener )
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void	NSetUIListenTouchMode(NUI *pui, const NUI_LISTEN_TOUCH_MODE_ENUM touch_mode)
{
	// ---------------------- FLAGS_NUI_TOUCH_MODE ( FLAGS compositions )-------------------------------------------------------------------
	//
	// ---------------------------------------------------------------------------------------------------------------------------------------
	//		S			U	|
	//		T			N	|
	//		A		O	D	|
	//		R	I	U	E	|
	//		T	N	T	R	|
	// ---------------------------------------------------------------------------------------------------------------------------------------
	//		X	X	X	X	|	Inconsistent		- TRACK MOVE IN OUT behavior and FLAG_NUI_RELEASE_TOUCH_MOVE_OUT are not compatible.
	//		X	X	X	.	|	MODE ON_THE_FLY		- Capture Touch Start and Touch Move In And keep them it until Touch End/Cancel, with specific UNDER TOUCH CONTROL behavior.
	//		X	X	.	X	|	MODE GREED_UNDER	- Capture Touch Start and Touch Move In And keep them it until Touch End/Cancel, with specific UNDER TOUCH CONTROL behavior.
	//		X	X	.	.	|	MODE GREED			- Capture Touch Start and Touch Move In And keep them it until Touch End/Cancel, without any specific behavior.
	//		X	.	X	X	|	Inconsistent		- TRACK MOVE IN OUT behavior and FLAG_NUI_RELEASE_TOUCH_MOVE_OUT are not compatible.
	//		X	.	X	.	|	MODE DROP			- Capture Touch Start and keep it until Move Out (or End/Cancel) , without any specific behavior.
	//		X	.	.	X	|	MODE BASIC_UNDER	- Capture Touch Start and keep it until Touch End/Cancel, with specific UNDER TOUCH CONTROL behavior.	
	//		X	.	.	.	|	MODE BASIC			- Capture Touch Start and keep it until Touch End/Cancel, without any specific behavior.
	//		.	X	X	X	|	Inconsistent		- TRACK MOVE IN OUT behavior and FLAGS settings are not compatible.
	//		.	X	X	.	|	MODE CATCH_RELEASE	- Capture Touch Move In Only ( not Touch Start )  and Release Touch at Move Out (or End/Cancel), without any specific behavior.	
	//		.	X	.	X	|	MODE CATCH_UNDER	- Capture Touch Move In Only ( not Touch Start )  and keep it until Touch End/Cancel, with specific UNDER TOUCH CONTROL behavior.
	//		.	X	.	.	|	MODE CATCH			- Capture Touch Move In Only ( not Touch Start )  and keep it until Touch End/Cancel, without any specific behavior.
	//		.	.	X	X	|	Inconsistent		- An UI can not behave TRACK MOVE IN OUT or Release Touch at Move Out (or End/Cancel) without any activated capture touch -- AND -- TRACK MOVE IN OUT behavior and FLAG_NUI_RELEASE_TOUCH_MOVE_OUT are not compatible. 
	//		.	.	X	.	|	Inconsistent		- An UI can not Release Touch at Move Out (or End/Cancel) without any activated capture touch ...
	//		.	.	.	X	|	Inconsistent		- An UI can not behave TRACK MOVE IN OUT without any activated capture touch ...
	//		.	.	.	.	|	MODE NULL			- No Capture/ No Release ... 

	// To be changed, an UI does not listening !
	NErrorIf( pui->pTouchUIListener, NERROR_UNAUTHORIZED_ACTION );

	switch(touch_mode)
	{
		case NUI_LISTEN_TOUCH_MODE_NULL:
			// First, update special flag FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN
			if( ISFLAG_ON(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN) )
				NDisableUIListenTouch_CaptureMoveIn(pui);
			// Then update others
			FLAG_OFF(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START
								|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT
								|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT		);
			// That means:
			// All the 4 Flags to OFF !
			break;

		case NUI_LISTEN_TOUCH_MODE_CATCH:			
			// First, update special flag FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN
			if( ISFLAG_OFF(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN) )
				NEnableUIListenTouch_CaptureMoveIn(pui);
			// Then update others
			FLAG_OFF(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START
								|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT
								|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT		);
			// That means:
			// FLAG_ON(pui->Flags, FLAG_NUI_CAPTURE_TOUCH_MOVE_IN);
			break;

		case NUI_LISTEN_TOUCH_MODE_CATCH_TRACK:
			// First, update special flag FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN
			if( ISFLAG_OFF(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN) )
				NEnableUIListenTouch_CaptureMoveIn(pui);
			// Then update others
			FLAG_ON(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT);
			FLAG_OFF(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START
								|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT	);
			// That means:
			// FLAG_ON(pui->Flags, FLAG_NUI_CAPTURE_TOUCH_MOVE_IN);
			// FLAG_ON(pui->Flags, FLAG_NUI_UNDER_TOUCH_CONTROL);
			break;

		case NUI_LISTEN_TOUCH_MODE_CATCH_RELEASE:
			// First, update special flag FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN
			if( ISFLAG_OFF(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN) )
				NEnableUIListenTouch_CaptureMoveIn(pui);
			// Then update others
			FLAG_ON(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT);
			FLAG_OFF(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START
								|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT		);
			// That means:
			// FLAG_ON(pui->Flags, FLAG_NUI_CAPTURE_TOUCH_MOVE_IN);
			// FLAG_ON(pui->Flags, FLAG_NUI_RELEASE_TOUCH_MOVE_OUT);
			break;

		case NUI_LISTEN_TOUCH_MODE_BASIC:			
			// First, update special flag FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN
			if( ISFLAG_ON(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN) )
				NDisableUIListenTouch_CaptureMoveIn(pui);
			// Then update others
			FLAG_ON(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START);
			FLAG_OFF(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT
								|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT		);
			// That means:
			// FLAG_ON(pui->Flags, FLAG_NUI_CAPTURE_TOUCH_START);
			break;

		case NUI_LISTEN_TOUCH_MODE_BASIC_TRACK:
			// First, update special flag FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN
			if( ISFLAG_ON(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN) )
				NDisableUIListenTouch_CaptureMoveIn(pui);
			// Then update others
			FLAG_ON(pui->Flags,  FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START
								|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT		);
			FLAG_OFF(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT );
			// That means:
			// FLAG_ON(pui->Flags, FLAG_NUI_CAPTURE_TOUCH_START);
			// FLAG_ON(pui->Flags, FLAG_NUI_UNDER_TOUCH_CONTROL);
			break;

		case NUI_LISTEN_TOUCH_MODE_DROP:
			// First, update special flag FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN
			if( ISFLAG_ON(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN) )
				NDisableUIListenTouch_CaptureMoveIn(pui);
			// Then update others
			FLAG_ON(pui->Flags,  FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START
								|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT	);
			FLAG_OFF(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT);
			// That means:
			// FLAG_ON(pui->Flags, FLAG_NUI_CAPTURE_TOUCH_START);
			// FLAG_ON(pui->Flags, FLAG_NUI_RELEASE_TOUCH_MOVE_OUT);
			break;

		case NUI_LISTEN_TOUCH_MODE_GREED:
			// First, update special flag FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN
			if( ISFLAG_OFF(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN) )
				NEnableUIListenTouch_CaptureMoveIn(pui);
			// Then update others
			FLAG_ON(pui->Flags,	 FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START);
			FLAG_OFF(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT
								|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT		);
			
			// That means:
			// FLAG_ON(pui->Flags, FLAG_NUI_CAPTURE_TOUCH_START);
			// FLAG_ON(pui->Flags, FLAG_NUI_CAPTURE_TOUCH_MOVE_IN);
			break;

		case NUI_LISTEN_TOUCH_MODE_GREED_TRACK:
			// First, update special flag FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN
			if( ISFLAG_OFF(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN) )
				NEnableUIListenTouch_CaptureMoveIn(pui);
			// Then update others
			FLAG_ON(pui->Flags,  FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START
								|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT	);
			FLAG_OFF(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT);

			// That means:
			// FLAG_ON(pui->Flags, FLAG_NUI_CAPTURE_TOUCH_START);
			// FLAG_ON(pui->Flags, FLAG_NUI_CAPTURE_TOUCH_MOVE_IN);
			// FLAG_ON(pui->Flags, FLAG_NUI_UNDER_TOUCH_CONTROL);
			break;

		case NUI_LISTEN_TOUCH_MODE_ON_THE_FLY:
			// First, update special flag FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN
			if( ISFLAG_OFF(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN) )
				NEnableUIListenTouch_CaptureMoveIn(pui);
			// Then update others
			FLAG_ON(pui->Flags,  FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START
								|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT	);
			FLAG_OFF(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT);
			
			// That means:
			// FLAG_ON(pui->Flags, FLAG_NUI_CAPTURE_TOUCH_START);
			// FLAG_ON(pui->Flags, FLAG_NUI_CAPTURE_TOUCH_MOVE_IN);
			// FLAG_ON(pui->Flags, FLAG_NUI_RELEASE_TOUCH_MOVE_OUT);
			break;
		
		default:
			NErrorIf(1,NERROR_UNAUTHORIZED_CASE);
			FLAG_ON(pui->Flags, FLAGS_NUI_TOUCH_MODE_BASIC);
			break;
	}
}

// ------------------------------------------------------------------------------------------
// void NEnableUIListenTouch_CaptureStart
// ------------------------------------------------------------------------------------------
// Description :
//	Enable the Touch capture mode to Capture touch start (only). That means the UI is going to capture 
//	Touch for listening at TOUCH START!
//	By default, this option is activated for all UI !
//	User can change that but only if UI is not listening ( that means without any bound listener )
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NEnableUIListenTouch_CaptureStart(NUI *pui)
{
	// To be changed, an UI does not listening !
	NErrorIf( pui->pTouchUIListener, NERROR_UNAUTHORIZED_ACTION );
	FLAG_ON(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START);

	// Post checks ( Check all the inconsistent flags compositions )
	// Now, these tests are made at the beginning of the function "NDispatchEvent".
/*
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
*/
}
// ------------------------------------------------------------------------------------------
// void NDisableUIListenTouch_CaptureStart
// ------------------------------------------------------------------------------------------
// Description :
//	Disable capture touch start (only). That means the UI is not going to capture 
//	Touch for listening at TOUCH START.
//	By default, this option is activated for all UI !
//	User can change that but only if UI is not listening ( that means without any bound listener )
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NDisableUIListenTouch_CaptureStart(NUI *pui)
{
	// To be changed, an UI does not listening !
	NErrorIf( pui->pTouchUIListener, NERROR_UNAUTHORIZED_ACTION );
	FLAG_OFF(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START);
	
	// Post checks ( Check all the inconsistent flags compositions )
	// Now, these tests are made at the beginning of the function "NDispatchEvent".
/*
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
*/
}
// ------------------------------------------------------------------------------------------
// void NEnableUIListenTouch_CaptureMoveIn
// ------------------------------------------------------------------------------------------
// Description :
//	Enable the Touch capture mode to Capture touch move In. That means the UI is going to capture 
//	Touch for listening at TOUCH Move In !
//	By default, this option is deactivated for all UI !
//	User can change that but only if UI is not listening ( that means without any bound listener )
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NEnableUIListenTouch_CaptureMoveIn(NUI *pui)
{
	// To be changed, an UI does not listening !
	NErrorIf( pui->pTouchUIListener, NERROR_UNAUTHORIZED_ACTION );
	
	if(	ISFLAG_OFF(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN) )
	{
		FLAG_ON(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN);

		// Post checks ( Check all the inconsistent flags compositions )
		// Now, these tests are made at the beginning of the function "NDispatchEvent".
/*
		NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
		NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT ), NERROR_INCONSISTENT_FLAGS );
		NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
		NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
		NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
		NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
*/

		// Parent Update ( for Process optimization ) 
		if( pui->pParent )
		{
			if( ISFLAG_OFF(((NUI*)pui->pParent)->Flags,FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN ))
				NSetON_FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN((NUI*)pui->pParent);
		}
		// ... If UI doesn't have parent and is Visible ... that means it's registered into the ActiveUI Array !!!
		else if( NIsUIVisible(pui) && ISFLAG_OFF(NEngineCoreFlags, FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN) )
		{
			#ifdef _DEBUG
			Nbool bfound = NFALSE;
			NUI ** pptr = (NUI**)ActiveUIRootsList.pFirst;
			for( Nu32 i = ActiveUIRootsList.Size; i!=0; i--, pptr ++ )
			{
				if(*pptr == pui)
				{
					bfound = NTRUE;
					break;
				}
			}
			NErrorIf(!bfound, NERROR_SYSTEM_CHECK);
			#endif

			FLAG_ON(NEngineCoreFlags, FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN);
		}
	}
}
// ------------------------------------------------------------------------------------------
// void NDisableUIListenTouch_CaptureMoveIn
// ------------------------------------------------------------------------------------------
// Description :
//	Disable capture touch Move In. That means the UI is not going to capture 
//	Touch for listening at TOUCH Move In.
//	By default, this option is Deactivated for all UI !
//	User can change that but only if UI is not listening ( that means without any bound listener )
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NDisableUIListenTouch_CaptureMoveIn(NUI *pui)
{
	// To be changed, an UI does not listening !
	NErrorIf( pui->pTouchUIListener, NERROR_UNAUTHORIZED_ACTION );

	if(	ISFLAG_ON(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN) )
	{
		FLAG_OFF(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN);

		// Post checks ( Check all the inconsistent flags compositions )
		// Now, these tests are made at the beginning of the function "NDispatchEvent".
/*
		NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
		NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT ), NERROR_INCONSISTENT_FLAGS );
		NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
		NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
		NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
		NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
*/

		// Parent Update ( for Process optimization ) 
		if(pui->pParent)
		{
			NUI *pparent = (NUI*)pui->pParent;		
			// Assume pui->Flags was ON before Toggled to OFF, so its parent must be flaged accordingly !!! 
			// That means pui->pParent should be flagged ON !
			NErrorIf(ISFLAG_OFF(pparent->Flags,FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN), NERROR_SYSTEM_CHECK);
			// Lets check, the pparent state after changing FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN of pui to OFF.
			// if it stills with children/hierarchy that manage touch move in ... nothing to do, otherwise turns its
			// FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN to OFF
			if( !NAreUIChildrenInterestedByTouchMoveIn(pparent) )
				NSetOFF_FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN(pparent);
		}
		else if( NIsUIVisible(pui) ) // No parent + Is Visible ---> Is In ActiveUIRootList !!
		{
			#ifdef _DEBUG
			Nbool bfound = NFALSE;
			NUI ** pptr = (NUI**)ActiveUIRootsList.pFirst;
			for( Nu32 i = ActiveUIRootsList.Size; i!=0; i--, pptr ++ )
			{
				if(*pptr == pui)
				{
					bfound = NTRUE;
					break;
				}
			}
			NErrorIf(!bfound, NERROR_SYSTEM_CHECK);
			#endif
			if( !NAreActiveUIRootsInterestedByTouchMoveIn() )
				FLAG_OFF(NEngineCoreFlags,FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN);
		}
	}
}
// ------------------------------------------------------------------------------------------
// void NEnableUIListenTouch_ReleaseMoveOut
// ------------------------------------------------------------------------------------------
// Description :
//	Enable the Touch Release at touch move Out. That means the UI is going to release the
//	Touch when it move out of the UI Shape!
//	By default, this option is deactivated for all UI !
//	User can change that but only if UI is not listening ( that means without any bound listener )
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NEnableUIListenTouch_ReleaseMoveOut(NUI *pui)
{
	// To be changed, an UI does not listening !
	NErrorIf( pui->pTouchUIListener, NERROR_UNAUTHORIZED_ACTION );
	// Touch Move Out is not suppose to be set to ON with Track Move In Out behavior ... 
	// It will not create a problem because those 2 flags are going to be used in 2 different process, 
	// but it's better to Warn user about this inconsistency. 
	// 'FLAG_NUI_UNDER_TOUCH_CONTROL' is used by UI that keep the touch from START/MOVE IN to END/CANCEL.
	//
	NErrorIf(ISFLAG_ON(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT), NERROR_INCONSISTENT_FLAGS);
	
	// Parent Update ( for Process optimization ) 
	FLAG_ON(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT);

	// Post checks ( Check all the inconsistent flags compositions )
	// Now, these tests are made at the beginning of the function "NDispatchEvent".
/*
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
*/
}
// ------------------------------------------------------------------------------------------
// void NDisableUIListenTouch_ReleaseMoveOut
// ------------------------------------------------------------------------------------------
// Description :
//	Disable the Touch Release at touch move Out. That means the UI is going to stop to release the
//	Touch when it move out of the UI Shape! UI is going to keep It !
//	By default, this option is deactivated for all UI !
//	User can change that but only if UI is not listening ( that means without any bound listener )
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NDisableUIListenTouch_ReleaseMoveOut(NUI *pui)
{
	// To be changed, an UI does not listening !
	NErrorIf( pui->pTouchUIListener, NERROR_UNAUTHORIZED_ACTION );
	
	FLAG_OFF(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT);
	
	// Post checks ( Check all the inconsistent flags compositions )
	// Now, these tests are made at the beginning of the function "NDispatchEvent".
/*
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
*/
}
// ------------------------------------------------------------------------------------------
// void NEnableUIListenTouch_TrackMoveInOut
// ------------------------------------------------------------------------------------------
// Description :
//	Enable the Move In Move Out Tracking. That means the UI is going to 'simulate' the release of the
//	Touch when it move out of the UI Shape ! BUT, the touch stills captured by the UI !!!
//	By default, this option is deactivated for all UI !
//	User can change that but only if UI is not listening ( that means without any bound listener )
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NEnableUIListenTouch_TrackMoveInOut(NUI* pui)
{
	// To be changed, an UI does not listening !
	NErrorIf( pui->pTouchUIListener, NERROR_UNAUTHORIZED_ACTION );

	// Touch Move Out is not suppose to be set to ON with Track Move In Out Behavior
	// It will not create a problem because those 2 flags are going to be used in 2 different process, 
	// but it's better to Warn user about this inconsistency. 
	// 'FLAG_NUI_UNDER_TOUCH_CONTROL' is used by UI that keep the touch from START/MOVE IN to END/CANCEL.
	//
	NErrorIf(ISFLAG_ON(pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT), NERROR_INCONSISTENT_FLAGS);
	
	FLAG_ON(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT);
	
	// Post checks ( Check all the inconsistent flags compositions )
	// Now, these tests are made at the beginning of the function "NDispatchEvent".
/*
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
*/
}
// ------------------------------------------------------------------------------------------
// void NDisableUIListenTouch_TrackMoveInOut
// ------------------------------------------------------------------------------------------
// Description :
//	Disable the Track Move In Out Behavior. That means the UI is going to STOP 'simulating' the release of the
//	Touch when it move out of the UI Shape !
//	By default, this option is deactivated for all UI !
//	User can change that but only if UI is not listening ( that means without any bound listener )
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NDisableUIListenTouch_TrackMoveInOut(NUI* pui)
{
	// To be changed, an UI does not listening !
	NErrorIf( pui->pTouchUIListener, NERROR_UNAUTHORIZED_ACTION );

	FLAG_OFF(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT);
	
	// Post checks ( Check all the inconsistent flags compositions )
	// Now, these tests are made at the beginning of the function "NDispatchEvent".
/*
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	NErrorIf( FLAGS_TEST(pui->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
*/
}


// ------------------------------------------------------------------------------------------
// void NEnableUITouchEvent_Interception
// ------------------------------------------------------------------------------------------
// Description :
//  UI is going to receive several different EVENTs.
//  There are two ways receive event: 'LISTEN' and 'INTERCEPT'.
// 		1)'LISTEN':		means an UI accepts to receive events addressed to it. ( == UI is the Event Recipient)
// 		2)'INTERCEPT':	means UI wants to intercept events addressed to its children.
//
//	Possibilities: 
//					1-	LISTEN OFF	INTERCEPT OFF
//					2-	LISTEN ON	INTERCEPT OFF
//					3-	LISTEN ON	INTERCEPT ON
//					!-	LISTEN OFF	INTERCEPT ON is not authorized ! To intercept Event, UI has to listen it too !
//
//	So here, this function Activate the UI property to Intercept Touch Event.
//	Due to the touch event complexity, it's not possible to modify UI touch event listen/intercept properties if UI is in listening.
//
//	Touch Event Interception is a little bit peculiar ...
//	All Touch events may be dispatch from system to application trough the NTouchDispatch_xxx functions.
//	And for the specific case of TOUCH CANCEL (ALL), they can be send directly from application to a specific UI Hierarchy branch...
//	In both cases,Touch events are dispatch from the UI hierarchy Root to the leaf. At each UI, touch event are filtered regarding 
//	the BBOX rectangle of UI for NTOUCH_START or regarding the UI Listener Touch Sequence for the other NTOUCH_EVENT.
//	... so by this way some touches are going to be ignored (the ones outside the BBOX or not referenced into the listener), 
//	and so on, children after children until process reach the terminated leaf ... AND then, these leaf(s) are going to receive
//	Touch Event ( if they Listen ).
//	So, Touch event "pass trough" UIs wit hout sending them events... excepted if UI is defined as INTERCEPTOR (for touch event)
//	In that case, just after Touches filtering against UI BBox or listened touch list of listener, UI will receive associated touch event
//	even if UI is not the terminated LEAF.
//	! AND, due to the dispatch process order, the Event Recipient is going to be the UI itself ( terminated Leaf(s) are not yet identified ).
//	!!! ... And that's THE DIFFERENCE between Touch Event intercepting and other event type intercepting... !!!
//
//	TO SUM UP:
//		for touch event type, Event Recipient is always the UI that receive the event, in interception and in standard listening !
//		for all other event types, Recipient is identified before sending event, so is always the targeted UI,...
//		... so, it's never the UI Interceptor itself !
//	
//	At each stage of the dispatch, if an UI return "NUI_EVENT_RESULT_PROCESSED" it will interrupt the dispatch !
//	An UI should return "NUI_EVENT_RESULT_DISPATCH" to let the dispatch continue...
//	!!! For TOUCH EVENT ONLY !!! there is a particular case: NTOUCH_ CANCEL oblige user to return "NUI_EVENT_RESULT_DISPATCH" !!!
// ------------------------------------------------------------------------------------------
// In	:
//			NUI *pui		A valid pointer on UI. 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NEnableUITouchEvent_Interception(NUI *pui)
{
	// To Intercept, UI has first to Listen !
	if( NIsUIListenTouchEvent(pui) )
	{
		FLAG_ON( pui->Flags, FLAG_NUI_INTERCEPT_TOUCH_EVENT );
	}
	#ifdef _DEBUG	
	else
	{
		NErrorIf(1,NERROR_INCONSISTENT_FLAGS); // To Intercept, UI has first to Listen !
	}
	#endif
}
// ------------------------------------------------------------------------------------------
// void NDisableUITouchEvent_Interception
// ------------------------------------------------------------------------------------------
// Description :
//	This function Deactivate the UI property to Intercept Touch Event.
//	Due to the touch event complexity, it's not possible to modify UI touch event listen/intercept properties if UI is in listening.
//	for more information have a look to the "NenableUITouchEvent_Interception" description.
// ------------------------------------------------------------------------------------------
// In	:
//			NUI *pui		A valid pointer on UI. 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NDisableUITouchEvent_Interception(NUI *pui)
{
	// To stop Intercepting, UI has first to Listen nothing !
	if( !pui->pTouchUIListener )
	{
		FLAG_OFF( pui->Flags, FLAG_NUI_INTERCEPT_TOUCH_EVENT );
	}
	else if( pui->pTouchUIListener->Listener.LTouchCount == 0 )
	{
		// if LTouchCount is ZERO, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN has to be set to ON, that means NDisableUITouchEvent_Listening is called
		// from NTOUCH_END or NTOUCH_CANCEL ...
		NErrorIf( ISFLAG_OFF( pui->pTouchUIListener->Listener.Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN ),NERROR_INCONSISTENT_PARAMETERS);
		FLAG_OFF( pui->Flags, FLAG_NUI_INTERCEPT_TOUCH_EVENT );
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_TOUCH_LISTENER_HAS_TO_BE_NULL);
	}
	#endif
}

// ------------------------------------------------------------------------------------------
// void NEnableUICoreEvent_Interception
// ------------------------------------------------------------------------------------------
// Description :
//
//	This function Activate the UI property to Intercept UICore Event.
//
//  UI is going to receive several different EVENTs.
//  There are two ways receive event: 'LISTEN' and 'INTERCEPT'.
// 		1)'LISTEN':		means an UI accepts to receive events addressed to it. ( == UI is the Event Recipient)
// 		2)'INTERCEPT':	means UI wants to intercept events addressed to its children.
//
//	Possibilities: 
//					1-	LISTEN OFF	INTERCEPT OFF
//					2-	LISTEN ON	INTERCEPT OFF
//					3-	LISTEN ON	INTERCEPT ON
//					!-	LISTEN OFF	INTERCEPT ON is not authorized ! To intercept Event, UI has to listen it too !
//
//	All Touch events are send directly trough 'NSendUIEvent' from application to a specific UI which is the Event Recipient.
//	The 'NSendUIEvent' Process check if some Recipient Parent wants to intercept the event before their child.
//	If this is the case, they will receive the event in that order: from the top parent to the Recipient.
//	At each stage of the dispatch, if an UI return "NUI_EVENT_RESULT_PROCESSED" it will interrupt the dispatch !
//	An UI should return "NUI_EVENT_RESULT_DISPATCH" to let the dispatch continue...
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NEnableUICoreEvent_Interception(NUI *pui)
{
	// To Intercept, UI has first to Listen !
	if(NIsUIListenUICoreEvent(pui))
	{
		FLAG_ON( pui->Flags, FLAG_NUI_INTERCEPT_UICORE_EVENT );
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_INCONSISTENT_FLAGS);
	}
	#endif
}
// ------------------------------------------------------------------------------------------
// void NDisableUICoreEvent_Interception
// ------------------------------------------------------------------------------------------
// Description :
//
//	This function Deactivate the UI property to Intercept UICore Event.
//	for more information have a look to the "NEnableUICoreEvent_Interception" description.
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NDisableUICoreEvent_Interception(NUI *pui)
{
	FLAG_OFF( pui->Flags, FLAG_NUI_INTERCEPT_UICORE_EVENT );
}
// ------------------------------------------------------------------------------------------
// void NEnableUICommandEvent_Interception
// ------------------------------------------------------------------------------------------
// Description :
//
//	This function Activate the UI property to Intercept UICommand Event.
//	for more information have a look to the "NEnableUICoreEvent_Interception" description.
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NEnableUICommandEvent_Interception(NUI *pui)
{
	// To Intercept, UI has first to Listen !
	if(NIsUIListenUICommandEvent(pui))
	{
		FLAG_ON( pui->Flags, FLAG_NUI_INTERCEPT_UICOMMAND_EVENT );
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_INCONSISTENT_FLAGS);
	}
	#endif
}
// ------------------------------------------------------------------------------------------
// void NDisableUICommandEvent_Interception
// ------------------------------------------------------------------------------------------
// Description :
//
//	This function Deactivate the UI property to Intercept UICommand Event.
//	for more information have a look to the "NEnableUICoreEvent_Interception" description.
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NDisableUICommandEvent_Interception(NUI *pui)
{
	FLAG_OFF( pui->Flags, FLAG_NUI_INTERCEPT_UICOMMAND_EVENT );
}
// ------------------------------------------------------------------------------------------
// void NEnableUITouchEvent_Listening
// ------------------------------------------------------------------------------------------
// Description :
//	This function Activate the UI property to Listen Touch Event. ( This property is activated by default at UI Creation )
//	!!! Don't forget to use 'NSetUITouchCountMax' to define the number of touches UI is able to listen !!!
//	!!! By default this value is ... 0 !!!
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
/*
void NEnableUITouchEvent_Listening(NUI *pui)
{
	FLAG_ON( pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT );
	if( pui->pParent )
	{
		NUpdateUI_FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN_State((NUI*)pui->pParent);
	}
}
*/
// ------------------------------------------------------------------------------------------
// void NDisableUITouchEvent_Listening
// ------------------------------------------------------------------------------------------
// Description :
//	This function Deactivate the UI property to Listen Touch Event.
//	Due to the complexity of touch event listening UI must be free of listened touch and 
//	can not being a Touch Event Interceptor to be able to turn off its Touch Event Listen properties !
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
/*
void NDisableUITouchEvent_Listening(NUI *pui)
{
	// To stop listening, UI has first to Listen nothing AND has not to be set as touch event interceptor!
	if( ISFLAG_OFF( pui->Flags, FLAG_NUI_INTERCEPT_TOUCH_EVENT ) )
	{
		if( !pui->pTouchUIListener ) 
		{
			FLAG_OFF( pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT );
			if( pui->pParent )
			{
				NUpdateUI_FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN_State((NUI*)pui->pParent);
			}
		}
		else if( pui->pTouchUIListener->Listener.LTouchCount == 0 )
		{
			// if LTouchCount is ZERO, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN has to be set to ON, that means NDisableUITouchEvent_Listening is called
			// from NTOUCH_END or NTOUCH_CANCEL ...
			NErrorIf( ISFLAG_OFF( pui->pTouchUIListener->Listener.Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN ),NERROR_INCONSISTENT_PARAMETERS);
			FLAG_OFF( pui->Flags, FLAG_NUI_LISTEN_TOUCH_EVENT );
			if( pui->pParent )
			{
				NUpdateUI_FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN_State((NUI*)pui->pParent);
			}
		}
		#ifdef _DEBUG
		else
		{
			NError(NERROR_INCONSISTENT_PARAMETERS); // TouchUIListener has to BE NULL (or LTouchCOunt=0) and FLAG INTERCEPTION TOUCH Event OFF !!!
		}
		#endif
	}
	#ifdef _DEBUG
	else
	{
		NError(NERROR_INCONSISTENT_PARAMETERS); // TouchUIListener has to BE NULL and FLAG INTERCEPTION TOUCH Event OFF !!!
	}
	#endif
}
*/
// ------------------------------------------------------------------------------------------
// void NEnableUICoreEvent_Listening
// ------------------------------------------------------------------------------------------
// Description :
//	This function Activate the UI property to Listen UICore Event. ( This property is activated by default at UI Creation )
//	An UI need to activate this property to receive UICore Event from system !
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NEnableUICoreEvent_Listening(NUI *pui)
{
	FLAG_ON( pui->Flags, FLAG_NUI_LISTEN_UICORE_EVENT );
}
// ------------------------------------------------------------------------------------------
// void NDisableUICoreEvent_Listening
// ------------------------------------------------------------------------------------------
// Description :
//	This function Deactivate the UI property to Listen UICore Event. ( This property is activated by default at UI Creation )
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NDisableUICoreEvent_Listening(NUI *pui)
{
	// To stop listening, UI has not to be set as UICORE event interceptor!
	if( ISFLAG_OFF( pui->Flags, FLAG_NUI_INTERCEPT_UICORE_EVENT ) )
	{
		FLAG_OFF( pui->Flags, FLAG_NUI_LISTEN_UICORE_EVENT );
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_INCONSISTENT_PARAMETERS); // TouchUIListener has to BE NULL and FLAG INTERCEPTION TOUCH Event OFF !!!
	}
	#endif
}
// ------------------------------------------------------------------------------------------
// void NEnableUICommandEvent_Listening
// ------------------------------------------------------------------------------------------
// Description :
//	This function Activate the UI property to Listen UICommand Event. ( This property is activated by default at UI Creation )
//	An UI need to activate this property to receive UICommand Event from system !
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NEnableUICommandEvent_Listening(NUI *pui)
{
	FLAG_ON( pui->Flags, FLAG_NUI_LISTEN_UICOMMAND_EVENT );
}
// ------------------------------------------------------------------------------------------
// void NDisableUICommandEvent_Listening
// ------------------------------------------------------------------------------------------
// Description :
//	This function Deactivate the UI property to Listen UICommand Event. ( This property is activated by default at UI Creation )
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NDisableUICommandEvent_Listening(NUI *pui)
{
	// To stop listening, UI has not to be set as UICOMMAND event interceptor!
	if( ISFLAG_OFF( pui->Flags, FLAG_NUI_INTERCEPT_UICOMMAND_EVENT ) )
	{
		FLAG_OFF( pui->Flags, FLAG_NUI_LISTEN_UICOMMAND_EVENT );
	}
	#ifdef _DEBUG
	else
	{
		NErrorIf(1,NERROR_INCONSISTENT_PARAMETERS); // TouchUIListener has to BE NULL and FLAG INTERCEPTION TOUCH Event OFF !!!
	}
	#endif
}



// ------------------------------------------------------------------------------------------
// void NSetOFF_FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN
// ------------------------------------------------------------------------------------------
// Description :
//	PRIVATE Function. The Flag "FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN" helps N engine
//	to choose the best/quicker Touch dispatch algorithms at each turn. It has to be updated each time
//	a UI hierarchical modification occurs. These NUI functions are using it:
//		NSetUIParent_EndInsert()
//		NSetUIParent_BeginingInsert()
//		NEnableUIListenTouch_CaptureMoveIn()
//		NDisableUIListenTouch_CaptureMoveIn()
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetOFF_FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN(NUI *pui)
{
	// Because of propagation process, this function can be called only if GLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN of pui
	// is ON !!!
	NErrorIf(ISFLAG_OFF(pui->Flags,FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN), NERROR_SYSTEM_CHECK);

	FLAG_OFF(pui->Flags,FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN);
	
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// +-+	PROPAGATION ... of 'FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN' set to OFF in the ascendant hierarchy ...
	// +
	// +					This set to OFF potentially engage the set to OFF for 'pui' parent...
	// +					But not necessarily.
	// +					It may only happen if 'pui' is a 'CaptureTouchMoveDummy' ( 'was' a 'CaptureTouchMoveDummy',  to be crystal clear, before the FLAG_OFF ... )
	// +					and if, after the FLAG_OFF, the parent of pui doesn't have any children interested by touch move in.
	// +					... so, in the opposite way, if, at least, one of the children ,of parent of pui, still interested by touch move in,
	// +					... then, nothing change ( because, obviously, the 'FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN' of parent of pui 
	// +					... is set to ON.
	// +					So lets check this in that way to potentially propagate this Set OFF !
	// +
	if( FLAGS_TEST( (pui)->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN,0 ) )
	{
		// Propagation to Pui->Parent
		if(pui->pParent)
		{
			NUI *pparent = (NUI*)pui->pParent;		

//	Nu32 flags = 0;
			NUI *pchild = (NUI*)pparent->ChildrenList.pFirst;
			while( NIS_NODE_VALID(pchild,&pparent->ChildrenList) )
			{
				if( NIsUICaptureTouchMoveIn(pchild) || NIsUICaptureTouchMoveInDummy(pchild) )
				{
					// No need of propagation to parent, because some of its children still interested by touch move in ...
					return;
				}
				pchild = (NUI*)NGET_NEXT_NODE(pchild);
			}

			// If we arrive here that means the previous 'while' loop doesn't find any children interested by touch move in.
			// PROPAGATION is requested ! 'pparent->Flags' as to be set to OFF !
			NSetOFF_FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN(pparent);
		}
		// Propagation to NEngine flags for UI Roots (with no parent)
		else if( NIsUIVisible(pui) ) // No parent + Is Visible ---> Is In ActiveUIRootList !! 
		{
			NErrorIf(ISFLAG_OFF(NEngineCoreFlags,FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN), NERROR_SYSTEM_CHECK);
			NUI ** pptr;
			Nu32 i;

			// --------------------------------------------------
			// Check pui is truly an element of ActiveUIRootList !
			#ifdef _DEBUG
			Nbool bfound = NFALSE;
			pptr = (NUI**)ActiveUIRootsList.pFirst;
			for( i = ActiveUIRootsList.Size; i!=0; i--, pptr ++ )
			{
				if(*pptr == pui)
				{
					bfound = NTRUE;
					break;
				}
			}
			NErrorIf(!bfound, NERROR_SYSTEM_CHECK);
			#endif
				// --------------------------------------------------

			pptr = (NUI**)ActiveUIRootsList.pFirst;
			for( i = ActiveUIRootsList.Size; i!=0; i--, pptr ++ )
			{
				if( NIsUICaptureTouchMoveIn(*pptr) || NIsUICaptureTouchMoveInDummy(*pptr) )
				{
					// Just return !!! Because 'NEngineCore' is already set to ON for FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN !
					// And it still like this because some of UI Roots in the ActiveUIRoot list still interested by touch move in ...
					return;
				}
			}
			// If we arrive here, that means none of the active ui are interested by touch move in ...
			// PROPAGATION is requested ! 'pparent->Flags' as to be set to OFF !
			FLAG_OFF(NEngineCoreFlags,FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN);
		}

	}
}

// ------------------------------------------------------------------------------------------
// void NSetON_FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN
// ------------------------------------------------------------------------------------------
// Description :
//	PRIVATE Function. The Flag "FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN" helps N engine
//	to choose the best/quicker Touch dispatch algorithms at each turn. It has to be updated each time
//	a UI hierarchical modification occurs. These NUI functions are using it:
//		NSetUIParent_EndInsert()
//		NSetUIParent_BeginingInsert()
//		NEnableUIListenTouch_CaptureMoveIn()
//		NDisableUIListenTouch_CaptureMoveIn()
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSetON_FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN(NUI *pui)
{
	// Because of propagation process, this function can be called only if GLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN of pui
	// is OFF !!!
	NErrorIf(ISFLAG_ON(pui->Flags,FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN), NERROR_SYSTEM_CHECK);

	FLAG_ON(pui->Flags,FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN);
	
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// +-+	PROPAGATION ... of 'FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN' set to ON in the ascendant hierarchy ...
	// +
	// +					This set to ON potentially engage the set to ON for 'pui' parent...
	// +					But not necessarily.
	// +					It may only happen if 'pui' IS a 'CaptureTouchMoveDummy'. (NOW, just after the FLAG_ON )
	// +					In that case it's parent will consider it as interested by touch move in, so has to have 'FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN' set to ON.
	// +					if it's not already the case ... PROPAGATION !
	// +
	if( FLAGS_TEST( (pui)->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN,0 ) )
	{
		// Propagation to Pui->Parent
		if(pui->pParent)
		{
			if( ISFLAG_OFF(((NUI*)pui->pParent)->Flags,FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN) )
				NSetON_FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN((NUI*)pui->pParent);
		}
		// Propagation to NEngine flags for UI Roots (with no parent)
		else if( NIsUIVisible(pui) ) // No parent + Is Visible ---> Is In ActiveUIRootList !! 
		{
			#ifdef _DEBUG
			Nbool bfound = NFALSE;
			NUI ** pptr = (NUI**)ActiveUIRootsList.pFirst;
			for( Nu32 i = ActiveUIRootsList.Size; i!=0; i--, pptr ++ )
			{
				if(*pptr == pui)
				{
					bfound = NTRUE;
					break;
				}
			}
			NErrorIf(!bfound, NERROR_SYSTEM_CHECK);
			#endif

			FLAG_ON(NEngineCoreFlags,FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN);
		}
	}
}

// ------------------------------------------------------------------------------------------
// Nbool NAreActiveUIRootsInterestedByTouchMoveIn
// ------------------------------------------------------------------------------------------
// Description :
//	Check all the Active UI Roots from the ActiveUIRootsList.
//	return NTRUE if, at least, one of them has the FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN set to ON
//				 or if, at least, one of them is a "CaptureTouchMoveInDummy" ( no capture for itself but with the FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN set to ON )
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
Nbool NAreActiveUIRootsInterestedByTouchMoveIn()
{
	NUI** pptr = (NUI**)ActiveUIRootsList.pFirst;
	for( Nu32 i = ActiveUIRootsList.Size; i!=0; i--, pptr ++ )
	{
		if( NIsUICaptureTouchMoveIn(*pptr) || NIsUICaptureTouchMoveInDummy(*pptr) )
		{
			return NTRUE;
		}
	}
	return NFALSE;
}

// ------------------------------------------------------------------------------------------
// Nbool NAreUIChildrenInterestedByTouchMoveIn
// ------------------------------------------------------------------------------------------
// Description :
//	Check all the children of an UI.
//	return NTRUE if, at least, one of them has the FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN set to ON
//				 or if, at least, one of them is a "CaptureTouchMoveInDummy" ( no capture for itself but with the FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN set to ON )
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
Nbool NAreUIChildrenInterestedByTouchMoveIn(const NUI* pui)
{
	NUI *pchild = (NUI*)pui->ChildrenList.pFirst;
	while( NIS_NODE_VALID(pchild,&pui->ChildrenList) )
	{
		if( NIsUICaptureTouchMoveIn(pchild) || NIsUICaptureTouchMoveInDummy(pchild))
		{
			return NTRUE;
		}
		pchild = (NUI*)NGET_NEXT_NODE(pchild);
	}
	return NFALSE;	
}




/*
inline void FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN_ToggleToON_UpdateHierarchy(NUI* pui)
{
	NErrorIf( ISFLAG_OFF(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN), NERROR_INCONSISTENT_FLAGS );
	if(pui->pParent)
	{
		FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN_set_ON_recursive((NUI*)pui->pParent);
	}
	else if( NIsUIVisible(pui) ) // No parent + Is Visible ---> Is In ActiveUIRootList !!
	{
#ifdef _DEBUG
		Nbool bfound = NFALSE;
		NUI ** pptr = (NUI**)ActiveUIRootsList.pFirst;
		for( Nu32 i = ActiveUIRootsList.Size; i!=0; i--, pptr ++ )
		{
			if(*pptr == pui)
			{
				bfound = NTRUE;
				break;
			}
		}
		NErrorIf(!bfound, NERROR_SYSTEM_CHECK)
#endif

			FLAG_ON(NEngineCoreFlags,FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN);
	}
}
inline void FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN_ToggleToOFF_UpdateHierarchy(NUI* pui)
{
	NErrorIf( ISFLAG_ON(pui->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN), NERROR_INCONSISTENT_FLAGS );

	if(pui->pParent)
	{
		NUI *pparent = (NUI*)pui->pParent;		
		// Assume pui->Flags was Toggled to OFF just before this function call, so its parent must be flaged accordingly !!!
		NErrorIf(ISFLAG_OFF(pparent->Flags,FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN), NERROR_SYSTEM_CHECK);

		Nu32 flags = 0;
		NUI *pchild = (NUI*)pparent->ChildrenList.pFirst;
		while( NIS_NODE_VALID(pchild,&pparent->ChildrenList) )
		{
			if( !FLAGS_TEST(pchild->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN,0) )
			{
				// Just return !!! Because 'pparent->Flags' already set to ON for FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN !
				// And it still like this, even if pui 'FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN' is changed to OFF ...
				return;
			}
			pchild = (NUI*)NGET_NEXT_NODE(pchild);
		}

		// If we arrive here that means 'pparent->Flags' as to be set to OFF !
		FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN_set_OFF_recursive(pparent);
	}
	else if( NIsUIVisible(pui) ) // No parent + Is Visible ---> Is In ActiveUIRootList !! 
	{
		NUI ** pptr;
		Nu32 i;

		// --------------------------------------------------
		// Check pui is truly an element of ActiveUIRootList !
#ifdef _DEBUG
		Nbool bfound = NFALSE;
		pptr = (NUI**)ActiveUIRootsList.pFirst;
		for( i = ActiveUIRootsList.Size; i!=0; i--, pptr ++ )
		{
			if(*pptr == pui)
			{
				bfound = NTRUE;
				break;
			}
		}
		NErrorIf(!bfound, NERROR_SYSTEM_CHECK)
#endif
			// --------------------------------------------------

			pptr = (NUI**)ActiveUIRootsList.pFirst;
		for( i = ActiveUIRootsList.Size; i!=0; i--, pptr ++ )
		{
			if( !FLAGS_TEST((*pptr)->Flags,FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN,0) )
			{
				// Just return !!! Because 'NEngineCore' already set to ON for FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN !
				// And it still like this even if pui 'FLAG_NUI_HIERARCHY_CAPTURE_TOUCH_MOVE_IN' is changed to OFF ...
				return;
			}
		}
		// If we arrive here, that means none of the active ui have at least one of the two flags set to ON ...
		FLAG_OFF(NEngineCoreFlags,FLAG_NENGINE_CORE_HIERARCHY_CAPTURE_TOUCH_MOVE_IN);
	}
}
*/
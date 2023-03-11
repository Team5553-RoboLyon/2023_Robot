#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "lib/N/NCStandard.h"
#include "lib/N/Geometry/Components/NGeometryAppearance.h"

#include "NUT_Editor_ATxC_Gizmo_Drag_InLine.h"
#include "NUT_Editor_ATxC_Gizmo_Texture_InLine.h"

#include "NUT_Editor_ATxC_Tool.h"

extern NVERTEX_SPECS			NVertexSpecs[];
extern NGEOMETRY_SPECS			NGeometrySpecs[];
extern NAPPEARANCE_SPECS		NAppearanceSpecs[];

Nu32 NUT_ATxC_PickToolDataGizmoArcSelector(const NUT_EDITOR_ATXC_TOOLDATA *ptooldata, const NVEC2 *pnorm_mouse)
{

	NVEC2	v0,vm,vex;
	Nf32		l,halfthickness;

	// Distance mouse from texture ring center:
	NUT_ATxC_GetDrag_HandleCenter( &v0, &ptooldata->HUDGizmo );
	NVec2Sub(&vm,pnorm_mouse,&v0);
	l = NVec2Length(&vm);

	// Compare this distance with the texture circle radius:
	NUT_ATxC_GetArcSelector_Extents(&vex, &ptooldata->HUDGizmo );
	halfthickness = NVec2FastLength(&ptooldata->HUDGizmo.HandleSize)*0.5f;

	if( l > vex.x + halfthickness ) // We know vex.x = vex.y, its a circle !
		return NUT_EDITOR_ATXC_ARC_SELECTOR_ID_VOID;
	if( l < vex.x - halfthickness ) // We know vex.x = vex.y, its a circle !
		return NUT_EDITOR_ATXC_ARC_SELECTOR_ID_VOID;

	// From here we have to find which arc the mouse is on ...

	NVec2ScaleBy(&vm,1.0f/l); // l > 0 we are sure, because if we are here we HAVE:  vex.x + halfthickness >= l >= vex.x - halfthickness 

	Nf32 c85= NFastCos(NANGLE90-NANGLE5);
	Nf32 s5 = NFastSin(NANGLE5);
	// GEOMSPEC or BLEND
	if(vm.x<-c85)
	{
		if(vm.y > s5)
		{
			return NUT_EDITOR_ATXC_ARC_SELECTOR_ID_GEOMSPECS;
		}
		else if(vm.y < -s5)
		{
			return NUT_EDITOR_ATXC_ARC_SELECTOR_ID_BLEND;		
		}
		else
		{
			return NUT_EDITOR_ATXC_ARC_SELECTOR_ID_VOID;		
		}
	}
	// TEXTURE UNIT or MATERIAL
	else if(vm.x > c85)
	{
		if(vm.y > s5)
		{
			return NUT_EDITOR_ATXC_ARC_SELECTOR_ID_TEXTUREUNIT;
		}
		else if(vm.y < -s5)
		{
			return NUT_EDITOR_ATXC_ARC_SELECTOR_ID_MATERIAL;		
		}
		else
		{
			return NUT_EDITOR_ATXC_ARC_SELECTOR_ID_VOID;		
		}
	}
	return NUT_EDITOR_ATXC_ARC_SELECTOR_ID_VOID;
}

void NUT_ATxC_ActivateGizmo(NUT_EDITOR_ATXC_TOOLDATA *ptooldata,const NVEC2 *pposition)
{
//	NBLEND		*pblend;
	Nu32		index;
	Nu8			available_tu;
	Nu8			available_uvset;
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + GIZMO  CURRENT ENTITY
	// +
	NErrorIf(!ptooldata->pCurrentEntity,NERROR_NULL_POINTER);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + GIZMO POSITION
	// +
	if( ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_MOVE_TO_PICK_POSITION) )
	{
		if(pposition)
		{
			ptooldata->HUDGizmo.Position.x = pposition->x - ptooldata->HUDGizmo.DragHandleSize.x*0.5f; // Main Gizmo Box X Center at X position 
			ptooldata->HUDGizmo.Position.y = pposition->y - ptooldata->HUDGizmo.DragHandleSize.y*0.5f; // Main Gizmo Box Y Center at Y position
		}
		else
		{
			ptooldata->HUDGizmo.Position.x = ptooldata->HUDGizmo.DefaultPosition.x - ptooldata->HUDGizmo.DragHandleSize.x*0.5f; // Main Gizmo Box X Center at X position 
			ptooldata->HUDGizmo.Position.y = ptooldata->HUDGizmo.DefaultPosition.y - ptooldata->HUDGizmo.DragHandleSize.y*0.5f; // Main Gizmo Box Y Center at Y position
		}	
	}
	//else ... nothing it just stays where it is ...

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + GIZMO GEOMSPECS
	// +
	if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_COMPATIBLE_GEOMETRY_RENDER_FCT) )
	{
		// if (number of compatible Geomspecs != 0)
		// FLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_GEOMSPECS_ON);
		// else
		FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_GEOMSPECS_ON);
	}
	else
	{
		FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_GEOMSPECS_ON);
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + GIZMO BLEND
	// +
	if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_BLEND) )
	{
		if( NCHECK_APPEARANCE_SPECS_BLEND(NAppearanceSpecs[NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(ptooldata->Elements.GeometryIDS)].Flags ) )
			FLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_BLEND_ON);
		else
			FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_BLEND_ON);
	}
	else
	{
		FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_BLEND_ON);
	}
	// ... Yes ! Blend ! So ... what about the Selector Handle ...
	if(	ISFLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_BLEND_ON) )
	{
		index = NUT_ATxC_GetBlendIndex(ptooldata,ptooldata->Elements.pBlend);
		if(index != NVOID)
		{
			NUT_ATxC_SetBlendHandlePosition(ptooldata,index);
		}
		else
		{
			//			Do something like that for this case ...
			// 			ptooldata->HUDGizmo.TextureHandlePosition.x = -2.0f; // to be out screen !!! UGLY
			// 			ptooldata->HUDGizmo.TextureHandlePosition.y = -2.0f; // to be out screen !!! UGLY		
			NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION);
		}
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + GIZMO MATERIAL
	// +
	if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_MATERIAL) )
	{
		if( NCHECK_APPEARANCE_SPECS_MATERIAL(NAppearanceSpecs[NGET_GEOMETRY_IDS_APPEARANCE_FORMAT(ptooldata->Elements.GeometryIDS)].Flags ) )
			FLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_MATERIAL_ON);
		else
			FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_MATERIAL_ON);
	}
	else
	{
		FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_MATERIAL_ON);
	}

	// ... Yes ! Material ! So ... what about the Selector Handle ...
	if(	ISFLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_MATERIAL_ON) )
	{
		NErrorIf(1,NERROR_SYSTEM_TODO);
		// The lines bellow are the ones but some functions need to be finalized/done: NUT_ATxC_GetMaterialIndex,NUT_ATxC_SetMaterialHandlePosition ...
/*
		index = NUT_ATxC_GetMaterialIndex(ptooldata,ptooldata->Elements.pMaterial);
		if(index != NVOID)
		{
			NUT_ATxC_SetMaterialHandlePosition(ptooldata,index);
		}
		else
		{
			//			Do something like that for this case ...
			// 			ptooldata->HUDGizmo.TextureHandlePosition.x = -2.0f; // to be out screen !!! UGLY
			// 			ptooldata->HUDGizmo.TextureHandlePosition.y = -2.0f; // to be out screen !!! UGLY		
			NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION);
		}
*/
	}
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + GIZMO ASPECT
	// +
	// +	+---+ TEXTUREUNIT ?
	// +
	if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXTURE) ) 
	{																					  

		// There is no user flag 'FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXTUREUNIT'
		// user just tells to edit TEXTURE and FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTUREUNIT_ON is set to ON or OFF
		// accordingly with the Edited Geometry FORMAT which could have ZERO, ONE or MULTIPLE Texture Units.
		// To be Cristal clear: 
		// FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTUREUNIT_ON is set to ON if Geometry contains 1 or more Texture Unit.
		// And is set to OFF if Geometry has NO Texture Unit.
		available_tu = NGeometrySpecs[NGET_GEOMETRY_IDS_GEOMETRY_FORMAT(ptooldata->Elements.GeometryIDS) ].UsedTextureUnitNumber;
		if( !available_tu )
		{
			FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTUREUNIT_ON);
		}
		else
		{
			FLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTUREUNIT_ON);
			ptooldata->HUDGizmo.EditedTextureUnit = NMIN(ptooldata->HUDGizmo.EditedTextureUnit,available_tu-1);// available_tu >=1 for sure due to the first test ...
			NUT_ATxC_SetTextureUnitHandlePosition(ptooldata);
		}
	}
	else
	{
		FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTUREUNIT_ON);
		ptooldata->HUDGizmo.EditedTextureUnit = 0;
	}
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + GIZMO ASPECT
	// +
	// +	+---+ TEXTURES ?
	// +
	NUT_ATxC_TextureRingActivation(ptooldata);
	// +
	// +	+---+ TEXTURES PARAMETERS ?
	// +
	NUT_ATxC_TextureParametersActivation(ptooldata);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + GIZMO ASPECT
	// +
	// +	+---+ TEXCOORD ?
	// +			|
	// +			+ --- TxCSET
	// +			+ --- TxCOFFSET
	// +			+ --- TxCSCALE
	// +			+ --- TxCRESET (reset OFFSET,SCALE)
	// +
	// +		It could be strange but for ATxC tools, Texture Coordinates and Texture are not really 'connected".
	// +		I mean, it's possible to Edit TxC Offset and Scale without having a texture. 
	// +
	available_uvset = NGET_VERTEX_SPECS_TEXCOORD_NUMBER(NVertexSpecs[ NGET_GEOMETRY_IDS_VERTEX_FORMAT(ptooldata->Elements.GeometryIDS) ].DirectAccess.Flags);
	if(! available_uvset)
	{
		// No UVSet...  All UV Flags to OFF
		FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_ON);
		FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_OFFSET_ON);
		FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_SCALE_ON);
		FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_RESET_ON);
// 		FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_SINGLE_ON);
// 		FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_SINGLE_ON_AUTO);
	}
	else
	{
		if(available_uvset == 1)
		{
			FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_ON); // Only one UVSet !!!
		}
		else // = 2 !
		{
			if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXCOORD_SET) )
			{
				FLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_ON);
			}
			else
			{
				FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_ON);
			}
		}
		
		if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXCOORD_OFFSET) )
			FLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_OFFSET_ON);
		else
			FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_OFFSET_ON);

		if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXCOORD_SCALE) )
			FLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_SCALE_ON);
		else
			FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_SCALE_ON);

		if( ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXCOORD_RESET_OFFSET)||
			ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXCOORD_RESET_SCALE) )
			FLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_RESET_ON);
		else
			FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_RESET_ON);
	}

	// Reset STATE to VOID ...
	ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_VOID;
	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_GIZMO );
	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_GEOMSPECS );
	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTUREUNIT );
	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTURE );
	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORDSET );
	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_OFFSET );
	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_SCALE );
	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_RESET );
// 	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_SINGLE );
	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_BLEND );
	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_MATERIAL );

	// ... AND ACTIVATE !
	FLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_ACTIVE);
}

void NUT_ATxC_UnactivateGizmo(NUT_EDITOR_ATXC_TOOLDATA *ptooldata)
{

	FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_ACTIVE);
	FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_GEOMSPECS_ON);
	FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTUREUNIT_ON);
	FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTURES_ON);
	FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_ON);
	FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_OFFSET_ON);
	FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_SCALE_ON);
	FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_RESET_ON);
// 	FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_SINGLE_ON);
// 	FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_SINGLE_ON_AUTO);
	FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_BLEND_ON);
	FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_MATERIAL_ON);

	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_GIZMO );
	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_GEOMSPECS );
	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTUREUNIT );
	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTURE );
	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORDSET );
	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_OFFSET );
	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_SCALE );
	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_RESET );
// 	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_SINGLE );
	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_BLEND );
	FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_MATERIAL );
	
	ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_VOID;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR

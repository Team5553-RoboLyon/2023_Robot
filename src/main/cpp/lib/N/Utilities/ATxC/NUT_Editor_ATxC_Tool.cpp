#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "lib/N/NCStandard.h"
#include "lib/N/GL/Ngl.h"
#include "lib/N/NRectf32.h"
#include "lib/N/Containers/NArray.h"
#include "../../NViewport.h"
#include "lib/N/Miscellaneous/N2DPointInside.h"
#include "../../NCoordinateSystemConversions.h"
#include "lib/N/Geometry/Components/NGeometryVertex.h"
#include "../../Event/NEventTouch.h"
#include "../NUT_3DMouse.h"
#include "../NUT_Pick.h"

#include "lib/N/Utilities/Draw/NUT_Draw.h"
#include "NUT_Editor_ATxC_Elements.h"
#include "NUT_Editor_ATxC_Gizmo.h"
#include "NUT_Editor_ATxC_Entity.h"
#include "NUT_Editor_ATxC_Gizmo_BLend_InLine.h"
#include "NUT_Editor_ATxC_Gizmo_Material_InLine.h"
#include "NUT_Editor_ATxC_Gizmo_Drag_InLine.h"
#include "NUT_Editor_ATxC_Gizmo_Texture_InLine.h"
#include "NUT_Editor_ATxC_Gizmo_TexCoord_InLine.h"
#include "NUT_Editor_ATxC_Gizmo_GeometryFormat_InLine.h"
#include "NUT_Editor_ATxC_Gizmo_TextureUnit_InLine.h"
#include "NUT_Editor_ATxC_Gizmo_Text_InLine.h"
#include "NUT_Editor_ATxC_Gizmo_TextureWrap_InLine.h"
#include "NUT_Editor_ATxC_Gizmo_TextureFilter_InLine.h"

#include "NUT_Editor_ATxC_Tool.h"



static inline void _all_gizmo_updating_flags_off(NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	FLAG_OFF(pgizmo->Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_GIZMO );
	FLAG_OFF(pgizmo->Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_GEOMSPECS );
	FLAG_OFF(pgizmo->Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTUREUNIT );
	FLAG_OFF(pgizmo->Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTURE );
	FLAG_OFF(pgizmo->Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTURE_WRAP );
	FLAG_OFF(pgizmo->Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTURE_FILTER );
	FLAG_OFF(pgizmo->Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORDSET );
	FLAG_OFF(pgizmo->Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_OFFSET );
	FLAG_OFF(pgizmo->Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_SCALE );
	FLAG_OFF(pgizmo->Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_RESET );
// 	FLAG_OFF(pgizmo->Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_SINGLE );
	FLAG_OFF(pgizmo->Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_BLEND );
	FLAG_OFF(pgizmo->Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_MATERIAL );
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + INLINE GIZMO PICKING and ACTIVATION
// +
//
static inline Nbool _look_for_hudgizmo_element(NUT_EDITOR_ATXC_TOOLDATA *ptooldata, const NVEC2 *pnorm_mouse)
{
	NRECTf32				rect;
//	NUT_EDITOR_ATXC_GIZMO	*pgizmo = &ptooldata->HUDGizmo;


	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + 1/ Look for GIZMO UV ORIGIN
	// +
	if( ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_OFFSET_ON) )
	{
		NUT_ATxC_GetTxCOffset_HandleRect( &rect,&ptooldata->Elements, ptooldata->pCurrentEntity, &ptooldata->HUDGizmo );
		if( N2DPointInside2DRectangle(&rect,pnorm_mouse) )
		{
			ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_OFFSET;
			FLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_OFFSET );
			return NTRUE;
		}
	}
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + 2/ Look for GIZMO TxC Scale 
	// +
	if( ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_SCALE_ON) )
	{
		//  U Arrowhead
		NUT_ATxC_GetTxCScale_UArrowHeadRect(&rect,&ptooldata->Elements,ptooldata->pCurrentEntity,&ptooldata->HUDGizmo); // The rect with the arrow inside !
		if( N2DPointInside2DRectangle(&rect,pnorm_mouse) )
		{
			// Choose the right Mode: Absolute or Speed ?
			if( NABS(ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID].x) > ptooldata->pCurrentEntity->EditingTxCScale_SpeedSwitchThreshold.x )
			{
				ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SCALE_USPEED;
				ptooldata->HUDGizmo.TexCoordScaleSpeed.x = 0.0f;
			}
			else
			{
				ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SCALE_U;
			}
			FLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_SCALE );
			return NTRUE;
		}

		// V Arrowhead
		NUT_ATxC_GetTxcScale_VArrowHeadRect(&rect,&ptooldata->Elements,ptooldata->pCurrentEntity,&ptooldata->HUDGizmo);
		if( N2DPointInside2DRectangle(&rect,pnorm_mouse) )
		{
			// Choose the right Mode Absolute or Speed ?
			// if "on air UVScale.y" > "max on air UVOrthonrmal Size " ( both in term of MainSize UNIT)
			if( NABS(ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID].y) > ptooldata->pCurrentEntity->EditingTxCScale_SpeedSwitchThreshold.y )
			{
				ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SCALE_VSPEED;
				ptooldata->HUDGizmo.TexCoordScaleSpeed.y = 0.0f;
			}
			else
			{
				ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SCALE_V;
			}

			FLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_SCALE );
			return NTRUE;
		}
	}
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + 3/ Look for GIZMO TxC RESET
	// +
	if( ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_RESET_ON) )
	{
		NUT_ATxC_GetTxCReset_UHandleRect(&rect,&ptooldata->Elements, ptooldata->pCurrentEntity,&ptooldata->HUDGizmo);
		if( N2DPointInside2DRectangle(&rect,pnorm_mouse) )
		{
			ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_RESET_U;
			FLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_RESET );
			return NTRUE;
		}
		NUT_ATxC_GetTxCReset_VHandleRect(&rect,&ptooldata->Elements, ptooldata->pCurrentEntity,&ptooldata->HUDGizmo);
		if( N2DPointInside2DRectangle(&rect,pnorm_mouse) )
		{
			ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_RESET_V;
			FLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_RESET );
			return NTRUE;
		}
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + 
	// + 4/ Look for GIZMO TEXTURE
	// +
	if( ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTURES_ON) )
	{
		if( NUT_ATxC_PickTextureRing(ptooldata,pnorm_mouse) )
		{
			ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_TEXTURE;
			FLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTURE );
			return NTRUE;
		}
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// +  5/ Look for GIZMO (ARCS) BLEND/MATERIAL/TEXTUREUNIT/GEOMSPECS
	// + 
	if( ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_BLEND_ON|FLAG_NUT_EDITOR_ATXC_GIZMO_MATERIAL_ON|
											FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTUREUNIT_ON|FLAG_NUT_EDITOR_ATXC_GIZMO_GEOMSPECS_ON ) )
	{
		Nu32 arc = NUT_ATxC_PickToolDataGizmoArcSelector(ptooldata,pnorm_mouse);
		switch(arc)
		{
			case NUT_EDITOR_ATXC_ARC_SELECTOR_ID_TEXTUREUNIT:
					if( ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTUREUNIT_ON) )
					{
						ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_TEXTUREUNIT;
						FLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTUREUNIT );
						return NTRUE;
					}
					break;

			case NUT_EDITOR_ATXC_ARC_SELECTOR_ID_GEOMSPECS:
				if( ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_GEOMSPECS_ON) )
				{
					ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_GEOMSPECS;
					FLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_GEOMSPECS );
					return NTRUE;
				}
				break;

			case NUT_EDITOR_ATXC_ARC_SELECTOR_ID_BLEND:
				if( ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_BLEND_ON) )
				{
					ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_BLEND;
					FLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_BLEND );
					return NTRUE;
				}
				break;

			case NUT_EDITOR_ATXC_ARC_SELECTOR_ID_MATERIAL:
				if( ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_MATERIAL_ON) )
				{
					ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_MATERIAL;
					FLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_MATERIAL );
					return NTRUE;
				}
				break;

			case NUT_EDITOR_ATXC_ARC_SELECTOR_ID_VOID:
				// Nothing to do !
				printf("\nNUT_EDITOR_ATXC_GIZMO_ARC_SELECTOR_VOID");
				break;

			default:
				NErrorIf(1,NERROR_UNAUTHORIZED_CASE);
				break;
		}
	}
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + TEXTURE WRAP
	// +
	// 6)Look for Texture WRAP_S and WRAP_T
	if( ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTURE_WRAP_ON) )
	{
		NUT_ATxC_GetWrap_S_HandleRect(&rect,&ptooldata->HUDGizmo);
		if( N2DPointInside2DRectangle(&rect,pnorm_mouse) )
		{
			ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_TEXTURE_WRAP_S;
			FLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTURE_WRAP );
			return NTRUE;
		}
		NUT_ATxC_GetWrap_T_HandleRect(&rect,&ptooldata->HUDGizmo);
		if( N2DPointInside2DRectangle(&rect,pnorm_mouse) )
		{
			ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_TEXTURE_WRAP_T;
			FLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTURE_WRAP );
			return NTRUE;
		}
	}
	// 7)Look for Texture MIN_FILTER and MAG_FILTER
	if( ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTURE_FILTER_ON) )
	{
		NUT_ATxC_GetMinFilter_HandleRect(&rect, &ptooldata->HUDGizmo);
		if( N2DPointInside2DRectangle(&rect,pnorm_mouse) )
		{
			ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_TEXTURE_MIN_FILTER;
			FLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTURE_FILTER );
			return NTRUE;
		}
		NUT_ATxC_GetMagFilter_HandleRect(&rect, &ptooldata->HUDGizmo);
		if( N2DPointInside2DRectangle(&rect,pnorm_mouse) )
		{
			ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_TEXTURE_MAG_FILTER;
			FLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTURE_FILTER );
			return NTRUE;
		}
	}


	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + GIZMO
	// +

	// 7)Look for Gizmo Itself
	NUT_ATxC_GetDrag_HandleRect(&rect,&ptooldata->HUDGizmo);
	if( N2DPointInside2DRectangle(&rect,pnorm_mouse) )
	{
		ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_GIZMO;
		FLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_GIZMO );
		return NTRUE;
	}


	return NFALSE;
}

/*
static inline Nu32 _look_for_singleUV( NUT_EDITOR_ATXC_TOOLDATA *ptooldata, const NVEC2s16 *ptouch_pos )
{
	Nf32 fmin = NF32_MAX;
	Nf32 fdist;

	ptooldata->HUDGizmo.SingleTexCoord_EditedVertexIndex		= NVOID;
	ptooldata->HUDGizmo.SingleTexCoord_pEditedVertexPosition	= NULL;

	if( ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_SINGLE_ON) )
	{
		Nf32 picksquareradius = NVec2SquareLength(&ptooldata->HUDGizmo.SingleTexCoordExtents);
		Nu32 stride = (Nu32)NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(ptooldata->Elements.GeometryIDS))->SizeOfVertex;
		Nu32 uvstride = (Nu32)NGetVertexSpecs(NGET_GEOMETRY_IDS_VERTEX_FORMAT(ptooldata->Elements.GeometryIDS))->Offset[NVERTEX_DATA_TEXCOORD0+ptooldata->HUDGizmo.EditedTexCoordSetID];
		NVEC3 rayorigin,rayvdir;
		NUT_Get3DMouseRay(&rayorigin,&rayvdir,NULL,ptouch_pos);
		NVEC3 *pv = (NVEC3*)ptooldata->Elements.pFirstVertex;
		for( Nu32 i=0; i<ptooldata->Elements.VertexNumber;i++,pv = (NVEC3*)((NBYTE*)pv+stride) )
		{
			if( fdist = NUT_Pick3DPointEx_SquareDist(&rayorigin,&rayvdir,NMIN(picksquareradius,fmin),pv) >=0.0f )
			{
				fmin	= fdist;
				ptooldata->HUDGizmo.SingleTexCoord_EditedVertexIndex		=  i;
				ptooldata->HUDGizmo.SingleTexCoord_pEditedVertexPosition	= pv;
				ptooldata->HUDGizmo.SingleTexCoord_pEditedVertexTexCoord		= (NTEXCOORD_2f*)((NBYTE*)pv+uvstride);
				
				ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SINGLE;
				FLAG_ON(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_SINGLE );
			}
		}
	}
	return ptooldata->HUDGizmo.SingleTexCoord_EditedVertexIndex;
}
*/
// ------------------------------------------------------------------------------------------
// NUT_Editor_ATxC_Tool_Start
// ------------------------------------------------------------------------------------------
// Description :
//				This Tool manages texture selection 
// ------------------------------------------------------------------------------------------
// In  :
//			const NEVENT		*pevent		:	Current event 
//			NEDITOR_SELECTION	*pselect	:	Current NEditor Selection
//			const Nu32			tool_param	:	Here tool_param is a "NUT_TestureAndUV_Modifier_Tool_Data"
//												... linked with the tool at its Registration. 											
// Out :
// ------------------------------------------------------------------------------------------
void  NUT_Editor_ATxC_Tool_Start(const NEDITOR_SELECTION *pselection,const Nu32 tool_param )
{
	NUT_EDITOR_ATXC_TOOLDATA *ptooldata = (NUT_EDITOR_ATXC_TOOLDATA*)tool_param;

	NUT_Editor_ATxC_ClearElements(&ptooldata->Elements);

	if( ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_GET_DEFAULT_ENTITY_ELEMENTS_AT_TOOL_START) )
	{
		NErrorIf(ptooldata->HUDGizmo.DefaultEntityID==NVOID,NERROR_SYSTEM_GURU_MEDITATION);// just to see ... and decide
		if(ptooldata->HUDGizmo.DefaultEntityID!=NVOID)
		{
			ptooldata->pCurrentEntity = (NUT_EDITOR_ATXC_ENTITY*)NGetArrayPtr(&ptooldata->EntityArray,ptooldata->HUDGizmo.DefaultEntityID);	
			if( ptooldata->pCurrentEntity->GetElements_CallBack(&ptooldata->Elements, NULL) )
			{
				NUT_Editor_ATxC_CheckAndStoreIncomingElementsData(ptooldata);
				NUT_ATxC_ActivateGizmo((NUT_EDITOR_ATXC_TOOLDATA*)tool_param,NULL);
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// NUT_Editor_ATxC_Tool_Stop
// ------------------------------------------------------------------------------------------
// Description :
//				This Tool manages texture selection 
// ------------------------------------------------------------------------------------------
// In  :
//			const NEVENT		*pevent		:	Current event 
//			NEDITOR_SELECTION	*pselect	:	Current NEditor Selection
//			const Nu32			tool_param	:	Here tool_param is a "NUT_TestureAndUV_Modifier_Tool_Data"
//												... linked with the tool at its Registration. 											
// Out :
// ------------------------------------------------------------------------------------------
void  NUT_Editor_ATxC_Tool_Stop(const NEDITOR_SELECTION *pselection,const Nu32 tool_param )
{
	NUT_EDITOR_ATXC_TOOLDATA *ptooldata	= (NUT_EDITOR_ATXC_TOOLDATA*)tool_param;

	NUT_ATxC_UnactivateGizmo(ptooldata);
	ptooldata->pCurrentEntity = NULL; // ??? yes ?
}

inline void _strcat_wrapname(Nchar *pstr,Ns32 wrapid)
{
	if(wrapid<0)
		wrapid += 4;

	Nchar* dicostr[]={"GL_CLAMP_TO_BORDER","GL_CLAMP_TO_EDGE","GL_MIRRORED_REPEAT","GL_REPEAT"};
	strcat(pstr,dicostr[wrapid%4]);
}
inline void _strcat_filtername(Nchar *pstr, Ns32 filterid)
{
	if(filterid<0)
		filterid += 6;

	Nchar *dicostr[]={"GL_NEAREST","GL_LINEAR","GL_NEAREST_MIPMAP_NEAREST","GL_LINEAR_MIPMAP_NEAREST","GL_NEAREST_MIPMAP_LINEAR","GL_LINEAR_MIPMAP_LINEAR"};
	strcat(pstr,dicostr[filterid%6]);
}
inline GLint  _get_wrapglint(Ns32 wrapid)
{
	if(wrapid<0)
		wrapid += 4;

	GLint dico[]={GL_CLAMP_TO_BORDER,GL_CLAMP_TO_EDGE,GL_MIRRORED_REPEAT,GL_REPEAT};
	return dico[wrapid%4];
}
inline Nu32 _get_wrapid(const GLint wrapglint)
{
	GLint dico[]={GL_CLAMP_TO_BORDER,GL_CLAMP_TO_EDGE,GL_MIRRORED_REPEAT,GL_REPEAT};
	for(Nu32 id=0;id<4;id++)
	{
		if(wrapglint == dico[id])
			return id;
	}
	return GL_CLAMP_TO_BORDER;
}

inline GLint _get_minfilterglint(Ns32 filterid)
{
	if(filterid<0)
		filterid += 6;

	GLint dico[]={GL_NEAREST,GL_LINEAR,GL_NEAREST_MIPMAP_NEAREST,GL_LINEAR_MIPMAP_NEAREST,GL_NEAREST_MIPMAP_LINEAR,GL_LINEAR_MIPMAP_LINEAR};
	return dico[filterid%6];
}

inline GLint _get_magfilterglint(Ns32 filterid)
{
	if(filterid<0)
		filterid += 2;

	GLint dico[]={GL_NEAREST,GL_LINEAR};
	return dico[filterid%2];
}

inline Nu32 _get_filterid(const GLint filterglint)
{
	GLint dico[]={GL_NEAREST,GL_LINEAR,GL_NEAREST_MIPMAP_NEAREST,GL_LINEAR_MIPMAP_NEAREST,GL_NEAREST_MIPMAP_LINEAR,GL_LINEAR_MIPMAP_LINEAR};
	for(Nu32 id=0;id<6;id++)
	{
		if(filterglint == dico[id])
			return id;
	}
	return GL_NEAREST;
}
// ------------------------------------------------------------------------------------------
// NUT_Editor_ATxC_Tool
// ------------------------------------------------------------------------------------------
// Description :
//				This Tool manages texture selection 
// ------------------------------------------------------------------------------------------
// In  :
//			const NEVENT		*pevent		:	Current event 
//			NEDITOR_SELECTION	*pselect	:	Current NEditor Selection
//			const Nu32			tool_param	:	Here tool_param is a "NUT_TestureAndUV_Modifier_Tool_Data"
//												... linked with the tool at its Registration. 											
// Out :
// ------------------------------------------------------------------------------------------
NEDITOR_RESULT	NUT_Editor_ATxC_Tool(const NEVENT *pevent,NEDITOR_SELECTION *pselect,const Nu32 tool_param)
{
	//NEVENT_TOUCH					*ptouch;
	NVEC2s16						touch_position;
	NUT_EDITOR_ATXC_TOOLDATA		*ptooldata;
	NUT_EDITOR_ATXC_ENTITY			*ppu;
	NUT_EDITOR_ATXC_UPDATA			updata;
	Nu32							i;
	NVEC2						norm_mouse,scaled_mousedelta,v0,v1;
	Nf32							angle,f;
	Nu32							sectornb;
//	Nu32							texture_index;
//	Nf32							sectorsize;
	Nu32							index;
	NEDITOR_SELECTION				tmp_select;
	NUT_EDITOR_ATXC_ELEMENTS		tmp_elements;
	Nu8								metatexturetype,currentmetatexturetype;
	Ns32							s32;
	void							*pmetatexture,*pcurrentmetatexture;
	

	switch(pevent->Type)
	{
		// THEY ARE 2 MAIN POSSIBILITIES
		//		1/	User "click" onto the "Tool Gizmo"
		//		2/	User doesn't "click" onto the "Tool Gizmo", so we assume he tries to pick a new object to modify
		//			or update its texture and uv.
		case NTOUCH_START:
			ptooldata	= (NUT_EDITOR_ATXC_TOOLDATA*)tool_param;
			FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_MOVING);
			FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_X_OFF);
			FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_Y_OFF);

			NGetEventTouchPosition(&touch_position,pevent,0);
			

			// 0)Get Normalized Mouse
			//			ptouch			= (NEVENT_TOUCH*)pevent;
			// 			norm_mouse.x	= NCoord_PixToHRel_X( ptouch->Position.x );
			// 			norm_mouse.y	= NCoord_PixToHRel_Y(-ptouch->Position.y );
			NCoord_PixToHRel( &norm_mouse, &touch_position );
			
			ptooldata->Normalized_TouchStart = ptooldata->Normalized_PreviousTouch = norm_mouse;

			if( _look_for_hudgizmo_element(ptooldata, &norm_mouse) )
			{
				return NEDITOR_RESULT_TOOL_ON;
			}
// 			else if( _look_for_singleUV(ptooldata,&ptouch->Position)!=NVOID )
// 			{
// 				return NEDITOR_RESULT_TOOL_ON;
// 			}
			else
			{	
				//NUT_ATxC_UnactivateGizmo(ptooldata);
				//NUT_Editor_ATxC_ClearElements(&ptooldata->Elements);
				//NResetToolSelection(pselect);
				
				// No Gizmo Element selected
 				ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_VOID;
 				_all_gizmo_updating_flags_off(&ptooldata->HUDGizmo);
				//NUT_ATxC_UnactivateGizmo(ptooldata);
				
				ppu = (NUT_EDITOR_ATXC_ENTITY*)NGetFirstArrayPtr(&ptooldata->EntityArray);
				for(i=NGetArraySize(&ptooldata->EntityArray);i!=0;i--,ppu++)
				{
					NResetToolSelection(&tmp_select);
					if( ppu->Pick_CallBack( &tmp_select,&touch_position, ppu->pEntry) )
					{
						NUT_Editor_ATxC_ClearElements(&tmp_elements);
						if( ppu->GetElements_CallBack(&tmp_elements,&tmp_select) )
						{
							NUT_ATxC_UnactivateGizmo(ptooldata);
							NUT_Editor_ATxC_ClearElements(&ptooldata->Elements);
							NResetToolSelection(pselect);
							ptooldata->Elements				= tmp_elements;
							*pselect						= tmp_select;
							ptooldata->Normalized_PreviousTouch = norm_mouse;
							ptooldata->pCurrentEntity		= ppu;
							
							NUT_Editor_ATxC_CheckAndStoreIncomingElementsData(ptooldata);
							NUT_ATxC_ActivateGizmo(ptooldata,&norm_mouse);
							return NEDITOR_RESULT_TOOL_ON;
						}
					}
				}
				// If we arrive here that means Entity selection fails !
				// If possible, try getting default entity Elements without selection ... if user allows it, of course...
				if( ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_KEEP_SELECTION) )
				{
					if(ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_ACTIVE) )
					{
						// Just keep already selected things like they are ...
						// and we are sure there are some things because GIZMO is ACTIVE !
						return NEDITOR_RESULT_TOOL_ON;
					}
				}
				else
				{
					NUT_ATxC_UnactivateGizmo(ptooldata);
					NUT_Editor_ATxC_ClearElements(&ptooldata->Elements);
					NResetToolSelection(pselect);
				}
			}
			break;

		case NTOUCH_MOVE:
			ptooldata = (NUT_EDITOR_ATXC_TOOLDATA*)tool_param;

// 			ptouch=(NEVENT_TOUCH*)pevent;
// 			norm_mouse.x = NCoord_PixToHRel_X( ptouch->Position.x );
// 			norm_mouse.y = NCoord_PixToHRel_Y(-ptouch->Position.y );
			NCoord_PixToHRel( &norm_mouse, NGetEventTouchPositionPtr(pevent,0) );
		
			switch(ptooldata->HUDGizmo.State)
			{
				case NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_OFFSET:
					f = ptooldata->pCurrentEntity->EditingTxCOffset_Max - ptooldata->pCurrentEntity->EditingTxCOffset_Min;
					scaled_mousedelta.x =  (norm_mouse.x - ptooldata->Normalized_PreviousTouch.x)*f*ptooldata->pCurrentEntity->EditingTxCOffset_TouchScale.x/ptooldata->HUDGizmo.DragHandleSize.x;
					scaled_mousedelta.y = -(norm_mouse.y - ptooldata->Normalized_PreviousTouch.y)*f*ptooldata->pCurrentEntity->EditingTxCOffset_TouchScale.y/ptooldata->HUDGizmo.DragHandleSize.y;
					
					// Lock axis system (to move only along X or Y)
					if(ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_MOVE_AXIS_RESTRICTION) )
					{
						// The first time it moves  FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_MOVING is OFF, and after ,ON for all the other TOUCH MOVE
						// until TOUCH_END
						if(ISFLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_MOVING))
						{
							if(scaled_mousedelta.y)
							{
								f = NABS(scaled_mousedelta.x/scaled_mousedelta.y);
								if( f < (1.0f/2.0f) )
									FLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_X_OFF);
								else 
									FLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_Y_OFF);
							}
							else if(scaled_mousedelta.x)
							{
								f = NABS(scaled_mousedelta.y/scaled_mousedelta.x);
								if( f < (1.0f/2.0f) )
									FLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_Y_OFF);
								else
									FLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_X_OFF);
							}
							else
							{
								// Strange to be here because we are in TOUCH MOVE, so ... at least on component X or Y should be changed ...
								NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION); // CRASH in debug mode ... to see
							}
						}

						if(ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_X_OFF))
							scaled_mousedelta.x = 0.0f;
						if(ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_Y_OFF))
							scaled_mousedelta.y = 0.0f;
					}

					updata.Type						= NUT_EDITOR_ATXC_UPDATA_UPDATE_TXCOFFSET;
					updata.TexCoord.Current.x		= NCLAMP(ptooldata->pCurrentEntity->EditingTxCOffset_Min,ptooldata->Elements.TexCoordOffset[ptooldata->HUDGizmo.EditedTexCoordSetID].x + scaled_mousedelta.x,ptooldata->pCurrentEntity->EditingTxCOffset_Max );
					updata.TexCoord.Current.y		= NCLAMP(ptooldata->pCurrentEntity->EditingTxCOffset_Min,ptooldata->Elements.TexCoordOffset[ptooldata->HUDGizmo.EditedTexCoordSetID].y + scaled_mousedelta.y,ptooldata->pCurrentEntity->EditingTxCOffset_Max ); 
					updata.TexCoord.TxCSet			= ptooldata->HUDGizmo.EditedTexCoordSetID;
// 					updata.TexCoord.pEditedVertexTxC= NULL;
// 					updata.TexCoord.EditedVertexId	= NVOID;

					updata.TexCoord.Previous		= ptooldata->Elements.TexCoordOffset[ptooldata->HUDGizmo.EditedTexCoordSetID];

					if( (updata.TexCoord.Current.x!=updata.TexCoord.Previous.x)||(updata.TexCoord.Current.y!=updata.TexCoord.Previous.y) )
					{
						ptooldata->Normalized_PreviousTouch = norm_mouse;
						if( ptooldata->pCurrentEntity->Update_CallBack(pselect,&updata) )
						{
							ptooldata->Elements.TexCoordOffset[ptooldata->HUDGizmo.EditedTexCoordSetID]	= updata.TexCoord.Current;
							printf("\nTxC Offset(%f,%f)",updata.TexCoord.Current.x,updata.TexCoord.Current.y);
						}
					}

					FLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_MOVING);
					return NEDITOR_RESULT_TOOL_ON;

				case NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SCALE_U:
					scaled_mousedelta.x =  (norm_mouse.x - ptooldata->Normalized_PreviousTouch.x)*ptooldata->pCurrentEntity->EditingTxCScale_TouchScale.x/(ptooldata->HUDGizmo.DragHandleSize.x*ptooldata->pCurrentEntity->EditingTxCScale_BaseScale.x);

					// Until TexCoordScale U value is less than the MAx define by user. Engine update it with the ABSOLUTE method.
					// But if incoming TexCoordScale value is out of range, a second method is apply: RELATIVE method using UVSpeed
					updata.Type		= NUT_EDITOR_ATXC_UPDATA_UPDATE_TXCSCALE;
					updata.TexCoord.Current.x	= ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID].x + scaled_mousedelta.x;
					updata.TexCoord.Current.y	= ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID].y;
					
					// Use UVSpeed Method ?
					if( updata.TexCoord.Current.x > ptooldata->pCurrentEntity->EditingTxCScale_SpeedSwitchThreshold.x )
					{
						updata.TexCoord.Current.x = ptooldata->pCurrentEntity->EditingTxCScale_SpeedSwitchThreshold.x;
						ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SCALE_USPEED;
						ptooldata->HUDGizmo.TexCoordScaleSpeed.x = 0.0f;
						ptooldata->HUDGizmo.TexCoordScaleSpeed.y = 0.0f;
					}
					else if( updata.TexCoord.Current.x < -ptooldata->pCurrentEntity->EditingTxCScale_SpeedSwitchThreshold.x ) 
					{
						updata.TexCoord.Current.x = -ptooldata->pCurrentEntity->EditingTxCScale_SpeedSwitchThreshold.x;
						ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SCALE_USPEED;
						ptooldata->HUDGizmo.TexCoordScaleSpeed.x = 0.0f;
						ptooldata->HUDGizmo.TexCoordScaleSpeed.y = 0.0f;
					}
					
					// ... Finalize Updata setup.
					updata.TexCoord.Previous		= ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID];
					updata.TexCoord.TxCSet			= ptooldata->HUDGizmo.EditedTexCoordSetID;
// 					updata.TexCoord.pEditedVertexTxC= NULL;
// 					updata.TexCoord.EditedVertexId	= NVOID;

					if( updata.TexCoord.Current.x != ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID].x )
					{
						if( ptooldata->pCurrentEntity->Update_CallBack(pselect,&updata) )
						{
							ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID] = updata.TexCoord.Current;
							printf("\nTxCScale U(%f)",updata.TexCoord.Current.x);
						}
					}
					ptooldata->Normalized_PreviousTouch = norm_mouse;
					return NEDITOR_RESULT_TOOL_ON;

				case NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SCALE_V:
					scaled_mousedelta.y = -(norm_mouse.y - ptooldata->Normalized_PreviousTouch.y)*ptooldata->pCurrentEntity->EditingTxCScale_TouchScale.y/(ptooldata->HUDGizmo.DragHandleSize.y*ptooldata->pCurrentEntity->EditingTxCScale_BaseScale.y);
					// Until TxCScale V value is less than the Max define by user. Engine update it with the ABSOLUTE method.
					// But if incoming TxCScale value is out of range, a second method is apply: RELATIVE method using UVSpeed
					updata.Type					= NUT_EDITOR_ATXC_UPDATA_UPDATE_TXCSCALE;
					updata.TexCoord.Current.x	= ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID].x;
					updata.TexCoord.Current.y	= ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID].y + scaled_mousedelta.y;

					// Use TxCScaleSpeed Method ?
					if( updata.TexCoord.Current.y > ptooldata->pCurrentEntity->EditingTxCScale_SpeedSwitchThreshold.y )
					{
						updata.TexCoord.Current.y = ptooldata->pCurrentEntity->EditingTxCScale_SpeedSwitchThreshold.y;
						ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SCALE_VSPEED;
						ptooldata->HUDGizmo.TexCoordScaleSpeed.x = 0.0f;
						ptooldata->HUDGizmo.TexCoordScaleSpeed.y = 0.0f;
					}
					else if( updata.TexCoord.Current.y < -ptooldata->pCurrentEntity->EditingTxCScale_SpeedSwitchThreshold.y ) 
					{
						updata.TexCoord.Current.y = -ptooldata->pCurrentEntity->EditingTxCScale_SpeedSwitchThreshold.y;
						ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SCALE_VSPEED;
						ptooldata->HUDGizmo.TexCoordScaleSpeed.x = 0.0f;
						ptooldata->HUDGizmo.TexCoordScaleSpeed.y = 0.0f;
					}
					// ... Finalize Updata Setup.
					updata.TexCoord.Previous			= ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID];
					updata.TexCoord.TxCSet				= ptooldata->HUDGizmo.EditedTexCoordSetID;
// 					updata.TexCoord.pEditedVertexTxC	= NULL;
// 					updata.TexCoord.EditedVertexId		= NVOID;

					if( updata.TexCoord.Current.y != updata.TexCoord.Previous.y )
					{
						if( ptooldata->pCurrentEntity->Update_CallBack(pselect,&updata) )
						{
							ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID]	= updata.TexCoord.Current;
							printf("\nTxC Scale V(%f)",updata.TexCoord.Current.y);
						}
					}
					ptooldata->Normalized_PreviousTouch = norm_mouse;
					return NEDITOR_RESULT_TOOL_ON;

				case NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SCALE_USPEED:
					scaled_mousedelta.x =  (norm_mouse.x - ptooldata->Normalized_PreviousTouch.x)*ptooldata->pCurrentEntity->EditingTxCScale_TouchScale.x/(ptooldata->HUDGizmo.DragHandleSize.x*ptooldata->pCurrentEntity->EditingTxCScale_BaseScale.x);
					// UV Speed U Update:
					ptooldata->HUDGizmo.TexCoordScaleSpeed.x = NCLAMP(-ptooldata->pCurrentEntity->EditingTxCScale_SpeedMax.x,
																		ptooldata->HUDGizmo.TexCoordScaleSpeed.x + scaled_mousedelta.x,
																		ptooldata->pCurrentEntity->EditingTxCScale_SpeedMax.x);
					ptooldata->Normalized_PreviousTouch = norm_mouse;
					// Notes: UVScale update is going to be performed into the TimeHandle process ! using the speed updated here !
					return NEDITOR_RESULT_TOOL_ON;

				case NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SCALE_VSPEED:
					scaled_mousedelta.y = -(norm_mouse.y - ptooldata->Normalized_PreviousTouch.y)*ptooldata->pCurrentEntity->EditingTxCScale_TouchScale.y/(ptooldata->HUDGizmo.DragHandleSize.y*ptooldata->pCurrentEntity->EditingTxCScale_BaseScale.y);
					// UV Speed V Update:
					ptooldata->HUDGizmo.TexCoordScaleSpeed.y = NCLAMP(-ptooldata->pCurrentEntity->EditingTxCScale_SpeedMax.y,
																		ptooldata->HUDGizmo.TexCoordScaleSpeed.y + scaled_mousedelta.y,
																		ptooldata->pCurrentEntity->EditingTxCScale_SpeedMax.y);
					ptooldata->Normalized_PreviousTouch = norm_mouse;
					// Notes: UVScale update is going to be performed into the TimeHandle process ! using the speed updated here !
					return NEDITOR_RESULT_TOOL_ON;

				case NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_RESET_U:
					ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_VOID;
					FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_RESET );
					return NEDITOR_RESULT_TOOL_ON;

				case NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_RESET_V:
					ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_VOID;
					FLAG_OFF(ptooldata->HUDGizmo.Flags, FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_RESET );
					return NEDITOR_RESULT_TOOL_ON;

				case NUT_EDITOR_ATXC_GIZMO_STATE_TEXTURE:
				// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// +
				// +---+ GET TEXTURE 
				// +
					// Touch Angle (from Gizmo Center)
					NUT_ATxC_GetDrag_HandleCenter( &v0, &ptooldata->HUDGizmo );
					NVec2Sub( &v1,&norm_mouse,&v0 );
					NVec2Normalize(&v1);
					angle = NRADtoDEG( acos(v1.x) );
					if(v1.y<0.0f)
						angle = 360.0f - angle;
					 // Retrieve Texture Ptr from Angle
					sectornb			= NUT_ATxC_GetTextureRingSectorNb(ptooldata);
					index				= NUT_ATxC_GetSectorIndexFromTextureRingSelector( angle, sectornb );
					
					pmetatexture		= NUT_ATxC_GetTextureRingSectorMetaTexture(&metatexturetype, ptooldata, index );
					pcurrentmetatexture = NUT_ATxC_GetCurrentMetaTexture(&currentmetatexturetype, ptooldata);
				// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				// +
				// +---+ RESULT ANALYSIS & UPDATE
				// +
					if(pmetatexture != pcurrentmetatexture )
					{
						// +---+ ANALYSIS: NULL, UPDATE or NEW ?
						if( pmetatexture )
						{
							if( !pcurrentmetatexture )
								updata.Type	= NUT_EDITOR_ATXC_UPDATA_NEW_TEXTURE;
							else
								updata.Type	= NUT_EDITOR_ATXC_UPDATA_UPDATE_TEXTURE;
						}
						else
						{
							updata.Type		= NUT_EDITOR_ATXC_UPDATA_NULL_TEXTURE;
						}
						
						switch(metatexturetype)
						{
							case NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_NULL:
								updata.Texture.pTexture			= NULL;
								updata.Texture.pAtlas_Element	= NULL;
								break;

							case NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_PLAIN:
								updata.Texture.pTexture			= (NTEXTURE*)pmetatexture;
								updata.Texture.pAtlas_Element	= NULL;
								break;

							case NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_ATLAS_ELEMENT:
								updata.Texture.pAtlas_Element	= (NATLAS_ELEMENT*)pmetatexture;
								updata.Texture.pTexture			= updata.Texture.pAtlas_Element->pTexture;
								break;

							default:
								NErrorIf(1,NERROR_INVALID_CASE);// It should never happen !
								break;
						}

						updata.Texture.TextureUnit	= ptooldata->HUDGizmo.EditedTextureUnit; 

						// +---+ UPDATE	
						if(ptooldata->pCurrentEntity->Update_CallBack(pselect,&updata))
						{
							ptooldata->Elements.pMetaTexture[updata.Texture.TextureUnit]	= pmetatexture;
							ptooldata->Elements.MetaTextureType[updata.Texture.TextureUnit]	= metatexturetype;
							// +---+ UPDATE	GIZMO TEXTURE HANDLE POSITION
							NUT_ATxC_SetTextureHandlePosition(ptooldata,index);
						}
					
						// +---+ Update GIZMO TEXTURE PARAMETERS EDITION
						NUT_ATxC_TextureParametersActivation(ptooldata);

					}
					return NEDITOR_RESULT_TOOL_ON;
				
				case NUT_EDITOR_ATXC_GIZMO_STATE_TEXTURE_WRAP_S:
					v0.x = (norm_mouse.x - ptooldata->Normalized_TouchStart.x);
					v0.y = (norm_mouse.y - ptooldata->Normalized_TouchStart.y);
					v1.x = (ptooldata->Normalized_PreviousTouch.x - ptooldata->Normalized_TouchStart.x);
					v1.y = (ptooldata->Normalized_PreviousTouch.y - ptooldata->Normalized_TouchStart.y);
					f = NVec2FastLength(&ptooldata->HUDGizmo.HandleSize);
					s32 = (Ns32)(floor(NVec2FastLength(&v1)/f)-floor(NVec2FastLength(&v0)/f));
					if(s32)
					{
						if( NUT_ATxC_GetCurrentMetaTexturePtrs(&updata.TextureGLParam.pAtlas_Element, &updata.TextureGLParam.pTexture, ptooldata) != NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_NULL )
						{
							updata.Type	=  NUT_EDITOR_ATXC_UPDATA_UPDATE_TEXTURE_WRAP_S;
							
							updata.TextureGLParam.Param = _get_wrapglint((Ns32)_get_wrapid(ptooldata->Elements.TextureWrapS)+s32);
							updata.TextureGLParam.TextureUnit = ptooldata->HUDGizmo.EditedTextureUnit;
							if( ptooldata->pCurrentEntity->Update_CallBack(pselect,&updata) )
							{
								ptooldata->Elements.TextureWrapS = updata.TextureGLParam.Param;
							}
						}
#ifdef _DEBUG
						else
						{
							NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION); // It should never happen !
						}
#endif
					}
					ptooldata->Normalized_PreviousTouch = norm_mouse;
					return NEDITOR_RESULT_TOOL_ON;

				case NUT_EDITOR_ATXC_GIZMO_STATE_TEXTURE_WRAP_T:
					v0.x = (norm_mouse.x - ptooldata->Normalized_TouchStart.x);
					v0.y = (norm_mouse.y - ptooldata->Normalized_TouchStart.y);
					v1.x = (ptooldata->Normalized_PreviousTouch.x - ptooldata->Normalized_TouchStart.x);
					v1.y = (ptooldata->Normalized_PreviousTouch.y - ptooldata->Normalized_TouchStart.y);
					f = NVec2FastLength(&ptooldata->HUDGizmo.HandleSize);
					s32 = (Ns32)(floor(NVec2FastLength(&v1)/f)-floor(NVec2FastLength(&v0)/f));
					if(s32)
					{
						if( NUT_ATxC_GetCurrentMetaTexturePtrs(&updata.TextureGLParam.pAtlas_Element, &updata.TextureGLParam.pTexture, ptooldata) != NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_NULL )
						{
							updata.Type	=  NUT_EDITOR_ATXC_UPDATA_UPDATE_TEXTURE_WRAP_T;

							updata.TextureGLParam.Param = _get_wrapglint((Ns32)_get_wrapid(ptooldata->Elements.TextureWrapT)+s32);
							updata.TextureGLParam.TextureUnit = ptooldata->HUDGizmo.EditedTextureUnit;
							if( ptooldata->pCurrentEntity->Update_CallBack(pselect,&updata) )
							{
								ptooldata->Elements.TextureWrapT = updata.TextureGLParam.Param;
							}
						}
#ifdef _DEBUG
						else
						{
							NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION); // It should never happen !
						}
#endif
					}
					ptooldata->Normalized_PreviousTouch = norm_mouse;
					return NEDITOR_RESULT_TOOL_ON;

				case NUT_EDITOR_ATXC_GIZMO_STATE_TEXTURE_MIN_FILTER:
					v0.x = (norm_mouse.x - ptooldata->Normalized_TouchStart.x);
					v0.y = (norm_mouse.y - ptooldata->Normalized_TouchStart.y);
					v1.x = (ptooldata->Normalized_PreviousTouch.x - ptooldata->Normalized_TouchStart.x);
					v1.y = (ptooldata->Normalized_PreviousTouch.y - ptooldata->Normalized_TouchStart.y);
					f = NVec2FastLength(&ptooldata->HUDGizmo.HandleSize);
					s32 = (Ns32)(floor(NVec2FastLength(&v1)/f)-floor(NVec2FastLength(&v0)/f));
					if(s32)
					{
						if( NUT_ATxC_GetCurrentMetaTexturePtrs(&updata.TextureGLParam.pAtlas_Element, &updata.TextureGLParam.pTexture, ptooldata) != NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_NULL )
						{
							updata.Type	=  NUT_EDITOR_ATXC_UPDATA_UPDATE_TEXTURE_MIN_FILTER;

							updata.TextureGLParam.Param = _get_minfilterglint((Ns32)_get_filterid(ptooldata->Elements.TextureMinFilter)+s32);
							updata.TextureGLParam.TextureUnit = ptooldata->HUDGizmo.EditedTextureUnit;
							if( ptooldata->pCurrentEntity->Update_CallBack(pselect,&updata) )
							{
								ptooldata->Elements.TextureMinFilter = updata.TextureGLParam.Param;
							}
						}
#ifdef _DEBUG
						else
						{
							NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION); // It should never happen !
						}
#endif
					}
					ptooldata->Normalized_PreviousTouch = norm_mouse;
					return NEDITOR_RESULT_TOOL_ON;

				case NUT_EDITOR_ATXC_GIZMO_STATE_TEXTURE_MAG_FILTER:
					v0.x = (norm_mouse.x - ptooldata->Normalized_TouchStart.x);
					v0.y = (norm_mouse.y - ptooldata->Normalized_TouchStart.y);
					v1.x = (ptooldata->Normalized_PreviousTouch.x - ptooldata->Normalized_TouchStart.x);
					v1.y = (ptooldata->Normalized_PreviousTouch.y - ptooldata->Normalized_TouchStart.y);
					f = NVec2FastLength(&ptooldata->HUDGizmo.HandleSize);
					s32 = (Ns32)(floor(NVec2FastLength(&v1)/f)-floor(NVec2FastLength(&v0)/f));
					if(s32)
					{
						if( NUT_ATxC_GetCurrentMetaTexturePtrs(&updata.TextureGLParam.pAtlas_Element, &updata.TextureGLParam.pTexture, ptooldata) != NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_NULL )
						{
							updata.Type	=  NUT_EDITOR_ATXC_UPDATA_UPDATE_TEXTURE_MAG_FILTER;

							updata.TextureGLParam.Param = _get_magfilterglint((Ns32)_get_filterid(ptooldata->Elements.TextureMagFilter)+s32);
							updata.TextureGLParam.TextureUnit = ptooldata->HUDGizmo.EditedTextureUnit;
							if( ptooldata->pCurrentEntity->Update_CallBack(pselect,&updata) )
							{
								ptooldata->Elements.TextureMagFilter = updata.TextureGLParam.Param;
							}
						}
#ifdef _DEBUG
						else
						{
							NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION); // It should never happen !
						}
#endif
					}
					ptooldata->Normalized_PreviousTouch = norm_mouse;
					return NEDITOR_RESULT_TOOL_ON;

				case NUT_EDITOR_ATXC_GIZMO_STATE_GEOMSPECS:
					return NEDITOR_RESULT_TOOL_ON;

				case NUT_EDITOR_ATXC_GIZMO_STATE_TEXTUREUNIT:
					NUT_ATxC_GetDrag_HandleCenter( &v0, &ptooldata->HUDGizmo );
					NVec2Sub( &v1,&norm_mouse,&v0 );
					NVec2Normalize(&v1);
					angle = NRADtoDEG( acos(v1.x) );
					if(v1.y<0.0f)
						angle = 360.0f - angle;

					// Retrieve Blend Ptr from Angle
					sectornb = NUT_ATxC_GetTextureUnitNumber(&ptooldata->Elements);
					index = NUT_ATxC_GetSectorIndexFromArcSelector(angle, sectornb, NUT_EDITOR_ATXC_ARC_SELECTOR_ID_TEXTUREUNIT  );
					// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
					// +
					// +---+ RESULT ANALYSIS & UPDATE
					// +
					if( index != ptooldata->HUDGizmo.EditedTextureUnit )
					{
						ptooldata->HUDGizmo.EditedTextureUnit = (Nu8)index;
						// +---+ UPDATE	GIZMO TEXTURE UNIT HANDLE POSITION
						// Recompute a 'perfect' angle in the exact middle of the angular sector
						// And calculate the associate coordinates.
						NUT_ATxC_SetTextureUnitHandlePosition(ptooldata);
						NUT_ATxC_TextureRingActivation( ptooldata );
						// +---+ Update GIZMO TEXTURE PARAMETERS EDITION
						NUT_ATxC_TextureParametersActivation(ptooldata);
					}
					return NEDITOR_RESULT_TOOL_ON;

				case NUT_EDITOR_ATXC_GIZMO_STATE_BLEND:
					NUT_ATxC_GetDrag_HandleCenter( &v0, &ptooldata->HUDGizmo );
					NVec2Sub( &v1,&norm_mouse,&v0 );
					NVec2Normalize(&v1);
					angle = NRADtoDEG( acos(v1.x) );
					if(v1.y<0.0f)
						angle = 360.0f - angle;
					
					 // Retrieve Blend Ptr from Angle
					sectornb = NUT_ATxC_GetBlendListSize(ptooldata);
					index = NUT_ATxC_GetSectorIndexFromArcSelector(angle, sectornb, NUT_EDITOR_ATXC_ARC_SELECTOR_ID_BLEND  );
					updata.Blend.pBlend = NUT_ATxC_GetBlendIndexPtr( ptooldata, index );
					// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
					// +
					// +---+ RESULT ANALYSIS & UPDATE
					// +
					if( (updata.Blend.pBlend != ptooldata->Elements.pBlend) )
					{
						// NULL, UPDATE or NEW ?
						if( updata.Blend.pBlend )
						{
							if( !ptooldata->Elements.pBlend )
								updata.Type	= NUT_EDITOR_ATXC_UPDATA_NEW_BLEND;
							else
								updata.Type	= NUT_EDITOR_ATXC_UPDATA_UPDATE_BLEND;
						}
						else
						{
							updata.Type		= NUT_EDITOR_ATXC_UPDATA_NULL_BLEND;
						}

						if( ptooldata->pCurrentEntity->Update_CallBack(pselect,&updata) )
						{
							ptooldata->Elements.pBlend	= updata.Blend.pBlend;
							// +---+ UPDATE	GIZMO TEXTURE HANDLE POSITION
							// Recompute a 'perfect' angle in the exact middle of the angular sector
							// And calculate the associate coordinates.
							NUT_ATxC_SetBlendHandlePosition(ptooldata,index);
						}
					}
					return NEDITOR_RESULT_TOOL_ON;

				case NUT_EDITOR_ATXC_GIZMO_STATE_MATERIAL:
					return NEDITOR_RESULT_TOOL_ON;

				case NUT_EDITOR_ATXC_GIZMO_STATE_GIZMO:
					ptooldata->HUDGizmo.Position.x  += norm_mouse.x - ptooldata->Normalized_PreviousTouch.x;
					ptooldata->HUDGizmo.Position.y  += norm_mouse.y - ptooldata->Normalized_PreviousTouch.y;
					ptooldata->Normalized_PreviousTouch = norm_mouse;
					return NEDITOR_RESULT_TOOL_ON;

				case NUT_EDITOR_ATXC_GIZMO_STATE_VOID:
					return NEDITOR_RESULT_TOOL_OFF;
			}		// switch(ptooldata->HUDGizmo.State)
			break;	// case NTOUCH_MOVE

	case NTOUCH_END:
		ptooldata = (NUT_EDITOR_ATXC_TOOLDATA*)tool_param;
		if( ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_ACTIVE) )
		{
			switch(ptooldata->HUDGizmo.State)
			{
				case NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_OFFSET:
					if( ISFLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_MOVING) )
					{
						FLAG_TOGGLE(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_MOVE_AXIS_RESTRICTION);
					}
					else
					{
						FLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_MOVING);
					}
					break;

				case NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_RESET_U:
					// Which KIND of UV RESET ?
					if(ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXCOORD_RESET_SCALE))
					{
						updata.Type						= NUT_EDITOR_ATXC_UPDATA_UPDATE_TXCSCALE;
						updata.TexCoord.Current.x		= ptooldata->InitialTexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID].x;
						updata.TexCoord.Current.y		= ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID].y;
						updata.TexCoord.Previous		= ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID];
						updata.TexCoord.TxCSet			= ptooldata->HUDGizmo.EditedTexCoordSetID;
// 						updata.TexCoord.pEditedVertexTxC= NULL;
// 						updata.TexCoord.EditedVertexId	= NVOID;

						if( updata.TexCoord.Current.x != updata.TexCoord.Previous.x )
						{
							if( ptooldata->pCurrentEntity->Update_CallBack(pselect,&updata) )
							{
								ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID].x	= updata.TexCoord.Current.x;
							}
						}
					}

					if(ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXCOORD_RESET_OFFSET))
					{
						updata.Type						= NUT_EDITOR_ATXC_UPDATA_UPDATE_TXCOFFSET;
						updata.TexCoord.Current.x		= ptooldata->InitialTexCoordOffset[ptooldata->HUDGizmo.EditedTexCoordSetID].x;
						updata.TexCoord.Current.y		= ptooldata->Elements.TexCoordOffset[ptooldata->HUDGizmo.EditedTexCoordSetID].y;
						updata.TexCoord.Previous		= ptooldata->Elements.TexCoordOffset[ptooldata->HUDGizmo.EditedTexCoordSetID];
						updata.TexCoord.TxCSet			= ptooldata->HUDGizmo.EditedTexCoordSetID;
// 						updata.TexCoord.pEditedVertexTxC= NULL;
// 						updata.TexCoord.EditedVertexId	= NVOID;

						if( updata.TexCoord.Current.x != updata.TexCoord.Previous.x )
						{
							if( ptooldata->pCurrentEntity->Update_CallBack(pselect,&updata) )
							{
								ptooldata->Elements.TexCoordOffset[ptooldata->HUDGizmo.EditedTexCoordSetID].x	= updata.TexCoord.Current.x - ceil(updata.TexCoord.Current.x);
							}
						}
					}
					break;

				case NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_RESET_V:
					// Which KIND of UV RESET ?
					if(ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXCOORD_RESET_SCALE))
					{
						updata.Type						= NUT_EDITOR_ATXC_UPDATA_UPDATE_TXCSCALE;
						updata.TexCoord.Current.x		= ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID].x;
						updata.TexCoord.Current.y		= ptooldata->InitialTexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID].y;
						updata.TexCoord.Previous		= ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID];
						updata.TexCoord.TxCSet			= ptooldata->HUDGizmo.EditedTexCoordSetID;
// 						updata.TexCoord.pEditedVertexTxC= NULL;
// 						updata.TexCoord.EditedVertexId	= NVOID;

						if( updata.TexCoord.Current.y != updata.TexCoord.Previous.y )
						{
							if( ptooldata->pCurrentEntity->Update_CallBack(pselect,&updata) )
							{
								ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID].y	= updata.TexCoord.Current.y;
							}
						}
					}
					if(ISFLAG_ON(ptooldata->Elements.Flags,FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXCOORD_RESET_OFFSET))
					{
						updata.Type						= NUT_EDITOR_ATXC_UPDATA_UPDATE_TXCOFFSET;
						updata.TexCoord.Current.x		= ptooldata->Elements.TexCoordOffset[ptooldata->HUDGizmo.EditedTexCoordSetID].x;
						updata.TexCoord.Current.y		= ptooldata->InitialTexCoordOffset[ptooldata->HUDGizmo.EditedTexCoordSetID].y;
						updata.TexCoord.Previous		= ptooldata->Elements.TexCoordOffset[ptooldata->HUDGizmo.EditedTexCoordSetID];
						updata.TexCoord.TxCSet			= ptooldata->HUDGizmo.EditedTexCoordSetID;
// 						updata.TexCoord.pEditedVertexTxC= NULL;
// 						updata.TexCoord.EditedVertexId	= NVOID;

						if( updata.TexCoord.Current.y != updata.TexCoord.Previous.y )
						{
							if( ptooldata->pCurrentEntity->Update_CallBack(pselect,&updata) )
							{
								ptooldata->Elements.TexCoordOffset[ptooldata->HUDGizmo.EditedTexCoordSetID].y = updata.TexCoord.Current.y - ceil(updata.TexCoord.Current.y);
							}
						}
					}
					break;
			}
		}

		ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_VOID;
		_all_gizmo_updating_flags_off(&ptooldata->HUDGizmo);
		
		return NEDITOR_RESULT_TOOL_ON;

		case NTOUCH_CANCEL:
			// Test and Check !!!! maybe there are some other things to do !
			ptooldata = (NUT_EDITOR_ATXC_TOOLDATA*)tool_param;
			ptooldata->HUDGizmo.State = NUT_EDITOR_ATXC_GIZMO_STATE_VOID;
			_all_gizmo_updating_flags_off(&ptooldata->HUDGizmo);
			break;

		default:
			break;
	}

	return NEDITOR_RESULT_TOOL_OFF;
}

// ------------------------------------------------------------------------------------------
// NUT_EditorTool_ATxC_Modifier_TimeHandle
// ------------------------------------------------------------------------------------------
// Description :
// Draws TUV Gizmo ...
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_Editor_ATxC_Tool_TimeHandle(const NTIME* ptime,const NEDITOR_SELECTION *pselect,const Nu32 tool_param )
{
	NATLAS_ELEMENT				*patlas_el;
	NUT_DRAW_ELLIPSE			ellipse;
	NUT_DRAW_ARROWS				arrows;
	NUT_DRAW_SPRING				spring;
	NUT_EDITOR_ATXC_TOOLDATA	*ptooldata;
	NUT_EDITOR_ATXC_UPDATA		updata;
	NCOLOR						*pcolor;
	NUT_DRAWTEXT				text_param;
	Nchar						str[64];


	ptooldata = (NUT_EDITOR_ATXC_TOOLDATA*)tool_param;

	NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
	NUT_SetDrawConstructionPlane(_PLANE_XY );


//	NVEC3	*pv;
//	Nu32		stride;
	NVEC3	v,w;
	NVEC3	center;
	NVEC2	extents;
	NVEC3	A, B;

	NVec3Set(&v,0,0,0);
	NVec3Set(&A, 0, 0, 0);
	NVec3Set(&B, 0, 0, 0);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + THE GIZMO MAIN QUAD
	// +
	if( ISFLAG_ON( ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_GIZMO ) )
	{
		pcolor = &ptooldata->HUDGizmo.InUseColor;
	}
	else
	{
		// GIZMO is set to ACTIVE, that means some parts of it are ON (texture, blend, ... )
		if( ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_ACTIVE) )
		{
			pcolor =  &ptooldata->HUDGizmo.ActiveColor;
		}
		else
		// GIZMO is set to PASSIVE, that means, there are no parts of it in activation (set to ON).
		// So, is just empty and there are no more than the drag handle to draw (and a specific 'CONSTANT_NUT_EDITOR_ATXC__GIZMO_OFF' information text )
		{
			pcolor = &ptooldata->HUDGizmo.PassiveColor;
		}
	}
	
	NUT_ATxC_GetDrag_HandleExtents(&extents, &ptooldata->HUDGizmo);
	NUT_ATxC_GetDrag_HandleCenter( (NVEC2*)&center, &ptooldata->HUDGizmo );
	center.z = 0.0f;
	NUT_Draw_Quad(&center,&extents,pcolor);
	if( ISFLAG_OFF(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_ACTIVE) )
	{
		// Adding specific Text if GIZMO is PASSIVE
		NUT_ATxC_GetText_Position2((NVEC2*)&v,&ptooldata->HUDGizmo);
		text_param.Color	= *pcolor;
		text_param.Size		= 0.02f;
		NUT_Draw_Text(CONSTANT_NUT_EDITOR_ATXC__GIZMO_OFF, &v, &text_param);
		// ... and stop drawing.
		return;
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + THE GIZMO WRAP SELECTOR
	// +
	if( ISFLAG_ON( ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTURE_WRAP_ON) )
	{
		if( ISFLAG_ON( ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTURE_WRAP) )
		{
			pcolor = &ptooldata->HUDGizmo.InUseColor;
			
			sprintf(str,"WRAP_S: ");
			_strcat_wrapname(str,_get_wrapid(ptooldata->Elements.TextureWrapS));
			strcat(str,"\nWRAP_T: ");
			_strcat_wrapname(str,_get_wrapid(ptooldata->Elements.TextureWrapT));
			NUT_ATxC_GetText_Position1((NVEC2*)&v,&ptooldata->HUDGizmo);
			text_param.Color	= *pcolor;
			text_param.Size		= 0.02f;
			NUT_Draw_Text(str, &v, &text_param);
		}
		else
		{
			pcolor = &ptooldata->HUDGizmo.ActiveColor;
		}

		center.z = 0.0f;
		NUT_ATxC_GetWrap_S_HandleExtents(&extents, &ptooldata->HUDGizmo);
		NUT_ATxC_GetWrap_S_HandleCenter( (NVEC2*)&center, &ptooldata->HUDGizmo );
		extents.x *=0.5f;
		extents.y *=0.9f;
		switch(ptooldata->Elements.TextureWrapS)
		{
			case GL_CLAMP_TO_BORDER:
				NUT_Draw_Quad(&center,&extents,pcolor);
				break;

			case GL_CLAMP_TO_EDGE:
				v.x = w.x = center.x;
				v.y = center.y - extents.y;
				w.y = center.y + extents.y;
				v.z = w.z = center.z;
				NUT_Draw_Segment(&v,&w,pcolor);
				break;

			case GL_REPEAT:
				NUT_Draw_Left(&center,&extents,pcolor);
				break;

			case GL_MIRRORED_REPEAT:
				NUT_Draw_Right(&center,&extents,pcolor);
			break;
		}
		NUT_ATxC_GetWrap_T_HandleExtents(&extents, &ptooldata->HUDGizmo);
		NUT_ATxC_GetWrap_T_HandleCenter( (NVEC2*)&center, &ptooldata->HUDGizmo );
		extents.x *=0.9f;
		extents.y *=0.5f;
		switch(ptooldata->Elements.TextureWrapT)
		{
			case GL_CLAMP_TO_BORDER:
				NUT_Draw_Quad(&center,&extents,pcolor);
				break;

			case GL_CLAMP_TO_EDGE:
				v.x = center.x - extents.x;
				w.x = center.x + extents.x;
				v.y = w.y = center.y;
				v.z = w.z = center.z;
				NUT_Draw_Segment(&v,&w,pcolor);
				break;

			case GL_REPEAT:
				NUT_Draw_Up(&center,&extents,pcolor);
				break;

			case GL_MIRRORED_REPEAT:
				NUT_Draw_Down(&center,&extents,pcolor);
				break;
		}
	}
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + THE GIZMO FILTER SELECTOR
	// +
	if( ISFLAG_ON( ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTURE_FILTER_ON) )
	{
		if( ISFLAG_ON( ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTURE_FILTER) )
		{
			pcolor = &ptooldata->HUDGizmo.InUseColor;
			
			sprintf(str,"MIN: ");
			_strcat_filtername(str,_get_filterid(ptooldata->Elements.TextureMinFilter));
			strcat(str,"\nMAG: ");
			_strcat_filtername(str,_get_filterid(ptooldata->Elements.TextureMagFilter));
			NUT_ATxC_GetText_Position1((NVEC2*)&v,&ptooldata->HUDGizmo);
			text_param.Color	= *pcolor;
			text_param.Size		= 0.02f;
			NUT_Draw_Text(str, &v, &text_param);
		}
		else
		{
			pcolor = &ptooldata->HUDGizmo.ActiveColor;
		}

		center.z = 0.0f;
		NUT_ATxC_GetMinFilter_HandleExtents(&extents, &ptooldata->HUDGizmo);
		NUT_ATxC_GetMinFilter_HandleCenter( (NVEC2*)&center, &ptooldata->HUDGizmo );
		switch(ptooldata->Elements.TextureMinFilter)
		{
			case GL_NEAREST:
				NUT_Draw_Quad(&center,&extents,pcolor);
				break;

			case GL_LINEAR:
				Nmem0(&ellipse,NUT_DRAW_ELLIPSE);
				ellipse.SliceNb	= 8;
				ellipse.Center	= center;
				ellipse.Extents = extents;
				ellipse.Color	= *pcolor;
				NUT_Draw_Ellipse(&ellipse);
				break;
			case GL_NEAREST_MIPMAP_NEAREST:
				NUT_Draw_Quad(&center,&extents,pcolor);
				NVec2ScaleBy(&extents,0.5f);
				NUT_Draw_Quad(&center,&extents,pcolor);
				break;

			case GL_LINEAR_MIPMAP_NEAREST:
				Nmem0(&ellipse,NUT_DRAW_ELLIPSE);
				ellipse.SliceNb	= 8;
				ellipse.Center	= center;
				ellipse.Extents = extents;
				ellipse.Color	= *pcolor;
				NUT_Draw_Ellipse(&ellipse);

				NVec2ScaleBy(&extents,0.5f);
				NUT_Draw_Quad(&center,&extents,pcolor);
				break;
			case GL_NEAREST_MIPMAP_LINEAR:
				NUT_Draw_Quad(&center,&extents,pcolor);
				Nmem0(&ellipse,NUT_DRAW_ELLIPSE);
				ellipse.SliceNb	= 8;
				ellipse.Center	= center;
				NVec2Scale(&ellipse.Extents,&extents,0.5f);
				ellipse.Color	= *pcolor;
				NUT_Draw_Ellipse(&ellipse);
				break;
			case GL_LINEAR_MIPMAP_LINEAR:
				Nmem0(&ellipse,NUT_DRAW_ELLIPSE);
				ellipse.SliceNb	= 8;
				ellipse.Center	= center;
				ellipse.Extents = extents;
				ellipse.Color	= *pcolor;
				NUT_Draw_Ellipse(&ellipse);
				NVec2ScaleBy(&ellipse.Extents,0.5f);
				NUT_Draw_Ellipse(&ellipse);
				break;
		}
	
		NUT_ATxC_GetMagFilter_HandleExtents(&extents, &ptooldata->HUDGizmo);
		NUT_ATxC_GetMagFilter_HandleCenter( (NVEC2*)&center, &ptooldata->HUDGizmo );
		switch(ptooldata->Elements.TextureMagFilter)
		{
			case GL_NEAREST:
				NUT_Draw_Quad(&center,&extents,pcolor);
				break;

			case GL_LINEAR:
				Nmem0(&ellipse,NUT_DRAW_ELLIPSE);
				ellipse.SliceNb	= 8;
				ellipse.Center	= center;
				ellipse.Extents = extents;
				ellipse.Color	= *pcolor;
				NUT_Draw_Ellipse(&ellipse);
				break;
		}
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + ALL THE GIZMO ARC SELECTORS 
	// +	BLEND / MATERIAL / TEXTURE UNITS / GEOMETRY SPECS
	// +
	// + BLEND
	if( ISFLAG_ON( ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_BLEND_ON ) )
	{
		if( ISFLAG_ON( ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_BLEND ) )
		{
			NUT_ATxC_GetText_Position1((NVEC2*)&v,&ptooldata->HUDGizmo);
			if(ptooldata->Elements.pBlend)
				NUT_Draw_Text(ptooldata->Elements.pBlend->pName, &v, NULL);
			else
				NUT_Draw_Text(CONSTANT_NUT_EDITOR_ATXC__NOBLEND, &v, NULL);
			
			pcolor = &ptooldata->HUDGizmo.InUseColor;
		}
		else
			pcolor =  &ptooldata->HUDGizmo.ActiveColor;

		// 1) Blend Arc
		Nmem0(&ellipse,NUT_DRAW_ELLIPSE);
		NErrorIf( !NUT_ATxC_GetBlendListSize(ptooldata),NERROR_ARRAY_WRONG_SIZE );
		FLAG_ON(ellipse.Flags,FLAG_NUT_DRAW_ELLIPSE_OPEN);
		ellipse.FromAngle		= 185.0f;
		ellipse.ToAngle			= 265.0f;
		ellipse.SliceNb			= 16;
		ellipse.MarkingOutSize	= 16;//	+../255
		ellipse.MarkingInSize	= 16;// -../255
		ellipse.MarkingNb		= (Nu16)(NUT_ATxC_GetBlendListSize(ptooldata)+1);

		FLAG_ON(ellipse.Flags,FLAG_NUT_DRAW_ELLIPSE_MARKINGS);
		NUT_ATxC_GetDrag_HandleCenter( (NVEC2*)&ellipse.Center, &ptooldata->HUDGizmo );
		NUT_ATxC_GetArcSelector_Extents(&ellipse.Extents, &ptooldata->HUDGizmo);
		ellipse.Color = *pcolor;
		NUT_Draw_Ellipse(&ellipse);

		// 2) Blend selector
		Nmem0(&ellipse,NUT_DRAW_ELLIPSE);
		ellipse.SliceNb		= 8;
		NUT_ATxC_GetArcSelector_HandleExtents(&ellipse.Extents,&ptooldata->HUDGizmo);
		NUT_ATxC_GetArcSelector_BlendHandlePosition( (NVEC2*)&ellipse.Center, &ptooldata->HUDGizmo );
		ellipse.Color = *pcolor;
		NUT_Draw_Ellipse(&ellipse);
	}
	// +
	// + TEXTURE UNIT
	// +
	if( ISFLAG_ON( ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTUREUNIT_ON ) )
	{
		if( ISFLAG_ON( ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTUREUNIT ) )
		{
			NUT_ATxC_GetText_Position1((NVEC2*)&v,&ptooldata->HUDGizmo);
			sprintf(str,"TextureUnit:%d",ptooldata->HUDGizmo.EditedTextureUnit);
			NUT_Draw_Text(str, &v, NULL);
			pcolor = &ptooldata->HUDGizmo.InUseColor;
		}
		else
			pcolor =  &ptooldata->HUDGizmo.ActiveColor;

		// 1) TextureUnit Arc
		Nmem0(&ellipse,NUT_DRAW_ELLIPSE);
		NErrorIf( !NUT_ATxC_GetTextureUnitNumber(&ptooldata->Elements),NERROR_WRONG_VALUE );
		FLAG_ON(ellipse.Flags,FLAG_NUT_DRAW_ELLIPSE_OPEN);
		ellipse.FromAngle		= 5.0f;
		ellipse.ToAngle			= 85.0f;
		ellipse.SliceNb			= 16;
		ellipse.MarkingOutSize	= 16;//	+../255
		ellipse.MarkingInSize	= 16;// -../255
		ellipse.MarkingNb		= (Nu16)(NUT_ATxC_GetTextureUnitNumber(&ptooldata->Elements)+1);

		FLAG_ON(ellipse.Flags,FLAG_NUT_DRAW_ELLIPSE_MARKINGS);
		NUT_ATxC_GetDrag_HandleCenter( (NVEC2*)&ellipse.Center, &ptooldata->HUDGizmo );
		NUT_ATxC_GetArcSelector_Extents(&ellipse.Extents, &ptooldata->HUDGizmo);
		ellipse.Color = *pcolor;
		NUT_Draw_Ellipse(&ellipse);

		// 2) Texture Selector
		Nmem0(&ellipse,NUT_DRAW_ELLIPSE);
		ellipse.SliceNb		= 8;
		NUT_ATxC_GetArcSelector_HandleExtents(&ellipse.Extents,&ptooldata->HUDGizmo);
		NUT_ATxC_GetArcSelector_TextureUnitHandlePosition( (NVEC2*)&ellipse.Center, &ptooldata->HUDGizmo );
		ellipse.Color = *pcolor;
		NUT_Draw_Ellipse(&ellipse);
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + THE GIZMO TEXTURE SELECTOR
	// +
	if( ISFLAG_ON( ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXTURES_ON ) )
	{
		if( ISFLAG_ON( ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXTURE ) )
		{
			NUT_ATxC_GetText_Position1((NVEC2*)&v,&ptooldata->HUDGizmo);
			switch( ptooldata->Elements.MetaTextureType[ptooldata->HUDGizmo.EditedTextureUnit] )
			{
				case NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_NULL:
					NErrorIf(ptooldata->Elements.pMetaTexture[ptooldata->HUDGizmo.EditedTextureUnit], NERROR_NON_NULL_POINTER);
					NUT_Draw_Text(CONSTANT_NUT_EDITOR_ATXC__NOTEXTURE, &v, NULL);
					break;

				case NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_PLAIN:
					NErrorIf(!ptooldata->Elements.pMetaTexture[ptooldata->HUDGizmo.EditedTextureUnit], NERROR_NULL_POINTER);
					NUT_Draw_Text(((NTEXTURE*)ptooldata->Elements.pMetaTexture[ptooldata->HUDGizmo.EditedTextureUnit])->pName, &v, NULL);
					break;

				case NUT_EDITOR_ATXC_ELEMENTS_TEXTURE_TYPE_ATLAS_ELEMENT:
					patlas_el	= (NATLAS_ELEMENT*)ptooldata->Elements.pMetaTexture[ptooldata->HUDGizmo.EditedTextureUnit];
					NErrorIf(!patlas_el, NERROR_NULL_POINTER);
					NErrorIf(!patlas_el->pTexture, NERROR_NULL_POINTER);
					NUT_Draw_Text(patlas_el->pName, &v, NULL);
					break;

				default:
					NErrorIf(1, NERROR_INVALID_CASE);
					NUT_Draw_Text(CONSTANT_NUT_EDITOR_ATXC__NOTEXTURE, &v, NULL);
					break;
			}
			pcolor = &ptooldata->HUDGizmo.InUseColor;
		}
		else
			pcolor =  &ptooldata->HUDGizmo.ActiveColor;

		// 1) Texture Ring
		Nmem0(&ellipse,NUT_DRAW_ELLIPSE);
		NErrorIf( !NUT_ATxC_GetTextureRingSectorNb(ptooldata),NERROR_ARRAY_WRONG_SIZE );
		ellipse.SliceNb			= 64;
		ellipse.MarkingNb	= (Nu16)NUT_ATxC_GetTextureRingSectorNb(ptooldata);
		if(ellipse.MarkingNb)
		{
			FLAG_ON(ellipse.Flags,FLAG_NUT_DRAW_ELLIPSE_MARKINGS);
			ellipse.MarkingOutSize	= 32;//	+../255
			ellipse.MarkingInSize	= 16;// -../255
		}
		
		NUT_ATxC_GetDrag_HandleCenter( (NVEC2*)&ellipse.Center, &ptooldata->HUDGizmo );
		NUT_ATxC_GetTextureRingSelector_Extents(&ellipse.Extents, &ptooldata->HUDGizmo);
		ellipse.Color = *pcolor;
		NUT_Draw_Ellipse(&ellipse);

		// 2) Texture selector
		Nmem0(&ellipse,NUT_DRAW_ELLIPSE);
		ellipse.SliceNb		= 8;
		NUT_ATxC_GetTextureRingSelector_HandleExtents(&ellipse.Extents,&ptooldata->HUDGizmo);
		NUT_ATxC_GetTextureRingSelector_HandlePosition( (NVEC2*)&ellipse.Center, &ptooldata->HUDGizmo );
		ellipse.Color = *pcolor;
		NUT_Draw_Ellipse(&ellipse);
	
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + THE GIZMO TXC OFFSET 
	// +
	if( ISFLAG_ON( ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_OFFSET_ON ) )
	{
		if( ISFLAG_ON( ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_OFFSET ) )
		{
			pcolor = &ptooldata->HUDGizmo.InUseColor;

			sprintf(str,"Offset:%.3f / %.3f",ptooldata->Elements.TexCoordOffset[ptooldata->HUDGizmo.EditedTexCoordSetID].x,ptooldata->Elements.TexCoordOffset[ptooldata->HUDGizmo.EditedTexCoordSetID].y );
			NUT_ATxC_GetText_Position1((NVEC2*)&v,&ptooldata->HUDGizmo);
			NUT_Draw_Text(str, &v, NULL);
		}
		else
			pcolor =  &ptooldata->HUDGizmo.ActiveColor;

		if( ISFLAG_ON(ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORDSET_MOVE_AXIS_RESTRICTION) )
		{
			ellipse.SliceNb		= 4;
			NUT_ATxC_GetTxCOffset_HandlePosition( (NVEC2*)&ellipse.Center, &ptooldata->Elements, ptooldata->pCurrentEntity,&ptooldata->HUDGizmo );
			NUT_ATxC_GetTxCOffset_HandleExtents(&ellipse.Extents, &ptooldata->HUDGizmo);
			ellipse.Color = *pcolor;
			NUT_Draw_Ellipse(&ellipse);
		}
		else
		{
			NUT_ATxC_GetTxCOffset_HandlePosition( (NVEC2*)&center, &ptooldata->Elements, ptooldata->pCurrentEntity,&ptooldata->HUDGizmo );
			NUT_ATxC_GetTxCOffset_HandleExtents(&extents, &ptooldata->HUDGizmo);
			NUT_Draw_Quad(&center,&extents,pcolor);
		}
	}
	
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + THE GIZMO UV RESET
	// +
	if( ISFLAG_ON( ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_RESET_ON ) )
	{
		Nmem0(&ellipse,NUT_DRAW_ELLIPSE);
		ellipse.SliceNb = 4;
		if( ISFLAG_ON( ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_RESET ) )
			ellipse.Color = ptooldata->HUDGizmo.InUseColor;
		else
			ellipse.Color = ptooldata->HUDGizmo.ActiveColor;
		// U
		NUT_ATxC_GetTxCReset_HandleExtents(&ellipse.Extents, &ptooldata->HUDGizmo);
		NUT_ATxC_GetTxCReset_UHandlePosition( (NVEC2*)&ellipse.Center, &ptooldata->Elements, ptooldata->pCurrentEntity,&ptooldata->HUDGizmo );
		NUT_Draw_Ellipse(&ellipse);
		// ... V
		NUT_ATxC_GetTxCReset_HandleExtents(&ellipse.Extents,&ptooldata->HUDGizmo);
		NUT_ATxC_GetTxCReset_VHandlePosition( (NVEC2*)&ellipse.Center,&ptooldata->Elements, ptooldata->pCurrentEntity,&ptooldata->HUDGizmo );
		NUT_Draw_Ellipse(&ellipse);
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + THE GIZMO UV SCALE
	// +
	if( ISFLAG_ON( ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_SCALE_ON ) )
	{
		if( ISFLAG_ON( ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_UPDATING_TEXCOORD_SCALE ) )
		{
			pcolor = &ptooldata->HUDGizmo.InUseColor;
			sprintf(str,"Scale:%.3f / %.3f",ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID].x,ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID].y );
			NUT_ATxC_GetText_Position1((NVEC2*)&v,&ptooldata->HUDGizmo);
			NUT_Draw_Text(str, &v, NULL);
		}
		else
			pcolor =  &ptooldata->HUDGizmo.ActiveColor;

		// 2) The Arrows
		Nmem0(&arrows,NUT_DRAW_ARROWS);
		FLAG_ON(arrows.Flags,FLAG_NUT_DRAW_ARROWS_ABSOLUTE_BARROWSIZE);
		//FLAG_ON(arrows.Flags,FLAG_NUT_DRAW_ARROWS_HIDE_AARROW);
		NUT_SetDrawArrows_ShapeB(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_FORWARD);
		//FLAG_ON(arrows.Flags,FLAG_NUT_DRAW_ARROWS_USEPARAM_BUILDVECTOR);
		//NVec3Set(&arrows.BuildVector,0,0,1);
		arrows.BArrowSize = NUT_ATxC_GetTxCScale_ArrowSize(&ptooldata->HUDGizmo);
		NUT_ATxC_GetTxCOffset_HandlePosition( (NVEC2*)&A, &ptooldata->Elements, ptooldata->pCurrentEntity,&ptooldata->HUDGizmo );	
		arrows.Color = *pcolor;

		// 2a) Gizmo UV.U Arrow(s)
		NUT_ATxC_GetTxCScale_UArrowEnd( (NVEC2*)&B, &ptooldata->Elements,ptooldata->pCurrentEntity,&ptooldata->HUDGizmo );	
		NUT_Draw_Arrows(&A,&B,&arrows);

		// 2b) Gizmo UV.V Arrow(s)
		NUT_ATxC_GetTxCScale_VArrowEnd( (NVEC2*)&B, &ptooldata->Elements,ptooldata->pCurrentEntity,&ptooldata->HUDGizmo );	
		NUT_Draw_Arrows(&A, &B, &arrows);

		// 3) THE UVSPEED SPRING ?
		//		... AND TIMED UPDATE	
		switch(ptooldata->HUDGizmo.State)
		{
			case NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SCALE_USPEED:
				updata.Type		= NUT_EDITOR_ATXC_UPDATA_UPDATE_TXCSCALE;
				updata.TexCoord.Current.x = ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID].x + ptooldata->HUDGizmo.TexCoordScaleSpeed.x*ptime->Nf32_Delay;
				updata.TexCoord.Current.y = ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID].y;

				if( NABS(updata.TexCoord.Current.x) < ptooldata->pCurrentEntity->EditingTxCScale_SpeedSwitchThreshold.x )
				{
					updata.TexCoord.Current.x	= NSIGN(updata.TexCoord.Current.x)*ptooldata->pCurrentEntity->EditingTxCScale_SpeedSwitchThreshold.x;
					ptooldata->HUDGizmo.State	= NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SCALE_U;
					ptooldata->HUDGizmo.TexCoordScaleSpeed.x = 0.0f;
					ptooldata->HUDGizmo.TexCoordScaleSpeed.y = 0.0f;
				}
				updata.TexCoord.Previous		= ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID];
				updata.TexCoord.TxCSet			= ptooldata->HUDGizmo.EditedTexCoordSetID;
// 				updata.TexCoord.pEditedVertexTxC= NULL;
// 				updata.TexCoord.EditedVertexId	= NVOID;

				if( updata.TexCoord.Current.x != updata.TexCoord.Previous.x )
				{
					if( ptooldata->pCurrentEntity->Update_CallBack(pselect,&updata) )
					{
						ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID]	= updata.TexCoord.Current;
					}
				}

				// Draw
				Nmem0(&spring,NUT_DRAW_SPRING);
				FLAG_ON(spring.Flags,FLAG_NUT_DRAW_SPRING_USEPARAM_BUILDVECTOR);
				NVec3Set(&spring.BuildVector,0,0,1);
				spring.Color= *pcolor;
				spring.SpringCoil  = 5;
				spring.Thickness = NUT_ATxC_GetTxCScale_SpeedSpringThickness(&ptooldata->HUDGizmo);
				NUT_ATxC_GetTxCScale_UArrowEnd( (NVEC2*)&spring.A, &ptooldata->Elements,ptooldata->pCurrentEntity,&ptooldata->HUDGizmo );
				NUT_ATxC_GetTxCScale_SpeedU_SpringEnd( (NVEC2*)&spring.B, &ptooldata->Elements, ptooldata->pCurrentEntity,&ptooldata->HUDGizmo );
				NUT_Draw_Spring(&spring);
				break;

			case NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SCALE_VSPEED:
				updata.Type					= NUT_EDITOR_ATXC_UPDATA_UPDATE_TXCSCALE;
				updata.TexCoord.Current.x	= ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID].x;
				updata.TexCoord.Current.y	= ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID].y + ptooldata->HUDGizmo.TexCoordScaleSpeed.y*ptime->Nf32_Delay;

				if( NABS(updata.TexCoord.Current.y) < ptooldata->pCurrentEntity->EditingTxCScale_SpeedSwitchThreshold.y )
				{
					updata.TexCoord.Current.y = NSIGN(updata.TexCoord.Current.y)*ptooldata->pCurrentEntity->EditingTxCScale_SpeedSwitchThreshold.y;

					ptooldata->HUDGizmo.State= NUT_EDITOR_ATXC_GIZMO_STATE_TEXCOORD_SCALE_V;
					ptooldata->HUDGizmo.TexCoordScaleSpeed.x = 0.0f;
					ptooldata->HUDGizmo.TexCoordScaleSpeed.y = 0.0f;
				}

				updata.TexCoord.Previous		= ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID];
				updata.TexCoord.TxCSet			= ptooldata->HUDGizmo.EditedTexCoordSetID;
// 				updata.TexCoord.pEditedVertexTxC= NULL;
// 				updata.TexCoord.EditedVertexId	= NVOID;

				if( updata.TexCoord.Current.y != updata.TexCoord.Previous.y )
				{
					if( ptooldata->pCurrentEntity->Update_CallBack(pselect,&updata) )
					{
						ptooldata->Elements.TexCoordScale[ptooldata->HUDGizmo.EditedTexCoordSetID] = updata.TexCoord.Current;
					}
				}

				// Draw
				Nmem0(&spring,NUT_DRAW_SPRING);
				FLAG_ON(spring.Flags,FLAG_NUT_DRAW_SPRING_USEPARAM_BUILDVECTOR);
				NVec3Set(&spring.BuildVector,0,0,1);
				spring.Color = *pcolor;
				spring.SpringCoil  = 5;
				spring.Thickness = NUT_ATxC_GetTxCScale_SpeedSpringThickness(&ptooldata->HUDGizmo);
				NUT_ATxC_GetTxCScale_VArrowEnd( (NVEC2*)&spring.A, &ptooldata->Elements,ptooldata->pCurrentEntity,&ptooldata->HUDGizmo);
				NUT_ATxC_GetTxCScale_SpeedV_SpringEnd( (NVEC2*)&spring.B, &ptooldata->Elements,ptooldata->pCurrentEntity,&ptooldata->HUDGizmo );
				NUT_Draw_Spring(&spring);
				break;

			default:
				break;
		}
	} // --->  if( ISFLAG_ON( ptooldata->HUDGizmo.Flags,FLAG_NUT_EDITOR_ATXC_GIZMO_TEXCOORD_SCALE_ON ) )

}

void NUT_Editor_ATxC_CheckAndStoreIncomingElementsData(NUT_EDITOR_ATXC_TOOLDATA *ptooldata)
{
	Nu32	i;
	Nf32	f0,f;
	// NUT_EDITOR_ATXC_ELEMENTS contains 8 members.
	//
	// Nothing to check with the first 3 members of NUT_EDITOR_ATXC_ELEMENTS structure. 
	// 			|
	// 		1	| Nu32					Flags;
	// 		2	| Nu32					GeometryIDS;								
	//		3	| NAPPEARANCE_RAW		Appearance;									
	// 			|
	// They can be used like they are, without any special treatment...
	// So, they can be used in their default/initial state, with values set by NUT_Editor_ATxC_SetupElements
	// or, used with the values set by hand by user in the NUT_EDITOR_ATXC_GET_ELEMENTS_CALLBACK callback function.
	//
	// Things are a little bit different with the 'UsedOAARectf32[]'member. 
	// 			|
	//		4	| NRECTf32				UsedOAARectf32[CONSTANT_NUT_EDITOR_ATXC_ELEMENTS_TEXCOORD_SET_NB];
	// 			|
	// N has to set up the associated Building TexCoord Matrix to Identity of each UsedOAARectf32.
	// If a UsedOAARectf32 is in its default/initial state with values set by NUT_Editor_ATxC_SetupElements
	// An Identity TexCoord Matrix is going to be set up.
	// A more specific one is going to be build, if user set the UsedOAARectf32 by hand.
	// All this matrix building is made by "NGetOAARectf32_TexCoords_Building2DMatrix3x3"
	//
/*
	for(i=0;i<CONSTANT_NUT_EDITOR_ATXC_ELEMENTS_TEXCOORD_SET_NB;i++)
	{
		NGetOAARectf32_TexCoords_Building2DMatrix3x3(&ptooldata->HUDGizmo.BuildTxC_Matrix[i],&ptooldata->Elements.UsedOAARectf32[i],0);
	}
*/
	// 'TexCoordOffset' member has to be check too, but before that, is going to be copied as 'InitialTexCoordOffset' for a potential RESET.
	// This check consists to constraint used value between 0 and 1 it's not a CLAMP, its a MODULO.
	//			|
	// 		5	|NTEXCOORD_2f			TexCoordOffset[CONSTANT_NUT_EDITOR_ATXC_ELEMENTS_TEXCOORD_SET_NB];
	//			|
	// 'TexCoordScale' member has to be check like 'TexCoordOffset', and copied as 'InitialTexCoordScale' for a potential RESET.
	// But the check is performed ONLY if User set to ON. Notice that the check is a CLAMP.
	//			|
	// 		6	|NTEXCOORD_2f			TexCoordScale[CONSTANT_NUT_EDITOR_ATXC_ELEMENTS_TEXCOORD_SET_NB];// |
	//			|
	f = ptooldata->pCurrentEntity->EditingTxCOffset_Max - ptooldata->pCurrentEntity->EditingTxCOffset_Min;
	for(i=0;i<CONSTANT_NUT_EDITOR_ATXC_ELEMENTS_TEXCOORD_SET_NB;i++)
	{
		// Store Initial Element Data for potential Reset (possible only if FLAG_NUT_EDITOR_ATXC_ELEMENTS_EDIT_TEXCOORD_RESET_xxx is/are
		// set to ON.
		ptooldata->InitialTexCoordOffset[i] = ptooldata->Elements.TexCoordOffset[i];
		ptooldata->InitialTexCoordScale[i]  = ptooldata->Elements.TexCoordScale[i];
		
		// In This Tool: ENTITY.EditingTxCOffset_Min < TexCoordOffset < ENTITY.EditingTxCOffset_Max
		// WAY #1: Modulo ... 
		f0 = ptooldata->Elements.TexCoordOffset[i].x - ptooldata->pCurrentEntity->EditingTxCOffset_Min;
		ptooldata->Elements.TexCoordOffset[i].x = ptooldata->pCurrentEntity->EditingTxCOffset_Min + (f0 - floor(f0/f)*f);
		
		f0 = ptooldata->Elements.TexCoordOffset[i].y - ptooldata->pCurrentEntity->EditingTxCOffset_Min;
		ptooldata->Elements.TexCoordOffset[i].y = ptooldata->pCurrentEntity->EditingTxCOffset_Min + (f0 - floor(f0/f)*f);
		
		// WAY #2: Clamp ... 
// 		ptooldata->Elements.TexCoordOffset[i].x = NCLAMP(ptooldata->pCurrentEntity->EditingTxCOffset_Min<ptooldata->Elements.TexCoordOffset[i].x<ptooldata->pCurrentEntity->EditingTxCOffset_Max);
// 		ptooldata->Elements.TexCoordOffset[i].y = NCLAMP(ptooldata->pCurrentEntity->EditingTxCOffset_Min<ptooldata->Elements.TexCoordOffset[i].y<ptooldata->pCurrentEntity->EditingTxCOffset_Max);

	}

	// Nothing to check with the last 2 members of NUT_EDITOR_ATXC_ELEMENTS structure. 
	// 			|
	// 		7	| void					*pFirstVertex;
	// 		8	| Nu32					VertexNumber;
	// 			|
	// They can be used like they are, without any special treatment...
	// So, they can be used in their default/initial state, with values set by NUT_Editor_ATxC_SetupElements
	// or, used with the values set by hand by user in the NUT_EDITOR_ATXC_GET_ELEMENTS_CALLBACK callback function.
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR

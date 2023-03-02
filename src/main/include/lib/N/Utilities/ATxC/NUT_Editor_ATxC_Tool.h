#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef __NUT_EDITOR_TEXTUREANDUV_TOOL_H
#define __NUT_EDITOR_TEXTUREANDUV_TOOL_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NUT_Editor_ATxC_Tool.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../NType.h"
#include "../../Maths/NVec3f32.h"
#include "../../NEditor.h"
#include "../../Containers/NArray.h"
#include "NUT_Editor_ATxC_Gizmo.h"
#include "NUT_Editor_ATxC_Elements.h"
#include "NUT_Editor_ATxC_Entity.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************
// **																																							**
// **														GLOBAL and GENERIC DEFINEs																			**
// **																																							**
// ***************************************************************************************************************************************************************
// ***************************************************************************************************************************************************************
// + CONSTANTS VALUES
#define CONSTANT_NUT_ATXC_TEXTURELIBRARY						((NARRAY*)NVOID)// Used instead of a valid array pointer to use the whole texture library.
#define CONSTANT_NUT_ATXC_ATLAS									((NARRAY*)NVOID)// Used instead of a valid array pointer to use the whole Atlas.
#define CONSTANT_NUT_EDITOR_ATXC_ENTITY_ARRAY_INITIAL_CAPACITY	1				// At NUT_EDITOR_ATXC_TOOLDATA setup, to initialize the included Array of TxC entity

typedef struct  
{
	NUT_EDITOR_ATXC_GIZMO		HUDGizmo;
	NUT_EDITOR_ATXC_ELEMENTS	Elements;
	
	NUT_EDITOR_ATXC_ENTITY		*pCurrentEntity;
	NARRAY						EntityArray;
	
	NVEC2					Normalized_PreviousTouch;
	NVEC2					Normalized_TouchStart;
	// UVs
	NTEXCOORD_2f				InitialTexCoordOffset[CONSTANT_NUT_EDITOR_ATXC_ELEMENTS_TEXCOORD_SET_NB];
	NTEXCOORD_2f				InitialTexCoordScale[CONSTANT_NUT_EDITOR_ATXC_ELEMENTS_TEXCOORD_SET_NB];
}NUT_EDITOR_ATXC_TOOLDATA;


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									 												**
// **									---												**
// **							     Functions											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
// LOW LEVEL FUNCTIONS
// NUT_EDITOR_ATXC_GIZMO*		NUT_Editor_SetUp_ATxC_Gizmo(NUT_EDITOR_ATXC_GIZMO *pgizmo, const NUT_EDITOR_ATXC_GIZMO_DESC *pdesc );
// void						NUT_Editor_Clear_ATxC_Gizmo(void *pgizmo);

void						NUT_Editor_ATxC_Tool_Start(const NEDITOR_SELECTION *pselection,const Nu32 tool_param );
void						NUT_Editor_ATxC_Tool_Stop(const NEDITOR_SELECTION *pselection,const Nu32 tool_param );
NEDITOR_RESULT				NUT_Editor_ATxC_Tool(const NEVENT *pevent,NEDITOR_SELECTION *pselect,const Nu32 tool_param);
void						NUT_Editor_ATxC_Tool_TimeHandle(const NTIME* ptime,const NEDITOR_SELECTION *pselection,const Nu32 tool_param );

// USER FUNCTIONS
// The common right way to use ATxC tool is:
//	1)	Create or Setup a  NUT_EDITOR_ATXC_TOOLDATA structure with its  NUT_EDITOR_ATXC_GIZMO_DESC parameter.
//		You can use NULL as NUT_EDITOR_ATXC_GIZMO_DESC, in that way all the default values are going to be used.
//		Or, you can use a NUT_EDITOR_ATXC_GIZMO_DESC to only setup values you want to personalize (and the associated FLAG).
//	2)	Register the Tool (maybe trough a button) by using the "NUT_Editor_Register_ATxC_Tool" function which performs all the right setup
//		for you.
//	3)	Bind all the entities you want to edit with this ATxC Tool.

NUT_EDITOR_ATXC_TOOLDATA*	NUT_Editor_Setup_ATxC_ToolData(NUT_EDITOR_ATXC_TOOLDATA *ptooldata, const NUT_EDITOR_ATXC_GIZMO_DESC *pdesc );
NUT_EDITOR_ATXC_TOOLDATA*	NUT_Editor_Create_ATxC_ToolData(const NUT_EDITOR_ATXC_GIZMO_DESC *pgizmodesc );
void						NUT_Editor_Clear_ATxC_ToolData(NUT_EDITOR_ATXC_TOOLDATA *ptooldata);
void						NUT_Editor_Delete_ATxC_ToolData(NUT_EDITOR_ATXC_TOOLDATA *ptooldata);


inline Nu32					NUT_Editor_Register_ATxC_Tool( NUT_EDITOR_ATXC_TOOLDATA *ptooldata )
{
	return NRegisterSingleTouchTool(	NUT_Editor_ATxC_Tool_Start,
										NUT_Editor_ATxC_Tool_Stop,
										NUT_Editor_ATxC_Tool,
										NUT_Editor_ATxC_Tool_TimeHandle,
										(Nu32)ptooldata );
}

Nu32						NUT_Editor_ATxC_Tool_BindEntity(NUT_EDITOR_ATXC_TOOLDATA *ptooldata, const NUT_EDITOR_ATXC_ENTITY_DESC *pdesc);
void						NUT_Editor_ATxC_Tool_UnbindEntity(NUT_EDITOR_ATXC_TOOLDATA *ptooldata, Nu32	id);

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + PRIVATE
// +
void		NUT_Editor_ATxC_CheckAndStoreIncomingElementsData(NUT_EDITOR_ATXC_TOOLDATA *ptooldata);
// GIZMO
void		NUT_ATxC_ActivateGizmo(NUT_EDITOR_ATXC_TOOLDATA *ptooldata,const NVEC2 *pposition);
void		NUT_ATxC_UnactivateGizmo(NUT_EDITOR_ATXC_TOOLDATA *ptooldata);
Nu32		NUT_ATxC_PickToolDataGizmoArcSelector(const NUT_EDITOR_ATXC_TOOLDATA *ptooldata, const NVEC2 *pnorm_mouse);

// TEXTURE
void		NUT_ATxC_TextureRingActivation( NUT_EDITOR_ATXC_TOOLDATA *ptooldata );
void		NUT_ATxC_TextureParametersActivation( NUT_EDITOR_ATXC_TOOLDATA *ptooldata );
Nbool		NUT_ATxC_PickTextureRing(const NUT_EDITOR_ATXC_TOOLDATA *ptooldata, const NVEC2 *pnorm_mouse);
Nu32		NUT_ATxC_GetTextureRingSectorNb(const NUT_EDITOR_ATXC_TOOLDATA *ptooldata);
Nu32		NUT_ATxC_GetTextureRingSectorIndex(const NUT_EDITOR_ATXC_TOOLDATA *ptooldata);
void*		NUT_ATxC_GetTextureRingSectorMetaTexture(Nu8 *pmetatexturetype, const NUT_EDITOR_ATXC_TOOLDATA *ptooldata, const Nu32 sectorindex);
void*		NUT_ATxC_GetCurrentMetaTexture(Nu8 *pmetatexturetype, const NUT_EDITOR_ATXC_TOOLDATA *ptooldata );
Nu8			NUT_ATxC_GetCurrentMetaTexturePtrs(NATLAS_ELEMENT **ppmetatexture_atlaselement, NTEXTURE **ppmetatexture_texture, const NUT_EDITOR_ATXC_TOOLDATA *ptooldata );

void		NUT_ATxC_SetTextureHandlePosition(NUT_EDITOR_ATXC_TOOLDATA *ptooldata, const Nu32 sectorindex);
void		NUT_ATxC_SetUndefinedTextureHandlePosition(NUT_EDITOR_ATXC_TOOLDATA *ptooldata);

// TEXTURE UNIT
void		NUT_ATxC_SetTextureUnitHandlePosition(NUT_EDITOR_ATXC_TOOLDATA *ptooldata);

// BLEND
Nu32		NUT_ATxC_GetBlendListSize(const NUT_EDITOR_ATXC_TOOLDATA *ptooldata);
Nu32		NUT_ATxC_GetBlendIndex(const NUT_EDITOR_ATXC_TOOLDATA *ptooldata, const NBLEND *pblend);
NBLEND*		NUT_ATxC_GetBlendIndexPtr(const NUT_EDITOR_ATXC_TOOLDATA *ptooldata, const Nu32 blendindex);
void		NUT_ATxC_SetBlendHandlePosition(NUT_EDITOR_ATXC_TOOLDATA *ptooldata, const Nu32 blendindex);


// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NUT_EDITOR_TEXTUREANDUV_TOOL_H

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR


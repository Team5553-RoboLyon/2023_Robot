#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef __NUT_EDITOR_ATXC_GIZMO_TEXTURE_WRAP_INLINE_H
#define __NUT_EDITOR_ATXC_GIZMO_TEXTURE_WRAP_INLINE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **					 NUT_Editor_ATxC_Gizmo_TextureWrap_InLine.h						**
// **																					**
// **						Should be used ONLY by NENGINE								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../NType.h"
#include "../../Maths/NVec3f32.h"
#include "NUT_Editor_ATxC_Entity.h"
#include "NUT_Editor_ATxC_Elements.h"
#include "NUT_Editor_ATxC_Gizmo.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// +
// +	Texture WRAP_T
// +
// +
inline void NUT_ATxC_GetWrap_T_HandleCenter(NVEC2 *prectcenter, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	prectcenter->x		=	pgizmo->Position.x + pgizmo->DragHandleSize.x*0.5f;
	prectcenter->y		=	pgizmo->Position.y + pgizmo->DragHandleSize.y + pgizmo->HandleSize.y*0.5f;
}

inline void NUT_ATxC_GetWrap_T_HandleExtents(NVEC2 *pextents, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	pextents->x = pgizmo->DragHandleSize.x*0.5f;
	pextents->y = pgizmo->HandleSize.y*0.5f;
}

inline void NUT_ATxC_GetWrap_T_HandleRect(NRECTf32 *prect, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	prect->left		= pgizmo->Position.x;
	prect->right	= pgizmo->Position.x + pgizmo->DragHandleSize.x;
	prect->bottom	= pgizmo->Position.y + pgizmo->DragHandleSize.y;
	prect->top		= prect->bottom + pgizmo->HandleSize.y;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// +
// +	Texture WRAP_S
// +
// +
inline void NUT_ATxC_GetWrap_S_HandleCenter(NVEC2 *prectcenter, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	prectcenter->x		=	pgizmo->Position.x - pgizmo->HandleSize.x*0.5f;
	prectcenter->y		=	pgizmo->Position.y + pgizmo->DragHandleSize.y*0.5f;
}

inline void NUT_ATxC_GetWrap_S_HandleExtents(NVEC2 *pextents, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	pextents->x = pgizmo->HandleSize.x*0.5f;
	pextents->y = pgizmo->DragHandleSize.y*0.5f;
}

inline void NUT_ATxC_GetWrap_S_HandleRect(NRECTf32 *prect, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	prect->left		= pgizmo->Position.x - pgizmo->HandleSize.x;
	prect->right	= pgizmo->Position.x;
	prect->bottom	= pgizmo->Position.y;
	prect->top		= pgizmo->Position.y + pgizmo->DragHandleSize.y;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NUT_EDITOR_ATXC_GIZMO_TEXTURE_WRAP_INLINE_H

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR


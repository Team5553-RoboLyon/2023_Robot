#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef __NUT_EDITOR_ATXC_GIZMO_TEXTURE_FILTER_INLINE_H
#define __NUT_EDITOR_ATXC_GIZMO_TEXTURE_FILTER_INLINE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **					 NUT_Editor_ATxC_Gizmo_TextureFilter_InLine.h					**
// **																					**
// **						Should be used ONLY by NENGINE								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/Maths/NVec3f32.h"
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
// +	Texture MIN_FILTER
// +
// +
inline void NUT_ATxC_GetMinFilter_HandleCenter(NVEC2 *prectcenter, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	prectcenter->x		=	pgizmo->Position.x + pgizmo->DragHandleSize.x - pgizmo->HandleSize.x*0.5f;
	prectcenter->y		=	pgizmo->Position.y - pgizmo->HandleSize.y*0.5f;
}

inline void NUT_ATxC_GetMinFilter_HandleExtents(NVEC2 *pextents, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	pextents->x = pgizmo->HandleSize.x*0.5f;
	pextents->y = pgizmo->HandleSize.y*0.5f;
}

inline void NUT_ATxC_GetMinFilter_HandleRect(NRECTf32 *prect, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	prect->right	= pgizmo->Position.x + pgizmo->DragHandleSize.x;
	prect->left		= prect->right - pgizmo->HandleSize.x;
	prect->bottom	= pgizmo->Position.y - pgizmo->HandleSize.y;
	prect->top		= pgizmo->Position.y;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// +
// +	Texture MAG_FILTER
// +
// +
inline void NUT_ATxC_GetMagFilter_HandleCenter(NVEC2 *prectcenter, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	prectcenter->x		=	pgizmo->Position.x + pgizmo->DragHandleSize.x + pgizmo->HandleSize.x*0.5f;
	prectcenter->y		=	pgizmo->Position.y + pgizmo->HandleSize.y*0.5f;
}

inline void NUT_ATxC_GetMagFilter_HandleExtents(NVEC2 *pextents, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	pextents->x = pgizmo->HandleSize.x*0.5f;
	pextents->y = pgizmo->HandleSize.y*0.5f;
}

inline void NUT_ATxC_GetMagFilter_HandleRect(NRECTf32 *prect, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	prect->left		= pgizmo->Position.x + pgizmo->DragHandleSize.x;
	prect->right	= prect->left + pgizmo->HandleSize.x;
	prect->bottom	= pgizmo->Position.y;
	prect->top		= pgizmo->Position.y + pgizmo->HandleSize.y;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NUT_EDITOR_ATXC_GIZMO_TEXTURE_FILTER_INLINE_H

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR


#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef __NUT_EDITOR_ATXC_GIZMO_TEXT_INLINE_H
#define __NUT_EDITOR_ATXC_GIZMO_TEXT_INLINE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NUT_Editor_ATxC_Gizmo_Text_InLine.h						**
// **																					**
// **						Should be used ONLY by NENGINE								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/Maths/NVec3f32.h"
#include "NUT_Editor_ATxC_Gizmo_Drag_InLine.h"
#include "NUT_Editor_ATxC_Gizmo.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

inline void NUT_ATxC_GetText_Position1(NVEC2 *pposition, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	pposition->x = pgizmo->Position.x;
	pposition->y = pgizmo->Position.y-pgizmo->DragHandleSize.y;
}

inline void NUT_ATxC_GetText_Position2(NVEC2 *pposition, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	pposition->x = pgizmo->Position.x;
	pposition->y = pgizmo->Position.y - 0.25f*pgizmo->DragHandleSize.y;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NUT_EDITOR_ATXC_GIZMO_TEXT_INLINE_H

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //__NUT_EDITOR_ATXC_PRIVATE_BLEND_H


#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef __NUT_EDITOR_ATXC_GIZMO_MATERIAL_INLINE_H
#define __NUT_EDITOR_ATXC_GIZMO_MATERIAL_INLINE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **						NUT_Editor_ATxC_Gizmo_Material_InLine.h						**
// **																					**
// **						Should be used ONLY by NENGINE								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../NType.h"
#include "../../Maths/NVec3f32.h"
#include "NUT_Editor_ATxC_Gizmo.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

inline void NUT_ATxC_GetArcSelector_MaterialHandlePosition( NVEC2 *pposition, const NUT_EDITOR_ATXC_GIZMO *pgizmo )
{
	NUT_ATxC_GetDrag_HandleCenter( pposition, pgizmo );
	NVec2AddTo( pposition, &pgizmo->MaterialHandlePosition) ;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NUT_EDITOR_ATXC_GIZMO_MATERIAL_INLINE_H

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR


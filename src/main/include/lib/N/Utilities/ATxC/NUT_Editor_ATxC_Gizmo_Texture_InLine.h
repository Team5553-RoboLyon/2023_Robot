#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef __NUT_EDITOR_ATXC_GIZMO_TEXTURE_INLINE_H
#define __NUT_EDITOR_ATXC_GIZMO_TEXTURE_INLINE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **						NUT_Editor_ATxC_Gizmo_Texture_InLine.h						**
// **																					**
// **						Should be used ONLY by NENGINE								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/Maths/NVec3f32.h"
#include "NUT_Editor_ATxC_Gizmo.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// TextureCircleSelector
// center:	is GIZMO Center
// Extents:
static inline void NUT_ATxC_GetTextureRingSelector_Extents(NVEC2 *pextents, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	pextents->x = pextents->y = 0.5f*NVec2FastLength(&pgizmo->DragHandleSize)+NVec2FastLength(&pgizmo->HandleSize);
}
// Handle Extents
static inline void NUT_ATxC_GetTextureRingSelector_HandleExtents(NVEC2 *pextents, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	pextents->x = pextents->y = 0.25f*NVec2FastLength(&pgizmo->HandleSize);
}
// Handle Position
static inline void NUT_ATxC_GetTextureRingSelector_HandlePosition( NVEC2 *pposition, const NUT_EDITOR_ATXC_GIZMO *pgizmo )
{
	NUT_ATxC_GetDrag_HandleCenter( pposition, pgizmo );
	NVec2AddTo( pposition, &pgizmo->TextureHandlePosition) ;
}

inline Nu32 NUT_ATxC_GetSectorIndexFromTextureRingSelector(const Nf32 angle_deg, const Nu32 sectornb )
{
	// Retrieve Index from Angle
	NErrorIf(!sectornb,NERROR_NULL_VALUE);
	return (Nu32)(((Nf32)sectornb*angle_deg)/360.0f)%sectornb;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NUT_EDITOR_ATXC_GIZMO_TEXTURE_INLINE_H

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR


#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef __NUT_EDITOR_ATXC_GIZMO_DRAG_INLINE_H
#define __NUT_EDITOR_ATXC_GIZMO_DRAG_INLINE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **					 NUT_Editor_ATxC_Gizmo_Drag_InLine.h							**
// **																					**
// **						Should be used ONLY by NENGINE								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/NFlags.h"
#include "lib/N/Maths/NVec3f32.h"
#include "lib/N/Miscellaneous/NColor.h"
#include "NUT_Editor_ATxC_Gizmo.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

inline void NUT_ATxC_GetDrag_HandleCenter(NVEC2 *prectcenter, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	prectcenter->x	= pgizmo->Position.x + pgizmo->DragHandleSize.x*0.5f;
	prectcenter->y	= pgizmo->Position.y + pgizmo->DragHandleSize.y*0.5f;
}

// GIZMO MAIN RECT:
// It has several uses:	
//			It serves as GIZMO HANDLE to drag and move GIZMO.
//			It surrounds UV Origin moves area.
inline void NUT_ATxC_GetDrag_HandleRect(NRECTf32 *prect, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	// The GIZMO main rect represents all the possible values for u and v origin uv.
	// these values are all between 0.0f and 1.0f, for u and v. It's not necessary to authorize bigger and/or smaller value,
	// because of Scale, u = 1.5f is equivalent to u = 0.5f ... so...
	// Is also used as an handle to drag and move the entire GIzmo on screen
	prect->left		= pgizmo->Position.x;
	prect->right	= pgizmo->Position.x + pgizmo->DragHandleSize.x;
	prect->bottom	= pgizmo->Position.y;
	prect->top		= pgizmo->Position.y + pgizmo->DragHandleSize.y;
}
inline void NUT_ATxC_GetDrag_HandleExtents(NVEC2 *pextents, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	pextents->x = pgizmo->DragHandleSize.x*0.5f;
	pextents->y = pgizmo->DragHandleSize.y*0.5f;
}

// ARC SELECTORS (Blend/Material/TextureUnit/Compatible Geometry Specs)
inline void NUT_ATxC_GetArcSelector_Extents(NVEC2 *pextents, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	// infos: pextents->x = pextents->y = 1.25f*NUT_ATxC_GetTextureRingSelector_Extents;
	pextents->x = pextents->y = 1.25f*(0.5f*NVec2FastLength(&pgizmo->DragHandleSize)+NVec2FastLength(&pgizmo->HandleSize));
}

inline void NUT_ATxC_GetArcSelector_HandleExtents(NVEC2 *pextents, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	pextents->x = pextents->y = 0.25f*NVec2FastLength(&pgizmo->HandleSize);
}



inline Nu32 NUT_ATxC_GetSectorIndexFromArcSelector(const Nf32 angle_deg, const Nu32 sectornb, const Nu32 arc_id  )
{
	// Retrieve Texture Ptr from Angle
	NErrorIf(CONSTANT_NUT_ATXC_ARC_START>=45.0f,NERROR_SYSTEM_NENGINEVERSION_CONFLICT); 
	NErrorIf(!sectornb,NERROR_NULL_VALUE);
	NErrorIf(arc_id < 1 || arc_id > 4 ,NERROR_WRONG_VALUE);// test OUTOFRANGE
	
	// 1/ Angle from Arc start ...
	Nf32 alpha = angle_deg - ( (arc_id-1)*90.0f + CONSTANT_NUT_ATXC_ARC_START ); 
	if( alpha > 0 && alpha < CONSTANT_NUT_ATXC_ARC_LENGTH )
	{
		return (Nu32)(((Nf32)sectornb*alpha)/CONSTANT_NUT_ATXC_ARC_LENGTH);
	}
	else
	{
		alpha -= CONSTANT_NUT_ATXC_ARC_LENGTH/2.0f;
		if(alpha>0)
			return sectornb-1;
		else
			return 0;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NUT_EDITOR_ATXC_GIZMO_DRAG_INLINE_H

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR


#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef __NUT_EDITOR_ATXC_GIZMO_TEXTUREUNIT_INLINE_H
#define __NUT_EDITOR_ATXC_GIZMO_TEXTUREUNIT_INLINE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **						NUT_Editor_ATxC_Gizmo_TextureUnit_InLine.h					**
// **																					**
// **						Should be used ONLY by NENGINE								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/Maths/NVec3f32.h"
#include "lib/N/Geometry/NGeometry.h"
#include "NUT_Editor_ATxC_Gizmo_Drag_InLine.h"
#include "NUT_Editor_ATxC_Elements.h"
#include "NUT_Editor_ATxC_Gizmo.h"

#ifdef __cplusplus
extern "C"
{
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

inline void NUT_ATxC_GetArcSelector_TextureUnitHandlePosition( NVEC2 *pposition, const NUT_EDITOR_ATXC_GIZMO *pgizmo )
{
	NUT_ATxC_GetDrag_HandleCenter( pposition, pgizmo );
	NVec2AddTo( pposition, &pgizmo->TextureUnitHandlePosition) ;
}

inline Nu8 NUT_ATxC_GetTextureUnitNumber( const NUT_EDITOR_ATXC_ELEMENTS *pelements)
{
	NErrorIf(!pelements, NERROR_NULL_POINTER);
	return NGetGeometrySpecs(NGET_GEOMETRY_IDS_GEOMETRY_FORMAT(pelements->GeometryIDS))->UsedTextureUnitNumber;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NUT_EDITOR_ATXC_GIZMO_TEXTUREUNIT_INLINE_H

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR


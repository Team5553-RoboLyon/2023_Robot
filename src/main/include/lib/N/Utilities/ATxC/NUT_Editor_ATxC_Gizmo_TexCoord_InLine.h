#ifdef _NEDITOR
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef __NUT_EDITOR_ATXC_GIZMO_UV_INLINE_H
#define __NUT_EDITOR_ATXC_GIZMO_UV_INLINE_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **						 NUT_Editor_ATxC_Gizmo_UV_InLine.h							**
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
// +	TexCoord Offset
// +
// +
inline void NUT_ATxC_GetTxCOffset_HandlePosition(NVEC2 *prectcenter, const NUT_EDITOR_ATXC_ELEMENTS *pelements, const NUT_EDITOR_ATXC_ENTITY *pentity, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	// "NUT_EDITOR_ATXC_ELEMENTS.TexCoordOffset" is relative to Current Used OAA Rect.
	// Here it's coordinates are always "Entity.EditingTxCOffset_Min < coord < Entity.EditingTxCOffset_Max ".
	// Notice subtlety for y... it's because Render Screen is vertically well oriented FROM bottom to TOP,
	// and TexCoord 'WORLD' is "conventional Weird" oriented FROM TOP to BOTTOM.
	// So this little "subtlety" to convert and orient rendered representation in the weird conventional way...
	Nf32 f = 1.0f/(pentity->EditingTxCOffset_Max - pentity->EditingTxCOffset_Min);
	prectcenter->x		=	pgizmo->Position.x + 
							(pelements->TexCoordOffset[pgizmo->EditedTexCoordSetID].x-pentity->EditingTxCOffset_Min)*f*pgizmo->DragHandleSize.x;

	prectcenter->y		=	pgizmo->Position.y +
							(1.0f-(pelements->TexCoordOffset[pgizmo->EditedTexCoordSetID].y-pentity->EditingTxCOffset_Min)*f)*pgizmo->DragHandleSize.y;
}

inline void NUT_ATxC_GetTxCOffset_HandleExtents(NVEC2 *pextents, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	NVec2Scale(pextents,&pgizmo->HandleSize,0.5f);
}

inline void NUT_ATxC_GetTxCOffset_HandleRect(NRECTf32 *prect, const NUT_EDITOR_ATXC_ELEMENTS *pelements, const NUT_EDITOR_ATXC_ENTITY *pentity, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	NVEC2	v0,vex;

	NUT_ATxC_GetTxCOffset_HandlePosition(&v0,pelements,pentity,pgizmo);
	NUT_ATxC_GetTxCOffset_HandleExtents(&vex,pgizmo);
	prect->left		= v0.x - vex.x;
	prect->right	= v0.x + vex.x;
	prect->bottom	= v0.y - vex.y;
	prect->top		= v0.y + vex.y;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// +
// +	TEXCOORD SCALE ARROW
// +
// +
inline Nf32 NUT_ATxC_GetTxCScale_ArrowSize(const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	return  NMIN(pgizmo->HandleSize.x,pgizmo->HandleSize.y);
}

/*
// unused 
inline void NUT_ATxC_GetUVScale_ArrowExtents(NVEC2 *pextents, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	pextents->x = pextents->y =  NUT_ATxC_GetTxCScale_ArrowSize(pgizmo)*0.5f;
}
*/
// Position of the Arrow extremity ...
inline void NUT_ATxC_GetTxCScale_UArrowEnd(NVEC2 *ppoint, const NUT_EDITOR_ATXC_ELEMENTS *pelements, const NUT_EDITOR_ATXC_ENTITY *pentity, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	// These 2 coordinates are calculating in the same way than 'TxCOffset_HandlePosition'
	NUT_ATxC_GetTxCOffset_HandlePosition(ppoint,pelements,pentity,pgizmo);
	ppoint->x +=  pgizmo->DragHandleSize.x * pentity->EditingTxCScale_BaseScale.x * NCLAMP(-pentity->EditingTxCScale_SpeedSwitchThreshold.x,pelements->TexCoordScale[pgizmo->EditedTexCoordSetID].x, pentity->EditingTxCScale_SpeedSwitchThreshold.x )
				+ NSIGN(pelements->TexCoordScale[pgizmo->EditedTexCoordSetID].x)*NUT_ATxC_GetTxCScale_ArrowSize(pgizmo);
}
// Center of the Arrow Head (= Center of the Arrow head surrounded rectangle)
inline void NUT_ATxC_GetTxCScale_UArrowHeadCenter(NVEC2 *prectcenter, const NUT_EDITOR_ATXC_ELEMENTS *pelements, const NUT_EDITOR_ATXC_ENTITY *pentity, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	NUT_ATxC_GetTxCScale_UArrowEnd(prectcenter,pelements,pentity,pgizmo);
	prectcenter->x	-=  NSIGN(pelements->TexCoordScale[pgizmo->EditedTexCoordSetID].x)*NUT_ATxC_GetTxCScale_ArrowSize(pgizmo)*0.5f;
}
// Surrounded UArrow Head Rectangle
inline void NUT_ATxC_GetTxCScale_UArrowHeadRect(NRECTf32 *prect, const NUT_EDITOR_ATXC_ELEMENTS *pelements, const NUT_EDITOR_ATXC_ENTITY *pentity, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	NVEC2	_o;
	NUT_ATxC_GetTxCScale_UArrowHeadCenter(&_o,pelements,pentity,pgizmo);
	Nf32		_halfsize	= NUT_ATxC_GetTxCScale_ArrowSize(pgizmo)*0.5f;

	prect->left		= _o.x - _halfsize;
	prect->right	= _o.x + _halfsize;
	prect->bottom	= _o.y - _halfsize;
	prect->top		= _o.y + _halfsize;
}
// Position of the Arrow extremity ...
inline void NUT_ATxC_GetTxCScale_VArrowEnd(NVEC2 *ppoint, const NUT_EDITOR_ATXC_ELEMENTS *pelements, const NUT_EDITOR_ATXC_ENTITY *pentity, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	// These 2 coordinates are calculating in the same way than 'TxCOffset_HandlePosition'
	NUT_ATxC_GetTxCOffset_HandlePosition(ppoint,pelements,pentity,pgizmo);
	ppoint->y -=  pgizmo->DragHandleSize.y * pentity->EditingTxCScale_BaseScale.y * NCLAMP(-pentity->EditingTxCScale_SpeedSwitchThreshold.y,pelements->TexCoordScale[pgizmo->EditedTexCoordSetID].y,pentity->EditingTxCScale_SpeedSwitchThreshold.y )
				+ NSIGN(pelements->TexCoordScale[pgizmo->EditedTexCoordSetID].y)*NUT_ATxC_GetTxCScale_ArrowSize(pgizmo);
}

inline void NUT_ATxC_GetTxCScale_VArrowHeadCenter(NVEC2 *prectcenter, const NUT_EDITOR_ATXC_ELEMENTS *pelements, const NUT_EDITOR_ATXC_ENTITY *pentity, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	NUT_ATxC_GetTxCScale_VArrowEnd(prectcenter,pelements,pentity,pgizmo);
	prectcenter->y	+=  NSIGN(pelements->TexCoordScale[pgizmo->EditedTexCoordSetID].y)*NUT_ATxC_GetTxCScale_ArrowSize(pgizmo)*0.5f;
	//				+ and not - because of the TexCoord  Conventional Weird Vertical orientation from TOP to BOTTOM
}

inline void NUT_ATxC_GetTxcScale_VArrowHeadRect(NRECTf32 *prect, const NUT_EDITOR_ATXC_ELEMENTS *pelements, const NUT_EDITOR_ATXC_ENTITY *pentity, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	NVEC2	_o;
	NUT_ATxC_GetTxCScale_VArrowHeadCenter(&_o,pelements,pentity,pgizmo);
	Nf32		_halfsize	= NUT_ATxC_GetTxCScale_ArrowSize(pgizmo)*0.5f;

	prect->left		= _o.x - _halfsize;
	prect->right	= _o.x + _halfsize;
	prect->bottom	= _o.y - _halfsize;
	prect->top		= _o.y + _halfsize;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// +
// +	TEXCOORD SCALE SPEED
// +
// +
inline Nf32 NUT_ATxC_GetTxCScale_SpeedSpringThickness(const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	return  NMIN(pgizmo->HandleSize.x,pgizmo->HandleSize.y); // Same value than "NUT_ATxC_GetTxCScale_ArrowSize"
}

inline void NUT_ATxC_GetTxCScale_SpeedU_SpringEnd( NVEC2 *pb,const NUT_EDITOR_ATXC_ELEMENTS *pelements, const NUT_EDITOR_ATXC_ENTITY *pentity, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	NUT_ATxC_GetTxCScale_UArrowEnd(pb,pelements,pentity,pgizmo);
	pb->x += pentity->EditingTxCScale_BaseScale.x * pgizmo->DragHandleSize.x*pgizmo->TexCoordScaleSpeed.x/pentity->EditingTxCScale_SpeedMax.x;
}
inline void NUT_ATxC_GetTxCScale_SpeedV_SpringEnd( NVEC2 *pb, const NUT_EDITOR_ATXC_ELEMENTS *pelements, const NUT_EDITOR_ATXC_ENTITY *pentity, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	NUT_ATxC_GetTxCScale_VArrowEnd(pb,pelements,pentity,pgizmo);
	pb->y -= pentity->EditingTxCScale_BaseScale.y * pgizmo->DragHandleSize.y*pgizmo->TexCoordScaleSpeed.y/pentity->EditingTxCScale_SpeedMax.y;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// +
// +	TEXCOORD RESET
// +
// +
inline void NUT_ATxC_GetTxCReset_UHandlePosition(NVEC2 *prectcenter, const NUT_EDITOR_ATXC_ELEMENTS *pelements, const NUT_EDITOR_ATXC_ENTITY *pentity, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	NUT_ATxC_GetTxCOffset_HandlePosition(prectcenter,pelements,pentity,pgizmo);
	prectcenter->x += -NSIGN(pelements->TexCoordScale[pgizmo->EditedTexCoordSetID].x)*pgizmo->HandleSize.x*(0.5f+CONSTANT_NUT_EDITOR_ATXC_SUBSIZEFACTOR);
}
inline void NUT_ATxC_GetTxCReset_HandleExtents(NVEC2 *pextents,const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	pextents->x = pgizmo->HandleSize.x * CONSTANT_NUT_EDITOR_ATXC_SUBSIZEFACTOR; 
	pextents->y = pgizmo->HandleSize.y * CONSTANT_NUT_EDITOR_ATXC_SUBSIZEFACTOR; 
}

inline void NUT_ATxC_GetTxCReset_UHandleRect(NRECTf32 *prect, const NUT_EDITOR_ATXC_ELEMENTS *pelements, const NUT_EDITOR_ATXC_ENTITY *pentity, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	NVEC2	v0,vex;

	NUT_ATxC_GetTxCReset_UHandlePosition(&v0,pelements,pentity,pgizmo);
	NUT_ATxC_GetTxCReset_HandleExtents(&vex,pgizmo);
	prect->left		= v0.x - vex.x;
	prect->right	= v0.x + vex.x;
	prect->bottom	= v0.y - vex.y;
	prect->top		= v0.y + vex.y;
}

inline void NUT_ATxC_GetTxCReset_VHandlePosition(NVEC2 *prectcenter,const NUT_EDITOR_ATXC_ELEMENTS *pelements, const NUT_EDITOR_ATXC_ENTITY *pentity, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	NUT_ATxC_GetTxCOffset_HandlePosition(prectcenter,pelements,pentity,pgizmo);
	prectcenter->y += NSIGN(pelements->TexCoordScale[pgizmo->EditedTexCoordSetID].y)*pgizmo->HandleSize.y*(0.5f+CONSTANT_NUT_EDITOR_ATXC_SUBSIZEFACTOR);
}

inline void NUT_ATxC_GetTxCReset_VHandleRect(NRECTf32 *prect,const NUT_EDITOR_ATXC_ELEMENTS *pelements, const NUT_EDITOR_ATXC_ENTITY *pentity, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	NVEC2	v0,vex;

	NUT_ATxC_GetTxCReset_VHandlePosition(&v0,pelements,pentity,pgizmo);
	NUT_ATxC_GetTxCReset_HandleExtents(&vex,pgizmo);
	prect->left		= v0.x - vex.x;
	prect->right	= v0.x + vex.x;
	prect->bottom	= v0.y - vex.y;
	prect->top		= v0.y + vex.y;
}

/*
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// +
// +	SINGLE TEXCOORD	
// +
// +
inline void NUT_ATxC_GetSingleTxC_HandleCenter(NVEC2 *prectcenter, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	prectcenter->x	= pgizmo->Position.x - pgizmo->HandleSize.x*0.5f;
	prectcenter->y	= pgizmo->Position.y + pgizmo->DragHandleSize.y*0.5f;
}

inline void NUT_ATxC_GetSingleTxC_HandleRect(NRECTf32 *prect, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	prect->left		= pgizmo->Position.x - pgizmo->HandleSize.x;
	prect->right	= pgizmo->Position.x;
	prect->bottom	= pgizmo->Position.y;
	prect->top		= pgizmo->Position.y + pgizmo->DragHandleSize.y;
}
inline void NUT_ATxC_SingleTxC_HandleExtents(NVEC2 *pextents, const NUT_EDITOR_ATXC_GIZMO *pgizmo)
{
	pextents->x = pgizmo->HandleSize.x*0.5f;
	pextents->y = pgizmo->DragHandleSize.y*0.5f;
}
*/
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NUT_EDITOR_ATXC_GIZMO_UV_INLINE_H

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
// !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!! ONLY IF COMPILER '_NEDITOR' FLAG IS SET TO ON !!!
#endif //_NEDITOR


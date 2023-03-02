#ifndef __NUT_GIZMO_RECTf32_H
#define __NUT_GIZMO_RECTf32_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_GizmoRectf32.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "../NFlags.h"
#include "lib/N/NType.h"
#include "../NRectf32.h"
#include "../Maths/NVec3f32.h"

#include "../NScreenPoint.h"
#include "lib/N/NErrorHandling.h"
#include "../NCoordinateSystemConversions.h"

#include "../Core/NSafeConversion.h"
#include "NUT_Slider.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Constants

#define CONSTANT_NUT_GIZMO_RECTf32_INITIALIZATION_COUNT_MAX 5
#define CONSTANT_NUT_GIZMO_RECTf32_INIT_SUM_THRESHOLD 0.05f // nearly 5% of the Viewport height
#define DEFAULT__GIZMO_RECTf32_HANDLE_HANDLE_EXTEND 0.007f

// Drawing constants
#define CONSTANT_NUT_GIZMO_RECTf32_MODE_ICON_SMALL_SIZE_RATIO 1.75f	 // Define small size of the INOUTSCALE (or TRANSF) icon radius regarding handle extend size
#define CONSTANT_NUT_GIZMO_RECTf32_MODE_ICON_SIZE_RATIO 2.5f		 // Define size of the INOUTSCALE (or TRANSF) icon radius regarding handle extend size
#define CONSTANT_NUT_GIZMO_RECTf32_INOUT_SCALE_ICON_SIGN_RATIO 0.75f // Define size of the INOUTSCALE SIGN icon   regarding INOUTSCALE icon size
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// HandleId and DockId
//	|Xtd| T | B | L | R |
//	+---+---+---+---+---+
#define BITS_GIZMO_RECTf32_NONE 0x0			//	| 	| 0	| 0	| 0	| 0	|	--> BIT_0,BIT_1,BIT_2,BIT_3 set to OFF
											//	+---+---+---+---+---+
#define BITS_GIZMO_RECTf32_LEFT_BOTTOM 0x6	//	|   | 0 | 1	| 1	| 0	|	--> BIT_1,BIT_2		set to ON
#define BITS_GIZMO_RECTf32_LEFT_MID 0x2		//	|  	| 0	| 0	| 1	| 0	|	--> BIT_1			set to ON
#define BITS_GIZMO_RECTf32_LEFT_TOP 0xA		//	|  	| 1	| 0	| 1	| 0	|	--> BIT_1,BIT_3		set to ON
#define BITS_GIZMO_RECTf32_MID_TOP 0x8		//	|  	| 1	| 0	| 0	| 0	|	--> BIT_3			set to ON
#define BITS_GIZMO_RECTf32_RIGHT_TOP 0x9	//	|  	| 1	| 0	| 0	| 1	|	--> BIT_0,BIT_3		set to ON
#define BITS_GIZMO_RECTf32_RIGHT_MID 0x1	//	|  	| 0	| 0	| 0	| 1	|	--> BIT_0			set to ON
#define BITS_GIZMO_RECTf32_RIGHT_BOTTOM 0x5 //	|  	| 0	| 1	| 0	| 1	|	--> BIT_0,BIT_2		set to ON
#define BITS_GIZMO_RECTf32_MID_BOTTOM 0x4	//	|  	| 0	| 1	| 0	| 0	|	--> BIT_2			set to ON
											//	+---+---+---+---+---+
#define BITS_GIZMO_RECTf32_INSIDE_RECT 0x10 //	| 1	|  	|  	| 	| 	|	--> BIT_4 set to ON
											//	+---+---+---+---+---+

#define MASK_GIZMO_RECTf32_HANDLE 0xF			// BIT_0 to BIT_3 included, only the HANDLE information ...
#define MASK_GIZMO_RECTf32_RECT_AND_HANDLE 0x1F // BIT_0 to BIT_4 included, HANDLE information + the information Inside/Outside the rect stored on the BIT_4

// To extract Top,Bottom,Left;Right and InRect information from HandleId or DockId
#define _GET_GIZMO_RECTf32_RIGHT(id) ((id)&1)
#define _GET_GIZMO_RECTf32_LEFT(id) (((id) >> 1) & 1)
#define _GET_GIZMO_RECTf32_BOTTOM(id) (((id) >> 2) & 1)
#define _GET_GIZMO_RECTf32_TOP(id) (((id) >> 3) & 1)
#define _GET_GIZMO_RECTf32_INSIDE_RECT(id) (((id) >> 4) & 1)

// Macros ( to make code crystal clear and easiest to understand... )
#define NUT_IsXYGizmoRectf32HandlePicked(hid) ((hid)&MASK_GIZMO_RECTf32_HANDLE)												   // Is 0 if not, otherwise NON-ZERO
#define NUT_IsXYGizmoRectf32RectPicked(hid) (_GET_GIZMO_RECTf32_INSIDE_RECT(hid))											   // cursor is Inside the rect ( and maybe inside of one of the 8  handles too... )
#define NUT_IsXYGizmoRectf32RectPickedOnly(hid) (((hid)&MASK_GIZMO_RECTf32_RECT_AND_HANDLE) == BITS_GIZMO_RECTf32_INSIDE_RECT) // cursor is Inside the rect and OUTSIDE of the 8  handles too.
#define NUT_IsXYGizmoRectf32HandleOrRectPicked(hid) ((hid)&MASK_GIZMO_RECTf32_RECT_AND_HANDLE)								   // Is 0 if not, otherwise NON-ZERO

	// #define FLAG_... RESERVED by SYSTEM !!!			BIT_0	|
	// #define FLAG_... RESERVED by SYSTEM !!!			BIT_1	|
	// #define FLAG_... RESERVED by SYSTEM !!!			BIT_2	|-- See above section "HandleId and DockId"
	// #define FLAG_... RESERVED by SYSTEM !!!			BIT_3	|
	// #define FLAG_... RESERVED by SYSTEM !!!			BIT_4	|

#define FLAG_RECTf32_MODE_LOCKED BIT_5
#define FLAG_RECTf32_CHANGE_MODE BIT_6

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Internal States
	//
	/*
typedef enum
{
	NUT_GIZMO_RECTf32_STATE_NULL = 0,
	NUT_GIZMO_RECTf32_STATE_HANDLE,
	NUT_GIZMO_RECTf32_STATE_HANDLE_RECT,
	NUT_GIZMO_RECTf32_STATE_SCALE_WAIT,
	NUT_GIZMO_RECTf32_STATE_SCALE_RECT,
	NUT_GIZMO_RECTf32_STATE_SCALE_IN_OUT,
	// -------------------------------------------------
	NUT_GIZMO_RECTf32_STATE_SIZE
}NUT_GIZMO_RECTf32_STATE_old;
*/
	typedef enum
	{
		NUT_GIZMO_RECTf32_STATE_NULL,
		NUT_GIZMO_RECTf32_STATE_HANDLE,
		NUT_GIZMO_RECTf32_STATE_RECT_TRANSF,
		NUT_GIZMO_RECTf32_STATE_RECT_SCALE,
		NUT_GIZMO_RECTf32_STATE_INOUT_SCALE,
		NUT_GIZMO_RECTf32_STATE_TRANSF_TO_SCALE,
		NUT_GIZMO_RECTf32_STATE_SCALE_TO_TRANSF,
		// -------------------------------------------------
		NUT_GIZMO_RECTf32_STATE_SIZE
	} NUT_GIZMO_RECTf32_STATE;

	typedef enum
	{
		NUT_GIZMO_RECTf32_MODE_TRANSF,
		NUT_GIZMO_RECTf32_MODE_SCALE,
		// -------------------------------------------------
		NUT_GIZMO_RECTf32_MODE_SIZE
	} NUT_GIZMO_RECTf32_MODE;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Modes
	//
	//	There are 3 modes to use with the functions "NUT_PickXYGizmoRectf32Handle" and "NUT_PickXYGizmoRectf32"
	//			NUT_GIZMO_RECTf32_MODE_FREE_TRANSF
	//			NUT_GIZMO_RECTf32_MODE_SCALE
	//			NUT_GIZMO_RECTf32_MODE_AUTO

	//	|31 |30 |29 |28 |27 |26 |25 |24 |23 |22 |21 |20 |19 |18 |17 |16 |15 |14 |13 |12 |11 |10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
	//	+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
	//	|	State if pick FAILED		|	State if RECT is picked		| State if an HANDLE is picked	|	BIT_1 to BIT_7 unused	|	|---BIT_0	: HITPOINT if an handle is picked ('hitpoint_if_pick_handle')
	//	+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+	ON		: use the HANDLE center position as Hitpoint
	//	| 0	| 0 | 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	|	OFF		: keep using the incomming pointer position as Hitpoint
	//	| 0 | 0 | 0 | 0	| 0	| 0	| 0 | 0 | 0	| 0	| 0	| 0 | 0 | 0	| 0	| 0	| 0 | 0 | 0 | 0	| 0	| 0	| 0 | 0 | 0	| 0	| 0	| 0 | 0 | 0	| 0	| 0	|
	//	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	| 0	|
	//	+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
	/*
	// To "pack" a Mode
	#define NUT_MAKE_GIZMO_RECTf32_MODE(state_if_pick_fail,state_if_pick_rect,state_if_pick_handle,hitpoint_if_pick_handle)		((((state_if_pick_fail)&0xFF)<<24) |	(((state_if_pick_rect)&0xFF)<<16) | (((state_if_pick_handle)&0xFF)<<8) | ((hitpoint_if_pick_handle)&0x1))
	// To retrieve informations from a Mode
	#define NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(mode)				(	(mode)&0x1			)
	#define NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_STATE(mode)			(	((mode)>> 8)&0xFF	)
	#define NUT_GET_GIZMO_RECTf32_MODE_PICK_RECT_STATE(mode)			(	((mode)>> 16)&0xFF	)
	#define NUT_GET_GIZMO_RECTf32_MODE_PICK_FAILED_STATE(mode)			(	((mode)>> 24)&0xFF	)

	#define NUT_GIZMO_RECTf32_MODE_HITPOINT_IS_USUAL					0
	#define NUT_GIZMO_RECTf32_MODE_HITPOINT_IS_HANDLE					1


	// The 3 different Modes
	//
	//																								|	State if pick FAILED			|	State if RECT is picked				| State if an HANDLE is picked				| HITPOINT if an handle is picked		|
	//	--------------------------------------------------------------------------------------------+-----------------------------------+---------------------------------------+-------------------------------------------+---------------------------------------+
	#define NUT_GIZMO_RECTf32_MODE_FREE_TRANSF							NUT_MAKE_GIZMO_RECTf32_MODE(NUT_GIZMO_RECTf32_STATE_NULL,		NUT_GIZMO_RECTf32_STATE_HANDLE_RECT,	NUT_GIZMO_RECTf32_STATE_HANDLE,				NUT_GIZMO_RECTf32_MODE_HITPOINT_IS_USUAL	)
	#define NUT_GIZMO_RECTf32_MODE_SCALE								NUT_MAKE_GIZMO_RECTf32_MODE(NUT_GIZMO_RECTf32_STATE_NULL,		NUT_GIZMO_RECTf32_STATE_SCALE_WAIT,		NUT_GIZMO_RECTf32_STATE_SCALE_WAIT,			NUT_GIZMO_RECTf32_MODE_HITPOINT_IS_HANDLE	)
	#define NUT_GIZMO_RECTf32_MODE_AUTO									NUT_MAKE_GIZMO_RECTf32_MODE(NUT_GIZMO_RECTf32_STATE_NULL,		NUT_GIZMO_RECTf32_STATE_AUTO,			NUT_GIZMO_RECTf32_STATE_HANDLE,				NUT_GIZMO_RECTf32_MODE_HITPOINT_IS_USUAL	)
	*/

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// structure

	typedef struct
	{
		Nu16 Flags;
		Nu16 Count;

		Nu8 ModeId;	  // Current Mode of the GIZMO. ( Transf or scale )
		Nu8 StateId;  // Current state of the GIZMO. ( Type of transformation detected: handle transform / scale rect / scale in or out )
		Nu8 HandleId; // Id of the current selected Handle.
		Nu8 DockId;	  // Id of the current selected DockingSlot.

		NRECTf32 Rect;
		NRECTf32 UpdatedRect;
		NVEC2f32 HandleExtend; // Size of the 9 handles spread all along the rectangle perimeter.
		NVEC2f32 HitPoint;
		NVEC2f32 DockingNormExtend; // Extend (in "Hrel Coord system") of the whole docking rectangle use to define docking surfaces.
									//		DockingNormExtend.x = Half screen width in Hrel coord system
									//		DockingNormExtend.y = Half screen height in Hrel coord system
									//		...Means the whole screen.
									//		Use "NUT_SetDockingRectangle" function to set them
		NVEC2f32 CurrentCursor;
		// NUT_SLIDER	ScaleSlider;
		Nf32 ScaleFactor;

	} NUT_GIZMO_RECTf32;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// Functions
	//
	// Basics
	NUT_GIZMO_RECTf32 *NUT_SetupGizmoRectf32(NUT_GIZMO_RECTf32 *pgm, const NVEC2f32 *phandle_extend);
	NUT_GIZMO_RECTf32 *NUT_CreateGizmoRectf32(const NVEC2f32 *phandle_extend);
	void NUT_ClearGizmoRectf32(NUT_GIZMO_RECTf32 *pgm);
	void NUT_DeleteGizmoRectf32(NUT_GIZMO_RECTf32 *pgm);

	// Additionnal Setups
	inline void NUT_ResetGizmoRectf32(NUT_GIZMO_RECTf32 *pgm)
	{
		pgm->StateId = _SafeNu32ToNu8(NUT_GIZMO_RECTf32_STATE_NULL);
		pgm->HandleId = BITS_GIZMO_RECTf32_NONE;
		pgm->Rect = pgm->UpdatedRect;
		pgm->Count = 0;
		pgm->CurrentCursor.x = pgm->CurrentCursor.y = 0.0f;
	}
	inline void NUT_SetGizmoRectf32_HandleExtend(NUT_GIZMO_RECTf32 *pgm, const NVEC2f32 *phandle_extend) { pgm->HandleExtend = *phandle_extend; }

	// Pick and Update
	Nu32 NUT_PickXYGizmoRectf32(NUT_GIZMO_RECTf32 *pgm, const NRECTf32 *prect, const NVEC2s16 *ppos);		// Research and pick Main rect only ( return BITS_GIZMO_RECTf32_HANDLE_NULL or BITS_GIZMO_RECTf32_HANDLE_INSIDE only )
	Nu32 NUT_PickXYGizmoRectf32Handle(NUT_GIZMO_RECTf32 *pgm, const NRECTf32 *prect, const NVEC2s16 *ppos); // Research and pick Main rect and its 8 handles.
	Nu32 NUT_UpdateXYGizmoRectf32(NUT_GIZMO_RECTf32 *pgm, const NVEC2s16 *ppos);
	Nu32 NUT_EndXYGizmoRectf32(NUT_GIZMO_RECTf32 *pgm, const NVEC2s16 *ppos);

	// Misc
	inline NVEC2f32 *NUT_SetGizmoRectf32DockingRectangle(NUT_GIZMO_RECTf32 *pgm, const Nf32 width, const Nf32 height)
	{
		pgm->DockingNormExtend.x = width * 0.5f;
		pgm->DockingNormExtend.y = height * 0.5f;
	}
	inline NVEC2f32 *NUT_GetGizmoUpdatedRectf32LeftBottomPosition(NVEC2f32 *prm, const NUT_GIZMO_RECTf32 *pgm)
	{
		prm->x = pgm->UpdatedRect.left;
		prm->y = pgm->UpdatedRect.bottom;
		return prm;
	}
	inline NRECTf32 *NUT_GetGizmoUpdatedRectf32(NUT_GIZMO_RECTf32 *pgm) { return &pgm->UpdatedRect; }
	inline NRECTf32 *NUT_GetGizmoPickedRectf32(NUT_GIZMO_RECTf32 *pgm) { return &pgm->Rect; }
	inline Nf32 NUT_GetGizmoUpdatedRatioW(NUT_GIZMO_RECTf32 *pgm) { return (pgm->UpdatedRect.right - pgm->UpdatedRect.left) / (pgm->Rect.right - pgm->Rect.left); }
	inline Nf32 NUT_GetGizmoUpdatedRatioH(NUT_GIZMO_RECTf32 *pgm) { return (pgm->UpdatedRect.top - pgm->UpdatedRect.bottom) / (pgm->Rect.top - pgm->Rect.bottom); }
// inline Nu32					NUT_GetGizmoCurrentHandleId(NUT_GIZMO_RECTf32 *pgm){return pgm->HandleId; }

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Draw ...
//			To draw a GizmoRectf32 on Screen have a look at NUT_Draw.h
//			(Use the function NUT_Draw_GizmoRectf32 with its parameters...)

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NUT_GIZMO_RECTf32_H

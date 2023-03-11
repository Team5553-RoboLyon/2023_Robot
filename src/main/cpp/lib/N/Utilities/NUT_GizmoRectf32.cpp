#include "NUT_GizmoRectf32.h"
#include "lib/N/NCoordinateSystemConversions.h"


// ------------------------------------------------------------------------------------------
// NUT_GIZMO_MOVE* NUT_SetupGizmoRectf32
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
NUT_GIZMO_RECTf32* NUT_SetupGizmoRectf32(NUT_GIZMO_RECTf32 *pgm, const NVEC2f32 *phandle_extend)
{
	NErrorIf(!pgm,NERROR_INCONSISTENT_PARAMETERS );
	Nmem0(pgm,NUT_GIZMO_RECTf32);
	if (phandle_extend)
		pgm->HandleExtend = *phandle_extend;
	else
	{
		pgm->HandleExtend.x = DEFAULT__GIZMO_RECTf32_HANDLE_HANDLE_EXTEND;
		pgm->HandleExtend.y = DEFAULT__GIZMO_RECTf32_HANDLE_HANDLE_EXTEND;
	}
	
	NVEC2f32 sliderxtend;
	sliderxtend.x = 0.2f;
	sliderxtend.y = 0.02f;
//	NUT_SetupSlider(&pgm->ScaleSlider,0.5f,1.5f, NULL, &sliderxtend, 3, CONSTANT_NUT_SLIDER_SNAP_STRENGTH_MEDIUM,NULL);
//	FLAG_OFF(pgm->ScaleSlider.Flags, FLAG_NUT_SLIDER_SHOW_OUTPUTVALUE);
	pgm->ScaleFactor = 1.0f;
	pgm->DockingNormExtend.x = 0.475f;
	pgm->DockingNormExtend.y = 0.475f;
	return pgm;
}
// ------------------------------------------------------------------------------------------
// NUT_GIZMO_RECTf32*	NUT_CreateGizmoMoveconst
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
NUT_GIZMO_RECTf32*	NUT_CreateGizmoRectf32(const NVEC2f32 *phandle_extend)
{
	return NUT_SetupGizmoRectf32(NEW(NUT_GIZMO_RECTf32), phandle_extend);
}

void NUT_ClearGizmoRectf32(NUT_GIZMO_RECTf32 *pgm)
{
//	NUT_ClearSlider(&pgm->ScaleSlider);
	Nmem0(pgm, NUT_GIZMO_RECTf32);
}

void NUT_DeleteGizmoRectf32(NUT_GIZMO_RECTf32 *pgm)
{
	NUT_ClearGizmoRectf32(pgm);
	Nfree(pgm);
}


Nu32 NUT_PickXYGizmoRectf32(NUT_GIZMO_RECTf32 *pgm, const NRECTf32 *prect, const NVEC2s16 *ppos)
{
	// TODO: IMPROVE IT !!!

	// Apply the failed state.
	if (pgm->HandleId == BITS_GIZMO_RECTf32_NONE)
	{
		Nf32 hrelx = NCoord_PixToHRel_X(ppos->x);

		// Inside the whole Rectangle:
		if (hrelx < prect->left)
			return BITS_GIZMO_RECTf32_NONE; // which is equal to current pgm->HandleId;
		if (hrelx > prect->right)
			return BITS_GIZMO_RECTf32_NONE; // which is equal to current pgm->HandleId;

		Nf32 hrely = NCoord_PixToHRel_Y(ppos->y);

		if (hrely < prect->bottom)
			return BITS_GIZMO_RECTf32_NONE; // which is equal to current pgm->HandleId;
		if (hrely > prect->top)
			return BITS_GIZMO_RECTf32_NONE; // which is equal to current pgm->HandleId;

		// is inside the inner Rect
		pgm->Rect = *prect;
		pgm->HitPoint.x = hrelx;
		pgm->HitPoint.y = hrely;
		//pgm->StateId = NUT_GET_GIZMO_RECTf32_MODE_PICK_RECT_STATE(update_mode);
		pgm->HandleId = BITS_GIZMO_RECTf32_INSIDE_RECT;

		return BITS_GIZMO_RECTf32_INSIDE_RECT; // which is equal to current pgm->HandleId;
	}
	else
		return BITS_GIZMO_RECTf32_NONE;
}

// ------------------------------------------------------------------------------------------
Nu32 NUT_PickXYGizmoRectf32Handle(NUT_GIZMO_RECTf32 *pgm, const NRECTf32 *prect, const NVEC2s16 *ppos)
{
	FLAG_OFF(pgm->Flags, FLAG_RECTf32_MODE_LOCKED);
	FLAG_OFF(pgm->Flags, FLAG_RECTf32_CHANGE_MODE);

	// Notes:
	// Try to picking a new rect, but only if the current Rectf32 status is not already fine defined with a potential already "picked rect".
	//	BITS_GIZMO_RECTf32_NONE			means no curent picked rect
	//	BITS_GIZMO_RECTf32_INSIDE_RECT	means there is a current picked rect but only inside the rect ( no handles )
	//	...
	if ((pgm->HandleId == BITS_GIZMO_RECTf32_NONE) || (pgm->HandleId == BITS_GIZMO_RECTf32_INSIDE_RECT))
	{
		NVEC2f32	hrelpos;
		NCoord_PixToHRel(&hrelpos, ppos);

		// The Outer rect
		Nf32 outer_left		= prect->left - pgm->HandleExtend.x;
		Nf32 outer_right	= prect->right + pgm->HandleExtend.x;
		Nf32 outer_top		= prect->top + pgm->HandleExtend.y;
		Nf32 outer_bottom	= prect->bottom - pgm->HandleExtend.y;

		// Is the cursor outside the OuterRect ?
		if (hrelpos.x < outer_left)
			return BITS_GIZMO_RECTf32_NONE;
		if (hrelpos.x > outer_right)
			return BITS_GIZMO_RECTf32_NONE;
		if (hrelpos.y > outer_top)
			return BITS_GIZMO_RECTf32_NONE;
		if (hrelpos.y < outer_bottom)
			return BITS_GIZMO_RECTf32_NONE;

		//
		// From here we are sure, cursor is NOT outside the OuterRect but INSIDE it !
		//

		// The Inner rect
		Nf32 inner_left		= prect->left + pgm->HandleExtend.x;
		Nf32 inner_right	= prect->right - pgm->HandleExtend.x;
		Nf32 inner_top		= prect->top - pgm->HandleExtend.y;
		Nf32 inner_bottom	= prect->bottom + pgm->HandleExtend.y;

		/*
		// Is the cursor inside the innerRect ?
		if (hrelpos.x > inner_left)
			if (hrelpos.x < inner_right)
				if (hrelpos.y > inner_bottom)
					if (hrelpos.y < inner_top)
		*/


		// The mid line
		NVEC2f32	center;
		center.y = (prect->top + prect->bottom)*0.5f;
		Nf32 mid_line_bottom	= center.y - pgm->HandleExtend.y;
		Nf32 mid_line_top		= center.y + pgm->HandleExtend.y;


		// Left column
		// ---------------------------------------------------------------------
		if (hrelpos.x <= inner_left)
		{
			// Left-Bottom Handle
			if (hrelpos.y <= inner_bottom)
			{
				pgm->Rect		= *prect;
				pgm->UpdatedRect= *prect;
				pgm->HitPoint.x = hrelpos.x;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? prect->left : hrelpos.x;
				pgm->HitPoint.y = hrelpos.y;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? prect->bottom : hrelpos.y;
				pgm->StateId	= NUT_GIZMO_RECTf32_STATE_NULL;
				pgm->HandleId	= BITS_GIZMO_RECTf32_LEFT_BOTTOM | ((hrelpos.x >= prect->left) << 4); // The second part set the BIT_4 to ON if the pointer is INSIDE the Rect ...
				return pgm->HandleId;
			}

			// Left-Top Handle:
			if (hrelpos.y >= inner_top)
			{
				pgm->Rect		= *prect;
				pgm->UpdatedRect= *prect;
				pgm->HitPoint.x = hrelpos.x;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? prect->left : hrelpos.x;
				pgm->HitPoint.y = hrelpos.y;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? prect->bottom : hrelpos.y;
				pgm->StateId	= NUT_GIZMO_RECTf32_STATE_NULL;
				pgm->HandleId	= BITS_GIZMO_RECTf32_LEFT_TOP | ((hrelpos.x >= prect->left) << 4); // The second part set the BIT_4 to ON if the pointer is INSIDE the Rect ...
				return pgm->HandleId;
			}

			// Left-Mid Handle:
			if ((hrelpos.y >= mid_line_bottom) && (hrelpos.y <= mid_line_top))
			{
				pgm->Rect = *prect;
				pgm->UpdatedRect = *prect;

				pgm->HitPoint.x = hrelpos.x;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? prect->left : hrelpos.x;
				pgm->HitPoint.y = hrelpos.y;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? (prect->bottom + prect->top)*0.5f : hrelpos.y;
				pgm->StateId = NUT_GIZMO_RECTf32_STATE_NULL;
				pgm->HandleId	= BITS_GIZMO_RECTf32_LEFT_MID | ((hrelpos.x >= prect->left) << 4); // The second part set the BIT_4 to ON if the pointer is INSIDE the Rect ...
				return pgm->HandleId;
			}
			
			// Toggle StateId ( FreeTransform <> Scale )
			pgm->ModeId = 1 - pgm->ModeId;

			pgm->Rect		= *prect;
			pgm->UpdatedRect = *prect;
			pgm->HitPoint.x = hrelpos.x;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? prect->left : hrelpos.x;
			pgm->HitPoint.y = hrelpos.y;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? prect->bottom : hrelpos.y;
			pgm->HandleId	= BITS_GIZMO_RECTf32_NONE | ((hrelpos.x >= prect->left) << 4); // The second part set the BIT_4 to ON if the pointer is INSIDE the Rect ...
			return pgm->HandleId;
		}

		// Right column
		// ---------------------------------------------------------------------
		if (hrelpos.x >= inner_right)
		{
			// Right-Bottom Handle
			if (hrelpos.y <= inner_bottom)
			{
				pgm->Rect = *prect;
				pgm->UpdatedRect = *prect;
				pgm->HitPoint.x = hrelpos.x;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? prect->right : hrelpos.x;
				pgm->HitPoint.y = hrelpos.y;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? prect->bottom : hrelpos.y;
				pgm->StateId = NUT_GIZMO_RECTf32_STATE_NULL;
				pgm->HandleId = BITS_GIZMO_RECTf32_RIGHT_BOTTOM | ((hrelpos.x <= prect->right) << 4); // The second part set the BIT_4 to ON if the pointer is INSIDE the Rect ...
				return pgm->HandleId;
			}

			// Right-Top Handle
			if (hrelpos.y>= inner_top)
			{
				pgm->Rect = *prect;
				pgm->UpdatedRect = *prect;
				pgm->HitPoint.x = hrelpos.x;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? prect->right : hrelpos.x;
				pgm->HitPoint.y = hrelpos.y;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? prect->top : hrelpos.y;
				pgm->StateId = NUT_GIZMO_RECTf32_STATE_NULL;
				pgm->HandleId = BITS_GIZMO_RECTf32_RIGHT_TOP | ((hrelpos.x <= prect->right) << 4); // The second part set the BIT_4 to ON if the pointer is INSIDE the Rect ...
				return pgm->HandleId;
			}

			// Right-Mid Handle
			if ( (hrelpos.y >= mid_line_bottom) && (hrelpos.y <= mid_line_top) )
			{
				pgm->Rect = *prect;
				pgm->UpdatedRect = *prect;
				pgm->HitPoint.x = hrelpos.x;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? prect->right : hrelpos.x;
				pgm->HitPoint.y = hrelpos.y;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? (prect->bottom + prect->top)*0.5f : hrelpos.y;
				pgm->StateId = NUT_GIZMO_RECTf32_STATE_NULL;
				pgm->HandleId = BITS_GIZMO_RECTf32_RIGHT_MID | ((hrelpos.x <= prect->right) << 4); // The second part set the BIT_4 to ON if the pointer is INSIDE the Rect ...
				return pgm->HandleId;
			}

			// Toggle StateId ( FreeTransform <> Scale )
			pgm->ModeId = 1 - pgm->ModeId;

			pgm->Rect = *prect;
			pgm->UpdatedRect = *prect;
			pgm->HitPoint.x = hrelpos.x;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? prect->left : hrelpos.x;
			pgm->HitPoint.y = hrelpos.y;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? prect->bottom : hrelpos.y;
			pgm->HandleId = BITS_GIZMO_RECTf32_NONE | ((hrelpos.x <= prect->right) << 4); // The second part set the BIT_4 to ON if the pointer is INSIDE the Rect ...
			return pgm->HandleId;
		}

		center.x = (prect->left + prect->right)*0.5f;
		Nf32 mid_column_left	= center.x - pgm->HandleExtend.x;
		Nf32 mid_column_right	= center.x + pgm->HandleExtend.x;

		// Top Line
		// ---------------------------------------------------------------------
		if (hrelpos.y >= inner_top)
		{
			// Mid-Top Handle
			if ((hrelpos.x >= mid_column_left) && (hrelpos.x <= mid_column_right))
			{
				pgm->Rect = *prect;
				pgm->UpdatedRect = *prect;
				pgm->HitPoint.x = hrelpos.x;// NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? (prect->left + prect->right)*0.5f : hrelpos.x;
				pgm->HitPoint.y = hrelpos.y;// NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? prect->bottom : hrelpos.y;
				pgm->StateId = NUT_GIZMO_RECTf32_STATE_NULL;
				pgm->HandleId = BITS_GIZMO_RECTf32_MID_TOP | ((hrelpos.y <= prect->top) << 4); // The second part set the BIT_4 to ON if the pointer is INSIDE the Rect ...
				return pgm->HandleId;
			}

			// Toggle ModeId ( FreeTransform <> Scale )
			pgm->ModeId = 1 - pgm->ModeId;

			pgm->Rect = *prect;
			pgm->UpdatedRect = *prect;
			pgm->HitPoint.x = hrelpos.x;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? prect->left : hrelpos.x;
			pgm->HitPoint.y = hrelpos.y;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? prect->bottom : hrelpos.y;
			pgm->HandleId = BITS_GIZMO_RECTf32_NONE | ((hrelpos.y <= prect->top) << 4); // The second part set the BIT_4 to ON if the pointer is INSIDE the Rect ...
			return pgm->HandleId;
		}

		// Bottom Line
		// ---------------------------------------------------------------------
		if (hrelpos.y <= inner_bottom)
		{
			// Mid-Bottom Handle
			if ((hrelpos.x >= mid_column_left) && (hrelpos.x <= mid_column_right))
			{
				pgm->Rect = *prect;
				pgm->UpdatedRect = *prect;
				pgm->HitPoint.x = hrelpos.x;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? (prect->left + prect->right)*0.5f : hrelpos.x;
				pgm->HitPoint.y = hrelpos.y;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? prect->top : hrelpos.y;
				pgm->StateId = NUT_GIZMO_RECTf32_STATE_NULL;
				pgm->HandleId = BITS_GIZMO_RECTf32_MID_BOTTOM | ((hrelpos.y >= prect->bottom) << 4); // The second part set the BIT_4 to ON if the pointer is INSIDE the Rect ...
				return pgm->HandleId;
			}

			// Toggle StateId ( FreeTransform <> Scale )
			pgm->ModeId = 1 - pgm->ModeId;

			pgm->Rect = *prect;
			pgm->UpdatedRect = *prect;
			pgm->HitPoint.x = hrelpos.x;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? prect->left : hrelpos.x;
			pgm->HitPoint.y = hrelpos.y;//NUT_GET_GIZMO_RECTf32_MODE_PICK_HANDLE_HP(update_mode) ? prect->bottom : hrelpos.y;
			pgm->HandleId = BITS_GIZMO_RECTf32_NONE | ((hrelpos.y <= prect->top) << 4); // The second part set the BIT_4 to ON if the pointer is INSIDE the Rect ...
			return pgm->HandleId;
		}
		// is inside the inner Rect
		pgm->Rect = *prect;
		pgm->UpdatedRect = *prect;
		pgm->HitPoint = hrelpos;
		pgm->CurrentCursor = hrelpos;
		if(pgm->ModeId == NUT_GIZMO_RECTf32_MODE_SCALE)
			pgm->StateId = NUT_GIZMO_RECTf32_STATE_SCALE_TO_TRANSF;
		else
			pgm->StateId = NUT_GIZMO_RECTf32_STATE_TRANSF_TO_SCALE;

		pgm->HandleId = BITS_GIZMO_RECTf32_INSIDE_RECT;
		return pgm->HandleId;
	}
	else
		return BITS_GIZMO_RECTf32_NONE;
}

Nu8 private_NUT_PickDockingSlot(NUT_GIZMO_RECTf32 *pgm, const NVEC2f32 *phrelpos)
{
	// Inside dockingSlots Rectangle:
	NRECTf32		xrect;
	NVEC2f32		center;
	NVEC2f32		xtend;
	
	center.x		= NCoord_NormToHRel_X(0.5f);
	center.y		= NCoord_NormToHRel_Y(-0.5f);
	xtend.y			= pgm->HandleExtend.y*2.0f;
	xtend.x			= NSize_NormToHRel_W(xtend.y);


	xrect.left		= center.x  - xtend.x*3.0f;
	xrect.right		= center.x  + xtend.x*3.0f;
	xrect.top		= center.y  + xtend.y*3.0f;
	xrect.bottom	= center.y  - xtend.y*3.0f;

	if (phrelpos->x < xrect.left)
		return 0;
	if (phrelpos->x > xrect.right)
		return 0;
	if (phrelpos->y < xrect.bottom)
		return 0;
	if (phrelpos->y > xrect.top)
		return 0;

	// Build the HandleId and DockId thanks to this Grid:
	//		+---+---+---+
	//	T	|  	|  	|  	|
	//		+---+---+---+
	//	0	|  	|  	|  	|
	//		+---+---+---+
	//	B	|  	|  	| 	|
	//		+---+---+---+
	//		  L	  0   R
	//
	// codex explanation
	//	|Xtd| Height| Width	|
	//	| 	| T	| B	| L	| R	|	
	//	+---+---+---+---+---+
	//	| 1	| 0	| 0	| 1	| 0	|	codex[0] =	0x2 --> Left
	//	| 1 | 0 | 0	| 0	| 0	|	codex[1] =	0x0 --> Center
	//	| 1	| 0	| 0	| 0	| 1	|	codex[2] =	0x1 --> Right
	//	+---+---+---+---+---+
	//	| 1	| 0	| 1	| 0	| 0	|	codey[0] =	0x4 --> Bottom
	//	| 1	| 0	| 0	| 0	| 0	|	codey[1] =	0x0 --> Center
	//	| 1	| 1	| 0	| 0	| 0	|	codey[2] =	0x8 --> Top
	//	+---+---+---+---+---+
	//	| 1	| 0	| 0	| 0	| 0	|	Xtd code	0x10--> In The Main Rect
	//	+---+---+---+---+---+

	Nu8	codex[3] = { 0x2,0x0,0x1 };
	Nu8	codey[3] = { 0x4,0x0,0x8 };
	
	return (0x10 | codex[(Nu32)floor((phrelpos->x - xrect.left) / (2.0f*xtend.x))] | codey[(Nu32)floor((phrelpos->y - xrect.bottom) / (2.0f*xtend.y))]);
}

Nu32 NUT_UpdateXYGizmoRectf32(NUT_GIZMO_RECTf32 *pgm, const NVEC2s16 *ppos)
{
	if (!NUT_IsXYGizmoRectf32HandleOrRectPicked(pgm->HandleId))
		return 0;

	NVEC2f32	hrelpos;
	NVEC2f32	xtend, dockxtend;
	NVEC3f32	v3;
	Nf32		_r;
	Nf32		_l;
	Nf32		_b;
	Nf32		_t;
	Nf32		f;
	Nf32		_x, _y;
	// User initialy picked inside the rect !
	if (NUT_IsXYGizmoRectf32RectPickedOnly(pgm->HandleId))
	{
		switch (pgm->ModeId)
		{
			case NUT_GIZMO_RECTf32_MODE_TRANSF:
				NCoord_PixToHRel(&pgm->CurrentCursor, ppos);

				// Changing to MODE SCALE ?
				if (ISFLAG_OFF(pgm->Flags, FLAG_RECTf32_MODE_LOCKED))
				{
					f = pgm->HandleExtend.x*CONSTANT_NUT_GIZMO_RECTf32_MODE_ICON_SMALL_SIZE_RATIO;
					_y = pgm->CurrentCursor.y - pgm->HitPoint.y;
					if (NABS(_y) > f)
					{
						FLAG_ON(pgm->Flags, FLAG_RECTf32_MODE_LOCKED);
					}
					else
					{
						_x = pgm->CurrentCursor.x - pgm->HitPoint.x;
						if (_x > f ||  _x < -4.0f*f)
						{
							FLAG_ON(pgm->Flags, FLAG_RECTf32_MODE_LOCKED);
						}
						else
						{
							pgm->StateId = NUT_GIZMO_RECTf32_STATE_TRANSF_TO_SCALE;

							if(_x < -f)
								FLAG_ON(pgm->Flags, FLAG_RECTf32_CHANGE_MODE);
							else
								FLAG_OFF(pgm->Flags, FLAG_RECTf32_CHANGE_MODE);

							return 1;
						}
					}
				}
				// Docking ?
				pgm->DockId = private_NUT_PickDockingSlot(pgm, &pgm->CurrentCursor);

				if (pgm->DockId)
				{
					_r = (Nf32)_GET_GIZMO_RECTf32_RIGHT(pgm->DockId);			// transform 1 bit value into a float so 0.0f or 1.0f
					_l = (Nf32)_GET_GIZMO_RECTf32_LEFT(pgm->DockId);			// transform 1 bit value into a float so 0.0f or 1.0f
					_b = (Nf32)_GET_GIZMO_RECTf32_BOTTOM(pgm->DockId);			// transform 1 bit value into a float so 0.0f or 1.0f
					_t = (Nf32)_GET_GIZMO_RECTf32_TOP(pgm->DockId);				// transform 1 bit value into a float so 0.0f or 1.0f

					dockxtend.x = NSize_NormToHRel_W(pgm->DockingNormExtend.x);
					dockxtend.y = NSize_NormToHRel_H(pgm->DockingNormExtend.y);

					xtend.x = dockxtend.x * (2.0f - _l - _r) / 2.0f;
					xtend.y = dockxtend.y * (2.0f - _t - _b) / 2.0f;

					v3.x = NCoord_NormToHRel_X(0.5f) + dockxtend.x*0.5f*(_r - _l);
					v3.y = NCoord_NormToHRel_Y(-0.5f) + dockxtend.y*0.5f*(_t - _b);
					v3.z = 0.0f;

					pgm->UpdatedRect.left = v3.x - xtend.x;
					pgm->UpdatedRect.right = v3.x + xtend.x;
					pgm->UpdatedRect.bottom = v3.y - xtend.y;
					pgm->UpdatedRect.top = v3.y + xtend.y;
				}
				else
				{
					hrelpos.x = pgm->CurrentCursor.x - pgm->HitPoint.x;
					hrelpos.y = pgm->CurrentCursor.y - pgm->HitPoint.y;

					pgm->UpdatedRect.left = pgm->Rect.left + hrelpos.x;
					pgm->UpdatedRect.right = pgm->Rect.right + hrelpos.x;
					pgm->UpdatedRect.bottom = pgm->Rect.bottom + hrelpos.y;
					pgm->UpdatedRect.top = pgm->Rect.top + hrelpos.y;
				}
				NRectf32Reorder(&pgm->UpdatedRect);
				pgm->StateId = NUT_GIZMO_RECTf32_STATE_RECT_TRANSF;
				return 1;

			case NUT_GIZMO_RECTf32_MODE_SCALE:
				pgm->CurrentCursor.x = NCoord_PixToHRel_X(ppos->x);
				pgm->CurrentCursor.y = NCoord_PixToHRel_Y(ppos->y);
				
				// Changing to MODE TRANSF ?
				if (ISFLAG_OFF(pgm->Flags, FLAG_RECTf32_MODE_LOCKED))
				{
					_y = pgm->CurrentCursor.y - pgm->HitPoint.y;
					f = pgm->HandleExtend.x*CONSTANT_NUT_GIZMO_RECTf32_MODE_ICON_SMALL_SIZE_RATIO;
					if (NABS(_y) > f)
					{
						FLAG_ON(pgm->Flags, FLAG_RECTf32_MODE_LOCKED);
					}
					else
					{
						_x = pgm->CurrentCursor.x - pgm->HitPoint.x;
						if (_x > f || _x < -4.0f*f )
						{
							FLAG_ON(pgm->Flags, FLAG_RECTf32_MODE_LOCKED);
						}
						else
						{
							pgm->StateId = NUT_GIZMO_RECTf32_STATE_SCALE_TO_TRANSF;
							
							if (_x < -f )
								FLAG_ON(pgm->Flags, FLAG_RECTf32_CHANGE_MODE);
							else
								FLAG_OFF(pgm->Flags, FLAG_RECTf32_CHANGE_MODE);
							return 1;
						}
					}
				}

				// SCALE MODE:
				if (NABS(pgm->CurrentCursor.x - pgm->HitPoint.x) < 0.01f)
				{
					f = pgm->CurrentCursor.y - pgm->HitPoint.y;
					if (f)
					{
						pgm->ScaleFactor = 1.0f + 5.0f*NCLAMP(-0.1f, f, 0.1f);
					}

					pgm->StateId = NUT_GIZMO_RECTf32_STATE_INOUT_SCALE;
					return 1;
				}
				else
					pgm->StateId = NUT_GIZMO_RECTf32_STATE_RECT_SCALE;
				return 1;

			default:
				NErrorIf(1, NERROR_SYSTEM_CHECK); // Not possible !!! one handle has to be picked here !!!
				pgm->StateId = NUT_GIZMO_RECTf32_STATE_NULL;
				return 0;
		}
	}
	// user picked an handle !
	else
	{
		NErrorIf(!NUT_IsXYGizmoRectf32HandlePicked(pgm->HandleId), NERROR_SYSTEM_CHECK); // Not possible !!! one handle has to be picked here !!!

		_r = (Nf32)_GET_GIZMO_RECTf32_RIGHT(pgm->HandleId);			// transform 1 bit value into a float so 0.0f or 1.0f
		_l = (Nf32)_GET_GIZMO_RECTf32_LEFT(pgm->HandleId);			// transform 1 bit value into a float so 0.0f or 1.0f
		_b = (Nf32)_GET_GIZMO_RECTf32_BOTTOM(pgm->HandleId);		// transform 1 bit value into a float so 0.0f or 1.0f
		_t = (Nf32)_GET_GIZMO_RECTf32_TOP(pgm->HandleId);			// transform 1 bit value into a float so 0.0f or 1.0f

		NCoord_PixToHRel(&hrelpos, ppos);

		hrelpos.x -= pgm->HitPoint.x;
		hrelpos.y -= pgm->HitPoint.y;

		pgm->UpdatedRect.left = pgm->Rect.left + _l * hrelpos.x;
		pgm->UpdatedRect.right = pgm->Rect.right + _r * hrelpos.x;
		pgm->UpdatedRect.bottom = pgm->Rect.bottom + _b * hrelpos.y;
		pgm->UpdatedRect.top = pgm->Rect.top + _t * hrelpos.y;
		NRectf32Reorder(&pgm->UpdatedRect);

		pgm->StateId = NUT_GIZMO_RECTf32_STATE_HANDLE;
		return 1;
	}

	return 0;
}
Nu32 NUT_EndXYGizmoRectf32(NUT_GIZMO_RECTf32 *pgm, const NVEC2s16 *ppos)
{

	if (!NUT_IsXYGizmoRectf32HandleOrRectPicked(pgm->HandleId))
		return 0;

	NVEC2f32	center;
	NVEC2f32	xtend;
	Nf32		factor;

	switch (pgm->StateId)
	{
		case NUT_GIZMO_RECTf32_STATE_NULL:
			return 0;

		case NUT_GIZMO_RECTf32_STATE_HANDLE:
			return 1;

		case NUT_GIZMO_RECTf32_STATE_RECT_TRANSF:
			return 1;

		case NUT_GIZMO_RECTf32_STATE_INOUT_SCALE:
			pgm->UpdatedRect.left	= pgm->HitPoint.x + (pgm->Rect.left - pgm->HitPoint.x)*pgm->ScaleFactor;
			pgm->UpdatedRect.right	= pgm->HitPoint.x + (pgm->Rect.right - pgm->HitPoint.x)*pgm->ScaleFactor;
			pgm->UpdatedRect.bottom = pgm->HitPoint.y + (pgm->Rect.bottom - pgm->HitPoint.y)*pgm->ScaleFactor;
			pgm->UpdatedRect.top	= pgm->HitPoint.y + (pgm->Rect.top - pgm->HitPoint.y)*pgm->ScaleFactor;
			return 1;

		case NUT_GIZMO_RECTf32_STATE_RECT_SCALE:
			pgm->CurrentCursor.x = NCoord_PixToHRel_X(ppos->x);
			pgm->CurrentCursor.y = NCoord_PixToHRel_Y(ppos->y);

			center.x = (pgm->HitPoint.x + pgm->CurrentCursor.x)*0.5f;
			center.y = (pgm->HitPoint.y + pgm->CurrentCursor.y)*0.5f;
			xtend.x = NABS(pgm->HitPoint.x - pgm->CurrentCursor.x);
			xtend.y = NABS(pgm->HitPoint.y - pgm->CurrentCursor.y);

			if (!xtend.x || !xtend.y)
				return 0;

			NErrorIf(NMIN(NSize_NormToHRel_W(1.0f) / xtend.x, NSize_NormToHRel_H(1.0f) / xtend.y) <= 0.0f, NERROR_INCONSISTENT_VALUES);
			factor = NMIN(NSize_NormToHRel_W(1.0f) / xtend.x, NSize_NormToHRel_H(1.0f) / xtend.y);

			pgm->UpdatedRect.left = (pgm->Rect.left - center.x)*factor + NCoord_NormToHRel_X(0.5f);
			pgm->UpdatedRect.right = (pgm->Rect.right - center.x)*factor + NCoord_NormToHRel_X(0.5f);
			pgm->UpdatedRect.bottom = (pgm->Rect.bottom - center.y)*factor + NCoord_NormToHRel_Y(-0.5f);
			pgm->UpdatedRect.top = (pgm->Rect.top - center.y)*factor + NCoord_NormToHRel_Y(-0.5f);
			return 1;

		case NUT_GIZMO_RECTf32_STATE_TRANSF_TO_SCALE:
			if (ISFLAG_ON(pgm->Flags, FLAG_RECTf32_CHANGE_MODE))
				pgm->ModeId = NUT_GIZMO_RECTf32_MODE_SCALE;
			return 1;

		case NUT_GIZMO_RECTf32_STATE_SCALE_TO_TRANSF:
			if (ISFLAG_ON(pgm->Flags, FLAG_RECTf32_CHANGE_MODE))
				pgm->ModeId = NUT_GIZMO_RECTf32_MODE_TRANSF;
			else
			{
				// Like a static click in "NUT_GIZMO_RECTf32_STATE_INOUT_SCALE" state
				pgm->UpdatedRect.left = pgm->HitPoint.x + (pgm->Rect.left - pgm->HitPoint.x)*pgm->ScaleFactor;
				pgm->UpdatedRect.right = pgm->HitPoint.x + (pgm->Rect.right - pgm->HitPoint.x)*pgm->ScaleFactor;
				pgm->UpdatedRect.bottom = pgm->HitPoint.y + (pgm->Rect.bottom - pgm->HitPoint.y)*pgm->ScaleFactor;
				pgm->UpdatedRect.top = pgm->HitPoint.y + (pgm->Rect.top - pgm->HitPoint.y)*pgm->ScaleFactor;

			}
			return 1;

		default:
			NErrorIf(1, NERROR_SYSTEM_CHECK); // Not possible !!! All the different states are tested inside the switch , so ...
			break;

	}
	return 0;
}
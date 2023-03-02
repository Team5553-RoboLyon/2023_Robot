#include "../../NCStandard.h"
#include "../../Geometry/NGeometry.h"
#include "../../NErrorHandling.h"
#include "../../NCoordinateSystemConversions.h"

#include "../NUT_GizmoRectf32.h"
#include "NUT_Draw.h"
#include "../NUT_Slider.h"
#include "NUT_DrawPencil.h"


extern NMATRIX3x3				NUT_DrawOrthonormal;

static inline void _draw_transf_icon(const NVEC3f32 *po, const Nf32 xtend, const NCOLOR* pcolor)
{
	NUT_DRAW_ARROWS	arrows;
	NVEC3			A, B;

	Nmem0(&arrows, NUT_DRAW_ARROWS);
	arrows.AArrowSize = 0.35f;
	arrows.BArrowSize = 0.35f;
	arrows.Color = *pcolor;
	NUT_SetDrawArrows_ShapeA(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_BACKWARD);
	NUT_SetDrawArrows_ShapeB(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_FORWARD);

	A.x = po->x - xtend;
	A.y = po->y;
	A.z = po->z;
	B.x = po->x + xtend;
	B.y = po->y;
	B.z = po->z;
	NUT_Draw_Arrows(&A, &B, &arrows);

	A.x = po->x;
	A.y = po->y - xtend;
	A.z = po->z;
	B.x = po->x;
	B.y = po->y + xtend;
	B.z = po->z;
	NUT_Draw_Arrows(&A, &B, &arrows);
}
static inline void _draw_inout_scale_icon(const NVEC3f32 *po, const Nf32 size, const Nf32 scalefactor, const NCOLOR* pcolor)
{
	Nf32	f;
	NVEC2	xtend;
	NUT_Draw_XYCircle(po, size, pcolor);
	if (scalefactor > 1.0f)
	{
		f = (scalefactor - 0.5f); f *= f;
		xtend.x = size * f*CONSTANT_NUT_GIZMO_RECTf32_INOUT_SCALE_ICON_SIGN_RATIO;
		xtend.y = xtend.x;
		NUT_Draw_XYMark(po, &xtend, pcolor);
	}
	else if (scalefactor < 1.0f)
	{
		f = (scalefactor - 1.5f); f *= f;

		xtend.x = size * f*CONSTANT_NUT_GIZMO_RECTf32_INOUT_SCALE_ICON_SIGN_RATIO;
		xtend.y = xtend.x;

		NVEC3 v3, v3b;

		v3b = v3 = *po;
		v3.x -= xtend.x;
		v3b.x += xtend.x;
		NUT_Draw_Segment(&v3, &v3b, pcolor);
	}
}
void NUT_Draw_XYGizmoRectf32( NUT_GIZMO_RECTf32 *pgm, const NCOLOR *pcolor, NCOLOR *pselectioncolor )
{
	NUT_DRAW_ARROWS	arrows;
	NUTDRAWVERTEX	v0,v1;
	NVEC3f32		v3,v3b;
	Nu32			a;
	Nu32			b;
	Nf32			f,radius;
	NVEC2f32		xtend;
	NVEC2f32		dockxtend;

	v0.Position_3f.z	= 0.0f;
	v1.Position_3f.z	= 0.0f;

	switch (pgm->StateId)
	{
		case NUT_GIZMO_RECTf32_STATE_NULL:
			break;

		case NUT_GIZMO_RECTf32_STATE_HANDLE:
			NErrorIf(!NUT_IsXYGizmoRectf32HandlePicked(pgm->HandleId), NERROR_INCONSISTENT_VALUES);
			// Draw the UPDATED RECT
			v0.Color0_4f = *pselectioncolor;
			v1.Color0_4f = *pselectioncolor;

			v0.Position_3f.x = pgm->UpdatedRect.left;
			v0.Position_3f.y = pgm->UpdatedRect.bottom;
			NUT_DrawPencil_From(&v0);

			v1.Position_3f.x = pgm->UpdatedRect.right;
			v1.Position_3f.y = pgm->UpdatedRect.bottom;
			NUT_DrawPencil_LineTo(&v1);

			v1.Position_3f.x = pgm->UpdatedRect.right;
			v1.Position_3f.y = pgm->UpdatedRect.top;
			NUT_DrawPencil_LineTo(&v1);

			v1.Position_3f.x = pgm->UpdatedRect.left;
			v1.Position_3f.y = pgm->UpdatedRect.top;
			NUT_DrawPencil_LineTo(&v1);
			NUT_DrawPencil_LineTo(&v0);

			// compute coords and draw current selected handle
			a = (Nu32)_GET_GIZMO_RECTf32_RIGHT(pgm->HandleId);
			b = (Nu32)_GET_GIZMO_RECTf32_LEFT(pgm->HandleId);
			v3.x = (pgm->UpdatedRect.left*(Nf32)(1 - a + b) + pgm->UpdatedRect.right * (Nf32)(1 + a - b)) / 2.0f;
			a = (Nu32)_GET_GIZMO_RECTf32_BOTTOM(pgm->HandleId);
			b = (Nu32)_GET_GIZMO_RECTf32_TOP(pgm->HandleId);
			v3.y = (pgm->UpdatedRect.top*(Nf32)(1 - a + b) + pgm->UpdatedRect.bottom * (Nf32)(1 + a - b)) / 2.0f;
			v3.z = 0.0f;
			NUT_Draw_XYQuad(&v3, &pgm->HandleExtend, &v0.Color0_4f);
			break;

		case NUT_GIZMO_RECTf32_STATE_RECT_TRANSF:
			NErrorIf(!NUT_IsXYGizmoRectf32RectPicked(pgm->HandleId), NERROR_INCONSISTENT_VALUES);
			// Draw the UPDATED RECT
			v0.Color0_4f = *pselectioncolor;
			v1.Color0_4f = *pselectioncolor;

			v0.Position_3f.x = pgm->UpdatedRect.left;
			v0.Position_3f.y = pgm->UpdatedRect.bottom;
			NUT_DrawPencil_From(&v0);

			v1.Position_3f.x = pgm->UpdatedRect.right;
			v1.Position_3f.y = pgm->UpdatedRect.bottom;
			NUT_DrawPencil_LineTo(&v1);

			v1.Position_3f.x = pgm->UpdatedRect.right;
			v1.Position_3f.y = pgm->UpdatedRect.top;
			NUT_DrawPencil_LineTo(&v1);

			v1.Position_3f.x = pgm->UpdatedRect.left;
			v1.Position_3f.y = pgm->UpdatedRect.top;
			NUT_DrawPencil_LineTo(&v1);
			NUT_DrawPencil_LineTo(&v0);

			// Draw the Docking paddles
			
			v0.Color0_4f = *pcolor;

			//Screen Center
			v3.x = NCoord_NormToHRel_X(0.5f);
			v3.y = NCoord_NormToHRel_Y(-0.5f);
			v3.z = 0.0f;
			//Xtend of One Dock
			dockxtend.y  = pgm->HandleExtend.y*2.0f;
			dockxtend.x  = NSize_NormToHRel_W(dockxtend.y);
			
			xtend.x = dockxtend.x;
			xtend.y = dockxtend.y *3.0f;
			NUT_Draw_XYQuad(&v3, &xtend, &v0.Color0_4f);

			xtend.x = dockxtend.x *3.0f;
			xtend.y = dockxtend.y;
			NUT_Draw_XYQuad(&v3, &xtend, &v0.Color0_4f);

			if (pgm->DockId)
			{
				Nf32		_r;
				Nf32		_l;
				Nf32		_b;
				Nf32		_t;

				_r = (Nf32)_GET_GIZMO_RECTf32_RIGHT(pgm->DockId);			// transform 1 bit value into a float so 0.0f or 1.0f
				_l = (Nf32)_GET_GIZMO_RECTf32_LEFT(pgm->DockId);			// transform 1 bit value into a float so 0.0f or 1.0f
				_b = (Nf32)_GET_GIZMO_RECTf32_BOTTOM(pgm->DockId);			// transform 1 bit value into a float so 0.0f or 1.0f
				_t = (Nf32)_GET_GIZMO_RECTf32_TOP(pgm->DockId);				// transform 1 bit value into a float so 0.0f or 1.0f

				v3.x = NCoord_NormToHRel_X(0.5f)	+ dockxtend.x*2.5f*(_r-_l);
				v3.y = NCoord_NormToHRel_Y(-0.5f)	+ dockxtend.y*2.5f*(_t-_b);
				v3.z = 0.0f;
				xtend.x = dockxtend.x *0.9f * (2.0f - _l -_r ) / 2.0f;
				xtend.y = dockxtend.y *0.9f * (2.0f - _t -_b) / 2.0f;
				NUT_Draw_XYQuad(&v3, &xtend, &v0.Color0_4f);

				// size of "transf" icon
				f = pgm->HandleExtend.x*CONSTANT_NUT_GIZMO_RECTf32_MODE_ICON_SIZE_RATIO*0.5f;
			}
			else
			{
				// size of "transf" icon
				f = pgm->HandleExtend.x*CONSTANT_NUT_GIZMO_RECTf32_MODE_ICON_SIZE_RATIO;
			}

			v3.x = pgm->CurrentCursor.x;
			v3.y = pgm->CurrentCursor.y;
			v3.z = 0.0f;
			_draw_transf_icon(&v3,f , pselectioncolor);
			break;
	
		case NUT_GIZMO_RECTf32_STATE_RECT_SCALE:
			v3.x = pgm->HitPoint.x;
			v3.y = pgm->HitPoint.y;
			v3.z = 0.0f;
			NUT_Draw_XYCircle(&v3, pgm->HandleExtend.x,pselectioncolor);

			v3.x = (pgm->HitPoint.x  + pgm->CurrentCursor.x)*0.5f;
			v3.y = (pgm->HitPoint.y  + pgm->CurrentCursor.y)*0.5f;
			xtend.x = NABS(pgm->HitPoint.x - pgm->CurrentCursor.x)*0.5f;
			xtend.y = NABS(pgm->HitPoint.y - pgm->CurrentCursor.y)*0.5f;
			NUT_Draw_XYQuad(&v3, &xtend,pselectioncolor);
			break;

		case NUT_GIZMO_RECTf32_STATE_INOUT_SCALE:
			v3.x = pgm->HitPoint.x;
			v3.y = pgm->HitPoint.y;
			v3.z = 0.0f;
			_draw_inout_scale_icon(&v3, pgm->HandleExtend.x*CONSTANT_NUT_GIZMO_RECTf32_MODE_ICON_SIZE_RATIO, pgm->ScaleFactor, pselectioncolor);
			/*
			radius = pgm->HandleExtend.x*CONSTANT_NUT_GIZMO_RECTf32_INOUT_SCALE_ICON_CIRCLE_RADIUS_RATIO;

			NUT_Draw_XYCircle(&v3, radius, pselectioncolor);
			
			if (pgm->ScaleFactor > 1.0f)
			{
				f = (pgm->ScaleFactor - 0.5f); f *= f;
				xtend.x = radius*f*CONSTANT_NUT_GIZMO_RECTf32_INOUT_SCALE_ICON_SIGN_RATIO;
				xtend.y = xtend.x;
				NUT_Draw_XYMark(&v3, &xtend, pselectioncolor);
			}
			else if(pgm->ScaleFactor < 1.0f)
			{
				f = (pgm->ScaleFactor - 1.5f); f *= f;

				xtend.x = radius * f*CONSTANT_NUT_GIZMO_RECTf32_INOUT_SCALE_ICON_SIGN_RATIO;
				xtend.y = xtend.x;

				v3b = v3;
				v3.x  -= xtend.x;
				v3b.x += xtend.x;
				NUT_Draw_Segment(&v3, &v3b, pselectioncolor);
			}
			*/
			break;

		case NUT_GIZMO_RECTf32_STATE_SCALE_TO_TRANSF:
				v3.x = pgm->HitPoint.x;
				v3.y = pgm->HitPoint.y;
				v3.z = 0.0f;

			if (ISFLAG_ON(pgm->Flags, FLAG_RECTf32_CHANGE_MODE))
			{
				// scale icon
				radius = pgm->HandleExtend.x*CONSTANT_NUT_GIZMO_RECTf32_MODE_ICON_SMALL_SIZE_RATIO*0.5f;
				// move(transf) icon
				xtend.x = xtend.y = pgm->HandleExtend.x*CONSTANT_NUT_GIZMO_RECTf32_MODE_ICON_SMALL_SIZE_RATIO;
			}
			else
			{
				// scale icon
				radius = pgm->HandleExtend.x*CONSTANT_NUT_GIZMO_RECTf32_MODE_ICON_SMALL_SIZE_RATIO;
				// move(transf) icon
				xtend.x = xtend.y = pgm->HandleExtend.x*CONSTANT_NUT_GIZMO_RECTf32_MODE_ICON_SMALL_SIZE_RATIO*0.5f;
			}
			
			// scale icon
			_draw_inout_scale_icon(&v3, radius, pgm->ScaleFactor, pselectioncolor);

			// move(transf) icon
			v3.x -= 3.0f*CONSTANT_NUT_GIZMO_RECTf32_MODE_ICON_SMALL_SIZE_RATIO*pgm->HandleExtend.x;
			//NUT_Draw_XYQuad(&v3, &xtend, pselectioncolor);
			_draw_transf_icon(&v3, xtend.x, pselectioncolor);

			// arrow between the 2 icons
			v3.x = pgm->HitPoint.x - pgm->HandleExtend.x*CONSTANT_NUT_GIZMO_RECTf32_MODE_ICON_SMALL_SIZE_RATIO;
			v3b.x = pgm->HitPoint.x - 2.0f*CONSTANT_NUT_GIZMO_RECTf32_MODE_ICON_SMALL_SIZE_RATIO*pgm->HandleExtend.x;
			v3b.y = v3.y;
			v3b.z = v3.z;
			Nmem0(&arrows, NUT_DRAW_ARROWS);
			arrows.BArrowSize = 0.6f;
			arrows.Color = *pselectioncolor;
			NUT_SetDrawArrows_ShapeB(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_FORWARD);
			NUT_Draw_Arrows(&v3, &v3b, &arrows);
			break;

		case NUT_GIZMO_RECTf32_STATE_TRANSF_TO_SCALE:
			v3.x = pgm->HitPoint.x;
			v3.y = pgm->HitPoint.y;
			v3.z = 0.0f;

			if (ISFLAG_ON(pgm->Flags, FLAG_RECTf32_CHANGE_MODE))
			{
				// scale icon
				radius = pgm->HandleExtend.x*CONSTANT_NUT_GIZMO_RECTf32_MODE_ICON_SMALL_SIZE_RATIO;
				// move(transf) icon
				xtend.x = xtend.y = pgm->HandleExtend.x*CONSTANT_NUT_GIZMO_RECTf32_MODE_ICON_SMALL_SIZE_RATIO*0.5f;
			}
			else
			{
				// scale icon
				radius = pgm->HandleExtend.x*CONSTANT_NUT_GIZMO_RECTf32_MODE_ICON_SMALL_SIZE_RATIO*0.5f;
				// move(transf) icon
				xtend.x = xtend.y = pgm->HandleExtend.x*CONSTANT_NUT_GIZMO_RECTf32_MODE_ICON_SMALL_SIZE_RATIO;
			}

			// move(transf) icon
			//NUT_Draw_XYQuad(&v3, &xtend, pselectioncolor);
			_draw_transf_icon(&v3, xtend.x, pselectioncolor);

			// scale icon
			v3.x -= 3.0f*CONSTANT_NUT_GIZMO_RECTf32_MODE_ICON_SMALL_SIZE_RATIO*pgm->HandleExtend.x;
			_draw_inout_scale_icon(&v3, radius, pgm->ScaleFactor, pselectioncolor);

			// arrow between the 2 icons
			v3.x = pgm->HitPoint.x - pgm->HandleExtend.x*CONSTANT_NUT_GIZMO_RECTf32_MODE_ICON_SMALL_SIZE_RATIO;
			v3b.x = pgm->HitPoint.x - 2.0f*CONSTANT_NUT_GIZMO_RECTf32_MODE_ICON_SMALL_SIZE_RATIO*pgm->HandleExtend.x;
			v3b.y = v3.y;
			v3b.z = v3.z;
			Nmem0(&arrows, NUT_DRAW_ARROWS);
			arrows.BArrowSize = 0.6f;
			arrows.Color = *pselectioncolor;
			NUT_SetDrawArrows_ShapeB(&arrows, BITS_NUT_DRAW_ARROWS_SHAPE_FORWARD);
			NUT_Draw_Arrows(&v3, &v3b, &arrows);
			break;

		default:
			break;
	}


	// Draw the current RECT (NOT the updated one ! )
	v0.Color0_4f = *pcolor;
	v1.Color0_4f = *pcolor;

	v0.Position_3f.x = pgm->Rect.left;
	v0.Position_3f.y = pgm->Rect.bottom;
	NUT_DrawPencil_From(&v0);

	v1.Position_3f.x = pgm->Rect.right;
	v1.Position_3f.y = pgm->Rect.bottom;
	NUT_DrawPencil_LineTo(&v1);

	v1.Position_3f.x = pgm->Rect.right;
	v1.Position_3f.y = pgm->Rect.top;
	NUT_DrawPencil_LineTo(&v1);

	v1.Position_3f.x = pgm->Rect.left;
	v1.Position_3f.y = pgm->Rect.top;
	NUT_DrawPencil_LineTo(&v1);
	NUT_DrawPencil_LineTo(&v0);

	// All the handles
	v3.z = 0.0f;
	v3.y = pgm->Rect.bottom;
	v3.x = pgm->Rect.left;

	if (pgm->ModeId == NUT_GIZMO_RECTf32_MODE_TRANSF)
	{
		NUT_Draw_XYQuad(&v3, &pgm->HandleExtend, &v0.Color0_4f);
		v3.x = (pgm->Rect.left + pgm->Rect.right) / 2.0f;
		NUT_Draw_XYQuad(&v3, &pgm->HandleExtend, &v0.Color0_4f);
		v3.x = pgm->Rect.right;
		NUT_Draw_XYQuad(&v3, &pgm->HandleExtend, &v0.Color0_4f);
		v3.y = pgm->Rect.top;
		v3.x = pgm->Rect.left;
		NUT_Draw_XYQuad(&v3, &pgm->HandleExtend, &v0.Color0_4f);
		v3.x = (pgm->Rect.left + pgm->Rect.right) / 2.0f;
		NUT_Draw_XYQuad(&v3, &pgm->HandleExtend, &v0.Color0_4f);
		v3.x = pgm->Rect.right;
		NUT_Draw_XYQuad(&v3, &pgm->HandleExtend, &v0.Color0_4f);
		v3.y = (pgm->Rect.top + pgm->Rect.bottom) / 2.0f;
		NUT_Draw_XYQuad(&v3, &pgm->HandleExtend, &v0.Color0_4f);
		v3.x = pgm->Rect.left;
		NUT_Draw_XYQuad(&v3, &pgm->HandleExtend, &v0.Color0_4f);
	}
	else
	{
		Nf32 radius = NMIN(pgm->HandleExtend.x, pgm->HandleExtend.y);
		NUT_Draw_XYCircle(&v3, radius, &v0.Color0_4f);
		v3.x = (pgm->Rect.left + pgm->Rect.right) / 2.0f;
		NUT_Draw_XYCircle(&v3, radius, &v0.Color0_4f);
		v3.x = pgm->Rect.right;
		NUT_Draw_XYCircle(&v3, radius, &v0.Color0_4f);
		v3.y = pgm->Rect.top;
		v3.x = pgm->Rect.left;
		NUT_Draw_XYCircle(&v3, radius, &v0.Color0_4f);
		v3.x = (pgm->Rect.left + pgm->Rect.right) / 2.0f;
		NUT_Draw_XYCircle(&v3, radius, &v0.Color0_4f);
		v3.x = pgm->Rect.right;
		NUT_Draw_XYCircle(&v3, radius, &v0.Color0_4f);
		v3.y = (pgm->Rect.top + pgm->Rect.bottom) / 2.0f;
		NUT_Draw_XYCircle(&v3, radius, &v0.Color0_4f);
		v3.x = pgm->Rect.left;
		NUT_Draw_XYCircle(&v3, radius, &v0.Color0_4f);
	}

}


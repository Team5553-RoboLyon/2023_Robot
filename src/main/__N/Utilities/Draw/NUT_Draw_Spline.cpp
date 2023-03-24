#include "../../NCStandard.h"
#include "../../Geometry/NGeometry.h"
#include "../../NErrorHandling.h"
#include "NUT_DrawPencil.h"
#include "NUT_Draw.h"


void NUT_Draw_Spline(const NSPLINE *pspline, const NUT_DRAW_SPLINE *pdrawspline)
{
	Nu32				i,j;
	Nf32				fr,fac;	
	NSPLINEKNOT			*spa,*spb;
	NUTDRAWVERTEX		R;
	NUT_DRAW_ELLIPSE	ellipse;

	NUT_DRAWTEXT		drawtxt;
	char				txt[32];
	
	// debug
//	NAABB	aabb;


	R.Color0_4f	= pdrawspline->Color;
// DRAW the "CURVE"
	if(ISFLAG_OFF(pdrawspline->Flags,FLAG_NUT_DRAW_SPLINE_HIDESPLINE))
	{
		spb =(NSPLINEKNOT*)NGetFirstArrayPtr(&pspline->KnotArray);
		spb++;
		for(i=1;i<NGetArraySize(&pspline->KnotArray);i++,spb++)
		{
			spa = spb-1;
			R.Position_3f = spa->Position;
			NUT_DrawPencil_From(&R);
			fr = fac = 1.0f/(Nf32)pspline->BezierAccuracy;
			for( j = 1; j<= pspline->BezierAccuracy; j++,fr+=fac )
			{
				NCasteljauPoint3f32(&R.Position_3f,fr, spa, spb);
				NUT_DrawPencil_LineTo(&R);
			}

			//---debug
/*
			aabb.fXMin = NMIN( spb->TA.x, NMIN( spb->Position.x, NMIN(spa->Position.x,spa->TB.x) ) );
			aabb.fYMin = NMIN( spb->TA.y, NMIN( spb->Position.y, NMIN(spa->Position.x,spa->TB.y) ) );
			aabb.fZMin = NMIN( spb->TA.z, NMIN( spb->Position.z, NMIN(spa->Position.x,spa->TB.z) ) );
			aabb.fXMax = NMAX( spb->TA.x, NMAX( spb->Position.x, NMAX(spa->Position.x,spa->TB.x) ) );
			aabb.fYMax = NMAX( spb->TA.y, NMAX( spb->Position.y, NMAX(spa->Position.x,spa->TB.y) ) );
			aabb.fZMax = NMAX( spb->TA.z, NMAX( spb->Position.z, NMAX(spa->Position.x,spa->TB.z) ) );

			NUT_Draw_AABB(&aabb, &pdrawspline->Color);
*/
		}
	}

// DRAW POINTS & TANGENTS
// Pre-setup ellipse(s) constant data:	
	Nmem0(&ellipse, NUT_DRAW_ELLIPSE);
	ellipse.SliceNb = CONSTANT_NUT_DRAW_SPLINE_ELLIPSE_HANDLE_SLICE_NB;
	ellipse.Extents.x = pdrawspline->KnotSize*0.5f;
	ellipse.Extents.y = pdrawspline->KnotSize*0.5f;
	ellipse.Color = pdrawspline->Color;

	NVEC3	t0, t1,v;
	NVEC2	xtd;
	Nf32		l0, l1,dot;


	spa =(NSPLINEKNOT*)NGetFirstArrayPtr(&pspline->KnotArray);
	for(i=0;i<NGetArraySize(&pspline->KnotArray);i++,spa++)
	{
		if (ISFLAG_OFF(pdrawspline->Flags, FLAG_NUT_DRAW_SPLINE_HIDETANGENTES))
		{
			R.Position_3f = spa->TA;		NUT_DrawPencil_From(&R);
			R.Position_3f = spa->Position;	NUT_DrawPencil_LineTo(&R);
			R.Position_3f = spa->TB;		NUT_DrawPencil_LineTo(&R);

			ellipse.Center = spa->TA;
			NUT_Draw_Ellipse(&ellipse);
			ellipse.Center = spa->TB;
			NUT_Draw_Ellipse(&ellipse);
		}

		// Xtra infos is activated...
		if (ISFLAG_OFF(pdrawspline->Flags, FLAG_NUT_DRAW_SPLINE_HIDE_XTRA))
		{
			// Check the length of the 2 tangents. if they are equal, draw a sign !
			NVec3Sub(&t0, &spa->TA, &spa->Position);
			NVec3Sub(&t1, &spa->TB, &spa->Position);
			l0 = NVec3Length(&t0);
			l1 = NVec3Length(&t1);

			if (ISFLAG_OFF(pdrawspline->Flags, FLAG_NUT_DRAW_SPLINE_HIDETANGENTES))
			{
				
				if (NABS(l0 - l1) < pdrawspline->Xtra_Threshold )
				{
					xtd.x = xtd.y = l0*0.05f;

					// Middle of t0
					v.x = spa->Position.x + 0.5f*t0.x;
					v.y = spa->Position.y + 0.5f*t0.y;
					v.z = spa->Position.z + 0.5f*t0.z;
					NUT_Draw_Cross(&v, &xtd, &pdrawspline->Color);
					// Middle of t1
					v.x = spa->Position.x + 0.5f*t1.x;
					v.y = spa->Position.y + 0.5f*t1.y;
					v.z = spa->Position.z + 0.5f*t1.z;
					NUT_Draw_Cross(&v, &xtd, &pdrawspline->Color);
				}
			}

			//if its not hidden, Knot is a circle if tangents are aligned or a square if they are not...
			if (ISFLAG_OFF(pdrawspline->Flags, FLAG_NUT_DRAW_SPLINE_HIDESPLINEKNOTS))
			{
				dot = NVec3DotProduct(&t0, &t1)/(l0*l1); // dot is supposed to be -1 when the tangents are perfectly aligned.
				if (NABS(1.0f + dot) < pdrawspline->Xtra_Threshold )
				{
					ellipse.Center = spa->Position;
					NUT_Draw_Ellipse(&ellipse);
				}
				else
				{
					NUT_Draw_XYSquare(&spa->Position, pdrawspline->KnotSize, &pdrawspline->Color);
				}
			}

			// DRAW KNOT INDEX 
			sprintf(txt, "  %d",i );
			drawtxt.Color = pdrawspline->Color;
			drawtxt.Size = pdrawspline->KnotSize;
			NUT_Draw_Text(txt, &spa->Position, &drawtxt);
		}
		else // No XTRA to Draw... 
		{
			// Knot is a square if its not hidden else nothing...
			if (ISFLAG_OFF(pdrawspline->Flags, FLAG_NUT_DRAW_SPLINE_HIDESPLINEKNOTS))
			{
				NUT_Draw_XYSquare(&spa->Position, pdrawspline->KnotSize, &pdrawspline->Color);
			}
		}

	}
}

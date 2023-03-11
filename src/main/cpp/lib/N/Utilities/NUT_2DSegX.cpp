#include "lib/N/NCStandard.h"
#include "lib/N/GL/Ngl.h"
#include "lib/N/NViewport.h"

#include "NUT_X.h"

Nbool NUT_2DSegXBezierFastAABB(const NVEC2 *psegA, const NVEC2 *psegB, const NSPLINEKNOT *spa, const NSPLINEKNOT *spb)
{
	if (NMAX(psegA->x, psegB->x) < NMIN(spb->TA.x, NMIN(spb->Position.x, NMIN(spa->Position.x, spa->TB.x))))
		return NFALSE;

	if (NMIN(psegA->x, psegB->x) > NMAX(spb->TA.x, NMAX(spb->Position.x, NMAX(spa->Position.x, spa->TB.x))))
		return NFALSE;

	if (NMAX(psegA->y, psegB->y) < NMIN(spb->TA.y, NMIN(spb->Position.y, NMIN(spa->Position.y, spa->TB.y))))
		return NFALSE;

	if (NMIN(psegA->y, psegB->y) > NMAX(spb->TA.y, NMAX(spb->Position.y, NMAX(spa->Position.y, spa->TB.y))))
		return NFALSE;

	return NTRUE;
}

Nbool NUT_2DSegXSeg(const NVEC2 *A, const NVEC2 *B, const NVEC2 *C, const NVEC2 *D, NUT_2DLINE_EXTEND_XRESULT *pxrx)
{
	Nf32 d, t, u;
	NVEC2 AB, AC, CD;

	// AB Cross CD ?
	// ==========================
	//				D
	//		 	   |
	//			  |
	//		 	 |
	//		 A ..+...........B
	//		    |
	//		   |
	//		  |
	//		 C

	AB.x = B->x - A->x;
	AB.y = B->y - A->y;
	CD.x = D->x - C->x;
	CD.y = D->y - C->y;

	d = AB.x * CD.y - CD.x * AB.y;
	if (d == 0.0f) // or maybe <NFLOAT_EPSILON
		return NFALSE;

	AC.x = C->x - A->x;
	AC.y = C->y - A->y;
	t = AC.x * CD.y - AC.y * CD.x;
	t /= d;
	if (t < 0.0f || t > 1.0f)
		return NFALSE;

	u = AC.x * AB.y - AC.y * AB.x;
	u /= d;
	if (u < 0.0f || u > 1.0f)
		return NFALSE;

	if (pxrx)
	{
		pxrx->I.x = A->x + (t * AB.x);
		pxrx->I.y = A->y + (t * AB.y);
		pxrx->ParamCoordAB = t;
		pxrx->ParamCoordCD = u;
	}

	return NTRUE;
}

Nbool NUT_2DSegXLine(const NVEC2 *psegA, const NVEC2 *psegB, const NVEC2 *plineC, const NVEC2 *plineD, NUT_2DLINE_XRESULT *pxr)
{
	Nf32 d, u;
	NVEC2 ab, ca, cd;

	// AB Cross CD ?
	// ==========================
	//				D
	//		 	   |
	//			  |
	//		 	 |
	//		 A ..+...........B
	//		    |
	//		   |
	//		  |
	//		 C

	ab.x = psegB->x - psegA->x;
	ab.y = psegB->y - psegA->y;
	cd.x = plineD->x - plineC->x;
	cd.y = plineD->y - plineC->y;

	d = cd.x * ab.y - ab.x * cd.y;
	if (d == 0.0f) // or maybe <NFLOAT_EPSILON
		return NFALSE;

	ca.x = psegA->x - plineC->x;
	ca.y = psegA->y - plineC->y;
	u = ca.x * cd.y - ca.y * cd.x;
	u /= d;
	if (u < 0.0f || u > 1.0f)
		return NFALSE;

	if (pxr)
	{
		pxr->I.x = psegA->x + (u * ab.x);
		pxr->I.y = psegA->y + (u * ab.y);
		pxr->ParamCoordAB = u;
	}

	return NTRUE;
}

// ------------------------------------------------------------------------------------------
// NUT_2DSegXSpline
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
//
// Out :
//
//	NFALSE if there is no Intersection
//	NTRUE if there is one !
//
//	The NUT_2DLINE_EXTEND_XRESULT structure is going to be filled like this
//	(only if function returns NTRUE)
//		NVEC2		XPos		Intersection coordinates
//		Nf32			Xt;			Normalized ratio to place I (intersection point) like this:
//										I = A+Xt*AB (where A & B are pSegA and pSegB )
//		Nf32			Xt2;		Normalized ratio to place I (intersection point) like this:
//										I = C+Xt*CD. Here CD represents the small spline segment
//									which intersects with segment AB. C and D coordinates are ephemeral,
//									but because the NUT_2DSegXSpline function uses the NUT_2DSegXSeg function,
//									Xt2 is calculated and we keep it, even if we don't have C and D.
// ------------------------------------------------------------------------------------------
Nbool NUT_2DSegXSpline(const NVEC2 *psegA, const NVEC2 *psegB, const NSPLINE *pspline, NUT_2DLINE_EXTEND_XRESULT *pxrx)
{
	Nu32 i, j;
	Nf32 fr, fac;
	NVEC3 RA, RB;
	NSPLINEKNOT *spa, *spb;

	spb = (NSPLINEKNOT *)NGetFirstArrayPtr(&pspline->KnotArray);
	spb++;
	// for(i=1;i<NGetArraySize(&pspline->KnotArray);i++,spb++)
	for (i = NGetArraySize(&pspline->KnotArray) - 1; i != 0; i--, spb++)
	{
		spa = spb - 1;
		if (NUT_2DSegXBezierFastAABB(psegA, psegB, spa, spb))
		{
			RA = spa->Position;
			fr = fac = 1.0f / (Nf32)pspline->BezierAccuracy;
			// for( j = 1; j<=pspline->BezierAccuracy; j++,fr+=fac )
			for (j = pspline->BezierAccuracy; j != 0; j--, fr += fac)
			{
				NCasteljauPoint3f32(&RB, fr, spa, spb);
				if (NUT_2DSegXSeg(psegA, psegB, (NVEC2 *)&RA, (NVEC2 *)&RB, pxrx))
				{
					return NTRUE;
				}

				RA = RB;
			}
		}
	}
	return NFALSE;
}

Nbool NUT_2DSegXVerticalLine(const NVEC2 *psegA, const NVEC2 *psegB, const NVEC2 *plineC, NUT_2DLINE_XRESULT *pxr)
{
	Nf32 u;
	NVEC2 AB;

	// AB Cross CD ?
	// ==========================
	//			 .	D
	//		 	 .  |
	//			 . |
	//		 	 |
	//		     + R
	//		    |.
	//		   | .
	//		  |  .
	//		 C   .
	//			 + A

	AB.x = psegB->x - psegA->x;
	if (AB.x == 0.0f) // or maybe <NFLOAT_EPSILON
		return NFALSE;

	u = (plineC->x - psegA->x) / AB.x;
	if (u < 0.0f || u > 1.0f)
		return NFALSE;

	if (pxr)
	{
		AB.y = psegB->y - psegA->y;

		pxr->I.x = psegA->x + (u * AB.x);
		pxr->I.y = psegA->y + (u * AB.y);
		pxr->ParamCoordAB = u;
	}

	return NTRUE;
}

Nbool NUT_2DSegXHorizontalLine(const NVEC2 *psegA, const NVEC2 *psegB, const NVEC2 *plineC, NUT_2DLINE_XRESULT *pxr)
{
	Nf32 u;
	NVEC2 ab;

	// AB Cross CD ?
	// ==========================
	//				B
	//		 	   |
	//			  |
	//		 	 |
	//		 C ..+...........
	//		    |
	//		   |
	//		  |
	//		A

	ab.y = psegB->y - psegA->y;
	if (ab.y == 0.0f) // or maybe <NFLOAT_EPSILON
		return NFALSE;

	u = (plineC->y - psegA->y) / ab.y;
	if (u < 0.0f || u > 1.0f)
		return NFALSE;

	if (pxr)
	{
		ab.x = psegB->x - psegA->x;

		pxr->I.x = psegA->x + (u * ab.x);
		pxr->I.y = psegA->y + (u * ab.y);
		pxr->ParamCoordAB = u;
	}

	return NTRUE;
}

// xresult.t represents the normalized distance between the two segment extremities
// the X function called returns id of the B extremity.
// which one is closest to the intersection POINT ? A or B ?
// Easy to know: if .t = 0.5f I is in the exact segment middle, otherwise ...
// .t <0.5f means I close to A (return value -1 )
// .t >0.5f means I close to B (return value )

Nu32 NUT_2DSegV3ArrayXVerticalLine(const NARRAY *pv3array, const NVEC2 *plineC, NUT_2DLINE_XRESULT *pxr)
{
	Nu32 i;
	NVEC3 *pa, *pb;

	pb = (NVEC3 *)NGetFirstArrayPtr(pv3array);
	pb++;
	for (i = 1; i < NGetArraySize(pv3array); i++, pb++)
	{
		pa = pb - 1;
		if (NUT_2DSegXVerticalLine((NVEC2 *)pa, (NVEC2 *)pb, plineC, pxr))
			return i;
	}
	return 0;
}

// xresult.t represents the normalized distance between the two segment extremities
// the X function called returns id of the B extremity.
// which one is closest to the intersection POINT ? A or B ?
// Easy to know: if .t = 0.5f I is in the exact segment middle, otherwise ...
// .t <0.5f means I close to A (return value -1 )
// .t >0.5f means I close to B (return value )
Nu32 NUT_2DSegV2ArrayXVerticalLine(const NARRAY *pv2array, const NVEC2 *plineC, NUT_2DLINE_XRESULT *pxr)
{
	Nu32 i;
	NVEC2 *pa, *pb;

	pb = (NVEC2 *)NGetFirstArrayPtr(pv2array);
	pb++;
	for (i = 1; i < NGetArraySize(pv2array); i++, pb++)
	{
		pa = pb - 1;
		if (NUT_2DSegXVerticalLine(pa, pb, plineC, pxr))
			return i;
	}
	return 0;
}

Nu32 NUT_2DSegV3ArrayXLine(const NARRAY *pv3array, const NVEC2 *plineC, const NVEC2 *plineD, NUT_2DLINE_XRESULT *pxr)
{
	Nu32 i;
	NVEC3 *pa, *pb;

	pb = (NVEC3 *)NGetFirstArrayPtr(pv3array);
	pb++;
	for (i = 1; i < NGetArraySize(pv3array); i++, pb++)
	{
		pa = pb - 1;
		if (NUT_2DSegXLine((NVEC2 *)pa, (NVEC2 *)pb, plineC, plineD, pxr))
			return i;
	}
	return 0;
}

Nu32 NUT_2DSegV2ArrayXLine(const NARRAY *pv2array, const NVEC2 *plineC, const NVEC2 *plineD, NUT_2DLINE_XRESULT *pxr)
{
	Nu32 i;
	NVEC2 *pa, *pb;

	pb = (NVEC2 *)NGetFirstArrayPtr(pv2array);
	pb++;
	for (i = 1; i < NGetArraySize(pv2array); i++, pb++)
	{
		pa = pb - 1;
		if (NUT_2DSegXLine(pa, pb, plineC, plineD, pxr))
			return i;
	}
	return 0;
}

Nu8 NUT_2DSegX2DCircle(const NVEC2 *pA, const NVEC2 *pB, const NVEC2 *pC, const Nf32 radius, NUT_2DLINE_XRESULT *pR1, NUT_2DLINE_XRESULT *pR2)
{
	Nf32 dx, dy, A, B, C, det, t;
	NVEC2 vAC;
	Nu8 soluce = 0;

	dx = pB->x - pA->x;
	dy = pB->y - pA->y;

	vAC.x = pA->x - pC->x;
	vAC.y = pA->y - pC->y;

	A = dx * dx + dy * dy;
	B = 2.0f * (dx * vAC.x + dy * vAC.y);
	C = vAC.x * vAC.x + vAC.y * vAC.y - radius * radius;

	det = B * B - 4.0f * A * C;
	if ((A <= 0.0000001) || (det < 0))
	{
		// No real solutions.
		return 0;
	}
	else if (det == 0)
	{
		// One solution.
		t = -B / (2.0f * A);
		if (t >= 0 && t <= 1)
		{
			pR1->I.x = pR2->I.x = pA->x + t * dx;
			pR1->I.y = pR2->I.y = pA->y + t * dy;
			pR1->ParamCoordAB = pR2->ParamCoordAB = t;
			return 1;
		}
		else
			return 0;
	}
	else
	{
		det = sqrtf(det);
		// Two solutions.
		t = (Nf32)((-B + det) / (2.0f * A));
		if (t >= 0 && t <= 1)
		{
			pR1->I.x = pA->x + t * dx;
			pR1->I.y = pA->y + t * dy;
			pR1->ParamCoordAB = t;
			soluce++;

			t = (Nf32)((-B - det) / (2.0f * A));
			if (t >= 0 && t <= 1)
			{
				pR2->I.x = pA->x + t * dx;
				pR2->I.y = pA->y + t * dy;
				pR2->ParamCoordAB = t;
				soluce++;
			}
		}
		else
		{
			t = (Nf32)((-B - det) / (2.0f * A));
			if (t >= 0 && t <= 1)
			{
				pR1->I.x = pA->x + t * dx;
				pR1->I.y = pA->y + t * dy;
				pR1->ParamCoordAB = t;
				soluce++;
			}
		}

		return soluce;
	}
}

#include "lib/N/NCStandard.h"
#include "lib/N/GL/Ngl.h"

#include "../Containers/NArray.h"
#include "../NViewport.h"
#include "NUT_X.h"

void NUT_lineXSpline(NUT_3DLINEXSPLINE_RESULT *presult, const NVEC3 *plineA, const NVEC3 *plineB, const NSPLINE *pspline)
{
	Nu32 i, j;
	NSPLINEKNOT *spa, *spb;
	Nf32 fr, fac;

	NVEC3 C, D;
	Nf32 squaredist;

	NUT_3DLINEXLINE_RESULT xresult;

	presult->SquareDistance = NF32_MAX;
	spb = (NSPLINEKNOT *)NGetFirstArrayPtr(&pspline->KnotArray);
	spb++;
	fac = 1.0f / (Nf32)pspline->BezierAccuracy;
	for (i = 1; i < NGetArraySize(&pspline->KnotArray); i++, spb++)
	{
		spa = spb - 1;
		C = spa->Position;
		fr = fac;
		for (j = 1; j <= pspline->BezierAccuracy; j++, fr += fac)
		{
			NCasteljauPoint3f32(&D, fr, spa, spb);

			NUT_LineXSeg(&xresult, plineA, plineB, &C, &D);
			squaredist = NUT_3DLineXLineResult_Extract_EstimateShortestSquareDistance(&xresult);

			// Keep result ?
			if (squaredist < presult->SquareDistance)
			{
				presult->pSplineKnotA = spa;
				presult->pSplineKnotB = spb;
				presult->SplineKnotBIndex = i;
				presult->LerpFactorEstimatedResult = fr + fac * (xresult.ParamCoordCD - 1.0f);
				presult->SquareDistance = squaredist;
			}

			C = D;
		}
	}

	// OBSOLET
	// ... Some last calculations ...
	// presult->Distance	= sqrt(presult->Distance);
}

// ------------------------------------------------------------------------------------------
// NUT_GetClosestPointOnSpline
// ------------------------------------------------------------------------------------------
// Description :
//		Look for a point on a spline which is the closest one from the incoming reference point.
// ------------------------------------------------------------------------------------------
// In  :
//		pC:	The incoming reference point. We are testing the spline against this one.
//
// Out :
//
// ------------------------------------------------------------------------------------------
void NUT_GetClosestPointOnSpline(NUT_CLOSESTPOINT_ON_SPLINE_RESULT *presult, const NVEC3 *pC, const NSPLINE *pspline)
{
	NErrorIf(pspline->KnotArray.Size < 2, NERROR_SPLINE_NOT_ENOUGH_SPLINEKNOTS);
	Nu32 i, j;
	NSPLINEKNOT *spa, *spb;
	Nf32 fr, fac;
	NVEC3 v;

	NVEC3 A, B, AB, AC, P, AP, CP;
	Nf32 l, t;

	presult->SquareDistance = NF32_MAX;
	spb = (NSPLINEKNOT *)pspline->KnotArray.pFirst;
	spb++;
	fac = 1.0f / (Nf32)pspline->BezierAccuracy;
	for (i = 1; i < pspline->KnotArray.Size; i++, spb++)
	{
		spa = spb - 1;
		A = spa->Position;
		fr = fac;
		for (j = 1; j <= pspline->BezierAccuracy; j++, fr += fac)
		{
			NCasteljauPoint3f32(&B, fr, spa, spb);

			NVec3Sub(&AB, &B, &A);
			NVec3Sub(&AC, pC, &A);
			t = NVec3DotProduct(&AB, &AC) / (AB.x * AB.x + AB.y * AB.y + AB.z * AB.z);

			if (t < 0.0f)
			{
				// NVec3Sub(&v,pC,&A); // allready done ... here, v = AC !
				l = AC.x * AC.x + AC.y * AC.y + AC.z * AC.z;
				if (l < presult->SquareDistance)
				{
					presult->pSplineKnotA = spa;
					presult->pSplineKnotB = spb;
					presult->SplineKnotBIndex = i;
					presult->LerpFactorEstimatedResult = fr - fac;
					presult->SquareDistance = l;
					presult->VResult = A;
				}
			}
			else if (t > 1.0f)
			{
				NVec3Sub(&v, pC, &B);
				l = v.x * v.x + v.y * v.y + v.z * v.z;
				if (l < presult->SquareDistance)
				{
					presult->pSplineKnotA = spa;
					presult->pSplineKnotB = spb;
					presult->SplineKnotBIndex = i;
					presult->LerpFactorEstimatedResult = fr;
					presult->SquareDistance = l;
					presult->VResult = B;
				}
			}
			else // that means t>=0 && t<=1.0f
			{
				NVec3Scale(&AP, &AB, t);
				NVec3Add(&P, &AP, &A);
				NVec3Sub(&CP, &P, pC);

				l = CP.x * CP.x + CP.y * CP.y + CP.z * CP.z;
				if (l < presult->SquareDistance)
				{
					presult->pSplineKnotA = spa;
					presult->pSplineKnotB = spb;
					presult->SplineKnotBIndex = i;
					presult->LerpFactorEstimatedResult = fr + fac * (t - 1.0f);
					presult->SquareDistance = l;
					presult->VResult = P;
				}
			}

			A = B;
		}
	}

	// OBSOLET
	// ... Some last calculations ...
	// presult->Distance	= sqrt(presult->Distance);
}

#include "lib/N/NCStandard.h"
#include "lib/N/GL/Ngl.h"

#include "../Containers/NArray.h"
#include "../NViewport.h"
#include "NUT_X.h"

// ------------------------------------------------------------------------------------------
// Nbool NUT_3DLineXLineResult_Extract_EstimatedIntersectionPoint
// ------------------------------------------------------------------------------------------
// Description :
//	Function to translate 'NUT_3DLINEXLINE_RESULT' which is the complex result of
//	the 'NUT_LineXLine' 'NUT_3DSegXLine' and 'NUT_SegXSeg' functions.
//	Actually its very very rare for 2 lines or segments to intersect each other into 3D space.
//	So, NUT_3D..X.. functions prefer to calculate the separation segment between the 2 lines (or segments).
//	In other Words these function looking for the closest point of line A from line B and the closest point
//  of line B from line A. These 2 points represents the separating segment, (the smallest distance)  between the 2 lines.
//	Extracting, or estimate intersection point means calculate and return the middle point of this separating segment.
// ------------------------------------------------------------------------------------------
// In	:
//			NVEC3*						pintersectionpoint			valid pointer on NVEC3 which is going to receive the extracted result
//			const NUT_3DLINEXLINE_RESULT	*pprecalculatedresult		valid pointer on NUT_3DLINEXLINE_RESULT already filled up by one of the 3 NUT_3D..X.. functions.
// Out :
//			NTRUE if the separating segment length is really small which means there is a real intersection !!! VERY RARE ACTUALLY, it should never happens.
//			NFALSE otherwise (most of the cases )
// ------------------------------------------------------------------------------------------
Nbool NUT_3DLineXLineResult_Extract_EstimatedIntersectionPoint(NVEC3 *pintersectionpoint, const NUT_3DLINEXLINE_RESULT *pprecalculatedresult)
{
	// 	NVec3Add(pintersectionpoint,&pprecalculatedresult->PointOnAB,&pprecalculatedresult->PointOnCD);
	// 	NVec3Scale(pintersectionpoint,pintersectionpoint,0.5f);
	pintersectionpoint->x = 0.5f * (pprecalculatedresult->PointOnAB.x + pprecalculatedresult->PointOnCD.x);
	pintersectionpoint->y = 0.5f * (pprecalculatedresult->PointOnAB.y + pprecalculatedresult->PointOnCD.y);
	pintersectionpoint->z = 0.5f * (pprecalculatedresult->PointOnAB.z + pprecalculatedresult->PointOnCD.z);
	// optional check to indicate if the lines truly intersect
	return ((NABS(pintersectionpoint->x) + NABS(pintersectionpoint->y) + NABS(pintersectionpoint->z)) < XEPSILON ? NTRUE : NFALSE);
}

// ------------------------------------------------------------------------------------------
// Nf32 NUT_3DLineXLineResult_Extract_EstimateShortestDistance
// ------------------------------------------------------------------------------------------
// Description :
//	Function to translate 'NUT_3DLINEXLINE_RESULT' which is the complex result of
//	the 'NUT_LineXLine' 'NUT_3DSegXLine' and 'NUT_SegXSeg' functions.
//	Actually its very very rare for 2 lines or segments to intersect each other into 3D space.
//	So, NUT_3D..X.. functions prefer to calculate the separation segment between the 2 lines (or segments).
//	In other Words these function looking for the closest point of line A from line B and the closest point
//  of line B from line A. These 2 points represents the separating segment, (the smallest distance)  between the 2 lines.
//	Extracting, or estimate shortest distance means calculate and return the length of this separating segment.
// ------------------------------------------------------------------------------------------
// In	:
//			const NUT_3DLINEXLINE_RESULT	*pprecalculatedresult		valid pointer on NUT_3DLINEXLINE_RESULT already filled up by one of the 3 NUT_3D..X.. functions.
// Out :
//			Length of the separating segment describe inti the 	NUT_3DLINEXLINE_RESULT structure.
//
// ------------------------------------------------------------------------------------------
Nf32 NUT_3DLineXLineResult_Extract_EstimateShortestDistance(const NUT_3DLINEXLINE_RESULT *pprecalculatedresult)
{
	NVEC3 v;

	NVec3Sub(&v, &pprecalculatedresult->PointOnAB, &pprecalculatedresult->PointOnCD);
	return NVec3Length(&v);
}
Nf32 NUT_3DLineXLineResult_Extract_EstimateShortestSquareDistance(const NUT_3DLINEXLINE_RESULT *pprecalculatedresult)
{
	NVEC3 v;

	NVec3Sub(&v, &pprecalculatedresult->PointOnAB, &pprecalculatedresult->PointOnCD);
	return NVec3SquareLength(&v);
}
// ------------------------------------------------------------------------------------------
// void NUT_LineXLine
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_LineXLine(NUT_3DLINEXLINE_RESULT *presult, const NVEC3 *plineA, const NVEC3 *plineB, const NVEC3 *plineC, const NVEC3 *plineD)
{
	NVEC3 CA, CD, AB;
	Nf32 CACD, CDAB, CAAB, CDCD, ABAB;
	Nf32 den;

	// .....................................................................................................
	// Part 0: Line AB and Line CD definitions
	AB.x = plineB->x - plineA->x; // NVec3Sub(&AB,plineB,plineA);
	AB.y = plineB->y - plineA->y;
	AB.z = plineB->z - plineA->z;

	CD.x = plineD->x - plineC->x; // NVec3Sub(&CD,plineD,plineC);
	CD.y = plineD->y - plineC->y;
	CD.z = plineD->z - plineC->z;

	// .....................................................................................................
	// Part I: Line AB
	ABAB = AB.x * AB.x + AB.y * AB.y + AB.z * AB.z; // NVec3SquareLength(&AB) or NVec3DotProduct(&AB,&AB)
	if (ABAB < XEPSILON_SQUARED)					// AB length close to 0 ! A and B are together ! line AB is degenerated !
	{												// So we are going to deal this case like a point/line shortest distance (point A and line CD)
		presult->PointOnAB = *plineA;				// NVec3Copy(&presult->PointOnAB,plineA);
		presult->ParamCoordAB = 0.0f;
		// printf("\n* NEW_3DLineXLine: Degenerated ABAB. points A & B are together. ");

		NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&presult->PointOnCD, plineC, &CD, plineA);
		return;
	}

	// .....................................................................................................
	// Part II: Line CD
	CDCD = CD.x * CD.x + CD.y * CD.y + CD.z * CD.z; // NVec3SquareLength(&CD) or NVec3DotProduct(&CD,&CD)
	if (CDCD < XEPSILON_SQUARED)					// CD length close to 0 ! C and D are together ! line CD is degenerated !
	{												// So we are going to deal this case like a point/line shortest distance (point C and line AB)
		presult->PointOnCD = *plineC;				// NVec3Copy(&presult->PointOnCD,plineC);
		presult->ParamCoordCD = 0.0f;
		// printf("\n* NEW_3DLineXLine: Degenerated CDCD. points C & D are together. ");
		NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&presult->PointOnAB, plineA, &AB, plineC);
		return;
	}

	// .....................................................................................................
	// Part III: Line AB X Line CD
	CDAB = CD.x * AB.x + CD.y * AB.y + CD.z * AB.z; // NVec3DotProduct(&CD,&AB)
	den = ABAB * CDCD - CDAB * CDAB;

	// printf("\n ------------------------- Det = %f",den);

	if (NABS(den) < XEPSILON) // AB and CD are Parallel Lines !
	{
		// printf("\n* NEW_3DLineXLine: Lines are parallel!");
		NUT_GetClosestPointsOfParallelLines(presult, plineA, plineB, &AB, plineC, plineD, &CD);
	}
	else
	{
		CA.x = plineA->x - plineC->x; // NVec3Sub(&CA,plineA,plineC);
		CA.y = plineA->y - plineC->y;
		CA.z = plineA->z - plineC->z;

		CACD = CA.x * CD.x + CA.y * CD.y + CA.z * CD.z; // NVec3DotProduct(&CA,&CD)
		CAAB = CA.x * AB.x + CA.y * AB.y + CA.z * AB.z; // NVec3DotProduct(&CA,&AB)

		presult->ParamCoordAB = (CACD * CDAB - CAAB * CDCD) / den;
		presult->ParamCoordCD = (CACD + CDAB * presult->ParamCoordAB) / CDCD;

		presult->PointOnAB.x = plineA->x + presult->ParamCoordAB * AB.x;
		presult->PointOnAB.y = plineA->y + presult->ParamCoordAB * AB.y;
		presult->PointOnAB.z = plineA->z + presult->ParamCoordAB * AB.z;

		presult->PointOnCD.x = plineC->x + presult->ParamCoordCD * CD.x;
		presult->PointOnCD.y = plineC->y + presult->ParamCoordCD * CD.y;
		presult->PointOnCD.z = plineC->z + presult->ParamCoordCD * CD.z;
	}
}

void NUT_SegXSeg(NUT_3DLINEXLINE_RESULT *presult, const NVEC3 *psegA, const NVEC3 *psegB, const NVEC3 *psegC, const NVEC3 *psegD)
{
	NVEC3 CA, CD, AB;
	Nf32 CACD, CDAB, CAAB, CDCD, ABAB;
	Nf32 den;

	// .....................................................................................................
	// Part 0: Segment AB and Segment CD definition
	AB.x = psegB->x - psegA->x; // NVec3Sub(&AB,psegB,psegA);
	AB.y = psegB->y - psegA->y;
	AB.z = psegB->z - psegA->z;

	CD.x = psegD->x - psegC->x; // NVec3Sub(&CD,psegD,psegC);
	CD.y = psegD->y - psegC->y;
	CD.z = psegD->z - psegC->z;

	// .....................................................................................................
	// Part I: Segment AB
	ABAB = AB.x * AB.x + AB.y * AB.y + AB.z * AB.z; // NVec3SquareLength(&AB) or NVec3DotProduct(&AB,&AB)
	if (ABAB < XEPSILON_SQUARED)					// AB length close to 0 ! A and B are together ! line AB is degenerated !
	{												// So we are going to deal this case like a point/line shortest distance (point A and line CD)
		presult->PointOnAB = *psegA;				// NVec3Copy(&presult->PointOnAB,psegA);
		presult->ParamCoordAB = 0.0f;
		// printf("\n* NEW_3DSegXSeg: Degenerated ABAB. points A & B are together. ");
		NUT_GetClosestPointOnSegment((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&presult->PointOnCD, psegC, &CD, psegA);
		return;
	}

	// .....................................................................................................
	// Part II: Segment CD
	CDCD = CD.x * CD.x + CD.y * CD.y + CD.z * CD.z; // NVec3SquareLength(&CD) or NVec3DotProduct(&CD,&CD)
	if (CDCD < XEPSILON_SQUARED)					// CD length close to 0 ! C and D are together ! Segment CD is degenerated !
	{												// So we are going to deal this case like a point/line shortest distance (point C and segment AB)
		presult->PointOnCD = *psegC;				// NVec3Copy(&presult->PointOnCD,psegC);
		presult->ParamCoordCD = 0.0f;
		// printf("\n* NEW_3DSegXSeg: Degenerated CDCD. points C & D are together. ");
		NUT_GetClosestPointOnSegment((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&presult->PointOnAB, psegA, &AB, psegC);
		return;
	}

	// .....................................................................................................
	// Part III: Line AB X Line CD
	CDAB = CD.x * AB.x + CD.y * AB.y + CD.z * AB.z; // NVec3DotProduct(&CD,&AB)
	den = ABAB * CDCD - CDAB * CDAB;

	// printf("\n ------------------------- Det = %f",den);

	if (NABS(den) < XEPSILON) // AB and CD are Parallel Lines !
	{
		// printf("\n* NEW_3DLineXLine: Lines are parallel!");
		NUT_GetClosestPointsOfParallelSegments(presult, psegA, psegB, &AB, psegC, psegD, &CD);
	}
	else
	{
		CA.x = psegA->x - psegC->x; // NVec3Sub(&CA,plineA,plineC);
		CA.y = psegA->y - psegC->y;
		CA.z = psegA->z - psegC->z;

		CACD = CA.x * CD.x + CA.y * CD.y + CA.z * CD.z; // NVec3DotProduct(&CA,&CD)
		CAAB = CA.x * AB.x + CA.y * AB.y + CA.z * AB.z; // NVec3DotProduct(&CA,&AB)

		Nf32 t0 = (CACD * CDAB - CAAB * CDCD) / den;
		Nf32 t1 = (CACD + CDAB * t0) / CDCD;

		// Both t0 and t1 have to be inside [0,1] range.
		// ... check all the cases and solve them.
		// So first, handle the case with both parameters Out of Range !
		if (OUT_OF_SEGMENTRANGE(t0) && OUT_OF_SEGMENTRANGE(t1))
		{
			t0 = NMAX(0.0f, NMIN(1.0f, t0));
			presult->PointOnAB.x = psegA->x + t0 * AB.x;
			presult->PointOnAB.y = psegA->y + t0 * AB.y;
			presult->PointOnAB.z = psegA->z + t0 * AB.z;
			presult->ParamCoordAB = t0;
			NUT_GetClosestPointOnSegment((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&presult->PointOnCD, psegC, &CD, &presult->PointOnAB);
			if (OUT_OF_SEGMENTRANGE(t1))
			{
				t1 = NMAX(0.0f, NMIN(1.0f, t1));
				presult->PointOnCD.x = psegC->x + t1 * CD.x;
				presult->PointOnCD.y = psegC->y + t1 * CD.y;
				presult->PointOnCD.z = psegC->z + t1 * CD.z;
				// presult->ParamCoordCD	= t1; // not necessary here because presult->PointOnCD and presult->ParamCoordCD will be recalculated just after
				NUT_GetClosestPointOnSegment((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&presult->PointOnAB, psegA, &AB, &presult->PointOnCD);
				NUT_GetClosestPointOnSegment((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&presult->PointOnCD, psegC, &CD, &presult->PointOnAB);
			}
		}
		// ... And, otherwise, handle the case where only one segment parameter is out of range
		else if (OUT_OF_SEGMENTRANGE(t0))
		{
			t0 = NMAX(0.0f, NMIN(1.0f, t0));
			presult->PointOnAB.x = psegA->x + t0 * AB.x;
			presult->PointOnAB.y = psegA->y + t0 * AB.y;
			presult->PointOnAB.z = psegA->z + t0 * AB.z;
			presult->ParamCoordAB = t0;
			NUT_GetClosestPointOnSegment((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&presult->PointOnCD, psegC, &CD, &presult->PointOnAB);
		}
		else if (OUT_OF_SEGMENTRANGE(t1))
		{
			t1 = NMAX(0.0f, NMIN(1.0f, t1));
			presult->PointOnCD.x = psegC->x + t1 * CD.x;
			presult->PointOnCD.y = psegC->y + t1 * CD.y;
			presult->PointOnCD.z = psegC->z + t1 * CD.z;
			presult->ParamCoordCD = t1;
			NUT_GetClosestPointOnSegment((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&presult->PointOnAB, psegA, &AB, &presult->PointOnCD);
		}
		// ... Or, if none of them are out of range, just keep them !
		else
		{
			presult->PointOnAB.x = psegA->x + t0 * AB.x;
			presult->PointOnAB.y = psegA->y + t0 * AB.y;
			presult->PointOnAB.z = psegA->z + t0 * AB.z;
			presult->ParamCoordAB = t0;

			presult->PointOnCD.x = psegC->x + t1 * CD.x;
			presult->PointOnCD.y = psegC->y + t1 * CD.x;
			presult->PointOnCD.z = psegC->z + t1 * CD.x;
			presult->ParamCoordCD = t1;
		}
	}
}

void NUT_LineXSeg(NUT_3DLINEXLINE_RESULT *presult, const NVEC3 *plineA, const NVEC3 *plineB, const NVEC3 *psegC, const NVEC3 *psegD)
{
	NVEC3 CA, CD, AB;
	Nf32 CACD, CDAB, CAAB, CDCD, ABAB;
	Nf32 den;

	// .....................................................................................................
	// Part 0: Line AB and segment CD definition
	AB.x = plineB->x - plineA->x; // NVec3Sub(&AB,plineB,plineA);
	AB.y = plineB->y - plineA->y;
	AB.z = plineB->z - plineA->z;

	CD.x = psegD->x - psegC->x; // NVec3Sub(&CD,psegD,psegC);
	CD.y = psegD->y - psegC->y;
	CD.z = psegD->z - psegC->z;

	// .....................................................................................................
	// Part I: Line AB
	ABAB = AB.x * AB.x + AB.y * AB.y + AB.z * AB.z; // NVec3SquareLength(&AB) or NVec3DotProduct(&AB,&AB)
	if (ABAB < XEPSILON_SQUARED)					// AB length close to 0 ! A and B are together ! line AB is degenerated !
	{												// So we are going to deal this case like a point/Segment shortest distance (point A and segment CD)
		presult->PointOnAB = *plineA;				// NVec3Copy(&presult->PointOnAB,plineA);
		presult->ParamCoordAB = 0.0f;
		// printf("\n* NEW_3DLineXSeg: Degenerated ABAB. points A & B are together. ");
		NUT_GetClosestPointOnSegment((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&presult->PointOnCD, psegC, &CD, plineA);
		return;
	}

	// .....................................................................................................
	// Part II: Line CD
	CDCD = CD.x * CD.x + CD.y * CD.y + CD.z * CD.z; // NVec3SquareLength(&CD) or NVec3DotProduct(&CD,&CD)
	if (CDCD < XEPSILON_SQUARED)					// CD length close to 0 ! C and D are together ! Segment CD is degenerated !
	{												// So we are going to deal this case like a point/line shortest distance (point C and line AB)
		presult->PointOnCD = *psegC;				// NVec3Copy(&presult->PointOnCD,psegC);
		presult->ParamCoordCD = 0.0f;
		// printf("\n* NEW_3DLineXSeg: Degenerated CDCD. points C & D are together. ");
		NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&presult->PointOnAB, plineA, &AB, psegC);
		return;
	}

	// .....................................................................................................
	// Part III: Line AB X Line CD
	CDAB = CD.x * AB.x + CD.y * AB.y + CD.z * AB.z; // NVec3DotProduct(&CD,&AB)
	den = ABAB * CDCD - CDAB * CDAB;

	// printf("\n ------------------------- Det = %f",den);

	if (NABS(den) < XEPSILON) // AB and CD are Parallel Lines !
	{
		// printf("\n* NEW_3DLineXSeg: Lines are parallel!");
		NUT_GetClosestPointsOfParallelLineSegment(presult, plineA, plineB, &AB, psegC, psegD, &CD);
	}
	else
	{
		CA.x = plineA->x - psegC->x; // NVec3Sub(&CA,plineA,psegC);
		CA.y = plineA->y - psegC->y;
		CA.z = plineA->z - psegC->z;

		CACD = CA.x * CD.x + CA.y * CD.y + CA.z * CD.z; // NVec3DotProduct(&CA,&CD)
		CAAB = CA.x * AB.x + CA.y * AB.y + CA.z * AB.z; // NVec3DotProduct(&CA,&AB)

		presult->ParamCoordAB = (CACD * CDAB - CAAB * CDCD) / den;
		presult->PointOnAB.x = plineA->x + presult->ParamCoordAB * AB.x;
		presult->PointOnAB.y = plineA->y + presult->ParamCoordAB * AB.y;
		presult->PointOnAB.z = plineA->z + presult->ParamCoordAB * AB.z;

		Nf32 t1 = (CACD + CDAB * presult->ParamCoordAB) / CDCD;
		if (OUT_OF_SEGMENTRANGE(t1))
		{
			t1 = NMAX(0.0f, NMIN(1.0f, t1));
			presult->PointOnCD.x = psegC->x + t1 * CD.x;
			presult->PointOnCD.y = psegC->y + t1 * CD.y;
			presult->PointOnCD.z = psegC->z + t1 * CD.z;
			presult->ParamCoordCD = t1;
			NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&presult->PointOnAB, plineA, &AB, &presult->PointOnCD);
		}
		else
		{
			presult->PointOnCD.x = psegC->x + t1 * CD.x;
			presult->PointOnCD.y = psegC->y + t1 * CD.y;
			presult->PointOnCD.z = psegC->z + t1 * CD.z;
			presult->ParamCoordCD = t1;
		}
	}
}
// ------------------------------------------------------------------------------------------
// void NUT_GetClosestPointOnLine
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_GetClosestPointOnLine(NUT_CLOSESTPOINT_ON_LINE_RESULT *presult, const NVEC3 *pA, const NVEC3 *pdirvect_lineAB, const NVEC3 *pC)
{
	Nf32 ABAB;

	ABAB = pdirvect_lineAB->x * pdirvect_lineAB->x + pdirvect_lineAB->y * pdirvect_lineAB->y + pdirvect_lineAB->z * pdirvect_lineAB->z; // NVec3SquareLength(pdirvect_lineAB);
	if (ABAB < XEPSILON_SQUARED)
	{
		// printf("\n* NUT_GetClosestPointOnLine: Degenerated ABAB. points A & B are together.!");
		presult->PointOnLine = *pA;
		presult->ParamCoord = 0.0f;
		return;
	}

	NVEC3 AC;
	AC.x = pC->x - pA->x; // NVec3Sub(&AC,pC,pA);
	AC.y = pC->y - pA->y;
	AC.z = pC->z - pA->z;
	presult->ParamCoord = (pdirvect_lineAB->x * AC.x + pdirvect_lineAB->y * AC.y + pdirvect_lineAB->z * AC.z) / ABAB; // NVec3DotProduct(pdirvect_lineAB,&AC) / ABAB;
	presult->PointOnLine.x = pA->x + presult->ParamCoord * pdirvect_lineAB->x;
	presult->PointOnLine.y = pA->y + presult->ParamCoord * pdirvect_lineAB->y;
	presult->PointOnLine.z = pA->z + presult->ParamCoord * pdirvect_lineAB->z;
}

void NUT_GetClosestPointOnSegment(NUT_CLOSESTPOINT_ON_LINE_RESULT *presult, const NVEC3 *pA, const NVEC3 *pdirvect_segAB, const NVEC3 *pC)
{
	Nf32 ABAB;

	ABAB = pdirvect_segAB->x * pdirvect_segAB->x + pdirvect_segAB->y * pdirvect_segAB->y + pdirvect_segAB->z * pdirvect_segAB->z; // NVec3SquareLength(pdirvect_segAB);
	if (ABAB < XEPSILON_SQUARED)
	{
		presult->PointOnLine.x = pA->x;
		presult->PointOnLine.y = pA->y;
		presult->PointOnLine.z = pA->z;
		presult->ParamCoord = 0.0f;
		return;
	}

	NVEC3 AC;
	AC.x = pC->x - pA->x; // NVec3Sub(&AC,pC,pA);
	AC.y = pC->y - pA->y;
	AC.z = pC->z - pA->z;

	presult->ParamCoord = NMAX(0.0f, NMIN(1.0f, (pdirvect_segAB->x * AC.x + pdirvect_segAB->y * AC.y + pdirvect_segAB->z * AC.z) / ABAB)); // NMAX(0.0f,NMIN(1.0f, NVec3DotProduct(pdirvect_segAB,&AC)/d));
	presult->PointOnLine.x = pA->x + presult->ParamCoord * pdirvect_segAB->x;
	presult->PointOnLine.y = pA->y + presult->ParamCoord * pdirvect_segAB->y;
	presult->PointOnLine.z = pA->z + presult->ParamCoord * pdirvect_segAB->z;
}

void NUT_GetClosestPointsOfParallelLines(NUT_3DLINEXLINE_RESULT *presult, const NVEC3 *plineA, const NVEC3 *plineB, const NVEC3 *pdirvect_lineAB, const NVEC3 *plineC, const NVEC3 *plineD, const NVEC3 *pdirvect_lineCD)
{
	NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&presult->PointOnAB, plineA, pdirvect_lineAB, plineC);
	presult->PointOnCD = *plineC; // NVec3Copy(&presult->PointOnCD,plineC);
	presult->ParamCoordCD = 0.0f;
}

void NUT_GetClosestPointsOfParallelSegments(NUT_3DLINEXLINE_RESULT *presult, const NVEC3 *psegA, const NVEC3 *psegB, const NVEC3 *pdirvect_segAB, const NVEC3 *psegC, const NVEC3 *psegD, const NVEC3 *pdirvect_segCD)
{
	NUT_CLOSESTPOINT_ON_LINE_RESULT result1, result2;
	Nf32 t;

	NUT_GetClosestPointOnLine(&result1, psegA, pdirvect_segAB, psegC);
	NUT_GetClosestPointOnLine(&result2, psegA, pdirvect_segAB, psegD);

	if (result1.ParamCoord < 0.f && result2.ParamCoord < 0.f)
	{
		presult->PointOnAB = *psegA; // NVec3Copy(&presult->PointOnAB,psegA);
		presult->ParamCoordAB = 0.0f;

		if (result1.ParamCoord < result2.ParamCoord)
		{
			presult->PointOnCD = *psegD; // NVec3Copy(&presult->PointOnCD,psegD);
			presult->ParamCoordCD = 1.0f;
		}
		else
		{
			presult->PointOnCD = *psegC; // NVec3Copy(&presult->PointOnCD,psegC);
			presult->ParamCoordCD = 0.0f;
		}
	}
	else if (result1.ParamCoord > 1.f && result2.ParamCoord > 1.f)
	{
		presult->PointOnAB = *psegB; // NVec3Copy(&presult->PointOnAB,psegB);
		presult->ParamCoordAB = 1.0f;

		if (result1.ParamCoord < result2.ParamCoord)
		{
			presult->PointOnCD = *psegC; // NVec3Copy(&presult->PointOnCD,psegC);
			presult->ParamCoordCD = 0.0f;
		}
		else
		{
			presult->PointOnCD = *psegD; // NVec3Copy(&presult->PointOnCD,psegD);
			presult->ParamCoordCD = 1.0f;
		}
	}
	else
	{
		t = 0.5f * (NMAX(0.0f, NMIN(1.0f, result1.ParamCoord)) + NMAX(0.0f, NMIN(1.0f, result2.ParamCoord)));
		presult->PointOnAB.x = (psegA->x + t * pdirvect_segAB->x);
		presult->PointOnAB.y = (psegA->y + t * pdirvect_segAB->y);
		presult->PointOnAB.z = (psegA->z + t * pdirvect_segAB->z);
		presult->ParamCoordAB = t;

		NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&presult->PointOnCD, psegC, pdirvect_segCD, &presult->PointOnAB);
	}
}

// ------------------------------------------------------------------------------------------
// void NUT_GetClosestPointOnLine
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
//
//
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_2DGetClosestPointOnLine(NUT_2DCLOSESTPOINT_ON_LINE_RESULT *presult, const NVEC2 *pA, const NVEC2 *pdirvect_lineAB, const NVEC2 *pC)
{
	Nf32 ABAB;

	ABAB = pdirvect_lineAB->x * pdirvect_lineAB->x + pdirvect_lineAB->y * pdirvect_lineAB->y; // NVec2SquareLength(pdirvect_lineAB);
	if (ABAB < XEPSILON_SQUARED)
	{
		// printf("\n* NUT_GetClosestPointOnLine: Degenerated ABAB. points A & B are together.!");
		presult->I = *pA;
		presult->ParamCoordAB = 0.0f;
		return;
	}

	NVEC2 AC;
	AC.x = pC->x - pA->x; // NVec3Sub(&AC,pC,pA);
	AC.y = pC->y - pA->y;
	presult->ParamCoordAB = (pdirvect_lineAB->x * AC.x + pdirvect_lineAB->y * AC.y) / ABAB; // NVec2DotProduct(pdirvect_lineAB,&AC) / ABAB;
	presult->I.x = pA->x + presult->ParamCoordAB * pdirvect_lineAB->x;
	presult->I.y = pA->y + presult->ParamCoordAB * pdirvect_lineAB->y;
}

void NUT_2DGetClosestPointOnSegment(NUT_2DCLOSESTPOINT_ON_LINE_RESULT *presult, const NVEC2 *pA, const NVEC2 *pdirvect_segAB, const NVEC2 *pC)
{
	Nf32 ABAB;

	ABAB = pdirvect_segAB->x * pdirvect_segAB->x + pdirvect_segAB->y * pdirvect_segAB->y; // NVec2SquareLength(pdirvect_segAB);
	if (ABAB < XEPSILON_SQUARED)
	{
		presult->I = *pA;
		presult->ParamCoordAB = 0.0f;
		return;
	}

	NVEC2 AC;
	AC.x = pC->x - pA->x;
	AC.y = pC->y - pA->y;

	presult->ParamCoordAB = NMAX(0.0f, NMIN(1.0f, (pdirvect_segAB->x * AC.x + pdirvect_segAB->y * AC.y) / ABAB)); // NMAX(0.0f,NMIN(1.0f, NVec2DotProduct(pdirvect_segAB,&AC)/ABAB));
	presult->I.x = pA->x + presult->ParamCoordAB * pdirvect_segAB->x;
	presult->I.y = pA->y + presult->ParamCoordAB * pdirvect_segAB->y;
}

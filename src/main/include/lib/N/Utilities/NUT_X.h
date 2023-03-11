#ifndef __NUT_INTERSECTION_H
#define __NUT_INTERSECTION_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NUT_Intersection.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/Maths/NVec3f32.h"
#include "lib/N/NBoundingVolumes.h"
#include "lib/N/Containers/NArray.h"
#include "lib/N/NSpline.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// ***************************************************************************************************************************************************************
	// ***************************************************************************************************************************************************************
	// **																																							**
	// **														GLOBAL and GENERIC DEFINEs																			**
	// **																																							**
	// ***************************************************************************************************************************************************************
	// ***************************************************************************************************************************************************************

#define XEPSILON 0.000001f					// (X for Cross !) tolerance value used to check some result against it (if value < XEPSILON means value = 0 !) used to check for degenerate lines, or parallel lines
#define XEPSILON_SQUARED XEPSILON *XEPSILON // Cristal clear
#define OUT_OF_SEGMENTRANGE(a) ((a) < 0.0f || (a) > 1.f)

	// Segment X Result. Used by all the '2DSegX' functions
	typedef struct
	{
		NVEC2f32 I;		   // 2D Intersection Point.
		Nf32 ParamCoordAB; // Normalized ratio to place I (intersection point) like this: I = A+t*AB
	} NUT_2DLINE_XRESULT;
	typedef NUT_2DLINE_XRESULT NUT_2DCLOSESTPOINT_ON_LINE_RESULT;

	typedef struct
	{

		NVEC2f32 I;		   // 2D Intersection Point.
		Nf32 ParamCoordAB; // Normalized ratio to place I (intersection point) like this: I = A+t*AB
		Nf32 ParamCoordCD; // Normalized ratio to place I (intersection point) like this: I = C+t2*CD, only used by "Seg X Seg"
	} NUT_2DLINE_EXTEND_XRESULT;

	// Used by advanced Crossing functions like:
	//						NUT_2DVerticalLineXSplineExtended

	typedef struct
	{
		NUT_2DLINE_XRESULT XResult; // Intersection Point 2D coordinates
		// --------------------------------------
		// NSPLINEKNOT			*pSplineKnot0;
		NSPLINEKNOT *pSplineKnot1;
		// --------------------------------------
		Nf32 SegA_BezierParamCoord; //
		Nf32 SegB_BezierParamCoord; //
		NVEC3 SegA_Position;		//
		NVEC3 SegB_Position;		//
									// --------------------------------------
	} NUT_2DSPLINE_XRESULT;

	typedef struct
	{
		NUT_2DLINE_XRESULT XResult; // Intersection Point 2D coordinates
		// --------------------------------------
		// NSPLINEKNOT			*pSplineKnot0;
		NSPLINEKNOT *pSplineKnot1;
		// --------------------------------------
		Nf32 SegA_BezierParamCoord; //
		Nf32 SegB_BezierParamCoord; //
		NVEC3 SegA_Position;		//
		NVEC3 SegB_Position;		//
		// --------------------------------------
		NSPLINE *pSpline;
	} NUT_2DSPLINE_PERSISTENT_XRESULT;

	// Result Data: used by 'NUT_GetClosestPointOnSpline'
	typedef struct
	{
		NSPLINEKNOT *pSplineKnotA;
		NSPLINEKNOT *pSplineKnotB;
		Nu32 SplineKnotBIndex; //... is  not necessary to store KnotA because it's obvious: A = B-1. Always !

		Nf32 LerpFactorEstimatedResult;
		Nf32 SquareDistance;
		NVEC3 VResult;
	} NUT_CLOSESTPOINT_ON_SPLINE_RESULT;

	// Result Data: used by 'NUT_GetClosestPointOnLine' 'NUT_GetClosestPointOnSegment'
	typedef struct
	{
		NVEC3 PointOnLine;
		Nf32 ParamCoord;
	} NUT_CLOSESTPOINT_ON_LINE_RESULT;
	// Result Data: used by 'NUT_LineXLine' 'NUT_LineXSeg' 'NUT_SegXSeg'
	typedef struct
	{
		// ----------------------
		NVEC3 PointOnAB;   // |__ NUT_CLOSESTPOINT_ON_LINE_RESULT;
		Nf32 ParamCoordAB; // |
		// ----------------------
		NVEC3 PointOnCD;   // |__ NUT_CLOSESTPOINT_ON_LINE_RESULT;
		Nf32 ParamCoordCD; // |
						   // ----------------------
						   // NVEC3	NearestPoint;
	} NUT_3DLINEXLINE_RESULT;

	// Result Data: used by 'NUT_GetClosestPointOnSpline'
	typedef struct
	{
		NSPLINEKNOT *pSplineKnotA;
		NSPLINEKNOT *pSplineKnotB;
		Nu32 SplineKnotBIndex; //... is  not necessary to store KnotA because it's obvious: A = B-1. Always !

		Nf32 LerpFactorEstimatedResult;
		Nf32 SquareDistance;
		NUT_3DLINEXLINE_RESULT XResult;
	} NUT_3DLINEXSPLINE_RESULT;

	// ***************************************************************************************************************************************************************
	// ***************************************************************************************************************************************************************
	// **																																							**
	// **																			FUNCTIONS																		**
	// **																																							**
	// ***************************************************************************************************************************************************************
	// ***************************************************************************************************************************************************************
	NUT_2DSPLINE_PERSISTENT_XRESULT *NUT_Setup2DSplinePersistentXResult(NUT_2DSPLINE_PERSISTENT_XRESULT *pprs, const NSPLINE *pspline);
	NUT_2DSPLINE_PERSISTENT_XRESULT *NUT_Create2DSplinePersistentXResult(const NSPLINE *pspline);
	void NUT_Clear2DSplinePersistentXResult(void *pobj);
	void NUT_Delete2DSplinePersistentXResult(void *pobj);
	inline NUT_2DSPLINE_PERSISTENT_XRESULT *NUT_Reset2DSplinePersistentXResult(NUT_2DSPLINE_PERSISTENT_XRESULT *pprs) { return NUT_Setup2DSplinePersistentXResult(pprs, pprs->pSpline); } // Reset XResult to it's initial state, with 'cursor' at the beginning of the spline

	Nbool NUT_2DSegXBezierFastAABB(const NVEC2 *psegA, const NVEC2 *psegB, const NSPLINEKNOT *spa, const NSPLINEKNOT *spb);
	Nbool NUT_2DSegXSeg(const NVEC2 *A, const NVEC2 *B, const NVEC2 *C, const NVEC2 *D, NUT_2DLINE_EXTEND_XRESULT *pxrx);
	Nbool NUT_2DSegXLine(const NVEC2 *psegA, const NVEC2 *psegB, const NVEC2 *plineC, const NVEC2 *plineD, NUT_2DLINE_XRESULT *pxr);
	Nbool NUT_2DSegXSpline(const NVEC2 *psegA, const NVEC2 *psegB, const NSPLINE *pspline, NUT_2DLINE_EXTEND_XRESULT *pxrx);
	Nbool NUT_2DSegXVerticalLine(const NVEC2 *psegA, const NVEC2 *psegB, const NVEC2 *plineC, NUT_2DLINE_XRESULT *pxr);
	Nbool NUT_2DSegXHorizontalLine(const NVEC2 *psegA, const NVEC2 *psegB, const NVEC2 *plineC, NUT_2DLINE_XRESULT *pxr);
	Nu8 NUT_2DSegX2DCircle(const NVEC2 *pA, const NVEC2 *pB, const NVEC2 *pC, const Nf32 radius, NUT_2DLINE_XRESULT *pR1, NUT_2DLINE_XRESULT *pR2);

	Nu32 NUT_2DSegV2ArrayXLine(const NARRAY *pv2array, const NVEC2 *plineC, const NVEC2 *plineD, NUT_2DLINE_XRESULT *pxr);
	Nu32 NUT_2DSegV2ArrayXVerticalLine(const NARRAY *pv2array, const NVEC2 *plineC, NUT_2DLINE_XRESULT *pxr);
	Nu32 NUT_2DSegV3ArrayXLine(const NARRAY *pv3array, const NVEC2 *plineC, const NVEC2 *plineD, NUT_2DLINE_XRESULT *pxr);
	Nu32 NUT_2DSegV3ArrayXVerticalLine(const NARRAY *pv3array, const NVEC2 *plineC, NUT_2DLINE_XRESULT *pxr);

	Nbool NUT_2DLineXLine(const NVEC2 *A, const NVEC2 *B, const NVEC2 *C, const NVEC2 *D, NUT_2DLINE_XRESULT *pxr);
	Nbool NUT_2DLineXLine_VDir(const NVEC2 *A, const NVEC2 *ADir, const NVEC2 *C, const NVEC2 *CDir, NUT_2DLINE_XRESULT *pxr);

	Nbool NUT_2DLineXSpline(const NVEC2 *plineA, const NVEC2 *plineB, const NSPLINE *pspline, NUT_2DSPLINE_XRESULT *pxr);
	Nbool NUT_2DVerticalLineXSpline(const NVEC2 *plineA, const NSPLINE *pspline, NUT_2DSPLINE_XRESULT *pxr);
	Nbool NUT_2DHorizontalLineXSpline(const NVEC2 *plineA, const NSPLINE *pspline, NUT_2DSPLINE_XRESULT *pxr);
	Nbool NUT_2DVerticalLineXSplineExtended(const NVEC2 *plineA, NUT_2DSPLINE_PERSISTENT_XRESULT *ppersistent);
	Nbool NUT_2DHorizontalLineXSplineExtended(const NVEC2 *plineA, NUT_2DSPLINE_PERSISTENT_XRESULT *ppersistent);

	Nbool NUT_2DVerticalLineXBezierFastAABB(const NVEC2 *plineA, const NSPLINEKNOT *spa, const NSPLINEKNOT *spb);
	Nbool NUT_2DHorizontalLineXBezierFastAABB(const NVEC2 *plineA, const NSPLINEKNOT *spa, const NSPLINEKNOT *spb);

	void NUT_2DGetClosestPointOnLine(NUT_2DCLOSESTPOINT_ON_LINE_RESULT *presult, const NVEC2 *pA, const NVEC2 *pdirvect_lineAB, const NVEC2 *pC);
	void NUT_2DGetClosestPointOnSegment(NUT_2DCLOSESTPOINT_ON_LINE_RESULT *presult, const NVEC2 *pA, const NVEC2 *pdirvect_segAB, const NVEC2 *pC);

	Nbool NUT_LineXAABB(const NVEC3 *pline_o, const NVEC3 *pline_dir, const NAABB *paabb);
	Nbool NUT_LineXPLane(const NVEC3 *plineA, const NVEC3 *plinedir, const NVEC3 *pplaneP, const NVEC3 *pplanedir, NVEC3 *presult);
	Nf32 NUT_LinePointShortestDistance(const NVEC3 *pline_o, const NVEC3 *pline_dir, const NVEC3 *ppoint);
	Nf32 NUT_LinePointShortestSquareDistance(const NVEC3 *pline_o, const NVEC3 *pline_dir, const NVEC3 *ppoint);

	void NUT_lineXSpline(NUT_3DLINEXSPLINE_RESULT *presult, const NVEC3 *plineA, const NVEC3 *plineB, const NSPLINE *pspline);
	void NUT_LineXLine(NUT_3DLINEXLINE_RESULT *presult, const NVEC3 *plineA, const NVEC3 *plineB, const NVEC3 *plineC, const NVEC3 *plineD);
	void NUT_LineXSeg(NUT_3DLINEXLINE_RESULT *presult, const NVEC3 *plineA, const NVEC3 *plineB, const NVEC3 *psegC, const NVEC3 *psegD);
	void NUT_SegXSeg(NUT_3DLINEXLINE_RESULT *presult, const NVEC3 *plineA, const NVEC3 *plineB, const NVEC3 *plineC, const NVEC3 *plineD);

	void NUT_GetClosestPointOnLine(NUT_CLOSESTPOINT_ON_LINE_RESULT *presult, const NVEC3 *pA, const NVEC3 *pdirvect_lineAB, const NVEC3 *pC);
	void NUT_GetClosestPointOnSegment(NUT_CLOSESTPOINT_ON_LINE_RESULT *presult, const NVEC3 *pA, const NVEC3 *pdirvect_segAB, const NVEC3 *pC);

	void NUT_GetClosestPointsOfParallelLines(NUT_3DLINEXLINE_RESULT *presult, const NVEC3 *plineA, const NVEC3 *plineB, const NVEC3 *pdirvect_lineAB, const NVEC3 *plineC, const NVEC3 *plineD, const NVEC3 *pdirvect_lineCD);
	void NUT_GetClosestPointsOfParallelSegments(NUT_3DLINEXLINE_RESULT *presult, const NVEC3 *psegA, const NVEC3 *psegB, const NVEC3 *pdirvect_segAB, const NVEC3 *psegC, const NVEC3 *psegD, const NVEC3 *pdirvect_segCD);
#define NUT_GetClosestPointsOfParallelLineSegment(presult, plineA, plineB, pdirvect_lineAB, psegC, psegD, pdirvect_segCD) NUT_GetClosestPointsOfParallelLines(presult, plineA, plineB, pdirvect_lineAB, psegC, psegD, pdirvect_segCD)

	void NUT_GetClosestPointOnSpline(NUT_CLOSESTPOINT_ON_SPLINE_RESULT *presult, const NVEC3 *pC, const NSPLINE *pspline);

	// NUT_3DLINEXLINE_RESULT "details" extraction
	//---------------------------------------------
	Nbool NUT_3DLineXLineResult_Extract_EstimatedIntersectionPoint(NVEC3 *pintersectionpoint, const NUT_3DLINEXLINE_RESULT *pprecalculatedresult);
	Nf32 NUT_3DLineXLineResult_Extract_EstimateShortestDistance(const NUT_3DLINEXLINE_RESULT *pprecalculatedresult);
	Nf32 NUT_3DLineXLineResult_Extract_EstimateShortestSquareDistance(const NUT_3DLINEXLINE_RESULT *pprecalculatedresult);

	// NUT_2DSPLINE_XRESULT "details" extraction
	//---------------------------------------------
	inline Nf32 NUT_2DSplineXResult_Extract_EstimatedBezierParamCoord(const NUT_2DSPLINE_XRESULT *pxr) { return pxr->SegA_BezierParamCoord + (pxr->SegB_BezierParamCoord - pxr->SegA_BezierParamCoord) * pxr->XResult.ParamCoordAB; }

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NUT_H

#include "lib/N/NCStandard.h"
#include "lib/N/NCoordinateSystemConversions.h"
#include "lib/N/Utilities/NUT_X.h"
#include "lib/N/Utilities/NUT_Spline.h"

#include "NUI.h"
#include "NUISlide.h"

void NSetUISlide_Constraint_None(NUI_SLIDE *pslayer)
{
	Nmem0(&pslayer->Constraint, sizeof(NUI_SLIDE_CONSTRAINT));
	pslayer->Constraint.ApplyConstraint_Handle = NApplyUISlideConstraints_Basic;
}

void NSetUISlide_Constraint_Segment(NUI_SLIDE *pslayer, const NVEC3 *psegment0, const NVEC3 *psegment1, const Nf32 Nf32_safedistance, const Nbool _safe_dist_is_relative)
{
	NErrorIf(!psegment0 || !psegment1, NERROR_NULL_POINTER);

	NUT_CLOSESTPOINT_ON_LINE_RESULT result;
	NVEC3 vseg01;
	Nf32 length;

	Nmem0(&pslayer->Constraint, sizeof(NUI_SLIDE_CONSTRAINT));

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Setup Constraint.Segment
	// +
	pslayer->Constraint.ApplyConstraint_Handle = NApplyUISlideConstraints_Segment;
	pslayer->Constraint.Segment.SegmentPoint0 = *psegment0;
	pslayer->Constraint.Segment.SegmentPoint1 = *psegment1;

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + from Nf32_safedistance to pslayer.SafeSegmentRatio
	// +
	// + for segment, pslayer.SafeSegmentRatio will represents a ratio distance on the segment.
	// + So, lets convert Nf32_safedistance into segment relative ratio.
	if (_safe_dist_is_relative)
	{
		pslayer->Constraint.Segment.SafeSegmentRatio = NCLAMP(0.0f, Nf32_safedistance, 0.5f);
	}
	else
	{
		NVec3Sub(&vseg01, psegment1, psegment0);
		length = NVec3Length(&vseg01);

		NErrorIf(length == 0.0f, NERROR_NULL_VALUE);
		// Min ratio is ZERO (no safe distance) to 0.5f (max safe distance, which is the half part of the segment for each side )
		pslayer->Constraint.Segment.SafeSegmentRatio = NCLAMP(0.0f, Nf32_safedistance / length, 0.5f);
	}

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Apply constraint segment to current Slide Position.
	// +
	// Find the closest point on Segment of the current sliding position
	NUT_GetClosestPointOnSegment(&result, psegment0, &vseg01, &pslayer->SlidePos);
	// And apply constraint for the first time
	pslayer->SlidePos = result.PointOnLine;
	pslayer->OldSlidePos = pslayer->SlidePos;
	NSetUIPosition((NUI *)pslayer, &pslayer->SlidePos);
}

void NSetUISlide_Constraint_Path(NUI_SLIDE *pslayer, const NSPLINE *ppath, const Nf32 Nf32_safedistance)
{
	NErrorIf(!ppath, NERROR_NULL_POINTER);

	NVEC3 v;
	Nf32 l;
	Nf32 step, factor;

	Nmem0(&pslayer->Constraint, sizeof(NUI_SLIDE_CONSTRAINT));

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Setup Constraint.Spline
	// +
	pslayer->Constraint.ApplyConstraint_Handle = NApplyUISlideConstraints_Spline;
	pslayer->Constraint.Spline.pSpline = (NSPLINE *)ppath;

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + from Nf32_safedistance to pslayer.SafeDistance
	// +
	// + for Spline, pslayer.SafeDistance will represents ratio distance on the spline.
	// + and because Spline is not like segment and doesn't use linear interpolation, we need to convert
	// + Nf32_safedistance into 2 different  relative ratios, one for the start safe distance (from the beginning of the spline).
	// + and a second one for the End safe distance, from the end of the spline.
	// +
	step = 1.0f / ppath->BezierAccuracy;
	NCasteljauPoint3f32(&v, step, NGetFirstSplineKnot(ppath), NGetFirstSplineKnot(ppath) + 1);
	NVec3SubFrom(&v, &NGetFirstSplineKnot(ppath)->Position);
	l = NVec3Length(&v);
	NErrorIf(l <= NF32_EPSILON_VECTOR_LENGTH, NERROR_VALUE_CLOSE_TO_ZERO);
	NVec3ScaleBy(&v, Nf32_safedistance / l);
	NVec3AddTo(&v, &NGetFirstSplineKnot(ppath)->Position);
	factor = 0.0f;
	NUT_WrapOnSpline(&v, &factor, &step, ppath);
	NErrorIf(factor >= NSplineFactor_NormXtdToXtd(1.0f, ppath) * 0.5f, NERROR_INCONSISTENT_PARAMETERS);
	pslayer->Constraint.Spline.SafeSplineRatioStart = factor;

	NCasteljauPoint3f32(&v, step, NGetLastSplineKnot(ppath) - 1, NGetLastSplineKnot(ppath));
	NVec3SubFrom(&v, &NGetLastSplineKnot(ppath)->Position);
	l = NVec3Length(&v);
	NErrorIf(l <= NF32_EPSILON_VECTOR_LENGTH, NERROR_VALUE_CLOSE_TO_ZERO);
	NVec3ScaleBy(&v, Nf32_safedistance / l);
	NVec3AddTo(&v, &NGetLastSplineKnot(ppath)->Position);
	factor = NSplineFactor_NormXtdToXtd(1.0f, ppath);
	step = -step;
	NUT_WrapOnSpline(&v, &factor, &step, ppath);
	NErrorIf(factor <= NSplineFactor_NormXtdToXtd(1.0f, ppath) * 0.5f, NERROR_INCONSISTENT_PARAMETERS);
	pslayer->Constraint.Spline.SafeSplineRatioEnd = factor;

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Apply constraint spline to current Slide Position.
	// +

	// + Find the closest point on spline of the current sliding position
	NUT_CLOSESTPOINT_ON_SPLINE_RESULT result;
	NUT_GetClosestPointOnSpline(&result, &pslayer->SlidePos, ppath);
	// And apply constraint for the first time
	pslayer->Constraint.Spline.TCurrentXtdFactor = NSplineFactor_FactorToXtd(result.pSplineKnotA, result.LerpFactorEstimatedResult, ppath);
	pslayer->SlidePos = result.VResult;
	pslayer->OldSlidePos = pslayer->SlidePos;
	NSetUIPosition((NUI *)pslayer, &pslayer->SlidePos);
}
void NSetUISlide_Constraint_AABB(NUI_SLIDE *pslayer, const NAABB *paabb, const Nf32 Nf32_safedistance_x, const Nf32 Nf32_safedistance_y, const Nf32 Nf32_safedistance_z)
{
	NErrorIf(!paabb, NERROR_NULL_POINTER);

	Nmem0(&pslayer->Constraint, sizeof(NUI_SLIDE_CONSTRAINT));

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Setup Constraint.Spline
	// +
	pslayer->Constraint.ApplyConstraint_Handle = NApplyUISlideConstraints_AABB;
	pslayer->Constraint.AABB.pAABB = (NAABB *)paabb;

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + from Nf32_safedistance to pslayer.Constraint.AABB.SafeAABBDistance
	// +
	// + for AABB, pslayer.Constraint.AABB.SafeAABBDistance is simply ... Nf32_safedistance.
	// + (we keep the 3 component to be able, it it seems necessary to make the 3 different ...)
	pslayer->Constraint.AABB.SafeAABBDistance.x = Nf32_safedistance_x;
	pslayer->Constraint.AABB.SafeAABBDistance.y = Nf32_safedistance_y;
	pslayer->Constraint.AABB.SafeAABBDistance.z = Nf32_safedistance_z;

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Apply constraint AABB to current Slide Position.
	// +
	// Check it against AABB
	pslayer->SlidePos.x = NCLAMP(pslayer->Constraint.AABB.pAABB->fXMin, pslayer->SlidePos.x, pslayer->Constraint.AABB.pAABB->fXMax);
	pslayer->SlidePos.y = NCLAMP(pslayer->Constraint.AABB.pAABB->fYMin, pslayer->SlidePos.y, pslayer->Constraint.AABB.pAABB->fYMax);
	pslayer->SlidePos.z = NCLAMP(pslayer->Constraint.AABB.pAABB->fZMin, pslayer->SlidePos.z, pslayer->Constraint.AABB.pAABB->fZMax);
	pslayer->OldSlidePos = pslayer->SlidePos;
	NSetUIPosition((NUI *)pslayer, (NVEC3 *)&pslayer->SlidePos);
}

Nu32 NApplyUISlideConstraints_Basic(NUI_SLIDE *pslayer)
{
	return 1;
}

Nu32 NApplyUISlideConstraints_AABB(NUI_SLIDE *psl)
{
	Nu32 ret = 1;

	// ++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + 1) Position vs Constraints
	// +
	if (psl->SlidePos.x < psl->Constraint.AABB.pAABB->fXMin)
	{
		psl->SlidePos.x = psl->Constraint.AABB.pAABB->fXMin;
		ret = 0;
	}
	else if (psl->SlidePos.x > psl->Constraint.AABB.pAABB->fXMax)
	{
		psl->SlidePos.x = psl->Constraint.AABB.pAABB->fXMax;
		ret = 0;
	}

	if (psl->SlidePos.y < psl->Constraint.AABB.pAABB->fYMin)
	{
		psl->SlidePos.y = psl->Constraint.AABB.pAABB->fYMin;
		ret = 0;
	}
	else if (psl->SlidePos.y > psl->Constraint.AABB.pAABB->fYMax)
	{
		psl->SlidePos.y = psl->Constraint.AABB.pAABB->fYMax;
		ret = 0;
	}

	if (psl->SlidePos.z < psl->Constraint.AABB.pAABB->fZMin)
	{
		psl->SlidePos.z = psl->Constraint.AABB.pAABB->fZMin;
		ret = 0;
	}
	else if (psl->SlidePos.z > psl->Constraint.AABB.pAABB->fZMax)
	{
		psl->SlidePos.z = psl->Constraint.AABB.pAABB->fZMax;
		ret = 0;
	}
	// ++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + 2) Repulse Force ?
	// +
	if (ISFLAG_ON(psl->UI.Flags, FLAG_NUI_SLIDE_LIMITS_REPULSE))
	{
		NVec3Null(&psl->SafeRepulseForce);
		FLAG_OFF(psl->UI.Flags, FLAG_NUI_SLIDE_REPULSE);

		NVEC3 v0, v1;
		NVec3Sub(&v0, &psl->SlidePos, &psl->Constraint.AABB.pAABB->VMin);
		NVec3Sub(&v1, &psl->Constraint.AABB.pAABB->VMax, &psl->SlidePos);
		if (v0.x < psl->Constraint.AABB.SafeAABBDistance.x)
		{
			psl->SafeRepulseForce.x = (psl->Constraint.AABB.SafeAABBDistance.x - v0.x) * psl->SafeSpringK;
			FLAG_ON(psl->UI.Flags, FLAG_NUI_SLIDE_REPULSE);
			printf("\nREPULSE XMin");
		}
		else if (v1.x < psl->Constraint.AABB.SafeAABBDistance.x)
		{
			psl->SafeRepulseForce.x = -(psl->Constraint.AABB.SafeAABBDistance.x - v1.x) * psl->SafeSpringK;
			FLAG_ON(psl->UI.Flags, FLAG_NUI_SLIDE_REPULSE);
			printf("\nREPULSE XMax");
		}

		if (v0.y < psl->Constraint.AABB.SafeAABBDistance.y)
		{
			psl->SafeRepulseForce.y = (psl->Constraint.AABB.SafeAABBDistance.y - v0.y) * psl->SafeSpringK;
			FLAG_ON(psl->UI.Flags, FLAG_NUI_SLIDE_REPULSE);
			printf("\nREPULSE YMin");
		}
		else if (v1.y < psl->Constraint.AABB.SafeAABBDistance.y)
		{
			psl->SafeRepulseForce.y = -(psl->Constraint.AABB.SafeAABBDistance.y - v1.y) * psl->SafeSpringK;
			FLAG_ON(psl->UI.Flags, FLAG_NUI_SLIDE_REPULSE);
			printf("\nREPULSE YMax");
		}

		if (v0.z < psl->Constraint.AABB.SafeAABBDistance.z)
		{
			psl->SafeRepulseForce.z = (psl->Constraint.AABB.SafeAABBDistance.z - v0.z) * psl->SafeSpringK;
			FLAG_ON(psl->UI.Flags, FLAG_NUI_SLIDE_REPULSE);
			printf("\nREPULSE Zmin");
		}
		else if (v1.z < psl->Constraint.AABB.SafeAABBDistance.z)
		{
			psl->SafeRepulseForce.z = -(psl->Constraint.AABB.SafeAABBDistance.z - v1.z) * psl->SafeSpringK;
			FLAG_ON(psl->UI.Flags, FLAG_NUI_SLIDE_REPULSE);
			printf("\nREPULSE ZMax");
		}
	}

	// "return 0" if position was corrected because out of the AABB limits.
	// "return 1" if position was inside the AABB.
	return ret;
}

Nu32 NApplyUISlideConstraints_Spline(NUI_SLIDE *psl)
{
	NErrorIf(psl->Constraint.Spline.TCurrentXtdFactor < 0.0f || psl->Constraint.Spline.TCurrentXtdFactor > (Nf32)(psl->Constraint.Spline.pSpline->KnotArray.Size - 1), NERROR_VALUE_OUTOFRANGE);

	// ++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + 1) Position vs Constraints
	// +
	Nu32 return_value;
	// Forget potential previous On Spline Warp Direction (and step by the same way) and let the NUT_WrapOnSpline recalculates a new one
	// to always wrap into the best direction regarding dragged position.
	if (psl->UI.CurrentState == NUI_STATE_SLIDE_DRAG || psl->pCurrentSnapPoint || ISFLAG_ON(psl->UI.Flags, FLAG_NUI_SLIDE_REPULSE))
		psl->Constraint.Spline.TStep = 0.0f;
	if (NUT_WrapOnSpline(&psl->SlidePos, &psl->Constraint.Spline.TCurrentXtdFactor, &psl->Constraint.Spline.TStep, psl->Constraint.Spline.pSpline) == 0)
		return_value = 0; // "return" 0 which means position was corrected because out of limits.
	else
		return_value = 1; // "return 1" which means position stays under limits.
						  // Notice that even if it seems strange because position was corrected anyway, in that case
						  // we are not going to consider position was beyond limits. For a spline limits are before the start or after the end

	// ++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + 2) Repulse Force ?
	// +
	// + For a Spline, SafeDistance is understood as a ratio distance, and only the X component of SafeDistance matters
	// + the 2 other components Y and Z  are ignored...
	if (ISFLAG_ON(psl->UI.Flags, FLAG_NUI_SLIDE_LIMITS_REPULSE))
	{
		NVec3Null(&psl->SafeRepulseForce);
		FLAG_OFF(psl->UI.Flags, FLAG_NUI_SLIDE_REPULSE);

		//		Nf32		tmax = (Nf32)(psl->Constraint.Spline.pSpline->KnotArray.Size-1);
		NVEC3 v;

		if (psl->Constraint.Spline.TCurrentXtdFactor < psl->Constraint.Spline.SafeSplineRatioStart)
		{
			NSplinePointD3f32(&v, psl->Constraint.Spline.SafeSplineRatioStart, psl->Constraint.Spline.pSpline);
			NVec3SubFrom(&v, &psl->SlidePos);
			NVec3Scale(&psl->SafeRepulseForce, &v, psl->SafeSpringK);
			FLAG_ON(psl->UI.Flags, FLAG_NUI_SLIDE_REPULSE);
		}
		else if (psl->Constraint.Spline.TCurrentXtdFactor > psl->Constraint.Spline.SafeSplineRatioEnd)
		{
			NSplinePointD3f32(&v, psl->Constraint.Spline.SafeSplineRatioEnd, psl->Constraint.Spline.pSpline);
			NVec3SubFrom(&v, &psl->SlidePos);
			NVec3Scale(&psl->SafeRepulseForce, &v, psl->SafeSpringK);
			FLAG_ON(psl->UI.Flags, FLAG_NUI_SLIDE_REPULSE);
		}
	}
	return return_value;
}

Nu32 NApplyUISlideConstraints_Segment(NUI_SLIDE *psl)
{
	Nu32 return_value;

	NUT_CLOSESTPOINT_ON_LINE_RESULT result;
	NVEC3 v01;
	NVec3Sub(&v01, &psl->Constraint.Segment.SegmentPoint1, &psl->Constraint.Segment.SegmentPoint0);
	NUT_GetClosestPointOnSegment(&result, &psl->Constraint.Segment.SegmentPoint0, &v01, &psl->SlidePos);
	psl->SlidePos = result.PointOnLine;

	if (result.ParamCoord > 0.0f && result.ParamCoord < 1.0f)
	{
		return_value = 1;
		// "return 1" which means position stays under limits.
		// Notice that even if it seems strange because position was corrected anyway, in that case
		// we are not going to consider position was beyond limits.
		// For a segment (as for a spline) limits are before the start or after the end.
	}
	else
	{
		// "return 0" which means position was corrected because out of limits.
		return_value = 0;
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + 2) Repulse Force ?
	// +
	// + For a Spline, SafeDistance is understood as a ratio distance, and only the X component of SafeDistance matters
	// + the 2 other components Y and Z  are ignored...
	if (ISFLAG_ON(psl->UI.Flags, FLAG_NUI_SLIDE_LIMITS_REPULSE))
	{
		NVec3Null(&psl->SafeRepulseForce);
		FLAG_OFF(psl->UI.Flags, FLAG_NUI_SLIDE_REPULSE);
		NVEC3 vs;

		if (result.ParamCoord < psl->Constraint.Segment.SafeSegmentRatio)
		{
			vs.x = psl->Constraint.Segment.SegmentPoint0.x + v01.x * psl->Constraint.Segment.SafeSegmentRatio;
			vs.y = psl->Constraint.Segment.SegmentPoint0.y + v01.y * psl->Constraint.Segment.SafeSegmentRatio;
			vs.z = psl->Constraint.Segment.SegmentPoint0.z + v01.z * psl->Constraint.Segment.SafeSegmentRatio;

			NVec3Sub(&psl->SafeRepulseForce, &vs, &psl->SlidePos);
			NVec3ScaleBy(&psl->SafeRepulseForce, psl->SafeSpringK);
			FLAG_ON(psl->UI.Flags, FLAG_NUI_SLIDE_REPULSE);
		}
		else if (result.ParamCoord > 1.0f - psl->Constraint.Segment.SafeSegmentRatio)
		{
			vs.x = psl->Constraint.Segment.SegmentPoint0.x + v01.x * (1.0f - psl->Constraint.Segment.SafeSegmentRatio);
			vs.y = psl->Constraint.Segment.SegmentPoint0.y + v01.y * (1.0f - psl->Constraint.Segment.SafeSegmentRatio);
			vs.z = psl->Constraint.Segment.SegmentPoint0.z + v01.z * (1.0f - psl->Constraint.Segment.SafeSegmentRatio);
			NVec3Sub(&psl->SafeRepulseForce, &vs, &psl->SlidePos);
			NVec3ScaleBy(&psl->SafeRepulseForce, psl->SafeSpringK);
			FLAG_ON(psl->UI.Flags, FLAG_NUI_SLIDE_REPULSE);
		}
	}
	return return_value;
}

NVEC3 *NConformPositionToConstraintContext(NVEC3 *ppos, const NUI_SLIDE *pslayer)
{
	if (pslayer->Constraint.ApplyConstraint_Handle == NApplyUISlideConstraints_Basic)
	{
		return ppos; // nothing special to do
	}
	else if (pslayer->Constraint.ApplyConstraint_Handle == NApplyUISlideConstraints_AABB)
	{
		ppos->x = NCLAMP(pslayer->Constraint.AABB.pAABB->fXMin, ppos->x, pslayer->Constraint.AABB.pAABB->fXMax);
		ppos->y = NCLAMP(pslayer->Constraint.AABB.pAABB->fYMin, ppos->y, pslayer->Constraint.AABB.pAABB->fYMax);
		ppos->z = NCLAMP(pslayer->Constraint.AABB.pAABB->fZMin, ppos->z, pslayer->Constraint.AABB.pAABB->fZMax);
	}
	else if (pslayer->Constraint.ApplyConstraint_Handle == NApplyUISlideConstraints_Segment)
	{
		// Find the closest point on Segment of the current sliding position
		NUT_CLOSESTPOINT_ON_LINE_RESULT line_result;
		NVEC3 v;
		NVec3Sub(&v, &pslayer->Constraint.Segment.SegmentPoint1, &pslayer->Constraint.Segment.SegmentPoint0);
		NUT_GetClosestPointOnSegment(&line_result, &pslayer->Constraint.Segment.SegmentPoint0, &v, ppos);
		// And apply constraint for the first time
		*ppos = line_result.PointOnLine;
	}
	else if (pslayer->Constraint.ApplyConstraint_Handle == NApplyUISlideConstraints_Spline)
	{
		NUT_CLOSESTPOINT_ON_SPLINE_RESULT result;
		NUT_GetClosestPointOnSpline(&result, ppos, pslayer->Constraint.Spline.pSpline);
		*ppos = result.VResult;
	}
	else
	{
		// Actually there are only 3 possible constraint contexts: Basic,AABB and Spline
		NErrorIf(1, NERROR_UI_UNKNOWN_SLIDINGLAYER_CONSTRAINT_CONTEXT);
	}

	return ppos;
}
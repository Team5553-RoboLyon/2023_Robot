#include "lib/N/NCStandard.h"
#include "../NCoordinateSystemConversions.h"
#include "../Utilities/NUT_X.h"
#include "NUI.h"
#include "NUISlide.h"

void NInsertUISlideSnapPoint(NUI_SLIDE *pslayer, const NVEC3 *pposition, const Nbool conform_to_constraint_context)
{
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + STEP 1/ Setup and Conform Snap Point position to potential Constraint AABB or Spline if user asks for.
	// +
	NUI_SLIDE_SNAPPOINT snappoint;
	Nmem0(&snappoint, NUI_SLIDE_SNAPPOINT);
	snappoint.Position = *pposition;
	// snappoint.Xtd_Factor	= 0.0f;
	if (conform_to_constraint_context)
	{
		NConformPositionToConstraintContext(&snappoint.Position, pslayer);
	}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + STEP 2/ Debug only. Advanced Check !
// + We are going to check if an already inserted snap point has almost the same position.
// + An Error occurs if we found one.
// +
#ifdef _DEBUG
	NVEC3 dbg_v;
	NUI_SLIDE_SNAPPOINT *dbg_psp = (NUI_SLIDE_SNAPPOINT *)pslayer->SnapPointArray.pFirst;
	for (Nu32 dbg_i = pslayer->SnapPointArray.Size; dbg_i != 0; dbg_i--, dbg_psp++)
	{
		NVec3Sub(&dbg_v, pposition, &dbg_psp->Position);
		if (NVec3Length(&dbg_v) <= CONSTANT_NUI_SLIDE_EPSILON_SNAPPOINT_DISTANCE)
			NErrorIf(1, NERROR_INCONSISTENT_PARAMETERS);
	}
#endif

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + STEP 3/ Insert !!!
	// +
	/*NUI_SLIDE_SNAPPOINT *psp = */ (NUI_SLIDE_SNAPPOINT *)NArrayPushBack(&pslayer->SnapPointArray, (NBYTE *)&snappoint);
}

void NUpdateCurrentUISlideSnapPoint_CaptureClosest(NUI_SLIDE *pslayer)
{
	Nu32 i;
	NUI_SLIDE_SNAPPOINT *psp, *psp0;
	NVEC3 v;
	NVEC3 v0;
	Nf32 f, fmin;

	v0 = pslayer->SlidePos;
	fmin = NF32_MAX;
	psp0 = NULL;
	psp = (NUI_SLIDE_SNAPPOINT *)pslayer->SnapPointArray.pFirst;
	for (i = pslayer->SnapPointArray.Size; i != 0; i--, psp++)
	{
		NVec3Sub(&v, &v0, &psp->Position);
		f = NVec3SquareLength(&v);
		if (f < fmin)
		{
			fmin = f;
			psp0 = psp;
		}
	}

	pslayer->pCurrentSnapPoint = psp0;
}

/*
void NApplyCurrentUISlideSnapPointConstraint(NUI_SLIDE *pslayer)
{
	if(pslayer->pCurrentSnapPoint)
	{
		NVec3Lerp(	&pslayer->SlidePos,
			&pslayer->pCurrentSnapPoint->Position,
			&pslayer->SlidePos,
			pslayer->AttractDamping	);

		if(ISFLAG_ON(pslayer->UI.Flags,FLAG_NUI_SLIDE_SNAP_THRESHOLD))
		{
			NVEC3 v;
			NVec3Sub(&v,&pslayer->SlidePos,&pslayer->pCurrentSnapPoint->Position);
			if(NVec3SquareLength(&v)<=pslayer->CaptureSquareRadius)
			{
				pslayer->SlidePos = pslayer->pCurrentSnapPoint->Position;
			}
		}
	}
}
*/
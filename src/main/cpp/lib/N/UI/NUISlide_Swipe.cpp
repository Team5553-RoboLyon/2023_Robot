#include "lib/N/NCStandard.h"
#include "../NCoordinateSystemConversions.h"
#include "../Utilities/NUT_X.h"
#include "NUI.h"
#include "NUISlide.h"

void NSetUISlide_Swipe(NUI_SLIDE *pslayer, const Nu32 flags_swipe)
{
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + SWIPE METHOD
	// +
	if (ISFLAG_ON(flags_swipe, FLAG_NUIDS_SLIDE_SWIPE_ONFIELD_SEEKING))
	{
		pslayer->Swipe_Handle = NSwipeUISlide_OnFieldSeeking;
	}
	else if (ISFLAG_ON(flags_swipe, FLAG_NUIDS_SLIDE_SWIPE_BIDIRECTIONNAL))
	{
		pslayer->Swipe_Handle = NSwipeUISlide_Bidirectionnal;
	}
	else if (ISFLAG_ON(flags_swipe, FLAG_NUIDS_SLIDE_SWIPE))
	{
		pslayer->Swipe_Handle = NSwipeUISlide_Basic;
	}
	else // useless but to stay crystal clear
	{
		pslayer->Swipe_Handle = NULL;
	}
}

void NSwipeUISlide_Basic(NUI_SLIDE *pslayer)
{
	if (pslayer->pCurrentSnapPoint)
	{
		Nf32 i = NVec2DotProduct(&pslayer->NormTouchMove, &pslayer->SwipeI);
		printf("\n SWIP DISTANCE = %.4f", i);
		if (NABS(i) >= pslayer->SwipeThreshold)
		{
			pslayer->pCurrentSnapPoint = NCLAMP((NUI_SLIDE_SNAPPOINT *)NGetFirstArrayPtr(&pslayer->SnapPointArray),
												pslayer->pCurrentSnapPoint - NSIGN(i) * pslayer->SwipeJump,
												(NUI_SLIDE_SNAPPOINT *)NGetLastArrayPtr(&pslayer->SnapPointArray));

			NErrorIf(!NIsValidArrayElementPtr(&pslayer->SnapPointArray, (NBYTE *)pslayer->pCurrentSnapPoint), NERROR_ADDRESS_OUTOFRANGE);
		}
		else // No Swipe detected ... Default behavior: just pick the closest Snap Point
		{
			NUpdateCurrentUISlideSnapPoint_CaptureClosest(pslayer);
		}
	}
	else // Default behavior: just pick the closest Snap Point
	{
		NUpdateCurrentUISlideSnapPoint_CaptureClosest(pslayer);
	}
}

void NSwipeUISlide_Bidirectionnal(NUI_SLIDE *pslayer)
{
	if (pslayer->pCurrentSnapPoint)
	{

		NVEC2 swipej;
		NVec2Set(&swipej, -pslayer->SwipeI.y, pslayer->SwipeI.x); // swipej and swipei are 2D orthogonal vectors.
		Nf32 i = NVec2DotProduct(&pslayer->NormTouchMove, &pslayer->SwipeI);
		Nf32 j = NVec2DotProduct(&pslayer->NormTouchMove, &swipej);
		Nf32 used_dot;
		Nu16 jump;

		if (NABS(i) >= NABS(j))
		{
			used_dot = i;
			jump = pslayer->SwipeJump;
		}
		else
		{
			used_dot = j;
			jump = pslayer->SwipeAsideJump;
		}

		printf("\n SWIP DISTANCE = %.4f", used_dot);

		if (NABS(used_dot) >= pslayer->SwipeThreshold)
		{
			pslayer->pCurrentSnapPoint = NCLAMP((NUI_SLIDE_SNAPPOINT *)NGetFirstArrayPtr(&pslayer->SnapPointArray),
												pslayer->pCurrentSnapPoint - NSIGN(used_dot) * jump,
												(NUI_SLIDE_SNAPPOINT *)NGetLastArrayPtr(&pslayer->SnapPointArray));
		}
		else // No Swipe detected ... Default behavior: just pick the closest Snap Point
		{
			NUpdateCurrentUISlideSnapPoint_CaptureClosest(pslayer);
		}
	}
	else // Default behavior: just pick the closest Snap Point
	{
		NUpdateCurrentUISlideSnapPoint_CaptureClosest(pslayer);
	}
}

void NSwipeUISlide_OnFieldSeeking(NUI_SLIDE *pslayer)
{
	if (pslayer->pCurrentSnapPoint)
	{
		Nf32 l = NVec2Length(&pslayer->NormTouchMove);

		printf("\n SWIP DISTANCE = %.4f", l);
		if (l >= pslayer->SwipeThreshold)
		{
			NUI_SLIDE_SNAPPOINT *pchoose = NULL;
			Nf32 mini = NF32_MAX;

			Nf32 dot;
			NVEC3 v;
			NVEC3 swipei;
			NVEC3 swipej;
			NVec3Set(&swipei, pslayer->SwipeI.x, pslayer->SwipeI.y, 0);
			NVec3Set(&swipej, -pslayer->SwipeI.y, pslayer->SwipeI.x, 0); // swipej and swipei are 2D orthogonal vectors.

			// Look for a new Snap point from the current one in the NormTouchMove Direction.
			// It's not a problem if NormTouchMove vector is not unitary, we just want to compare values
			// between them...
			NUI_SLIDE_SNAPPOINT *psnap = (NUI_SLIDE_SNAPPOINT *)pslayer->SnapPointArray.pFirst;
			for (Nu32 i = pslayer->SnapPointArray.Size; i != 0; i--, psnap++)
			{
				if (pslayer->pCurrentSnapPoint != psnap)
				{
					NVec3Sub(&v, &psnap->Position, &pslayer->pCurrentSnapPoint->Position);
					dot = NVec3DotProduct(&v, &swipei);
					if (dot > 0.0f && dot < mini)
					{
						if (NABS(NVec3DotProduct(&v, &swipej)) <= pslayer->SwipeOnFieldSeekingAccuracy)
						{
							mini = dot;
							pchoose = psnap;
						}
					}
				}
			}
			if (pchoose)
				pslayer->pCurrentSnapPoint = pchoose;
			else // No effective Swipe ... Default behavior: just pick the closest Snap Point
				NUpdateCurrentUISlideSnapPoint_CaptureClosest(pslayer);
		}
	}
	else // Default behavior: just pick the closest Snap Point
	{
		NUpdateCurrentUISlideSnapPoint_CaptureClosest(pslayer);
	}
}

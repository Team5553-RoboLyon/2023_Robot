#include "lib/N/NCStandard.h"
#include "lib/N/NCoordinateSystemConversions.h"
#include "lib/N/Utilities/NUT_X.h"
#include "lib/N/Event/NEventTouch.h"
#include "NUI.h"
#include "NUIClearWatchStack.h"
#include "NUISlide.h"
#include "lib/N/Miscellaneous/NDamping.h"

void NUISlide_EventToState_Handle(NUI *pui, const NEVENT *pevent)
{
	Nf32 dist;
	NUI_SLIDE *psl;
	NVEC2s16 mov;

	switch (pevent->Type)
	{
	case NTOUCH_START:
		if (NIsUIFirstTouchStarted(pui))
		{
			psl = (NUI_SLIDE *)pui;
			switch (psl->UI.CurrentState)
			{
			case NUI_STATE_SLIDE_REST:
			case NUI_STATE_SLIDE_FREE:
			case NUI_STATE_SLIDE_FREE_STOP:
				psl->NormTouchMoveAccumulator.x = 0.0f;
				psl->NormTouchMoveAccumulator.y = 0.0f;
				psl->NormTouchMove.x = 0.0f;
				psl->NormTouchMove.y = 0.0f;
				psl->DragPos = psl->SlidePos;

				FLAG_ON(psl->UI.Flags, FLAG_NUI_SLIDE_DRAG_LOCK);
				psl->UI.CurrentState = NUI_STATE_SLIDE_DRAG;
				break;

				// case NUI_STATE_SLIDE_DRAG or Unknown STATE, both are NOT supported at Touch Start !!!
			default:
				NErrorIf(1, NERROR_UNAUTHORIZED_CASE);
				break;
			}
		}
		break;

	case NTOUCH_MOVE:
		psl = (NUI_SLIDE *)pui;
		switch (pui->CurrentState)
		{
		case NUI_STATE_SLIDE_DRAG:
			// Use all the Moved touch(s)... Maybe it could be the fastest ???
			psl->NormTouchMove.x = 0.0f;
			psl->NormTouchMove.y = 0.0f;
			NGetEventFastestTouchMove(&mov, pevent);
			psl->NormTouchMove.x += NCoord_PixToHRel_X(mov.x);
			psl->NormTouchMove.y += NCoord_PixToHRel_Y(mov.y); //-NCoord_PixToHRel_Y(mov.y);
			psl->DragPos.x += psl->NormTouchMove.x;
			psl->DragPos.y += psl->NormTouchMove.y;
			psl->DragPos.z = 0.0f;

			// It's clever to move UI here instead of moving it within the UIstate handle.
			// ... because UI will still under NUI_STATE_SLIDE_DRAG state until TOUCH_END...
			// and user may keep touching screen without moving. So doing NMoveUIframe here guarantee to do this
			// only when a real move occurs... and not always during the state life.

			// Kill Focus ? .............................
			psl->NormTouchMoveAccumulator.x += psl->NormTouchMove.x;
			psl->NormTouchMoveAccumulator.y += psl->NormTouchMove.y;
			dist = NFastSqrt(psl->NormTouchMoveAccumulator.x * psl->NormTouchMoveAccumulator.x + psl->NormTouchMoveAccumulator.y * psl->NormTouchMoveAccumulator.y);
			printf("\n SLIDE DIST ---------------> %.3f", dist);
			// Child Focus removing ?
			if (ISFLAG_ON(psl->UI.Flags, FLAG_NUI_SLIDE_KILL_CHILDREN_TOUCH_LISTENING) && (dist > CONSTANT_NUI_SLIDE_KILL_TOUCH_LISTENING_SLIDING_DISTANCE))
			{
				NUITouchCancelAll(pui, NFALSE);
			}
			break;

		default:
			break;
		}
		break;

	case NTOUCH_END:
		if (NIsUILastTouchEnded(pui))
		{
			psl = (NUI_SLIDE *)pui;
			switch (psl->UI.CurrentState)
			{
			case NUI_STATE_SLIDE_DRAG:
				if (psl->Swipe_Handle)
					psl->Swipe_Handle(psl);
				else // Default behavior: just pick the closest Snap Point
					NUpdateCurrentUISlideSnapPoint_CaptureClosest(psl);
				if (ISFLAG_ON(psl->UI.Flags, FLAG_NUI_SLIDE_TOUCH_END_SPEED_MULTIPLIER))
					NVec3Lerp(&psl->SlidePos, &psl->OldSlidePos, &psl->SlidePos, psl->TouchEndSpeedMultiplier);
				psl->UI.CurrentState = NUI_STATE_SLIDE_FREE;
				psl->NormTouchMove.x = psl->NormTouchMove.y = 0.0f;
				psl->NormTouchMoveAccumulator.x = psl->NormTouchMoveAccumulator.y = 0.0f;
				FLAG_OFF(psl->UI.Flags, FLAG_NUI_SLIDE_DRAG_LOCK);
				break;

			default:
				break;
			}
		}
		break;

	case NTOUCH_CANCEL:
		if (NIsUILastTouchEnded(pui))
		{
			psl = (NUI_SLIDE *)pui;
			switch (psl->UI.CurrentState)
			{
			case NUI_STATE_SLIDE_DRAG:
				// NTOUCH_CANCEL response is slightly different than NTOUCH_END response.
				// Because UISLide understands NTOUCH_CANCEL in a different way than a NTOUCH_END ...
				// ... for UIslide, NTOUCH_CANCEL doesn't generate a SWIPE.
				NUpdateCurrentUISlideSnapPoint_CaptureClosest(psl);
				psl->UI.CurrentState = NUI_STATE_SLIDE_FREE;
				psl->NormTouchMove.x = psl->NormTouchMove.y = 0.0f;
				FLAG_OFF(psl->UI.Flags, FLAG_NUI_SLIDE_DRAG_LOCK);
				break;

			default:
				break;
			}
		}
		break;

	case NUI_SHOW:
		// In case of UICORE interception Activation by User !!!
		if ((NUI *)pevent->Head.pRecipient == pui)
		{
			psl = (NUI_SLIDE *)pui;
			psl->NormTouchMoveAccumulator.x = 0;
			psl->NormTouchMoveAccumulator.y = 0;
			NVec3Set(&psl->SafeRepulseForce, 0, 0, 0);

			if (psl->pCurrentSnapPoint)
			{
				psl->SlidePos = psl->OldSlidePos = psl->pCurrentSnapPoint->Position;
			}
			else
			{
				psl->OldSlidePos = psl->SlidePos;
			}
		}
		break;

	default:
		break;
	}
}

static inline Nu32 _applyverletforces(NUI_SLIDE *pslide, const Nf32 fdelay)
{
	NVEC3 velocity;
	NVEC3 v;

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + 1-PRIOR SETTINGS
	// +
	NVec3Sub(&velocity, &pslide->SlidePos, &pslide->OldSlidePos);

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + 2-ALL FORCES ACCUMULATION
	// +
	// + 2a) Frictions due to Speed itself
	NVec3Scale(&pslide->ForceAccumulator, &velocity, -pslide->VerletFriction);
	// + 2b) Current Snap Point Attraction
	if (pslide->UI.CurrentState != NUI_STATE_SLIDE_DRAG && pslide->pCurrentSnapPoint)
	{
		NVec3Sub(&v, &pslide->pCurrentSnapPoint->Position, &pslide->SlidePos);
		NVec3ScaleBy(&v, pslide->SnapSpringK);
		NVec3AddTo(&pslide->ForceAccumulator, &v);
	}
	// + 2c) DragPoint
	if (ISFLAG_OFF(pslide->UI.Flags, FLAG_NUI_SLIDE_DRAG_LOCK) && pslide->UI.CurrentState == NUI_STATE_SLIDE_DRAG)
	{
		NVec3Sub(&v, &pslide->DragPos, &pslide->SlidePos);
		NVec3ScaleBy(&v, pslide->DragSpringK);
		NVec3AddTo(&pslide->ForceAccumulator, &v);
	}

	// + 2d) Safety Marge Limit constraint
	if (ISFLAG_ON(pslide->UI.Flags, FLAG_NUI_SLIDE_REPULSE))
	{
		NVec3AddTo(&pslide->ForceAccumulator, &pslide->SafeRepulseForce);

		// Safe System additionnal Friction
		if (pslide->UI.CurrentState == NUI_STATE_SLIDE_FREE)
		{
			NVec3Scale(&v, &velocity, -pslide->SafeSpringFriction);
			NVec3AddTo(&pslide->ForceAccumulator, &v);
		}
	}
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + 3-INTEGRATION
	// +
	// + 3a)
	pslide->OldSlidePos = pslide->SlidePos;

	NVec3Scale(&v, &pslide->ForceAccumulator, fdelay * fdelay);
	pslide->SlidePos.x += velocity.x * pslide->VerletDamping + v.x;
	pslide->SlidePos.y += velocity.y * pslide->VerletDamping + v.y;
	pslide->SlidePos.z += velocity.z * pslide->VerletDamping + v.z;

	// + 3b) Empty Forces Accumulator
	NVec3Null(&pslide->ForceAccumulator);

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + 4- APPLY CONSTRAINTS
	// +
	// + 4a) Drag Point
	if (ISFLAG_ON(pslide->UI.Flags, FLAG_NUI_SLIDE_DRAG_LOCK))
	{
		pslide->SlidePos = pslide->DragPos;
	}
	// + 4b) Current SnapPoint Constraint (Tested Only if Drag Point is deactivated because drag point will always win !)
	else if (pslide->pCurrentSnapPoint && ISFLAG_ON(pslide->UI.Flags, FLAG_NUI_SLIDE_SNAP_THRESHOLD))
	{
		NVec3Sub(&v, &pslide->SlidePos, &pslide->pCurrentSnapPoint->Position);
		if (NVec3SquareLength(&v) <= pslide->SquareSnapCaptureRadius)
		{
			pslide->SlidePos = pslide->pCurrentSnapPoint->Position;
		}
	}
	// + 4c) Pins
	Nf32 velocity_squarenorm = NVec3SquareLength(&velocity);
	NUI_SLIDE_PIN *ppin = (NUI_SLIDE_PIN *)pslide->PinArray.pFirst;

	for (Nu32 i = pslide->PinArray.Size; i != 0; i--, ppin++)
	{
		// if( velocity_squarenorm <= ppin->SquareSpeedThreshold )
		//  Why "velocity_squarenorm <= ppin->SquareCaptureRadius*4.0f" ?
		//  Simple actually, we want velocity norm less than the capture circle diameter.
		//  If not, we consider speed is to high to let the layer be capture by the pin.
		//  So 2 times means 4 times because of 'square'...
		if (velocity_squarenorm <= ppin->SquareCaptureRadius * 4.0f)
		{
			NVec3Sub(&v, &pslide->SlidePos, &ppin->Position);
			if (NVec3SquareLength(&v) <= ppin->SquareCaptureRadius)
			{
				if (ISFLAG_ON(ppin->Flags, FLAG_NUI_SLIDE_PIN_USE_SPEED_THRESHOLD_DIRECTION))
				{
					if (NVec3DotProduct(&velocity, &ppin->SpeedThresholdDirection) > 0.0f)
					{
						pslide->SlidePos = pslide->OldSlidePos = ppin->Position;
						break;
					}
				}
				else
				{
					pslide->SlidePos = pslide->OldSlidePos = ppin->Position;
					break;
				}
			}
		}
	}
	// + 4d) Main constraint (AABB, Segment or Spline)
	//		 ( ... and return status against main constraint to maybe do something else like changing state ...)
	return pslide->Constraint.ApplyConstraint_Handle(pslide);
}

static void NUISlide_States_Handle(NUI *pui, const NTIME *ptime)
{
	NUI_SLIDE *pslayer;
	//	NVEC3	velocity,acc;

	switch (pui->CurrentState)
	{
	case NUI_STATE_SLIDE_REST:
		//	printf("\n NUI_STATE_SLIDE_REST");
		break;

	case NUI_STATE_SLIDE_DRAG:
		//	printf("\n NUI_STATE_SLIDE_DRAG");
		pslayer = (NUI_SLIDE *)pui;
		// Trick:	'VERLET Speed' is computed on the fly by subtracting old position to the current one.
		//			So, to reduce releasing speed on the fly when user doesn't move but keep a finger on screen
		//			we are going to cheat the old position by making it closer and closer to the current one.
		if (!_applyverletforces(pslayer, ptime->Nf32_Delay) && ISFLAG_ON(pui->Flags, FLAG_NUI_SLIDE_LIMITS_KILL_STATE_DRAG))
		{
			NUpdateCurrentUISlideSnapPoint_CaptureClosest(pslayer);
			FLAG_OFF(pui->Flags, FLAG_NUI_SLIDE_DRAG_LOCK);
			pslayer->UI.CurrentState = NUI_STATE_SLIDE_FREE;
		}

		if (ISFLAG_ON(pui->Flags, FLAG_NUI_SLIDE_REPULSE))
		{
			FLAG_OFF(pui->Flags, FLAG_NUI_SLIDE_DRAG_LOCK);
		}
		else if (ISFLAG_OFF(pui->Flags, FLAG_NUI_SLIDE_DRAG_LOCK))
		{
			FLAG_ON(pui->Flags, FLAG_NUI_SLIDE_DRAG_LOCK);
			pslayer->DragPos = pslayer->SlidePos;
		}

		// ... Here is the trick to reduce releasing speed ...
		NVec3Lerp(&pslayer->OldSlidePos, &pslayer->SlidePos, &pslayer->OldSlidePos, pslayer->DragDamping);
		NSetUIPosition(pui, &pslayer->SlidePos);
		break;

	case NUI_STATE_SLIDE_FREE:
		// printf("\n NUI_STATE_SLIDE_FREE");
		pslayer = (NUI_SLIDE *)pui;
		/*
				// VERLET Integration
				NVec3Sub(&velocity,&psl->SlidePos,&psl->OldSlidePos);
				NVec3Scale(&acc,&velocity,-psl->VerletFriction*ptime->Nf32_Delay*ptime->Nf32_Delay);
				psl->OldSlidePos = psl->SlidePos;
				psl->SlidePos.x += velocity.x*psl->VerletDamping + acc.x;
				psl->SlidePos.y += velocity.y*psl->VerletDamping + acc.y;
				psl->SlidePos.z += velocity.z*psl->VerletDamping + acc.z;

				NApplyCurrentUISlideSnapPointConstraint(psl);
				psl->Constraint.ApplyConstraint_Handle(psl);
		*/
		_applyverletforces(pslayer, ptime->Nf32_Delay);
		NSetUIPosition(pui, &pslayer->SlidePos);

		// Check for stopping, only if there is no active repulsive force.
		if (ISFLAG_OFF(pui->Flags, FLAG_NUI_SLIDE_REPULSE))
		{
			NVEC3 v;
			if (pslayer->pCurrentSnapPoint)
			{
				NVec3Sub(&v, &pslayer->SlidePos, &pslayer->pCurrentSnapPoint->Position);
			}
			else
			{
				NVec3Sub(&v, &pslayer->SlidePos, &pslayer->OldSlidePos);
			}
			if (NVec3FastLength(&v) < NF32_EPSILON_VECTOR_LENGTH)
			{
				pui->CurrentState = NUI_STATE_SLIDE_FREE_STOP;
			}
		}
		break;

	case NUI_STATE_SLIDE_FREE_STOP:
		// printf("\n NUI_STATE_SLIDE_FREE_STOP");
		pslayer = (NUI_SLIDE *)pui;
		pslayer->OldSlidePos = pslayer->SlidePos;
		pui->CurrentState = NUI_STATE_SLIDE_REST;
		break;

	default:
		break;
	}

	/*
		// DEBUG
		psl = (NUI_SLIDE *)pui;
		NUT_DRAW	draw;
		NVEC3	shift;
		Nmem0(&draw,NUT_DRAW);
		NVec3Set(&shift,NCoord_NormToHRel_X( 1.0f ),NCoord_NormToHRel_Y( -0.5f ),0);
		NCopySpline(&psl->dbg_spline,psl->Constraint.Spline.pSpline);
		NShiftSpline(&psl->dbg_spline,&shift);
		draw.Spline.pDrawCallBack = NUT_GetDrawCallBack(NUT_DRAW_ID_SPLINE);
		NSetColorf(&draw.Spline.Color,NCOLOR_PRESET3F_BLUE_ELECTRIC);
		draw.Spline.pspline = &psl->dbg_spline;
		FLAG_ON(draw.Spline.Flags,FLAG_NUT_DRAW_SPLINE_HIDETANGENTES|FLAG_NUT_DRAW_SPLINE_HIDESPLINEKNOTS);
		NUT_Draw(&draw,NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
		Nmem0(&draw,NUT_DRAW);
		NVec3Add(&draw.Quad.Center,&psl->SlidePos,&shift);
		if(psl->Constraint.Spline.TStep == 0.0f)
			NSetColorf(&draw.Quad.Color,NCOLOR_PRESET3F_WHITE);
		else if(psl->Constraint.Spline.TStep > 0.0f)
			NSetColorf(&draw.Quad.Color,NCOLOR_PRESET3F_GREEN);
		else
			NSetColorf(&draw.Quad.Color,NCOLOR_PRESET3F_ORANGE);
		NVec3Set(&draw.Quad.ConstructionPlane_DirectorVector,0,0,1);
		NVec2Set(&draw.Quad.Extents,0.025f,0.025f);
		draw.Quad.pDrawCallBack = NUT_GetDrawCallBack(NUT_DRAW_ID_QUAD);
		NUT_Draw(&draw,NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);

		NUI_SLIDE_SNAPPOINT *psnappoint = (NUI_SLIDE_SNAPPOINT *)psl->SnapPointArray.pFirst;
		for(Nu32 nb=0;nb<psl->SnapPointArray.Size;nb++,psnappoint++)
		{
			Nmem0(&draw,NUT_DRAW);
			NVec3Add(&draw.Quad.Center,&psnappoint->Position,&shift);
			NSetColorf(&draw.Quad.Color,NCOLOR_PRESET3F_RED);
			NVec3Set(&draw.Quad.ConstructionPlane_DirectorVector,0,0,1);
			NVec2Set(&draw.Quad.Extents,0.025f,0.025f);
			draw.Quad.pDrawCallBack = NUT_GetDrawCallBack(NUT_DRAW_ID_QUAD);
			NUT_Draw(&draw,NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
		}
		if(psl->pCurrentSnapPoint)
		{
			NVEC3 dbg_A,dbg_B;
			NCOLOR	 dbg_color;
			NVec3Add(&dbg_A,&psl->SlidePos,&shift);
			NVec3Add(&dbg_B,&psl->pCurrentSnapPoint->Position,&shift);
			NSetColorf(&dbg_color,NCOLOR_PRESET3F_RED);
			NUT_Draw_OrthographicSegment(&dbg_A,&dbg_B,&dbg_color);
		}
		// DEBUG
	*/
}

NUI_SLIDE *NCreateUISlide(NUI *parent, NUI_SLIDE_DESC *pslayerdesc, const Nu32 user32)
{
	return NSetupUISlide(parent, NEW(NUI_SLIDE), pslayerdesc, user32);
}

NUI_SLIDE *NSetupUISlide(NUI *parent, NUI_SLIDE *pslayer, NUI_SLIDE_DESC *pslayerdesc, const Nu32 user32)
{
	NPushUISetUpProcess((NUI *)pslayer, (NUI_DESC *)pslayerdesc);

	// Notes about Sliding Layer Bounding Box.
	//	By default, Sliding Layer Bounding Box is set to the default UI values which is the entire screen...
	//	... and these values still the same, without any further updates.
	//	That means, Sliding layer will intercept all the Touch events, ALWAYS.
	//	It's possible, trough NUI_DESC to change these default parameters and use all the possible flags and params to do this.
	//	All of the possibles flags and params are working well with a sliding layer, with different results.

	// Call Parent Structure Setup (here, parent structure is NUI).

#ifdef _DEBUG
	if (!NSetupUI(parent, (NUI *)pslayer, (NUI_DESC *)pslayerdesc, user32))
	{
		// NPopUISetUpProcess triggered EVENT sending from NSetupUI but it's a bug !
		// And unfortunately, it seem's user delete the UI during event  processing (NUI_SETUP or/and NUI_ENABLE) ...
		// This should never happen !
		// (Maybe it's not exactly that but certainly something like that to have a NULL pointer returned by NSetupUI)
		NErrorIf(1, NERROR_UI_SETUPPROCESS_STACK_CORRUPTED);
	}
#else
	NSetupUI(parent, (NUI *)pslayer, (NUI_DESC *)pslayerdesc, user32);
#endif

	/*
						// TOUCH
						NVEC2s16				InitialTouchPosition;
						NVEC2s16				TouchPosition;
		// SWIPE
		NVEC2					NormTouchMove;	// SWIPE survey real touch move to decide what it has to do.
		NVEC2					SwipeI;			// To orient Swipe detection. This is the I Vector. J Vector will be calculated from this one if necessary.
		// By default, SwipeI is (1,0)
		Nf32						SwipeThreshold;	// DotProduct(TouchMove,SwipeI) has to be greater than "SwipeThreshold" to SWIPE
		// or DotProduct(TouchMove,SwipeJ) has to be greater than "SwipeThreshold" to SWIPE ASIDE (if its allowed of course)
		Nu16						SwipeJump;		// Index distance into the snap point array from current snap point to choose the next one in case of SWIPE.
		Nu16						SwipeAsideJump; // Index distance into the snap point array from current snap point to choose the next one in case of SWIPE ASIDE.
		Nf32						SwipeOnFieldSeekingAccuracy;
		NUI_SLIDE_SWIPE		Swipe_Handle;

		// PHYSICS
		NVEC3					SlidePos;
		NVEC3					OldSlidePos;

		Nf32						VerletDamping;			// Damping value used by VERLET Integration Sliding system to damp on fly extracted speed.
		Nf32						VerletFriction;			// Sliding Friction. The only one force applied to the slider when it slide.

		Nf32						DragDamping;			// Damping value used to "kill" initial sliding speed if user still static between TOUCH_START and TOUCH_END for a while...
		// ...Initial sliding speed is the speed the slider has when user TOUCH_END the slider. Because there is no speed but old and current position
		// ...this DragDamping value changes old position and push it closer and closer to the current position, each UI.
		// SNAP POINTS
		NUI_SLIDE_SNAPPOINT		*pCurrentSnapPoint;		// The current SnapPoint ... Active when Layer Slide
		NARRAY						SnapPointArray;			// All The Snap Points of the Layer
		Nf32						AttractDamping;			// Damping value used by SnapPoint to Attract slider by damping the distance between them
		Nf32						CaptureSquareRadius;	// radius x radius, just because its faster to test !

		// CONSTRAINT
		NUI_SLIDE_CONSTRAINT	Constraint;
	*/

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + Re-Setup System data & functions ( could be overwritten )
	pslayer->UI.CurrentState = NUI_STATE_SLIDE_REST;
	pslayer->UI.UIState_Proc = NUISlide_States_Handle;
	pslayer->UI.UIEventToState_Proc = NUISlide_EventToState_Handle;
	pslayer->UI.ClearUI_Proc = NClearUISlide;

	// Specific Default Touch Mode ( different than Simple UI )
	if (ISFLAG_OFF(pslayerdesc->UIDesc.Flags_Style, FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE))
		NSetUIListenTouchMode((NUI *)pslayer, NUI_LISTEN_TOUCH_MODE_BASIC);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + Setup TOUCH
	// 	pslayer->InitialTouchPosition.x		= 0;
	// 	pslayer->InitialTouchPosition.y		= 0;
	// 	pslayer->TouchPosition.x			= 0;
	// 	pslayer->TouchPosition.y			= 0;
	pslayer->NormTouchMoveAccumulator.x = 0;
	pslayer->NormTouchMoveAccumulator.y = 0;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Setup PHYSICS
	// +
	NVec3Set(&pslayer->OldSlidePos, 0, 0, 0);
	NVec3Set(&pslayer->SlidePos, 0, 0, 0);
	NVec3Set(&pslayer->SafeRepulseForce, 0, 0, 0);

	if (ISFLAG_ON(pslayerdesc->UIDesc.Flags_Core, FLAG_NUIDC_SLIDE_LIMITS_REPULSE))
		FLAG_ON(pslayer->UI.Flags, FLAG_NUI_SLIDE_LIMITS_REPULSE);

	if (ISFLAG_ON(pslayerdesc->UIDesc.Flags_Core, FLAG_NUIDC_SLIDE_TOUCH_END_SPEED_MULTIPLIER))
		FLAG_ON(pslayer->UI.Flags, FLAG_NUI_SLIDE_TOUCH_END_SPEED_MULTIPLIER);

	if (ISFLAG_ON(pslayerdesc->XFlags, FLAG_NUIX_SLIDE_USEPARAM_VERLETDAMPING))
		pslayer->VerletDamping = pslayerdesc->VerletDamping;
	else
		pslayer->VerletDamping = DEFAULT_NUI_SLIDE_VERLETDAMPING;

	if (ISFLAG_ON(pslayerdesc->XFlags, FLAG_NUIX_SLIDE_USEPARAM_VERLETFRICTION))
		pslayer->VerletFriction = pslayerdesc->VerletFriction;
	else
		pslayer->VerletFriction = DEFAULT_NUI_SLIDE_VERLETFRICTION;

	if (ISFLAG_ON(pslayerdesc->XFlags, FLAG_NUIX_SLIDE_USEPARAM_DRAGDAMPING))
		pslayer->DragDamping = pslayerdesc->DragDamping;
	else
		pslayer->DragDamping = DEFAULT_NUI_SLIDE_DRAGDAMPING;

	if (ISFLAG_ON(pslayerdesc->XFlags, FLAG_NUIX_SLIDE_USEPARAM_DRAG_SPRINGK))
		pslayer->DragSpringK = pslayerdesc->DragSpringK;
	else
		pslayer->DragSpringK = DEFAULT_NUI_SLIDE_DRAG_SPRINGK;

	if (ISFLAG_ON(pslayerdesc->XFlags, FLAG_NUIX_SLIDE_USEPARAM_SAFE_SPRINGK))
		pslayer->SafeSpringK = pslayerdesc->SafeSpringK;
	else
		pslayer->SafeSpringK = DEFAULT_NUI_SLIDE_SAFE_SPRINGK;

	if (ISFLAG_ON(pslayerdesc->XFlags, FLAG_NUIX_SLIDE_USEPARAM_SNAP_SPRINGK))
		pslayer->SnapSpringK = pslayerdesc->SnapSpringK;
	else
		pslayer->SnapSpringK = DEFAULT_NUI_SLIDE_SNAP_SPRINGK;

	if (ISFLAG_ON(pslayerdesc->UIDesc.Flags_Core, FLAG_NUIX_SLIDE_USEPARAM_SNAP_CAPTURE_RADIUS))
		pslayer->SquareSnapCaptureRadius = pslayerdesc->SnapCaptureRadius * pslayerdesc->SnapCaptureRadius;
	else
		pslayer->SquareSnapCaptureRadius = DEFAULT_NUI_SLIDE_SNAP_CAPTURE_RADIUS * DEFAULT_NUI_SLIDE_SNAP_CAPTURE_RADIUS;

	if (ISFLAG_ON(pslayerdesc->XFlags, FLAG_NUIX_SLIDE_USEPARAM_SAFE_SPRINGFRICTION))
		pslayer->SafeSpringFriction = pslayerdesc->SafeSpringFriction;
	else
		pslayer->SafeSpringFriction = DEFAULT_NUI_SLIDE_SAFE_SPRINGFRICTION;

	if (ISFLAG_ON(pslayerdesc->XFlags, FLAG_NUIX_SLIDE_USEPARAM_TOUCH_END_SPEED_MULTIPLIER))
		pslayer->TouchEndSpeedMultiplier = pslayerdesc->TouchEndSpeedMultiplier;
	else
		pslayer->SafeSpringFriction = DEFAULT_NUI_SLIDE_TOUCH_END_SPEED_MULTIPLIER;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Setup SNAP POINTS
	// +
	pslayer->pCurrentSnapPoint = NULL;

	if (ISFLAG_ON(pslayerdesc->UIDesc.Flags_Core, FLAG_NUIDC_SLIDE_SNAP_THRESHOLD))
		FLAG_ON(pslayer->UI.Flags, FLAG_NUI_SLIDE_SNAP_THRESHOLD);

	if (ISFLAG_ON(pslayerdesc->UIDesc.Flags_Core, FLAG_NUIDC_SLIDE_USEPARAM_SNAPPOINT_ARRAY_CAPACITY))
		NSetupArray(&pslayer->SnapPointArray, pslayerdesc->SnapPointArrayCapacity, sizeof(NUI_SLIDE_SNAPPOINT));
	else
		NSetupArray(&pslayer->SnapPointArray, 0, sizeof(NUI_SLIDE_SNAPPOINT));

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Setup PINs
	// +
	if (ISFLAG_ON(pslayerdesc->UIDesc.Flags_Core, FLAG_NUIDC_SLIDE_USEPARAM_PIN_ARRAY_CAPACITY))
		NSetupArray(&pslayer->PinArray, pslayerdesc->PinArrayCapacity, sizeof(NUI_SLIDE_PIN));
	else
		NSetupArray(&pslayer->PinArray, 0, sizeof(NUI_SLIDE_PIN));

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Setup WIPE
	// +
	if (ISFLAG_ON(pslayerdesc->UIDesc.Flags_Style, FLAG_NUIDS_SLIDE_USEPARAM_SWIPE_THRESHOLD))
		pslayer->SwipeThreshold = pslayerdesc->SwipeThreshold;
	else
		pslayer->SwipeThreshold = DEFAULT_NUI_SLIDE_SWIPE_THRESHOLD;

	if (ISFLAG_ON(pslayerdesc->UIDesc.Flags_Style, FLAG_NUIDS_SLIDE_USEPARAM_SWIPE_IDIRECTION))
		pslayer->SwipeI = pslayerdesc->SwipeIDirection;
	else
		NVec2Set(&pslayer->SwipeI, DEFAULT_NUI_SLIDE_SWIPE_IDIRECTION_2F);

	if (ISFLAG_ON(pslayerdesc->UIDesc.Flags_Style, FLAG_NUIDS_SLIDE_USEPARAM_SWIPE_JUMP))
		pslayer->SwipeJump = pslayerdesc->SwipeJump;
	else
		pslayer->SwipeJump = DEFAULT_NUI_SLIDE_SWIPE_JUMP;

	if (ISFLAG_ON(pslayerdesc->UIDesc.Flags_Style, FLAG_NUIDS_SLIDE_USEPARAM_SWIPE_ASIDE_JUMP))
		pslayer->SwipeAsideJump = pslayerdesc->SwipeAsideJump;
	// else ... No default value here, User HAS to DEFINE it if he wants to use it

	if (ISFLAG_ON(pslayerdesc->UIDesc.Flags_Style, FLAG_NUIDS_SLIDE_USEPARAM_SWIPE_ONFIELD_SEEKING_ACCURACY))
		pslayer->SwipeOnFieldSeekingAccuracy = pslayerdesc->SwipeOnFieldSeekingAccuracy;
	else
		pslayer->SwipeOnFieldSeekingAccuracy = DEFAULT_NUI_SLIDE_SWIPE_ONFIELD_SEEKING_ACCURACY;

	if (ISFLAG_ON(pslayerdesc->UIDesc.Flags_Style, FLAG_NUIDS_SLIDE_SWIPE_ONFIELD_SEEKING))
	{
		pslayer->Swipe_Handle = NSwipeUISlide_OnFieldSeeking;
	}
	else if (ISFLAG_ON(pslayerdesc->UIDesc.Flags_Style, FLAG_NUIDS_SLIDE_SWIPE_BIDIRECTIONNAL))
	{
		pslayer->Swipe_Handle = NSwipeUISlide_Bidirectionnal;
	}
	else if (ISFLAG_ON(pslayerdesc->UIDesc.Flags_Style, FLAG_NUIDS_SLIDE_SWIPE))
	{
		pslayer->Swipe_Handle = NSwipeUISlide_Basic;
	}
	else // useless but to stay crystal clear
	{
		pslayer->Swipe_Handle = NULL;
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Setup Misc Flags
	// +
	if (ISFLAG_ON(pslayerdesc->UIDesc.Flags_Core, FLAG_NUIDC_SLIDE_KILL_CHILD_TOUCH_FOCUS))
	{
		FLAG_ON(pslayer->UI.Flags, FLAG_NUI_SLIDE_KILL_CHILDREN_TOUCH_LISTENING);
	}
	else
	{
		FLAG_OFF(pslayer->UI.Flags, FLAG_NUI_SLIDE_KILL_CHILDREN_TOUCH_LISTENING);
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Setup default Sliding Layer constraints
	// +
	Nmem0(&pslayer->Constraint, NUI_SLIDE_CONSTRAINT);
	pslayer->Constraint.ApplyConstraint_Handle = NApplyUISlideConstraints_Basic;

	return (NUI_SLIDE *)NPopUISetUpProcess((NUI *)pslayer, (NUI_DESC *)pslayerdesc);
}
/*

// TOUCH
NVEC2s16				InitialTouchPosition;
NVEC2s16				TouchPosition;

// SWIPE
NVEC2					NormTouchMove;	// SWIPE survey real touch move to decide what it has to do.
NVEC2					SwipeI;			// To orient Swipe detection. This is the I Vector. J Vector will be calculated from this one if necessary.
// By default, SwipeI is (1,0)
Nf32						SwipeThreshold;	// DotProduct(TouchMove,SwipeI) has to be greater than "SwipeThreshold" to SWIPE
// or DotProduct(TouchMove,SwipeJ) has to be greater than "SwipeThreshold" to SWIPE ASIDE (if its allowed of course)
Nu16						SwipeJump;		// Index distance into the snap point array from current snap point to choose the next one in case of SWIPE.
Nu16						SwipeAsideJump; // Index distance into the snap point array from current snap point to choose the next one in case of SWIPE ASIDE.
Nf32						SwipeOnFieldSeekingAccuracy;
NUI_SLIDE_SWIPE		Swipe_Handle;

// PHYSICS
NVEC3					SlidePos;
NVEC3					OldSlidePos;

Nf32						VerletDamping;			// Damping value used by VERLET Integration Sliding system to damp on fly extracted speed.
Nf32						VerletFriction;			// Sliding Friction. The only one force applied to the slider when it slide.

Nf32						DragDamping;			// Damping value used to "kill" initial sliding speed if user still static between TOUCH_START and TOUCH_END for a while...
// ...Initial sliding speed is the speed the slider has when user TOUCH_END the slider. Because there is no speed but old and current position
// ...this DragDamping value changes old position and push it closer and closer to the current position, each UI.
// SNAP POINTS
NUI_SLIDE_SNAPPOINT		*pCurrentSnapPoint;		// The current SnapPoint ... Active when Layer Slide
NARRAY						SnapPointArray;			// All The Snap Points of the Layer
Nf32						AttractDamping;			// Damping value used by SnapPoint to Attract slider by damping the distance between them
Nf32						CaptureSquareRadius;	// radius x radius, just because its faster to test !

// CONSTRAINT
NUI_SLIDE_CONSTRAINT	Constraint;*/
void NClearUISlide(NUI *pui)
{
	// a)Clear Sliding Layer Section

	// 	((NUI_SLIDE*)pui)->InitialTouchPosition.x	= 0;
	// 	((NUI_SLIDE*)pui)->InitialTouchPosition.y	= 0;
	// 	((NUI_SLIDE*)pui)->TouchPosition.x			= 0;
	// 	((NUI_SLIDE*)pui)->TouchPosition.y			= 0;
	((NUI_SLIDE *)pui)->NormTouchMoveAccumulator.x = 0;
	((NUI_SLIDE *)pui)->NormTouchMoveAccumulator.y = 0;

	((NUI_SLIDE *)pui)->NormTouchMove.x = 0;
	((NUI_SLIDE *)pui)->NormTouchMove.y = 0;

	((NUI_SLIDE *)pui)->SwipeI.x = 0;
	((NUI_SLIDE *)pui)->SwipeI.y = 0;

	((NUI_SLIDE *)pui)->SwipeThreshold = 0;
	((NUI_SLIDE *)pui)->SwipeJump = 0;
	((NUI_SLIDE *)pui)->SwipeAsideJump = 0;
	((NUI_SLIDE *)pui)->SwipeOnFieldSeekingAccuracy = 0;
	((NUI_SLIDE *)pui)->Swipe_Handle = NULL;

	NVec3Set(&((NUI_SLIDE *)pui)->OldSlidePos, 0, 0, 0);
	NVec3Set(&((NUI_SLIDE *)pui)->SlidePos, 0, 0, 0);
	((NUI_SLIDE *)pui)->VerletDamping = 0.0f;
	((NUI_SLIDE *)pui)->VerletFriction = 0.0f;
	((NUI_SLIDE *)pui)->DragDamping = 0.0f;
	((NUI_SLIDE *)pui)->pCurrentSnapPoint = NULL;
	NClearArray(&((NUI_SLIDE *)pui)->SnapPointArray, NULL);

	NClearArray(&((NUI_SLIDE *)pui)->PinArray, NULL);

	((NUI_SLIDE *)pui)->DragSpringK = 0.0f;
	((NUI_SLIDE *)pui)->SafeSpringK = 0.0f;
	((NUI_SLIDE *)pui)->SnapSpringK = 0.0f;
	((NUI_SLIDE *)pui)->SquareSnapCaptureRadius = 0.0f;

	// !!! UISlide doesn't clear linked spline and linked AABB they should be already deleted and
	// Their pointer here should be set to NULL by user.
	NErrorIf(((NUI_SLIDE *)pui)->Constraint.ApplyConstraint_Handle == NApplyUISlideConstraints_Spline && ((NUI_SLIDE *)pui)->Constraint.Spline.pSpline, NERROR_NON_NULL_POINTER);
	NErrorIf(((NUI_SLIDE *)pui)->Constraint.ApplyConstraint_Handle == NApplyUISlideConstraints_AABB && ((NUI_SLIDE *)pui)->Constraint.AABB.pAABB, NERROR_NON_NULL_POINTER);
	Nmem0(&((NUI_SLIDE *)pui)->Constraint, NUI_SLIDE_CONSTRAINT);

	// a)Clear UI Section
	NClearUI_Basics(pui);
}

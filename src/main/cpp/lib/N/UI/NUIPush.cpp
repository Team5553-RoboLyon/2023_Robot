#include "lib/N/NCStandard.h"
#include "../Core/NSafeConversion.h"
#include "NUI.h"
#include "NUIColor.h"
#include "NUIPush.h"

static inline void UpdateFramePushTHN(NUI *pui)
{
	NVEC3 v, v0;

	Nf32 eased_ratio = NCLAMP(0.0f, pui->pStyle->pPushUIEasingHandle(pui->fPushUpDownRatio), 1.0f); // CLAMP due to Nf32 approxim...

	// printf("\n ratio = %.5f xratio = %.5f",pui->fPushUpDownRatio,eased_ratio);
	//  TRANSLATION
	//	"NO translations " means v0(0,0,0)
	//	So, Vector Translation involve between v0(0,0,0) and Push_DownTranslation(x,y,z).
	//  ... an NVEC3 Scale is enough to animate these Vector
	NVec3Scale(&v, &pui->pStyle->Push_DownTranslation, eased_ratio);
	NSetTransformHNodePosition(pui->pUITransformHNode, &v);

	// SCALE
	// "NO Scale" means vScale0(1,1,1)
	// So,  Vector Scale involve between vScale0(1,1,1) and Push_DownScale
	// ... we have to Lerp between these 2 vectors.
	NVec3Set(&v0, 1.0f, 1.0f, 1.0f);
	NVec3Lerp(&v, &v0, &pui->pStyle->Push_DownScale, eased_ratio);
	NSetTransformHNodeScale(pui->pUITransformHNode, &v);

	printf("\nRatio: %.2f --> %.2f SCALE: %.2f,%.2f,%.2f", pui->fPushUpDownRatio, eased_ratio, v.x, v.y, v.z);

	// ROTATIONS
	// "No Rotations" means quat identity.
	// So, Quat rotation involve between identity and Push_DownQuatRotation
	// ... we have to SLerp between these 2 Quaternions.
	NQUATERNION quat;
	NIdentityQuaternion(&quat);
	NQuaternionSlerp(&quat, &quat, &pui->pStyle->Push_DownQuatRotation, eased_ratio);
	NSetTransformHNodeRotationQuat(pui->pUITransformHNode, &quat);
}

Nu32 NUIPush_EventToState_PreHandle(NUI *pui, const NEVENT *pevent)
{
	Nu16 key_instant = _SafeNu32ToNu16(NUI_STATE_NULL);
	Nu16 previous_state = _SafeNu32ToNu16(pui->CurrentState);

	//	Check the recipient to be sure This UI is the recipient of the Event !!!
	//	BECAUSE it could be intercepting a child Event ( which is the real Recipient of the event !!! )
	//  The Event to State function that owns this push pre handle must test that before calling "NUIPush_EventToState_PreHandle"
	// ( have a look at "NUIPushButton_EventToState_Handle" )
	NErrorIf((NUI *)pevent->Head.pRecipient != pui, NERROR_SYSTEM_CHECK);
	// Prefer using NErrorIf here instead of the 2 lines bellow to encourage earlier test in the EventToState "parent" function...
	// 	if( (NUI*)pevent->Head.pRecipient != pui )
	// 		return NMAKE_UIPUSH_PREHANDLE_RESULT(key_instant,previous_state);

	switch (pevent->Type)
	{
	case NTOUCH_START:
		if (NIsUIFirstTouchMoveIn(pui) /*NIsFirstUITouchStarted(pui)*/)
		{
			// COLOR UPDATE ?
			if (ISFLAG_ON(pui->Flags, FLAG_NUI_COLOR_UPDATE))
				NSetUIColor(pui, NUI_COLORSET_FOCUS);

			switch (pui->CurrentState)
			{
			case NUI_STATE_PUSH_UP_REST:
			case NUI_STATE_PUSH_TO_UP:		  // |_ These 2 STATES exist only for Animated PUSH UI !
			case NUI_STATE_PUSH_TO_UP_CANCEL: // |
				// Animated PUSH
				if (pui->pUITransformHNode != pui->pRenderable->pTransformHNode)
				{
					pui->CurrentState = NUI_STATE_PUSH_TO_DOWN;
				}
				// NON Animated PUSH
				else
				{
					pui->CurrentState = NUI_STATE_PUSH_DOWN_WAIT;

					// KEY INSTANT PUSH DOWN
					if (ISFLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_AT_PUSH_KEY_INSTANTS))
						FLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_ONCE);

					key_instant = _SafeNu32ToNu16(NUI_KEY_INSTANT_PUSH_DOWN);
				}
				break;

#ifdef _DEBUG
			case NUI_STATE_PUSH_TO_DOWN:
				//	Impossible case here !!
				// 	Because the only way to be into this state is a first "NTOUCH_START".
				//  If we are already in this state, that would mean a first "NTOUCH_START" already happens ... which is not possible because initial test
				//	checks first THIS IS A FIRST touch start !!!!!
				NErrorIf(1, NERROR_SYSTEM_CHECK);
				break;

			case NUI_STATE_PUSH_DOWN_WAIT:
				//	Impossible case here !!
				NErrorIf(1, NERROR_SYSTEM_CHECK);
				break;
#endif

			default:
				NErrorIf(1, NERROR_SYSTEM_CHECK); // That means one state is not handle ???
				break;
			}
		}
		break;

	case NTOUCH_MOVE:
		// COLOR UPDATE ?
		if (ISFLAG_ON(pui->Flags, FLAG_NUI_COLOR_UPDATE))
		{
			if (NIsUIFirstTouchMoveIn(pui))
				NSetUIColor(pui, NUI_COLORSET_FOCUS);
			else if (NIsUILastTouchMoveOut(pui))
				NSetUIColor(pui, NUI_COLORSET_ENABLE);
		}
		switch (pui->CurrentState)
		{
		case NUI_STATE_PUSH_UP_REST:
		case NUI_STATE_PUSH_TO_UP_CANCEL:
			// 					if( ISFLAG_ON(pui->Flags,FLAG_NUI_UNDER_TOUCH_CONTROL) )
			// 					{
			// 						if( ISFLAG_ON(pui->Flags, FLAG_NUI_IS_UNDER_TOUCH) )
			if (NGetUITouchCountIn(pui))
			{
				// Animated PUSH
				if (pui->pUITransformHNode != pui->pRenderable->pTransformHNode)
				{
					pui->CurrentState = NUI_STATE_PUSH_TO_DOWN;
				}
				// NON Animated PUSH
				else
				{
					pui->CurrentState = NUI_STATE_PUSH_DOWN_WAIT;

					// KEY INSTANT PUSH DOWN
					if (ISFLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_AT_PUSH_KEY_INSTANTS))
						FLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_ONCE);

					key_instant = _SafeNu32ToNu16(NUI_KEY_INSTANT_PUSH_DOWN);
				}
			}
			// 					}
			break;

		case NUI_STATE_PUSH_TO_DOWN:
			// 					if( ISFLAG_ON(pui->Flags,FLAG_NUI_UNDER_TOUCH_CONTROL) )
			// 					{
			// 						if( ISFLAG_OFF(pui->Flags, FLAG_NUI_IS_UNDER_TOUCH) )
			if (!NGetUITouchCountIn(pui))
			{
				// Animated PUSH ONLY
				// It's not necessary to test if( pui->pUITransformHNode != pui->pRenderable->pTransformHNode )
				// because current state is "..._PUSH_TO_DOWN", which is only available for animated Pushbutton !!!
				pui->CurrentState = NUI_STATE_PUSH_TO_UP_CANCEL;
			}
			// 					}
			break;

		case NUI_STATE_PUSH_DOWN_WAIT:
			// 					if( ISFLAG_ON(pui->Flags,FLAG_NUI_UNDER_TOUCH_CONTROL) )
			// 					{
			// 						if( ISFLAG_OFF(pui->Flags, FLAG_NUI_IS_UNDER_TOUCH) )
			if (!NGetUITouchCountIn(pui))
			{
				// From "NUIPushButton_EventToState_Proc" and "NUI_STATE_PUSH_DOWN_WAIT" Only !!!
				// Purpose of this FrameInstant is sending a CANCEL COMMAND from "NUIPushButton_States_Handle"
				// like all the other COMMAND... even if the UI is not animated !

				// Animated PUSH
				if (pui->pUITransformHNode != pui->pRenderable->pTransformHNode)
				{
					pui->CurrentState = NUI_STATE_PUSH_TO_UP_CANCEL;
					// Note:There is no BBox updating here.
					//		for Push-animated Push,it going happens later at ending of "..._PUSH_TO_UP_CANCEL".
				}
				// NON Animated PUSH
				else
				{
					pui->CurrentState = NUI_STATE_PUSH_UP_REST;

					if (ISFLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_AT_PUSH_KEY_INSTANTS))
						FLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_ONCE);
				}
				// KEY INSTANT PUSH DOWN CANCEL
				key_instant = _SafeNu32ToNu16(NUI_KEY_INSTANT_PUSH_DOWN_CANCEL);
			}
			// 					}
			break;

#ifdef _DEBUG
		case NUI_STATE_PUSH_TO_UP:
			//	Impossible case here !!
			// 	Because the only way to be into this state is a last "NTOUCH_END" from NUI_STATE_PUSH_TO_DOWN or NUI_STATE_PUSH_TO_DOWN_WAIT.
			//  If we are already in this state, that would mean a last "NTOUCH_END" already happens ... which is not possible !!!!
			NErrorIf(1, NERROR_SYSTEM_CHECK);
			break;
#endif

		default:
			NErrorIf(1, NERROR_SYSTEM_CHECK); // That means one state is not handle ???
			break;
		}
		break;

	case NTOUCH_END:
		if (NIsUILastTouchEnded(pui))
		{
			// COLOR UPDATE ?
			if (ISFLAG_ON(pui->Flags, FLAG_NUI_COLOR_UPDATE))
				NSetUIColor(pui, NUI_COLORSET_ENABLE);

			switch (pui->CurrentState)
			{
			case NUI_STATE_PUSH_UP_REST:
			case NUI_STATE_PUSH_TO_UP_CANCEL:
				break;

			case NUI_STATE_PUSH_TO_DOWN:
			case NUI_STATE_PUSH_DOWN_WAIT:

				// Animated PUSH
				if (pui->pUITransformHNode != pui->pRenderable->pTransformHNode)
				{
					pui->CurrentState = NUI_STATE_PUSH_TO_UP;
				}
				// NON Animated PUSH
				else
				{
					pui->CurrentState = NUI_STATE_PUSH_UP_REST;

					// KEY INSTANT PUSH UP
					if (ISFLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_AT_PUSH_KEY_INSTANTS))
						FLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_ONCE);

					key_instant = _SafeNu32ToNu16(NUI_KEY_INSTANT_PUSH_UP);
				}
				break;

#ifdef _DEBUG
			case NUI_STATE_PUSH_TO_UP:
				//	Impossible case here !!
				// 	Because the only way to be into this state is "NTOUCH_END".
				//  If we are already in this state, that would mean "NTOUCH_END" already happens ... which is not possible !
				NErrorIf(1, NERROR_SYSTEM_CHECK);
				break;
#endif

			default:
				NErrorIf(1, NERROR_SYSTEM_CHECK); // That means one state is not handle ???
				break;
			}
		}
		break;

	case NTOUCH_CANCEL:
		if (NIsUILastTouchEnded(pui))
		{
			// COLOR UPDATE ?
			if (ISFLAG_ON(pui->Flags, FLAG_NUI_COLOR_UPDATE))
				NSetUIColor(pui, NUI_COLORSET_ENABLE);

			switch (pui->CurrentState)
			{
			case NUI_STATE_PUSH_UP_REST:
			case NUI_STATE_PUSH_TO_UP_CANCEL:
				break;

			case NUI_STATE_PUSH_TO_DOWN:
			case NUI_STATE_PUSH_TO_UP:
				// Animated PUSH
				if (pui->pUITransformHNode != pui->pRenderable->pTransformHNode)
				{
					pui->CurrentState = NUI_STATE_PUSH_TO_UP_CANCEL;
				}
				// NON Animated PUSH
				else
				{
					// We are not going to use "NUI_INSTANT_PUSHBUTTON_CANCEL"
					// we don't want to send any COMMAND (CANCEL or whatever ) just return to the initial state right now !
					// So, we do that ... with taking care of BBOX updating
					if (ISFLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_AT_PUSH_KEY_INSTANTS))
						FLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_ONCE);
					pui->CurrentState = NUI_STATE_PUSH_UP_REST;
				}
				break;

			case NUI_STATE_PUSH_DOWN_WAIT:
				// Animated PUSH
				if (pui->pUITransformHNode != pui->pRenderable->pTransformHNode)
				{
					pui->CurrentState = NUI_STATE_PUSH_TO_UP_CANCEL;
				}
				// NON Animated PUSH
				else
				{
					// taking care of BBOX updating
					if (ISFLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_AT_PUSH_KEY_INSTANTS))
						FLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_ONCE);
					pui->CurrentState = NUI_STATE_PUSH_UP_REST;
				}

				// KEY INSTANT PUSH DOWN CANCEL
				key_instant = _SafeNu32ToNu16(NUI_KEY_INSTANT_PUSH_DOWN_CANCEL);
				break;

			default:
				NErrorIf(1, NERROR_SYSTEM_CHECK); // That means one state is not handle ???
				break;
			}
		}
		break;

	// STATE RESET:
	case NUI_HIDE:
	case NUI_DISABLE:
	case NUI_CLEAR:
		if (pui->CurrentState != NUI_STATE_PUSH_UP_REST)
		{
			if (pui->fPushUpDownRatio != 0.0f)
			{
				pui->fPushUpDownRatio = 0.0f;
				UpdateFramePushTHN(pui);

				if (ISFLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_AT_PUSH_KEY_INSTANTS))
					FLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_ONCE);
			}

			pui->CurrentState = NUI_STATE_PUSH_UP_REST;
			key_instant = _SafeNu32ToNu16(NUI_KEY_INSTANT_PUSH_RESET);
		}
		break;

	default:
		break;
	}

	// function returns Previous UI current state packed with a potential Key instant.
	return NMAKE_UIPUSH_PREHANDLE_RESULT(key_instant, previous_state);
}

Nu32 NUIPush_States_PreHandle(NUI *pui, const NTIME *ptime)
{
	Nu16 key_instant = _SafeNu32ToNu16(NUI_STATE_NULL);
	Nu16 previous_state = _SafeNu32ToNu16(pui->CurrentState);

	switch (pui->CurrentState)
	{
	case NUI_STATE_PUSH_UP_REST:
		// Nothing to do
		break;

	case NUI_STATE_PUSH_DOWN_WAIT:
		// Nothing to do ! This FrameState is entirely managed by "NUIPushButton_EventToState_Proc".
		break;

	case NUI_STATE_PUSH_TO_UP:
		pui->fPushUpDownRatio -= ptime->Nf32_Delay * pui->pStyle->Push_UpSpeed;
		if (ISFLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_DURING_PUSH))
			FLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_ONCE);

		if (pui->fPushUpDownRatio <= 0.0f)
		{
			pui->fPushUpDownRatio = 0.0f;
			UpdateFramePushTHN(pui);

			pui->CurrentState = NUI_STATE_PUSH_UP_REST;

			// delayed KEY INSTANT PUSH_UP
			if (ISFLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_AT_PUSH_KEY_INSTANTS))
				FLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_ONCE);

			key_instant = _SafeNu32ToNu16(NUI_KEY_INSTANT_PUSH_UP);
		}
		else
		{
			UpdateFramePushTHN(pui);
		}
		break;

	case NUI_STATE_PUSH_TO_DOWN:
		pui->fPushUpDownRatio += ptime->Nf32_Delay * pui->pStyle->Push_DownSpeed;
		if (ISFLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_DURING_PUSH))
			FLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_ONCE);

		if (pui->fPushUpDownRatio >= 1.0f)
		{
			pui->fPushUpDownRatio = 1.0f;
			UpdateFramePushTHN(pui);

			pui->CurrentState = NUI_STATE_PUSH_DOWN_WAIT;

			// delayed KEY INSTANT PUSH_UP
			if (ISFLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_AT_PUSH_KEY_INSTANTS))
				FLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_ONCE);

			key_instant = _SafeNu32ToNu16(NUI_KEY_INSTANT_PUSH_DOWN);
		}
		else
		{
			UpdateFramePushTHN(pui);
		}
		break;

	case NUI_STATE_PUSH_TO_UP_CANCEL:
		pui->fPushUpDownRatio -= ptime->Nf32_Delay * pui->pStyle->Push_CancelSpeed;
		if (ISFLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_DURING_PUSH))
			FLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_ONCE);

		if (pui->fPushUpDownRatio <= 0.0f)
		{
			pui->fPushUpDownRatio = 0.0f;

			// BBOX updating ?
			if (ISFLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_AT_PUSH_KEY_INSTANTS))
				FLAG_ON(pui->Flags, FLAG_NUI_BBOX_UPDATE_ONCE);
			pui->CurrentState = NUI_STATE_PUSH_UP_REST;
		}
		UpdateFramePushTHN(pui);
		break;

	default:
		// It's not possible for a "PUSH" UI to manage some other NUI_STATE ... So CRASH !
		// Maybe it's just because a lack of proper init : "NUI.CurrentState = NUI_STATE_PUSH_UP_REST"
		NErrorIf(1, NERROR_UNAUTHORIZED_CASE);
		break;
	}

	// function returns Previous UI current state packed with a potential Key instant.
	return NMAKE_UIPUSH_PREHANDLE_RESULT(key_instant, previous_state);
}

#include "../../NCStandard.h"
#include "NUT_UITransitions.h"

NUITHNANIM_PLAYER*	NUT_SetupUITransition( NUITHNANIM_PLAYER *puithnanimplayer, const NUI *pui, const NVEC3 *pv, const NUT_UITRANSITION_ENUM transition_id, const Nf32 time, const NEASING_HANDLE_ID_ENUM easing, const Nbool ui_hide_force_transition_end_key_and_delete, const Nbool send_nnotify_start_end_events )
{
	NTHNANIM_KEY key[2];

	// Setup Animation keys
/*
	key[0].Time				= 0.0f;
	key[0].EasingHandleID	= NEASING_LINEAR;
	
	key[1].Time = 0.5f;
	key[1].EasingHandleID = NEASING_OUT;
	NVec3Set(&key[0].Translation,NCoord_NormToHRel_X(0.5f),NCoord_NormToHRel_Y(-0.5f),0);
	NVec3Set(&key[1].Translation,NCoord_NormToHRel_X(SNKS_PICKEDBONUSMENU_X),NCoord_NormToHRel_Y(SNKS_PICKEDBONUSMENU_Y+i*SNKS_PICKEDBONUSMENU_YSHIFT),0.0f);
	NVec3Set(&key[0].Scale,1.0f,1.0f,1.0f);
	NVec3Set(&key[1].Scale,1.0f,1.0f,1.0f);
	NQuaternionRotation(&key[0].Rotation,0,0,0);
	NQuaternionRotation(&key[1].Rotation,0,0,0);

	NSetUIPosition((NUI*)Snks_GameData.pInGamePickedBonusButton[i],&key[0].Translation); // Because this function is called from a postupdate ( collision ), after timer call ... so the first animation update is goig to happen at the next loop !
	NShowUI((NUI*)Snks_GameData.pInGamePickedBonusButton[i] );
	NSetUIUserData((NUI*)Snks_GameData.pInGamePickedBonusButton[i],NMAKELONG(1,bonus_id) );

	// And animate ...
	NCreateUIThnAnimationPlayer(	(NUI*)Snks_GameData.pInGamePickedBonusButton[i],0,
									FLAG_NUITHNANIM_PLAYER_NCOMMAND_END,
									NTHNANIM_PLAYER_END_BEHAVIOR_DELETE,
									0,
									key,2 );*/


	switch(transition_id)
	{
		// ---------------------------------------------------------------------------------------------------------
		// Transition from Left of the screen to the current position of UI.
		// (force UI to be out of screen and animate it to its current position or to pv position if it is defined )
		case NUT_UITRANSITION_IN_LEFT:
			NSetThnAnimKey_from_Thn(&key[1],pui->pRenderable->pTransformHNode,time,easing);
			if(pv)
				key[1].Translation = *pv;

			key[0] = key[1];
			key[0].Translation.x = -( pui->BoundingBox.right - pui->BoundingBox.left );
			// Change UI position to its Start point 
			NSetUIPosition(pui,&key[0].Translation);
			break;

		// ---------------------------------------------------------------------------------------------------------
		// Transition from right of the screen to the current position of UI.
		// (force UI to be out of screen and animate it to its current position or to pv position if it is defined )
		case NUT_UITRANSITION_IN_RIGHT:
			NSetThnAnimKey_from_Thn(&key[1],pui->pRenderable->pTransformHNode,time,easing);
			if(pv)
				key[1].Translation = *pv;

			key[0] = key[1];
			key[0].Translation.x = NCoord_NormToHRel_X(1.0f) + ( pui->BoundingBox.right - pui->BoundingBox.left );
			// Change UI position to its Start point 
			NSetUIPosition(pui,&key[0].Translation);
			break;

		// ---------------------------------------------------------------------------------------------------------
		// Transition from top of the screen to the current position of UI.
		// (force UI to be out of screen and animate it to its current position or to pv position if it is defined )
		case NUT_UITRANSITION_IN_TOP:
			NSetThnAnimKey_from_Thn(&key[1],pui->pRenderable->pTransformHNode,time,easing);
			if(pv)
				key[1].Translation = *pv;
			key[0] = key[1];
			key[0].Translation.y = ( pui->BoundingBox.top - pui->BoundingBox.bottom );
			// Change UI position to its Start point 
			NSetUIPosition(pui,&key[0].Translation);
			break;

		// ---------------------------------------------------------------------------------------------------------
		// Transition from bottom of the screen to the current position of UI.
		// (force UI to be out of screen and animate it to its current position or to pv position if it is defined )
		case NUT_UITRANSITION_IN_BOTTOM:
			NSetThnAnimKey_from_Thn(&key[1],pui->pRenderable->pTransformHNode,time,easing);
			if(pv)
				key[1].Translation = *pv;

			key[0] = key[1];
			key[0].Translation.y = NCoord_NormToHRel_Y(-1.0f) - ( pui->BoundingBox.top - pui->BoundingBox.bottom );
			
			// Change UI position to its Start point 
			NSetUIPosition(pui,&key[0].Translation);
			break;

		// ---------------------------------------------------------------------------------------------------------
		// Transition from a lower scale to the current scale of UI.
		// (force UI to be lower scaled and animate it to its current scale or to pv scale if it is defined )
		case NUT_UITRANSITION_IN_ZOOM:
			NSetThnAnimKey_from_Thn(&key[1],pui->pRenderable->pTransformHNode,time,easing);
			if(pv)
				key[1].Scale = *pv;

			key[0] = key[1];
			NVec3ScaleBy(&key[0].Scale, CONSTANT_TRANSITION_ZOOM_SCALE );

			// Change UI scale to its Start point 
			NSetUIScale(pui,&key[0].Scale);
			break;

			// ---------------------------------------------------------------------------------------------------------
			// Transition from a lower scale to the current scale of UI, but not too much.
			// (force UI to be lower scaled and animate it to its current scale or to pv scale if it is defined )
		case NUT_UITRANSITION_IN_ZOOM_SUBTLE:
			NSetThnAnimKey_from_Thn(&key[1],pui->pRenderable->pTransformHNode,time,easing);
			if(pv)
				key[1].Scale = *pv;

			key[0] = key[1];
			NVec3ScaleBy(&key[0].Scale, CONSTANT_TRANSITION_ZOOM_SCALE_SUBTLE );

			// Change UI scale to its Start point 
			NSetUIScale(pui,&key[0].Scale);
			break;

			// ---------------------------------------------------------------------------------------------------------
		// Transition current position of UI (or from pv position if it is defined) to LEFT of the screen.
		case NUT_UITRANSITION_OUT_LEFT:
			NSetThnAnimKey_from_Thn(&key[0],pui->pRenderable->pTransformHNode,time,easing);
			if(pv)
			{
				key[0].Translation = *pv;
				// Change UI position to its new Start point 
				NSetUIPosition(pui,pv);
			}
			key[1] = key[0];
			key[1].Translation.x = -( pui->BoundingBox.right - pui->BoundingBox.left );
			break;

		// ---------------------------------------------------------------------------------------------------------
		// Transition current position of UI (or from pv position if it is defined) to RIGHT of the screen.
		case NUT_UITRANSITION_OUT_RIGHT:
			NSetThnAnimKey_from_Thn(&key[0],pui->pRenderable->pTransformHNode,time,easing);
			if(pv)
			{
				key[0].Translation = *pv;
				// Change UI position to its new Start point 
				NSetUIPosition(pui,pv);
			}

			key[1] = key[0];
			key[1].Translation.x = NCoord_NormToHRel_X(1.0f) + ( pui->BoundingBox.right - pui->BoundingBox.left );
			break;

		// ---------------------------------------------------------------------------------------------------------
		// Transition current position of UI (or from pv position if it is defined) to TOP of the screen.
		case NUT_UITRANSITION_OUT_TOP:
			NSetThnAnimKey_from_Thn(&key[0],pui->pRenderable->pTransformHNode,time,easing);
			if(pv)
			{
				key[0].Translation = *pv;
				// Change UI position to its new Start point 
				NSetUIPosition(pui,pv);
			}

			key[1] = key[0];
			key[1].Translation.y = ( pui->BoundingBox.top - pui->BoundingBox.bottom );
			break;

		// ---------------------------------------------------------------------------------------------------------
		// Transition current position of UI (or from pv position if it is defined) to BOTTOM of the screen.
		case NUT_UITRANSITION_OUT_BOTTOM:
			NSetThnAnimKey_from_Thn(&key[0],pui->pRenderable->pTransformHNode,time,easing);
			if(pv)
			{
				key[0].Translation = *pv;
				// Change UI position to its new Start point 
				NSetUIPosition(pui,pv);
			}

			key[1] = key[0];
			key[1].Translation.y = NCoord_NormToHRel_Y(-1.0f) - ( pui->BoundingBox.top - pui->BoundingBox.bottom );
			break;

		// ---------------------------------------------------------------------------------------------------------
		// Transition from current scale of UI (or from pv scale if it is defined) to a lower scale.
		case NUT_UITRANSITION_OUT_ZOOM:
			NSetThnAnimKey_from_Thn(&key[0],pui->pRenderable->pTransformHNode,time,easing);
			if(pv)
			{
				key[0].Scale = *pv;
				NSetUIScale(pui,pv);
			}

			key[1] = key[0];
			NVec3ScaleBy(&key[1].Scale, CONSTANT_TRANSITION_ZOOM_SCALE );
			break;

			// ---------------------------------------------------------------------------------------------------------
			// Transition from current scale of UI (or from pv scale if it is defined) to a lower scale, but not too much.
		case NUT_UITRANSITION_OUT_ZOOM_SUBTLE:
			NSetThnAnimKey_from_Thn(&key[0],pui->pRenderable->pTransformHNode,time,easing);
			if(pv)
			{
				key[0].Scale = *pv;
				NSetUIScale(pui,pv);
			}

			key[1] = key[0];
			NVec3ScaleBy(&key[1].Scale, CONSTANT_TRANSITION_ZOOM_SCALE_SUBTLE );
			break;
	}
	
	Nu8 flags = 0;
	if( ui_hide_force_transition_end_key_and_delete )
	{
		// Force the Last Key and Delete Thn Anim Player if UI is hidden
		flags |= FLAGS_NUITHNANIM_PLAYER_UI_HIDE_FORCE_END_AND_DELETE;
	}
	if( send_nnotify_start_end_events )
	{
		// Send event NNOTIFY_START and  event NNOTIFY_END  to UI 
		flags |= FLAG_NUITHNANIM_PLAYER_NNOTIFY_START|FLAG_NUITHNANIM_PLAYER_NNOTIFY_END;
	}
	return NCreateUIThnAnimationPlayer(	pui,
										(Nu32)transition_id,	// ID of the transition
										flags,					// In line with user wishes thanks to the 2 incoming Nbool
										NTHNANIM_PLAYER_END_BEHAVIOR_DELETE,
										0,						// Repeat: No Repeat !
										key,2 );				// Key List with 2 keys
}

NUITHNANIM_PLAYER*	NUT_CreateUITransition( const NUI *pui, const NVEC3 *ppos, const NUT_UITRANSITION_ENUM transition_id, const Nf32 time, const NEASING_HANDLE_ID_ENUM easing, const Nbool ui_hide_force_transition_end_key_and_delete, const Nbool send_nnotify_start_end_events )
{
	return NUT_SetupUITransition( NEW(NUITHNANIM_PLAYER),pui,ppos,transition_id,time,easing,ui_hide_force_transition_end_key_and_delete,send_nnotify_start_end_events );
}


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **						NUIAnimationPlayer.cpp										**
// **																					**
// **																					**
// **	!!! Very close (but different) to  "NThnAnimationPlayer.cpp"	!!!				**
// **																					**
// **																					**
// **				!!! COMPARE the 2 files in case of update !!!						**
// **																					**
// **																					**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NUIThnAnimationPlayer.h"


extern NARRAY				NTimerPtrArray;

#define _uithnplayer_keyBindex(puiplayer)		(puiplayer->pKeyB - puiplayer->pFirstKey)
static void NUIThnAnimationPlayerTimer_TimerHandle(NTIMER *ptimer, const NTIME *ptime)
{			
	NUITHNANIM_PLAYER		*puiplayer = (NUITHNANIM_PLAYER*)ptimer;
	Nf32								ratio;
	NVEC3							v;
	NQUATERNION							quat;
	NTRANSFORM_HNODE					*pthn = puiplayer->pUI->pRenderable->pTransformHNode;
	NEVENT								nevent;
	
	if( !NIsUIVisible(puiplayer->pUI) )
	{
		switch( puiplayer->Flags&MASK_NUITHNANIM_PLAYER_UI_HIDE_BEHAVIOR )
		{
			case 0:
				// No FLAG__NUITHNANIM_PLAYER_UI_HIDE_xxx flag set to ON.
				// There is nothing special to do.
				// the UIThnPlayer is going to update its Thn and potentially send Event to its UI 
				// even if this one is hidden. (_UINOTIFY event may be sent to hidden and/or disable UI)
				break;

			case FLAGS_NUITHNANIM_PLAYER_UI_HIDE_FORCE_STOP_AND_DELETE:	// = 1
				// NUITHNANIM_PLAYER is going to be deleted.
				// THN is going to stay like it is ( equal to its last player update ).
				// There is No Event Sent to UI to notify this 
				if( FLAGS_TEST(puiplayer->Flags, FLAG_NUITHNANIM_PLAYER_NNOTIFY_STOP|FLAG_NUITHNANIM_PLAYER_STARTED,FLAG_NUITHNANIM_PLAYER_NNOTIFY_STOP|FLAG_NUITHNANIM_PLAYER_STARTED) )
				{
					NFillupEventHead(nevent,NNOTIFY_UITHNANIM_STOP,_SafeNs32ToNu8(_uithnplayer_keyBindex(puiplayer)),NEVENT_RECIPIENT_CAST_UI,puiplayer->pUI);
					nevent.UIThnAnim.UserID = puiplayer->UserID;
				}
				else
				{
					nevent.Type = 0;
				}

				NDeleteUIThnAnimationPlayer(puiplayer);

				// Post Event after deleting Thn to avoid to manage potential user nested UI Thn Delete
				if(nevent.Type)
					NPostDelayedUIEvent(&nevent);
				return;

			case FLAGS_NUITHNANIM_PLAYER_UI_HIDE_FORCE_END_AND_DELETE:	// = 2
				// TRANSLATION/SCALE/ROTATIONS
				NSetTransformHNodePosition(pthn,&puiplayer->pLastKey->Translation);
				NSetTransformHNodeScale(pthn,&puiplayer->pLastKey->Scale);
				NSetTransformHNodeRotationQuat(pthn,&puiplayer->pLastKey->Rotation);
				if( ISFLAG_ON(puiplayer->Flags, FLAG_NUITHNANIM_PLAYER_NNOTIFY_END) )
				{
					NFillupEventHead(nevent,NNOTIFY_UITHNANIM_END,_SafeNs32ToNu8(_uithnplayer_keyBindex(puiplayer)),NEVENT_RECIPIENT_CAST_UI,puiplayer->pUI);
					nevent.UIThnAnim.UserID = puiplayer->UserID;
				}
				else
				{
					nevent.Type = 0;
				}

				NDeleteUIThnAnimationPlayer(puiplayer);

				// Post Event after deleting Thn to avoid to manage potential user nested UI Thn Delete
				if(nevent.Type)
					NPostDelayedUIEvent(&nevent);
				return;

			case FLAGS_NUITHNANIM_PLAYER_UI_HIDE_FORCE_PAUSE:			// = 3	
				// Nothing Special, just return.
				// There is no Event to notify UITHN Anim Player entering Pause.
				return;

			default:
				NErrorIf(1,NERROR_UNAUTHORIZED_CASE);
				break;
		}
	}

	if( ISFLAG_OFF(puiplayer->Flags,FLAG_NUITHNANIM_PLAYER_STARTED) )
	{
		if( ISFLAG_ON(puiplayer->Flags, FLAG_NUITHNANIM_PLAYER_NNOTIFY_START) )
		{
			NFillupEventHead(nevent,NNOTIFY_UITHNANIM_START,0,NEVENT_RECIPIENT_CAST_UI,puiplayer->pUI);
			nevent.UIThnAnim.UserID = puiplayer->UserID;
			NPostDelayedUIEvent(&nevent);
		}
		FLAG_ON(puiplayer->Flags,FLAG_NUITHNANIM_PLAYER_STARTED);
	}

	puiplayer->Time += ptime->Nf32_Delay;

	// Manage Time and associated "KeyA ---> KeyB"
	while( puiplayer->Time >= puiplayer->pKeyB->Time )
	{
		puiplayer->Time -= puiplayer->pKeyB->Time;

		if(puiplayer->pKeyB == puiplayer->pLastKey) // reach End
		{
			switch(puiplayer->EndBehavior)
			{
				case NTHNANIM_PLAYER_END_BEHAVIOR_DELETE:
					// TRANSLATION/SCALE/ROTATIONS
					NSetTransformHNodePosition(pthn,&puiplayer->pKeyB->Translation);
					NSetTransformHNodeScale(pthn,&puiplayer->pKeyB->Scale);
					NSetTransformHNodeRotationQuat(pthn,&puiplayer->pKeyB->Rotation);
					if( ISFLAG_ON(puiplayer->Flags, FLAG_NUITHNANIM_PLAYER_NNOTIFY_END) )
					{
						NFillupEventHead(nevent,NNOTIFY_UITHNANIM_END,_SafeNs32ToNu8(_uithnplayer_keyBindex(puiplayer)),NEVENT_RECIPIENT_CAST_UI,puiplayer->pUI);
						nevent.UIThnAnim.UserID = puiplayer->UserID;
					}
					else
					{
						nevent.Type = 0;
					}

					NDeleteUIThnAnimationPlayer(puiplayer);

					// Post Event after deleting Thn to avoid to manage potential user nested UI Thn Delete
					if(nevent.Type)
						NPostDelayedUIEvent(&nevent);
					return;

				case NTHNANIM_PLAYER_END_BEHAVIOR_REPEAT:
					if(puiplayer->Repeat)
					{
						if(puiplayer->CurrentLoop == puiplayer->Repeat)
						{
							// TRANSLATION/SCALE/ROTATIONS
							NSetTransformHNodePosition(pthn,&puiplayer->pKeyB->Translation);
							NSetTransformHNodeScale(pthn,&puiplayer->pKeyB->Scale);
							NSetTransformHNodeRotationQuat(pthn,&puiplayer->pKeyB->Rotation);
							if( ISFLAG_ON(puiplayer->Flags, FLAG_NUITHNANIM_PLAYER_NNOTIFY_END) )
							{
								NFillupEventHead(nevent,NNOTIFY_UITHNANIM_END,_SafeNs32ToNu8(_uithnplayer_keyBindex(puiplayer)),NEVENT_RECIPIENT_CAST_UI,puiplayer->pUI);
								nevent.UIThnAnim.UserID = puiplayer->UserID;
							}
							else
							{
								nevent.Type = 0;
							}

							NDeleteUIThnAnimationPlayer(puiplayer);
							
							// Post Event after deleting Thn to avoid to manage potential user nested UI Thn Delete
							if(nevent.Type)
								NPostDelayedUIEvent(&nevent);
							return;
						}
						else
						{
							puiplayer->CurrentLoop ++;
							puiplayer->pKeyA			= puiplayer->pKeyB;
							puiplayer->pKeyB			= puiplayer->pFirstKey;	
							puiplayer->EasingHandle	= NGetEasingHandle(puiplayer->pKeyB->EasingHandleID);
							
							if( ISFLAG_ON(puiplayer->Flags, FLAG_NUITHNANIM_PLAYER_NNOTIFY_REPEAT) )
							{
								NFillupEventHead(nevent,NNOTIFY_UITHNANIM_REPEAT,puiplayer->CurrentLoop,NEVENT_RECIPIENT_CAST_UI,puiplayer->pUI);
								nevent.UIThnAnim.UserID = puiplayer->UserID;
								NPostDelayedUIEvent(&nevent);
							}
						}
					}
					else // LOOP (always !!!).  Outside 'NDeleteThnAnimationPlayer' is the only way to stop (and delete) it !
					{
						puiplayer->pKeyA			= puiplayer->pKeyB;
						puiplayer->pKeyB			= puiplayer->pFirstKey;	
						puiplayer->EasingHandle	= NGetEasingHandle(puiplayer->pKeyB->EasingHandleID);

						if( ISFLAG_ON(puiplayer->Flags, FLAG_NUITHNANIM_PLAYER_NNOTIFY_REPEAT) )
						{
							NFillupEventHead(nevent,NNOTIFY_UITHNANIM_REPEAT,0,NEVENT_RECIPIENT_CAST_UI,puiplayer->pUI);
							nevent.UIThnAnim.UserID = puiplayer->UserID;
							NPostDelayedUIEvent(&nevent);
						}
					}
					break;

				default:
					NErrorIf(1,NERROR_UNAUTHORIZED_CASE);
					return;
			}
		}
		else
		{
			// First send Event about Key ... to prevent player that Animation reach Key B !
			if( ISFLAG_ON(puiplayer->Flags, FLAG_NUITHNANIM_PLAYER_NNOTIFY_KEY) )
			{
				NFillupEventHead(nevent,NNOTIFY_UITHNANIM_KEY,_SafeNs32ToNu8(_uithnplayer_keyBindex(puiplayer)),NEVENT_RECIPIENT_CAST_UI,puiplayer->pUI);
				nevent.UIThnAnim.UserID = puiplayer->UserID;
				NPostDelayedUIEvent(&nevent);
			}
			// Then update keyA/KeyB
			puiplayer->pKeyA = puiplayer->pKeyB;
			puiplayer->pKeyB ++;
			puiplayer->EasingHandle = NGetEasingHandle(puiplayer->pKeyB->EasingHandleID);
			
		}
	}
	
	// Calculating ratio using KeyB Easing Handle
	ratio = puiplayer->EasingHandle( puiplayer->Time/puiplayer->pKeyB->Time );

	// TRANSLATION
	NVec3Lerp(&v,&puiplayer->pKeyA->Translation,&puiplayer->pKeyB->Translation,ratio);
	NSetTransformHNodePosition(pthn,&v);

	// SCALE
	NVec3Lerp(&v,&puiplayer->pKeyA->Scale,&puiplayer->pKeyB->Scale,ratio);
	NSetTransformHNodeScale(pthn,&v);

	// ROTATIONS
	NQuaternionSlerp(&quat,&puiplayer->pKeyA->Rotation,&puiplayer->pKeyB->Rotation,ratio);
	NSetTransformHNodeRotationQuat(pthn,&quat);

// 	printf("\n NUIThnAnimationPlayerTimer_TimerHandle called  %.3f/%.3f [%.4f]",puiplayer->Time , puiplayer->pKeyB->Time,ratio);
}

NUITHNANIM_PLAYER*	NSetupUIThnAnimationPlayer( NUITHNANIM_PLAYER *puithnanimplayer, const NUI *pui,const Nu32 userid, const Nu8 _flags,const NTHNANIM_PLAYER_END_BEHAVIOR_ENUM endbehavior,const Nu8 repeat,const NTHNANIM_KEY *pkeylist, const Nu32 keylist_size )
{
	NErrorIf(!puithnanimplayer||!pui||!pkeylist, NERROR_NULL_POINTER);	// all incoming pointers have to be define !
	NErrorIf(keylist_size<2,NERROR_INCONSISTENT_VALUES);				// An animation exists only with a minimum of 2 keys !
	NErrorIf(endbehavior >= NTHNANIM_PLAYER_END_BEHAVIOR_ENUM_SIZE, NERROR_INCONSISTENT_VALUES); // user doesn't use predefined values ?!
	NErrorIf((_flags&MASK_NUITHNANIM_PLAYER_USER_FLAGS)!=_flags, NERROR_UNAUTHORIZED_FLAG); // User try to use non allowed flags !!
	
	#ifdef _DEBUG
	// Check all keys integrity
	// Especially check if EasingHandleID is well defined !!!
	for(Nu32 dbg_i = 0; dbg_i < keylist_size;dbg_i++)
	{
		NErrorIf(pkeylist[dbg_i].EasingHandleID >= NEASING_HANDLE_ID_ENUM_SIZE, NERROR_INCONSISTENT_VALUES );
	}
	#endif
	
	Nmem0(puithnanimplayer,NUITHNANIM_PLAYER);
	NSetupTimer((NTIMER*)puithnanimplayer,NUIThnAnimationPlayerTimer_TimerHandle);
	puithnanimplayer->Timer.ID		= NTIMER_ID_UITHNANIM;		
	puithnanimplayer->pUI			= (NUI*)pui;
	puithnanimplayer->Flags			= _flags;		
	puithnanimplayer->EndBehavior	= _SafeNu32ToNu8(endbehavior);
	puithnanimplayer->Repeat		= repeat;
	puithnanimplayer->pFirstKey		= (NTHNANIM_KEY*)Nmalloc( keylist_size*sizeof(NTHNANIM_KEY) );
	memcpy( puithnanimplayer->pFirstKey, pkeylist, keylist_size*sizeof(NTHNANIM_KEY)  );

	puithnanimplayer->pLastKey		= puithnanimplayer->pFirstKey + (keylist_size - 1);
	puithnanimplayer->pKeyA			= puithnanimplayer->pFirstKey;
	puithnanimplayer->pKeyB			= puithnanimplayer->pKeyA + 1;
	puithnanimplayer->EasingHandle	= NGetEasingHandle(puithnanimplayer->pKeyB->EasingHandleID);
	puithnanimplayer->CurrentLoop	= 0;
	puithnanimplayer->Time			= 0.0f;
	
	puithnanimplayer->UserID		= userid;
	return puithnanimplayer;
}

NUITHNANIM_PLAYER*	NCreateUIThnAnimationPlayer(const NUI *pui,const Nu32 userid, const Nu8 _flags,const NTHNANIM_PLAYER_END_BEHAVIOR_ENUM endbehavior,const Nu8 repeat,const NTHNANIM_KEY *pkeylist, const Nu32 keylist_size )
{
	return NSetupUIThnAnimationPlayer( NEW(NUITHNANIM_PLAYER),pui,userid,_flags,endbehavior,repeat,pkeylist,keylist_size );
}

void NClearUIThnAnimationPlayer( NUITHNANIM_PLAYER *puithnanimplayer )
{
	NErrorIf(puithnanimplayer->Timer.ID != NTIMER_ID_UITHNANIM, NERROR_TIMER_WRONG_ID);
	puithnanimplayer->Timer.ID = NTIMER_ID_TIMER; // To authorized 'NClearTimer'
	NClearTimer((NTIMER*)puithnanimplayer);
	Nfree(puithnanimplayer->pFirstKey);
	Nmem0(puithnanimplayer,NUITHNANIM_PLAYER);
}
void NDeleteUIThnAnimationPlayer( NUITHNANIM_PLAYER *puithnanimplayer )
{
	NClearUIThnAnimationPlayer(puithnanimplayer);
	Nfree(puithnanimplayer);
}

// ------------------------------------------------------------------------------------------
// NTHNANIM_PLAYER* NGetThnAnimationPlayer
// ------------------------------------------------------------------------------------------
// Description :
//	Look for a transform Hnode animation Player in relation with incoming 'pthn' NTRANSFORM_HNODE.
//	The function returns the first Thn Animation Player it found. Be aware 'pthn' may have several  Animation Players
//	related with !
// ------------------------------------------------------------------------------------------
// In	:
//			NTRANSFORM_HNODE *pthn			a valid pointer on a transform Hnode.
// 
// Out :
//			NULL if no Animation Player was found otherwise a valid pointer of the first Thn Animation Player found.
//			
//
// ------------------------------------------------------------------------------------------
NUITHNANIM_PLAYER* NGetUIThnAnimationPlayer( NUI *pui )
{
	NTIMER** pptimer = (NTIMER**)NTimerPtrArray.pFirst;
	for( Nu32 i=NTimerPtrArray.Size;i!=0;i--,pptimer++ )
	{
		if((*pptimer) && (*pptimer)->ID == NTIMER_ID_UITHNANIM && ((NUITHNANIM_PLAYER*)(*pptimer))->pUI == pui )
		{
			return (NUITHNANIM_PLAYER*)*pptimer;
		}
	}
	return NULL;
}

// ------------------------------------------------------------------------------------------
// void NDeleteAllUIThnAnimationPlayer
// ------------------------------------------------------------------------------------------
// Description :
//	Look for and delete all transform Hnode animation Player in relation with incoming 'pthn' NTRANSFORM_HNODE.
//	If user doesn't define 'pui', all the UI transform Hnode animation Players are going to be deleted 
// ------------------------------------------------------------------------------------------
// In	:
//			NTRANSFORM_HNODE *pthn			a valid pointer on a transform Hnode.
//											or NULL pointer to delete All UI Thn Animation Players
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NDeleteAllUIThnAnimationPlayer( NUI *pui )
{
	NTIMER** pptimer = (NTIMER**)NTimerPtrArray.pFirst;
	if(pui)
	{
		for( Nu32 i=NTimerPtrArray.Size;i!=0;i--,pptimer++ )
		{
			if((*pptimer) && (*pptimer)->ID == NTIMER_ID_UITHNANIM && ((NUITHNANIM_PLAYER*)(*pptimer))->pUI == pui )
			{
				NDeleteUIThnAnimationPlayer((NUITHNANIM_PLAYER*)*pptimer);
			}
		}
	}
	else
	{
		for( Nu32 i=NTimerPtrArray.Size;i!=0;i--,pptimer++ )
		{
			if((*pptimer) && (*pptimer)->ID == NTIMER_ID_UITHNANIM )
			{
				NDeleteUIThnAnimationPlayer((NUITHNANIM_PLAYER*)*pptimer);
			}
		}
	}
}

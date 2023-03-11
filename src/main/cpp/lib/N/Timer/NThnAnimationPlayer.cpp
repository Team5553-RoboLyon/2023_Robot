// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							NThnAnimationPlayer.cpp									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/Core/NSafeConversion.h"
#include "NThnAnimationPlayer.h"

/*
#define NSetThnAnimPlayerTransf_FromKeyB(pthnplayer)					NSetTransformHNodePosition(pthnplayer->pThn,&pthnplayer->pKeyB->Translation);				\
																		NSetTransformHNodeScale(pthnplayer->pThn,&pthnplayer->pKeyB->Scale);						\
																		NSetTransformHNodeRotationQuat(pthnplayer->pThn,&pthnplayer->pKeyB->Rotation);


#define NSetThnAnimPlayerTransf_LerpKeyAKeyB(pthnplayer,v,quat,ratio)	NVec3Lerp(&v,&pthnplayer->pKeyA->Translation,&pthnplayer->pKeyB->Translation,ratio);		\
																		NSetTransformHNodePosition(pthnplayer->pThn,&v);											\	
																		NVec3Lerp(&v,&pthnplayer->pKeyA->Scale,&pthnplayer->pKeyB->Scale,ratio);					\
																		NSetTransformHNodeScale(pthnplayer->pThn,&v);												\	
																		NQuaternionSlerp(&quat,&pthnplayer->pKeyA->Rotation,&pthnplayer->pKeyB->Rotation,ratio);	\	
																		NSetTransformHNodeRotationQuat(pthnplayer->pThn,&quat);
																				
																				
#define NSetThnAnimPlayerTransf_Loop(pthnplayer)						pthnplayer->pKeyA			= pthnplayer->pKeyB;											\					
																		pthnplayer->pKeyB			= pthnplayer->pFirstKey;										\
																		pthnplayer->EasingHandle	= NGetEasingHandle(pthnplayer->pKeyB->EasingHandleID);


#define NSetThnAnimPlayerTransf_NextKey(pthnplayer)						pthnplayer->pKeyA = pthnplayer->pKeyB;														\		
																		pthnplayer->pKeyB ++;																		\	
																		pthnplayer->EasingHandle = NGetEasingHandle(pthnplayer->pKeyB->EasingHandleID);

*/

extern NARRAY				NTimerPtrArray;

static void NThnAnimationPlayerTimer_TimerHandle(NTIMER *ptimer, const NTIME *ptime)
{
	NTHNANIM_PLAYER	*pthnplayer = (NTHNANIM_PLAYER*)ptimer;
	Nf32							ratio;
	NVEC3						v;
	NQUATERNION						quat;
	
	pthnplayer->Time += ptime->Nf32_Delay;

	// Manage Time and associated "KeyA ---> KeyB"
	while( pthnplayer->Time >= pthnplayer->pKeyB->Time )
	{
		pthnplayer->Time -= pthnplayer->pKeyB->Time;

		if(pthnplayer->pKeyB == pthnplayer->pLastKey) // reach End
		{
			switch(pthnplayer->EndBehavior)
			{
				case NTHNANIM_PLAYER_END_BEHAVIOR_DELETE:
					// TRANSLATION/SCALE/ROTATIONS
					NSetTransformHNodePosition(pthnplayer->pThn,&pthnplayer->pKeyB->Translation);
					NSetTransformHNodeScale(pthnplayer->pThn,&pthnplayer->pKeyB->Scale);
					NSetTransformHNodeRotationQuat(pthnplayer->pThn,&pthnplayer->pKeyB->Rotation);
					
					NDeleteThnAnimationPlayer(pthnplayer);
					return;

				case NTHNANIM_PLAYER_END_BEHAVIOR_REPEAT:
					if(pthnplayer->Repeat)
					{
						if(pthnplayer->CurrentLoop == pthnplayer->Repeat)
						{
							// TRANSLATION/SCALE/ROTATIONS
							NSetTransformHNodePosition(pthnplayer->pThn,&pthnplayer->pKeyB->Translation);
							NSetTransformHNodeScale(pthnplayer->pThn,&pthnplayer->pKeyB->Scale);
							NSetTransformHNodeRotationQuat(pthnplayer->pThn,&pthnplayer->pKeyB->Rotation);

							NDeleteThnAnimationPlayer(pthnplayer);
						}
						else
						{
							pthnplayer->CurrentLoop ++;
							pthnplayer->pKeyA			= pthnplayer->pKeyB;
							pthnplayer->pKeyB			= pthnplayer->pFirstKey;	
							pthnplayer->EasingHandle	= NGetEasingHandle(pthnplayer->pKeyB->EasingHandleID);
						}
					}
					else // LOOP (always !!!).  'NDeleteThnAnimationPlayer' is the only way to stop (and delete) it from outside!
					{
						pthnplayer->pKeyA			= pthnplayer->pKeyB;
						pthnplayer->pKeyB			= pthnplayer->pFirstKey;	
						pthnplayer->EasingHandle	= NGetEasingHandle(pthnplayer->pKeyB->EasingHandleID);
					}
					return;

				default:
					NErrorIf(1,NERROR_UNAUTHORIZED_CASE);
					return;
			}
		}
		else
		{
			pthnplayer->pKeyA = pthnplayer->pKeyB;
			pthnplayer->pKeyB ++;
			pthnplayer->EasingHandle = NGetEasingHandle(pthnplayer->pKeyB->EasingHandleID);
		}
	}
	
	// Calculating ratio using KeyB Easing Handle
	ratio = pthnplayer->EasingHandle( pthnplayer->Time/pthnplayer->pKeyB->Time );

	// TRANSLATION
	NVec3Lerp(&v,&pthnplayer->pKeyA->Translation,&pthnplayer->pKeyB->Translation,ratio);
	NSetTransformHNodePosition(pthnplayer->pThn,&v);

	// SCALE
	NVec3Lerp(&v,&pthnplayer->pKeyA->Scale,&pthnplayer->pKeyB->Scale,ratio);
	NSetTransformHNodeScale(pthnplayer->pThn,&v);

	// ROTATIONS
	NQuaternionSlerp(&quat,&pthnplayer->pKeyA->Rotation,&pthnplayer->pKeyB->Rotation,ratio);
	NSetTransformHNodeRotationQuat(pthnplayer->pThn,&quat);
}

NTHNANIM_PLAYER*	NSetupThnAnimationPlayer( NTHNANIM_PLAYER *pthnanimplayer, const NTRANSFORM_HNODE *pthn,const NTHNANIM_PLAYER_END_BEHAVIOR_ENUM endbehavior,const Nu8 repeat,const NTHNANIM_KEY *pkeylist, const Nu32 keylist_size )
{
	NErrorIf(!pthnanimplayer||!pthn||!pkeylist, NERROR_NULL_POINTER);	// all incoming pointers have to be define !
	NErrorIf(keylist_size<2,NERROR_INCONSISTENT_VALUES);				// An animation exists only with a minimum of 2 keys !
	NErrorIf(endbehavior >= NTHNANIM_PLAYER_END_BEHAVIOR_ENUM_SIZE , NERROR_INCONSISTENT_VALUES); // user doesn't use predefined values ?!
	
	#ifdef _DEBUG
	// Check all keys integrity
	// Especially check if EasingHandleID is well defined !!!
	for(Nu32 dbg_i = 0; dbg_i < keylist_size;dbg_i++)
	{
		NErrorIf(pkeylist[dbg_i].EasingHandleID >= NEASING_HANDLE_ID_ENUM_SIZE, NERROR_INCONSISTENT_VALUES );
	}
	#endif
	
	Nmem0(pthnanimplayer,NTHNANIM_PLAYER);
	NSetupTimer((NTIMER*)pthnanimplayer,NThnAnimationPlayerTimer_TimerHandle);
	pthnanimplayer->Timer.ID		= NTIMER_ID_THN_ANIMATION;		
	pthnanimplayer->pThn			= (NTRANSFORM_HNODE*)pthn;
	pthnanimplayer->EndBehavior		= _SafeNu32ToNu8(endbehavior);
	pthnanimplayer->Repeat			= repeat;
	pthnanimplayer->pFirstKey		= (NTHNANIM_KEY*)Nmalloc( keylist_size*sizeof(NTHNANIM_KEY) );
	memcpy( pthnanimplayer->pFirstKey, pkeylist, keylist_size*sizeof(NTHNANIM_KEY)  );

	pthnanimplayer->pLastKey		= pthnanimplayer->pFirstKey + (keylist_size - 1);
	pthnanimplayer->pKeyA			= pthnanimplayer->pFirstKey;
	pthnanimplayer->pKeyB			= pthnanimplayer->pKeyA + 1;
	pthnanimplayer->EasingHandle	= NGetEasingHandle(pthnanimplayer->pKeyB->EasingHandleID);
	pthnanimplayer->CurrentLoop		= 0;
	pthnanimplayer->Time			= 0.0f;
	
	return pthnanimplayer;
}

NTHNANIM_PLAYER*	NCreateThnAnimationPlayer(const NTRANSFORM_HNODE *pthn,const NTHNANIM_PLAYER_END_BEHAVIOR_ENUM endbehavior,const Nu8 repeat,const NTHNANIM_KEY *pkeylist, const Nu32 keylist_size )
{
	return NSetupThnAnimationPlayer( NEW(NTHNANIM_PLAYER),pthn,endbehavior,repeat,pkeylist,keylist_size );
}

void NClearThnAnimationPlayer( NTHNANIM_PLAYER *pthnanimplayer )
{
	NErrorIf(pthnanimplayer->Timer.ID != NTIMER_ID_THN_ANIMATION, NERROR_TIMER_WRONG_ID);
	pthnanimplayer->Timer.ID = NTIMER_ID_TIMER; // To authorized 'NClearTimer'
	NClearTimer((NTIMER*)pthnanimplayer);
	Nfree(pthnanimplayer->pFirstKey);
	Nmem0(pthnanimplayer,NTHNANIM_PLAYER);
}
void NDeleteThnAnimationPlayer( NTHNANIM_PLAYER *pthnanimplayer )
{
	NClearThnAnimationPlayer(pthnanimplayer);
	Nfree(pthnanimplayer);
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
NTHNANIM_PLAYER* NGetThnAnimationPlayer( NTRANSFORM_HNODE *pthn )
{
	NTIMER** pptimer = (NTIMER**)NTimerPtrArray.pFirst;
	for( Nu32 i=NTimerPtrArray.Size;i!=0;i--,pptimer++ )
	{
		if((*pptimer) && (*pptimer)->ID == NTIMER_ID_THN_ANIMATION && ((NTHNANIM_PLAYER*)(*pptimer))->pThn == pthn )
		{
			return (NTHNANIM_PLAYER*)*pptimer;
		}
	}
	return NULL;
}

// ------------------------------------------------------------------------------------------
// void NDeleteAllThnAnimationPlayer
// ------------------------------------------------------------------------------------------
// Description :
//	Look for and delete all transform Hnode animation Player in relation with incoming 'pthn' NTRANSFORM_HNODE.
//	If user doesn't define 'pui', all the transform Hnode animation Players are going to be deleted 
// ------------------------------------------------------------------------------------------
// In	:
//			NTRANSFORM_HNODE *pthn			a valid pointer on a transform Hnode.
//											or NULL pointer to delete All Thn Animation Players
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NDeleteAllThnAnimationPlayer( NTRANSFORM_HNODE *pthn )
{
	NTIMER** pptimer = (NTIMER**)NTimerPtrArray.pFirst;
	if(pthn)
	{
		for( Nu32 i=NTimerPtrArray.Size;i!=0;i--,pptimer++ )
		{
			if((*pptimer) && (*pptimer)->ID == NTIMER_ID_THN_ANIMATION && ((NTHNANIM_PLAYER*)(*pptimer))->pThn == pthn )
			{
				NDeleteThnAnimationPlayer((NTHNANIM_PLAYER*)*pptimer);
			}
		}
	}
	else
	{
		for( Nu32 i=NTimerPtrArray.Size;i!=0;i--,pptimer++ )
		{
			if((*pptimer) && (*pptimer)->ID == NTIMER_ID_THN_ANIMATION )
			{
				NDeleteThnAnimationPlayer((NTHNANIM_PLAYER*)*pptimer);
			}
		}
	}
}

#ifndef __N_THN_ANIMATION_PLAYER_H
#define __N_THN_ANIMATION_PLAYER_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NThnAnimationPlayer.h							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NType.h"
#include "../NTransformationTree.h"
#include "../Containers/NArray.h"
#include "NThnAnimationKey.h"
#include "NTimer.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **					NThnAnimationPlayer Structures/Define & flags					**
// ***************************************************************************************
typedef enum
{
	NTHNANIM_PLAYER_END_BEHAVIOR_DELETE = 0,
	NTHNANIM_PLAYER_END_BEHAVIOR_REPEAT,
// --------------------------------------------------
	NTHNANIM_PLAYER_END_BEHAVIOR_ENUM_SIZE
}NTHNANIM_PLAYER_END_BEHAVIOR_ENUM;

typedef struct
{
	NTIMER				Timer;
	NTRANSFORM_HNODE	*pThn;
	Nu8					Flags;
	Nu8					CurrentLoop;
	Nu8					Repeat;
	Nu8					EndBehavior;
	NTHNANIM_KEY		*pFirstKey;
	NTHNANIM_KEY		*pLastKey;
	NTHNANIM_KEY		*pKeyA;
	NTHNANIM_KEY		*pKeyB;
	NEASING_HANDLE		EasingHandle;
	Nf32				Time;
}NTHNANIM_PLAYER;

// ***************************************************************************************
// **						 ElapsedTimer Functions										**
// ***************************************************************************************
NTHNANIM_PLAYER*	NSetupThnAnimationPlayer( NTHNANIM_PLAYER *pthnanimplayer, const NTRANSFORM_HNODE *pthn, const NTHNANIM_PLAYER_END_BEHAVIOR_ENUM endbehavior,const Nu8 repeat,const NTHNANIM_KEY *pkeylist, const Nu32 keylist_size );
NTHNANIM_PLAYER*	NCreateThnAnimationPlayer(const NTRANSFORM_HNODE *pthn,const NTHNANIM_PLAYER_END_BEHAVIOR_ENUM endbehavior,const Nu8 repeat,const NTHNANIM_KEY *pkeylist, const Nu32 keylist_size );
void				NClearThnAnimationPlayer(NTHNANIM_PLAYER *pthnanimplayer);
void				NDeleteThnAnimationPlayer(NTHNANIM_PLAYER *pthnanimplayer);

// Advanced
NTHNANIM_PLAYER*	NGetThnAnimationPlayer( NTRANSFORM_HNODE *pthn );
void				NDeleteAllThnAnimationPlayer( NTRANSFORM_HNODE *pthn );

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __N_THN_ANIMATION_PLAYER_H 


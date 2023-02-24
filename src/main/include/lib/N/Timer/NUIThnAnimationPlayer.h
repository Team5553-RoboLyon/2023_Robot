#ifndef __N_UI_THN_ANIMATION_PLAYER_H
#define __N_UI_THN_ANIMATION_PLAYER_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NUIThnAnimationPlayer.h							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "../NTransformationTree.h"
#include "../Containers/NArray.h"
#include "../Miscellaneous/NEasing.h"
#include "../UI/NUI.h"
#include "NThnAnimationPlayer.h"
#include "NTimer.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

	// ***************************************************************************************
	// **					NUIAnimationPlayer Structures/Define & flags					**
	// ***************************************************************************************
	// About FLAGS_NUITHNANIM_PLAYER_UI_HIDE_xxx Flags
	//		The NUITHNANIM player may have specific behavior if it detect UI is hidden.
	//		By default it does nothing special and continues to update its THN and continue to sent appropriate Events
	//		if user asks for. ( !!! UI may receive _UINOTIFY event even if it is hidden or disable !!! )
	//		By setting to ON one of these 3 Flags sets, the player may behave in a different way.
	//
	//			FLAGS_NUITHNANIM_PLAYER_UI_HIDE_FORCE_STOP_AND_DELETE	When player detect its UI is hidden it stops is update and is automatically deleted.
	//																	THN is going to stay like it is ( equal to its last player update ).
	//
	//			FLAGS_NUITHNANIM_PLAYER_UI_HIDE_FORCE_END_AND_DELETE	When player detect its UI is hidden it stops is update and is automatically deleted.
	//																	THN is going to be update with the last player key values.
	//
	//			FLAGS_NUITHNANIM_PLAYER_UI_HIDE_FORCE_PAUSE				When player detect its UI is hidden it returns without doing anything.
	//																	THN is going to stay like it is ( equal to its last player update ).
	//																	but Player is not deleted and continue to be called each turn.
	//																	Update will continue as usual when UI is going to be Showed !

#define FLAGS_NUITHNANIM_PLAYER_UI_HIDE_FORCE_STOP_AND_DELETE BIT_0 // = 1
#define FLAGS_NUITHNANIM_PLAYER_UI_HIDE_FORCE_END_AND_DELETE BIT_1	// = 2
#define FLAGS_NUITHNANIM_PLAYER_UI_HIDE_FORCE_PAUSE (BIT_0 | BIT_1) // = 3

#define FLAG_NUITHNANIM_PLAYER_NNOTIFY_START BIT_2
#define FLAG_NUITHNANIM_PLAYER_NNOTIFY_KEY BIT_3
#define FLAG_NUITHNANIM_PLAYER_NNOTIFY_END BIT_4
#define FLAG_NUITHNANIM_PLAYER_NNOTIFY_REPEAT BIT_5
#define FLAG_NUITHNANIM_PLAYER_NNOTIFY_STOP BIT_6
#define FLAG_NUITHNANIM_PLAYER_STARTED BIT_7

#define MASK_NUITHNANIM_PLAYER_NCOMMAND (FLAG_NUITHNANIM_PLAYER_NNOTIFY_START | FLAG_NUITHNANIM_PLAYER_NNOTIFY_KEY | FLAG_NUITHNANIM_PLAYER_NNOTIFY_END | FLAG_NUITHNANIM_PLAYER_NNOTIFY_REPEAT | FLAG_NUITHNANIM_PLAYER_NNOTIFY_STOP)
#define MASK_NUITHNANIM_PLAYER_UI_HIDE_BEHAVIOR (BIT_0 | BIT_1)
#define MASK_NUITHNANIM_PLAYER_USER_FLAGS (MASK_NUITHNANIM_PLAYER_NCOMMAND | MASK_NUITHNANIM_PLAYER_UI_HIDE_BEHAVIOR)

	typedef struct
	{
		NTIMER Timer;
		NUI *pUI;
		Nu8 Flags;
		Nu8 CurrentLoop;
		Nu8 Repeat;
		Nu8 EndBehavior;
		NTHNANIM_KEY *pFirstKey;
		NTHNANIM_KEY *pLastKey;
		NTHNANIM_KEY *pKeyA;
		NTHNANIM_KEY *pKeyB;
		NEASING_HANDLE EasingHandle;
		Nf32 Time;
		Nu32 UserID;
	} NUITHNANIM_PLAYER;

	// ***************************************************************************************
	// **						 ElapsedTimer Functions										**
	// ***************************************************************************************
	NUITHNANIM_PLAYER *NSetupUIThnAnimationPlayer(NUITHNANIM_PLAYER *puithnanimplayer, const NUI *pui, const Nu32 userid, const Nu8 _flags, const NTHNANIM_PLAYER_END_BEHAVIOR_ENUM endbehavior, const Nu8 repeat, const NTHNANIM_KEY *pkeylist, const Nu32 keylist_size);
	NUITHNANIM_PLAYER *NCreateUIThnAnimationPlayer(const NUI *pui, const Nu32 userid, const Nu8 _flags, const NTHNANIM_PLAYER_END_BEHAVIOR_ENUM endbehavior, const Nu8 repeat, const NTHNANIM_KEY *pkeylist, const Nu32 keylist_size);
	void NClearUIThnAnimationPlayer(NUITHNANIM_PLAYER *puithnanimplayer);
	void NDeleteUIThnAnimationPlayer(NUITHNANIM_PLAYER *puithnanimplayer);

	// Advanced
	NUITHNANIM_PLAYER *NGetUIThnAnimationPlayer(NUI *pui);
	void NDeleteAllUIThnAnimationPlayer(NUI *pui);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __N_UI_THN_ANIMATION_PLAYER_H

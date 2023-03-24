#ifndef __NUT_UI_TRANSITIONS_H
#define __NUT_UI_TRANSITIONS_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NUT_UITransitions.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../UI/NUI.h"
#include "../../Timer/NUIThnAnimationPlayer.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

#define CONSTANT_TRANSITION_ZOOM_SCALE			0.75f		// Used by NUT_UITRANSITION_IN_ZOOM /NUT_UITRANSITION_OUT_ZOOM 		
#define CONSTANT_TRANSITION_ZOOM_SCALE_SUBTLE	0.95f		// Used by NUT_UITRANSITION_IN_ZOOM_SUBTLE /NUT_UITRANSITION_OUT_ZOOM_SUBTLE 		

typedef enum
{
	NUT_UITRANSITION_IN_LEFT,
	NUT_UITRANSITION_IN_RIGHT,
	NUT_UITRANSITION_IN_BOTTOM,
	NUT_UITRANSITION_IN_TOP,
	NUT_UITRANSITION_IN_ZOOM,
	NUT_UITRANSITION_IN_ZOOM_SUBTLE,
	
	NUT_UITRANSITION_OUT_LEFT,
	NUT_UITRANSITION_OUT_RIGHT,
	NUT_UITRANSITION_OUT_BOTTOM,
	NUT_UITRANSITION_OUT_TOP,
	NUT_UITRANSITION_OUT_ZOOM,
	NUT_UITRANSITION_OUT_ZOOM_SUBTLE,
// ---------------------------------
	NUT_UITRANSITION_ENUM_SIZE
}NUT_UITRANSITION_ENUM;

// These functions setup,create clear and delete simple 'UIThnAnimationPlayer'.
 
NUITHNANIM_PLAYER*	NUT_SetupUITransition( NUITHNANIM_PLAYER *puithnanimplayer, const NUI *pui, const NVEC3 *ppos, const NUT_UITRANSITION_ENUM transition_id, const Nf32 time, const NEASING_HANDLE_ID_ENUM easing, const Nbool ui_hide_force_transition_end_key_and_delete, const Nbool send_nnotify_start_end_events );
NUITHNANIM_PLAYER*	NUT_CreateUITransition( const NUI *pui, const NVEC3 *ppos, const NUT_UITRANSITION_ENUM transition_id, const Nf32 time, const NEASING_HANDLE_ID_ENUM easing, const Nbool ui_hide_force_transition_end_key_and_delete, const Nbool send_nnotify_start_end_events );
#define				NUT_ClearUITransition(pt)		NClearUIThnAnimationPlayer(pt)
#define				NUT_DeleteUITransition(pt)		NDeleteUIThnAnimationPlayer(pt)



// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NUT_UI_TRANSITIONS_H 




#ifndef __NGAMESTATES_H
#define __NGAMESTATES_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NGameStates.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NType.h"
#include "lib/N/Containers/NArray.h"
#include "lib/N/Event/NEvent.h"
#include "lib/N/Core/NTime.h"
#include "lib/N/Touch/NTouchListener.h"
#include "NGameStatesUID.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **					  Functions prototypes and Structures							**
// ***************************************************************************************

// #define STATE_HANDLEFCT_LISTSIZE			4			// To define, for each GameState, how many functions pointers can be initially stored into.
// #define STATE_HANDLEFCT_LISTREALLOCSIZE		4		// In case of dynamic reallocation, how many new blocs are reallocated ?

// NGAMESTATE use the "NTOUCH_LISTENER.Flags member" with its own flags ( into range of flags authorized by NTOUCH_LISTENER... of course )
// GameStates are working in slightly diffrent way than UI bout touch capture.
// There is No Touch capture Mode for GameState, a game state with an event handle is suppose to listen all the touch gestures, and
// because "gamestates" dont have any BBOX the concept of Touch Move IN, Touch Move OUT and Touch Under doesn't exist.
// ... Excepted in ONE SPECIFIC CASE ...
// ... When an UI that blocks Touch Event for Game states release a Touch due to a touch move OUT ...
// this UI doesn't mask this touch any more ... By default, a game state can not receive the touch because it didn't receive first the
// associated toucch start even t( it was blecked by the UI ).
// BUT with the flag set to ON ... it can ...
#define FLAG_NGAMESTATE_NTOUCH_LISTENER_CAPTURE_TOUCH_MOVE_RELEASED_BY_UI BIT_4

	// "Eat messages" functions
	typedef void (*STATEHANDLE_EVENTDRIVEN_FCT)(const NEVENT *pevent, const Nu32 User_Nu32);
	typedef void (*STATEHANDLE_TIMEDRIVEN_FCT)(const NTIME *ptime, const Nu32 User_Nu32);
	typedef void (*STATESTART_FCT)(const NSTATEUID_ENUM previous_stateuid, const Nu32 User_Nu32);
	typedef void (*STATEEND_FCT)(const NSTATEUID_ENUM next_stateuid, const Nu32 User_Nu32);

	// a GameState
	typedef struct NGAMESTATE NGAMESTATE;
	struct NGAMESTATE
	{
		STATEHANDLE_EVENTDRIVEN_FCT pEventDrivenHandle;
		STATEHANDLE_TIMEDRIVEN_FCT pTimeDrivenHandle;

		STATESTART_FCT pGameStateStart;
		STATEEND_FCT pGameStateEnd;

		Nu32 User_Nu32;

		// Interactive Data
		NTOUCH_LISTENER Listener;

		NGAMESTATE *pParent;
		NGAMESTATE *pCurrentChild;
		NGAMESTATE *pCurrentEventChild;
		NGAMESTATE *pCurrentTimeChild;
	};

	// a GameState Machine
	typedef struct
	{
		NGAMESTATE *pGameStatesList;

		NSTATEUID_ENUM CurrentGameStateUID;
		NGAMESTATE *pCurrentGameState;
		NGAMESTATE *pCurrentRootGameState;

		NGAMESTATE *pCurrentEventRootGameState;
		NGAMESTATE *pCurrentTimeRootGameState;
	} NGAMESTATEMACHINE;

	// ***************************************************************************************
	// **								 Functions											**
	// ***************************************************************************************
	void NBindGameStateEventDrivenHandle(const NSTATEUID_ENUM state, const STATEHANDLE_EVENTDRIVEN_FCT handle_fct);
	void NBindGameStateEndHandle(const NSTATEUID_ENUM state, const STATEEND_FCT handle_fct);
	void NBindGameStateStartHandle(const NSTATEUID_ENUM state, const STATESTART_FCT handle_fct);
	void NBindGameStateTimeDrivenHandle(const NSTATEUID_ENUM state, const STATEHANDLE_TIMEDRIVEN_FCT handle_fct);
	void NSetCurrentGameState(const NSTATEUID_ENUM state);
	void NSetGameStateParent(NSTATEUID_ENUM state, const NSTATEUID_ENUM parentstate);
	void NSetGameState_User_Nu32(const NSTATEUID_ENUM state, const Nu32 User_Nu32);

	void NInitializeGameStateMachine();
	void NDisableGameStateMachine();

	// void	NDispatchGameStateEvent(const NEVENT *pevent);

	// GameState Touch
	void NSetGameStateTouchCountMax(const NSTATEUID_ENUM state, const Nu8 touch_count_max);
	void NEnableGameStateCaptureTouchMoveReleasedByUI(const NSTATEUID_ENUM state);
	void NDisableGameStateCaptureTouchMoveReleasedByUI(const NSTATEUID_ENUM state);

	Nu32 NGetGameStateTouchCount(const NSTATEUID_ENUM state);
	Nu32 NGetGameStatePreviousTouchCount(const NSTATEUID_ENUM state);
	Nu32 NGetGameStateTouchCountIn(const NSTATEUID_ENUM state);
	Nu32 NGetGameStatePreviousTouchCountIn(const NSTATEUID_ENUM state);
	Nu32 NGetGameStateTouchCountOut(const NSTATEUID_ENUM state);
	Nu32 NGetGameStatePreviousTouchCountOut(const NSTATEUID_ENUM state);

	Nu32 NIsGameStateFirstTouchStarted(const NSTATEUID_ENUM state);
	Nu32 NIsGameStateLastTouchEnded(const NSTATEUID_ENUM state);
	Nu32 NIsGameStateFirstTouchMoveIn(const NSTATEUID_ENUM state);
	Nu32 NIsGameStateLastTouchMoveOut(const NSTATEUID_ENUM state);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NGAMESTATES_H

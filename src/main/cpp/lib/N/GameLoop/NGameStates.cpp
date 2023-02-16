#include "../NCStandard.h"
#include "../GameLoop/NGameStates.h"
#include "../GameLoop/NGameStatesUID.h"
#include "../Event/NEvent.h"
#include "../Touch/NTouch.h"
#include "../Touch/NTouchListener.h"

#include "../Core/NSafeConversion.h"
#include "../NErrorHandling.h"

/*
void Default_VoidGameStateChangeFunction()
{
	// nothing to do.
}
*/

// Globale
NGAMESTATEMACHINE	NGameStateMachine;

/*
static void NDefault_GameStateStart(const Nu32 user_Nu32){}
static void NDefault_GameStateEnd(const Nu32 user_Nu32){}
*/
/*
static void NNull_GameState_EventHandle(const NEVENT* pevent,const Nu32 User_Nu32){}
static void NNull_GameState_TimeHandle(const NTIME* ptime,const Nu32 User_Nu32){}
*/

void NInitializeGameStateMachine()
{
	Nmem0(&NGameStateMachine,NGAMESTATEMACHINE);
	// Allocation of all the game states at the same time ( and fill up them with ZEROs )
	NGameStateMachine.pGameStatesList = (NGAMESTATE*)Nmalloc_mem0(NSTATEUID_ENUM_SIZE*sizeof(NGAMESTATE));
	// Null members ( already setup to NULL by Nmem0 )
	// NGameStateMachine.pCurrentGameState			= NULL;
	// NGameStateMachine.pCurrentRootGameState		= NULL;
	// NGameStateMachine.pCurrentEventRootGameState = NULL;
	// NGameStateMachine.pCurrentTimeRootGameState	= NULL;
	// Setup non-null members
	NGameStateMachine.CurrentGameStateUID			= NSTATEUID_NULL;

	// By default, set all GameState Listener as capture touch start 
	for(Nu32 i=0;i<NSTATEUID_ENUM_SIZE;i++)
		FLAG_ON(NGameStateMachine.pGameStatesList[i].Listener.Flags, FLAG_NGAMESTATE_NTOUCH_LISTENER_CAPTURE_TOUCH_MOVE_RELEASED_BY_UI );
}

void NDisableGameStateMachine()
{
	Nfree(NGameStateMachine.pGameStatesList);
	
	// Null members
	NGameStateMachine.pGameStatesList			= NULL;
	NGameStateMachine.pCurrentGameState			= NULL;
	NGameStateMachine.pCurrentRootGameState		= NULL;
	NGameStateMachine.pCurrentEventRootGameState= NULL;
	NGameStateMachine.pCurrentTimeRootGameState	= NULL;

	// non-null members
	NGameStateMachine.CurrentGameStateUID		= NSTATEUID_NULL;
}

void NBindGameStateStartHandle(const NSTATEUID_ENUM state, const STATESTART_FCT handle_fct )
{
	NErrorIf(NGameStateMachine.pCurrentGameState,NERROR_GAMESTATE_MACHINE_ACTIVE);
	NErrorIf(!handle_fct , NERROR_NULL_POINTER);
	NErrorIf(state >= NSTATEUID_NULL, NERROR_GAMESTATE_UNAUTHORIZED_STATE_USE);
	NGameStateMachine.pGameStatesList[state].pGameStateStart = (STATESTART_FCT)handle_fct;
}
void NBindGameStateEndHandle(const NSTATEUID_ENUM state, const STATEEND_FCT handle_fct )
{
	NErrorIf(NGameStateMachine.pCurrentGameState,NERROR_GAMESTATE_MACHINE_ACTIVE);
	NErrorIf(!handle_fct , NERROR_NULL_POINTER);
	NErrorIf(state >= NSTATEUID_NULL, NERROR_GAMESTATE_UNAUTHORIZED_STATE_USE);
	NGameStateMachine.pGameStatesList[state].pGameStateEnd = (STATEEND_FCT)handle_fct;
}

void NBindGameStateEventDrivenHandle(const NSTATEUID_ENUM state, const STATEHANDLE_EVENTDRIVEN_FCT handle_fct )
{
	NErrorIf(NGameStateMachine.pCurrentGameState,NERROR_GAMESTATE_MACHINE_ACTIVE);
	NErrorIf(!handle_fct , NERROR_NULL_POINTER);
	NErrorIf(state >= NSTATEUID_NULL, NERROR_GAMESTATE_UNAUTHORIZED_STATE_USE);
	NGameStateMachine.pGameStatesList[state].pEventDrivenHandle = handle_fct;
}

void NBindGameStateTimeDrivenHandle(const NSTATEUID_ENUM state, const STATEHANDLE_TIMEDRIVEN_FCT handle_fct )
{
	NErrorIf(NGameStateMachine.pCurrentGameState,NERROR_GAMESTATE_MACHINE_ACTIVE);
	NErrorIf(!handle_fct , NERROR_NULL_POINTER);
	NErrorIf(state >= NSTATEUID_NULL, NERROR_GAMESTATE_UNAUTHORIZED_STATE_USE);
	NGameStateMachine.pGameStatesList[state].pTimeDrivenHandle = handle_fct;
}

void NSetGameState_User_Nu32( const NSTATEUID_ENUM state, const Nu32 User_Nu32 )
{
	NErrorIf(state >= NSTATEUID_NULL, NERROR_GAMESTATE_UNAUTHORIZED_STATE_USE);
	NGameStateMachine.pGameStatesList[state].User_Nu32 = (Nu32)User_Nu32;
}


void NSetGameStateParent( NSTATEUID_ENUM state, const NSTATEUID_ENUM parentstate )
{
	NErrorIf(NGameStateMachine.pCurrentGameState,NERROR_GAMESTATE_MACHINE_ACTIVE);
	NErrorIf(state >= NSTATEUID_NULL, NERROR_GAMESTATE_UNAUTHORIZED_STATE_USE);
	NErrorIf(parentstate > NSTATEUID_NULL, NERROR_GAMESTATE_UNAUTHORIZED_STATE_USE);
	NErrorIf(NGameStateMachine.pGameStatesList[parentstate].pParent == &NGameStateMachine.pGameStatesList[state], NERROR_GAMESTATE_CYCLIC_HIERARCHY);
	
	if(parentstate == NSTATEUID_NULL )
	{
		NGameStateMachine.pGameStatesList[state].pParent = NULL;
	}
	else
	{
		NGameStateMachine.pGameStatesList[state].pParent = &NGameStateMachine.pGameStatesList[parentstate];
	}
}

void NSetCurrentGameState( const NSTATEUID_ENUM new_state )
{
	NErrorIf(new_state > NSTATEUID_NULL, NERROR_GAMESTATE_UNAUTHORIZED_STATE_USE);

	// 1/ End the current GameState ( it will be the previous one just after)
	// 2/ Set the NEW current game State and ...
	// 3/ ... Start it !
	// ( No need of NULL tests because there are always Start and End bound functions ! ) 
	
	// Check incoming state:
	if(new_state==NGameStateMachine.CurrentGameStateUID)
	{
		NErrorIf(1, NERROR_GAMESTATE_STATE_ALREADY_ACTIVE);
		return;
	}

	// 1/	ASCENDING Look for intersection between "End Chain" AND "Start Chain"
	//		Start from incoming state and move up the hierarchy until cross a state already used by the current chain.
	//		During this process, look for the new "pCurrentRootGameState"
	NGAMESTATE	*proot		= NULL;
	NGAMESTATE	*pchild		= NULL;
	NGAMESTATE	*pintersect = NULL;
	NGAMESTATE	*pstate		= &NGameStateMachine.pGameStatesList[new_state];
	
	while( pstate )
	{
		if( pstate->pCurrentChild )
		{
			proot					= NGameStateMachine.pCurrentRootGameState;
			pintersect				= pstate;
			pstate->pCurrentChild	= pchild;
			break;
		}
		else
		{
			proot = pstate;
			pstate->pCurrentChild = pchild;
		}
		pchild = pstate;
		pstate = pstate->pParent;
	}
	NGameStateMachine.pCurrentRootGameState = proot;

	// 2/	ASCENDING End the Current State Chain from the current state to the "Intersection" (excluded)
	//		If "Intersection" is NULL, it works too ...
	
	NEVENT	nevent;
	
	//NFillupEventHead(nevent,NTOUCH_CANCEL,0,NEVENT_RECIPIENT_CAST_GAMESTATE,pstate);
	nevent.Head.Type						= NTOUCH_CANCEL;
	nevent.Head.Param						= NEVENT_PARAM_TOUCH_CANCEL__ALL;			
	nevent.Head.RecipientCast				= NEVENT_RECIPIENT_CAST_GAMESTATE;
	nevent.Touch.TouchSequence.TouchCount	= 0;

	pstate = NGameStateMachine.pCurrentGameState;
	while( pstate != pintersect )
	{
		// Canceling potential listened touches ?
		if( pstate->Listener.LTouchCount )
		{
			NErrorIf(!pstate->pEventDrivenHandle, NERROR_SYSTEM_CHECK);
			// Not appropriate here : NErrorIf( pstate->Listener.LTouchCount != pstate->Listener.LTouchBufferSize, NERROR_SYSTEM_CHECK);
			// ---> Because			: "pstate->Listener.LTouchCount != pstate->Listener.LTouchBufferSize" is a possible case here !!!
			//						  User can call "NSetCurrentGameState" from "this_state->pEventDrivenHandle" as a response
			//						  to "NTOUCH_CANCEL" or "NTOUCH_END" event ...
			//						  ... So in that case, some listened touches have their phase equal to _END or _CANCEL ...
			pstate->Listener.PreviousLTouchCount = pstate->Listener.LTouchCount;
			pstate->Listener.LTouchCount = 0;
			FLAG_ON(pstate->Listener.Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);
			nevent.Head.pRecipient = pstate;
// SEND EVENT
// ------------------------------------------------------------------------------------------------
			pstate->pEventDrivenHandle(&nevent,pstate->User_Nu32);
// ------------------------------------------------------------------------------------------------
// SEND EVENT
			// Clean Up!
			NCleanTouchListenerUp(&(pstate)->Listener);
		}

		if(pstate->pGameStateEnd)
			pstate->pGameStateEnd(new_state,pstate->User_Nu32);
		
		pstate->pCurrentChild		= NULL;
		pstate->pCurrentEventChild	= NULL;
		pstate->pCurrentTimeChild	= NULL;
		
		pstate = pstate->pParent;
	}

	// 3/ DESCENDING Start the new State Chain from the new Root or from the "Intersection" (excluded), to the new Current State.
	if(pintersect)
	{
		pstate = pintersect->pCurrentChild;
	}
	else
	{
		pstate = NGameStateMachine.pCurrentRootGameState;
	}

	while( pstate )
	{
		if(pstate->pGameStateStart)
			pstate->pGameStateStart(NGameStateMachine.CurrentGameStateUID, pstate->User_Nu32);
		
		pstate = pstate->pCurrentChild;
	}
	// 4/	KEEP Current state ID and Pointer on it.
	//		Prefer having NULL pointer for "NSTATEUID_NULL" id instead of a pointer on this element ( it exists at the End of the list ! ) 
	NGameStateMachine.CurrentGameStateUID	= new_state;
	if( new_state == NSTATEUID_NULL)
	{
		NGameStateMachine.pCurrentGameState = NULL;
	}
	else
	{
		NGameStateMachine.pCurrentGameState	= &NGameStateMachine.pGameStatesList[new_state];
	}

	// 5/ Build specific and Optimized Event and Time Handling Descending Chains
	NGAMESTATE *peventbase	= NULL;
	NGAMESTATE *ptimebase	= NULL;
	NGameStateMachine.pCurrentEventRootGameState= NULL;
	NGameStateMachine.pCurrentTimeRootGameState	= NULL;
	pstate = NGameStateMachine.pCurrentRootGameState;
	while(pstate)
	{
		// Reset
		pstate->pCurrentEventChild	= NULL;
		pstate->pCurrentTimeChild	= NULL;

		// Optimize EventDriven Update Chain.
		if( pstate->pEventDrivenHandle )
		{
			if(!peventbase)	// Root ...
			{
				NGameStateMachine.pCurrentEventRootGameState = peventbase = pstate;
			}
			else		// or Not Root
			{
				peventbase = peventbase->pCurrentEventChild = pstate;
			}
		}
		// Optimize TimeDriven Update Chain.
		if( pstate->pTimeDrivenHandle )
		{
			if(!ptimebase)	// Root ...
			{
				NGameStateMachine.pCurrentTimeRootGameState = ptimebase = pstate;
			}
			else			// or Not Root
			{
				ptimebase = ptimebase->pCurrentTimeChild = pstate;
			}
		}

		pstate = pstate->pCurrentChild;
	}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Debug test to check GameStates Machine Integrity
#ifdef _DEBUG
	// Check each game state of "NGameStateMachine.pGameStatesList" against current updating game state chains 
	Nu32		dbg_check;
	NGAMESTATE	*dbg_pstate = NGameStateMachine.pGameStatesList;
	for(Nu32 dbg_i=0;dbg_i<NSTATEUID_ENUM_SIZE;dbg_i++,dbg_pstate++)
	{
		// 0/ Init
		dbg_check = 0;

		// 1/ ROBUST CHECK: Check the chain from CurrentGameState
		pstate = NGameStateMachine.pCurrentGameState;
		while(pstate)
		{
			// Check ! 'pstate' a state of the current "from child to root" chain, is IN the "NGameStateMachine.pGameStatesList"
			if(dbg_pstate == pstate) 
			{
				dbg_check += 1;
				break;
			}
			pstate = pstate->pParent;
		}

		// 2/ WEAKPOINT#1: Check the chain from CurrentRootGameState
		pstate = NGameStateMachine.pCurrentRootGameState;
		while(pstate)
		{
			if(dbg_pstate == pstate)
			{
				dbg_check += 1;
				break;
			}
			pstate = pstate->pCurrentChild;
		}
		// "dbg_check" value should be:
		//		= 0, that means "dbg_pstate" seems not to be in the current chain.
		//		= 2, that means "dbg_pstate" seems to be in the current chain.
		// There are inconsistencies if
		//		= 1, that means the 2 check tests find 2 different responses ... it's BAD !
		NErrorIf(dbg_check==1,NERROR_INCONSISTENT_VALUES);

		// 3/ WEAKPOINT#2: Check the Event chain from CurrentRootGameState
		pstate = NGameStateMachine.pCurrentEventRootGameState;
		while(pstate)
		{
			if(dbg_pstate == pstate)
			{
				NErrorIf(!pstate->pEventDrivenHandle, NERROR_INCONSISTENT_VALUES);
				NErrorIf(dbg_check != 2, NERROR_INCONSISTENT_VALUES);
				break;
			}
			pstate = pstate->pCurrentEventChild;
		}
		// 4/ WEAKPOINT#3: Check the Time chain from CurrentRootGameState
		pstate = NGameStateMachine.pCurrentTimeRootGameState;
		while(pstate)
		{
			if(dbg_pstate == pstate)
			{
				NErrorIf(!pstate->pTimeDrivenHandle, NERROR_INCONSISTENT_VALUES);
				NErrorIf(dbg_check != 2, NERROR_INCONSISTENT_VALUES);
				break;
			}
			pstate = pstate->pCurrentTimeChild;
		}

		// 5/ WEAKPOINT#3: Check non-included Game State consistency
		if( dbg_check == 0 )
		{
			NErrorIf(dbg_pstate->pCurrentChild,NERROR_INCONSISTENT_VALUES);
			NErrorIf(dbg_pstate->pCurrentEventChild,NERROR_INCONSISTENT_VALUES);
			NErrorIf(dbg_pstate->pCurrentTimeChild,NERROR_INCONSISTENT_VALUES);

			if(dbg_pstate->pParent)
			{
				NErrorIf(dbg_pstate->pParent->pCurrentChild			== dbg_pstate,NERROR_INCONSISTENT_VALUES);
				NErrorIf(dbg_pstate->pParent->pCurrentEventChild	== dbg_pstate,NERROR_INCONSISTENT_VALUES);
				NErrorIf(dbg_pstate->pParent->pCurrentTimeChild		== dbg_pstate,NERROR_INCONSISTENT_VALUES);
			}
		}

		// 6/ WEAKPOINT#4: Check Event_handle and Touch Count Max consistency
		// In one hand state doesn't have any Handle to manage Event ... and in other hand it has a Touch Count Max which is not ZERO !
		// It's Weird !
		NErrorIf( dbg_pstate->Listener.LTouchCountMax && (!dbg_pstate->pEventDrivenHandle), NERROR_INCONSISTENT_VALUES );

	}
#endif
}

/*
void NDispatchGameStateEvent(const NEVENT *pevent)
{
	NErrorIf(!pevent,NERROR_NULL_POINTER);
	// Check Event Main type before dispatch to be sure incoming Event is compatible with GameState
	NErrorIf( (NGET_EVENT_MAIN(pevent->Type) != _TOUCH) && (NGET_EVENT_MAIN(pevent->Type) != _SYSTEM), NERROR_EVENT_DISPATCH_UNAUTHORIZED );
	//List of authorized Event (2017/03/21): 
	// All System and All Touch Events... that means:
	// SYSTEM
	//	NSYS_NOEVENT		
	//	NSYS_QUIT			
	//	NSYS_RETURN_BACK deprecated replaced by NKEY_DOW_BACK
	// 	
	// TOUCH
	//  NTOUCH_START	
	//  NTOUCH_MOVE	
	//  NTOUCH_END		
	//  NTOUCH_CANCEL	

	NGAMESTATE *pcurrentstate = NGameStateMachine.pCurrentEventRootGameState;
	while( pcurrentstate )
	{
		pcurrentstate->pEventDrivenHandle(&pevent,pcurrentstate->User_Nu32);
		pcurrentstate = pcurrentstate->pCurrentEventChild;
	}
}
*/

#define _GSLISTENER(state)	( NGameStateMachine.pGameStatesList[state].Listener )

void	NEnableGameStateCaptureTouchMoveReleasedByUI( const NSTATEUID_ENUM state )
{
	NErrorIf(_GSLISTENER(state).LTouchBufferSize, NERROR_UNAUTHORIZED_ACTION );
	FLAG_ON(_GSLISTENER(state).Flags, FLAG_NGAMESTATE_NTOUCH_LISTENER_CAPTURE_TOUCH_MOVE_RELEASED_BY_UI );
}

void	NDisableGameStateCaptureTouchMoveReleasedByUI( const NSTATEUID_ENUM state )
{
	NErrorIf(_GSLISTENER(state).LTouchBufferSize, NERROR_UNAUTHORIZED_ACTION );
	FLAG_OFF(_GSLISTENER(state).Flags, FLAG_NGAMESTATE_NTOUCH_LISTENER_CAPTURE_TOUCH_MOVE_RELEASED_BY_UI );
}

void NSetGameStateTouchCountMax( const NSTATEUID_ENUM state, const Nu8 touch_count_max )
{
	NErrorIf(state >= NSTATEUID_NULL, NERROR_GAMESTATE_UNAUTHORIZED_STATE_USE);
	NSetTouchListenerTouchCountMax(&_GSLISTENER(state), touch_count_max);
}

Nu32 NGetGameStateTouchCount( const NSTATEUID_ENUM state )
{
	NErrorIf(state >= NSTATEUID_NULL, NERROR_GAMESTATE_UNAUTHORIZED_STATE_USE);
	// return NGetListenerTouchCount(&_GSLISTENER(state));
	// ... quicker in this way ...
	return _GSLISTENER(state).LTouchCount;
}
Nu32 NGetGameStatePreviousTouchCount( const NSTATEUID_ENUM state )
{
	NErrorIf(state >= NSTATEUID_NULL, NERROR_GAMESTATE_UNAUTHORIZED_STATE_USE);
	// return NGetListenerPreviousTouchCount(&_GSLISTENER(state));
	// ... quicker in this way ...
	return _GSLISTENER(state).PreviousLTouchCount;
}
Nu32 NGetGameStateTouchCountIn( const NSTATEUID_ENUM state )
{
	NErrorIf(state >= NSTATEUID_NULL, NERROR_GAMESTATE_UNAUTHORIZED_STATE_USE);
	// return NGetListenerTouchCountIn(&_GSLISTENER(state));
	// ... quicker in this way ...
	NErrorIf( _GSLISTENER(state).LTouchCountIn != _GSLISTENER(state).LTouchCount, NERROR_INCONSISTENT_VALUES);
	return (_GSLISTENER(state).LTouchCountIn );
}
Nu32 NGetGameStatePreviousTouchCountIn( const NSTATEUID_ENUM state )
{
	NErrorIf(state >= NSTATEUID_NULL, NERROR_GAMESTATE_UNAUTHORIZED_STATE_USE);
	// return NGetListenerPreviousTouchCountIn(&_GSLISTENER(state));
	// ... quicker in this way ...
	NErrorIf( _GSLISTENER(state).PreviousLTouchCountIn != _GSLISTENER(state).PreviousLTouchCount, NERROR_INCONSISTENT_VALUES );
	return (_GSLISTENER(state).PreviousLTouchCountIn );
}
Nu32 NGetGameStateTouchCountOut( const NSTATEUID_ENUM state )
{
	NErrorIf(state >= NSTATEUID_NULL, NERROR_GAMESTATE_UNAUTHORIZED_STATE_USE);
	// return NGetListenerTouchCountOut(&NGameStateMachine.pGameStatesList[state].Listener);
	// ... quicker in this way ...
	// !!! Especially with "LTouchCountOut" which is always ZERO for a GameState !
	NErrorIf( _GSLISTENER(state).LTouchCountIn != _GSLISTENER(state).LTouchCount, NERROR_INCONSISTENT_VALUES);
	return 0;
}
Nu32 NGetGameStatePreviousTouchCountOut( const NSTATEUID_ENUM state )
{
	NErrorIf(state >= NSTATEUID_NULL, NERROR_GAMESTATE_UNAUTHORIZED_STATE_USE);
	// return NGetListenerPreviousTouchCountOut(&NGameStateMachine.pGameStatesList[state].Listener);
	// ... quicker in this way ...
	// !!! Especially with "PreviousLTouchCountOut" which is always ZERO for a GameState !
	NErrorIf( _GSLISTENER(state).PreviousLTouchCountIn != _GSLISTENER(state).PreviousLTouchCount, NERROR_INCONSISTENT_VALUES );
	return 0;
}

Nu32 NIsGameStateFirstTouchStarted( const NSTATEUID_ENUM state )
{
	NErrorIf(state >= NSTATEUID_NULL, NERROR_GAMESTATE_UNAUTHORIZED_STATE_USE);
	// return NIsListenerFirstTouchStarted(&NGameStateMachine.pGameStatesList[state].Listener);
	// ... quicker in this way ...
	NErrorIf( _GSLISTENER(state).LTouchCountIn != _GSLISTENER(state).LTouchCount, NERROR_INCONSISTENT_VALUES);
	NErrorIf( _GSLISTENER(state).PreviousLTouchCountIn != _GSLISTENER(state).PreviousLTouchCount, NERROR_INCONSISTENT_VALUES );

	return ( ( _GSLISTENER(state).LTouchCount && (!_GSLISTENER(state).PreviousLTouchCount) ) ? 1:0 );
}

Nu32 NIsGameStateLastTouchEnded(const NSTATEUID_ENUM state )
{
	NErrorIf(state >= NSTATEUID_NULL, NERROR_GAMESTATE_UNAUTHORIZED_STATE_USE);
	// return NIsListenerLastTouchEnded(&NGameStateMachine.pGameStatesList[state].Listener);
	// ... quicker in this way ...
	NErrorIf( _GSLISTENER(state).LTouchCountIn != _GSLISTENER(state).LTouchCount, NERROR_INCONSISTENT_VALUES);
	NErrorIf( _GSLISTENER(state).PreviousLTouchCountIn != _GSLISTENER(state).PreviousLTouchCount, NERROR_INCONSISTENT_VALUES );

	return( ( (!_GSLISTENER(state).LTouchCount) && _GSLISTENER(state).PreviousLTouchCount ) ? 1:0 );
}

Nu32 NIsGameStateFirstTouchMoveIn(const NSTATEUID_ENUM state)
{
	NErrorIf(state >= NSTATEUID_NULL, NERROR_GAMESTATE_UNAUTHORIZED_STATE_USE);
	// return NIsListenerFirstTouchMoveIn(&NGameStateMachine.pGameStatesList[state].Listener);
	// ... quicker in this way ...
	NErrorIf( _GSLISTENER(state).LTouchCountIn != _GSLISTENER(state).LTouchCount, NERROR_INCONSISTENT_VALUES);
	NErrorIf( _GSLISTENER(state).PreviousLTouchCountIn != _GSLISTENER(state).PreviousLTouchCount, NERROR_INCONSISTENT_VALUES );

	return( ( (_GSLISTENER(state).LTouchCount)&&(!(_GSLISTENER(state).PreviousLTouchCount)) ) ? 1:0 );
}

Nu32 NIsGameStateLastTouchMoveOut(const NSTATEUID_ENUM state)
{
	NErrorIf(state >= NSTATEUID_NULL, NERROR_GAMESTATE_UNAUTHORIZED_STATE_USE);
	// return NIsListenerLastTouchMoveOut(&NGameStateMachine.pGameStatesList[state].Listener);
	// ... quicker in this way ...
	NErrorIf( _GSLISTENER(state).LTouchCountIn != _GSLISTENER(state).LTouchCount, NERROR_INCONSISTENT_VALUES);
	NErrorIf( _GSLISTENER(state).PreviousLTouchCountIn != _GSLISTENER(state).PreviousLTouchCount, NERROR_INCONSISTENT_VALUES );

	return( ((!(_GSLISTENER(state).LTouchCount))&&(_GSLISTENER(state).PreviousLTouchCount) ) ? 1:0 );
}


#include "../NCStandard.h"
#include "../NType.h"
#include "NUI.h"

NUI_EVENT_RESULT	NDefaultUIEvent_Handle(NUI *pui, const NEVENT *pevent)
{
/*
	if( NEVENT_IS_PRIVATE(pevent->Type) )
	{
		return NUI_EVENT_RESULT_NONPROCESSED_PRIVATE;
	}
	else
	{
		return NUI_EVENT_RESULT_NONPROCESSED_PUBLIC;
	}
*/
	return NUI_EVENT_RESULT_DISPATCH;
}

/*

NUI_EVENT_RESULT	NDefaultUIEvent_Proc(NUI *pui, const NEVENT *pevent)
{
	NUI_EVENT_RESULT	returnvalue = 	NUI_RESULT_DEFAULT;

	// Public or Private Incoming NEVENT ?
	// This function ignore Private NEVENT 
	// It only takes care of PUBLIC NEVENT handling and their Hierarchical diffusion
	if( NEVENT_ISPRIVATE(pevent->Type) )
		return NUI_RESULT_DEFAULT;

	// There is a child with FOCUS
	// So it will receive the incoming event
	if( pui->pChildWithFocus )
	{
		if( pevent->Type == NTOUCH_MOVE)
			_update_uiframe_undertouch_status(pui->pChildWithFocus,pevent);
		
		pui->pChildWithFocus->UIEventToState_Proc(pui->pChildWithFocus,pevent);
		returnvalue = pui->pChildWithFocus->Event_Proc(pui->pChildWithFocus,pevent);
/ *
		// Focused UI
		pui = pFocusedUI;
		pui->UIEventToState_Proc(pui,pevent);
		pui->Event_Proc(pui,pevent);
		
		// Propagation
		while( ISFLAG_OFF(pui->Flags,FLAG_NUI_STOP_EVENT_TO_PARENT_PROPAGATION) )
		{
			pui = (NUI*)pui->pParent;
			NErrorIf(!pui, NERROR_NULL_POINTER); // That means an UI Root was not flagged with FLAG_NUI_STOP_EVENT_TO_PARENT_PROPAGATION... it's a BUG 
			
			pui->UIEventToState_Proc(pui,pevent);
			pui->Event_Proc(pui,pevent);
		}
* /

		// Focus management
		if( pevent->Type == NTOUCH_END || pevent->Type == N_FOCUS_KILLED )
		{
			FLAG_OFF( pui->pChildWithFocus->Flags,FLAG_NUI_FOCUS );

			// Under Touch control
			if( ISFLAG_ON(pui->pChildWithFocus->Flags,FLAG_NUI_UNDER_TOUCH_CONTROL) )
			{
				FLAG_OFF( pui->pChildWithFocus->Flags,FLAG_NUI_IS_UNDER_TOUCH );
				FLAG_OFF( pui->pChildWithFocus->Flags,FLAG_NUI_WAS_UNDER_TOUCH );
			}

			if(ISFLAG_ON(pui->pChildWithFocus->Flags,FLAG_NUI_COLOR_UPDATE))
				_ApplyFocusColors(pui->pChildWithFocus,NFALSE);
			
			_DecreaseFocusCount();
			pui->pChildWithFocus = NULL;
		}
		return returnvalue;
	}
	else if(pevent->Type == NTOUCH_START)
	{
		// There is no CHILD UI with focus ... So, No messages to dispatch !
		// BUT !!! If the incoming message is NTOUCH_START we are looking for a new focused child UI
		// The whole list of UI children is parsed to look for a new focused UI
		// ... and if we find one we are going send it the NTOUCH_START message and keep it as ChildWithFocus to send it all the
		// future incoming messages !
		pui->pChildWithFocus = NUISearchFocus(&pui->ChildrenList,pevent->Touch.Position.x,pevent->Touch.Position.y);
		if(pui->pChildWithFocus)
		{
			// Focus management
			FLAG_ON(pui->pChildWithFocus->Flags,FLAG_NUI_FOCUS);
			
			// Under Touch control
			if( ISFLAG_ON(pui->pChildWithFocus->Flags,FLAG_NUI_UNDER_TOUCH_CONTROL) )
			{
				FLAG_ON(pui->pChildWithFocus->Flags,FLAG_NUI_IS_UNDER_TOUCH);
				FLAG_ON( pui->pChildWithFocus->Flags,FLAG_NUI_WAS_UNDER_TOUCH );
			}
			if(ISFLAG_ON(pui->pChildWithFocus->Flags,FLAG_NUI_COLOR_UPDATE))
				_ApplyFocusColors(pui->pChildWithFocus,NTRUE);
			
		//	if( ISFLAG_OFF(pui->pChildWithFocus->Flags,FLAG_NUI_NON_BLOCKING_GAMESTATE) )
				_IncreaseFocusCount();
			pui->pChildWithFocus->UIEventToState_Proc(pui->pChildWithFocus,pevent);
			return pui->pChildWithFocus->Event_Proc(pui->pChildWithFocus,pevent);
		}
	}

	return NUI_RESULT_DEFAULT;
}

*/

#include "../NCStandard.h"
#include "../UI/NUIPush.h"
#include "../UI/NUIPushButton.h"

void NUIPushButton_EventToState_Handle(NUI *pui, const NEVENT *pevent) 
{
	//	Check the recipient to be sure This UI is the recipient of the Event !!!
	//	BECAUSE it could be intercepting a child Event (which is weird for a PushButton but anyway ...) 
	//  in that case function returns immediately ...
	if( (NUI*)pevent->Head.pRecipient != pui )
		return;

	// First, call the EventToState 'Push' function.
	// It will manage 'ALONE' the UI "State". 
	// Actually, purpose of 'NUICustom_EventToState_Handle' is just sending appropriate Direct EVENT according 
	// with push KEY INSTANTs.
	NEVENT		nevent;
	
	switch( NGET_UIPUSH_PREHANDLE_KEY_INSTANT( NUIPush_EventToState_PreHandle(pui,pevent) ) )
	{
		case NUI_KEY_INSTANT_PUSH_DOWN_CANCEL: // When the PUSH UI Leave "Down Wait position" due to a Cancel decision ( = Touch Move outside the UI, after this one was resting into dow wait position)
			printf("\n NUI_KEY_INSTANT_PUSH_DOWN_CANCEL");
			// Send CANCEL Command Event ...
			if( ISFLAG_ON(pui->Flags,FLAG_NUI_PUSHBUTTONCOMMAND_DOWN) )
			{
				memset(&nevent,0,sizeof(NEVENT));
				NFillupEventHead(nevent,NCOMMAND_PUSHBUTTON_CANCEL_DOWN,0,NEVENT_RECIPIENT_CAST_UI,pui);
				NSendUICommandEvent(&nevent); 
			}
			break;

		case NUI_KEY_INSTANT_PUSH_UP: // When the PUSH UI reach "Up position"
			printf("\n NUI_KEY_INSTANT_PUSH_UP");
			// Send Command Event ...
			if( ISFLAG_ON(pui->Flags,FLAG_NUI_PUSHBUTTONCOMMAND_UP) )
			{
				memset(&nevent,0,sizeof(NEVENT));
				NFillupEventHead(nevent,NCOMMAND_PUSHBUTTON_UP,0,NEVENT_RECIPIENT_CAST_UI,pui);
				NSendUICommandEvent(&nevent); 
			}
			break;

		case NUI_KEY_INSTANT_PUSH_DOWN: // When the PUSH UI reach "Down position"
			printf("\n NUI_KEY_INSTANT_PUSH_DOWN");
			// Send Command Event ...
			if( ISFLAG_ON(pui->Flags,FLAG_NUI_PUSHBUTTONCOMMAND_DOWN) )
			{
				memset(&nevent,0,sizeof(NEVENT));
				NFillupEventHead(nevent,NCOMMAND_PUSHBUTTON_DOWN,0,NEVENT_RECIPIENT_CAST_UI,pui);
				NSendUICommandEvent(&nevent); 
			}
			break;

		case NUI_KEY_INSTANT_PUSH_RESET:
			// Nothing to do yet ...
			break;

		default:
			break;
	}
}

static void NUIPushButton_States_Handle(NUI *pui, const NTIME	*ptime)
{
	NEVENT		nevent;

	switch( NGET_UIPUSH_PREHANDLE_KEY_INSTANT( NUIPush_States_PreHandle(pui,ptime) ) )
	{
		// There are only 2 possible DELAYED Key Instants:
		// ( NUI_KEY_INSTANT_PUSH_DOWN_CANCEL is never Delayed ! )
		// Handle DELAYED Key Instant PUSH UP
		case NUI_KEY_INSTANT_PUSH_UP:
			printf("\n Delayed NUI_KEY_INSTANT_PUSH_UP");
			// Send Command Event ...
			if( ISFLAG_ON(pui->Flags,FLAG_NUI_PUSHBUTTONCOMMAND_UP) )
			{
				memset(&nevent,0,sizeof(NEVENT));
				NFillupEventHead(nevent,NCOMMAND_PUSHBUTTON_UP,0,NEVENT_RECIPIENT_CAST_UI,pui);
				NPostDelayedUIEvent(&nevent); 
			}
			break;

		// Handle DELAYED Key Instant PUSH DOWN
		case NUI_KEY_INSTANT_PUSH_DOWN:
			printf("\n NUI_KEY_INSTANT_PUSH_DOWN");
			// Send Command Event ...
			if( ISFLAG_ON(pui->Flags,FLAG_NUI_PUSHBUTTONCOMMAND_DOWN) )
			{
				memset(&nevent,0,sizeof(NEVENT));
				NFillupEventHead(nevent,NCOMMAND_PUSHBUTTON_DOWN,0,NEVENT_RECIPIENT_CAST_UI,pui);
				NPostDelayedUIEvent(&nevent); 
			}
			break;

		default:
			break;
	}
}

NUI_PUSHBUTTON*	NCreateUIPushButton(NUI *parent, NUI_PUSHBUTTON_DESC *pbuttondesc, const Nu32 user32)
{
	return NSetupUIPushButton(parent,NEW(NUI_PUSHBUTTON),pbuttondesc,user32);
}

NUI_PUSHBUTTON*	NSetupUIPushButton(NUI *parent, NUI_PUSHBUTTON *pbutton, NUI_PUSHBUTTON_DESC *pbuttondesc, const Nu32 user32)
{
	NPushUISetUpProcess( (NUI*)pbutton, (NUI_DESC*)pbuttondesc );
	
	// Force TransformHNode Creation !
	FLAG_ON(pbuttondesc->UIDesc.Flags_Core,FLAG_NUIDC_UI_TRANSFORMHNODE);

	// Call Parent Structure Setup (here, parent structure is NUI)
#ifdef _DEBUG
	if( ! NSetupUI(parent,(NUI*)pbutton,(NUI_DESC*)pbuttondesc,user32) )
	{
		// NPopUISetUpProcess triggered EVENT sending from NSetupUI but it's a bug !
		// And unfortunately, it seem's user delete the UI during event  processing (NUI_SETUP or/and NUI_ENABLE) ...
		// This should never happen ! 
		// (Maybe it's not exactly that but certainly something like that to have a NULL pointer returned by NSetupUI)
		NErrorIf(1,NERROR_UI_SETUPPROCESS_STACK_CORRUPTED); 
	}
#else
	NSetupUI(parent,(NUI*)pbutton,(NUI_DESC*)pbuttondesc,user32);
#endif		
	
	// Re-Setup System data & functions ( could be overwritten )
	pbutton->UI.CurrentState				= NUI_STATE_PUSH_UP_REST; 
	pbutton->UI.UIState_Proc				= NUIPushButton_States_Handle;
	pbutton->UI.UIEventToState_Proc			= NUIPushButton_EventToState_Handle;
	pbutton->UI.ClearUI_Proc				= NClearUIPushButton;
	
	// Specific Default Touch Mode ( different than Simple UI )
	if( ISFLAG_OFF(pbuttondesc->UIDesc.Flags_Style, FLAG_NUIDS_USEPARAM_LISTEN_TOUCH_MODE) )
		NSetUIListenTouchMode((NUI*)pbutton, NUI_LISTEN_TOUCH_MODE_BASIC_TRACK);
	
	// b)Check one flag by one flag, and set it to ON if it's requested and to OFF if its not
	if( ISFLAG_ON(pbuttondesc->UIDesc.Flags_Core,FLAG_NUIDC_PUSHBUTTONCOMMAND_UP) )
		FLAG_ON(pbutton->UI.Flags,FLAG_NUI_PUSHBUTTONCOMMAND_UP);
	else
		FLAG_OFF(pbutton->UI.Flags,FLAG_NUI_PUSHBUTTONCOMMAND_UP);

	if( ISFLAG_ON(pbuttondesc->UIDesc.Flags_Core,FLAG_NUIDC_PUSHBUTTONCOMMAND_DOWN) )
		FLAG_ON(pbutton->UI.Flags,FLAG_NUI_PUSHBUTTONCOMMAND_DOWN);
	else
		FLAG_OFF(pbutton->UI.Flags,FLAG_NUI_PUSHBUTTONCOMMAND_DOWN);

	
	return (NUI_PUSHBUTTON*)NPopUISetUpProcess( (NUI*)pbutton, (NUI_DESC*)pbuttondesc );
}

void	NClearUIPushButton(NUI *pui)
{
	// a)Clear Pushbutton Section
	// ... nothing yet !	
	// b)Clear UI Section
	NClearUI_Basics(pui);
}



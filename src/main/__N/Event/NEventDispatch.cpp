#include "../NCStandard.h"
#include "../NType.h"
#include "../NCoordinateSystemConversions.h"
#include "../UI/NUI.h"
#include "../Containers/NRingBuffer.h"
#include "../Touch/NTouchStream.h"
#include "../GameLoop/NGameStates.h"
#include "../NCore.h"
#include "NEvent.h"

extern NGAMESTATEMACHINE	NGameStateMachine;				// THE 'NGameState' machine


#ifdef _DEBUG
extern	NARRAY 	ActiveUIRootsList;				// Array with all the active UI. The one which can be modified during the loop.

static void _Recursive_ErrorOnUIListenTouchEventFLagsInconsistency(NUI *pui)
{
	// To check UI Listen Touch Flags consistency ...
	// By using "NSetUIListenTouchMode" there is no problem, 
	// but using:  
	//		NEnableUIListenTouch_CaptureMoveIn	/ NDisableUIListenTouch_CaptureMoveIn
	//		NEnableUIListenTouch_ReleaseMoveOut / NDisableUIListenTouch_ReleaseMoveOut
	//		NEnableUIListenTouch_TrackMoveInOut	/ NDisableUIListenTouch_TrackMoveInOut
	//
	// ... may cause inconsistencies ... These checks verify UI Flags consistency ...
	
	// First, check all it's children
	NUI *puichild = (NUI*)pui->ChildrenList.pFirst;
	while( NIS_NODE_VALID(puichild,&pui->ChildrenList) )
	{
		_Recursive_ErrorOnUIListenTouchEventFLagsInconsistency(puichild);
		puichild = (NUI*)NGET_NEXT_NODE(puichild);
	}

	// Then check UI ...
	NErrorIf( FLAGS_TEST((pui)->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );																					
	NErrorIf( FLAGS_TEST((pui)->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT ), NERROR_INCONSISTENT_FLAGS );																							
	NErrorIf( FLAGS_TEST((pui)->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );												
	NErrorIf( FLAGS_TEST((pui)->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );	
	NErrorIf( FLAGS_TEST((pui)->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );	
	NErrorIf( FLAGS_TEST((pui)->Flags, MASK_NUI_LISTEN_TOUCH_EVENT, FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_START|FLAG_NUI_LISTEN_TOUCH_EVENT_CAPTURE_MOVE_IN|FLAG_NUI_LISTEN_TOUCH_EVENT_RELEASE_MOVE_OUT|FLAG_NUI_LISTEN_TOUCH_EVENT_TRACK_MOVE_IN_OUT ), NERROR_INCONSISTENT_FLAGS );
	
	// !
	// ! In case of a CRASH HERE that means, somewhere in the code, FLAG_NUI_LISTEN_TOUCH_xxx are modified one by one with one of these function:
	//		NEnableUIListenTouch_CaptureMoveIn	/ NDisableUIListenTouch_CaptureMoveIn
	//		NEnableUIListenTouch_ReleaseMoveOut / NDisableUIListenTouch_ReleaseMoveOut
	//		NEnableUIListenTouch_TrackMoveInOut	/ NDisableUIListenTouch_TrackMoveInOut
	// ... or directly by hand ! 
	// AND, at the end of the day the FLAG_NUI_LISTEN_TOUCH_xxx combination of the UI still unsupported.
	// Check "FLAGS_NUI_TOUCH_MODE ( FLAGS compositions Table )" into "NUI.h".

}
#endif



void _dispatch_event_to_gamestates(const NEVENT *pevent)
{
	NGAMESTATE *pcurrentstate = NGameStateMachine.pCurrentEventRootGameState;
	while( pcurrentstate )
	{
		((NEVENT*)pevent)->Head.pRecipient = pcurrentstate;
		((NEVENT*)pevent)->Head.RecipientCast = NEVENT_RECIPIENT_CAST_GAMESTATE;
		pcurrentstate->pEventDrivenHandle(pevent,pcurrentstate->User_Nu32);
		pcurrentstate = pcurrentstate->pCurrentEventChild;
	}
}
void NDispatchEvent( const NEVENT *pevent )
{

#ifdef _DEBUG
	// To check UI Listen Touch Flags consistency ...
	NUI **dbg_ppui= (NUI**)ActiveUIRootsList.pFirst;
	for(Nu32 dbg_i=ActiveUIRootsList.Size;dbg_i!=0;dbg_i--, dbg_ppui ++)
	{
		_Recursive_ErrorOnUIListenTouchEventFLagsInconsistency(*dbg_ppui);
	}
#endif


	NTOUCH_SEQUENCE		touch_sequence;
	//NGAMESTATE			*pcurrentstate;
	
	#ifdef _NOUT_SYS_EVENT
	printf("\n> Dispatch of Event Process Beginning. ");
	#endif
/*

	// UI & GC DISPATCH
	switch(pevent->Type)
	{
		// ----------------------------------------------------------------------------------------
		// -
		// - _SYSTEM
		// -
		// System Events are all specifics. 
		// Some of them may generate first TOUCH CANCEL ALL event dispatch (to UI and GameState) --> NSYS_QUIT
		// Some of them may be dispatch only to game state hierarchy ------------------------------> NSYS_RETURN_BACK deprecated replaced by NKEY_DOW_BACK
		// Some other ... To Be Defined ...
		//
		case NSYS_QUIT:
			#ifdef _NOUT_SYS_EVENT
			printf("\n> NDispatch NSYS_QUIT.");
			printf("\n\t> -As NTOUCH_CANCEL (ALL) to UIs.");
			#endif
			// Generate a TOUCH CANCEL ALL dispatch to 'KILL' all current Touch Listening
			// UI and Game State are going to receive it ...
			touch_sequence.TouchCount = 0;
			NDispatchTouch_Cancel(&touch_sequence);
			#ifdef _NOUT_SYS_EVENT
			printf("\n\t> -As NSYS_QUIT to GameStates.");
			#endif
			// ... and dispatch NSYS_QUIT to active Game State hierarchy ONLY (UIs don't Handle it)!
			//			obsolet: NDispatchGameStateEvent(pevent);
			pcurrentstate = NGameStateMachine.pCurrentEventRootGameState;
			while( pcurrentstate )
			{
				((NEVENT*)pevent)->Head.pRecipient = pcurrentstate;
				((NEVENT*)pevent)->Head.RecipientCast = NEVENT_RECIPIENT_CAST_GAMESTATE;
				pcurrentstate->pEventDrivenHandle(pevent,pcurrentstate->User_Nu32);
				pcurrentstate = pcurrentstate->pCurrentEventChild;
			}
			// ... And Inform Engine that user asks to Quit
			N_AskToQuit();
			break;

		case NSYS_RETURN_BACK:
			#ifdef _NOUT_SYS_EVENT
			printf("\n> NDispatch NSYS_RETURN_BACK to GameStates ONLY.");
			#endif
			// This Event is generated when user Touch the Return Back button on "ANDROID" machine
			// So it is only dispatched to Game State, UI are not concerned ...
			//			obsolet: NDispatchGameStateEvent(pevent);
			pcurrentstate = NGameStateMachine.pCurrentEventRootGameState;
			while( pcurrentstate )
			{
				((NEVENT*)pevent)->Head.pRecipient = pcurrentstate;
				((NEVENT*)pevent)->Head.RecipientCast = NEVENT_RECIPIENT_CAST_GAMESTATE;
				pcurrentstate->pEventDrivenHandle(pevent,pcurrentstate->User_Nu32);
				pcurrentstate = pcurrentstate->pCurrentEventChild;
			}
			break;


		// ----------------------------------------------------------------------------------------
		// -
		// - _TOUCH
		// -
		// Touch Events are dispatched to UIs and GAMESTATEs. Each Touch Event has its own specific 
		// and optimized dispatch function.
		case NTOUCH_START:
			#ifdef _NOUT_SYS_EVENT	
			printf("\n> NDispatchTouch_Start.");
			#endif
			NDispatchTouch_Start(&pevent->Touch.TouchSequence);
			break;
		
		case NTOUCH_MOVE:
			#ifdef _NOUT_SYS_EVENT	
			printf("\n> NDispatchTouch_Move.");
			#endif
			NDispatchTouch_Move(&pevent->Touch.TouchSequence);
			break;
		
		case NTOUCH_END:
			#ifdef _NOUT_SYS_EVENT	
			printf("\n> NDispatchTouch_End.");
			#endif
			NDispatchTouch_End(&pevent->Touch.TouchSequence);
			break;

		case NTOUCH_CANCEL:
			#ifdef _NOUT_SYS_EVENT	
			printf("\n> NDispatchTouch_Cancel.");
			#endif
			NDispatchTouch_Cancel(&pevent->Touch.TouchSequence);
			break;
	
		case NSYS_NOT_TRANSLATED:
			#ifdef _NOUT_SYS_EVENT
			printf("\n> Event Not Translated/Not Dispatched .");
			#endif
			break;

		// ----------------------------------------------------------------------------------------
		default:
			break;
	}

	// Remove NTOUCH associated with the TouchEvent from NTouchStream Ring Buffer
	if( NGET_EVENT_MAIN(pevent->Type)==_TOUCH )
	{
		#ifdef _NOUT_SYS_EVENT
		printf("\n> Dispatch of TOUCH Event Terminated. Remove Touch(es) from Touch Stream Buffer.");
		printf("\n -------------------------------------------------------------------------------------------------- \n");
		#endif

		if( pevent->Touch.TouchSequence.TouchCount == 1 )
		{
			NRingBufferFreeFront(&NTouchStream);
		}
		else if( pevent->Touch.TouchSequence.TouchCount )
		{
			NRingBufferFreeRangeFront(&NTouchStream, pevent->Touch.TouchSequence.TouchCount);
		}
	}
*/

	/// NEW DISPATCH SYSTEM

	// UI & GC DISPATCH
	switch( NGET_EVENT_MAIN(pevent->Type) )
	{
		case _SYSTEM:
			switch(pevent->Type)
			{
				// ----------------------------------------------------------------------------------------
				// -
				// - _SYSTEM
				// -
				// System Events are all specifics. 
				// Some of them may generate first TOUCH CANCEL ALL event dispatch (to UI and GameState) --> NSYS_QUIT
				// Some of them may be dispatch only to game state hierarchy ------------------------------> NSYS_RETURN_BACK
				// Some other ... To Be Defined ...
				//
				case NSYS_QUIT:
					#ifdef _NOUT_SYS_EVENT
					printf("\n> NDispatch NSYS_QUIT.");
					printf("\n\t> -As NTOUCH_CANCEL (ALL) to UIs.");
					#endif
					// Generate a TOUCH CANCEL ALL dispatch to 'KILL' all current Touch Listening
					// UI and Game State are going to receive it ...
					touch_sequence.TouchCount = 0;
					NDispatchTouch_Cancel(&touch_sequence);
					#ifdef _NOUT_SYS_EVENT
					printf("\n\t> -As NSYS_QUIT to GameStates.");
					#endif
					// ... and dispatch NSYS_QUIT to active Game State hierarchy ONLY (UIs don't Handle it)!
					_dispatch_event_to_gamestates(pevent);	
					// ... And Inform Engine that user asks to Quit
					N_AskToQuit();
					break;

				case NSYS_NOT_TRANSLATED:
					#ifdef _NOUT_SYS_EVENT
					printf("\n> Event Not Translated/Not Dispatched .");
					#endif
					break;

				default:
					break;
			}
			break;

		case _KEY_DOWN:
			#ifdef _NOUT_SYS_EVENT
			printf("\n> NDispatch _KEY_DOWN + _KEYCODE to GameStates ONLY.");
			#endif
			// This Event is generated when user Touch the Return Back button on "ANDROID" machine
			// So it is only dispatched to Game State, UI are not concerned ...
			_dispatch_event_to_gamestates(pevent);
			break;

		case _KEY_UP:
			#ifdef _NOUT_SYS_EVENT
			printf("\n> NDispatch _KEY_UP + _KEYCODE to GameStates ONLY.");
			#endif
			// This Event is generated when user Touch the Return Back button on "ANDROID" machine
			// So it is only dispatched to Game State, UI are not concerned ...
			_dispatch_event_to_gamestates(pevent);
			break;

// 		case _UICORE:			
// 			break;

// 		case _UINOTIFY:		
// 			break;

		case _TOUCH:
			switch(pevent->Type)
			{
				case NTOUCH_START:
					#ifdef _NOUT_SYS_EVENT	
					printf("\n> NDispatchTouch_Start.");
					#endif
					NDispatchTouch_Start(&pevent->Touch.TouchSequence);
					break;

				case NTOUCH_MOVE:
					#ifdef _NOUT_SYS_EVENT	
					printf("\n> NDispatchTouch_Move.");
					#endif
					NDispatchTouch_Move(&pevent->Touch.TouchSequence);
					break;

				case NTOUCH_END:
					#ifdef _NOUT_SYS_EVENT	
					printf("\n> NDispatchTouch_End.");
					#endif
					NDispatchTouch_End(&pevent->Touch.TouchSequence);
					break;

				case NTOUCH_CANCEL:
					#ifdef _NOUT_SYS_EVENT	
					printf("\n> NDispatchTouch_Cancel.");
					#endif
					NDispatchTouch_Cancel(&pevent->Touch.TouchSequence);
					break;
			}
			#ifdef _NOUT_SYS_EVENT
			printf("\n> Dispatch of TOUCH Event Terminated. Remove Touch(es) from Touch Stream Buffer.");
			printf("\n -------------------------------------------------------------------------------------------------- \n");
			#endif
			if( pevent->Touch.TouchSequence.TouchCount == 1 )
			{
				NRingBufferFreeFront(&NTouchStream);
			}
			else if( pevent->Touch.TouchSequence.TouchCount )
			{
				NRingBufferFreeRangeFront(&NTouchStream, pevent->Touch.TouchSequence.TouchCount);
			}
			break;

// 		case _UICOMMAND:		
// 			break;

		case _AD_REWARDED_VIDEO:
			#ifdef _NOUT_SYS_EVENT
			printf("\n> NDispatch _AD_REWARDED_VIDEO Event to GameStates ONLY.");
			#endif

			// This Event is generated when an Advertising Rewarded Video is doing something ...
			// So it is only dispatched to Game State, UI are not concerned ...
			_dispatch_event_to_gamestates(pevent);	
			break;

		case _AD_INTERSTITIAL:
			#ifdef _NOUT_SYS_EVENT
			printf("\n> NDispatch _AD_INTERSTITIAL Event to GameStates ONLY.");
			#endif

			// This Event is generated when an Advertising Rewarded Video is doing something ...
			// So it is only dispatched to Game State, UI are not concerned ...
			_dispatch_event_to_gamestates(pevent);	
			break;

		case _AD_BANNER:		
			#ifdef _NOUT_SYS_EVENT
			printf("\n> NDispatch _AD_BANNER Event to GameStates ONLY.");
			#endif

			// This Event is generated when an Advertising Rewarded Video is doing something ...
			// So it is only dispatched to Game State, UI are not concerned ...
			_dispatch_event_to_gamestates(pevent);	
			break;
	}
}

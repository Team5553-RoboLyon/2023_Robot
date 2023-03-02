#ifndef __NUI_PUSH_H
#define __NUI_PUSH_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									  NUIPush.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "NUI.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// Notes:
// Push UI system using the following flags define into NUI.h:
// 'NUI_DESC.Flags_Core'
//	#define		FLAG_NUIDC_BBOX_UPDATE_AT_PUSH_UP						BIT_8
//	#define		FLAG_NUIDC_BBOX_UPDATE_AT_PUSH_DOWN						BIT_9
// 'NUI.Flags'
//	#define		FLAG_NUI_BBOX_UPDATE_AT_PUSH_UP							BIT_12
//	#define		FLAG_NUI_BBOX_UPDATE_AT_PUSH_DOWN						BIT_13


// LOW LEVEL HOW TO USE IT
// Its possible to create new controller with a push behavior by using these 2 functions in the right way.
// This controller will use "Push NUI_STATES" and only these ones. It will be impossible to use some additionnal States.
// 
//		-------------------------------------------------------------------------------------------------
//		1)	Use "NUIPush_EventToState_PreHandle" as "pui->UIEventToState_Proc".
//			Or encapsulate it into your own "EventToState_Handle" to decorate the cases you want.
//			Be aware that it's not possible to change states by yourself.
//
// 			void your_EventToState_Handle(NUI *pui, const NEVENT *pevent)
// 			{
// 				switch(pevent->Type)
// 				{
// 					case NTOUCH_START:
// 						Do your stuff here WITHOUT CHANGING UI STATE by yourself !!! ('NUIPush_EventToState_PreHandle' will do it )
//						break;
// 			
// 					case NTOUCH_MOVE:
// 						Do your stuff here WITHOUT CHANGING UI STATE by yourself !!! ('NUIPush_EventToState_PreHandle' will do it )
// 						break;
// 			
// 					case NTOUCH_END:
// 						Do your stuff here WITHOUT CHANGING UI STATE by yourself !!! ('NUIPush_EventToState_PreHandle' will do it )
// 						break;
// 			
// 					case N_FOCUS_KILLED:
// 						Do your stuff here WITHOUT CHANGING UI STATE by yourself !!! ('NUIPush_EventToState_PreHandle' will do it )
// 						break;
// 			
// 					default:
// 						break;
// 				}

// 				NUIPush_EventToState_PreHandle(pui,pevent);
// 			}
//
//		or Like this,
//
//			void your_EventToState_Handle(NUI *pui, const NEVENT *pevent)
//			{
//				First, call the EventToState 'Push' function.
//				It will manage 'ALONE' the UI "State". 
//				Actually, purpose of 'your_EventToState_Handle' is just sending appropriate Direct EVENT according 
//				with some specific case.
//				NEVENT		nevent;
//
//				
//				switch(NUIPush_EventToState_PreHandle(pui,pevent))
//				{
//					case NUI_KEY_INSTANT_PUSH_DOWN_CANCEL: // When the PUSH UI Leave "Down Wait position" due to a Cancel decision ( = Touch Move outside the UI, after this one was resting into dow wait position)
//						Do your Stuff here Do your stuff here WITHOUT CHANGING UI STATE by yourself !!! ('NUIPush_EventToState_PreHandle' already did it )
//						... for example...
//						Send CANCEL Command Event ...
//						memset(&nevent,0,sizeof(NEVENT));
//						nevent.Type = NCOMMAND_PUSHBUTTON_DOWN_CANCEL;
//						pui->Event_Proc(pui,&nevent); 
//						break;
//
//					case NUI_KEY_INSTANT_PUSH_UP: // When the PUSH UI reach "Up position"
//						Do your Stuff here Do your stuff here WITHOUT CHANGING UI STATE by yourself !!! ('NUIPush_EventToState_PreHandle' already did it )
//						Send Command Event ...
//						memset(&nevent,0,sizeof(NEVENT));
//						nevent.Type = NCOMMAND_PUSHBUTTON_UP;
//						pui->Event_Proc(pui,&nevent); 
//						break;
//
//					case NUI_KEY_INSTANT_PUSH_DOWN: // When the PUSH UI reach "Down position"
//						Do your Stuff here Do your stuff here WITHOUT CHANGING UI STATE by yourself !!! ('NUIPush_EventToState_PreHandle' already did it )
//						Send Command Event ...
//						memset(&nevent,0,sizeof(NEVENT));
//						nevent.Type = NCOMMAND_PUSHBUTTON_DOWN;
//						pui->Event_Proc(pui,&nevent);
//						break;
//
//					default:
//						break;
//				}
// 			}
//		-------------------------------------------------------------------------------------------------
//		2) Encapsulate "NUIPush_States_PreHandle" into your own states_handle like this:
//			
// 			void your_States_Handle(NUI *pui, const NTIME	*ptime)
// 			{
// 				switch( NUIPush_States_PreHandle(pui,ptime) )
// 				{
//					case NUI_STATE_PUSH_UP_REST:
// 						break;
//					case NUI_STATE_PUSH_TO_UP: 
// 						break;
//					case NUI_STATE_PUSH_TO_UP_CANCEL:
// 						break;
//					case NUI_STATE_PUSH_TO_DOWN:
// 						break;
//					case NUI_STATE_PUSH_DOWN_WAIT:
// 						break;
// 					case NUI_KEY_INSTANT_PUSH_DOWN_CANCEL:
// 						break;
// 					case NUI_KEY_INSTANT_PUSH_UP:
//						break;
// 					case NUI_KEY_INSTANT_PUSH_DOWN:
// 						break;
//					default:
// 						break;
// 				}
// 				NDefaultUIState_Proc(pui, ptime);
// 			}
//
//		-------------------------------------------------------------------------------------------------
//			As you see, the switch/case is based on the "NUIPush_States_PreHandle" return value and not on "pui->CurrentState" ...
//			Its because "NUIPush_States_PreHandle" already update "pui->CurrentState" but return the previous "pui->currentState".    
//			So, your 'switch' can decorate 'the state' like they were before "NUIPush_States_PreHandle(pui,ptime)".
//			BUT AGAIN in your switch/case, don't change any state, just 'break' and call 'NDefaultUIState_Proc(pui, ptime)'

#define NMAKE_UIPUSH_PREHANDLE_RESULT(key_instant,prev_state)		NMAKELONG(key_instant,prev_state)
#define NGET_UIPUSH_PREHANDLE_KEY_INSTANT(Nu32_prehandle_result)	NLOWORD(Nu32_prehandle_result)
#define NGET_UIPUSH_PREHANDLE_PREV_STATE(Nu32_prehandle_result)		NHIWORD(Nu32_prehandle_result)

// FUNCTIONS
Nu32	NUIPush_EventToState_PreHandle(NUI *pui, const NEVENT *pevent);
Nu32	NUIPush_States_PreHandle(NUI *pui, const NTIME	*ptime);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif	// __NUI_PUSH_H 


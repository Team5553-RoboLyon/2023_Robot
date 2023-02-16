#include "../NCStandard.h"
#include "../NCoordinateSystemConversions.h"
#include "../NViewport.h"
#include "../NCore.h"
#include "../UI/NUI.h"
#include "../UI/NUIClearWatchStack.h"
#include "NEvent.h"

//#include "../Utilities/NUT_Draw.h"
/*
static Nu32 UIFocusCount;
static inline void	_DecreaseFocusCount(){UIFocusCount--;/ *printf("UIFocusCount = %d\n",UIFocusCount);* /}
static inline void	_IncreaseFocusCount(){UIFocusCount++;/ *printf("UIFocusCount = %d\n",UIFocusCount);* /}
Nu32 NGetUIFocusCount(){return UIFocusCount;}
*/
extern NARRAY	ActiveUIRootsList;
extern NUI		*pFocusedUI;
extern Nu32		NEngineCoreFlags;


NARRAY			DelayedUIEventArray;


#ifdef _NOUT_SYS_EVENT
Nu32 debug_id;
#endif


#ifdef _NOUT_SYS_EVENT
void _debug_Printf_EVENT_NAME(const NEVENT *pevent)
{
	switch(pevent->Type)
	{
	case NSYS_NOEVENT:
		printf(" NSYS_NOEVENT ");
		break;

	case NSYS_NOT_TRANSLATED:
		printf(" NSYS_NOT_TRANSLATED ");
		break;

	case NSYS_QUIT:					
		printf(" NSYS_QUIT ");
		break;

	case NKEY_DOW_BACK:
		printf(" NKEY_DOW_BACK ");
		break;

	case NTOUCH_START:
		printf(" NTOUCH_START ");
		break;
	case NTOUCH_MOVE:					
		printf(" NTOUCH_MOVE ");
		break;
	case NTOUCH_END:					
		printf(" NTOUCH_END ");
		break;
	case NTOUCH_CANCEL:
		if(pevent->Touch.TouchSequence.TouchCount)
			printf(" NTOUCH_CANCEL ");
		else
			printf(" NTOUCH_CANCEL (All)");
		break;
	case NUI_SETUP:						
		printf(" NUI_SETUP ");
		break;
	case NUI_CLEAR:						
		printf(" NUI_CLEAR ");
		break;
	case NUI_SHOW:						
		printf(" NUI_SHOW ");
		break;
	case NUI_HIDE:						
		printf(" NUI_HIDE ");
		break;
	case NUI_ENABLE:					
		printf(" NUI_ENABLE ");
		break;
	case NUI_DISABLE:					
		printf(" NUI_DISABLE ");
		break;
	// UI NOTIFY
	case NNOTIFY_UITHNANIM_START:
		printf(" NNOTIFY_UITHNANIM_START ");
		break;
	case NNOTIFY_UITHNANIM_KEY:		
		printf(" NNOTIFY_UITHNANIM_KEY ");
		break;
	case NNOTIFY_UITHNANIM_REPEAT:	
		printf(" NNOTIFY_UITHNANIM_REPEAT ");
		break;
	case NNOTIFY_UITHNANIM_END:		
		printf(" NNOTIFY_UITHNANIM_END ");
		break;

	// COMMAND AND CANCEL
	case NCOMMAND_PUSHBUTTON_DOWN:		
		printf(" NCOMMAND_PUSHBUTTON_DOWN ");
		break;
	case NCOMMAND_PUSHBUTTON_UP:		
		printf(" NCOMMAND_PUSHBUTTON_UP ");
		break;
	case NCOMMAND_PUSHBUTTON_CANCEL_DOWN:		
		printf(" NCOMMAND_PUSHBUTTON_CANCEL_DOWN ");
		break;

	case NCOMMAND_UICUSTOM_DOWN:		
		printf(" NCOMMAND_UICUSTOM_DOWN ");
		break;
	case NCOMMAND_UICUSTOM_UP:			
		printf(" NCOMMAND_UICUSTOM_UP ");
		break;
	case NCOMMAND_UICUSTOM_CANCEL_DOWN:		
		printf(" NCOMMAND_UICUSTOM_CANCEL_DOWN ");
		break;

	case NCOMMAND_VALUPDATER_NF32:		
		printf(" NCOMMAND_VALUPDATER_NF32 ");
		break;
	case NCOMMAND_VALUPDATER_NS32:		
		printf(" NCOMMAND_VALUPDATER_NS32 ");
		break;

	case NCOMMAND_CHECKBOX_CHECK:		
		printf(" NCOMMAND_CHECKBOX_CHECK ");
		break;
	case NCOMMAND_CHECKBOX_UNCHECK:		
		printf(" NCOMMAND_CHECKBOX_UNCHECK ");
		break;
	case NCOMMAND_CHECKBOX_TOGGLE:		
		printf(" NCOMMAND_CHECKBOX_TOGGLE ");
		break;

	case NCOMMAND_COLORCIRCLE_RGBA8:	
		printf(" NCOMMAND_COLORCIRCLE_RGBA8 ");
		break;
	case NCOMMAND_COLORCIRCLE_RGBA32:	
		printf(" NCOMMAND_COLORCIRCLE_RGBA32 ");
		break;
	
	default:
		printf(" ? Unknown EVENT ? ");
		break;
	}
}
#endif

void NInitDelayedUIEventArray()
{
	NSetupArray(&DelayedUIEventArray,CONSTANT_DELAYED_UI_EVENT_ARRAY_INITIAL_CAPACITY,sizeof(NEVENT));
}

void NQuitDelayedUIEventArray()
{
	NClearArray(&DelayedUIEventArray,NULL);
}

// ------------------------------------------------------------------------------------------
// void NDelayedUIEventArray_UIClear_Notification
// ------------------------------------------------------------------------------------------
// Description :
//	Purpose of this function is removing 'all' the Delayed Event with 'pui' as recipient.
//	Because UI is under clearing process !
//
//		--> _SYSTEM		Can not be delayed ! So are not concerned because not suppose to be into the Delayed Event Array !
//		--> _TOUCH		Can not be delayed ! So are not concerned because not suppose to be into the Delayed Event Array !
//		--> _UICORE		Can not be delayed ! So are not concerned because not suppose to be into the Delayed Event Array !
//
//		--> _UICOMMAND	May be delayed ! And has to be remove ! Because a Cleared/Deleted UI can not receive _UICOMMAND Event !!!
//		--> _UINOTIFY	May be delayed ! And has to be remove ! Because a Cleared/Deleted UI can not receive _UINOTIFY Event !!!
//

// ------------------------------------------------------------------------------------------
// In	:
//			NUI *pui	The UI that is under clearing process.
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NDelayedUIEventArray_UIClear_Notification(const NUI *pui)
{
	NEVENT *pevent = (NEVENT*)DelayedUIEventArray.pFirst;
	for(Nu32 i=DelayedUIEventArray.Size;i!=0;i--, pevent ++)
	{
		if((NUI*)((NEVENT_HEAD*)pevent)->pRecipient == pui)
		{
			((NEVENT_HEAD*)pevent)->pRecipient		= NULL;
			((NEVENT_HEAD*)pevent)->RecipientCast	= NEVENT_RECIPIENT_CAST_NULL; 
		}
	}
}

// ------------------------------------------------------------------------------------------
// void NDelayedUIEventArray_UIHide_Notification
// ------------------------------------------------------------------------------------------
// Description :
//	Purpose of this function is removing the Delayed Events with 'pui' as recipient,
//	which are not compatible with the "HIDE" status of UI
//	Because UI is under Hide process !
//
//		--> _SYSTEM		Can not be delayed ! So are not concerned because not suppose to be into the Delayed Event Array !
//		--> _TOUCH		Can not be delayed ! So are not concerned because not suppose to be into the Delayed Event Array !
//		--> _UICORE		Can not be delayed ! So are not concerned because not suppose to be into the Delayed Event Array !
//
//		--> _UICOMMAND	May be delayed ! And has to be remove ! Because a Hidden UI can not receive _UICOMMAND Event !!!
//		--> _UINOTIFY	May be delayed ! And has NOT to be remove ! Because a Hidden UI may receive _UINOTIFY Event !!!
//
//	As you can see, 'NDelayedUIEventArray_UIHide_Notification' does exactly the same thing than 'NDelayedUIEventArray_UIDisable_Notification'
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
//	 NDelayedUIEventArray_UIHide_Notification(pui) is defined in NEvent.h as a define up on  "NDelayedUIEventArray_UIDisable_Notification"

// ------------------------------------------------------------------------------------------
// void NDelayedUIEventArray_UIDisable_Notification
// ------------------------------------------------------------------------------------------
// Description :
//	Purpose of this function is removing the Delayed Events with 'pui' as recipient,
//	which are not compatible with the "DISABLE" status of UI
//	Because UI is under Disable process !
//
//		--> _SYSTEM		Can not be delayed ! So are not concerned because not suppose to be into the Delayed Event Array !
//		--> _TOUCH		Can not be delayed ! So are not concerned because not suppose to be into the Delayed Event Array !
//		--> _UICORE		Can not be delayed ! So are not concerned because not suppose to be into the Delayed Event Array !
//
//		--> _UICOMMAND	May be delayed ! And has to be remove ! Because a Disable UI can not receive _UICOMMAND Event !!!
//		--> _UINOTIFY	May be delayed ! And has NOT to be remove ! Because a Disable UI may receive _UINOTIFY Event !!!
//
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NDelayedUIEventArray_UIDisable_Notification(const NUI *pui)
{
	NEVENT *pevent = (NEVENT*)DelayedUIEventArray.pFirst;
	for(Nu32 i=DelayedUIEventArray.Size;i!=0;i--, pevent ++)
	{
		if((NUI*)((NEVENT_HEAD*)pevent)->pRecipient == pui && (NGET_EVENT_MAIN(pevent->Type)==_UICOMMAND) )// _UICOMMAND need an enable ui to work !!! so remove them all !!
		{
			((NEVENT_HEAD*)pevent)->pRecipient		= NULL;
			((NEVENT_HEAD*)pevent)->RecipientCast	= NEVENT_RECIPIENT_CAST_NULL; 
		}
	}
}

/*
void NDelayedUIEventArray_GetEventMainTypeNumber(const NUI *pui, const Nu32 event_main_type)
{
	Nu32 nb = 0;
	NEVENT *pevent = (NEVENT*)DelayedUIEventArray.pFirst;
	for(Nu32 i=DelayedUIEventArray.Size;i!=0;i--, pevent ++)
	{
		if((NUI*)((NEVENT_HEAD*)pevent)->pRecipient == pui && (NGET_EVENT_MAIN(pevent->Type)==event_main_type ) )
		{
			nb ++;
		}
	}
	return nb;
}
*/

void NPostDelayedUIEvent(const NEVENT *pevent)
{
	// Event must be define !
	NErrorIf(!pevent, NERROR_NULL_POINTER);
	// Event must me authorized to be delayed ! Only _UICOMMAND and _UINOTIFY events are authorized !
	NErrorIf( NGET_EVENT_MAIN(pevent->Type)!=_UICOMMAND && NGET_EVENT_MAIN(pevent->Type)!=_UINOTIFY, NERROR_EVENT_UNAUTHORIZED_MAIN_TYPE );
	// Event Recipient definition must be consistent !
	NErrorIf(pevent->Head.pRecipient == NULL && pevent->Head.RecipientCast != NEVENT_RECIPIENT_CAST_NULL, NERROR_INCONSISTENT_VALUES );
	NErrorIf(pevent->Head.pRecipient != NULL && pevent->Head.RecipientCast == NEVENT_RECIPIENT_CAST_NULL, NERROR_INCONSISTENT_VALUES );
	// Event Recipient must be define !
	NErrorIf(pevent->Head.pRecipient == NULL, NERROR_NULL_POINTER);
	NErrorIf(pevent->Head.RecipientCast == NEVENT_RECIPIENT_CAST_NULL, NERROR_NULL_VALUE);
	
	// About UI Visible and UI Enable and Delayed Event.
	//		_UINOTIFY event can be sent to UI even if UI is hidden and/or Disable.
	//		_UICOMMAND event are more restrictive. They can be sent to Visible AND Enable UI only.
	//		These two kind of events may be delayed ( only these 2 ones actually ! ). But it's not purpose of 'NPostDelayedUIEvent'
	//		to check UI states. When Event is going to be dispatch by 'NDispatchDelayedUIEvent' the appropriate sending function ...
	//		... 'NSendUICommandEvent' or 'NSendUINotifyEvent' is going to be called and will make the checks ( In debug mode ).
	//
	//		AND, The functions:
	//				"NDelayedUIEventArray_UIDisable_Notification"
	//				"NDelayedUIEventArray_UIHide_Notification"
	//				"NDelayedUIEventArray_UIClear_Notification"
	//
	//		... are going to cleanup the Delayed Event Array each time an UI is cleared/deleted, disable or hide.
	//
	#ifdef _NOUT_SYS_EVENT
	debug_id ++;
	Nu32 local_debug_id = debug_id;
	printf("\n \n ================================================= \n[%d] - NPostDelayedUIEvent: ",local_debug_id);
	_debug_Printf_EVENT_NAME(pevent);
	#endif

	NArrayPushBack(&DelayedUIEventArray,(NBYTE*)pevent);
}

void NDispatchDelayedUIEvent()
{
#ifdef _NOUT_SYS_EVENT
	if(DelayedUIEventArray.Size)
		printf("\n ========================= %d Next SendUIEvent(s) is(are) DELAYED ===============",DelayedUIEventArray.Size);
#endif

	NEVENT *pevent = (NEVENT*)DelayedUIEventArray.pFirst;
	for(Nu32 i=DelayedUIEventArray.Size;i!=0;i--, pevent ++)
	{
		if(pevent->Head.pRecipient) // Because it may be set to NULL by NUI Clear process
		{
			NErrorIf(pevent->Head.RecipientCast != NEVENT_RECIPIENT_CAST_UI, NERROR_EVENT_INCONSISTENT_RECIPIENT);
			switch(NGET_EVENT_MAIN(pevent->Type))
			{
				case _UICOMMAND:
					NSendUICommandEvent(pevent);
					break;

				case _UINOTIFY:
					NSendUINotifyEvent(pevent);
					break;

				default:
					// Only _UICOMMAND and _UINOTIFY event main types can be delayed !!!
					NErrorIf(1,NERROR_SYSTEM_CHECK);
					break;
			}
		}
	}
	DelayedUIEventArray.Size = 0;
}


// ------------------------------------------------------------------------------------------
// void NSendUIEvent
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
//			NUI				*pui		A Valid pointer to UI which will receive the Event.
//
//			const NEVENT	*pevent		A valid pointer to the Event to sent.
// Out :
// ------------------------------------------------------------------------------------------
/*

static inline Nu32 _get_uievent_listen_and_interception_flags(const Nu32 event_type)
{
	switch( NGET_EVENT_MAIN(event_type) )
	{

		case _UICORE:					
			return FLAG_NUI_INTERCEPT_UICORE_EVENT|FLAG_NUI_LISTEN_UICORE_EVENT;

		case _UINOTIFY:				
			return FLAG_NUI_INTERCEPT_UINOTIFY_EVENT|FLAG_NUI_LISTEN_UINOTIFY_EVENT|FLAG_NUI_SHOW;

		case _UICOMMAND:				
			return FLAG_NUI_INTERCEPT_UICOMMAND_EVENT|FLAG_NUI_LISTEN_UICOMMAND_EVENT|FLAG_NUI_SHOW|FLAG_NUI_ENABLE;

// 		case _SYSTEM:
//			Not possible here because this EVENT MAIN TYPE is not accepted by UI !!!
//			...So let 'default'	case manage it.
// 			break;

//		case _TOUCH:
//			Not possible here because this EVENT MAIN TYPE is not accepted by UI !!!
//			...So let 'default'	case manage it.
//			BUT BUT BUT, if we want to make it works with _TOUCH ... it would necessary to check event subtype ( start and move )
//			against all the 4 listen touch event flags, especially ...capture_start and ...capture_move_in.
//			return ???;

		default:
			// 	"_get_uievent_listen_and_interception_flags" should manage all the EVENT MAIN TYPE accepted by UI ...		
			NErrorIf(1,NERROR_SYSTEM_CHECK);
			return 0;
	}
}
*/


/*
#define NGetEvent_ListenAndInterceptionMask(event_type)		(  3 << (NGET_EVENT_MAIN(event_type) << 1) )
#define NGetEvent_ListenMask(event_type)					(  1 << (NGET_EVENT_MAIN(event_type) << 1) )
#define NGetEvent_InterceptionMask(event_type)				(  2 << (NGET_EVENT_MAIN(event_type) << 1) )
*/

// ------------------------------------------------------------------------------------------
// void NSendUICoreEvent
// ------------------------------------------------------------------------------------------
// Description :
//		Send a UICore Event to a UI Recipient.
//
//		!!! ALWAYS SEND UICORE EVENT TO UI RECIPIENT.UIEventToState_Proc !!!
//		!!! SEND UICORE EVENT TO UI RECIPIENT.Event_Proc ONLY If 'dispatch' is not refused by 
//			one of the potential parent interceptor.
//		!!! BUT BUT BUT, even if UI doesn't receive it's UICore Event trough its "Event_Proc" due to a dispatch refusal,
//		!!! that doesn't means the action or change associated with the event is not perform. 
//		!!! Understand UICORE Event like notification of something that is happening !
//			For example:
//				N send the UICOre Event 'NUI_HIDE' to UI.
//				One parent of UI is intercepting the Event and doesn't return DISPATCH ( it returns PROCESSED ).
//				....
//				That doesn't means UI is not going to be hidden. It will be, but UI is not going to receive the
//				NUI_HIDE Event...
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSendUICoreEvent(const NEVENT *pevent)
{
	NErrorIf(!pevent,NERROR_NULL_VALUE);
	NErrorIf(pevent->Head.RecipientCast != NEVENT_RECIPIENT_CAST_UI, NERROR_EVENT_INCONSISTENT_RECIPIENT);
	// Check if event is "UI compatible"
	NErrorIf(	NGET_EVENT_MAIN(pevent->Type)!=_UICORE, NERROR_EVENT_INCONSISTENT_TYPE	);

	// ----------------------------------------------------------------------------------------------------------
	// - Debug OutPut
	//
	#ifdef _NOUT_SYS_EVENT
	Nu32 local_debug_id;
	debug_id ++;
	local_debug_id = debug_id;
	printf("\n \n -------------------------------------------------------------------------------------------------- ");
	printf("\n[%d] + NSendUICoreEvent: ",local_debug_id);
	_debug_Printf_EVENT_NAME(pevent);
	#endif
	//
	// ----------------------------------------------------------------------------------------------------------

	// 1)	Check for a potential UI Event Blocking. 
	if( ISFLAG_ON(NEngineCoreFlags,FLAG_NENGINE_CORE_BLOCK_EVENT_TO_UI) )
	{
		// ------------------------------------
		// - Debug OutPut
		#ifdef _NOUT_SYS_EVENT
		printf("\n		|");
		printf("\n		+--> Non Send.Blocked By Engine.");
		printf("\n \n -------------------------------------------------------------------------------------------------- ");
		#endif
		// ------------------------------------
		return;
	}

	// Check UI address for using and Push them to StackSurvey to be aware of a potential UI delete
	
	NUI		*pui						= (NUI*)pevent->Head.pRecipient;
	Nu32	stacksize					= 0;

	// 2)	Push UI on the ClearWatchStack ...
	//		( this push is not added to the stacksize counter...it's normal, it will pop AFTER the parent interception process which use the stacsize )
	NUIClearWatchStackPush( pui );

	#ifdef _DEBUG	
	Nu32 dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
	#endif

	// 3)	Is UI Parents chain intercept this Kind of event ?
	NUI *pparent = (NUI*)pui->pParent;
	while(pparent)
	{
		if( ISFLAG_ON(pparent->Flags,FLAG_NUI_INTERCEPT_UICORE_EVENT)  )
		{
			NUIClearWatchStackPush( pparent );
			stacksize ++;
		}
		pparent = (NUI*)pparent->pParent;
	}

	// ----------------------------------------------------------------------------------------------------------
	// - Debug OutPut
	//
	#ifdef _NOUT_SYS_EVENT
	if( stacksize )
	{
		printf("\n		|");
		printf("\n		+--> UIRecipient and %d UI Parent Pushed to NUIClearWatchStack.", stacksize );
		printf("\n		+--> Start Interception Process: %d Parent Interception(s) planned before Sending to UI recipient.",(stacksize) );
	}
	else
	{
		printf("\n		|");
		printf("\n		+--> UIRecipient and %d UI Parent Pushed to NUIClearWatchStack.", stacksize );
		printf("\n		+--> Start Interception Process: No Parent Interception planned." );
	}
	#endif
	// -
	// ----------------------------------------------------------------------------------------------------------
	Nu32 dispatch = (ISFLAG_ON(pui->Flags,FLAG_NUI_LISTEN_UICORE_EVENT))? NTRUE:NFALSE;

	while(stacksize)
	{
		if( pparent = NUIClearWatchStackPop() )
		{
			// ----------------------------------------------------------------------------------------------------------
			// - Debug OutPut
			//
			#ifdef _NOUT_SYS_EVENT
			printf("\n		|");
			printf("\n		+--> UI#%d[0x%p] pops from Stack.",stacksize,pparent);
			printf("\n				+");
			printf("\n				|");
			if(pparent->pName)
				printf("\n				+--> Interception By Parent[0x%p][%s].Event_Proc()-->", pparent, pparent->pName );
			else
				printf("\n				+--> Interception By Parent[0x%p].Event_Proc()-->", pparent );
			#endif
			// -
			// ----------------------------------------------------------------------------------------------------------
			// !!! UICore Event doesn't send event trough "pparent->UIEventToState_Proc(pparent,pevent)" for parent that intercept !!!

			// Stop Event Dispatching ( request by parent )
			if( ! NIS_UI_EVENT_RESULT_DISPATCH(pparent->Event_Proc(pparent,pevent)) )
			{
				#ifdef _NOUT_SYS_EVENT
				printf("return 'NUI_EVENT_RESULT_PROCESSED'" );
				printf("\n				|");
				printf("\n				+--> INTERCEPTION CHAIN INTERRUPTED by USER" );
				#endif

				// Reset stack size to initial value:
				NDecreaseUIClearWatchStackSize(stacksize-1);
				dispatch = NFALSE;
				break;
			}
			#ifdef _NOUT_SYS_EVENT
			else
			{
				printf("return 'NUI_EVENT_RESULT_DISPATCH'" );
			}
			#endif
		}
		#ifdef _NOUT_SYS_EVENT
		else
		{
			printf("\n				|");
			printf("\n				+--> UI Parent deleted during event Interception Processing.");
			printf("\n					 This Parent failed to Intercept Event.");
			printf("\n					 Interception Process Continuing.");
		}
		#endif

		stacksize--;
	}

	NErrorIf(NGetUIClearWatchStackSize()!=dbg_initial_clear_watch_stack_size, NERROR_SYSTEM_CHECK);

	// ----------------------------------------------------------------------------------------------------------
	// - Debug OutPut
	//
	#ifdef _NOUT_SYS_EVENT
	printf("\n		|");
	printf("\n		+--> END Interception Process." );
	#endif
	// -
	// ----------------------------------------------------------------------------------------------------------
	if( pui == NUIClearWatchStackPop() )
	{
		// ----------------------------------------------------------------------------------------------------------
		// - Debug OutPut
		//
		#ifdef _NOUT_SYS_EVENT
		printf("\n		|");
		printf("\n		+--> UI#%d[0x%p] pops from Stack.",stacksize,pui);
		printf("\n				+");
		printf("\n				|");
		printf("\n				+--> Send Event to UI Recipient[0x%p].UIEventToState_Proc()",pui );
		#endif
		// -
		// ----------------------------------------------------------------------------------------------------------

		// !!! ALWAYS SEND UICORE EVENT TO UI RECIPIENT.UIEventToState_Proc !!!
		pui->UIEventToState_Proc(pui,pevent);
		// !!! SEND UICORE EVENT TO UI RECIPIENT.Event_Proc ONLY If user asked
		if( dispatch  )
		{
			#ifdef _NOUT_SYS_EVENT
			printf("\n				+--> Send Event to UI Recipient[0x%p].Event_Proc()",pui );
			printf("\n");
			#endif
			pui->Event_Proc(pui,pevent);
		}
		#ifdef _NOUT_SYS_EVENT
		else
		{
			printf("\n				+--> Event NOT SENT to UI Recipient[0x%p].Event_Proc()",pui );
			printf("\n");
		}
		#endif
	#ifdef _NOUT_SYS_EVENT

		printf("\n		+");
		printf("\n		|");
		printf("\n		+--> END Dispatch/Interception Process." );

	#endif

	}
}

// ------------------------------------------------------------------------------------------
// void NSendUICommandEvent
// ------------------------------------------------------------------------------------------
// Description :
//		!!! UICommand Event is ONLY Send trough "Event_Proc" for UI recipient and all its 
//		potential parent interceptors.
//
//		UICommand is NEVER send trough "UIEventToState_Proc" 
//		
//		!!! UIRecipient doesn't receive the event if one of its parent refuse to continue dispatching.
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSendUICommandEvent(const NEVENT *pevent)
{
	NErrorIf(!pevent,NERROR_NULL_VALUE);
	NErrorIf(pevent->Head.RecipientCast != NEVENT_RECIPIENT_CAST_UI, NERROR_EVENT_INCONSISTENT_RECIPIENT);
	// Check if event is "UI compatible"
	NErrorIf(	NGET_EVENT_MAIN(pevent->Type)!=_UICOMMAND, NERROR_EVENT_INCONSISTENT_TYPE	);
	// Check UI is Active ! ONLY Active UI (Visible + Enable) can receive _UICOMMAND Event !
	NErrorIf(	!NIsUIActive((NUI*)pevent->Head.pRecipient), NERROR_SYSTEM_CHECK	);
	
	// ----------------------------------------------------------------------------------------------------------
	// - Debug OutPut
	//
	#ifdef _NOUT_SYS_EVENT
	Nu32 local_debug_id;
	debug_id ++;
	local_debug_id = debug_id;
	printf("\n \n -------------------------------------------------------------------------------------------------- ");
	printf("\n[%d] + NSendUICommandEvent: ",local_debug_id);
	_debug_Printf_EVENT_NAME(pevent);
	#endif
	//
	// ----------------------------------------------------------------------------------------------------------

	// 1)	Check for a potential UI Event Blocking. 
	if( ISFLAG_ON(NEngineCoreFlags,FLAG_NENGINE_CORE_BLOCK_EVENT_TO_UI) )
	{
		// ------------------------------------
		// - Debug OutPut
		#ifdef _NOUT_SYS_EVENT
		printf("\n		|");
		printf("\n		+--> Non Send.Blocked By Engine.");
		printf("\n \n -------------------------------------------------------------------------------------------------- ");
		#endif
		// ------------------------------------
		return;
	}

	#ifdef _DEBUG	
	Nu32 dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
	#endif

	// Check UI address for using and Push them to StackSurvey to be aware of a potential UI delete

	NUI		*pui						= (NUI*)pevent->Head.pRecipient;
	Nu32	stacksize					= 0;

	// 2)	Push UI on the ClearWatchStack ...
	//		( this push is not added to the stacksize counter...it's normal, it will pop AFTER the parent interception process which use the stacsize )
	if( ISFLAG_ON(pui->Flags,FLAG_NUI_LISTEN_UICOMMAND_EVENT) )
	{
		NUIClearWatchStackPush( pui );
		stacksize ++;
	}


	// 3)	Is UI Parents chain intercept this Kind of event ?
	pui = (NUI*)pui->pParent;
	while(pui)
	{
		if( ISFLAG_ON(pui->Flags,FLAG_NUI_INTERCEPT_UICOMMAND_EVENT)  )
		{
			NUIClearWatchStackPush( pui );
			stacksize ++;
		}
		pui = (NUI*)pui->pParent;
	}

	// ----------------------------------------------------------------------------------------------------------
	// - Debug OutPut
	//
	#ifdef _NOUT_SYS_EVENT
	if( stacksize > 1 )
	{
		printf("\n		|");
		printf("\n		+--> %d UI Pushed to NUIClearWatchStack.", stacksize );
		printf("\n		+--> Start Dispatch/Interception Process: %d Parent Interception(s) planned before Sending to UI recipient.",(stacksize-1) );
	}
	else if( stacksize == 1)
	{
		printf("\n		|");
		printf("\n		+--> %d UI Pushed to NUIClearWatchStack.", stacksize );
		printf("\n		+--> Start Dispatch Process: No Parent Interception planned. Just UI Recipient Sent !" );
	}
	else // (stacksize == 0)
	{
		printf("\n		|");
		printf("\n		+--> %d UI Pushed to NUIClearWatchStack.", stacksize );
		printf("\n		+--> Event NOT SENT to UI Recipient[0x%p].Event_Proc()",pui );
		printf("\n		|	 Because UI doesn't Listen UICommand Event." );
		printf("\n		|	 and NOT INTERCEPT by UI Parent(s), because no UI parent intercept UICommand Event." );

	}
	#endif
	// -
	// ----------------------------------------------------------------------------------------------------------

	while(stacksize)
	{
		if( pui = NUIClearWatchStackPop() )
		{
			// ----------------------------------------------------------------------------------------------------------
			// - Debug OutPut
			//
			#ifdef _NOUT_SYS_EVENT
			printf("\n		|");
			printf("\n		+--> UI#%d[0x%p] pops from Stack.",stacksize,pui);
			printf("\n				+");
			printf("\n				|");

			if(pui->pName)
			{
				if( pui == (NUI*)pevent->Head.pRecipient )
				{
// 					printf("\n				+--> Listen By UIRecipient[0x%p][%s].UIEventToState_Proc()-->", pui, pui->pName );
					printf("\n				+--> Listen By UIRecipient[0x%p][%s].Event_Proc()-->", pui, pui->pName );
				}
				else
				{
// 					printf("\n				+--> Interception By Parent[0x%p][%s].UIEventToState_Proc()-->", pui, pui->pName );
					printf("\n				+--> Interception By Parent[0x%p][%s].Event_Proc()-->", pui, pui->pName );
				}
			}
			else
			{
				if( pui == (NUI*)pevent->Head.pRecipient )
				{
// 					printf("\n				+--> Listen By UIRecipient[0x%p].UIEventToState_Proc()-->", pui );
					printf("\n				+--> Listen By UIRecipient[0x%p].Event_Proc()-->", pui );
				}
				else
				{
// 					printf("\n				+--> Interception By Parent[0x%p].UIEventToState_Proc()-->", pui );
					printf("\n				+--> Interception By Parent[0x%p].Event_Proc()-->", pui );
				}
			}
			#endif
			// -
			// ----------------------------------------------------------------------------------------------------------
			// !!! UICommand Event NEVER send event trough "pparent->UIEventToState_Proc(pparent,pevent)" neither for parent that intercept neither for listen UI !!!
			// pui->UIEventToState_Proc(pui,pevent);
			// Stop Event Dispatching ( request by parent )
			if( ! NIS_UI_EVENT_RESULT_DISPATCH(pui->Event_Proc(pui,pevent)) )
			{
				#ifdef _NOUT_SYS_EVENT
				printf("return 'NUI_EVENT_RESULT_PROCESSED'" );
				if( pui != (NUI*)pevent->Head.pRecipient )
				{
					printf("\n				|");
					printf("\n				+--> INTERCEPTION CHAIN INTERRUPTED by USER" );
				}
				#endif

				// Reset stack size to initial value:
				NDecreaseUIClearWatchStackSize(stacksize-1);
				break;
			}
			#ifdef _NOUT_SYS_EVENT
			else
			{
				printf("return 'NUI_EVENT_RESULT_DISPATCH'" );
			}
			#endif
		}
		#ifdef _NOUT_SYS_EVENT
		else
		{
			printf("\n				|");
			printf("\n				+--> UI deleted during event Interception Processing.");
			printf("\n					 Dispatch/Interception Process Continuing.");
		}
		#endif

		stacksize--;
	}

	NErrorIf(NGetUIClearWatchStackSize()!=dbg_initial_clear_watch_stack_size, NERROR_SYSTEM_CHECK);

	// ----------------------------------------------------------------------------------------------------------
	// - Debug OutPut
	//
	#ifdef _NOUT_SYS_EVENT
	printf("\n		+");
	printf("\n		|");
	printf("\n		+--> END Dispatch/Interception Process." );
	#endif
	// -
	// ----------------------------------------------------------------------------------------------------------

}
// ------------------------------------------------------------------------------------------
// void NSendUINotifyEvent
// ------------------------------------------------------------------------------------------
// Description :
//		!!! UINotify Event (like UICommand) is ONLY Send trough "Event_Proc" for UI recipient and all its 
//		potential parent interceptors.
//
//		UINotify Event (like UICommand) is NEVER send trough "UIEventToState_Proc" 
//		
//		!!! UIRecipient doesn't receive the event if one of its parent refuse to continue dispatching.
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NSendUINotifyEvent(const NEVENT *pevent)
{
	NErrorIf(!pevent,NERROR_NULL_VALUE);
	NErrorIf(pevent->Head.RecipientCast != NEVENT_RECIPIENT_CAST_UI, NERROR_EVENT_INCONSISTENT_RECIPIENT);
	// Check if event is "UI compatible"
	NErrorIf(	NGET_EVENT_MAIN(pevent->Type)!=_UINOTIFY, NERROR_EVENT_INCONSISTENT_TYPE	);
	// ----------------------------------------------------------------------------------------------------------
	// - Debug OutPut
	//
	#ifdef _NOUT_SYS_EVENT
	Nu32 local_debug_id;
	debug_id ++;
	local_debug_id = debug_id;
	printf("\n \n -------------------------------------------------------------------------------------------------- ");
	printf("\n[%d] + NSendUINotifyEvent: ",local_debug_id);
	_debug_Printf_EVENT_NAME(pevent);
	#endif
	//
	// ----------------------------------------------------------------------------------------------------------

	// 1)	Check for a potential UI Event Blocking. 
	if( ISFLAG_ON(NEngineCoreFlags,FLAG_NENGINE_CORE_BLOCK_EVENT_TO_UI) )
	{
		// ------------------------------------
		// - Debug OutPut
		#ifdef _NOUT_SYS_EVENT
		printf("\n		|");
		printf("\n		+--> Non Send.Blocked By Engine.");
		printf("\n \n -------------------------------------------------------------------------------------------------- ");
		#endif
		// ------------------------------------
		return;
	}

	#ifdef _DEBUG	
	Nu32 dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
	#endif

	// Check UI address for using and Push them to StackSurvey to be aware of a potential UI delete

	NUI		*pui						= (NUI*)pevent->Head.pRecipient;
	Nu32	stacksize					= 0;

	// 2)	Push UI on the ClearWatchStack ...
	//		( this push is not added to the stacksize counter...it's normal, it will pop AFTER the parent interception process which use the stacsize )
	if( ISFLAG_ON(pui->Flags,FLAG_NUI_LISTEN_UINOTIFY_EVENT) )
	{
		NUIClearWatchStackPush( pui );
		stacksize ++;
	}


	// 3)	Is UI Parents chain intercept this Kind of event ?
	pui = (NUI*)pui->pParent;
	while(pui)
	{
		if( ISFLAG_ON(pui->Flags,FLAG_NUI_INTERCEPT_UINOTIFY_EVENT)  )
		{
			NUIClearWatchStackPush( pui );
			stacksize ++;
		}
		pui = (NUI*)pui->pParent;
	}

	// ----------------------------------------------------------------------------------------------------------
	// - Debug OutPut
	//
	#ifdef _NOUT_SYS_EVENT
	if( stacksize > 1 )
	{
		printf("\n		|");
		printf("\n		+--> %d UI Pushed to NUIClearWatchStack.", stacksize );
		printf("\n		+--> Start Dispatch/Interception Process: %d Parent Interception(s) planned before Sending to UI recipient.",(stacksize-1) );
	}
	else if( stacksize == 1)
	{
		printf("\n		|");
		printf("\n		+--> %d UI Pushed to NUIClearWatchStack.", stacksize );
		printf("\n		+--> Start Dispatch Process: No Parent Interception planned. Just UI Recipient Sent !" );
	}
	else // (stacksize == 0)
	{
		printf("\n		|");
		printf("\n		+--> %d UI Pushed to NUIClearWatchStack.", stacksize );
		printf("\n		+--> Event NOT SENT to UI Recipient[0x%p].Event_Proc()",pui );
		printf("\n		|	 Because UI doesn't Listen UICommand Event." );
		printf("\n		|	 and NOT INTERCEPT by UI Parent(s), because no UI parent intercept UINotify Event." );

	}
	#endif
	// -
	// ----------------------------------------------------------------------------------------------------------

	while(stacksize)
	{
		if( pui = NUIClearWatchStackPop() )
		{
			// ----------------------------------------------------------------------------------------------------------
			// - Debug OutPut
			//
			#ifdef _NOUT_SYS_EVENT
			printf("\n		|");
			printf("\n		+-->  UI#%d[0x%p] pops from Stack.",stacksize,pui);
			printf("\n				+");
			printf("\n				|");

			if(pui->pName)
			{
				if( pui == (NUI*)pevent->Head.pRecipient )
				{
// 					printf("\n				+--> Listen By UIRecipient[0x%p][%s].UIEventToState_Proc()-->", pui, pui->pName );
					printf("\n				+--> Listen By UIRecipient[0x%p][%s].Event_Proc()-->", pui, pui->pName );
				}
				else
				{
// 					printf("\n				+--> Interception By Parent[0x%p][%s].UIEventToState_Proc()-->", pui, pui->pName );
					printf("\n				+--> Interception By Parent[0x%p][%s].Event_Proc()-->", pui, pui->pName );
				}
			}
			else
			{
				if( pui == (NUI*)pevent->Head.pRecipient )
				{
// 					printf("\n				+--> Listen By UIRecipient[0x%p].UIEventToState_Proc()-->", pui );
					printf("\n				+--> Listen By UIRecipient[0x%p].Event_Proc()-->", pui );
				}
				else
				{
// 					printf("\n				+--> Interception By Parent[0x%p].UIEventToState_Proc()-->", pui );
					printf("\n				+--> Interception By Parent[0x%p].Event_Proc()-->", pui );
				}
			}
			#endif
			// -
			// ----------------------------------------------------------------------------------------------------------
			// !!! UI Notify Event NEVER send event trough "pparent->UIEventToState_Proc(pparent,pevent)" neither for parent that intercept neither for listen UI !!!
			// pui->UIEventToState_Proc(pui,pevent);
			// Stop Event Dispatching ( request by parent )
			if( ! NIS_UI_EVENT_RESULT_DISPATCH(pui->Event_Proc(pui,pevent)) )
			{
				#ifdef _NOUT_SYS_EVENT
				printf("return 'NUI_EVENT_RESULT_PROCESSED'" );
				if( pui != (NUI*)pevent->Head.pRecipient )
				{
					printf("\n				|");
					printf("\n				+--> INTERCEPTION CHAIN INTERRUPTED by USER" );
				}
				#endif

				// Reset stack size to initial value:
				NDecreaseUIClearWatchStackSize(stacksize-1);
				break;
			}
			#ifdef _NOUT_SYS_EVENT
			else
			{
				printf("return 'NUI_EVENT_RESULT_DISPATCH'" );
			}
			#endif
		}
		#ifdef _NOUT_SYS_EVENT
		else
		{
			printf("\n				|");
			printf("\n				+--> UI deleted during event Interception Processing.");
			printf("\n					 Dispatch/Interception Process Continuing.");
		}
		#endif

		stacksize--;
	}

	NErrorIf(NGetUIClearWatchStackSize()!=dbg_initial_clear_watch_stack_size, NERROR_SYSTEM_CHECK);

	// ----------------------------------------------------------------------------------------------------------
	// - Debug OutPut
	//
#ifdef _NOUT_SYS_EVENT
	printf("\n		+");
	printf("\n		|");
	printf("\n		+--> END Dispatch/Interception Process." );
#endif
	// -
	// ----------------------------------------------------------------------------------------------------------

}


/*
void NSendUIEvent(const NEVENT *pevent,const NUI *psendby)
{
	NErrorIf(!pevent,NERROR_NULL_VALUE);
	NErrorIf(pevent->Head.pRecipient == psendby, NERROR_EVENT_INCONSISTENT_RECIPIENT);
	NErrorIf(pevent->Head.RecipientCast != NEVENT_RECIPIENT_CAST_UI, NERROR_EVENT_INCONSISTENT_RECIPIENT);
	
	// Check if event is "UI compatible" ---------------------->
	NErrorIf(	NGET_EVENT_MAIN(pevent->Type)==_SYSTEM, NERROR_EVENT_INCONSISTENT_TYPE	);
	NErrorIf(	NGET_EVENT_MAIN(pevent->Type)==_TOUCH,  NERROR_EVENT_INCONSISTENT_TYPE	);


	#ifdef _DEBUG
	Nu32 local_debug_id;
	debug_id ++;
	local_debug_id = debug_id;
	printf("\n \n ------------------------------------------------- \n[%d] - NSendUIEvent: ",local_debug_id);
	_debug_Printf_EVENT_NAME(pevent);
	#endif

	// Check for a potential UI Event Blocking. 
	if( ISFLAG_ON(NEngineCoreFlags,FLAG_NENGINE_CORE_BLOCK_EVENT_TO_UI) )
	{
		#ifdef _DEBUG
		printf(".Non Send.Blocked By Engine.");
		#endif
		return;
	}
	
	// Check UI address for using and Push them to StackSurvey to be aware of a potential UI delete
	#ifdef _DEBUG	
	Nu32 dbg_initial_clear_watch_stack_size = NGetUIClearWatchStackSize();
	#endif

	Nu32	stacksize					= 0;
	NUI		*pui						= (NUI*)pevent->Head.pRecipient;

	// "listen_interception_flags" retrieve the flags associated with the event type. 
	// An UI must have the right flags set to ON to receive the event (listen and intercept).
	// These flags are a combination of listen, intercept and activation flags.
	Nu32	listen_interception_flags	= _get_uievent_listen_and_interception_flags(pevent->Type);
	// "listen_flags" only keep  the flags use to determine if an ui is able to listen the event.
	Nu32	listen_flags				= listen_interception_flags&(MASK_NSendUIEvent_LISTEN_EVENT|MASK_NUI_ACTIVE);

	// Is UI Listen this kind of Event ?
	if( FLAGS_TEST(pui->Flags,listen_flags,listen_flags) )
	{
		NUIClearWatchStackPush( pui ); // --> stacksize	= 1;
		stacksize++;
	}	
	
	// Is UI Parents chain intercept this Kind of event ?
	// keep  the flags use to determine if an ui is able to intercept the event.
	listen_interception_flags = listen_interception_flags&(MASK_NSendUIEvent_INTERCEPT_EVENT|MASK_NUI_ACTIVE);
	pui = (NUI*)pui->pParent;
	while(pui)
	{
		if( FLAGS_TEST(pui->Flags,listen_interception_flags,listen_interception_flags)  && (pui != psendby)  )
		{
			NUIClearWatchStackPush( pui );
			stacksize ++;
		}
		pui = (NUI*)pui->pParent;
	}
		
	
#ifdef _DEBUG
	if( stacksize )
		printf(" +--> Sent Road Map: %d Interception(s) by Parent(s) before Sending to UI recipient.",(stacksize-1) );
	else
		printf(" +--> Sent Road Map: NO Interception by Parent AND no Sending to UI recipient. (Is not Listening this kind of Event)" );
#endif

	while(stacksize)
	{
		if( pui = NUIClearWatchStackPop() )
		{
			#ifdef _DEBUG
			
			if( pui == (NUI*)pevent->Head.pRecipient )
				printf("\n[%d] - Send To UI Recipient (0x%X)----------------->",local_debug_id, (Nu32)pui );
			else
				printf("\n[%d] - Interception By UI Recipient Parent (0x%X)-->",local_debug_id, (Nu32)pui );
			#endif

			pui->UIEventToState_Proc(pui,pevent);
			// Stop Event Dispatching ( request by parent )
			if( ! NIS_UI_EVENT_RESULT_DISPATCH(pui->Event_Proc(pui,pevent)) )
			{
				#ifdef _DEBUG
				printf("return 'NUI_EVENT_RESULT_PROCESSED'" );
				if(stacksize!=1)
					printf("n[%d] - SENDING CHAIN INTERRUPTED by USER before UI RECIPIENT SEND.",local_debug_id);
				#endif

				// Reset stack size to initial value:
				NDecreaseUIClearWatchStackSize(stacksize-1);
				break;
			}
			#ifdef _DEBUG
			else
			{
				printf("return 'NUI_EVENT_RESULT_DISPATCH'" );
			}
			#endif
		}
		#ifdef _DEBUG
		else
		{
			printf("\n[%d] - UI deleted during event Processing. Send Aborted.",local_debug_id );
		}
		#endif

		stacksize--;
	}

	#ifdef _DEBUG
	NErrorIf(NGetUIClearWatchStackSize()!=dbg_initial_clear_watch_stack_size, NERROR_SYSTEM_CHECK);
	printf("\n[%d] - Sending Process End.",local_debug_id );
	#endif
}
*/

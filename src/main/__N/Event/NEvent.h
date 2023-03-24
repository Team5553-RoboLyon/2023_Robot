#ifndef __N_EVENT_H
#define __N_EVENT_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NEvent.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NType.h"
#include "../NScreenPoint.h"
#include "../Miscellaneous/NColor.h"
#include "../Touch/NTouch.h"
#include "./NEventKeyCode.h"



#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef struct	NUI								NUI;

// ***************************************************************************************
// **					  Functions prototypes and Structures							**
// ***************************************************************************************

// Delayed Events
#define CONSTANT_DELAYED_UI_EVENT_ARRAY_INITIAL_CAPACITY	3

// 16 BITS EVENT TYPE
// |___________|___________|___________|___________|
// |15|14|13|12|11|10|09|08|07|06|05|04|03|02|01|00|
// |				 |							   |
// |   MAIN TYPE	 |			SUBTYPE			   |
// |				 |	  (specify Main Type)	   |
// | 0 to 63(incl.)	 |	  0 to 1023 (included)	   |
// |-----------------|-----------------------------|

#define NMAKE_EVENT(_main,_sub)			( ( ((_main)&0x3F)<<10 )|( (_sub)&0x3FF ) )
#define NGET_EVENT_MAIN(_event_type)	( ((_event_type)>>10)&0x3F )
#define NGET_EVENT_SUB(_event_type)		( (_event_type)&0x3FF )

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + N_EVENT Main Type
// +
#define _SYSTEM							0	// Events from the System, 'NOEVENT' included
#define _TOUCH							1	// Events generated when user Touch the screen							( 4: NTOUCH_START/NTOUCH_MOVE/NTOUCH_END/NTOUCH_CANCEL )		
#define _UICORE							2	// Events generated when a UI is created/destroyed/show/hide/enable/disable ( 6: NUI_SETUP/NUI_CLEAR/NUI_SHOW/NUI_HIDE/NUI_ENABLE/NUI_DISABLE )		
#define _UICOMMAND						3	// Command Events generated by the UI controllers to deliver the data that they are supposed to work with ( 11: NCOMMAND_PUSHBUTTON_DOWN/... )	
#define _UINOTIFY						4	// Notification Events generated by external objects ( like UIANIMPLAYER ) to UI that they are linked with.
#define _AD_REWARDED_VIDEO				5	// Notification Events generated by External Advertising System
#define _AD_INTERSTITIAL				6	// Notification Events generated by External Advertising System
#define _AD_BANNER						7	// Notification Events generated by External Advertising System
#define	_KEY_DOWN						8	// Events generated when a keyboard ( virtual or not )key is pressed
#define	_KEY_UP						    9	// Events generated when a keyboard ( virtual or not )key is released


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + N_EVENT SubType
// +
// +
// +-+ _KEY Subtypes
// +
// For KEY Event, because they are numerous check yourself with "NEventKeyCode.h"

// +-+ _SYSTEM Subtypes
// +
#define _NOSUB							0	// To Build 'NSYS_NOEVENT' ( Never used ! )
#define _NOT_TRANSLATED					5
#define _QUIT							6
//#define _RETURN_BACK					7

// +
// +-+ _AD Subtypes
// +											| Banner | Interstitial	| Rewarded  |
#define _AD_LOADED						1	//	|	X	 |		X		|	  X		|	AD is LOADED ready to be displayed
#define _AD_OPENED						2	//	|	X	 |		X		|	  X		|	AD is opened ( displayed )
#define _AD_CLOSED						3	//	|	X	 |		X		|	  X		|   AD is closed ( out of screen )
#define _AD_LEFT_APPLICATION			4	//	|	X	 |		X		|	  X		|	User Touch AD and has left the Application...
#define _AD_FAILED_TO_LOAD				5	//	|	X	 |		X		|	  X		|	AD Loading encounter an Error
#define _AD_STARTED						6	//	|		 |				|	  X		|	Rewarded Video Only. Video Started.
#define _AD_REWARDED					7	//	|		 |				|	  X		|	Rewarded Video Only. Add should be Rewarded.
//#define _AD_FAILED_TO_SHOW				8	//	|	X	 |		X		|	  X		|	AD Show failed ... maybe its not a good idea to do this
// +
// +-+ _TOUCH
// +
#define _START							4	// |											| = NTOUCH_PHASE_START  = 4
#define _MOVE							3	// |_ !!! Correlate with N_TOUCH_PHASE_xxx !!! _| = NTOUCH_PHASE_MOVE	= 3
#define _END							2	// |	    correlation not used yet 			| = NTOUCH_PHASE_END	= 2	
#define _CANCEL							1	// |		by engine and may be changed !		| = NTOUCH_PHASE_CANCEL	= 1

// #define _START_X						?	// |   
// #define _MOVE_X						?	// |
// #define _END_X						?	// |


// +
// +-+ _UICORE Subtypes
// +
#define _SETUP							8
#define _CLEAR							9
#define _SHOW							10
#define _HIDE							11
#define _ENABLE							12
#define _DISABLE						13	

// +
// +-+ _UINOTIFY Subtypes
// +
#define _UITHNANIM_START				14		
#define _UITHNANIM_KEY					15		
#define _UITHNANIM_REPEAT				16		
#define _UITHNANIM_END					17		
#define _UITHNANIM_STOP					18		

// +
// +-+ _UICOMMAND and _UICANCEL Subtypes
// +
#define _PUSHBUTTON_DOWN				19		
#define _PUSHBUTTON_UP					20		
#define _PUSHBUTTON_CANCEL_DOWN			21		
#define _UICUSTOM_DOWN					22		
#define _UICUSTOM_UP					23		
#define _UICUSTOM_CANCEL_DOWN			24		

#define _VALUP_NF32						25		
#define _VALUP_NS32						26		
#define _CHECKBOX_CHECK					27		
#define _CHECKBOX_UNCHECK				28		
#define _CHECKBOX_TOGGLE				29		
#define _COLORCIRCLE_RGBA8				30		
#define _COLORCIRCLE_RGBA32				31		

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + N_EVENT Type (Full Type = Main Type + SubType)
// +

// KEY DOWN / KEY UP
// Because they are a lot of them, N doesn't Define a specific "name" for each of them,
// and let user use NMAKE_EVENT(_KEY_DOWN,_KEYCODE_xxx) or  NMAKE_EVENT(_KEY_UP,_KEYCODE_xxx) instead
// But, some of them still have a definition:
#define NKEY_DOW_BACK                       NMAKE_EVENT( _KEY_DOWN,		_KEYCODE_BACK)


//
// SYSTEM
// ... 2 pure system events without specific NEVENT_xxx typedef struct...
#define NSYS_NOEVENT						0	// Never Used !!! It also could be written like this : NMAKE_EVENT( 0,_SYSTEM,_NOSUB) which is ZERO !
#define NSYS_NOT_TRANSLATED					NMAKE_EVENT( _SYSTEM,		_NOT_TRANSLATED)

#define NSYS_QUIT							NMAKE_EVENT( _SYSTEM,		_QUIT)
//#define NSYS_RETURN_BACK					NMAKE_EVENT( _SYSTEM,		_RETURN_BACK) deprecated replaced by NKEY_DOW_BACK

// ADD
#define NAD_BANNER_LOADED					NMAKE_EVENT( _AD_BANNER,		_AD_LOADED)
#define NAD_BANNER_OPENED					NMAKE_EVENT( _AD_BANNER,		_AD_OPENED)
#define NAD_BANNER_CLOSED					NMAKE_EVENT( _AD_BANNER,		_AD_CLOSED)
#define NAD_BANNER_LEFT_APPLICATION			NMAKE_EVENT( _AD_BANNER,		_AD_LEFT_APPLICATION)
#define NAD_BANNER_FAILED_TO_LOAD			NMAKE_EVENT( _AD_BANNER,		_AD_FAILED_TO_LOAD)

#define NAD_INTERSTITIAL_LOADED				NMAKE_EVENT( _AD_INTERSTITIAL,	_AD_LOADED)
#define NAD_INTERSTITIAL_OPENED				NMAKE_EVENT( _AD_INTERSTITIAL,	_AD_OPENED)
#define NAD_INTERSTITIAL_CLOSED				NMAKE_EVENT( _AD_INTERSTITIAL,	_AD_CLOSED)
#define NAD_INTERSTITIAL_LEFT_APPLICATION	NMAKE_EVENT( _AD_INTERSTITIAL,	_AD_LEFT_APPLICATION)
#define NAD_INTERSTITIAL_FAILED_TO_LOAD		NMAKE_EVENT( _AD_INTERSTITIAL,	_AD_FAILED_TO_LOAD)

#define NAD_REWARDED_VIDEO_LOADED			NMAKE_EVENT( _AD_REWARDED_VIDEO,_AD_LOADED)
#define NAD_REWARDED_VIDEO_OPENED			NMAKE_EVENT( _AD_REWARDED_VIDEO,_AD_OPENED)
#define NAD_REWARDED_VIDEO_CLOSED			NMAKE_EVENT( _AD_REWARDED_VIDEO,_AD_CLOSED)
#define NAD_REWARDED_VIDEO_LEFT_APPLICATION	NMAKE_EVENT( _AD_REWARDED_VIDEO,_AD_LEFT_APPLICATION)
#define NAD_REWARDED_VIDEO_FAILED_TO_LOAD	NMAKE_EVENT( _AD_REWARDED_VIDEO,_AD_FAILED_TO_LOAD)
#define NAD_REWARDED_VIDEO_STARTED			NMAKE_EVENT( _AD_REWARDED_VIDEO,_AD_STARTED)
#define NAD_REWARDED_VIDEO_REWARDED			NMAKE_EVENT( _AD_REWARDED_VIDEO,_AD_REWARDED)

// TOUCH
#define NTOUCH_START						NMAKE_EVENT( _TOUCH,		_START)
#define NTOUCH_MOVE							NMAKE_EVENT( _TOUCH,		_MOVE)
#define NTOUCH_END							NMAKE_EVENT( _TOUCH,		_END)
#define NTOUCH_CANCEL						NMAKE_EVENT( _TOUCH,		_CANCEL)

// UI CORE
#define NUI_SETUP							NMAKE_EVENT( _UICORE,		_SETUP)
#define NUI_CLEAR							NMAKE_EVENT( _UICORE,		_CLEAR)
#define NUI_SHOW							NMAKE_EVENT( _UICORE,		_SHOW)
#define NUI_HIDE							NMAKE_EVENT( _UICORE,		_HIDE)
#define NUI_ENABLE							NMAKE_EVENT( _UICORE,		_ENABLE)
#define NUI_DISABLE							NMAKE_EVENT( _UICORE,		_DISABLE)

// UI NOTIFY
#define NNOTIFY_UITHNANIM_START				NMAKE_EVENT( _UINOTIFY,		_UITHNANIM_START)
#define NNOTIFY_UITHNANIM_KEY				NMAKE_EVENT( _UINOTIFY,		_UITHNANIM_KEY)
#define NNOTIFY_UITHNANIM_REPEAT			NMAKE_EVENT( _UINOTIFY,		_UITHNANIM_REPEAT)
#define NNOTIFY_UITHNANIM_END				NMAKE_EVENT( _UINOTIFY,		_UITHNANIM_END)
#define NNOTIFY_UITHNANIM_STOP				NMAKE_EVENT( _UINOTIFY,		_UITHNANIM_STOP)

// UI COMMAND ( ... AND CANCEL )
#define NCOMMAND_PUSHBUTTON_DOWN			NMAKE_EVENT( _UICOMMAND,	_PUSHBUTTON_DOWN)
#define NCOMMAND_PUSHBUTTON_UP				NMAKE_EVENT( _UICOMMAND,	_PUSHBUTTON_UP)
#define NCOMMAND_PUSHBUTTON_CANCEL_DOWN		NMAKE_EVENT( _UICOMMAND,	_PUSHBUTTON_CANCEL_DOWN)
//		N_CANCEL_PUSHBUTTON_UP				doesn't exist... it's normal there is nothing to CANCEL after a UP because UI loses FOCUS !

#define NCOMMAND_UICUSTOM_DOWN				NMAKE_EVENT( _UICOMMAND,	_UICUSTOM_DOWN)
#define NCOMMAND_UICUSTOM_UP				NMAKE_EVENT( _UICOMMAND,	_UICUSTOM_UP)
#define NCOMMAND_UICUSTOM_CANCEL_DOWN		NMAKE_EVENT( _UICOMMAND,	_UICUSTOM_CANCEL_DOWN)
//		N_CANCEL_UICUSTOM_UP				doesn't exist... it's normal there is nothing to CANCEL after a UP because UI loses FOCUS !

#define NCOMMAND_VALUPDATER_NF32			NMAKE_EVENT( _UICOMMAND,	_VALUP_NF32)
#define NCOMMAND_VALUPDATER_NS32			NMAKE_EVENT( _UICOMMAND,	_VALUP_NS32)
//		N_CANCEL_VALUPDATER_NF32			doesn't exist... maybe it should be ???
//		N_CANCEL_VALUPDATER_NS32			doesn't exist... maybe it should be ???

#define NCOMMAND_CHECKBOX_CHECK				NMAKE_EVENT( _UICOMMAND,	_CHECKBOX_CHECK)
#define NCOMMAND_CHECKBOX_UNCHECK			NMAKE_EVENT( _UICOMMAND,	_CHECKBOX_UNCHECK)
#define NCOMMAND_CHECKBOX_TOGGLE			NMAKE_EVENT( _UICOMMAND,	_CHECKBOX_TOGGLE)
//		N_CANCEL_CHECKBOX_TOGGLE			doesn't exist... maybe it should be ??? ( it will be available to cancel  Check, UnCheck and ... Toggle of course ) 

#define NCOMMAND_COLORCIRCLE_RGBA8			NMAKE_EVENT( _UICOMMAND,	_COLORCIRCLE_RGBA8)
#define NCOMMAND_COLORCIRCLE_RGBA32			NMAKE_EVENT( _UICOMMAND,	_COLORCIRCLE_RGBA32)
//		N_CANCEL_COLORCIRCLE_RGBA8			doesn't exist... maybe it should be ???
//		N_CANCEL_COLORCIRCLE_RGBA32			doesn't exist... maybe it should be ???


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + NUI_EVENT_RESULT
// +
// + The common way is, for Event handle, to return one of the 4 predefine NUI_EVENT_RESULT ( see bellow ).
// + But is possible to "decorate" the NUI_EVENT_RESULT 32 bits value by using the HIGHWORD to store a user Nu16 value.
// + It's not used yet but it could be by some controllers to allow user to return some infos about what he did in the EventHandle for a specific NEVENT ( NCOMMAND )
// + The 4 "common" NUI_EVENT_RESULT return by Event_Handle are:
// +
// +			Return by DefaultEvent_Handle, inside Event_Handle, for all the cases user doesn't want to manage ( default: return DefaultEvent_Handle(pui,pevent);)
// +			NUI_EVENT_RESULT_NONPROCESSED_PUBLIC		Event_Handle handle's an event but, doesn't make any change on it's side AND accepts parent listening for that EVENT. ( !!! NEVENT itself MUST BE PUBLIC, if not ... CRASH in debug mode, nothing in release BUT N_EVENT will still PRIVATE !!! )
// +			NUI_EVENT_RESULT_NONPROCESSED_PRIVATE		Event_Handle handle's an event but, doesn't make any change on it's side AND accepts parent listening for that EVENT. ( !!! NEVENT itself may be PUBLIC or PRIVATE, both of them may be turn  to PRIVATE !!! )
// +
// +			Return by User, inside Event_Handle, at the end of each handled cases:
// +			NUI_EVENT_RESULT_PROCESSED_PUBLIC			Event_Handle handle's an event, AND accepts parent listening for that EVENT. ( !!! NEVENT itself MUST BE PUBLIC, if not ... CRASH in debug mode, nothing in release BUT N_EVENT will still PRIVATE !!! )
// +			NUI_EVENT_RESULT_PROCESSED_PRIVATE		Event_Handle handle's an event but, AND doesn't accept parent listening for that EVENT. ( !!! NEVENT itself may be PUBLIC or PRIVATE, both of them may be turn  to PRIVATE !!! )
// +
// + 
typedef Nu32	NUI_EVENT_RESULT;
#define NUI_EVENT_RESULT_PROCESSED								0
#define NUI_EVENT_RESULT_DISPATCH								1
#define NUI_EVENT_RESULT_NON_SENT								0xFFFF

#define NMAKE_UI_EVENT_RESULT(core,usernu16)					NMAKELONG(core,usernu16)
#define NIS_UI_EVENT_RESULT_PROCESSED(uiresult)					(NLOWORD(uiresult)==NUI_EVENT_RESULT_PROCESSED)
#define NIS_UI_EVENT_RESULT_DISPATCH(uiresult)					(NLOWORD(uiresult)==NUI_EVENT_RESULT_DISPATCH)
#define NIS_UI_EVENT_RESULT_NON_SENT(uiresult)					(NLOWORD(uiresult)==NUI_EVENT_RESULT_NON_SENT)

#define NGET_UI_EVENT_RESULT_USER_NU16(uiresult)				NHIWORD(uiresult)

// MACROS definition
#define NIS_TOUCH_CANCEL_ALL(pev)								((pev->Type == NTOUCH_CANCEL)&&(!pev->Touch.TouchSequence.TouchCount)) 
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// TODO
// Create something like message box but in N system ... so the idea is do what you want to do in your event function in response to N_CONFIRMATION_OK instead of the initial command
// and from your initial command call something like NUIMessageBox("your message",pui_your_current_frame, pyour_current_event):
// pui_your_current_frame: because when your are going to touch OK or cancel, messagebox will send a NCONFIRMATION_OK or CANCEL to your UI !
// pyour_current_event: because this event will be copied and resend to you include into the NCONFIRMATION event like a parent to help you to regognize, the confirmation of what ... copy ?
// ... imagine your UI ask several different confirmation in response of different event... you will be happy to remember them no ?
// #define N_CONFIRMATION_OK
// #define N_CONFIRMATION_CANCEL
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define NEVENT_RECIPIENT_CAST_NULL			0
#define NEVENT_RECIPIENT_CAST_UI			1
#define NEVENT_RECIPIENT_CAST_GAMESTATE		2
#ifdef _NEDITOR
#define NEVENT_RECIPIENT_CAST_NEDITOR_TOOL	3
#endif

#define NEVENT_PARAM_NULL										0
#define NEVENT_PARAM_TOUCH_START__TOUCH_MOVE_IN					1	// With N_TOUCH_CANCEL_ALL only - TOUCH CANCEL ALL due to a user call ( thanks to a ...CancelAll... function call )
#define NEVENT_PARAM_TOUCH_CANCEL__TOUCH_MOVE_OUT				2	// With N_TOUCH_CANCEL_ALL only - TOUCH CANCEL ALL due to a user call ( thanks to a ...CancelAll... function call )
#define NEVENT_PARAM_TOUCH_CANCEL__ALL							3	// With N_TOUCH_CANCEL_ALL only - TOUCH CANCEL ALL due to a user call ( thanks to a ...CancelAll... function call )


typedef struct
{
	Nu16		Type;
	Nu8			Param;				
	Nu8			RecipientCast;		// NEVENT_RECIPIENT_CAST_UI // NEVENT_RECIPIENT_CAST_GAMESTATE
	void		*pRecipient;		// pointer to UI or GAMESTATE which is the Recipient of this Event. ( useful for listeners to define if the Event is for them or for listen children )
}NEVENT_HEAD;

/*
typedef struct
{
	Nu16		Type;				//| -------------->	NSYS_QUIT
	Nu8			Param;				//|_NEVENT_HEAD	
	Nu8			RecipientCast;		//|
	void		*pRecipient;		//|
}NEVENT_QUIT;

typedef struct
{
	Nu16		Type;				//| -------------->	NUI_SETUP/NUI_CLEAR/NUI_SHOW/NUI_HIDE/NUI_ENABLE/NUI_DISABLE
	Nu8			Param;				//|_NEVENT_HEAD	
	Nu8			RecipientCast;		//|
	void		*pRecipient;		//|
}NEVENT_UI_CORE;

typedef struct
{
	Nu16		Type;				//| -------------->	NCOMMAND_PUSHBUTTON_DOWN/NCOMMAND_PUSHBUTTON_UP/N_CANCEL_PUSHBUTTON_DOWN
	Nu8			Param;				//|_NEVENT_HEAD	
	Nu8			RecipientCast;		//|
	void		*pRecipient;		//|
}NEVENT_COMMAND_PUSHBUTTON;

typedef struct
{
	Nu16		Type;				//| -------------->	NCOMMAND_UICUSTOM_DOWN/NCOMMAND_UICUSTOM_UP/N_CANCEL_UICUSTOM_DOWN
	Nu8			Param;				//|_NEVENT_HEAD	
	Nu8			RecipientCast;		//|
	void		*pRecipient;		//|
}NEVENT_COMMAND_UICUSTOM;

*/
typedef NEVENT_HEAD		NEVENT_AD;
typedef NEVENT_HEAD		NEVENT_RETURN_BACK;
typedef NEVENT_HEAD		NEVENT_QUIT;
typedef NEVENT_HEAD		NEVENT_UI_CORE;
typedef NEVENT_HEAD		NEVENT_COMMAND_PUSHBUTTON;
typedef NEVENT_HEAD		NEVENT_COMMAND_UICUSTOM;


typedef struct
{
	Nu16		Type;				//| -------------->	NTOUCH_START / NTOUCH_MOVE / NTOUCH_END / NTOUCH_CANCEL
	Nu8			Param;				//|_NEVENT_HEAD	--> NEVENT_PARAM_NULL, NEVENT_PARAM_TOUCH_CANCEL_ALL
	Nu8			RecipientCast;		//|
	void		*pRecipient;		//|

	NTOUCH_SEQUENCE	TouchSequence;	//  --------------> Touch Index Sequence of the touch(es) concerned by the event ( one or several touches may be START or MOVE or END or CANCEL at the same time ... )
}NEVENT_TOUCH;

typedef struct
{
	Nu16		Type;				//| -------------->	NNOTIFY_UITHNANIM_START/NNOTIFY_UITHNANIM_KEY/NNOTIFY_UITHNANIM_END/NNOTIFY_UITHNANIM_STOP
	Nu8			Param;				//|_NEVENT_HEAD	--> Current 'KeyB' index ( KeyB is the targeted Key ).
	Nu8			RecipientCast;		//|
	void		*pRecipient;		//|

	Nu32		UserID;				//  --------------> Specific UI THN ANIMATION User ID, defined by user at animation creation. 
}NEVENT_NOTIFY_UITHNANIM;

typedef struct
{
	Nu16		Type;				//| -------------->	NCOMMAND_CHECKBOX_CHECK/NCOMMAND_CHECKBOX_UNCHECK/NCOMMAND_CHECKBOX_TOGGLE
	Nu8			Param;				//|_NEVENT_HEAD	
	Nu8			RecipientCast;		//|
	void		*pRecipient;		//|

	Nbool		bCheck;
}NEVENT_COMMAND_CHECKBOX;

typedef struct
{
	Nu16		Type;				//| -------------->	NCOMMAND_VALUPDATER_NF32
	Nu8			Param;				//|_NEVENT_HEAD	
	Nu8			RecipientCast;		//|
	void		*pRecipient;		//|

	Nf32		Value_Nf32;
}NEVENT_COMMAND_VALUPDATERF;
// VALUE UPDATER event structure
typedef struct
{
	Nu16		Type;				//| -------------->	NCOMMAND_VALUPDATER_NS32
	Nu8			Param;				//|_NEVENT_HEAD	
	Nu8			RecipientCast;		//|
	void		*pRecipient;		//|

	Ns32		Value_Ns32;
}NEVENT_COMMAND_VALUPDATERI;

typedef struct
{
	Nu16		Type;				//| -------------->	NCOMMAND_COLORCIRCLE_RGBA8
	Nu8			Param;				//|_NEVENT_HEAD	
	Nu8			RecipientCast;		//|
	void		*pRecipient;		//|

	Nu32		Color;
}NEVENT_COMMAND_COLORCIRCLE_RGBA8;

typedef struct
{
	Nu16		Type;				//| -------------->	NCOMMAND_COLORCIRCLE_RGBA32
	Nu8			Param;				//|_NEVENT_HEAD	
	Nu8			RecipientCast;		//|
	void		*pRecipient;		//|

	NCOLOR		Color;
}NEVENT_COMMAND_COLORCIRCLE_RGBA32;

// General event structure
typedef union
{
	Nu16								Type;
	NEVENT_HEAD							Head;
	//	---------------------------------------
	NEVENT_AD							AD;						//	| Event for Game State Only.
	//	---------------------------------------
	NEVENT_RETURN_BACK					Return_Back;			//	| Event for Game State Only.
	NEVENT_QUIT							Quit;					//	| Event for Game State and interpreted as N_TOUCH_CANCEL (all) for UI.
	//	---------------------------------------					
	NEVENT_TOUCH						Touch;					//	| Events for UIs and GAMESTATEs.
	//	---------------------------------------					
	NEVENT_UI_CORE						UICore;					//	|
	//	---------------------------------------					//  |
	NEVENT_NOTIFY_UITHNANIM				UIThnAnim;				//  |
	//	---------------------------------------					//  |
	NEVENT_COMMAND_CHECKBOX				CheckBox;				//	|
	NEVENT_COMMAND_PUSHBUTTON			PushButton;				//	|
	NEVENT_COMMAND_UICUSTOM				UICustom;				//	| Events for UIs Only.
	NEVENT_COMMAND_VALUPDATERF			ValUpdaterf;			//	|
	NEVENT_COMMAND_VALUPDATERI			ValUpdateri;			//	|
	NEVENT_COMMAND_COLORCIRCLE_RGBA8	ColorCircle_RGBA8;		//	|
	NEVENT_COMMAND_COLORCIRCLE_RGBA32	ColorCircle_RGBA32;		//	|
}NEVENT;


// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
//void		NSendUIEvent(const NEVENT *pevent,const NUI *psendby);
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + 
// + PRIVATE USE
// +
#define NFillupEventHead(ev,type,_param,cast,precepi)	\
	ev.Head.Type			= type;						\
	ev.Head.Param			= _param;					\
	ev.Head.RecipientCast	= cast;						\
	ev.Head.pRecipient		= precepi;


// Send Event
void		NSendUICoreEvent(const NEVENT *pevent);
void		NSendUICommandEvent(const NEVENT *pevent);
void		NSendUINotifyEvent(const NEVENT *pevent);

// Delayed Event management
void		NDispatchDelayedUIEvent();
void		NPostDelayedUIEvent(const NEVENT *pevent);
void		NInitDelayedUIEventArray();
void		NQuitDelayedUIEventArray();

void		NDelayedUIEventArray_UIClear_Notification(const NUI *pui);
void		NDelayedUIEventArray_UIDisable_Notification(const NUI *pui);
#define		NDelayedUIEventArray_UIHide_Notification(pui)	NDelayedUIEventArray_UIDisable_Notification(pui)

// Queued Event management
void		NDispatchEvent( const NEVENT *pevent );

#ifdef _NWINDOWS
Nu8			NCreateNEventFromMSG( const MSG *pmsg );
#endif
/*
#ifdef _NWINDOWS
Nu8			NWindowsPollEvent(NEVENT *pevent);
#endif
*/

/*
#if defined _NIOS || defined _NANDROID
NEVENT*		NPollEvent();
#endif
*/
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NEVENT_H 


#ifndef __NTOUCH_H
#define __NTOUCH_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NTouch.h											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NCStandard.h"
#include "../NType.h"
#include "../Maths/NVec3f32.h"
#include "../NScreenPoint.h"
#include "../NUsualCoordinates.h"
#include "../GameLoop/NGameStatesUID.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NTOUCH CONSTANT ( Number Max of Touch managed by Engine and Stream Buffer Size )
// + !!! Important !!!
// + These 2 CONSTANTS may be updated BUT ...
// +		1)	They are both Nu8, so ... 
// +					255 Max for CONSTANT_NTOUCH_STREAM_BUFFER_CAPACITY
// +					31	Max for CONSTANT_NTOUCH_COUNT_MAX ( because 31 * 8 = 248, ... and 32 * 8 = 256 which is > 255 ! )
// +
// +		2)	For a perfect memory alignment, (CONSTANT_NTOUCH_COUNT_MAX + 1)  should be multiple of 4. 
// +			Possible values: 3(+1=4),7(+1=8),11(+1=12), 15(+1=16), 19(+1=20),23(+1=24),27(+1=28),31(+1=32)
// +
// +		3)	CONSTANT_NTOUCH_STREAM_BUFFER_CAPACITY should be able to store simultaneously the Maximum number of manages touches
// +			for 8 Events (also stored simultaneously . This constraint is absolutely arbitrary but seems cool ;)
// +
#define CONSTANT_NTOUCH_COUNT_MAX									11		
#define CONSTANT_NTOUCH_STREAM_BUFFER_CAPACITY						CONSTANT_NTOUCH_COUNT_MAX*8 // 8 events with all touches updated for each !

#ifdef _NWINDOWS    
#define CONSTANT_NTOUCH_SYSTEMID__WINDOWS_MOUSE							1
#define CONSTANT_NTOUCH_SYSTEMID__WINDOWS_TOUCH_EMULATION_KEY_ROOT		2	
#define NMAKE_TOUCH_SYSTEMID_FROM_KEY(sdl_key,instance)					NMAKELONG(_SafeNu32ToNu16(sdl_key + CONSTANT_NTOUCH_SYSTEMID__WINDOWS_TOUCH_EMULATION_KEY_ROOT),instance)
#endif
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NTOUCH_PHASE
#define NTOUCH_PHASE_NULL							0 // Never Used ! 
#define NTOUCH_PHASE_CANCEL							1 // |										| _CANCEL	= 1
#define NTOUCH_PHASE_END							2 // |_ Correlate with EVENT Touch Subtype _| _END		= 2		
#define NTOUCH_PHASE_MOVE							3 // |		correlation not used yet 		| _MOVE		= 3
#define NTOUCH_PHASE_START							4 // |		by engine and may be changed !	| _START	= 4

#define NIS_TOUCH_PHASE_ACTIVE(phase)				((phase)>2)			// START or MOVE
#define NIS_TOUCH_PHASE_INACTIVE(phase)				((phase)<3)			// END or CANCEL (or NULL)

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NTOUCH FLAGS

// #define FLAG_DEVICE_RESERVED_0				BIT_0	| 
// #define FLAG_DEVICE_RESERVED_1				BIT_1	|- 3 BITS reserved to store NTOUCH_DEVICE_xxx 3 bits value		 
// #define FLAG_DEVICE_RESERVED_2				BIT_2	| 
#define FLAG_NTOUCH_LOCK_1						BIT_3	// Used by "NListenTouchStartSequenceMsk/NListenTouchEventMsk" only the TOUCH without "lock flag" are going to be listen
#define FLAG_NTOUCH_LOCK_2						BIT_4	// Used by "NListenTouchStartSequenceMsk/NListenTouchEventMsk" only the TOUCH without "lock flag" are going to be listen
#define FLAG_NTOUCH_LOCK_GAMESTATE				BIT_5	// ON: NTOUCH is NOT dispatch to "GameState"
#define FLAG_NTOUCH_IS_IN						BIT_6	// Common with NTOUCH.Flags and NLISTENED_TOUCH.Flags
#define FLAG_NTOUCH_WAS_IN						BIT_7	// Common with NTOUCH.Flags and NLISTENED_TOUCH.Flags
//#define FLAG_NTOUCH_xxx						BIT_8
//#define FLAG_NTOUCH_xxx						BIT_9
//#define FLAG_NTOUCH_xxx						BIT_10
//#define FLAG_NTOUCH_xxx						BIT_11
//#define FLAG_NTOUCH_xxx						BIT_12
//#define FLAG_NTOUCH_xxx						BIT_13
//#define FLAG_NTOUCH_xxx						BIT_14
//#define FLAG_NTOUCH_xxx						BIT_15

#define MASK_NTOUCH_TRACK_MOVE_IN_OUT			0xC0		// 0xC000 -> BIT_6|BIT_7
#define MASK_NTOUCH_LOCKS						0x38		// 0x3800 -> BIT_3|BIT_4|BIT_5
#define MASK_NTOUCH_DEVICE						0x7			// 0x7000 -> BIT_0|BIT_1|BIT_2

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// + NTOUCH FLAGS :: Touch Device ( 3 bits value into NTOUCH.Flags )
#define NGET_TOUCH_DEVICE(touch_flags)			((touch_flags)&MASK_NTOUCH_DEVICE)
#define NSET_TOUCH_DEVICE(touch_flags,device)	(((touch_flags)&~MASK_NTOUCH_DEVICE)|((device)&MASK_NTOUCH_DEVICE))

#define NTOUCH_DEVICE_TOUCHSCREEN				0
#define NTOUCH_DEVICE_MOUSE						1	// Used by Windows to simulate One finger touch
#define NTOUCH_DEVICE_KEYBOARD					2	// Used by Windows to emulate Several additionnal touch ( static only )
#define NTOUCH_DEVICE_4							3
#define NTOUCH_DEVICE_5							4
#define NTOUCH_DEVICE_6							5
#define NTOUCH_DEVICE_7							6
#define NTOUCH_DEVICE_8							7

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + NMOUSE BUTTON ID and FLAG build
/*
#define NMOUSE_BUTTON(id)			(1 << ((id)-1))
#define NMOUSE_BUTTON_LEFT			1
#define NMOUSE_BUTTON_MIDDLE		2
#define NMOUSE_BUTTON_RIGHT			3
#define NMOUSE_BUTTON_WHEELUP		4
#define NMOUSE_BUTTON_WHEELDOWN		5
#define NMOUSE_BUTTON_X1			6
#define NMOUSE_BUTTON_X2			7
*/

typedef struct
{
	Nu32			SystemID;			//|_NTOUCH HEAD 
	Nu16			Flags;				//|	 
	Nu8				Phase;				//|					+ ---> Current Phase of the touch. (NTOUCH_PHASE_xxx )Correlate with EVENT TYPE.
	Nu8				TapCount;			//|-NTOUCH DATA		+ ---> 0 to 255
	NVEC2s16		Position;			//|					+ ---> Touch Position into Screen Coord system.
	NVEC2s16		PreviousPosition;	//|					+ ---> Previous Touch Position into Screen Coord system.
}NTOUCH;



typedef struct
{
	Nu8		TouchCount;
	Nu8		TouchIndex[CONSTANT_NTOUCH_COUNT_MAX];	// Table of NTOUCH structure absolute Index into the NTouchStreamBuffer or into other NTouch Buffer ...
}NTOUCH_SEQUENCE;


// NGetStreamedTouch_...Ptr
// To obtain an unmodifiable pointer on the NTOUCH Structure which is IN the Touch Stream Buffer, or on the desired member of it !
// By using the touch_stream_index
const NTOUCH*		NGetStreamedTouchPtr(const Nu8 touch_stream_index);
const NVEC2s16*	NGetStreamedTouchPositionPtr(const Nu8 touch_stream_index);
const NVEC2s16*	NGetStreamedTouchPreviousPositionPtr(const Nu8 touch_stream_index);
// NGetTouch_...Ptr
// To obtain an unmodifiable pointer on the NTOUCH Structure which is IN the Touch Stream Buffer, or on the desired member of it !
// By using the touch_sequence_index
#define				NGetTouchPtr(ptouch_seq, touch_sequence_index)							NGetStreamedTouchPtr((ptouch_seq)->TouchIndex[touch_sequence_index] )
#define				NGetTouchPositionPtr(ptouch_seq, touch_sequence_index)					NGetStreamedTouchPositionPtr((ptouch_seq)->TouchIndex[touch_sequence_index] )
#define				NGetTouchPreviousPositionPtr(ptouch_seq, touch_sequence_index)			NGetStreamedTouchPreviousPositionPtr((ptouch_seq)->TouchIndex[touch_sequence_index] )

// NGetStreamedTouch_...
// To obtain a copy of the NTOUCH Structure which is IN the Touch Stream Buffer, or a copy of the desired member of it !
// By using the touch_stream_index
NTOUCH*				NGetStreamedTouch(NTOUCH *ptouch,const Nu8 touch_stream_index);
NVEC2s16*		NGetStreamedTouchPosition(NVEC2s16 *pposition, const Nu8 touch_stream_index);
NVEC2s16*		NGetStreamedTouchPreviousPosition(NVEC2s16 *pposition, const Nu8 touch_stream_index);
NVEC2s16*		NGetStreamedTouchMove(NVEC2s16 *pmove, const Nu8 touch_stream_index);
Nu8					NGetStreamedTouchTapCount(const Nu8 touch_stream_index);

// to obtain "touch stream index" from "touch sequence index"
#define				NExtractTouchStreamIndex(ptouch_seq,id_in_touch_seq)	((ptouch_seq)->pTouchIndex[id_in_touch_seq])

// NGetTouch_...
// To obtain a copy of the NTOUCH Structure which is IN the Touch Stream Buffer, or a copy of the desired member of it !
// By using the touch_sequence_index
#define				NGetTouch(ptouch,ptouch_seq, touch_sequence_index)						NGetStreamedTouch(ptouch,(ptouch_seq)->TouchIndex[touch_sequence_index] )
#define				NGetTouchPosition(pposition, ptouch_seq, touch_sequence_index)			NGetStreamedTouchPosition(pposition, (ptouch_seq)->TouchIndex[touch_sequence_index] )
#define				NGetTouchPreviousPosition(pposition,ptouch_seq, touch_sequence_index)	NGetStreamedTouchPreviousPosition(pposition,(ptouch_seq)->TouchIndex[touch_sequence_index] )
#define				NGetTouchMove(pmove,ptouch_seq, touch_sequence_index)					NGetStreamedTouchMove(pmove,(ptouch_seq)->TouchIndex[touch_sequence_index] )
#define				NGetTouchTapCount(ptouch_seq, touch_sequence_index)						NGetStreamedTouchTapCount((ptouch_seq)->TouchIndex[touch_sequence_index])


// UI
typedef struct NUI	NUI;
void				NUITouchCancelAll(NUI *pui, const Nbool bthis_ui_included);

// GAME STATE
void				NGameStateTouchCancelAll(const NSTATEUID_ENUM stateid);
void				NGameStateTouchCancelAllForwardHierarchy(const NSTATEUID_ENUM stateid, const Nbool bthis_state_included);
void				NGameStateMachineTouchCancelAll();

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
NTOUCH*				NGetFastestTouchPtr(const NTOUCH_SEQUENCE *pts);
NVEC2s16*		NGetFastestTouchMove(NVEC2s16 *pmove, const NTOUCH_SEQUENCE *pts);
NTOUCH*				NGetFastestTouchCoordPtr(const NTOUCH_SEQUENCE *pts, const NV3COORD_ENUM coord_id );
Ns16				NGetFastestTouchCoordMove( const NTOUCH_SEQUENCE *pts, const NV3COORD_ENUM coord_id );



// PRIVATE
// Touch System Initialization and Quit ( Used only by NInit() and NQuit() )
void				NInitTouchCore();
void				NQuitTouchCore();

// Touch Dispatch
void				NDispatchTouch_Start( const NTOUCH_SEQUENCE *pin );
void				NDispatchTouch_Move( const NTOUCH_SEQUENCE *pin );
void				NDispatchTouch_End( const NTOUCH_SEQUENCE *pin );
void				NDispatchTouch_Cancel( const NTOUCH_SEQUENCE *pin );
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif  // __NTOUCH_H 


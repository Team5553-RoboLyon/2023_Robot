#ifndef __NTOUCH_LISTENER_H
#define __NTOUCH_LISTENER_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								NTouchListener.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NCStandard.h"
#include "../NType.h"
#include "../NFlags.h"
#include "../Event/NEvent.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef struct
{
	Nu32			SystemID;			//|
	Nu8				Flags;				//|
	Nu8				Phase;				//|
	Nu16			User_Nu16;			
}NLISTENED_TOUCH;

#define FLAG_NTOUCH_LISTENER_TO_BE_CLEAN		BIT_0
//#define FLAG_NTOUCH_LISTENER_xxx				BIT_1
//#define FLAG_NTOUCH_LISTENER_xxx				BIT_2
//#define FLAG_NTOUCH_LISTENER_xxx				BIT_3
// ... All flags until BIT_3 (included) are reserved for NTOUCH_LISTENER use only.
// So, BIT_4 to BIT_7 included can be used by structures inherited from NTOUCH_LISTENER

typedef struct NTOUCH_LISTENER	NTOUCH_LISTENER;
struct NTOUCH_LISTENER
{
	NLISTENED_TOUCH	*pLTouchBuffer;
	Nu8				LTouchCount;				// Current Number of Listened Touch (see LTouchBufferSize)
	Nu8				PreviousLTouchCount;		// Previous Number of Listened Touch
	Nu8				LTouchCountIn;				// Number of listened touch Inside the Listener ( UI rectangle ). !!! All of them are Listened Touch but all listened touches are not necessary IN ( some of them may be OUT ) !!!
	Nu8				PreviousLTouchCountIn;		// Previous Number of listened touch IN 
	Nu8				LTouchCountMax;				// Maximum number of touch this listener can listen ( see LTouchBufferCapacity )
	Nu8				LTouchBufferSize;			// Number of Listened Touch the buffer contains. "ALWAYS == LTouchCount" excepted during END/CANCEL processes where LTouchCount is reduced before LToouchBufferSize
	Nu8				LTouchBufferCapacity;		// Maximum of Listened Touch the buffer can contain. "ALWAYS >= LTouchCountMax". To avoid too many reallocs with shared Listener ( UIs shared their listener ), buffer is not reallocated when user change LTouchCountMax for a value lower than LTouchCapacity. (See 'NSetTouchListenerTouchCountMax' function ).   
	Nu8				Flags;						// BIT_0 for internal use only. All Other available
};

NTOUCH_LISTENER*		NSetupTouchListener(NTOUCH_LISTENER *plistener, const Nu8	capacity);
NTOUCH_LISTENER*		NCreateTouchListener(const Nu8	capacity);
void					NClearTouchListener(NTOUCH_LISTENER *plistener);
void					NDeleteTouchListener(NTOUCH_LISTENER *plistener);

void					NCopyTouchListener(NTOUCH_LISTENER *pdst, const NTOUCH_LISTENER *psrc);
// + Touch Counts.
inline Nu32				NGetListenerTouchCount(const NTOUCH_LISTENER *pl)			{return pl->LTouchCount;}
inline Nu32				NGetListenerPreviousTouchCount(const NTOUCH_LISTENER *pl)	{return pl->PreviousLTouchCount;}
inline Nu32				NGetListenerTouchCountIn(const NTOUCH_LISTENER *pl)			{return pl->LTouchCountIn;}
inline Nu32				NGetListenerTouchCountOut(const NTOUCH_LISTENER *pl)		{return pl->LTouchCount - pl->LTouchCountIn;}
inline Nu32				NGetListenerPreviousTouchCountIn(const NTOUCH_LISTENER *pl)	{return pl->PreviousLTouchCountIn;}
inline Nu32				NGetListenerPreviousTouchCountOut(const NTOUCH_LISTENER *pl){return pl->PreviousLTouchCount - pl->PreviousLTouchCountIn;}

inline Nu32				NIsListenerLastTouchEnded(const NTOUCH_LISTENER *pl)		{return( ( (!pl->LTouchCount) && pl->PreviousLTouchCount ) ? 1:0 );}//{if((!pl->LTouchCount) && pl->PreviousLTouchCount){return 1;}else{return 0;} }
inline Nu32				NIsListenerFirstTouchStarted(const NTOUCH_LISTENER *pl)		{return( ( pl->LTouchCount && (!pl->PreviousLTouchCount) ) ? 1:0 );}//{if(pl->LTouchCount && (!pl->PreviousLTouchCount)){return 1;}else{return 0;} }
inline Nu32				NIsListenerFirstTouchMoveIn(const NTOUCH_LISTENER *pl)		{return( ( (pl->LTouchCountIn)&&(!pl->PreviousLTouchCountIn) ) ? 1:0 );}//{if((pl->LTouchCount - pl->LTouchCountOut)&&(!(pl->PreviousLTouchCount - pl->PreviousLTouchCountOut))){return 1;}else{return 0;} }
inline Nu32				NIsListenerLastTouchMoveOut(const NTOUCH_LISTENER *pl)		{return( ( (!pl->LTouchCountIn)&&(pl->PreviousLTouchCountIn) ) ? 1:0 );}//{if((!(pl->LTouchCount - pl->LTouchCountOut))&&(pl->PreviousLTouchCount - pl->PreviousLTouchCountOut)){return 1;}else{return 0;} }

void					NSetTouchListenerTouchCountMax(NTOUCH_LISTENER *plistener, const Nu8 count_max);

// + Touch
inline NLISTENED_TOUCH* NGetListenedTouch(NTOUCH_LISTENER *plistener, const Nu32 touch_id){NLISTENED_TOUCH	*plt=plistener->pLTouchBuffer;for(Nu8 j=plistener->LTouchBufferSize;j!=0;j--,plt++){if(plt->SystemID==touch_id){return plt;}}return NULL;}

// Notes:	All the following functions return the number of Listened Touch updated into the Listener Buffer.
//			Be aware that number is equal to the "plistened_touchsequence->TouchCount" EXCEPTED in the SPECIAL CASE "CANCEL ALL".
// 
//			CANCEL ALL is a NTOUCH_CANCEL Event with a TouchSequence without any touch index described into.
//			In that case, the listen function (NListen...TouchCancel...) will return, as all the NListen... function, the number of 
//			Listened Touch updated into the Listener Buffer. Here this number is equal to the number of Listened Touch in 
//			the listener buffer before listening CANCEL ALL.( It's obvious, all the listened touch are going to be canceled, so all of them are updated ...).
//			... BUT the "plistened_touchsequence->TouchCount" is going to be ZERO !!! ( Actually, like the "psource_touchsequence->touchCount"! )
//
//			Use the Event Macro "NIS_TOUCH_CANCEL_ALL(event pointer)" if you need to deal with it.
//
// To CANCEL ALL Listened Touch of a listener without listen an event. ( to force it ... )
Nu8						NTouchListenerCancelAll(NTOUCH_LISTENER *plistener);

// To clean Up a Listener after listening END or CANCEL TouchSequence
void					NCleanTouchListenerUp(NTOUCH_LISTENER *plistener);

// "Simple" Listening:		Just build Up the listened TouchSequence for a Listener 
//							( or complete Event for high level function 'NListenTouchEvent')
//Nu8						NListenTouchSequence(NTOUCH_LISTENER *plistener, const Nu8 touch_phase, NTOUCH_SEQUENCE *plistened_touchsequence, const NTOUCH_SEQUENCE *psource_touchsequence );
Nu8						NListenTouchEvent(NTOUCH_LISTENER *plistener, NEVENT *plistened_touchevent,const NEVENT *psrc_touch_event );
Nu8						NListenTouchStartSequence(NTOUCH_LISTENER *plistener, NTOUCH_SEQUENCE *plistened_touchsequence, const NTOUCH_SEQUENCE *psource_touchsequence );
Nu8						NListenTouchMoveSequence(NTOUCH_LISTENER *plistener, NTOUCH_SEQUENCE *plistened_touchsequence, const NTOUCH_SEQUENCE *psource_touchsequence );
Nu8						NListenTouchCancelSequence(NTOUCH_LISTENER *plistener, NTOUCH_SEQUENCE *plistened_touchsequence, const NTOUCH_SEQUENCE *psource_touchsequence );
Nu8						NListenTouchEndSequence(NTOUCH_LISTENER *plistener, NTOUCH_SEQUENCE *plistened_touchsequence, const NTOUCH_SEQUENCE *psource_touchsequence );

// "Masked" Listening:		Build Up the listened TouchSequence for a Listener with a mask to ignored some masked incoming NTOUCH
//							( or complete Event for high level function 'NListenTouchEventMsk')
//							Masked does only matter for Start Event (Obviously !... because if a NTOUCH is masked/ignored at START ... 
//							...is going to be ignored automatically for all the other phases ... Obviously ! )
Nu8						NListenTouchEventMsk(NTOUCH_LISTENER *plistener, NEVENT *plistened_touchevent,const NEVENT *ptouch_event, const Nu16 touch_lockmask );
Nu8						NListenTouchStartSequenceMsk(NTOUCH_LISTENER *plistener, NTOUCH_SEQUENCE *plistened_touchsequence, const NTOUCH_SEQUENCE *psource_touchsequence, const Nu16 touch_lockmask );


// "Advanced" Listening:	Build Up the listened TouchSequence for a Listener 
//							AND
//							Remove the Listened Touch Sequence from the source touch sequence
//							There is no high level functions "NListenAndExtractTouchEvent" and "NListenAndExtractTouchEventMsk"
//							Advanced user who use "Advanced" listening has to manage touch event by himself
Nu8						NListenAndExtractTouchStartSequence(NTOUCH_LISTENER *plistener, NTOUCH_SEQUENCE *plistened_touchsequence, NTOUCH_SEQUENCE *psource_touchsequence );
Nu8						NListenAndExtractTouchMoveSequence(NTOUCH_LISTENER *plistener, NTOUCH_SEQUENCE *plistened_touchsequence, NTOUCH_SEQUENCE *psource_touchsequence );
Nu8						NListenAndExtractTouchCancelSequence(NTOUCH_LISTENER *plistener, NTOUCH_SEQUENCE *plistened_touchsequence, NTOUCH_SEQUENCE *psource_touchsequence );
Nu8						NListenAndExtractTouchEndSequence(NTOUCH_LISTENER *plistener, NTOUCH_SEQUENCE *plistened_touchsequence, NTOUCH_SEQUENCE *psource_touchsequence );

Nu8						NListenAndExtractTouchStartSequenceMsk(NTOUCH_LISTENER *plistener, NTOUCH_SEQUENCE *plistened_touchsequence, NTOUCH_SEQUENCE *psource_touchsequence, const Nu16 touch_lockmask );


// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif  // __NTOUCH_H 


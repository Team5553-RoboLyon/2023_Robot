#ifndef __N_EVENT_TOUCH_H
#define __N_EVENT_TOUCH_H


// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NEventTouch.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../NCStandard.h"
#include "../NType.h"
#include "../Event/NEvent.h"
#include "../Touch/NTouch.h"
#include "../Touch/NTouchListener.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **					  Functions prototypes and Structures							**
// ***************************************************************************************


// NGetEventTouch_...Ptr
// To obtain an unmodifiable pointer on the NTOUCH Structure which is stored into the Touch Buffer pointed by the Event ( this buffer is specific to each Listener or GameState)!
// By using the touch_sequence_index
#define				NGetEventTouchPtr(pev,touch_index)							NGetTouchPtr(&(pev)->Touch.TouchSequence,touch_index)
#define				NGetEventTouchPositionPtr(pev,touch_index)					NGetTouchPositionPtr(&(pev)->Touch.TouchSequence,touch_index)
#define				NGetEventTouchPreviousPositionPtr(pev,touch_index)			NGetTouchPreviousPositionPtr(&(pev)->Touch.TouchSequence,touch_index)

#define 			NGetEventFastestTouchPtr(pev)								NGetFastestTouchPtr(&(pev)->Touch.TouchSequence)
#define 			NGetEventFastestTouchCoordPtr(pev,coord_id)					NGetFastestTouchCoordPtr(&(pev)->Touch.TouchSequence, (coord_id) );

// NGetEventTouch_...
// To obtain a copy of the NTOUCH Structure which is stored into the Touch Buffer pointed by the Event ( this buffer is specific to each Listener or GameState)!
// By using the touch_sequence_index
#define				NGetEventTouch(ptouch,pev,touch_index)						NGetTouch(ptouch,&(pev)->Touch.TouchSequence,touch_index)
#define				NGetEventTouchTapCount(pev,touch_index)						NGetTouchTapCount(&(pev)->Touch.TouchSequence,touch_index)
#define				NGetEventTouchPosition(ppos,pev,touch_index)				NGetTouchPosition(ppos,&(pev)->Touch.TouchSequence,touch_index)
#define				NGetEventTouchPreviousPosition(ppos,pev,touch_index)		NGetTouchPreviousPosition(ppos,&(pev)->Touch.TouchSequence,touch_index)
#define				NGetEventTouchMove(pmove,pev,touch_index)					NGetTouchMove(pmove,&(pev)->Touch.TouchSequence,touch_index)

#define				NGetEventFastestTouchMove(pmove,pev)						NGetFastestTouchMove((pmove),&(pev)->Touch.TouchSequence);
#define				NGetEventFastestTouchCoordMove(pev,coord_id)				NGetFastestTouchCoordMove(&(pev)->Touch.TouchSequence, (coord_id) );

// NGetEventRecipientListener ( useful to use inside a function that processes event and may be call from UI or gameState.)
NTOUCH_LISTENER*	NGetEventRecipientTouchListener(const NEVENT *pevent);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NEVENT_H 


#include "../UI/NUI.h"
#include "../GameLoop/NGameStates.h"

#include "NEventTouch.h"

#ifdef _NEDITOR
#include "../NEditor.h"
extern NEDITOR NEditor;
#endif


NTOUCH_LISTENER*  NGetEventRecipientTouchListener(const NEVENT *pevent)
{
	NErrorIf(NGET_EVENT_MAIN(pevent->Type) != _TOUCH, NERROR_GEOMETRY_INCONSISTENT_PARAMETER ); // pevent is not a TOUCH Event !
	NErrorIf(!pevent->Head.pRecipient, NERROR_NULL_POINTER);

	// There is no Test  to verify if "pevent->Head.pRecipient" exists.
	// ... Because it suppose to be ! 
	switch( pevent->Head.RecipientCast )
	{
		case NEVENT_RECIPIENT_CAST_UI:
			return &((NUI*)pevent->Head.pRecipient)->pTouchUIListener->Listener;
		
		case NEVENT_RECIPIENT_CAST_GAMESTATE:
			return &((NGAMESTATE*)pevent->Head.pRecipient)->Listener;
		
#ifdef _NEDITOR
		case NEVENT_RECIPIENT_CAST_NEDITOR_TOOL:
			return &NEditor.Listener;
#endif

		default:
			NErrorIf(1,NERROR_UNAUTHORIZED_CASE);
			return NULL;
	}
}
/*

Nu32 NGetEventRecipientLTouchCount(const NEVENT *pevent)
{
	NErrorIf(NGET_EVENT_MAIN(pevent->Type) != _TOUCH, NERROR_GEOMETRY_INCONSISTENT_PARAMETER ); // pevent is not a TOUCH Event !
	NErrorIf(!pevent->Head.pRecipient, NERROR_NULL_POINTER);

	// There is no Test  to verify if "pevent->Head.pRecipient" exists.
	// ... Because it suppose to be ! 
	switch( pevent->Head.RecipientCast )
	{
		case NEVENT_RECIPIENT_CAST_UI:
			return ((NUI*)pevent->Head.pRecipient)->pTouchUIListener->Listener.LTouchCount;

		case NEVENT_RECIPIENT_CAST_GAMESTATE:
			return ((NGAMESTATE*)pevent->Head.pRecipient)->Listener.LTouchCount;

		default:
			NError(NERROR_UNAUTHORIZED_CASE);
			return 0;
	}
}

Nu32 NGetEventRecipientLTouchCountMax(const NEVENT *pevent)
{
	NErrorIf(NGET_EVENT_MAIN(pevent->Type) != _TOUCH, NERROR_GEOMETRY_INCONSISTENT_PARAMETER ); // pevent is not a TOUCH Event !
	NErrorIf(!pevent->Head.pRecipient, NERROR_NULL_POINTER);

	// There is no Test  to verify if "pevent->Head.pRecipient" exists.
	// ... Because it suppose to be ! 
	switch( pevent->Head.RecipientCast )
	{
	case NEVENT_RECIPIENT_CAST_UI:
		return ((NUI*)pevent->Head.pRecipient)->pTouchUIListener->Listener.LTouchCountMax;

	case NEVENT_RECIPIENT_CAST_GAMESTATE:
		return ((NGAMESTATE*)pevent->Head.pRecipient)->Listener.LTouchCountMax;

	default:
		NError(NERROR_UNAUTHORIZED_CASE);
		return 0;
	}
}
*/

#include "../NCStandard.h"
#include "../Containers/NRingBuffer.h"
#include "../Containers/NNode.h"


#include "../NErrorHandling.h"

#include "NTouchStream.h"
#include "NTouchUIListener.h"
#include "NTouch.h"

// Circular Buffer where the System pile up the Touch data on it's side 
// IOS: 
//		touchesBegan/touchesMoved/touchesEnded/touchesCancelled methods filled up this Stream Buffer on their side (and NEvent Stream buffer in the same time).
// ANDROID:
//		...
// WINDOWS:
//	
NRINGBUFFER			NTouchStream;

// UI Touch Listening Management.
// These 2 NODE LISTS store the NTOUCH_UI_LISTENER structures used and referenced by UIs to Listen Touch as quick as possible.
NNODELIST			UIListener_List;
NNODELIST			AvailableUIListener_List;

// Used to manage multiple/single UI activation(s) 
Nu8					NTouchUICompatibility;

void NInitTouchCore()
{	
	// FIRST OF ALL
	// On windows, check SDL version and N version compatibility:
/*
	#ifdef _NWINDOWS
	// Check Mouse button ids
	NErrorIf(NMOUSE_BUTTON_LEFT							!= SDL_BUTTON_LEFT,						NERROR_SYSTEM_CHECK);
	NErrorIf(NMOUSE_BUTTON_RIGHT						!= SDL_BUTTON_RIGHT,					NERROR_SYSTEM_CHECK);
	NErrorIf(NMOUSE_BUTTON_MIDDLE						!= SDL_BUTTON_MIDDLE,					NERROR_SYSTEM_CHECK);
	NErrorIf(NMOUSE_BUTTON_WHEELUP						!= SDL_BUTTON_WHEELUP,					NERROR_SYSTEM_CHECK);
	NErrorIf(NMOUSE_BUTTON_WHEELDOWN					!= SDL_BUTTON_WHEELDOWN,				NERROR_SYSTEM_CHECK);
	NErrorIf(NMOUSE_BUTTON_X1							!= SDL_BUTTON_X1,						NERROR_SYSTEM_CHECK);
	NErrorIf(NMOUSE_BUTTON_X2							!= SDL_BUTTON_X2,						NERROR_SYSTEM_CHECK);
	// Check Mouse button flags
	NErrorIf(NMOUSE_BUTTON(NMOUSE_BUTTON_LEFT)			!= SDL_BUTTON(SDL_BUTTON_LEFT),			NERROR_SYSTEM_CHECK);
	NErrorIf(NMOUSE_BUTTON(NMOUSE_BUTTON_RIGHT)			!= SDL_BUTTON(SDL_BUTTON_RIGHT),		NERROR_SYSTEM_CHECK);
	NErrorIf(NMOUSE_BUTTON(NMOUSE_BUTTON_MIDDLE)		!= SDL_BUTTON(SDL_BUTTON_MIDDLE),		NERROR_SYSTEM_CHECK);
	NErrorIf(NMOUSE_BUTTON(NMOUSE_BUTTON_WHEELUP)		!= SDL_BUTTON(SDL_BUTTON_WHEELUP),		NERROR_SYSTEM_CHECK);
	NErrorIf(NMOUSE_BUTTON(NMOUSE_BUTTON_WHEELDOWN)		!= SDL_BUTTON(SDL_BUTTON_WHEELDOWN),	NERROR_SYSTEM_CHECK);
	NErrorIf(NMOUSE_BUTTON(NMOUSE_BUTTON_X1)			!= SDL_BUTTON(SDL_BUTTON_X1),			NERROR_SYSTEM_CHECK);
	NErrorIf(NMOUSE_BUTTON(NMOUSE_BUTTON_X2)			!= SDL_BUTTON(SDL_BUTTON_X2),			NERROR_SYSTEM_CHECK);
	#endif
*/

	// A)Stream Buffer
	NSetupRingBuffer(&NTouchStream,CONSTANT_NTOUCH_STREAM_BUFFER_CAPACITY,sizeof(NTOUCH));
	// B)Touch Listeners Lists ( used to avoid create/Delete TouchUIListener )
	NSETUP_NODELIST(&UIListener_List);
	NSETUP_NODELIST(&AvailableUIListener_List);
	//		- Insert a first Listener into the available Listener list
	NNodeInsertBeginning((NNODE*)NCreateTouchUIListener(),&AvailableUIListener_List);
}

void NQuitTouchCore()
{	
	// A)Stream Buffer
	NClearRingBuffer(&NTouchStream,NULL);
	// B)Touch Listeners Lists ( used to avoid create/Delete TouchUIListener )
	NErrorIf(!NIS_NODELIST_EMPTY(&UIListener_List), NERROR_SYSTEM_CHECK);
	NCLEAR_NODELIST(&UIListener_List);

	NTOUCH_UI_LISTENER *plistenerb;
	NTOUCH_UI_LISTENER *plistener = (NTOUCH_UI_LISTENER*)AvailableUIListener_List.pFirst;
	while(NIS_NODE_VALID(plistener,&AvailableUIListener_List))
	{
		plistenerb = (NTOUCH_UI_LISTENER*)NGET_NEXT_NODE(plistener);

		NDeleteTouchUIListener(plistener);
		plistener = plistenerb;
	}
	NCLEAR_NODELIST(&AvailableUIListener_List);
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// +	GET STREAMED TOUCH ...
// +
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const NTOUCH* NGetStreamedTouchPtr(const Nu8 touch_stream_index)
{
	NErrorIf(!NIsValidRingBufferAbsoluteIndex(&NTouchStream,touch_stream_index), NERROR_RINGBUFFER_INDEX_OUTOFRANGE);
	return &_STREAMTOUCH(touch_stream_index);//((NTOUCH*)NTouchStream.pBuffer)[touch_stream_index];
}

const NVEC2s16* NGetStreamedTouchPositionPtr(const Nu8 touch_stream_index)
{
	NErrorIf(!NIsValidRingBufferAbsoluteIndex(&NTouchStream,touch_stream_index), NERROR_RINGBUFFER_INDEX_OUTOFRANGE);
	return &_STREAMTOUCH(touch_stream_index).Position;//&((NTOUCH*)NTouchStream.pBuffer)[touch_stream_index].Position;
}

const NVEC2s16* NGetStreamedTouchPreviousPositionPtr(const Nu8 touch_stream_index)
{
	NErrorIf(!NIsValidRingBufferAbsoluteIndex(&NTouchStream,touch_stream_index), NERROR_RINGBUFFER_INDEX_OUTOFRANGE);
	return &_STREAMTOUCH(touch_stream_index).PreviousPosition;//&((NTOUCH*)NTouchStream.pBuffer)[touch_stream_index].PreviousPosition;
}

NTOUCH* NGetStreamedTouch(NTOUCH *ptouch,const Nu8 touch_stream_index)
{
	NErrorIf(!ptouch, NERROR_NULL_VALUE);
	NErrorIf(!NIsValidRingBufferAbsoluteIndex(&NTouchStream,touch_stream_index), NERROR_RINGBUFFER_INDEX_OUTOFRANGE);
	*ptouch = _STREAMTOUCH(touch_stream_index);//((NTOUCH*)NTouchStream.pBuffer)[touch_stream_index];
	return ptouch;
}


NVEC2s16* NGetStreamedTouchPosition(NVEC2s16 *pposition, const Nu8 touch_stream_index)
{
	NErrorIf(!pposition, NERROR_NULL_VALUE);
	NErrorIf(!NIsValidRingBufferAbsoluteIndex(&NTouchStream,touch_stream_index), NERROR_RINGBUFFER_INDEX_OUTOFRANGE);
	*pposition = _STREAMTOUCH(touch_stream_index).Position;//((NTOUCH*)NTouchStream.pBuffer)[touch_stream_index].Position;
	return pposition;
}

NVEC2s16* NGetStreamedTouchPreviousPosition(NVEC2s16 *pposition, const Nu8 touch_stream_index)
{
	NErrorIf(!pposition, NERROR_NULL_VALUE);
	NErrorIf(!NIsValidRingBufferAbsoluteIndex(&NTouchStream,touch_stream_index), NERROR_RINGBUFFER_INDEX_OUTOFRANGE);
	*pposition = _STREAMTOUCH(touch_stream_index).PreviousPosition;//((NTOUCH*)NTouchStream.pBuffer)[touch_stream_index].PreviousPosition;
	return pposition;
}

NVEC2s16* NGetStreamedTouchMove(NVEC2s16 *pmove, const Nu8 touch_stream_index)
{
	NErrorIf(!pmove, NERROR_NULL_VALUE);
	NErrorIf(!NIsValidRingBufferAbsoluteIndex(&NTouchStream,touch_stream_index), NERROR_RINGBUFFER_INDEX_OUTOFRANGE);
	pmove->x = _STREAMTOUCH(touch_stream_index).Position.x - _STREAMTOUCH(touch_stream_index).PreviousPosition.x;//((NTOUCH*)NTouchStream.pBuffer)[touch_stream_index].PreviousPosition;
	pmove->y = _STREAMTOUCH(touch_stream_index).Position.y - _STREAMTOUCH(touch_stream_index).PreviousPosition.y;//((NTOUCH*)NTouchStream.pBuffer)[touch_stream_index].PreviousPosition;
	return pmove;
}

Nu8 NGetStreamedTouchTapCount(const Nu8 touch_stream_index)
{
	NErrorIf(!NIsValidRingBufferAbsoluteIndex(&NTouchStream,touch_stream_index), NERROR_RINGBUFFER_INDEX_OUTOFRANGE);
	return _STREAMTOUCH(touch_stream_index).TapCount;//((NTOUCH*)NTouchStream.pBuffer)[touch_stream_index].TapCount;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// +	ADVANCED TOUCH SEQUENCES FUNCTIONS
// +
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
NTOUCH* NGetFastestTouchPtr(const NTOUCH_SEQUENCE *pts)
{
	// Not an error but user should not call this function if pts->TouchCount is ZERO
	NErrorIf(!pts->TouchCount, NERROR_NULL_VALUE); 
	if(pts->TouchCount == 1)
	{
		// Not an error but user should not call this function if there is no MOVE ...
		NErrorIf(_STREAMTOUCH(pts->TouchIndex[0]).Phase != NTOUCH_PHASE_MOVE, NERROR_INCONSISTENT_REQUEST );
		return &_STREAMTOUCH(pts->TouchIndex[0]);
	}
	else
	{
		Nu32	dist;
		Nu32	mindist = 0;
		NTOUCH	*ptmin = NULL;
		NTOUCH	*ptouch;
		Nu8		*ptindex = (Nu8*)pts->TouchIndex;
		for(Nu32 i=pts->TouchCount;i!=0;i--,ptindex++)
		{
			ptouch = &_STREAMTOUCH(*ptindex);
			NErrorIf(ptouch->Phase != NTOUCH_PHASE_MOVE, NERROR_INCONSISTENT_REQUEST );
			
			dist = NABS(ptouch->Position.x - ptouch->PreviousPosition.x) + NABS(ptouch->Position.y - ptouch->PreviousPosition.y);
			if(dist > mindist)
			{
				mindist = dist;
				ptmin	= ptouch;
			}
		}
		NErrorIf(!ptmin, NERROR_SYSTEM_CHECK ); // NOT POSSIBLE .... 
		return ptmin;
	}
}

NVEC2s16* NGetFastestTouchMove(NVEC2s16 *pmove, const NTOUCH_SEQUENCE *pts)
{
	NErrorIf(!pmove, NERROR_NULL_POINTER);
	// Not an error but user should not call this function if pts->TouchCount is ZERO
	NErrorIf(!pts->TouchCount, NERROR_NULL_VALUE); 
	
	NTOUCH	*ptouch;

	if(pts->TouchCount == 1)
	{
		// Not an error but user should not call this function if there is no MOVE ...
		NErrorIf(_STREAMTOUCH(pts->TouchIndex[0]).Phase != NTOUCH_PHASE_MOVE, NERROR_INCONSISTENT_REQUEST );
		ptouch = &_STREAMTOUCH(pts->TouchIndex[0]);
		
		pmove->x = ptouch->Position.x - ptouch->PreviousPosition.x;
		pmove->y = ptouch->Position.y - ptouch->PreviousPosition.y;
		return pmove;
	}
	else
	{
		Nu32	dist;
		Nu32	mindist = 0;
		NTOUCH	*ptmin = NULL;
		Nu8		*ptindex = (Nu8*)pts->TouchIndex;
		for(Nu32 i=pts->TouchCount;i!=0;i--,ptindex++)
		{
			ptouch = &_STREAMTOUCH(*ptindex);
			NErrorIf(ptouch->Phase != NTOUCH_PHASE_MOVE, NERROR_INCONSISTENT_REQUEST );

			dist = NABS(ptouch->Position.x - ptouch->PreviousPosition.x) + NABS(ptouch->Position.y - ptouch->PreviousPosition.y);
			if(dist > mindist)
			{
				mindist = dist;
				ptmin	= ptouch;
			}
		}
		NErrorIf(!ptmin, NERROR_SYSTEM_CHECK ); // NOT POSSIBLE .... 

		pmove->x = ptmin->Position.x - ptmin->PreviousPosition.x;
		pmove->y = ptmin->Position.y - ptmin->PreviousPosition.y;
		return pmove;
	}
}

NTOUCH* NGetFastestTouchCoordPtr(const NTOUCH_SEQUENCE *pts, const NV3COORD_ENUM coord_id )
{
	// Not an error but user should not call this function if pts->TouchCount is ZERO
	NErrorIf(!pts->TouchCount, NERROR_NULL_VALUE); 
	
	// This function work only with _X and _Y which are respectively 0 and 1 !!! 
	if(coord_id >= _Z)
	{
		NErrorIf(1, NERROR_INCONSISTENT_VALUES ); 
		return NULL;
	}
	if(pts->TouchCount == 1)
	{
		// Not an error but user should not call this function if there is no MOVE ...
		NErrorIf(_STREAMTOUCH(pts->TouchIndex[0]).Phase != NTOUCH_PHASE_MOVE, NERROR_INCONSISTENT_REQUEST );
		return &_STREAMTOUCH(pts->TouchIndex[0]);
	}
	else
	{
		Nu16	mindist = 0;
		Nu16	dist;
		NTOUCH	*ptmin = NULL;
		NTOUCH	*ptouch;
		Nu8		*ptindex = (Nu8*)pts->TouchIndex;
		for(Nu32 i=pts->TouchCount;i!=0;i--,ptindex++)
		{
			ptouch = &_STREAMTOUCH(*ptindex);
			NErrorIf(ptouch->Phase != NTOUCH_PHASE_MOVE, NERROR_INCONSISTENT_REQUEST );
			dist = NABS(ptouch->Position.coord[coord_id] - ptouch->PreviousPosition.coord[coord_id]);
			if( dist > mindist )
			{
				mindist = dist;
				ptmin	= ptouch;
			}
		}
		NErrorIf(!ptmin, NERROR_SYSTEM_CHECK ); // NOT POSSIBLE .... 
		return ptmin;
	}
}

Ns16 NGetFastestTouchCoordMove( const NTOUCH_SEQUENCE *pts, const NV3COORD_ENUM coord_id )
{
	// Not an error but user should not call this function if pts->TouchCount is ZERO
	NErrorIf(!pts->TouchCount, NERROR_NULL_VALUE); 

	// This function work only with _X and _Y which are respectively 0 and 1 !!! 
	if(coord_id >= _Z)
	{
		NErrorIf(1, NERROR_INCONSISTENT_VALUES ); 
		return 0;
	}
	
	Ns16	mov = 0;
	if(pts->TouchCount == 1)
	{
		// Not an error but user should not call this function if there is no MOVE ...
		NErrorIf(_STREAMTOUCH(pts->TouchIndex[0]).Phase != NTOUCH_PHASE_MOVE, NERROR_INCONSISTENT_REQUEST );
		mov = _STREAMTOUCH(pts->TouchIndex[0]).Position.coord[coord_id] - _STREAMTOUCH(pts->TouchIndex[0]).PreviousPosition.coord[coord_id];
		return mov;
	}
	else
	{
		Ns16	mindist = 0;
		Ns16	dist;
		NTOUCH	*ptouch;
		Nu8		*ptindex = (Nu8*)pts->TouchIndex;
		for(Nu32 i=pts->TouchCount;i!=0;i--,ptindex++)
		{
			ptouch = &_STREAMTOUCH(*ptindex);
			NErrorIf(ptouch->Phase != NTOUCH_PHASE_MOVE, NERROR_INCONSISTENT_REQUEST );
			dist = ptouch->Position.coord[coord_id] - ptouch->PreviousPosition.coord[coord_id];
			if( NABS(dist) > mindist )
			{
				mindist = NABS(dist);
				mov		= dist;
			}
		}
		return mov;
	}
}
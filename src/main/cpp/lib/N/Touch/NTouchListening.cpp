#include "lib/N/NCStandard.h"
#include "lib/N/NErrorHandling.h"
#include "../Containers/NRingBuffer.h"
#include "NTouchStream.h"
#include "NTouch.h"
#include "NTouchListener.h"

Nu8 NListenTouchStartSequence(NTOUCH_LISTENER *plistener, NTOUCH_SEQUENCE *plistened_touchsequence, const NTOUCH_SEQUENCE *psource_touchsequence)
{
	NErrorIf(!plistener || plistened_touchsequence || psource_touchsequence, NERROR_NULL_VALUE);
	NErrorIf(ISFLAG_ON(plistener->Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN), NERROR_TOUCH_LISTENER_HAS_TO_BE_CLEAN_UP);

	NErrorIf(!psource_touchsequence->TouchCount, NERROR_NULL_VALUE);							 // Source sequence must have, at least one Touch !
	NErrorIf(plistened_touchsequence->TouchCount, NERROR_NON_NULL_VALUE);						 // listened sequence must be empty !
	NErrorIf(plistener->LTouchCount != plistener->LTouchBufferSize, NERROR_INCONSISTENT_VALUES); // Both must be equal at this stage ! ( the only moment when they are different is within touch End/Cancel processes )

	if (plistener->LTouchCount == plistener->LTouchCountMax)
		return 0;

	Nu8 *ptindex = (Nu8 *)psource_touchsequence->TouchIndex;
	NErrorIf(ptindex != (Nu8 *)&psource_touchsequence->TouchIndex[0], NERROR_SYSTEM_CHECK);
	for (Nu8 t = psource_touchsequence->TouchCount; t != 0; t--, ptindex++)
	{
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef _DEBUG
		// + In Debug Mode, check if incoming Touch is really in Phase START !
		NErrorIf(_STREAMTOUCH(*ptindex).Phase != NTOUCH_PHASE_START, NERROR_INCONSISTENT_VALUES);
		// + In debug Mode, check if incoming Touch is not already registered !!
		NLISTENED_TOUCH *plt = plistener->pLTouchBuffer;
		for (Nu8 i_dbg = plistener->LTouchBufferSize; i_dbg != 0; i_dbg--, plt++)
		{
			NErrorIf(plt->SystemID == _STREAMTOUCH(*ptindex).SystemID, NERROR_TOUCH_ALREADY_LISTEN);
		}
#endif
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// Build up the Listened Touch Sequence
		plistened_touchsequence->TouchIndex[plistened_touchsequence->TouchCount] = *ptindex;
		plistened_touchsequence->TouchCount++;

		// Store the new Listened Touch Data into the Listener Buffer !
		plistener->pLTouchBuffer[plistener->LTouchBufferSize].SystemID = _STREAMTOUCH(*ptindex).SystemID;
		plistener->pLTouchBuffer[plistener->LTouchBufferSize].Phase = NTOUCH_PHASE_START;
		plistener->pLTouchBuffer[plistener->LTouchBufferSize].Flags = 0;
		plistener->pLTouchBuffer[plistener->LTouchBufferSize].User_Nu16 = 0;

		if (plistener->LTouchCountMax == plistener->LTouchBufferSize++)
			break;
	}

	if (plistener->LTouchCount != plistener->LTouchBufferSize)
	{
		plistener->PreviousLTouchCount = plistener->LTouchCount;
		plistener->LTouchCount = plistener->LTouchBufferSize;
	}

	// Return the exact number of NEW listened touch(es) pushed back in the buffer
	return plistened_touchsequence->TouchCount;
}

Nu8 NListenTouchStartSequenceMsk(NTOUCH_LISTENER *plistener, NTOUCH_SEQUENCE *plistened_touchsequence, const NTOUCH_SEQUENCE *psource_touchsequence, const Nu16 touch_lockmask)
{
	NErrorIf(!plistener || plistened_touchsequence || psource_touchsequence, NERROR_NULL_VALUE);
	NErrorIf(ISFLAG_ON(plistener->Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN), NERROR_TOUCH_LISTENER_HAS_TO_BE_CLEAN_UP);

	NErrorIf(!psource_touchsequence->TouchCount, NERROR_NULL_VALUE);							 // Source sequence must have, at least one Touch !
	NErrorIf(plistened_touchsequence->TouchCount, NERROR_NON_NULL_VALUE);						 // listened sequence must be empty !
	NErrorIf(plistener->LTouchCount != plistener->LTouchBufferSize, NERROR_INCONSISTENT_VALUES); // Both must be equal at this stage ! ( the only moment when they are different is within touch End/Cancel processes )
	NErrorIf((touch_lockmask & MASK_NTOUCH_LOCKS) != touch_lockmask, NERROR_UNAUTHORIZED_FLAG);	 // NON LOCK FLAG are used inside touch_lockmask. It's BAD !

	if (plistener->LTouchCount == plistener->LTouchCountMax)
		return 0;

	Nu8 *ptindex = (Nu8 *)psource_touchsequence->TouchIndex;
	NErrorIf(ptindex != (Nu8 *)&psource_touchsequence->TouchIndex[0], NERROR_SYSTEM_CHECK);
	for (Nu8 t = psource_touchsequence->TouchCount; t != 0; t--, ptindex++)
	{
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef _DEBUG
		// + In Debug Mode, check if incoming Touch is really in Phase START !
		NErrorIf(_STREAMTOUCH(*ptindex).Phase != NTOUCH_PHASE_START, NERROR_INCONSISTENT_VALUES);
		// + In debug Mode, check if incoming Touch is not already registered !!
		NLISTENED_TOUCH *plt = plistener->pLTouchBuffer;
		for (Nu8 i_dbg = plistener->LTouchBufferSize; i_dbg != 0; i_dbg--, plt++)
		{
			NErrorIf(plt->SystemID == _STREAMTOUCH(*ptindex).SystemID, NERROR_TOUCH_ALREADY_LISTEN);
		}
#endif
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// With Lock Mask, only the TOUCH without "lock flag" are going to be listen !
		if ((_STREAMTOUCH(*ptindex).Flags & touch_lockmask) != touch_lockmask)
		{
			// Build up the Listened Touch Sequence
			plistened_touchsequence->TouchIndex[plistened_touchsequence->TouchCount] = *ptindex;
			plistened_touchsequence->TouchCount++;

			// Store the new Listened Touch Data into the Listener Buffer !
			plistener->pLTouchBuffer[plistener->LTouchBufferSize].SystemID = _STREAMTOUCH(*ptindex).SystemID;
			plistener->pLTouchBuffer[plistener->LTouchBufferSize].Phase = NTOUCH_PHASE_START;
			plistener->pLTouchBuffer[plistener->LTouchBufferSize].Flags = 0;
			plistener->pLTouchBuffer[plistener->LTouchBufferSize].User_Nu16 = 0;

			if (plistener->LTouchCountMax == plistener->LTouchBufferSize++)
				break;
		}
	}

	if (plistener->LTouchCount != plistener->LTouchBufferSize)
	{
		plistener->PreviousLTouchCount = plistener->LTouchCount;
		plistener->LTouchCount = plistener->LTouchBufferSize;
	}

	// Return the exact number of NEW listened touch(es) pushed back in the buffer
	return plistened_touchsequence->TouchCount;
}

Nu8 NListenTouchMoveSequence(NTOUCH_LISTENER *plistener, NTOUCH_SEQUENCE *plistened_touchsequence, const NTOUCH_SEQUENCE *psource_touchsequence)
{
	NErrorIf(!plistener || !plistened_touchsequence || !psource_touchsequence, NERROR_NULL_VALUE);
	NErrorIf(ISFLAG_ON(plistener->Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN), NERROR_TOUCH_LISTENER_HAS_TO_BE_CLEAN_UP);

	NErrorIf(!psource_touchsequence->TouchCount, NERROR_NULL_VALUE);							 // Source sequence must have, at least one Touch !
	NErrorIf(plistened_touchsequence->TouchCount, NERROR_NON_NULL_VALUE);						 // listened sequence must be empty !
	NErrorIf(plistener->LTouchCount != plistener->LTouchBufferSize, NERROR_INCONSISTENT_VALUES); // Both must be equal at this stage ! ( the only moment when they are different is within touch End/Cancel processes )

	if (!plistener->LTouchCount)
		return 0;

	Nu32 streamed_systemid;
	NLISTENED_TOUCH *plistened;
	Nu8 s, l;
	Nu8 *ptindex = (Nu8 *)psource_touchsequence->TouchIndex;
	NErrorIf(ptindex != (Nu8 *)&psource_touchsequence->TouchIndex[0], NERROR_SYSTEM_CHECK);
	for (s = psource_touchsequence->TouchCount; s != 0; s--, ptindex++)
	{
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// + In Debug Mode, check if incoming Touch is really in Phase MOVE !
		NErrorIf(_STREAMTOUCH(*ptindex).Phase != NTOUCH_PHASE_MOVE, NERROR_TOUCH_INCONSISTENT_PHASE);
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		streamed_systemid = _STREAMTOUCH(*ptindex).SystemID;
		plistened = plistener->pLTouchBuffer;
		for (l = plistener->LTouchBufferSize; l != 0; l--, plistened++)
		{
			// Match !
			if (streamed_systemid == plistened->SystemID)
			{
				// Listened associated NTOUCH should not have ..._END or _...CANCEL phase ! ( should have _START or _MOVE )
				NErrorIf(NIS_TOUCH_PHASE_INACTIVE(plistened->Phase), NERROR_TOUCH_INCONSISTENT_PHASE);

				// Build up the Listened Touch Sequence
				plistened_touchsequence->TouchIndex[plistened_touchsequence->TouchCount] = *ptindex;
				plistened_touchsequence->TouchCount++;

				// Update Listened Touch Data into the Listener Buffer !
				plistened->Phase = NTOUCH_PHASE_MOVE;
				//		plistened->SystemID		= not updated;
				//		plistened->Flags		= not updated;
				//		plistened->User_Nu16	= not updated;
				break;
			}
		}

		// Are all the current Listened Touch in Buffer updated ?
		if (plistened_touchsequence->TouchCount == plistener->LTouchCount)
			break;
	}

	// Return the exact number of listened touch(es) updated in the buffer
	return plistened_touchsequence->TouchCount;
}

Nu8 NListenTouchCancelSequence(NTOUCH_LISTENER *plistener, NTOUCH_SEQUENCE *plistened_touchsequence, const NTOUCH_SEQUENCE *psource_touchsequence)
{
	NErrorIf(!plistener || plistened_touchsequence || psource_touchsequence, NERROR_NULL_VALUE);
	NErrorIf(ISFLAG_ON(plistener->Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN), NERROR_TOUCH_LISTENER_HAS_TO_BE_CLEAN_UP);

	// NErrorIf(!psource_touchsequence->TouchCount, NERROR_NULL_VALUE );								// Source sequence must have, at least one Touch !
	NErrorIf(plistened_touchsequence->TouchCount, NERROR_NON_NULL_VALUE);						 // listened sequence must be empty !
	NErrorIf(plistener->LTouchCount != plistener->LTouchBufferSize, NERROR_INCONSISTENT_VALUES); // Both must be equal at this stage ! ( the only moment when they are different is within touch End/Cancel processes )

	if (!plistener->LTouchCount)
		return 0;

	// Cancel All (without any STREAMED NTOUCH Data )
	if (psource_touchsequence->TouchCount == 0)
	{
		plistened_touchsequence->TouchCount = 0;
		plistener->PreviousLTouchCount = plistener->LTouchCount;
		plistener->LTouchCount = 0;
		plistener->LTouchBufferSize = 0;

		// Call to 'NCleanTouchListenerUp' is NOT requested here because "CANCEL ALL" consider there is no touch data to use
		// so the ListenedTouchBuffer is CLEANED UP here !
		// FLAG_ON(plistener->Flags,FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);

		return plistener->PreviousLTouchCount; // which is the number of Listened Touch updated in the buffer (all in that case)
	}
	// Cancel Some (maybe all, but STREAMED NTOUCH Data are existing !!! )
	else
	{
		Nu32 streamed_systemid;
		NLISTENED_TOUCH *plistened;
		Nu8 s, l;
		Nu8 *ptindex = (Nu8 *)psource_touchsequence->TouchIndex;
		NErrorIf(ptindex != &psource_touchsequence->TouchIndex[0], NERROR_SYSTEM_CHECK);
		for (s = psource_touchsequence->TouchCount; s != 0; s--, ptindex++)
		{
			// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// + In Debug Mode, check if incoming Touch is really in Phase CANCEL !
			NErrorIf(_STREAMTOUCH(*ptindex).Phase != NTOUCH_PHASE_CANCEL, NERROR_TOUCH_INCONSISTENT_PHASE);
			// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			streamed_systemid = _STREAMTOUCH(*ptindex).SystemID;
			plistened = plistener->pLTouchBuffer;
			for (l = plistener->LTouchBufferSize; l != 0; l--, plistened++)
			{
				// Match !
				if (streamed_systemid == plistened->SystemID)
				{
					// Listened associated NTOUCH should not have ..._END or _...CANCEL phase ! ( should have _START or _MOVE )
					NErrorIf(NIS_TOUCH_PHASE_INACTIVE(plistened->Phase), NERROR_TOUCH_INCONSISTENT_PHASE);

					// Build up the Listened Touch Sequence
					plistened_touchsequence->TouchIndex[plistened_touchsequence->TouchCount] = *ptindex;
					plistened_touchsequence->TouchCount++;

					// Update Listened Touch Data into the Listener Buffer !
					plistened->Phase = NTOUCH_PHASE_CANCEL;
					//		plistened->SystemID		= not updated;
					//		plistened->Flags		= not updated;
					//		plistened->User_Nu16	= not updated;
					break;
				}
			}

			// Are all the current Listened Touch in Buffer updated ?
			if (plistened_touchsequence->TouchCount == plistener->LTouchCount)
				break;
		}

		if (plistened_touchsequence->TouchCount)
		{
			NErrorIf(plistened_touchsequence->TouchCount > plistener->LTouchCount, NERROR_INCONSISTENT_VALUES); // ??? IT SOULD NEVER HAPPEN !
			plistener->PreviousLTouchCount = plistener->LTouchCount;
			plistener->LTouchCount -= plistened_touchsequence->TouchCount;

			// A call to 'NCleanTouchListenerUp' is requested ( just after potential 'listened touch sequence' using process ... )
			FLAG_ON(plistener->Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);

			// Return the exact number of listened touch(es) updated in the buffer
			return plistened_touchsequence->TouchCount;
		}
		else
		{
			return 0;
		}
	}
}

Nu8 NListenTouchEndSequence(NTOUCH_LISTENER *plistener, NTOUCH_SEQUENCE *plistened_touchsequence, const NTOUCH_SEQUENCE *psource_touchsequence)
{
	NErrorIf(!plistener || plistened_touchsequence || psource_touchsequence, NERROR_NULL_VALUE);
	NErrorIf(ISFLAG_ON(plistener->Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN), NERROR_TOUCH_LISTENER_HAS_TO_BE_CLEAN_UP);

	NErrorIf(!psource_touchsequence->TouchCount, NERROR_NULL_VALUE);							 // Source sequence must have, at least one Touch !
	NErrorIf(plistened_touchsequence->TouchCount, NERROR_NON_NULL_VALUE);						 // listened sequence must be empty !
	NErrorIf(plistener->LTouchCount != plistener->LTouchBufferSize, NERROR_INCONSISTENT_VALUES); // Both must be equal at this stage ! ( the only moment when they are different is within touch End/Cancel processes )

	if (!plistener->LTouchCount)
		return 0;

	Nu32 streamed_systemid;
	NLISTENED_TOUCH *plistened;
	Nu8 s, l;
	Nu8 *ptindex = (Nu8 *)psource_touchsequence->TouchIndex;
	NErrorIf(ptindex != (Nu8 *)&psource_touchsequence->TouchIndex[0], NERROR_SYSTEM_CHECK);
	for (s = psource_touchsequence->TouchCount; s != 0; s--, ptindex++)
	{
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// + In Debug Mode, check if incoming Touch is really in Phase END !
		NErrorIf(_STREAMTOUCH(*ptindex).Phase != NTOUCH_PHASE_END, NERROR_TOUCH_INCONSISTENT_PHASE);
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		streamed_systemid = _STREAMTOUCH(*ptindex).SystemID;
		plistened = plistener->pLTouchBuffer;
		for (l = plistener->LTouchBufferSize; l != 0; l--, plistened++)
		{
			// Match !
			if (streamed_systemid == plistened->SystemID)
			{
				// Listened associated NTOUCH should not have ..._END or _...CANCEL phase ! ( should have _START or _MOVE )
				NErrorIf(NIS_TOUCH_PHASE_INACTIVE(plistened->Phase), NERROR_TOUCH_INCONSISTENT_PHASE);

				// Build up the Listened Touch Sequence
				plistened_touchsequence->TouchIndex[plistened_touchsequence->TouchCount] = *ptindex;
				plistened_touchsequence->TouchCount++;

				// Update Listened Touch Data into the Listener Buffer !
				plistened->Phase = NTOUCH_PHASE_END;
				//		plistened->SystemID		= not updated;
				//		plistened->Flags		= not updated;
				//		plistened->User_Nu16	= not updated;
				break;
			}
		}

		// Are all the current Listened Touch in Buffer updated ?
		if (plistened_touchsequence->TouchCount == plistener->LTouchCount)
			break;
	}

	if (plistened_touchsequence->TouchCount)
	{
		NErrorIf(plistened_touchsequence->TouchCount > plistener->LTouchCount, NERROR_INCONSISTENT_VALUES); // ??? IT SOULD NEVER HAPPEN !
		plistener->PreviousLTouchCount = plistener->LTouchCount;
		plistener->LTouchCount -= plistened_touchsequence->TouchCount;

		// A call to 'NCleanTouchListenerUp' is requested ( just after potential 'listened touch sequence' using process ... )
		FLAG_ON(plistener->Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);

		// Return the exact number of listened touch(es) updated in the buffer
		return plistened_touchsequence->TouchCount;
	}
	else
	{
		return 0;
	}
}

/*Nu8*/ void NCleanTouchListenerUp(NTOUCH_LISTENER *plistener)
{
	// Is Clean Up requested ? If not, just return 0.
	if (ISFLAG_OFF(plistener->Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN))
	{
		NErrorIf(plistener->LTouchCount != plistener->LTouchBufferSize, NERROR_TOUCH_LISTENER_HAS_TO_BE_CLEAN_UP);
		// return 0;
		return;
	}

	NErrorIf(plistener->LTouchCount == plistener->LTouchBufferSize, NERROR_TOUCH_LISTENER_IS_ALREADY_CLEAN);

	// 'plistener->LTouchCount' may be <= 'plistener->LTouchBufferSize'
	// If 'plistener->LTouchBufferSize' is ZERO ! Just reset PreviousLTouchCount to ZERO ... (see some lines bottom )

	if (plistener->LTouchCount)
	{
		// Notes about "Clean loop":
		// Instead of looking for non-active touch into the whole buffer from the first to the last element with a "for" loop...
		// N is going to start from the first listened touch in the buffer and looks for a precise number of element to erase.
		// This precise number is the exact difference between 'LTouchCount' and 'LTouchBufferSize'.
		// These 2 values are always identical excepted when some non-active listened touch are in the buffer, and the difference
		// between those 2 values represents the number of non-active listened touch ... which is exactly what we are looking for !!!
		//
		Nu8 erased = 0;
		Nu8 nb = plistener->LTouchBufferSize - plistener->LTouchCount;
		NLISTENED_TOUCH *plistened = plistener->pLTouchBuffer;
		while (erased != nb)
		{
			if (NIS_TOUCH_PHASE_INACTIVE(plistened->Phase))
			{
				// Quick and Manual Erase:
				// Copy the last element of the buffer (index = LTouchBufferSize -1) 'ON' the erased element.
				// ... and reduce the buffer size by one ! ( So start by reducing the buffer size by one and use the value as index too ... just a simple trick )
				// Note: if plistened IS the last element of the buffer before erasing, is going to be copied on itself !!!
				//		 but it's not a problem ... IT'S BETTER to do like this, to staying simple and avoiding additionnal tests.
				plistener->LTouchBufferSize--;
				*plistened = *(plistener->pLTouchBuffer + plistener->LTouchBufferSize);

				// ... and increase by one the number of effective erase.
				erased++;
			}
			else
			{
				plistened++;
				// Number of Listened Touch to erase is going to be reached before 'plistened' goes OUT of the buffer limits.
				// If not, there is a SYSTEM PROBLEM
				NErrorIf((NBYTE *)plistened > ((NBYTE *)plistener->pLTouchBuffer + plistener->LTouchBufferSize * sizeof(NLISTENED_TOUCH)), NERROR_SYSTEM_CHECK);
			}
		}

		NErrorIf(plistener->LTouchCount != plistener->LTouchBufferSize, NERROR_SYSTEM_CHECK); // JUST IMPOSSIBLE !!!

		if (!plistener->LTouchBufferSize)
		{
			NErrorIf(plistener->LTouchCount || plistener->LTouchCountIn, NERROR_INCONSISTENT_VALUES);
			// Reset All !
			plistener->PreviousLTouchCount = 0;
			plistener->PreviousLTouchCountIn = 0;
			// 		plistener->LTouchCountIn			= 0; |- Already == 0 !
		}

		//		FLAG_OFF(plistener->Flags,FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);
		//		return erased;
	}
	// plistener->LTouchBufferSize == 0
	else
	{
		NErrorIf(plistener->LTouchCount || plistener->LTouchCountIn, NERROR_INCONSISTENT_VALUES);
		// Reset All !
		plistener->PreviousLTouchCount = 0;
		plistener->PreviousLTouchCountIn = 0;
		//		plistener->LTouchCount				= 0; |
		// 		plistener->LTouchCountIn			= 0; |- Already == 0 !
		plistener->LTouchBufferSize = 0;
		//		FLAG_OFF(plistener->Flags,FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);
		//		return 0;
	}

	FLAG_OFF(plistener->Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);
}

/*
Nu8 NListenTouchSequence(NTOUCH_LISTENER *plistener, const Nu8 touch_phase, NTOUCH_SEQUENCE *plistened_touchsequence, const NTOUCH_SEQUENCE *psource_touchsequence )
{

	switch( touch_phase )
	{
		case NTOUCH_PHASE_START:
			return NListenTouchStartSequence(plistener,plistened_touchsequence,psource_touchsequence);

		case NTOUCH_PHASE_MOVE:
			return NListenTouchMoveSequence(plistener,plistened_touchsequence,psource_touchsequence);

		case NTOUCH_PHASE_END:
			return NListenTouchEndSequence(plistener,plistened_touchsequence,psource_touchsequence);

		case NTOUCH_PHASE_CANCEL:
			return NListenTouchCancelSequence(plistener,plistened_touchsequence,psource_touchsequence);

		default:
			NErrorIf(1,NERROR_INVALID_CASE);
			return 0;
	}
}
*/

Nu8 NListenTouchEvent(NTOUCH_LISTENER *plistener, NEVENT *plistened_touchevent, const NEVENT *psrc_touch_event)
{
	NErrorIf(NGET_EVENT_MAIN(psrc_touch_event->Type) != _TOUCH, NERROR_EVENT_INCONSISTENT_TYPE);
	plistened_touchevent->Head = psrc_touch_event->Head;

	switch (psrc_touch_event->Type)
	{
	case NTOUCH_START:
		return NListenTouchStartSequence(plistener, &plistened_touchevent->Touch.TouchSequence, &psrc_touch_event->Touch.TouchSequence);

	case NTOUCH_MOVE:
		return NListenTouchMoveSequence(plistener, &plistened_touchevent->Touch.TouchSequence, &psrc_touch_event->Touch.TouchSequence);

	case NTOUCH_END:
		return NListenTouchEndSequence(plistener, &plistened_touchevent->Touch.TouchSequence, &psrc_touch_event->Touch.TouchSequence);

	case NTOUCH_CANCEL:
		return NListenTouchCancelSequence(plistener, &plistened_touchevent->Touch.TouchSequence, &psrc_touch_event->Touch.TouchSequence);

	default:
		NErrorIf(1, NERROR_INVALID_CASE);
		return 0;
	}
}
Nu8 NListenTouchEventMsk(NTOUCH_LISTENER *plistener, NEVENT *plistened_touchevent, const NEVENT *psrc_touch_event, const Nu16 touch_lockmask)
{
	NErrorIf(NGET_EVENT_MAIN(psrc_touch_event->Type) != _TOUCH, NERROR_EVENT_INCONSISTENT_TYPE);
	plistened_touchevent->Head = psrc_touch_event->Head;

	switch (psrc_touch_event->Type)
	{
	case NTOUCH_START:
		return NListenTouchStartSequenceMsk(plistener, &plistened_touchevent->Touch.TouchSequence, &psrc_touch_event->Touch.TouchSequence, touch_lockmask); // <--- There is only one difference with 'NListenTouchEvent' and it's just here !

	case NTOUCH_MOVE:
		return NListenTouchMoveSequence(plistener, &plistened_touchevent->Touch.TouchSequence, &psrc_touch_event->Touch.TouchSequence);

	case NTOUCH_END:
		return NListenTouchEndSequence(plistener, &plistened_touchevent->Touch.TouchSequence, &psrc_touch_event->Touch.TouchSequence);

	case NTOUCH_CANCEL:
		return NListenTouchCancelSequence(plistener, &plistened_touchevent->Touch.TouchSequence, &psrc_touch_event->Touch.TouchSequence);

	default:
		NErrorIf(1, NERROR_INVALID_CASE);
		return 0;
	}
}

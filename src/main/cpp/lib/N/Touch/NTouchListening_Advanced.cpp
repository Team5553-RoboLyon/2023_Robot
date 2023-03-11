#include "lib/N/NCStandard.h"
#include "lib/N/NErrorHandling.h"
#include "lib/N/Containers/NRingBuffer.h"
#include "NTouchStream.h"
#include "NTouch.h"
#include "NTouchListener.h"

NLISTENED_TOUCH *_get_listener_listened_touch(NTOUCH_LISTENER *plistener, const Nu32 touch_systemid)
{
	NLISTENED_TOUCH *plistened = plistener->pLTouchBuffer;
	for (Nu8 l = plistener->LTouchBufferSize; l != 0; l--, plistened++)
	{
		// Match !
		if (touch_systemid == plistened->SystemID)
		{
			return plistened;
		}
	}
	return NULL;
}

Nu8 NListenAndExtractTouchStartSequence(NTOUCH_LISTENER *plistener, NTOUCH_SEQUENCE *plistened_touchsequence, NTOUCH_SEQUENCE *psource_touchsequence)
{
	NErrorIf(!plistener || plistened_touchsequence || psource_touchsequence, NERROR_NULL_VALUE);
	NErrorIf(ISFLAG_ON(plistener->Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN), NERROR_TOUCH_LISTENER_HAS_TO_BE_CLEAN_UP);

	NErrorIf(!psource_touchsequence->TouchCount, NERROR_NULL_VALUE);							 // Source sequence must have, at least one Touch !
	NErrorIf(plistened_touchsequence->TouchCount, NERROR_NON_NULL_VALUE);						 // listened sequence must be empty !
	NErrorIf(plistener->LTouchCount != plistener->LTouchBufferSize, NERROR_INCONSISTENT_VALUES); // Both must be equal at this stage ! ( the only moment when they are different is within touch End/Cancel processes )

	if (plistener->LTouchCount == plistener->LTouchCountMax)
		return 0;

	Nu8 new_source_touch_count = 0;
	Nu8 *ptindex = psource_touchsequence->TouchIndex;
	for (Nu8 t = psource_touchsequence->TouchCount; t != 0; t--, ptindex++)
	{
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef _DEBUG
		// + In Debug Mode, check if incoming Touch is really in Phase START !
		NErrorIf(_STREAMTOUCH(*ptindex).Phase != NTOUCH_PHASE_START, NERROR_TOUCH_INCONSISTENT_PHASE);
		// + In debug Mode, check if incoming Touch is not already registered !!
		NLISTENED_TOUCH *plt = plistener->pLTouchBuffer;
		for (Nu8 i_dbg = plistener->LTouchBufferSize; i_dbg != 0; i_dbg--, plt++)
		{
			NErrorIf(plt->SystemID == _STREAMTOUCH(*ptindex).SystemID, NERROR_TOUCH_ALREADY_LISTEN);
		}
#endif
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (plistener->LTouchBufferSize < plistener->LTouchCountMax)
		{
			// Build up the Listened Touch Sequence
			plistened_touchsequence->TouchIndex[plistened_touchsequence->TouchCount] = *ptindex;
			plistened_touchsequence->TouchCount++;

			// Store the new Listened Touch Data into the Listener Buffer !
			plistener->pLTouchBuffer[plistener->LTouchBufferSize].SystemID = _STREAMTOUCH(*ptindex).SystemID;
			plistener->pLTouchBuffer[plistener->LTouchBufferSize].Phase = NTOUCH_PHASE_START;
			plistener->pLTouchBuffer[plistener->LTouchBufferSize].Flags = 0;
			plistener->pLTouchBuffer[plistener->LTouchBufferSize].User_Nu16 = 0;

			plistener->LTouchBufferSize++;
		}
		else
		{
			psource_touchsequence->TouchIndex[new_source_touch_count] = *ptindex;
			new_source_touch_count++;
		}
	}

	if (plistener->LTouchCount != plistener->LTouchBufferSize)
	{
		plistener->PreviousLTouchCount = plistener->LTouchCount;
		plistener->LTouchCount = plistener->LTouchBufferSize;

		// Update Source Touch Sequence Touch Count
		NErrorIf(plistened_touchsequence->TouchCount + new_source_touch_count != psource_touchsequence->TouchCount, NERROR_SYSTEM_CHECK); // Logical Error !!!
		psource_touchsequence->TouchCount = new_source_touch_count;
	}

	// Return the exact number of NEW listened touch(es) pushed back in the buffer
	return plistened_touchsequence->TouchCount;
}

Nu8 NListenAndExtractTouchStartSequenceMsk(NTOUCH_LISTENER *plistener, NTOUCH_SEQUENCE *plistened_touchsequence, NTOUCH_SEQUENCE *psource_touchsequence, const Nu16 touch_lockmask)
{
	NErrorIf(!plistener || plistened_touchsequence || psource_touchsequence, NERROR_NULL_VALUE);
	NErrorIf(ISFLAG_ON(plistener->Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN), NERROR_TOUCH_LISTENER_HAS_TO_BE_CLEAN_UP);

	NErrorIf(!psource_touchsequence->TouchCount, NERROR_NULL_VALUE);							 // Source sequence must have, at least one Touch !
	NErrorIf(plistened_touchsequence->TouchCount, NERROR_NON_NULL_VALUE);						 // listened sequence must be empty !
	NErrorIf(plistener->LTouchCount != plistener->LTouchBufferSize, NERROR_INCONSISTENT_VALUES); // Both must be equal at this stage ! ( the only moment when they are different is within touch End/Cancel processes )

	if (plistener->LTouchCount == plistener->LTouchCountMax)
		return 0;

	Nu8 new_source_touch_count = 0;
	Nu8 *ptindex = psource_touchsequence->TouchIndex;
	for (Nu8 t = psource_touchsequence->TouchCount; t != 0; t--, ptindex++)
	{
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef _DEBUG
		// + In Debug Mode, check if incoming Touch is really in Phase START !
		NErrorIf(_STREAMTOUCH(*ptindex).Phase != NTOUCH_PHASE_START, NERROR_TOUCH_INCONSISTENT_PHASE);
		// + In debug Mode, check if incoming Touch is not already registered !!
		NLISTENED_TOUCH *plt = plistener->pLTouchBuffer;
		for (Nu8 i_dbg = plistener->LTouchBufferSize; i_dbg != 0; i_dbg--, plt++)
		{
			NErrorIf(plt->SystemID == _STREAMTOUCH(*ptindex).SystemID, NERROR_TOUCH_ALREADY_LISTEN);
		}
#endif
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// With Lock Mask, only the TOUCH without "lock flag" are going to be listen !
		if (((_STREAMTOUCH(*ptindex).Flags & touch_lockmask) != touch_lockmask) && plistener->LTouchBufferSize < plistener->LTouchCountMax)
		{
			// Build up the Listened Touch Sequence
			plistened_touchsequence->TouchIndex[plistened_touchsequence->TouchCount] = *ptindex;
			plistened_touchsequence->TouchCount++;

			// Store the new Listened Touch Data into the Listener Buffer !
			plistener->pLTouchBuffer[plistener->LTouchBufferSize].SystemID = _STREAMTOUCH(*ptindex).SystemID;
			plistener->pLTouchBuffer[plistener->LTouchBufferSize].Phase = NTOUCH_PHASE_START;
			plistener->pLTouchBuffer[plistener->LTouchBufferSize].Flags = 0;
			plistener->pLTouchBuffer[plistener->LTouchBufferSize].User_Nu16 = 0;

			plistener->LTouchBufferSize++;
		}
		else
		{
			psource_touchsequence->TouchIndex[new_source_touch_count] = *ptindex;
			new_source_touch_count++;
		}
	}

	if (plistener->LTouchCount != plistener->LTouchBufferSize)
	{
		plistener->PreviousLTouchCount = plistener->LTouchCount;
		plistener->LTouchCount = plistener->LTouchBufferSize;

		// Update Source Touch Sequence Touch Count
		NErrorIf(plistened_touchsequence->TouchCount + new_source_touch_count != psource_touchsequence->TouchCount, NERROR_SYSTEM_CHECK); // Logical Error !!! ENGINE BIG BUG !!!
		psource_touchsequence->TouchCount = new_source_touch_count;
	}

	// Return the exact number of NEW listened touch(es) pushed back in the buffer
	return plistened_touchsequence->TouchCount;
}

Nu8 NListenAndExtractTouchMoveSequence(NTOUCH_LISTENER *plistener, NTOUCH_SEQUENCE *plistened_touchsequence, NTOUCH_SEQUENCE *psource_touchsequence)
{
	NErrorIf(!plistener || plistened_touchsequence || psource_touchsequence, NERROR_NULL_VALUE);
	NErrorIf(ISFLAG_ON(plistener->Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN), NERROR_TOUCH_LISTENER_HAS_TO_BE_CLEAN_UP);

	NErrorIf(!psource_touchsequence->TouchCount, NERROR_NULL_VALUE);							 // Source sequence must have, at least one Touch !
	NErrorIf(plistened_touchsequence->TouchCount, NERROR_NON_NULL_VALUE);						 // listened sequence must be empty !
	NErrorIf(plistener->LTouchCount != plistener->LTouchBufferSize, NERROR_INCONSISTENT_VALUES); // Both must be equal at this stage ! ( the only moment when they are different is within touch End/Cancel processes )

	if (!plistener->LTouchCount)
		return 0;

	Nu8 new_source_touch_count = 0;
	NLISTENED_TOUCH *plistened;
	Nu8 s;
	Nu8 *ptindex = psource_touchsequence->TouchIndex;
	for (s = psource_touchsequence->TouchCount; s != 0; s--, ptindex++)
	{
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// + In Debug Mode, check if incoming Touch is really in Phase MOVE !
		NErrorIf(_STREAMTOUCH(*ptindex).Phase != NTOUCH_PHASE_MOVE, NERROR_TOUCH_INCONSISTENT_PHASE);
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if ((plistened = _get_listener_listened_touch(plistener, _STREAMTOUCH(*ptindex).SystemID)))
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

			// Are all the current Listened Touch in Buffer updated ?
			if (plistened_touchsequence->TouchCount == plistener->LTouchCount)
				break;
		}
		else
		{
			psource_touchsequence->TouchIndex[new_source_touch_count] = *ptindex;
			new_source_touch_count++;
		}
	}

	// Update Source Touch Sequence Touch Count
	psource_touchsequence->TouchCount -= plistened_touchsequence->TouchCount;
	NErrorIf(psource_touchsequence->TouchCount != new_source_touch_count, NERROR_SYSTEM_CHECK); // Logical Error !!! ENGINE BIG BUG !!!

	// Return the exact number of listened touch(es) updated in the buffer
	return plistened_touchsequence->TouchCount;
}

Nu8 NListenAndExtractTouchEndSequence(NTOUCH_LISTENER *plistener, NTOUCH_SEQUENCE *plistened_touchsequence, NTOUCH_SEQUENCE *psource_touchsequence)
{
	NErrorIf(!plistener || plistened_touchsequence || psource_touchsequence, NERROR_NULL_VALUE);
	NErrorIf(ISFLAG_ON(plistener->Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN), NERROR_TOUCH_LISTENER_HAS_TO_BE_CLEAN_UP);

	NErrorIf(!psource_touchsequence->TouchCount, NERROR_NULL_VALUE);							 // Source sequence must have, at least one Touch !
	NErrorIf(plistened_touchsequence->TouchCount, NERROR_NON_NULL_VALUE);						 // listened sequence must be empty !
	NErrorIf(plistener->LTouchCount != plistener->LTouchBufferSize, NERROR_INCONSISTENT_VALUES); // Both must be equal at this stage ! ( the only moment when they are different is within touch End/Cancel processes )

	if (!plistener->LTouchCount)
		return 0;

	NLISTENED_TOUCH *plistened;
	Nu8 s;
	Nu8 new_source_touch_count = 0;
	Nu8 *ptindex = psource_touchsequence->TouchIndex;
	for (s = psource_touchsequence->TouchCount; s != 0; s--, ptindex++)
	{
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// + In Debug Mode, check if incoming Touch is really in Phase END !
		NErrorIf(_STREAMTOUCH(*ptindex).Phase != NTOUCH_PHASE_END, NERROR_TOUCH_INCONSISTENT_PHASE);
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if ((plistened = _get_listener_listened_touch(plistener, _STREAMTOUCH(*ptindex).SystemID)))
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

			// Are all the current Listened Touch in Buffer updated ?
			if (plistened_touchsequence->TouchCount == plistener->LTouchCount)
				break;
		}
		else
		{
			psource_touchsequence->TouchIndex[new_source_touch_count] = *ptindex;
			new_source_touch_count++;
		}
	}

	if (plistened_touchsequence->TouchCount)
	{
		NErrorIf(plistened_touchsequence->TouchCount > plistener->LTouchCount, NERROR_INCONSISTENT_VALUES); // ??? IT SOULD NEVER HAPPEN !
		plistener->PreviousLTouchCount = plistener->LTouchCount;
		plistener->LTouchCount -= plistened_touchsequence->TouchCount;

		// A call to 'NCleanTouchListenerUp' is requested ( just after potential 'listened touch sequence' using process ... )
		FLAG_ON(plistener->Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);

		// Update Source Touch Sequence Touch Count
		NErrorIf(plistened_touchsequence->TouchCount + new_source_touch_count != psource_touchsequence->TouchCount, NERROR_SYSTEM_CHECK); // Logical Error !!! ENGINE BIG BUG !!!
		psource_touchsequence->TouchCount = new_source_touch_count;

		// Return the exact number of listened touch(es) updated in the buffer
		return plistened_touchsequence->TouchCount;
	}
	else
	{
		return 0;
	}
}

Nu8 NListenAndExtractTouchCancelSequence(NTOUCH_LISTENER *plistener, NTOUCH_SEQUENCE *plistened_touchsequence, NTOUCH_SEQUENCE *psource_touchsequence)
{
	NErrorIf(!plistener || plistened_touchsequence || psource_touchsequence, NERROR_NULL_VALUE);
	NErrorIf(ISFLAG_ON(plistener->Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN), NERROR_TOUCH_LISTENER_HAS_TO_BE_CLEAN_UP);

	NErrorIf(!psource_touchsequence->TouchCount, NERROR_NULL_VALUE);							 // Source sequence must have, at least one Touch !
	NErrorIf(plistened_touchsequence->TouchCount, NERROR_NON_NULL_VALUE);						 // listened sequence must be empty !
	NErrorIf(plistener->LTouchCount != plistener->LTouchBufferSize, NERROR_INCONSISTENT_VALUES); // Both must be equal at this stage ! ( the only moment when they are different is within touch End/Cancel processes )

	if (!plistener->LTouchCount)
		return 0;

	// Cancel All (without any STREAMED NTOUCH Data )
	// Note for this special case, psource_touchsequence still the same ( obviously it's not possible to extract listened touch from a touch sequence without any touch ... )
	// ... and plistened_touchsequence is going to have ZERO element too ...
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
		NLISTENED_TOUCH *plistened;
		Nu8 s;
		Nu8 new_source_touch_count = 0;
		Nu8 *ptindex = psource_touchsequence->TouchIndex;
		for (s = psource_touchsequence->TouchCount; s != 0; s--, ptindex++)
		{
			// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// + In Debug Mode, check if incoming Touch is really in Phase CANCEL !
			NErrorIf(_STREAMTOUCH(*ptindex).Phase != NTOUCH_PHASE_CANCEL, NERROR_TOUCH_INCONSISTENT_PHASE);
			// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if ((plistened = _get_listener_listened_touch(plistener, _STREAMTOUCH(*ptindex).SystemID)))
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

				// Are all the current Listened Touch in Buffer updated ?
				if (plistened_touchsequence->TouchCount == plistener->LTouchCount)
					break;
			}
			else
			{
				psource_touchsequence->TouchIndex[new_source_touch_count] = *ptindex;
				new_source_touch_count++;
			}
		}

		if (plistened_touchsequence->TouchCount)
		{
			NErrorIf(plistened_touchsequence->TouchCount > plistener->LTouchCount, NERROR_INCONSISTENT_VALUES); // ??? IT SOULD NEVER HAPPEN !
			plistener->PreviousLTouchCount = plistener->LTouchCount;
			plistener->LTouchCount -= plistened_touchsequence->TouchCount;

			// A call to 'NCleanTouchListenerUp' is requested ( just after potential 'listened touch sequence' using process ... )
			FLAG_ON(plistener->Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);

			// Update Source Touch Sequence Touch Count
			NErrorIf(plistened_touchsequence->TouchCount + new_source_touch_count != psource_touchsequence->TouchCount, NERROR_SYSTEM_CHECK); // Logical Error !!! ENGINE BIG BUG !!!
			psource_touchsequence->TouchCount = new_source_touch_count;

			// Return the exact number of listened touch(es) updated in the buffer
			return plistened_touchsequence->TouchCount;
		}
		else
		{
			return 0;
		}
	}
}
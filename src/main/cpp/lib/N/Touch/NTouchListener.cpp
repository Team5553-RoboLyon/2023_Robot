#include "lib/N/NCStandard.h"
#include "lib/N/NErrorHandling.h"
#include "NTouch.h"
#include "NTouchListener.h"

NTOUCH_LISTENER *NSetupTouchListener(NTOUCH_LISTENER *plistener, const Nu8 capacity)
{
	NErrorIf(!plistener, NERROR_NULL_POINTER);
	Nmem0(plistener, NTOUCH_LISTENER);
	if (capacity)
	{
		plistener->pLTouchBuffer = (NLISTENED_TOUCH *)Nmalloc_mem0(capacity * sizeof(NLISTENED_TOUCH));
		plistener->LTouchBufferCapacity = capacity;
	}
	return plistener;
}

NTOUCH_LISTENER *NCreateTouchListener(const Nu8 capacity)
{
	return NSetupTouchListener(NEW(NTOUCH_LISTENER), capacity);
}

void NClearTouchListener(NTOUCH_LISTENER *plistener)
{
	// A listener available for clearing must be:
	//		Without any Listened Touch registered
	NErrorIf(plistener->LTouchCount || plistener->LTouchCountIn, NERROR_NON_NULL_VALUE);

	Nfree(plistener->pLTouchBuffer);
	Nmem0(plistener, NTOUCH_LISTENER);
}

void NDeleteTouchListener(NTOUCH_LISTENER *plistener)
{
	NClearTouchListener(plistener);
	Nfree(plistener);
}

void NSetTouchListenerTouchCountMax(NTOUCH_LISTENER *plistener, const Nu8 count_max)
{
	NErrorIf(count_max > CONSTANT_NTOUCH_COUNT_MAX, NERROR_VALUE_OUTOFRANGE);

	// A listener available for updating touch count max must be:
	//		Without any Listened Touch registered
	NErrorIf(plistener->LTouchCount || plistener->LTouchCountIn, NERROR_NON_NULL_VALUE);

	if (count_max > plistener->LTouchBufferCapacity)
	{
		plistener->pLTouchBuffer = (NLISTENED_TOUCH *)Nrealloc(plistener->pLTouchBuffer, count_max * sizeof(NLISTENED_TOUCH));
		plistener->LTouchBufferCapacity = count_max;
	}
	plistener->LTouchCountMax = count_max;
}

Nu8 NTouchListenerCancelAll(NTOUCH_LISTENER *plistener)
{
	plistener->PreviousLTouchCount = plistener->LTouchCount;
	plistener->PreviousLTouchCountIn = plistener->LTouchCountIn;
	plistener->LTouchCount = 0;
	plistener->LTouchCountIn = 0;
	plistener->LTouchBufferSize = 0;

	FLAG_ON(plistener->Flags, FLAG_NTOUCH_LISTENER_TO_BE_CLEAN);

	return plistener->PreviousLTouchCount; // which is the number of Listened Touch updated in the buffer (all in that case)
}

void NCopyTouchListener(NTOUCH_LISTENER *pdst, const NTOUCH_LISTENER *psrc)
{
	if (pdst->LTouchBufferCapacity != psrc->LTouchBufferCapacity)
	{
		pdst->LTouchBufferCapacity = psrc->LTouchBufferCapacity;
		pdst->pLTouchBuffer = (NLISTENED_TOUCH *)Nrealloc(pdst->pLTouchBuffer, pdst->LTouchBufferCapacity * sizeof(NLISTENED_TOUCH));
	}
	pdst->LTouchCount = psrc->LTouchCount;
	pdst->PreviousLTouchCount = psrc->PreviousLTouchCount;
	pdst->LTouchCountIn = psrc->LTouchCountIn;
	pdst->PreviousLTouchCountIn = psrc->PreviousLTouchCountIn;
	pdst->LTouchCountMax = psrc->LTouchCountMax;
	pdst->LTouchBufferSize = psrc->LTouchBufferSize;
	pdst->Flags = psrc->Flags;

	memcpy(pdst->pLTouchBuffer, psrc->pLTouchBuffer, pdst->LTouchBufferSize * sizeof(NLISTENED_TOUCH));
}
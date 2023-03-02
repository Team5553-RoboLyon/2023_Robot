// #if defined _NIOS || defined _NANDROID
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NErrorHandling.h"
#include "lib/N/NCore.h"
#include "lib/N/Containers/NRingBuffer.h"
#include "lib/N/Event/NEvent.h"
#include "lib/N/Event/NEventBuffer.h"

extern Nu32 NEngineCoreFlags;

NRINGBUFFER NEventBuffer;

void NInitEventBuffer()
{
	// A)Task Buffer
	NSetupRingBuffer(&NEventBuffer, CONSTANT_EVENT_BUFFER_CAPACITY, sizeof(NEVENT));
}

static void _clear_event_in_buffer(void *ptr)
{
	Nmem0(ptr, NEVENT);
}

void NQuitEventBuffer()
{
	// A)Event Buffer
	NClearRingBuffer(&NEventBuffer, NULL /*_clear_event_in_buffer*/);
}

//
NEVENT *NEventAllocBack()
{
	return (NEVENT *)NRingBufferAllocBack(&NEventBuffer);
}

void NFreePolledEvent()
{
	// NRingBufferFreeFrontXtd(&NEventBuffer,_clear_event_in_buffer);
	NRingBufferFreeFront(&NEventBuffer);
}

NEVENT *NPollEvent()
{
	if (NEventBuffer.Size)
	{
		return (NEVENT *)NRingBufferGetFront(&NEventBuffer);
	}
	else
	{
		return NULL;
	}
}
// #endif //_NIOS || _NANDROID

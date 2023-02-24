#ifndef __NRING_BUFFER_H
#define __NRING_BUFFER_H

#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NMath.h"
#include "lib/N/NErrorHandling.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

	// ***************************************************************************************
	// ***************************************************************************************
	// **																					**
	// **								   NRingBuffer.h									**
	// **																					**
	// ***************************************************************************************
	// ***************************************************************************************
	typedef void (*NRINGBUFFER_ELEMENT_DESTRUCTOR_CALLBACK)(void *pelement);

	/*
	first 4 members like NARRAY ... Maybe a good idea to rearrange NRINGBUFFER members like this ...
	typedef struct
	{
		void *pBuffer;		// Data buffer
		Nu32 Size;			// Current number of elements in the buffer
		Nu32 Capacity;		// Maximum number of elements in the buffer
		Nu32 ElementSize;	// Size of each element in the buffer

		void *pLast;		// Last available element address in the buffer.
		Nu32 Head;			// Index to head
		Nu32 Tail;			// Index to tail
		void *pHead;		// Pointer to head
		void *pTail;		// Pointer to tail
	}NRINGBUFFER;
	*/
	// ***************************************************************************************
	// **						Structures													**
	// ***************************************************************************************
	// A Ring Buffer Structure based on Pointers
	typedef struct
	{
		void *pBuffer;	  // Data buffer
		void *pLast;	  // Last available element address in the buffer.
		Nu32 Head;		  // Index to head
		Nu32 Tail;		  // Index to tail
		void *pHead;	  // Pointer to head
		void *pTail;	  // Pointer to tail
		Nu32 Capacity;	  // Maximum number of elements in the buffer
		Nu32 Size;		  // Current number of elements in the buffer
		Nu32 ElementSize; // Size of each element in the buffer
	} NRINGBUFFER;

	// ***************************************************************************************
	// **						FUNCTIONS													**
	// ***************************************************************************************
	NRINGBUFFER *NSetupRingBuffer(NRINGBUFFER *pringbuffer, const Nu32 capacity, const Nu32 element_size);
	NRINGBUFFER *NCreateRingBuffer(const Nu32 capacity, const Nu32 element_size);
	void NClearRingBuffer(NRINGBUFFER *pringbuffer, NRINGBUFFER_ELEMENT_DESTRUCTOR_CALLBACK element_destructor_callback);
	void NDeleteRingBuffer(NRINGBUFFER *pringbuffer, NRINGBUFFER_ELEMENT_DESTRUCTOR_CALLBACK element_destructor_callback);

	// Erase all Ring Buffer Content (all elements. Prefer using 'Xtd' version if each element need to perform some clear by itself )
	void NEraseRingBuffer(NRINGBUFFER *pringbuffer);
	void NEraseRingBufferXtd(NRINGBUFFER *pringbuffer, NRINGBUFFER_ELEMENT_DESTRUCTOR_CALLBACK element_destructor_callback);

	inline Nu32 NGetRingBufferCapacity(const NRINGBUFFER *pringbuffer) { return pringbuffer->Capacity; }
	inline Nu32 NGetRingBufferSize(const NRINGBUFFER *pringbuffer) { return pringbuffer->Size; }
	inline Nu32 NGetRingBufferElementSize(const NRINGBUFFER *pringbuffer) { return pringbuffer->ElementSize; }

	inline Nbool NIsValidRingBufferAbsoluteIndex(const NRINGBUFFER *pringbuffer, const Nu32 id)
	{
		// return ( (NSIGN( (Ns32)id-(Ns32)pringbuffer->Head)  == NSIGN( (Ns32)id-(Ns32)pringbuffer->Tail)) ? 0:1 );
		if (pringbuffer->Head > pringbuffer->Tail)
		{
			return ((id >= pringbuffer->Tail) && (id < pringbuffer->Head));
		}
		else // pringbuffer->Head <= pringbuffer->Tail
		{
			return ((id >= pringbuffer->Tail) || (id < pringbuffer->Head));
		}
	}

	inline Nbool NIsValidRingBufferRelativeIndex(const NRINGBUFFER *pringbuffer, const Nu32 id)
	{
		Nu32 absid = id + pringbuffer->Capacity;
		return (NSIGN(absid - (pringbuffer)->Head) == (NSIGN(absid - (pringbuffer)->Tail)) ? 0 : 1);
	}

	inline void *NGetRingBufferAbsoluteIndexPtr(const NRINGBUFFER *pringbuffer, const Nu32 id)
	{
		NErrorIf(!NIsValidRingBufferAbsoluteIndex(pringbuffer, id), NERROR_RINGBUFFER_INDEX_OUTOFRANGE);
		return (NBYTE *)pringbuffer->pBuffer + id * pringbuffer->ElementSize;
	}
	inline void *NGetRingBufferHeadRelativeIndexPtr(const NRINGBUFFER *pringbuffer, const Nu32 id)
	{
		NErrorIf(!NIsValidRingBufferRelativeIndex(pringbuffer, id), NERROR_RINGBUFFER_INDEX_OUTOFRANGE);
		return (NBYTE *)pringbuffer->pBuffer + ((pringbuffer->Head + id) % pringbuffer->Capacity) * pringbuffer->ElementSize;
	}

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Add element into the Ring Buffer.
	// +	There are 3 Mains ways to do with 4 versions for each of them.
	// +
	// + The 5 Main ways are:
	//		PushBack			--> Basic.		Copy incoming element. No return value
	//		AllocBack			--> Quicker.	No incoming Element (No copy). Return Buffer Element Address.
	//		AllocBackI			--> Quicker.	No incoming Element (No copy). Return Buffer Element Absolute Index (relative to the Absolut beginning of the Buffer).
	//		PushBackXtd			--> Extended.	Copy incoming element. Return Buffer Element Address.
	//		PushBackXtdI		--> ExtendedI.	Copy incoming element.  Return Buffer Element Absolute Index (relative to the Absolut beginning of the Buffer).
	//
	// + The 4 versions available for each of these 5 Main ways are:
	//
	//		_Basic version doesn't manage overflow and let user take care of it ! Better to avoid !
	//		_DropLatest			manages overflow by avoiding allocation. In that case function returns NULL instead of a valid pointer
	//							on buffer element location.
	//		_OverwriteOldest	manages overflow by deleting the oldest element of the buffer to get back one element space to allocate
	//							the new one. !!! WARNING !!! This function doesn't call any destructor before removing the oldest element !!!
	//							Prefer using "_OverwriteOldestXtd" to call the destructor
	//		_OverwriteOldestXtd	manages overflow like _OverwriteOldest BUT calls the element callback destructor before removing the oldest element !!!

	// "...PushBack" add one element at the RingBuffer head and copy the content of incoming 'Element'.
	void NRingBufferPushBack(NRINGBUFFER *pringbuffer, void *pelement);
	void NRingBufferPushBack_DropLatest(NRINGBUFFER *pringbuffer, void *pelement);
	void NRingBufferPushBack_OverwriteOldest(NRINGBUFFER *pringbuffer, void *pelement);
	void NRingBufferPushBack_OverwriteOldestXtd(NRINGBUFFER *pringbuffer, void *pelement, NRINGBUFFER_ELEMENT_DESTRUCTOR_CALLBACK element_destructor_callback);

	// "...AllocBack" allocate memory space with a size of one element at the head of the RingBuffer and returns its memory address.
	void *NRingBufferAllocBack(NRINGBUFFER *pringbuffer);
	void *NRingBufferAllocBack_DropLatest(NRINGBUFFER *pringbuffer);
	void *NRingBufferAllocBack_OverwriteOldest(NRINGBUFFER *pringbuffer);
	void *NRingBufferAllocBack_OverwriteOldestXtd(NRINGBUFFER *pringbuffer, NRINGBUFFER_ELEMENT_DESTRUCTOR_CALLBACK element_destructor_callback);

	// "...AllocBackI" allocate memory space with a size of one element at the head of the RingBuffer AND returns its Absolute Buffer Index ( relative to the Absolute beginning of the Buffer).
	Nu32 NRingBufferAllocBackI(NRINGBUFFER *pringbuffer);
	Nu32 NRingBufferAllocBackI_DropLatest(NRINGBUFFER *pringbuffer);
	Nu32 NRingBufferAllocBackI_OverwriteOldest(NRINGBUFFER *pringbuffer);
	Nu32 NRingBufferAllocBackI_OverwriteOldestXtd(NRINGBUFFER *pringbuffer, NRINGBUFFER_ELEMENT_DESTRUCTOR_CALLBACK element_destructor_callback);

	// "...PushBackXtd" add one element at the RingBuffer head, copy the content of incoming 'Element' into  AND returns the memory address of the Buffer element (like AllocBack).
	void *NRingBufferPushBackXtd(NRINGBUFFER *pringbuffer, void *pelement);
	void *NRingBufferPushBackXtd_DropLatest(NRINGBUFFER *pringbuffer, void *pelement);
	void *NRingBufferPushBackXtd_OverwriteOldest(NRINGBUFFER *pringbuffer, void *pelement);
	void *NRingBufferPushBackXtd_OverwriteOldestXtd(NRINGBUFFER *pringbuffer, void *pelement, NRINGBUFFER_ELEMENT_DESTRUCTOR_CALLBACK element_destructor_callback);

	// "...PushBackXtdI" add one element at the RingBuffer head, copy the content of incoming 'Element' into  AND returns its Absolute Buffer Index. (relative to the Absolute beginning of the Buffer).
	Nu32 NRingBufferPushBackXtdI(NRINGBUFFER *pringbuffer, void *pelement);
	Nu32 NRingBufferPushBackXtdI_DropLatest(NRINGBUFFER *pringbuffer, void *pelement);
	Nu32 NRingBufferPushBackXtdI_OverwriteOldest(NRINGBUFFER *pringbuffer, void *pelement);
	Nu32 NRingBufferPushBackXtdI_OverwriteOldestXtd(NRINGBUFFER *pringbuffer, void *pelement, NRINGBUFFER_ELEMENT_DESTRUCTOR_CALLBACK element_destructor_callback);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + Retrieve element from the Ring Buffer.
	// +
	inline void *NRingBufferGetFront(NRINGBUFFER *pringbuffer) { return pringbuffer->pTail; }
	inline Nu32 NRingBufferGetFrontI(NRINGBUFFER *pringbuffer) { return pringbuffer->Tail; }

	void NRingBufferPopFront(NRINGBUFFER *pringbuffer, void *pelement);
	void NRingBufferFreeFront(NRINGBUFFER *pringbuffer);
	void NRingBufferFreeFrontXtd(NRINGBUFFER *pringbuffer, NRINGBUFFER_ELEMENT_DESTRUCTOR_CALLBACK element_destructor_callback);
	void NRingBufferFreeRangeFront(NRINGBUFFER *pringbuffer, const Nu32 range_size);
	void NRingBufferFreeRangeFrontXtd(NRINGBUFFER *pringbuffer, const Nu32 range_size, NRINGBUFFER_ELEMENT_DESTRUCTOR_CALLBACK element_destructor_callback);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif //  __NRING_BUFFER_H

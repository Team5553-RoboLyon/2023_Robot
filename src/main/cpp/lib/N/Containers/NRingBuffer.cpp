// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								   NRingBuffer.cpp									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************

#include "../NCStandard.h"
#include "../NType.h"

#include "NRingBuffer.h"

#define _RB_HEAD_WRITE(prb,pin)										\
	memcpy((prb)->pHead,pin,(prb)->ElementSize);				

#define _RB_HEAD_ONE_STEP_FORWARD(prb)								\
	if( (prb)->pHead == (prb)->pLast ){(prb)->pHead=(prb)->pBuffer;(prb)->Head=0;}else{(prb)->pHead = (NBYTE*)(prb)->pHead + (prb)->ElementSize;(prb)->Head++;}		

#define _RB_SIZE_UP(prb)											\
	(prb)->Size ++;

#define _RB_TAIL_READ(prb,pout)										\
	memcpy(pout,(prb)->pTail,(prb)->ElementSize);				

#define _RB_TAIL_ONE_STEP_FORWARD(prb)								\
	if( (prb)->pTail == (prb)->pLast ){(prb)->pTail=(prb)->pBuffer;(prb)->Tail=0;}else{(prb)->pTail = (NBYTE*)(prb)->pTail + (prb)->ElementSize;(prb)->Tail++;}	

#define _RB_TAIL_DESTROY(prb,destroy_callback)						\
	if(destroy_callback){destroy_callback((prb)->pTail);}

#define _RB_SIZE_DOWN(prb)											\
	(prb)->Size --;


NRINGBUFFER*	NSetupRingBuffer(NRINGBUFFER *pringbuffer, const Nu32 capacity,const Nu32 element_size )
{
	if(capacity)
	{
		pringbuffer->pBuffer = Nmalloc(capacity*element_size);
		//memset(pringbuffer->pBuffer,0,capacity*element_size);
		pringbuffer->pLast	 = (NBYTE*)pringbuffer->pBuffer + (capacity-1)*element_size;
	}
	else
	{
		pringbuffer->pBuffer = NULL; // We don't allocate any memory if capacity == 0 !
		pringbuffer->pLast	 = NULL;
	}
	pringbuffer->pHead			= pringbuffer->pBuffer;
	pringbuffer->pTail			= pringbuffer->pBuffer;
	pringbuffer->Head			= 0;
	pringbuffer->Tail			= 0;

	pringbuffer->ElementSize	= element_size;
	pringbuffer->Capacity		= capacity;
	pringbuffer->Size			= 0;
	return pringbuffer;
}

NRINGBUFFER* NCreateRingBuffer(const Nu32 capacity,const Nu32 element_size )
{
	return NSetupRingBuffer(NEW(NRINGBUFFER),capacity,element_size);
}

void NClearRingBuffer(NRINGBUFFER *pringbuffer, NRINGBUFFER_ELEMENT_DESTRUCTOR_CALLBACK element_destructor_callback)
{
	if(element_destructor_callback)
	{
		void	*ptr = pringbuffer->pTail;
		for( Nu32 i=pringbuffer->Size;i!=0;i--,ptr = (NBYTE*)ptr+pringbuffer->ElementSize )
		{
			element_destructor_callback(ptr);
		}
	}
	// Free memory
	Nfree(pringbuffer->pBuffer);
	//... and set all parameters to 0 ... 
/*
	pringbuffer->pBuffer		= NULL;
	pringbuffer->pEnd			= NULL;
	pringbuffer->pHead			= NULL;
	pringbuffer->pTail			= NULL;
	pringbuffer->Head			= 0;
	pringbuffer->Tail			= 0;

	pringbuffer->ElementSize	= NULL;
	pringbuffer->Capacity		= NULL;
	pringbuffer->Size			= 0;
*/
	Nmem0(pringbuffer,NRINGBUFFER);
}
void NDeleteRingBuffer(NRINGBUFFER *pringbuffer, NRINGBUFFER_ELEMENT_DESTRUCTOR_CALLBACK element_destructor_callback)
{
	NClearRingBuffer(pringbuffer,element_destructor_callback);
	Nfree(pringbuffer);
}


void NEraseRingBuffer(NRINGBUFFER *pringbuffer)
{
	pringbuffer->pHead	= pringbuffer->pBuffer;
	pringbuffer->pTail	= pringbuffer->pBuffer;
	pringbuffer->Head	= 0;
	pringbuffer->Tail	= 0;
	pringbuffer->Size	= 0;
}


void NEraseRingBufferXtd(NRINGBUFFER *pringbuffer, NRINGBUFFER_ELEMENT_DESTRUCTOR_CALLBACK element_destructor_callback)
{
	if(element_destructor_callback)
	{
		void	*ptr = pringbuffer->pTail;
		for( Nu32 i=pringbuffer->Size;i!=0;i--,ptr = (NBYTE*)ptr+pringbuffer->ElementSize )
		{
			element_destructor_callback(ptr);
		}
	}
	pringbuffer->pHead	= pringbuffer->pBuffer;
	pringbuffer->pTail	= pringbuffer->pBuffer;
	pringbuffer->Head	= 0;
	pringbuffer->Tail	= 0;
	pringbuffer->Size	= 0;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + Add element into the Ring Buffer.
// +	There are 3 Mains ways to do with 4 versions for each of them.
// +
// + The 5 Main ways are: 
//		PushBack			--> Basic.		Copy incoming element. No return value
//		AllocBack			--> Quicker.	No incoming Element (No copy). Return Buffer Element Address.
//		AllocBackI			--> Quicker.	No incoming Element (No copy). Return Buffer Element Index relative to the Absolute beginning of the Buffer.
//		PushBackXtd			--> Extended.	Copy incoming element. Return Buffer Element Address.
//		PushBackXtdI		--> ExtendedI.	Copy incoming element.  Return Buffer Element Index relative to the Absolut beginning of the Buffer.
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


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + RINGBUFFER PUSH BACK
// +
// + Push a new element at Ring Buffer Head (make a copy of new incoming element). 
// +
void NRingBufferPushBack(NRINGBUFFER *pringbuffer,void *pelement)
{
	// This function doesn't Handle Overflow. User has to manage by himself...
	// If user doesn't want to manage overflow he should use... 'NRingBufferPushBack_DropLatest' or 'NRingBufferPushBack_OverwriteOldest'
	NErrorIf(pringbuffer->Size == pringbuffer->Capacity, NERROR_RINGBUFFER_OVERFLOW); 
	_RB_HEAD_WRITE(pringbuffer,pelement)
	_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
	_RB_SIZE_UP(pringbuffer)
}

void NRingBufferPushBack_DropLatest(NRINGBUFFER *pringbuffer,void *pelement)
{
	// Overflow condition
	if( pringbuffer->Size < pringbuffer->Capacity )
	{
		_RB_HEAD_WRITE(pringbuffer,pelement)
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
		_RB_SIZE_UP(pringbuffer)
	}
}

void NRingBufferPushBack_OverwriteOldest(NRINGBUFFER *pringbuffer,void *pelement)
{
	// Overflow condition
	if( pringbuffer->Size == pringbuffer->Capacity )
	{
		// Tail data is "removed" without any call of destructor_callback ! 
		// Use "NRingBufferPushBack_OverwriteOldestXtd" if you need a destructor_callback
		_RB_TAIL_ONE_STEP_FORWARD(pringbuffer)	
		_RB_HEAD_WRITE(pringbuffer,pelement)	
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
	}
	else
	{
		_RB_HEAD_WRITE(pringbuffer,pelement)
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
		_RB_SIZE_UP(pringbuffer)
	}
}

void NRingBufferPushBack_OverwriteOldestXtd(NRINGBUFFER *pringbuffer,void *pelement, NRINGBUFFER_ELEMENT_DESTRUCTOR_CALLBACK element_destructor_callback)
{
	// Overflow condition
	if( pringbuffer->Size == pringbuffer->Capacity )
	{
		_RB_TAIL_DESTROY(pringbuffer,element_destructor_callback)
		_RB_TAIL_ONE_STEP_FORWARD(pringbuffer)
		_RB_HEAD_WRITE(pringbuffer,pelement)
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
	}
	else
	{
		_RB_HEAD_WRITE(pringbuffer,pelement)
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
		_RB_SIZE_UP(pringbuffer)
	}
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + RINGBUFFER ALLOC BACK
// +
// + Allocate a new element at Ring Buffer Head without any copy.
// + Return a valid memory address and let user setup element with it.
// +

void* NRingBufferAllocBack(NRINGBUFFER *pringbuffer)
{
// 	printf("\nDEBUG------------------> -o- NRingBufferAllocBack call-begin .	Head = %d Tail = %d Size = %d ", pringbuffer->Head,pringbuffer->Tail,pringbuffer->Size );
	// This function doesn't Handle Overflow. User has to manage by himself...
	// If user doesn't want to manage overflow he should use... 'NRingBufferAllocBack_DropLatest' or 'NRingBufferAllocBack_OverwriteOldest'
	NErrorIf(pringbuffer->Size == pringbuffer->Capacity, NERROR_RINGBUFFER_OVERFLOW); 
	void *palloc = pringbuffer->pHead;
	_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
	_RB_SIZE_UP(pringbuffer)
//	printf("\nDEBUG------------------> -o- NRingBufferAllocBack call-end.	Head = %d Tail = %d Size = %d ", pringbuffer->Head,pringbuffer->Tail,pringbuffer->Size );
	return palloc;
}

void* NRingBufferAllocBack_DropLatest(NRINGBUFFER *pringbuffer)
{
	// Overflow condition
	if( pringbuffer->Size < pringbuffer->Capacity )
	{
		void *palloc = pringbuffer->pHead;
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
		_RB_SIZE_UP(pringbuffer)
		return palloc;
	}
	else
	{
		return NULL;
	}
}

void* NRingBufferAllocBack_OverwriteOldest(NRINGBUFFER *pringbuffer)
{
	void *palloc = pringbuffer->pHead;
	// Overflow condition
	if( pringbuffer->Size == pringbuffer->Capacity )
	{
		_RB_TAIL_ONE_STEP_FORWARD(pringbuffer)
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
	}
	else
	{
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
		_RB_SIZE_UP(pringbuffer)
	}
	return palloc;
}

void* NRingBufferAllocBack_OverwriteOldestXtd(NRINGBUFFER *pringbuffer, NRINGBUFFER_ELEMENT_DESTRUCTOR_CALLBACK element_destructor_callback)
{
	void *palloc = pringbuffer->pHead;

	// Overflow condition
	if( pringbuffer->Size == pringbuffer->Capacity )
	{
		// -- Extended part -------------------------------------------- 
		_RB_TAIL_DESTROY(pringbuffer,element_destructor_callback)
		// -- Extended part -------------------------------------------- 
		_RB_TAIL_ONE_STEP_FORWARD(pringbuffer)
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
	}
	else
	{
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
		_RB_SIZE_UP(pringbuffer)
	}

	return palloc;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + RINGBUFFER ALLOC BACK INDEEXED
// +
// + Allocate a new element at Ring Buffer Head without any copy. 
// + Return a valid Buffer Index relative to the Absolute beginning of the Buffer.
// +

Nu32 NRingBufferAllocBackI(NRINGBUFFER *pringbuffer)
{
	// This function doesn't Handle Overflow. User has to manage by himself...
	// If user doesn't want to manage overflow he should use... 'NRingBufferAllocBack_DropLatest' or 'NRingBufferAllocBack_OverwriteOldest'
	NErrorIf(pringbuffer->Size == pringbuffer->Capacity, NERROR_RINGBUFFER_OVERFLOW); 
	Nu32 alloc = pringbuffer->Head;
	_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
	_RB_SIZE_UP(pringbuffer)
	return alloc;
}

Nu32 NRingBufferAllocBackI_DropLatest(NRINGBUFFER *pringbuffer)
{
	// Overflow condition
	if( pringbuffer->Size < pringbuffer->Capacity )
	{
		Nu32 alloc = pringbuffer->Head;
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
		_RB_SIZE_UP(pringbuffer)
		return alloc;
	}
	else
	{
		return NVOID; 
	}
}

Nu32 NRingBufferAllocBackI_OverwriteOldest(NRINGBUFFER *pringbuffer)
{
	Nu32 alloc = pringbuffer->Head;
	// Overflow condition
	if( pringbuffer->Size == pringbuffer->Capacity )
	{
		_RB_TAIL_ONE_STEP_FORWARD(pringbuffer)
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
	}
	else
	{
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
		_RB_SIZE_UP(pringbuffer)
	}
	return alloc;
}

Nu32 NRingBufferAllocBackI_OverwriteOldestXtd(NRINGBUFFER *pringbuffer, NRINGBUFFER_ELEMENT_DESTRUCTOR_CALLBACK element_destructor_callback)
{
	Nu32 alloc = pringbuffer->Head;

	// Overflow condition
	if( pringbuffer->Size == pringbuffer->Capacity )
	{
		// -- Extended part -------------------------------------------- 
		_RB_TAIL_DESTROY(pringbuffer,element_destructor_callback)
		// -- Extended part -------------------------------------------- 
		_RB_TAIL_ONE_STEP_FORWARD(pringbuffer)
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
	}
	else
	{
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
		_RB_SIZE_UP(pringbuffer)
	}

	return alloc;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + RINGBUFFER PUSH BACK EXTENDED
// +
// + Push a new element at Ring Buffer Head (make a copy of new incoming element) ... 
// + ... AND ...
// + Return a pointer on memory address where element is stored into the RingBuffer.
// +
void* NRingBufferPushBackXtd(NRINGBUFFER *pringbuffer,void *pelement)
{
	// This function doesn't Handle Overflow. User has to manage by himself...
	// If user doesn't want to manage overflow he should use... 'NRingBufferPushBackXtd_DropLatest' or 'NRingBufferPushBackXtd_OverwriteOldest'
	NErrorIf(pringbuffer->Size == pringbuffer->Capacity, NERROR_RINGBUFFER_OVERFLOW); 
	_RB_HEAD_WRITE(pringbuffer,pelement)
	// -- Extended part -------------------------------------------- 
	void *preturn = pringbuffer->pHead;
	// -- Extended part -------------------------------------------- 
	_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
	_RB_SIZE_UP(pringbuffer)
	// -- Extended part -------------------------------------------- 
	return preturn;
	// -- Extended part -------------------------------------------- 
}

void* NRingBufferPushBackXtd_DropLatest(NRINGBUFFER *pringbuffer,void *pelement)
{
	// Overflow condition
	if( pringbuffer->Size < pringbuffer->Capacity )
	{
		// -- Extended part -------------------------------------------- 
		void *preturn = pringbuffer->pHead;
		// -- Extended part -------------------------------------------- 
		_RB_HEAD_WRITE(pringbuffer,pelement)
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
		_RB_SIZE_UP(pringbuffer)
		
		// -- Extended part -------------------------------------------- 
		return preturn;
		// -- Extended part -------------------------------------------- 
	}
	// -- Extended part -------------------------------------------- 
	else
	{
		return NULL;
	}
	// -- Extended part -------------------------------------------- 
}

void* NRingBufferPushBackXtd_OverwriteOldest(NRINGBUFFER *pringbuffer,void *pelement)
{
	// -- Extended part -------------------------------------------- 
	void *preturn = pringbuffer->pHead;
	// -- Extended part -------------------------------------------- 

	// Overflow condition
	if( pringbuffer->Size == pringbuffer->Capacity )
	{
		_RB_TAIL_ONE_STEP_FORWARD(pringbuffer)
		_RB_HEAD_WRITE(pringbuffer,pelement)
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)

	}
	else
	{
		_RB_HEAD_WRITE(pringbuffer,pelement)
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
		_RB_SIZE_UP(pringbuffer)
	}

	// -- Extended part -------------------------------------------- 
	return preturn;
	// -- Extended part -------------------------------------------- 
}

void* NRingBufferPushBackXtd_OverwriteOldestXtd(NRINGBUFFER *pringbuffer,void *pelement, NRINGBUFFER_ELEMENT_DESTRUCTOR_CALLBACK element_destructor_callback)
{
	// -- Extended part -------------------------------------------- 
	void *preturn = pringbuffer->pHead;
	// -- Extended part -------------------------------------------- 
	
	// Overflow condition
	if( pringbuffer->Size == pringbuffer->Capacity )
	{
		_RB_TAIL_DESTROY(pringbuffer,element_destructor_callback)
		_RB_TAIL_ONE_STEP_FORWARD(pringbuffer)
		_RB_HEAD_WRITE(pringbuffer,pelement)
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
	}
	else
	{
		_RB_HEAD_WRITE(pringbuffer,pelement)
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
		_RB_SIZE_UP(pringbuffer)
	}
	// -- Extended part -------------------------------------------- 
	return preturn;
	// -- Extended part -------------------------------------------- 
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + RINGBUFFER PUSH BACK EXTENDED INDEXED
// +
// + Push a new element at Ring Buffer Head (make a copy of new incoming element) ... 
// + ... AND ...
// + Return its Absolute Buffer Index ( relative to the Absolute beginning of the Buffer).
// +
Nu32 NRingBufferPushBackXtdI(NRINGBUFFER *pringbuffer,void *pelement)
{
	// This function doesn't Handle Overflow. User has to manage by himself...
	// If user doesn't want to manage overflow he should use... 'NRingBufferPushBackXtd_DropLatest' or 'NRingBufferPushBackXtd_OverwriteOldest'
	NErrorIf(pringbuffer->Size == pringbuffer->Capacity, NERROR_RINGBUFFER_OVERFLOW); 
	_RB_HEAD_WRITE(pringbuffer,pelement)
	// -- Extended part -------------------------------------------- 
	Nu32 ret = pringbuffer->Head;
	// -- Extended part -------------------------------------------- 
	_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
	_RB_SIZE_UP(pringbuffer)
	// -- Extended part -------------------------------------------- 
	return ret;
	// -- Extended part -------------------------------------------- 
}

Nu32 NRingBufferPushBackXtdI_DropLatest(NRINGBUFFER *pringbuffer,void *pelement)
{
	// Overflow condition
	if( pringbuffer->Size < pringbuffer->Capacity )
	{
		// -- Extended part -------------------------------------------- 
		Nu32 ret = pringbuffer->Head;
		// -- Extended part -------------------------------------------- 
		_RB_HEAD_WRITE(pringbuffer,pelement)
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
		_RB_SIZE_UP(pringbuffer)
		// -- Extended part -------------------------------------------- 
		return ret;
		// -- Extended part -------------------------------------------- 
	}
	// -- Extended part -------------------------------------------- 
	else
	{
		return NVOID;
	}
	// -- Extended part -------------------------------------------- 
}

Nu32 NRingBufferPushBackXtdI_OverwriteOldest(NRINGBUFFER *pringbuffer,void *pelement)
{
	// -- Extended part -------------------------------------------- 
	Nu32 ret = pringbuffer->Head;
	// -- Extended part -------------------------------------------- 

	// Overflow condition
	if( pringbuffer->Size == pringbuffer->Capacity )
	{
		_RB_TAIL_ONE_STEP_FORWARD(pringbuffer)
		_RB_HEAD_WRITE(pringbuffer,pelement)
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
	}
	else
	{
		_RB_HEAD_WRITE(pringbuffer,pelement)
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
		_RB_SIZE_UP(pringbuffer)
	}

	// -- Extended part -------------------------------------------- 
	return ret;
	// -- Extended part -------------------------------------------- 
}

Nu32 NRingBufferPushBackXtdI_OverwriteOldestXtd(NRINGBUFFER *pringbuffer,void *pelement, NRINGBUFFER_ELEMENT_DESTRUCTOR_CALLBACK element_destructor_callback)
{
	// -- Extended part -------------------------------------------- 
	Nu32 ret = pringbuffer->Head;
	// -- Extended part -------------------------------------------- 

	// Overflow condition
	if( pringbuffer->Size == pringbuffer->Capacity )
	{
		_RB_TAIL_DESTROY(pringbuffer,element_destructor_callback)
		_RB_TAIL_ONE_STEP_FORWARD(pringbuffer)
		_RB_HEAD_WRITE(pringbuffer,pelement)
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
	}
	else
	{
		_RB_HEAD_WRITE(pringbuffer,pelement)
		_RB_HEAD_ONE_STEP_FORWARD(pringbuffer)
		_RB_SIZE_UP(pringbuffer)
	}
	// -- Extended part -------------------------------------------- 
	return ret;
	// -- Extended part -------------------------------------------- 
}


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + RINGBUFFER POP and FREE
// +
// + To get back an element from the Buffer ( get a copy and free the buffer place )
// + OR
// + Free the buffer place ( == make it available for a new pushback )
// +
void NRingBufferPopFront(NRINGBUFFER *pringbuffer,void *pelement)
{
	NErrorIf(pringbuffer->Size == 0, NERROR_RINGBUFFER_IS_EMPTY);
	_RB_TAIL_READ(pringbuffer,pelement)
	_RB_TAIL_ONE_STEP_FORWARD(pringbuffer)
	_RB_SIZE_DOWN(pringbuffer)
}

void NRingBufferFreeFront(NRINGBUFFER *pringbuffer)
{
//	printf("\nDEBUG-->NRingBufferFreeFront call-begin .	Head = %d Tail = %d Size = %d ", pringbuffer->Head,pringbuffer->Tail,pringbuffer->Size );

	NErrorIf(pringbuffer->Size == 0, NERROR_RINGBUFFER_IS_EMPTY);
	_RB_TAIL_ONE_STEP_FORWARD(pringbuffer)
	_RB_SIZE_DOWN(pringbuffer)

//	printf("\nDEBUG-->NRingBufferFreeFront call-end .	Head = %d Tail = %d Size = %d ", pringbuffer->Head,pringbuffer->Tail,pringbuffer->Size );
}

void NRingBufferFreeFrontXtd(NRINGBUFFER *pringbuffer, NRINGBUFFER_ELEMENT_DESTRUCTOR_CALLBACK element_destructor_callback)
{
	NErrorIf(pringbuffer->Size == 0, NERROR_RINGBUFFER_IS_EMPTY);
	_RB_TAIL_DESTROY(pringbuffer,element_destructor_callback)
	_RB_TAIL_ONE_STEP_FORWARD(pringbuffer)
	_RB_SIZE_DOWN(pringbuffer)
}

void NRingBufferFreeRangeFront(NRINGBUFFER *pringbuffer, const Nu32 range_size)
{
	NErrorIf(pringbuffer->Size == 0, NERROR_RINGBUFFER_IS_EMPTY);
	NErrorIf( !range_size, NERROR_NULL_VALUE);
	NErrorIf(pringbuffer->Size < range_size, NERROR_RINGBUFFER_SIZE_OUTOFRANGE);
	
	pringbuffer->Tail = (pringbuffer->Tail + range_size)%pringbuffer->Capacity;
	pringbuffer->pTail= (NBYTE*)pringbuffer->pBuffer + pringbuffer->Tail*pringbuffer->ElementSize;
	pringbuffer->Size -= range_size;
}


void NRingBufferFreeRangeFrontXtd(NRINGBUFFER *pringbuffer, const Nu32 range_size, NRINGBUFFER_ELEMENT_DESTRUCTOR_CALLBACK element_destructor_callback)
{
	NErrorIf(pringbuffer->Size == 0, NERROR_RINGBUFFER_IS_EMPTY);
	NErrorIf( !range_size, NERROR_NULL_VALUE);
	NErrorIf(pringbuffer->Size < range_size, NERROR_RINGBUFFER_SIZE_OUTOFRANGE);

	for(Nu32 i=range_size;i!=0;i--)
	{
		_RB_TAIL_DESTROY(pringbuffer,element_destructor_callback)
		_RB_TAIL_ONE_STEP_FORWARD(pringbuffer)
		_RB_SIZE_DOWN(pringbuffer)
	}
}

/*
void cb_init(circular_buffer *cb, size_t capacity, size_t sz)
{
	cb->buffer = malloc(capacity * sz);
	if(cb->buffer == NULL)
		// handle error
		cb->buffer_end = (char *)cb->buffer + capacity * sz;
	cb->capacity = capacity;
	cb->count = 0;
	cb->sz = sz;
	cb->head = cb->buffer;
	cb->tail = cb->buffer;
}

void cb_free(circular_buffer *cb)
{
	free(cb->buffer);
	// clear out other fields too, just to be safe
}

void cb_push_back(circular_buffer *cb, const void *item)
{
	if(cb->count == cb->capacity){
		// handle error
	}
	memcpy(cb->head, item, cb->sz);
	cb->head = (char*)cb->head + cb->sz;
	if(cb->head == cb->buffer_end)
		cb->head = cb->buffer;
	cb->count++;
}

void cb_pop_front(circular_buffer *cb, void *item)
{
	if(cb->count == 0){
		// handle error
	}
	memcpy(item, cb->tail, cb->sz);
	cb->tail = (char*)cb->tail + cb->sz;
	if(cb->tail == cb->buffer_end)
		cb->tail = cb->buffer;
	cb->count--;
}*/
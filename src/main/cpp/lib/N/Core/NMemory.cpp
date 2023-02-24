// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NMemory.cpp										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************

// .......................................................................................
// All these functions are only working into DEBUG mode.
// in Release mode, Nmalloc and Nfree are inline functions and simply call malloc and free.
#if defined _DEBUG && defined _NWINDOWS

#include "lib/N/NCStandard.h"
#include "../NMemory.h"
#include "lib/N/NErrorHandling.h"

static Nu32 NMalloc_AllocatedMemory_Size;
static Nu32 NMalloc_Calls_Nb;
static Nu32 NRealloc_Calls_Nb;
static Nu32 NFree_Calls_Nb;

void *Nmalloc_debug(Nu32 size, const char *pfname, int nbline)
{
	void *ret_alloc;

	ret_alloc = _malloc_dbg(size, _NORMAL_BLOCK, pfname, nbline);
	if (!ret_alloc)
	{
		NErrorIf(1, NERROR_NULL_MALLOC);
	}
	else
	{
		NMalloc_AllocatedMemory_Size += _msize_dbg(ret_alloc, _NORMAL_BLOCK);
		NMalloc_Calls_Nb++;
	}
	return ret_alloc;
}

void *Nrealloc_debug(void *ptr, Nu32 newsize, const char *pfname, int nbline)
{
	void *ret_alloc = ptr;

	if (!newsize)
	{
		Nfree_debug(ptr);
		return NULL;
	}
	else if (!ptr)
	{
		return Nmalloc_debug(newsize, pfname, nbline);
	}
	else
	{
		NMalloc_AllocatedMemory_Size -= _msize_dbg(ptr, _NORMAL_BLOCK);
		ret_alloc = _realloc_dbg(ptr, newsize, _NORMAL_BLOCK, pfname, nbline);
		if (!ret_alloc)
		{
			NErrorIf(1, NERROR_NULL_REALLOC);
		}
		else
		{
			NMalloc_AllocatedMemory_Size += _msize_dbg(ret_alloc, _NORMAL_BLOCK);
			NRealloc_Calls_Nb++;
		}

		return ret_alloc;
	}
}

void Nfree_debug(void *ptr)
{
	NFree_Calls_Nb++;
	NMalloc_AllocatedMemory_Size -= _msize_dbg(ptr, _NORMAL_BLOCK);
	_free_dbg(ptr, _NORMAL_BLOCK);
}

void NMemoryStats_Debug()
{
	printf("\n--------------------MEMORY STATS---------------------------------");
	printf("\n NmallocAllocatedMemory = %d bytes = %2f Ko = %2f Mo", NMalloc_AllocatedMemory_Size, (Nf32)NMalloc_AllocatedMemory_Size / 1024.0f, (Nf32)NMalloc_AllocatedMemory_Size / (1024.0f * 1024.0f));
	printf("\n NmallocCalls = %d calls", NMalloc_Calls_Nb);
	printf("\n NReallocCalls = %d calls", NRealloc_Calls_Nb);
	printf("\n NfreeCalls = %d calls", NFree_Calls_Nb);
	printf("\n-----------------------------------------------------------------");
}

void NMemoryCheck_Debug(const void *ptr, const Nu32 memory_size, const Nu8 check_u8)
{
	Nu32 i;
	Nu8 *ptr_u8;

	ptr_u8 = (Nu8 *)ptr;
	for (i = 0; i < memory_size; i++, ptr_u8++)
	{
		if (*ptr_u8 != check_u8)
			NErrorIf(1, NERROR_MEMORY_CHECK_FAILED);
	}
}

// .......................................................................................
#endif

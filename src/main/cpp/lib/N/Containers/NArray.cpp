#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/Maths/NMath.h"
#include "lib/N/NFlags.h"
#include "lib/N/NErrorHandling.h"

#include "lib/N/Containers/NArray.h"
// ------------------------------------------------------------------------------------------
// NClearArray
// ------------------------------------------------------------------------------------------
// Description :
//	Clear content
//  All the elements of the array are dropped.
//  for each of them the "destructor_callback" function is called (if any) and then,
//  they are removed from the Array container, leaving the container with a size of 0.
//	All the others parameters are reset to 0, ElementSize included !
// ------------------------------------------------------------------------------------------
// In  :
//			parray					a Ptr on the array to clear.
//
//			destructor_callback		a function called for each element before removing them
//									it can be NULL (in that case there is no function call) 
// Out :
//
// ------------------------------------------------------------------------------------------
void NClearArray(NARRAY* parray,const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback)
{
	if(destructor_callback)
	{
		NBYTE	*ptr = parray->pFirst;
		for(Nu32 i=parray->Size;i!=0;i--,ptr+=parray->ElementSize)
		{
			destructor_callback(ptr);
		}
	}
	// Free memory
	Nfree(parray->pFirst);
	//... and set all parameters to 0 ... 
	parray->pFirst		= NULL;
	parray->Capacity	= 0;
	parray->Size		= 0;
	parray->ElementSize = 0; // Added 2013-01-24 ... oups!
}
// ------------------------------------------------------------------------------------------
// NCreateArray
// ------------------------------------------------------------------------------------------
// Description :
//	Create an Array and initialize its container and structure members.
//  Memory space for one NARRAY is reserved by a malloc and memory is also allocated for the
//	container by a simple call of "NSetupArray" Function with the parameters of "capacity"
//	and "element_size"
//  
// ------------------------------------------------------------------------------------------
// In  :
//			capacity				Max Number of elements that the array container can store.
//
//			element_size			size (in NBYTE) of one element of the array (usually returned
//									by "sizeof(element)" )
//
// Out :
//			a valid NARRAY* pointer on the created and initalized NARRAY structure.
//			NULL pointer in case of trouble.
// ------------------------------------------------------------------------------------------
NARRAY* NCreateArray(const Nu32 capacity,const Nu32 element_size)
{
	return NSetupArray(NEW(NARRAY),capacity,element_size);
}

// ------------------------------------------------------------------------------------------
// NDeleteArray
// ------------------------------------------------------------------------------------------
// Description :
//	Delete an Array created with "NCreateArray"
//	Memory allocated by the container is freed by a call of "NClearArray" and the memory
//	allocated for the NARRAY structure is freed too.
//  If any, before clearing the container, each element is destroyed by a call of 
//	the callback function "destructor_callback"
// ------------------------------------------------------------------------------------------
// In  :
//			parray				a VALID POINTER of a NARRAY structure created with "NArrayCreate"
//
//			destructor_callback	NARRAY_CALLBACK function called for each element before clearing
//								the container. 
//								It can be NULL; In that case, no callback function is called.
//
// Out :
// ------------------------------------------------------------------------------------------
void NDeleteArray(NARRAY* parray,const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback)
{
	NClearArray(parray, destructor_callback);
	Nfree(parray);
}
// ------------------------------------------------------------------------------------------
// NSetupArray
// ------------------------------------------------------------------------------------------
// Description :
//	Initialize an Array
//  Reserve enough Memory for the container that it can store a max number of "capacity" element
//	with a memory size of "element_size" for each of them.
//  
// -------------------------------------------------------------------------------------------
// In  :
//			parray					a VALID POINTER of a NARRAY structure.
//							which IS NOT created
//									by a call of "NArrayCreated"
//
//			capacity				Max Number of elements that the array container can store.
//
//			element_size			size (in NBYTE) of one element of the array (usually returned
//									by "sizeof(element)" )
//
// Out :
//			a valid NARRAY* pointer on the created and initalized NARRAY structure.
//			NULL pointer in case of trouble.
// ------------------------------------------------------------------------------------------
NARRAY* NSetupArray(NARRAY *parray, const Nu32 capacity,const Nu32 element_size )
{
	NErrorIf(!element_size, NERROR_ARRAY_WRONG_ELEMENT_SIZE);
	if(capacity)
	{
		parray->pFirst = (NBYTE*)Nmalloc(capacity*element_size);
		memset(parray->pFirst,0,capacity*element_size);
	}
	else
		parray->pFirst = NULL; // We don't allocate any memory if capacity == 0 !
	
	parray->ElementSize			= element_size;
	parray->Capacity			= capacity;
	parray->Size				= 0;
	return parray;
}
/*
//
// Function is well written but seems not really the best thing to do ...
//
//
// ------------------------------------------------------------------------------------------
// NSetupArrayXprt
// ------------------------------------------------------------------------------------------
// Description :
//	Initialize an Array in an Expert way !
//  
//	
//  
// -------------------------------------------------------------------------------------------
// In  :
//			parray				a VALID POINTER of a NARRAY structure.
//								which IS NOT created
//								by a call of "NArrayCreated"
//
//			capacity			Max Number of elements that the array container can store.
//
//			element_size		size (in NBYTE) of one element of the array (usually returned
//								by "sizeof(element)" )
//
//			size				current size of the array
//	
//			pbuffer				a pointer to a valid buffer allocated in the right way with the exact space of capacity*element_size
//
// Out :
//			parray itself
//			
// ------------------------------------------------------------------------------------------
NARRAY* NSetupArrayXprt(NARRAY* parray, const Nu32 size, const Nu32 capacity, const Nu32 element_size, NBYTE* pbuffer)
{
	// An eXPeRT setup may be performed ONLY on a full of ZEROS NARRAY struct !
	NErrorIf(parray->pFirst, NERROR_NON_NULL_POINTER);
	NErrorIf(parray->ElementSize, NERROR_NON_NULL_VALUE);
	NErrorIf(parray->Capacity, NERROR_NON_NULL_VALUE);
	NErrorIf(parray->Size, NERROR_NON_NULL_VALUE);

	NErrorIf(!pbuffer, NERROR_NULL_POINTER);
	NErrorIf(!element_size, NERROR_ARRAY_WRONG_ELEMENT_SIZE);
	NErrorIf(!capacity, NERROR_ARRAY_WRONG_CAPACITY);
	NErrorIf(size > capacity, NERROR_INCONSISTENT_VALUES);

	parray->pFirst		= pbuffer;
	parray->ElementSize = element_size;
	parray->Capacity	= capacity;
	parray->Size		= size;
	return parray;
}
*/

// ------------------------------------------------------------------------------------------
// NEnumArray
// ------------------------------------------------------------------------------------------
// Description :
//	Enumerates the array elements by passing the pointer to each of them, in turn, to an application-defined callback function. 
//	NEnumArray continues until the last element is enumerated or the callback function returns NFALSE.
// -------------------------------------------------------------------------------------------
// In  :
//			parray				a VALID POINTER on an NARRAY structure.
//
//			enum_callback		NARRAY_ENUM_CALLBACK function called for each element
//								If the callback returns FALSE, function Enum stops
//
// Out :	
//			0					all Array elements were enumerated 
//			non zero			enum interrupted by an element. 
//								index of this element is ( parray->Size - return value )
// ------------------------------------------------------------------------------------------
Nu32 NEnumArray(NARRAY* parray, const NARRAY_ENUM_CALLBACK enum_callback)
{
	NErrorIf(!enum_callback, NERROR_ARRAY_NULL_ADDRESS);
	NErrorIf(!parray->Size, NERROR_ARRAY_IS_EMPTY);

	Nu32 i;
	NBYTE* ptr = parray->pFirst;// NGetFirstArrayPtr(parray);
	for (i = parray->Size; i != 0; i--, ptr += parray->ElementSize)
	{
		if (!enum_callback(ptr))
			break;
	}
	return i;
}



// ------------------------------------------------------------------------------------------
// NEraseArrayElement
// ------------------------------------------------------------------------------------------
// Description :
//	Erase a specific element of an array
//  if any the destructor_callback function is called before destroying the element.
//	At the end of the function process, element is destroyed, all the elements at the right
//	of the destroyed element are shifted to the left, and size of the container is decrease
//	by one.
//  
// -------------------------------------------------------------------------------------------
// In  :
//			parray					a VALID POINTER on an NARRAY structure.
//
//			index					Index of the element to erase (first = 0, last = size-1).
//
//			destructor_callback		NARRAY_CALLBACK function called before destroying the element
//									It can be NULL; In that case, no callback function is called.
//
// Out :
//			-1 if there is a problem, like an invalid index.
//			1 success
// ------------------------------------------------------------------------------------------
Nbool	NEraseArrayElement(NARRAY *parray,const Nu32 index, const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback)
{
	NErrorIf(index >= parray->Size,NERROR_ARRAY_INDEX_BEYOND_LIMITS);
	
	NBYTE *ptr = parray->pFirst + index*parray->ElementSize;
	// call the callback destruction function (if any)
	if(destructor_callback)
		destructor_callback(ptr);
	// Shift the part of the array which is at the right of the erased element
	if( index != (parray->Size-1) )// if not, the element is the last one of the array. there is no shifting to do.
	{
		memcpy(ptr,ptr + parray->ElementSize,(parray->Size - index - 1)*parray->ElementSize);
	}
	parray->Size-=1;
	return NTRUE;
}
void	NQuickEraseArrayElement(NARRAY *parray,const Nu32 index, const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback)
{
	NErrorIf(index >= parray->Size,NERROR_ARRAY_INDEX_BEYOND_LIMITS);
	NBYTE *ptr = parray->pFirst + index*parray->ElementSize;
	// call the callback destruction function (if any)
	if(destructor_callback)
		destructor_callback(ptr);
	// Replace the Erased element by the last one
	if( index != (parray->Size-1) )// if not, the element is the last one of the array. there is no replacing to do.
	{
		memcpy( ptr,parray->pFirst+(parray->Size-1)*parray->ElementSize,parray->ElementSize );
	}
	parray->Size-=1;
}
void	NEraseArrayElementPtr(NARRAY *parray,const NBYTE* ptr, const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback)
{
	NErrorIf(ptr >= parray->pFirst + parray->Size*parray->ElementSize ,NERROR_ARRAY_PTR_BEYOND_LIMITS);
	NErrorIf(ptr < parray->pFirst,NERROR_ARRAY_PTR_BEYOND_LIMITS);
#ifdef _DEBUG
	Nu32 debug_id = (ptr-parray->pFirst)/parray->ElementSize;
	NErrorIf(parray->pFirst + debug_id *parray->ElementSize != ptr, NERROR_ARRAY_CORRUPTED_PTR);
#endif
	// call the callback destruction function (if any)
	if(destructor_callback)
		destructor_callback((void*)ptr);
	// Shift the part of the array which is at the right of the erased element
	if( ptr != parray->pFirst + (parray->Size-1)*parray->ElementSize )// if not, the element is the last one of the array. there is no shifting to do.
	{
		memcpy((void*)ptr,ptr + parray->ElementSize, parray->pFirst + (parray->Size*parray->ElementSize) - ptr );
	}
	parray->Size-=1;
}
void	NQuickEraseArrayElementPtr(NARRAY *parray,const NBYTE* ptr, const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback)
{
	NErrorIf(ptr >= parray->pFirst + parray->Size*parray->ElementSize ,NERROR_ARRAY_PTR_BEYOND_LIMITS);
	NErrorIf(ptr < parray->pFirst,NERROR_ARRAY_PTR_BEYOND_LIMITS);
	
	// call the callback destruction function (if any)
	if(destructor_callback)
		destructor_callback((void*)ptr);
	// Shift the part of the array which is at the right of the erased element
	NBYTE	*ptr2 = parray->pFirst + (parray->Size-1)*parray->ElementSize;
	if( ptr != ptr2 )// if not, the element is the last one of the array. there is no shifting to do.
	{
		memcpy((void*)ptr,ptr2,parray->ElementSize );
	}
	parray->Size-=1;
}


// ------------------------------------------------------------------------------------------
// NEraseArrayRange
// ------------------------------------------------------------------------------------------
// Description :
//	Erase a range of elements of an array
//  if any, the destructor_callback function is called before destroying each element.
//	At the end of the function process, all the elements in the range are destroyed,
//	all the elements at the right of the last destroyed element are shifted to the left, and size
//	of the container is decrease by the destroyed range size.
//  
// -------------------------------------------------------------------------------------------
// In  :
//			parray					a VALID POINTER on an NARRAY structure.
//
//			index					Index of the first element to erase.
//
//			rangesize				number of elements to destroyed (the first one included)
//
//			destructor_callback		NARRAY_CALLBACK function called for each element before clearing
//									the container. 
//									It can be NULL; In that case, no callback function is called.
//
// Out :
//			-1 if there is a problem, like an invalid index or an invalid range.
//			0 nothing to do, because rangesize == 0
//			1 success
// ------------------------------------------------------------------------------------------
Nbool NEraseArrayRange(NARRAY *parray,const Nu32 index,const Nu32 rangesize,const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback)
{
	NErrorIf((index+rangesize) > parray->Size,NERROR_ARRAY_INDEX_BEYOND_LIMITS);
	
	// Nothing to do
	if(rangesize == 0)
		return NTRUE;

	NBYTE	*ptr = parray->pFirst + index*parray->ElementSize;
	Nu32	right_index = index + rangesize;

	// call the callback destruction function (if any)
	if(destructor_callback)
	{
		NBYTE *ptr2 = ptr;
		for(Nu32 i=rangesize;i!=0;i--,ptr2 += parray->ElementSize)
		{
			destructor_callback(ptr2);
		}
	}
	// Shift the part of the array which is at the right of the last erased element
	if(right_index < parray->Size)// if not, the element is the last one of the array. there is no shifting to do.
	{
		memcpy(ptr,parray->pFirst + right_index*parray->ElementSize,(parray->Size - right_index)*parray->ElementSize);
	}
	parray->Size -= rangesize;
	return NTRUE;
}
/*
void NEraseArrayRangePtr(NARRAY *parray,const NBYTE *ptr ,const Nu32 rangesize,const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback)
{
	NErrorIf(ptr >= parray->pFirst + parray->Size*parray->ElementSize ,NERROR_ARRAY_INDEX_BEYOND_LIMITS);
	NErrorIf(ptr < parray->pFirst,NERROR_ARRAY_INDEX_BEYOND_LIMITS);
	// Some Checks ...
	// Nothing to do
	if(rangesize == 0)
		return;

	// call the callback destruction function (if any)
	if(destructor_callback)
	{
		NBYTE *ptr2 = (NBYTE*)ptr;
		for(Nu32 i=rangesize;i!=0;i--,ptr2 += parray->ElementSize)
		{
			destructor_callback(ptr2);
		}
	}
	// Shift the part of the array which is at the right of the last erased element
	NBYTE *ptr_right = (NBYTE*)ptr + rangesize*parray->ElementSize;
	if(ptr_right < parray->pFirst + parray->Size*parray->ElementSize)// if not, the element is the last one of the array. there is no shifting to do.
	{
		memcpy((NBYTE*)ptr,ptr_right,((NBYTE*)(parray->Size*parray->ElementSize)) - ptr_right); !!!!! ERROR
		//memcpy((NBYTE*)ptr,ptr_right,((NBYTE*)(parray->Size*parray->ElementSize)) - ptr_right);
	}
	parray->Size -= rangesize;
}
*/
void NEraseArrayElementsBatch(NARRAY *parray,const Nu32 *pNu32_bitfield,const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback)
{
	NBYTE	*pel		= parray->pFirst;
	Nu32	array_size	= parray->Size;
	Nu32	n			= 0;	
	Nu32	size		= 0;
	NBYTE	*pdst,*psrc;

	// Look for the first "to delete" element. 
	while(!Nu32FIELD_BITGET(pNu32_bitfield,n) || n<array_size ){pel+=parray->ElementSize;n++;};
	pdst = pel; 

	while(n<array_size)
	{
		// Look for the first "to keep" element  
		while(Nu32FIELD_BITGET(pNu32_bitfield,n) || n<array_size ){pel+=parray->ElementSize;n++;};
		psrc = pel;size = n;
		// Look for the Next "to delete" element and Size to copy 
		while(!Nu32FIELD_BITGET(pNu32_bitfield,n) || n<array_size ){pel+=parray->ElementSize;n++;};
		size = n-size;

		memcpy(pdst,psrc,size*parray->ElementSize);
		pdst += size*parray->ElementSize;
		// add size to pdst at the end ! Like this,  at the loop end
		// 'pdst' is going to be the next address after the last valid One !!!
		// It seems weird but it's not ! 
		// We are going to use it to calculate the final size of the Array... 
	}
	// Calculate new size...
	parray->Size = (Nu32)( (pdst - parray->pFirst)/parray->ElementSize );
}
// ------------------------------------------------------------------------------------------
// NEraseArray
// ------------------------------------------------------------------------------------------
// Description :
//	Erase All the elements of an array without changing CAPACITY
//  if any, the destructor_callback function is called before destroying each element.
//	At the end of the function process, all the elements in the range are destroyed,
//	and the size is 0; 
// -------------------------------------------------------------------------------------------
// In  :
//			parray					a VALID POINTER on an NARRAY structure.
//
//			destructor_callback		NARRAY_CALLBACK function called for each element before clearing
//									the container. 
//									It can be NULL; In that case, no callback function is called.
//
// Out :
// ------------------------------------------------------------------------------------------
void NEraseArray(NARRAY *parray,const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback)
{
	// Some elements have to be dropped
	if(destructor_callback)
	{
		NBYTE* ptr = parray->pFirst;// NGetFirstArrayPtr(parray);
		for(Nu32 i=parray->Size;i!=0;i--,ptr+=parray->ElementSize)
		{
			destructor_callback(ptr);
		}
	}
	parray->Size = 0;
}


// ------------------------------------------------------------------------------------------
// NInsertArrayElement
// ------------------------------------------------------------------------------------------
// Description :
//	Insert a new element in an array
//	The container is extended by inserting the new element before the element "refindex".
//  Size of the container is going to be increase by one, and if it's necessay capacity will be increse too
// -------------------------------------------------------------------------------------------
// In  :
//			parray					a VALID POINTER on an NARRAY structure.
//
//			refindex				A valid Index in the Container. The new element will be inserted before it.
//
//			pelement				A valid pointer of an element structure casted like a (NBYTE*).
//									the function will perform a copy of this element into the new one inserted in
//									the container.
//
// Out :
//			A pointer casted like a (NBYTE*) on the new element inserted in the container
// ------------------------------------------------------------------------------------------
NBYTE* NInsertArrayElement(NARRAY *parray, const Nu32 refindex,const NBYTE *pelement)
{
	NErrorIf(refindex >= parray->Size, NERROR_ARRAY_INDEX_BEYOND_LIMITS);
	NErrorIf(!pelement, NERROR_NULL_POINTER);

	NBYTE	*ptr,*ptr2;
	Nu32	newcapacity,delta;

	if(parray->Size == parray->Capacity)
	{
		newcapacity = (Nu32)( (Nf32)parray->Capacity*(1.0f + NARRAY_INCREASE_CAPACITY_RATIO) + 1.0f ); 
		delta	= newcapacity - parray->Capacity;
		parray->pFirst = (NBYTE*)Nrealloc(parray->pFirst,newcapacity*parray->ElementSize);

		ptr = parray->pFirst + parray->Capacity * parray->ElementSize;
		memset(ptr,0,delta*parray->ElementSize);
		parray->Capacity = newcapacity;
	}

	// Shifting to the right
	ptr = parray->pFirst + refindex*parray->ElementSize;
	ptr2 = ptr + parray->ElementSize;
	memcpy(ptr2,ptr,( parray->Size - refindex )*parray->ElementSize);

	// Insert and copy the new element
	memcpy(ptr,pelement,parray->ElementSize);
	parray->Size += 1;
	return ptr;
}

// ------------------------------------------------------------------------------------------
// NIncreaseArrayCapacity
// ------------------------------------------------------------------------------------------
// Description :
//	Increase the capacity of an array container.
//  It doesn't change the current size of the container and doesn't remove any element.
//	The new amount of memory reallocated is filled with 0.
// -------------------------------------------------------------------------------------------
// In  :
//			parray					a VALID POINTER on an NARRAY structure.
//
//			add_capacity			Additional capacity, added to the current capacity.
//
// Out :
//			0 nothing to do, because add_capacity = 0
//			1 success
// ------------------------------------------------------------------------------------------
Ns32	NIncreaseArrayCapacity(NARRAY *parray,const Nu32 add_capacity)
{
	Nu32	newcapacity;
	NBYTE	*ptr;

	if(add_capacity == 0)
		return 0;

	newcapacity = parray->Capacity+add_capacity; 
	parray->pFirst = (NBYTE*)Nrealloc(parray->pFirst,newcapacity*parray->ElementSize);

	ptr = parray->pFirst + parray->Capacity * parray->ElementSize;
	memset(ptr,0,add_capacity*parray->ElementSize);
	parray->Capacity = newcapacity;
	
	return 1;
}
// ------------------------------------------------------------------------------------------
// NDecreaseArrayCapacity
// ------------------------------------------------------------------------------------------
// Description :
//	Decrease the capacity of an array container.
//  WARNING !! It Can change the current size of the container and possibly remove some elements !
//	In that case the destructot_Callback function is called (if any) for each deleted element 
// -------------------------------------------------------------------------------------------
// In  :
//			parray					a VALID POINTER on an NARRAY structure.
//
//			sub_capacity			subtract capacity, subtracted to the current capacity.
//
//			destructor_callback		NARRAY_CALLBACK function called for each element before deleting it
//									only if it's necessary. (It happens only if the new capacity is less
//									than the current size of the container.)
//									It can be NULL; In that case, no callback function is called.
//
// Out :
//			 0 nothing to do, because add_capacity = 0
//			-1 error. Sub_capacity is bigger than capacity
//			 1 success
// ------------------------------------------------------------------------------------------
Ns32	NDecreaseArrayCapacity(NARRAY *parray,const Nu32 sub_capacity, const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback )
{
	Nu32	i;
	Ns32	newcapacity;
	NBYTE	*ptr;

	if(sub_capacity == 0)
		return 0;

	newcapacity = (Ns32)parray->Capacity - (Ns32)sub_capacity; 
	if(newcapacity < 0)
		return -1;

	// Maybe some Element have to be destroyed
	if((Nu32)newcapacity < parray->Size)
	{
		if(destructor_callback)
		{
			ptr = parray->pFirst + (parray->Size-1)*parray->ElementSize;
			for(i=0;i<(parray->Size-newcapacity);i++)
			{
				destructor_callback(ptr);
				ptr -= parray->ElementSize;
			}
		}
		parray->Size = newcapacity;
	}

	parray->pFirst = (NBYTE*)Nrealloc(parray->pFirst,newcapacity*parray->ElementSize); // if newcapacity = 0 then realloc works like 'free'
	parray->Capacity = newcapacity;
	return 1;
}

// ------------------------------------------------------------------------------------------
// NSetArrayCapacity
// ------------------------------------------------------------------------------------------
// Description :
//	Decrease or Increase the capacity of an array container to reach "new_capacity"
//  WARNING !! It Can change the current size of the container and possibly remove some elements !
// -------------------------------------------------------------------------------------------
// In  :
//			parray					a VALID POINTER on an NARRAY structure.
//
//			new_capacity			New capacity of the container.
//
//			destructor_callback		NARRAY_CALLBACK function called for each element before deleting it
//									only if it's necessary. (It happens only if the new capacity is less
//									than the current size of the container.)
//									It can be NULL; In that case, no callback function is called.
//
// Out :
//			 0 nothing to do, because add_capacity = 0
//			-1 error. Sub_capacity is bigger than capacity
//			 1 success
// ------------------------------------------------------------------------------------------
Ns32 NSetArrayCapacity(NARRAY *parray,const Nu32 new_capacity, const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback )
{
	Ns32 diff = (Ns32)new_capacity - (Ns32)parray->Capacity;
	if(diff >= 0)
		return NIncreaseArrayCapacity(parray,(Nu32)diff);
	else
		return NDecreaseArrayCapacity(parray,(Nu32)NABS(diff),destructor_callback);
}

// ------------------------------------------------------------------------------------------
// NOptimizeArrayCapacity
// ------------------------------------------------------------------------------------------
// Description :
//	Adjust the capacity of an array container to match the exact size of the container.
//  It doesn't change the current size of the container and doesn't remove any element !
// -------------------------------------------------------------------------------------------
// In  :
//			parray					a VALID POINTER on an NARRAY structure.
//
// Out :
//			 0 nothing to do, because capacity already equal to the size
//			 1 success
// ------------------------------------------------------------------------------------------
Ns32 NOptimizeArrayCapacity(NARRAY *parray) 
{
	// Notice :
	// There is no need to call "NARRAY_CALLBACK destructor_callback" in that case,
	//	because all the element which are going to be removed are just pre-allocated but not initialized !
	if(parray->Size!=parray->Capacity)
	{
		parray->pFirst = (NBYTE*)Nrealloc(parray->pFirst,parray->Size*parray->ElementSize);
		parray->Capacity = parray->Size;
		return 1;
	}
	else
		return 0;
}

// ------------------------------------------------------------------------------------------
// NResizeArray
// ------------------------------------------------------------------------------------------
// Description :
// Change size
// Resizes the Array Container to contain new_size elements.
// If new_size is smaller than the current size, the content is reduced to its first new_size elements,
// the rest being dropped, and the destructor_callback function (if any) is called for each dropped element.
// If new_size is greater than the current size, the content is expanded by inserting at the end as many copies
// of pelement as needed to reach a size of new_size elements. This may cause a reallocation.
// Notice that this function changes the actual content of the array container by inserting or erasing elements.
// It does not only change its storage capacity.
// To direct a change only in storage capacity, use 'NSetArrayCapacity' instead.
// -------------------------------------------------------------------------------------------
// In  :
//			parray					a VALID POINTER on an NARRAY structure.
//
//			new_size				size requested
//
//			pelement				a Valid Pointer on an element which will be copy as many times as needed
//									to reach a size of new_size elements. It's happening only if new_size is
//									greater than current size. 
//									There is no copies if pelement is a NULL pointer
//
//			destructor_callback		NARRAY_CALLBACK function called for each element before deleting it.
//									It happens only if the new_size is smaller	than the current size.
//									It can be NULL; In that case, no callback function is called.
// Out :
//			 0 nothing to do, because capacity already equal to the size
//			 1 success
// ------------------------------------------------------------------------------------------
Ns32 NResizeArray(NARRAY *parray, const Nu32 new_size, const NBYTE *pelement, const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback )
{
	NBYTE	*ptr;
	Nu32	i;

	// Nothing to do
	if(new_size == parray->Size)
	{
		return 0;
	}
	else  if (new_size < parray->Size )
	{
		// Some elements have to be dropped
		if(destructor_callback)
		{
			ptr = NGetArrayPtr(parray,new_size);
			for(i=(parray->Size - new_size);i!=0;i--,ptr+=parray->ElementSize)
			{
				destructor_callback(ptr);
			}
		}
		parray->Size = new_size;
		return 1;
	}
	else// Beyond this point we are sure that new_size > parray->Size
	{
		// Increasing Capacity, Only if it's necessary
		if(new_size > parray->Capacity)
		{
			NIncreaseArrayCapacity(parray,new_size-parray->Capacity);
		}

		// duplicate the content of pelement in each new element created (if pelement is not NULL Pointer )
		if(pelement)
		{
			ptr = parray->pFirst+parray->Size*parray->ElementSize; // We don't use, NGetArrayPtr(parray,parray->Size) because this function
																   // Can only work into the actual size of the table
			for(i=new_size-parray->Size;i!=0;i--,ptr += parray->ElementSize)
			{
				memcpy(ptr,pelement,parray->ElementSize);
			}
		}
		parray->Size = new_size;
		return 1;
	}
}

// ------------------------------------------------------------------------------------------
// NArrayPopBack
// ------------------------------------------------------------------------------------------
// Description :
//	returns the last element in the array container, effectively reducing the size by one.
//	This calls the "destructor_callback" function (if any). 
// -------------------------------------------------------------------------------------------
// In  :
//			parray					a VALID POINTER on an NARRAY structure.
//
//			pelement				A valid pointer of an element structure casted like a (NBYTE*).
//									the function will perform a copy of the last element to this incoming element
// Out :
// ------------------------------------------------------------------------------------------
void NArrayPopBack(NARRAY *parray, NBYTE*pelement)
{
	NErrorIf(!pelement, NERROR_ARRAY_NULL_ADDRESS);
	NErrorIf(!parray->Size, NERROR_ARRAY_IS_EMPTY);

	if(parray->Size)
	{
		NBYTE	*plast = parray->pFirst + (parray->Size-1)*parray->ElementSize;
		memcpy(pelement,plast,parray->ElementSize);
		parray->Size -= 1;
	}
}
// ------------------------------------------------------------------------------------------
// NArrayPushBack
// ------------------------------------------------------------------------------------------
// Description :
//	Adds a new element at the end of the array container, right after its current last element. 
//	The content of this new element is initialized to a copy of "pelement".
//	This effectively increases the array size by one.
// -------------------------------------------------------------------------------------------
// In  :
//			parray					a VALID POINTER on an NARRAY structure.
//
//			pelement				A valid pointer of an element structure casted like a (NBYTE*).
//									the function will perform a copy of this element into
//									the new one inserted in the container.
// Out :
//			A pointer casted like a (NBYTE*) on the new element added a the end of the container
// ------------------------------------------------------------------------------------------
NBYTE* NArrayPushBack(NARRAY*parray,const NBYTE*pelement)
{
	NErrorIf(!parray, NERROR_NULL_POINTER);
	NErrorIf(!pelement, NERROR_NULL_POINTER);	// Prefer using "NArrayAllocBack" to retrieve a new element from array without incoming pelement

	if(parray->Size == parray->Capacity)
	{
		Nu32	newcapacity,delta;

		newcapacity = (Nu32)( (Nf32)parray->Capacity*(1.0f + NARRAY_INCREASE_CAPACITY_RATIO) + 1.0f ); 
		delta	= newcapacity - parray->Capacity;
		parray->pFirst = (NBYTE*)Nrealloc(parray->pFirst,newcapacity*parray->ElementSize);
		memset(parray->pFirst + parray->Capacity * parray->ElementSize,0,delta*parray->ElementSize);
		parray->Capacity = newcapacity;
	}

	NBYTE *ptr = parray->pFirst + parray->Size*parray->ElementSize;
	memcpy(ptr,pelement,parray->ElementSize);
	parray->Size += 1;

	return ptr;
}

NBYTE* NArrayBufferPushBack(NARRAY* parray, void* pbuffer, Nu32 nb_buffer_element)
{
	NErrorIf(!parray, NERROR_NULL_POINTER);
	NErrorIf(!pbuffer, NERROR_NULL_POINTER);	
	NErrorIf(!nb_buffer_element, NERROR_NULL_VALUE);

	if ((parray->Size + nb_buffer_element) > parray->Capacity)
	{
		parray->Capacity = parray->Size + nb_buffer_element;
		parray->pFirst = (NBYTE*)Nrealloc(parray->pFirst, parray->Capacity * parray->ElementSize);
	}

	NBYTE* ptr = parray->pFirst + parray->Size * parray->ElementSize;
	memcpy(ptr, pbuffer, nb_buffer_element*parray->ElementSize);
	parray->Size += nb_buffer_element;

	return ptr;
}
// ------------------------------------------------------------------------------------------
// NArrayAllocBack
// ------------------------------------------------------------------------------------------
// Description :
//	Close to NArrayPushBack, excepted that the content of the new element still uninitialized.
//	Adds a new element at the end of the array container, right after its current last element. 
//	This effectively increases the array size by one.
// -------------------------------------------------------------------------------------------
// In  :
//			parray					a VALID POINTER on an NARRAY structure.
//
// Out :
//			A pointer casted like a (NBYTE*) on the new element added a the end of the container.
//			!!! THE NEW ELEMENT IS NOT INITIALIZED !!!!
// ------------------------------------------------------------------------------------------
NBYTE* NArrayAllocBack(NARRAY*parray)
{
	NErrorIf(!parray, NERROR_NULL_POINTER);

	if(parray->Size == parray->Capacity)
	{
		Nu32	newcapacity,delta;

		newcapacity = (Nu32)( (Nf32)parray->Capacity*(1.0f + NARRAY_INCREASE_CAPACITY_RATIO) + 1.0f ); 
		delta	= newcapacity - parray->Capacity;
		parray->pFirst = (NBYTE*)Nrealloc(parray->pFirst,newcapacity*parray->ElementSize);
		memset(parray->pFirst + parray->Capacity * parray->ElementSize,0,delta*parray->ElementSize);
		parray->Capacity = newcapacity;
	}

	NBYTE	*ptr = parray->pFirst + parray->Size*parray->ElementSize;
	parray->Size += 1;
	return ptr;
}

// ------------------------------------------------------------------------------------------
// NInsertArrayElement
// ------------------------------------------------------------------------------------------
// Description :
//	Insert a new element in an array
//	The container is extended by inserting the new element before the element "refindex".
//  Size of the container is going to be increase by one, and if it's necessay capacity will be increse too
// -------------------------------------------------------------------------------------------
// In  :
//			parray					a VALID POINTER on an NARRAY structure.
//
//			refindex				A valid Index in the Container. The new element will be allocated before it.
//
//
// Out :
//			A pointer casted like a (NBYTE*) on the new element inserted in the container
// ------------------------------------------------------------------------------------------
NBYTE* NArrayAllocInPlace(NARRAY* parray, const Nu32 refindex)
{
	NErrorIf(refindex >= parray->Size, NERROR_ARRAY_INDEX_BEYOND_LIMITS);

	NBYTE* ptr, * ptr2;
	Nu32	newcapacity, delta;

	if (parray->Size == parray->Capacity)
	{
		newcapacity = (Nu32)((Nf32)parray->Capacity * (1.0f + NARRAY_INCREASE_CAPACITY_RATIO) + 1.0f);
		delta = newcapacity - parray->Capacity;
		parray->pFirst = (NBYTE*)Nrealloc(parray->pFirst, newcapacity * parray->ElementSize);

		ptr = parray->pFirst + parray->Capacity * parray->ElementSize;
		memset(ptr, 0, delta * parray->ElementSize);
		parray->Capacity = newcapacity;
	}

	// Shifting to the right
	ptr = parray->pFirst + refindex * parray->ElementSize;
	ptr2 = ptr + parray->ElementSize;
	memcpy(ptr2, ptr, (parray->Size - refindex) * parray->ElementSize);
	parray->Size += 1;
	return ptr;
}




// ------------------------------------------------------------------------------------------
// NCopyArray
// ------------------------------------------------------------------------------------------
// Description :
//	Copy an entire ARRAY to an another one. 
//	After the copy both ARRAY are identical in term of content...
//	Except for the memory addresses : for sure, these 2 ARRAYS have their own memory allocations
//
//		!!!! WARNING !!!!
//		This function doesn't perform any previous destinations array elements erasing.
//		All the previous destination data are going to be lost.
//		a non proper use of this function may cause memory corruption !
// -------------------------------------------------------------------------------------------
// In  :
//			pdst					a VALID POINTER on an NARRAY structure.
//									as a destination ARRAY
//
//			psrc					a VALID POINTER on an NARRAY structure.
//									as a source ARRAY
//								
// Out :
// ------------------------------------------------------------------------------------------
void NCopyArray(NARRAY*pdst,const NARRAY*psrc)
{
	Nu32	memsize = psrc->Capacity*psrc->ElementSize;
	if( (pdst->Capacity*pdst->ElementSize) != memsize )
		pdst->pFirst = (NBYTE*)Nrealloc(pdst->pFirst,memsize );
	pdst->Capacity		= psrc->Capacity;
	pdst->ElementSize	= psrc->ElementSize;
	pdst->Size			= psrc->Size;
	memcpy(pdst->pFirst,psrc->pFirst,memsize);
}

void NCopyArrayEx(NARRAY* pdst, const NARRAY* psrc, const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback, const NARRAY_COPYEX_CALLBACK copy_callback)
{
	// Appel du destructeur pour chaque �l�ment de pdst
	if (destructor_callback)
	{
		NBYTE* ptr = pdst->pFirst;
		for (Nu32 i = pdst->Size; i != 0; i--, ptr += pdst->ElementSize)
		{
			destructor_callback(ptr);
		}
	}

	// reallocation de pdst si besoin
	Nu32	memsize = psrc->Capacity * psrc->ElementSize;
	if ((pdst->Capacity * pdst->ElementSize) != memsize)
		pdst->pFirst = (NBYTE*)Nrealloc(pdst->pFirst, memsize);
	
	// copie des param�tres:
	pdst->Capacity		= psrc->Capacity;
	pdst->ElementSize	= psrc->ElementSize;
	pdst->Size			= psrc->Size;

	// copie des �l�ments un � un
	NBYTE* ptr_dst = pdst->pFirst;
	NBYTE* ptr_src = psrc->pFirst;

	for (Nu32 i = pdst->Size; i != 0; i--, ptr_src += psrc->ElementSize, ptr_dst += pdst->ElementSize)
	{
		copy_callback(ptr_dst, ptr_src);
	}
}

void NMoveArrayElementBack( NARRAY*parray,const Nu32 index )
{
	NErrorIf(index >= parray->Size,NERROR_ARRAY_INDEX_BEYOND_LIMITS);
	
	// already at the back !!
	if( index == parray->Size-1 )
		return;

	// from this point we are sure that index < parray->Size-1
	NBYTE	*ptr	= parray->pFirst + index*parray->ElementSize;
	NBYTE	*ptmp	= (NBYTE*)Nmalloc(parray->ElementSize);
	memcpy(ptmp,ptr,parray->ElementSize);
	memcpy(ptr,ptr + parray->ElementSize,(parray->Size - index - 1)*parray->ElementSize);
	ptr = parray->pFirst + (parray->Size-1)*parray->ElementSize; // Get the last one Adr
	memcpy(ptr,ptmp,parray->ElementSize);
	Nfree(ptmp);
}

// ------------------------------------------------------------------------------------------
// void NSwapArrayContent
// ------------------------------------------------------------------------------------------
// Description :
//	Very low level function. Use it carefully.
//	It literally swaps all the content between 2 arrays
//	
// ------------------------------------------------------------------------------------------
// In	:
//			array1 and array2
// 
// Out :
//			nothing.
//
// ------------------------------------------------------------------------------------------
void NSwapArrayContent(NARRAY *parray1, NARRAY *parray2)
{
	NARRAY tmp;

	tmp = *parray1;
	*parray1=*parray2;
	*parray2=tmp;
}


// 2021/04/06 
// ------------------------------------------------------------------------------------------
/**
 *	@brief	Check l'integrit� d'une Array par rapport � un descriptif 'Arraybounds'
 *			
 *	@param	parr l'Array � checker
 *	@param	pb arraybounds de reference
 *	@param	bautosetup_zero_array bool�en qui, si il est d�fini � NTRUE, autorise la fonction � faire un setup 'basic' de l'Array UNIQUEMENT dans le cas ou celui-ci est
 *			enti�rement � ZERO ( tous les membres de NARRAY sont �gal � ZERO ). A l'issue de ce setup l'Array aura toujours tous ces membres � ZERO SAUF
 *			ElementSize qui aura pris la valeur de BOUND.ElementSize !
 *
 *	@return		NARRAYCHK_OK			Parfait tout est � l'identique ! ( taille d'un element, capacit� et taille )
 *				NARRAYCHK_ZEROS			Tous les membres de Array sont �gal � ZERO et le param�tre entrant 'bautosetup_zero_array' �tant � NFALSE
 *										la fonction signale juste cette configuration sans faire de setup automatique.
 *				NARRAYCHK_NULLPTR		?! le pointeur parr pass� en param�tre est NULL !	
 *				NARRAYCHK_INCONSISTENCY	Incoh�rence ! ElementSize est NON NULL et diff�rent
 *				NARRAYCHK_CORRUPTED		Corrompu !	ElementSize de Array est NULL mais ces membres Size et/ou capacit� NON NULL !!
 *				NARRAYCHK_AUTOSETUP		l'Array �tait non d�finie avec tous ses membres � ZERO ( elementSize, Size et capacity  = 0 )
 *										Le param�tre entrant 'bautosetup_zero_array' �tant � NTRUE la fonction a effectu� automatiquement un Setup simple 
 *										pour matcher avec ArrayBounds en taille d'�l�ment uniquement.
 *										Aucune allocation m�moire n'est effectu�e par la fonction. Cela signifie que maintenant parr->ElementSize = pb->ElementSize.
 *										La capacit� et La taille de array restent donc � 0.
 *				NARRAYCHK_SIZ_DIFF		Array et Bounds ont la meme taille d'�l�ment la meme capacit� mais la taille differe
 *				NARRAYCHK_CAP_DIFF		Array et Bounds ont la meme taille d'�l�ment, la meme taille mais la capacit� differe
 *				NARRAYCHK_SIZCAP_DIFF	Array et Bounds ont la meme taille d'�l�ment mais ni la meme capacit� ni la meme la taille
 */
 // ------------------------------------------------------------------------------------------
Ns32 NCheckArrayIntegrity(NARRAY* parr, const NARRAYBOUNDS* pb, const Nbool bautosetup_zero_array)
{
	NErrorIf(!pb, NERROR_NULL_POINTER); // Check possible uniquement avec une structure BOUNDS d�finie !!!
	if (!parr)
		return NARRAYCHK_NULLPTR;

	if (parr->ElementSize == 0)
	{
		if (parr->pFirst || parr->Size || parr->Capacity)
		{
			NErrorIf(1, NERROR_ARRAY_CORRUPTED);
			return NARRAYCHK_CORRUPTED; // Signifie m�moire corrompue ( peut-�tre tout simplement non initialis�e, ou alors ... A�e )
		}
		else
		{
			if (bautosetup_zero_array)
			{
				NSetupArray(parr, 0, pb->ElementSize);
				return NARRAYCHK_AUTOSETUP;
			}
			else
				return NARRAYCHK_ZEROS;
		}
	}
	else
	{
		// check integrity
		// En effet, l'array semble "sett�e" car elementSize est NON NULL
		// reste � v�rifier que cela correspond aux valeurs entrantes ( seule la taille d'un element est determinante, taille et capacit� peuvent varier )...
		if (parr->ElementSize != pb->ElementSize)
			return NARRAYCHK_INCONSISTENCY; // Signifie incoh�rence: la taille de l'�lement de l'array est diff�rente de celle d�crite dan sle Bounds
		else
		{
			if (parr->Capacity == pb->Capacity)
			{
				if (parr->Size == pb->Size)
				{
					return NARRAYCHK_OK;
				}
				else
				{
					return NARRAYCHK_SIZ_DIFF;
				}
			}
			else if (parr->Size == pb->Size)
			{
				return NARRAYCHK_CAP_DIFF;
			}
			else
			{
				return NARRAYCHK_SIZCAP_DIFF;
			}
		}
	}
}





void _QuickSortArrayOfNu32Ptr( void **pstart, void **pend, const NARRAY_QSORT_COMPARE compare)
{
	void		*ptemp;
	void        **pleft	= pstart - 1;
	void		**pright= pend	+ 1;
	//void		*pivot	= pstart; // 

	// Nothing to do if array size is null
	if(pstart >= pend)
		return;

	// Otherwise, parsing array one time from right to left, and another time from left to right
	// to look for some miss placed element which swap. If these to process crossing each other, we stop.
	while(1)
	{
		do pright--; while( compare(*pright,*pstart)>0 );
		do pleft ++; while( compare(*pleft,*pstart)<0 );

		if(pleft < pright)
		{
			ptemp	= *pleft;
			*pleft	= *pright;
			*pright	= ptemp;
		}
		else break;
	}

	// Now all the element lower than the pivot are before the ones greater than the pivot.
	// So, we have 2 groups to sort again. To do that, we are going to use quicksort itself !
	_QuickSortArrayOfNu32Ptr( pstart,	pright,	compare );
	_QuickSortArrayOfNu32Ptr( pright+1,	pend,	compare );
}
void _QuickSortArray(NBYTE *pstart, NBYTE *pend, Nu32 size, const NARRAY_QSORT_COMPARE compare )
{
	NBYTE		*ptemp;
	NBYTE       *pleft	= pstart - size;
	NBYTE		*pright = pend	+ size;
	//void		*pivot	= pstart; // 

	// Nothing to do if array size is null
	if(pstart >= pend)
		return;

	// Otherwise, parsing array one time from right to left, and another time from left to right
	// to look for some miss placed element which swap. If these to process crossing each other, we stop.
	while(1)
	{
		do pright-=size; while( compare(pright,pstart)>0 );
		do pleft +=size; while( compare(pleft,pstart)<0  );

		if(pleft < pright)
		{
			ptemp	= (NBYTE*)Nmalloc(size);
			memcpy(ptemp,pleft,size);
			memcpy(pleft,pright,size);
			memcpy(pright,ptemp,size);
			Nfree(ptemp);
		}
		else break;
	}

	// Now all the element lower than the pivot are before the ones greater than the pivot.
	// So, we have 2 groups to sort again. To do that, we are going to use quicksort itself !
	_QuickSortArray( pstart,		pright,	size, compare);
	_QuickSortArray( pright+size,	pend,	size, compare);
}


void NQuickSortArrayOfPtr(NARRAY *parray, const NARRAY_QSORT_COMPARE compare)
{
	NErrorIf(parray->ElementSize != sizeof(void*), NERROR_ARRAY_WRONG_ELEMENT_SIZE);
	if(parray->Size < 2)
		return;
	_QuickSortArrayOfNu32Ptr( (void**)(parray->pFirst), (void**)(parray->pFirst+(parray->Size-1)*sizeof(void*)),compare );
}

void NQuickSortArray(NARRAY *parray, const NARRAY_QSORT_COMPARE compare)
{
	if(parray->Size < 2)
		return;
	_QuickSortArray( parray->pFirst, parray->pFirst+(parray->Size-1)*parray->ElementSize, parray->ElementSize, compare );
}

void NQuickSortArrayOfPtrEx(NARRAY *parray, const Nu32 first, const Nu32 last, const NARRAY_QSORT_COMPARE compare)
{
	NErrorIf(parray->ElementSize != sizeof(void*), NERROR_ARRAY_WRONG_ELEMENT_SIZE);
	if(last-first == 0)
		return;
	_QuickSortArrayOfNu32Ptr( (void**)NGetArrayPtr(parray,first),(void**)NGetArrayPtr(parray,last),compare );
}

void NQuickSortArrayEx(NARRAY *parray, const Nu32 first, const Nu32 last, const NARRAY_QSORT_COMPARE compare)
{
	if(last-first == 0)
		return;
	_QuickSortArray( NGetArrayPtr(parray,first),NGetArrayPtr(parray,last), parray->ElementSize, compare );
}

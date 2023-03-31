#pragma once
/*
#ifndef __NARRAY_H
#define __NARRAY_H
*/

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NArray.h										**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
//#include "../assert.h"
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NErrorHandling.h"
/*
#ifdef __cplusplus
extern "C"
{
#endif
*/
// --------------------------------------------------------------------------------------------------------------------------------------------------------------

// ***************************************************************************************
// **								 Structures											**
// ***************************************************************************************
#define		NARRAY_INCREASE_CAPACITY_RATIO	1.0f		// new capacity = oldcapacity + oldcapacity * NARRAY_INCREASE_CAPACITY_RATIO + 1

// an Array of something
typedef struct
{
	NBYTE	*pFirst;		// Ptr on the Array (on the first element of...)
	Nu32	Size;			// current size of the Array ( = current number of elements in the Array )
	Nu32	Capacity;		// Max possible size of the Array ( pre-allocated and bigger than Size )
	Nu32	ElementSize;	// size of one element of the Array
	
	//void	(*Destructor_CallBack)(NBYTE*pelement);
}NARRAY;

// Just a descriptive structure ( not used by engine but usefull for user... for write/read purpose for example )
typedef struct
{
	Nu32	Size;			// current size of the Array ( = current number of elements in the Array )
	Nu32	Capacity;		// Max possible size of the Array ( pre-allocated and bigger than Size )
	Nu32	ElementSize;	// size of one element of the Array
}NARRAYBOUNDS;
inline NARRAYBOUNDS* NGetArrayBounds(NARRAYBOUNDS* pab, NARRAY* parr) { pab->Capacity = parr->Capacity; pab->ElementSize = parr->ElementSize; pab->Size = parr->Size; return pab; }

#define NARRAYCHK_CORRUPTED			-4		// |
#define NARRAYCHK_INCONSISTENCY		-3		// |__ Les valeurs de retour signalant un probleme potentiel sont n�gatives  
#define NARRAYCHK_NULLPTR			-2		// |
#define NARRAYCHK_ZEROS				-1		// |
#define NARRAYCHK_OK				 0		// +-- La valeur de retour "0" signale une �galit� parfaite entre Array et BOUNDS
#define NARRAYCHK_AUTOSETUP			 1		// |
#define NARRAYCHK_SIZ_DIFF			 2		// |__ Les valeurs de retour > 0 signalent une incoh�rence non critique.
#define NARRAYCHK_CAP_DIFF			 3		// |   	
#define NARRAYCHK_SIZCAP_DIFF		 4		// |
Ns32			NCheckArrayIntegrity(NARRAY* parr, const NARRAYBOUNDS* pb, const Nbool bautosetup_zero_array);
#define NIsArrayCorruptedOrInconsistent(parr,pb,b)			(NCheckArrayIntegrity(parr,pb,b)<-2?1:0)
#define NIsArrayCorruptedOrInconsistentOrNullPtr(parr,pb,b)	(NCheckArrayIntegrity(parr,pb,b)<-1?1:0)
#define NIsArrayAwkward(parr,pb,b)							(NCheckArrayIntegrity(parr,pb,b)< 0?1:0)

// ***************************************************************************************
// **								 Functions											**
// ***************************************************************************************
// a Callback function prototype used for operation on elements ( destruction, ... )
//typedef void (*NARRAY_CALLBACK)(NBYTE*pelement);	
typedef void (*NARRAY_ELEMENT_DESTRUCTOR_CALLBACK)(void*pelement)/*(NBYTE*pelement)*/;	
typedef Nbool (*NARRAY_ENUM_CALLBACK)(void* pelement);
typedef void(*NARRAY_COPYEX_CALLBACK)(void* pdst_element, const void*psrc_element);

//inline void		NArraySetDestructorCallBack(NARRAY* parray,NARRAY_CALLBACK destructor_callback){parray->Destructor_CallBack = destructor_callback;};
NARRAY* 		NSetupArray(NARRAY *parray, const Nu32 capacity,const Nu32 element_size );
NARRAY*			NCreateArray(const Nu32 capacity,const Nu32 element_size);
void			NClearArray(NARRAY* parray,const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback);
void			NDeleteArray(NARRAY* parray,const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback);


void			NCopyArray(NARRAY*pdst,const NARRAY*psrc);
void			NCopyArrayEx(NARRAY* pdst, const NARRAY* psrc, const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback, const NARRAY_COPYEX_CALLBACK copy_callback );
void			NSwapArrayContent(NARRAY *parray1, NARRAY *parray2);

Nu32			NEnumArray(NARRAY* parray, const NARRAY_ENUM_CALLBACK enum_callback);

void			NEraseArray(NARRAY *parray,const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback);
Nbool			NEraseArrayElement(NARRAY *parray,const Nu32 index,const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback);
Nbool			NEraseArrayRange(NARRAY *parray,const Nu32 index,const Nu32 rangesize,const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback);

void			NEraseArrayElementPtr(NARRAY *parray,const NBYTE* ptr, const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback);
//void			NEraseArrayRangePtr(NARRAY *parray,const NBYTE *ptr ,const Nu32 rangesize,const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback);

void			NQuickEraseArrayElement(NARRAY *parray,const Nu32 index, const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback);
void			NQuickEraseArrayElementPtr(NARRAY *parray,const NBYTE* ptr, const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback);


NBYTE*			NInsertArrayElement(NARRAY *parray, const Nu32 refindex,const NBYTE* pelement);

inline	Nu32	NGetArrayCapacity(const NARRAY *parray){ return parray->Capacity;}
inline	Nbool	NIsValidArrayElementPtr(const NARRAY *parray,const NBYTE*ptr)
{
	if(ptr<parray->pFirst)return NFALSE;
	if(!parray->Size)return NFALSE;
	if(!parray->ElementSize)return NFALSE;
	Nu32 id = (ptr-parray->pFirst)/parray->ElementSize;
	if(id>(parray->Size-1))return NFALSE;
	if(ptr != (parray->pFirst+id*parray->ElementSize))return NFALSE;
	return NTRUE;
}

inline	Nbool	NIsValidArrayElementIndex(const NARRAY *parray,const Nu32 id){if(id<parray->Size)return NTRUE;else return NFALSE;}
inline	NBYTE*	NGetFirstArrayPtr(const NARRAY *parray){return parray->pFirst;}
inline	NBYTE*	NGetLastArrayPtr(const NARRAY *parray){if(parray->Size)return parray->pFirst+(parray->Size-1)*parray->ElementSize;else return NULL;}
inline	NBYTE*	NGetArrayPtr(const NARRAY *parray,const Nu32 index){ NErrorIf(index>=parray->Size,NERROR_ARRAY_INDEX_BEYOND_LIMITS);return parray->pFirst+index*parray->ElementSize; }
//inline	Nu32	NGetArrayIndex(const NARRAY *parray,NBYTE*ptr){Nu32 i; NBYTE*ptrb; ptrb = parray->pFirst; for(i=0;i<parray->Size;i++){if(ptrb==ptr){return i;}ptrb+=parray->ElementSize;} 	assert(i<parray->Size); return NVOID;}
inline	Nu32	NGetArrayIndex(const NARRAY *parray,const NBYTE*ptr){NErrorIf(!NIsValidArrayElementPtr(parray,ptr),NERROR_ARRAY_INDEX_BEYOND_LIMITS);return ( (ptr - parray->pFirst)/parray->ElementSize );}


inline	Nu32	NGetArraySize(const NARRAY *parray){ return parray->Size; }
inline	Nu32	NGetArrayElementSize(const NARRAY *parray){ return parray->ElementSize; }
Ns32			NResizeArray(NARRAY* parray, const Nu32 new_size, const NBYTE* pelement, const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback);
inline	Ns32	NUpSizeArray(NARRAY* parray, Nu32 up_size, const NBYTE* pelement) { return NResizeArray(parray, (parray->Size + up_size), pelement, NULL); };

Ns32			NDecreaseArrayCapacity(NARRAY *parray,const Nu32 sub_capacity, const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback );
Ns32			NIncreaseArrayCapacity(NARRAY *parray,const Nu32 add_capacity);
Ns32			NOptimizeArrayCapacity(NARRAY *parray);
Ns32			NSetArrayCapacity(NARRAY *parray,const Nu32 new_capacity, const NARRAY_ELEMENT_DESTRUCTOR_CALLBACK destructor_callback );
#define			NEnsureArrayCapacity(parray,capa)	if ((parray)->Capacity < (capa) ){ NIncreaseArrayCapacity(parray, (capa)-(parray)->Capacity); }

inline Nbool	NIsArrayFull(const NARRAY *parray){if(parray->Capacity==parray->Size)return NTRUE;else return NFALSE;}
inline	Nbool	NIsArrayCleared(const NARRAY *parray){if( NGetArrayCapacity(parray)||NGetArraySize(parray)||NGetFirstArrayPtr(parray)||NGetArrayElementSize(parray) ){return NFALSE;}else{return NTRUE;}}



void			NArrayPopBack(NARRAY *parray, NBYTE*pelement);
NBYTE*			NArrayPushBack(NARRAY*parray,const NBYTE* pelement);
NBYTE*			NArrayBufferPushBack(NARRAY* parray, void* pbuffer, Nu32 nb_buffer_element);
NBYTE*			NArrayAllocBack(NARRAY*parray);
#define			NArrayAllocBack0(parray)	memset(NArrayAllocBack(parray),0,(parray)->ElementSize)
NBYTE*			NArrayAllocInPlace(NARRAY* parray, const Nu32 refindex);

void			NMoveArrayElementBack( NARRAY*parray,const Nu32 index );

typedef Ns32	(*NARRAY_QSORT_COMPARE)(const void *pa, const void *pb); // return values: pa<pb return -1, pa>pb return 1, pa==pb return 0   
void			NQuickSortArray(NARRAY *parray, const NARRAY_QSORT_COMPARE compare);
void			NQuickSortArrayEx(NARRAY *parray, const Nu32 first, const Nu32 last, const NARRAY_QSORT_COMPARE compare);
void			NQuickSortArrayOfPtr(NARRAY *parray, const NARRAY_QSORT_COMPARE compare);
void			NQuickSortArrayOfPtrEx(NARRAY *parray, const Nu32 first, const Nu32 last, const NARRAY_QSORT_COMPARE compare);

/*
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus
#endif // __NARRAY_H 
*/


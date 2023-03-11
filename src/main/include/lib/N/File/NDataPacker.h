#ifndef __NDATAPACKER_H
#define __NDATAPACKER_H

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **									NDatapacker.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/NCStandard.h"
#include "lib/N/NType.h"
#include "lib/N/NFlags.h"
#include "lib/N/Containers/NHashMap.h"
#include "lib/N/Core/NVersion.h"

#include "NDataPack.h"

// ---------------------------------------------------------------------------------------
// FOR USERS: These 2 shapes are the ones you must implement/use for each DataPack you want to create
//
//			Nu32 write_function(void* pelement, NDATAPACKER* pdatapacker, FILE* pfile);
//			Nu32 read_function(void* pvoid, NDATAPACKER* pdatapacker, FILE* pfile);

// NDATAPACKER
//		|
//		|
//		+---- NDATAPACK[0]
//		|			|
//		|			+---- NDPNODE[0]  --> Adress of a Valid NARRAY, of a valid MemBloc, of a valid Single Object.
//		|			|
//		|			+---- NDPNODE[1]  --> Adress of a Valid NARRAY, of a valid MemBloc, of a valid Single Object.
//		|			|
//		|			+---- NDPNODE[2]  --> Adress of a Valid NARRAY, of a valid MemBloc, of a valid Single Object.
//		|			|
//		|			+---- NDPNODE[...]--> Adress of a Valid NARRAY, of a valid MemBloc, of a valid Single Object.
//		|			|
//		|			+---- NDPNODE[I]  --> Adress of a Valid NARRAY, of a valid MemBloc, of a valid Single Object.
//		|
//		|
//		+---- NDATAPACK[1]
//		|			|
//		|			+---- NDPNODE[0]  --> Adress of a Valid NARRAY, of a valid MemBloc, of a valid Single Object.
//		|			|
//		|			+---- NDPNODE[1]  --> Adress of a Valid NARRAY, of a valid MemBloc, of a valid Single Object.
//		|			.
//		|			.
//		|			.
//		|
//		+---- NDATAPACK[...]
//		|			|
//		|			+---- NDPNODE[0]  --> Adress of a Valid NARRAY, of a valid MemBloc, of a valid Single Object.
//		|			|
//		|			+---- NDPNODE[1]  --> Adress of a Valid NARRAY, of a valid MemBloc, of a valid Single Object.
//		.			.
//		.			.
//		.			.

#ifdef __cplusplus
extern "C"
{
#endif
	// --------------------------------------------------------------------------------------------------------------------------------------------------------------

#define FLAG_NDATAPACKER_READING_PROCESS BIT_0
#define FLAG_NDATAPACKER_WRITING_PROCESS BIT_1
#define FLAG_NDATAPACKER_WRITING_PROCESS_AGAIN BIT_2
	typedef struct NDATAPACKER NDATAPACKER;
	struct NDATAPACKER // old name: NSERIALIZER
	{
		Nu32 Flags;
		NDATAPACK *pDataPack;
		Nu16 DataPackTableSize;		  // Full size ( number of allocated NDATAPACK in a row with malloc )
		Nu16 DataPackTableValidEntry; // Number of effective NDATAPACK inserted in the table.
									  // ... Because each insert NDATAPACK is at its own place in the Row ( NDATAPACK.key is this index ).
									  // ... So if DataPackTableSize constains 2 NDATAPACK, the first with key = 1 and the second with key = 16
									  // ... then,
									  // ...			DataPackTableSize will be equal to 17 ( 16 + 1)
									  // ...			the first NDATAPACK is going to be located at index = his key = 1
									  // ...			the second NDATAPACK is going to be located at index = his key = 16
									  // ...			And DataPackTableValidEntry is going to be equal to 2 !
		Nu32 CurrentWrittenDataPack;
	};
#define VERSION_NDATAPACKER_HEADER NMAKEVERSION(0, 0, 0) // >>> 2021/04/04
	typedef struct NDATAPACKERHEADER NDATAPACKERHEADER;
	struct NDATAPACKERHEADER
	{
		Nu16 DataPackTableSize;		  // Size of the DataPack 'table'
		Nu16 DataPackTableValidEntry; // Number of effective NDATAPACK inserted in the table.
	};

	NDATAPACKER *NSetupDataPacker(NDATAPACKER *pdpacker, const Nu16 additionnal_key_nb);
	NDATAPACKER *NCreateDataPacker(const Nu16 additionnal_key_nb);
	void NClearDataPacker(NDATAPACKER *pdpacker);
	void NDeleteDataPacker(NDATAPACKER *pdpacker);

	Nu32 NReadDataPacker(NDATAPACKER *pdpacker, FILE *pfile);
	Nu32 NWriteDataPacker(NDATAPACKER *pdpacker, FILE *pfile);

	NDATAPACK *NInsertDataPack(NDATAPACKER *pserializer, const Nu16 key, const Nu32 elementsize, NDP_WRITE write, NDP_SREAD sread);

	// SERIALIZER : NXDeclareSerialNode function and define(s)
	//				Those function/defines do exactly the same thing than NDeclareSerialNode ( actually, NXDeclareSerialNode simply call NDeclareSerialNode !)
	//				The subtle differrence is just the way to point the Serial where SerialNode will be insterted in !
	//					NXDeclareSerialNode wait for a pointer to serializer and a key to determine the 'pserial' pointer used to insert in
	//					NDeclareSerialNode simply wait the 'pserial' pointer to insert in
	//				except this subtlety both are the same.
	inline void NDeclareDataPackNode(NDATAPACKER *pdpacker, const Nu16 key, const Nu32 cfg, const void *padress, const Nu32 membloc_object_count)
	{
		NErrorIf(key >= pdpacker->DataPackTableSize, NERROR_VALUE_OUTOFRANGE);
		NErrorIf(pdpacker->DataPackTableValidEntry > pdpacker->DataPackTableSize, NERROR_INCONSISTENT_VALUES);
		NErrorIf(pdpacker->pDataPack[key].Header.Key != key, NERROR_INCONSISTENT_VALUES);
		NErrorIf(ISFLAG_OFF(pdpacker->Flags, FLAG_NDATAPACKER_READING_PROCESS | FLAG_NDATAPACKER_READING_PROCESS) && ISFLAG_ON(pdpacker->pDataPack[key].Flags, FLAG_NDATAPACK_LOCKED), NERROR_INCONSISTENT_FLAGS);
		NErrorIf(ISFLAG_ON(pdpacker->Flags, FLAG_NDATAPACKER_READING_PROCESS | FLAG_NDATAPACKER_READING_PROCESS) && ISFLAG_OFF(pdpacker->pDataPack[key].Flags, FLAG_NDATAPACK_LOCKED), NERROR_INCONSISTENT_FLAGS);
		NInsertDataPackNode(&pdpacker->pDataPack[key], cfg, padress, membloc_object_count);
	}
	/*
	#define NXDeclareObject_UserPreAllocObj(psrlz,key,ptr)													NXDeclareSerialNode(psrlz,key, USER_PREALLOC_OBJ, (void*)ptr, 0, NULL)
	#define NXDeclareObject_SerializerAllocObj(psrlz,key,ptr)												NXDeclareSerialNode(psrlz,key, SERIAL_ALLOC_OBJ, (void*)ptr, 0, NULL)

	#define NXDeclareArrayOfObjects_UserPreAllocArrayAndObj(psrlz,key,parray)								NXDeclareSerialNode(psrlz,key, USER_PREALLOC_ARRAY_AND_OBJ, (void*)parray, 0, NULL)
	#define NXDeclareArrayOfObjects_UserPreAllocArray_SerializerAllocObj(psrlz,key,parray)					NXDeclareSerialNode(psrlz,key, USER_PREALLOC_ARRAY__SERIAL_ALLOC_OBJ, (void*)parray, 0, NULL)
	#define NXDeclareArrayOfObjects_SerializerAllocArrayAndObj(psrlz,key,parray)							NXDeclareSerialNode(psrlz,key, SERIAL_ALLOC_ARRAY_AND_OBJ, (void*)parray, 0, NULL)

	#define NXDeclareMemBlocOfObjects_UserPreAllocMemBlocAndObj(psrlz,key,pmbloc,objcount)					NXDeclareSerialNode(psrlz,key, USER_PREALLOC_MBLOC_AND_OBJ, (void*)pmbloc,objcount, NULL)
	#define NXDeclareMemBlocOfObjects_UserPreAllocMemBloc_SerializerAllocObj(psrlz,key,pmbloc,objcount)		NXDeclareSerialNode(psrlz,key, USER_PREALLOC_MBLOC__SERIAL_ALLOC_OBJ, (void*)pmbloc,objcount, NULL)
	#define NXDeclareMemBlocOfObjects_SerializerAllocMemBlocAndObj(psrlz,key,pmbloc,objcount)				NXDeclareSerialNode(psrlz,key, SERIAL_ALLOC_MBLOC_AND_OBJ, (void*)pmbloc,objcount, NULL)

	#define NXDeclareArrayOfPointers_UserPreAllocArrayAndPtr(psrlz,key,parray)								NXDeclareSerialNode(psrlz,key, USER_PREALLOC_ARRAY_AND_PTR, (void*)parray, 0, NULL)
	#define NXDeclareArrayOfPointers_UserPreAllocArray_SerialzerAllocPtr(psrlz,key,parray)					NXDeclareSerialNode(psrlz,key, USER_PREALLOC_ARRAY__SERIAL_ALLOC_PTR, (void*)parray, 0, NULL)
	#define NXDeclareArrayOfPointers_SerializerAllocArrayAndPtr(psrlz,key,parray)							NXDeclareSerialNode(psrlz,key, SERIAL_ALLOC_ARRAY_AND_PTR, (void*)parray, 0, NULL)
	#define NXDeclareArrayOfPointers_UserPreAllocArray_BatchAllocPtr(psrlz,key,parray,pbatch)				NXDeclareSerialNode(psrlz,key, USER_PREALLOC_ARRAY__BATCH_ALLOC_PTR, (void*)parray, 0, pbatch)

	#define NXDeclareMemBlocOfPointers_UserPreAllocMemBlocAndPtr(psrlz,key,pmbloc,objcount)					NXDeclareSerialNode(psrlz,key, USER_PREALLOC_MBLOC_AND_PTR, (void*)pmbloc,objcount, NULL)
	#define NXDeclareMemBlocOfPointers_UserPreAllocMemBloc_SerialzerAllocPtr(psrlz,key,pmbloc,objcount)		NXDeclareSerialNode(psrlz,key, USER_PREALLOC_MBLOC__SERIAL_ALLOC_PTR, (void*)pmbloc,objcount, NULL)
	#define NXDeclareMemBlocOfPointers_SerializerAllocMemBlocAndPtr(psrlz,key,pmbloc,objcount)				NXDeclareSerialNode(psrlz,key, SERIAL_ALLOC_MBLOC_AND_PTR, (void*)pmbloc,objcount, NULL)
	#define NXDeclareMemBlocOfPointers_UserPreAllocMemBloc_BatchAllocPtr(psrlz,key,pmbloc,objcount,pbatch)	NXDeclareSerialNode(psrlz,key, USER_PREALLOC_MBLOC__BATCH_ALLOC_PTR, (void*)pmbloc,objcount, pbatch)
	*/
	Nu32 NGetDataPackerIndex(NDATAPACKER *pdpacker, const void *ptr, const Nu32 key);
	void *NGetDataPackerPointer(const NDATAPACKER *pdpacker, const Nu32 index, const Nu32 key);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus
#endif // __NDATAPACKER_H

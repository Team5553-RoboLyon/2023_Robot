#include "../NCStandard.h"
#include "../NMath.h"
#include "../NErrorHandling.h"
/*
#include "../NCore.h"
#include "../GL/Ngl.h"
#include "../NType.h"
#include "../File/NFile.h"
#include "../File/NFileExt.h"
#include "../NString.h"
*/
#include "../Core/NVersion.h"
#include "NFile.h"
#include "NDataPackKey.h"
#include "NDataPackNode.h"
#include "NDataPack.h"
#include "NDataPacker.h"

/*
static void _DataPackDestructor(void* ptr)
{
	NDeleteDataPack((NDATAPACK*)ptr);
}
*/
// ------------------------------------------------------------------------------------------
/**
 *	@brief	Setup DataPacker
 *
 *	@param	ppacker is a pointer to the datapacker to setup
 *	@param	additionnal_key_nb represent the number of additionnal keys to "pre"allocate with the number of system keys at setup. 
 *			It's possible to let it to ZERO. But, to avoid memory realloc each time a datapack with a new key is inserted 
 *			is a good practice to set this value in the right way.
 *	@return ppacker itself
 */
 // ------------------------------------------------------------------------------------------
NDATAPACKER* NSetupDataPacker(NDATAPACKER* ppacker, const Nu16 additionnal_key_nb)
{
	ppacker->pDataPack				= (NDATAPACK*)Nmalloc(sizeof(NDATAPACK) * (NDPKEY_SYSTEM_COUNT + additionnal_key_nb));	// By default, allocation of the minimum which correspond to the exact number of System NSERIALKEY
	memset(ppacker->pDataPack, 0xFF, sizeof(NDATAPACK) * (NDPKEY_SYSTEM_COUNT + additionnal_key_nb));						// Fill memory with a full of 0xFFFFFFFFFFFF. Like this all pserial->key will be set as INVALID ( NSERIALKEY_INVALID = NVOID = 0xFFFFFFFF)
	ppacker->DataPackTableSize			= NDPKEY_SYSTEM_COUNT + additionnal_key_nb;
	ppacker->DataPackTableValidEntry	= 0;
	ppacker->Flags						= 0;
	//pcollector->CurrentWrittenDataPack= ?;
	return ppacker;
}
NDATAPACKER* NCreateDataPacker(const Nu16 additionnal_key_nb)
{
	return NSetupDataPacker(NEW(NDATAPACKER), additionnal_key_nb);
}
void	NClearDataPacker(NDATAPACKER* pdpacker)
{
	NErrorIf( ISFLAG_ON(pdpacker->Flags, (FLAG_NDATAPACKER_READING_PROCESS | FLAG_NDATAPACKER_READING_PROCESS)), NERROR_UNAUTHORIZED_ACTION);
	pdpacker->Flags = 0;
	NDATAPACK* pdatapack = pdpacker->pDataPack;
	for (Nu16 i = 0; i < pdpacker->DataPackTableSize; i++, pdatapack++)
	{
		if (pdatapack->Header.Key == NDPKEY_INVALID)
			continue;
		else
			NClearDataPack(pdatapack);
	}
	Nfree_NULL(pdpacker->pDataPack);
	pdpacker->DataPackTableSize = 0;
}
void	NDeleteDataPacker(NDATAPACKER* ppacker)
{
	NClearDataPacker(ppacker);
	Nfree(ppacker);
}

NDATAPACK* NInsertDataPack(NDATAPACKER* pdpacker, const Nu16 key, const Nu32 elementsize, NDP_WRITE write, NDP_SREAD sread)
{
	NErrorIf(key > NDPKEY_WATCHDOG, NERROR_SYSTEM_CHECK);
	NErrorIf(pdpacker->DataPackTableValidEntry > pdpacker->DataPackTableSize, NERROR_INCONSISTENT_VALUES);						// Weird !!!
	NErrorIf( ( key< pdpacker->DataPackTableSize) && ( pdpacker->pDataPack[key].Header.Key != NDPKEY_INVALID) , NERROR_ALREADY_DONE);	// Initialization of the serial seems to be already done !!!

	if (ISFLAG_OFF(pdpacker->Flags, FLAG_NDATAPACKER_READING_PROCESS | FLAG_NDATAPACKER_READING_PROCESS))
	{
		if (key >= pdpacker->DataPackTableSize)
		{
			pdpacker->pDataPack = (NDATAPACK*)Nrealloc(pdpacker->pDataPack, sizeof(NDATAPACK) * (key + 1));
			// Set all new allocated serials to ZEROS
			memset(&pdpacker->pDataPack[pdpacker->DataPackTableSize], 0xFF, sizeof(NDATAPACK) * ((key + 1) - pdpacker->DataPackTableSize));
			pdpacker->DataPackTableSize = key + 1;
		}
		NSetupDataPack(&pdpacker->pDataPack[key], key, elementsize, write, sread);
		pdpacker->DataPackTableValidEntry += 1;
	}
#ifdef _DEBUG
	else
	{
		// FLAG_NSERIALIZER_READ_ONLY and/or  FLAG_NSERIALIZER_WRITE_ONLY is/are ON !!!
		NErrorIf(1, NERROR_UNAUTHORIZED_ACTION);
	}
#endif

	return &pdpacker->pDataPack[key];
}

Nu32 NGetDataPackerIndex(NDATAPACKER*pdpacker, const void* ptr, const Nu32 key)
{
	NErrorIf(key >= pdpacker->DataPackTableSize, NERROR_VALUE_OUTOFRANGE);
	NErrorIf(pdpacker->DataPackTableValidEntry > pdpacker->DataPackTableSize, NERROR_INCONSISTENT_VALUES);						// Weird !!!
	NErrorIf(pdpacker->pDataPack[key].Header.Key != key, NERROR_INCONSISTENT_VALUES);

	// Get Index is only available in  WRITE mode !!!
	if (FLAGS_TEST(pdpacker->Flags, (FLAG_NDATAPACKER_READING_PROCESS | FLAG_NDATAPACKER_WRITING_PROCESS), FLAG_NDATAPACKER_WRITING_PROCESS))
	{
		if (!ptr)
			return NVOID;
		NDATAPACK* pdp = &pdpacker->pDataPack[key];
		void** vptr = (void**)pdp->ArrayOfPtr.pFirst;
		for (Nu32 i = 0; i < pdp->ArrayOfPtr.Size; i++, vptr++)
		{
			if (*vptr == ptr)
				return i;
		}
		// Passing here means : ptr is not declared yet...
		// So let's do it ! Right now !
		// FIRST: we are sure to be inside WRITING PROCESS ... so check if the pushback will be done into the serial in process or a serial already processed,  OR NOT ...
		//												   ... which means in a serial not already processed ! ( which is absolutly not a problem )
		if (key <= pdpacker->CurrentWrittenDataPack)
			FLAG_ON(pdpacker->Flags, FLAG_NDATAPACKER_WRITING_PROCESS_AGAIN);

		NArrayPushBack(&pdp->ArrayOfPtr, (NBYTE*)&ptr);
		return pdp->ArrayOfPtr.Size - 1;
	}
	else
	{
		// NLSERIALIZER::FLAGS::READ_ONLY /  NLSERIALIZER::FLAGS::WRITE_ONLY  are not in the right configuration !
		// ONLY NLSERIALIZER::FLAGS::WRITE_ONLY must be ON
		NErrorIf(1, NERROR_UNAUTHORIZED_ACTION);

		return NVOID; 
	}
}

void* NGetDataPackerPointer(const NDATAPACKER* pdpacker, const Nu32 index, const Nu32 key)
{
	NErrorIf(key >= pdpacker->DataPackTableSize, NERROR_VALUE_OUTOFRANGE);
	NErrorIf(pdpacker->DataPackTableValidEntry > pdpacker->DataPackTableSize, NERROR_INCONSISTENT_VALUES);// Weird !!!
	NErrorIf(pdpacker->pDataPack[key].Header.Key != key, NERROR_INCONSISTENT_VALUES);

	// Get Pointer is only available in READ mode !!!
	if (FLAGS_TEST(pdpacker->Flags, (FLAG_NDATAPACKER_READING_PROCESS | FLAG_NDATAPACKER_WRITING_PROCESS), FLAG_NDATAPACKER_READING_PROCESS))
	{
		if (index == NVOID)
			return NULL;

		NDATAPACK* pdp = &pdpacker->pDataPack[key];
		NErrorIf(index >= pdp->ArrayOfPtr.Size, NERROR_VALUE_OUTOFRANGE);
		return (void*)((NBYTE**)pdp->ArrayOfPtr.pFirst)[index];
	}
	else
	{
		// NLSERIALIZER::FLAGS::READ_ONLY /  NLSERIALIZER::FLAGS::WRITE_ONLY  ne sont pas dans la bonne config !
		// Seul NLSERIALIZER::FLAGS::READ_ONLY doit être ON !!!!
		NErrorIf(1, NERROR_UNAUTHORIZED_ACTION);

		return NULL; // ce qui techniquement va faire planter un peu plus loin si l'utilisateur ne fait pas attention !!!
	}
}

Nu32 NReadDataPacker(NDATAPACKER* pdpacker, FILE* pfile)
{
	FILE* plog = fopen("__NReadDataPacker_log.txt", "w");
	fprintf(plog, "\n[%d]	OPEN LOG\n", ftell(pfile));

	NErrorIf(ISFLAG_ON(pdpacker->Flags, (FLAG_NDATAPACKER_READING_PROCESS | FLAG_NDATAPACKER_WRITING_PROCESS)), NERROR_UNAUTHORIZED_ACTION);

	Nu32		_ftell;
	Nu32		i,j,k,sz;
	Nu32		what;
	NBYTE		*ptr;
	NBYTE		**pptr;

	NDATAPACKERHEADER		datapacker_header;
	NDATAPACKNODE			*pdatapacknode;
	NDATAPACKNODEHEADER		datapacknode_header;
	NDATAPACKHEADER			datapack_header;
	// 0) Keep in mind:
	Nu32 start_file = ftell(pfile);

	// 1) Initialization : Version read 
	fprintf(plog, "\n+-------------------------------------------------------------------------------------------------------------------------\n");
	fprintf(plog, "|\n");
	fprintf(plog, "|	1) Initialization : version and header\n");
	fprintf(plog, "|\n");
	fprintf(plog, "+-------------------------------------------------------------------------------------------------------------------------\n");

	Nu32	version_u32;
	fprintf(plog, "[%d] read VERSION_NDATAPACKER_HEADER[size=%d]\n", ftell(pfile), sizeof(Nu32));
	if (fread(&version_u32, sizeof(Nu32), 1, pfile) != 1)
	{
		fseek(pfile, start_file, SEEK_SET);
		return 0;
	}
	// ... simplified version managment ( no switch case, just a simple test ! just because there is no version yet ...)
	if (version_u32 != NGETVERSION_MAIN(VERSION_NDATAPACKER_HEADER))
	{
		fseek(pfile, start_file, SEEK_SET);
		return 0;
	}

	// ... Datapacker Header read
	fprintf(plog, "[%d] read NDATAPACKER_HEADER[size=%d]\n", ftell(pfile), sizeof(NDATAPACKERHEADER));
	if (fread(&datapacker_header, sizeof(NDATAPACKERHEADER), 1, pfile) != 1)
	{
		fseek(pfile, start_file, SEEK_SET);
		return 0;
	}

	// First consistency Checks between incoming and in memory data
	if (datapacker_header.DataPackTableSize != pdpacker->DataPackTableSize)
	{
		NErrorIf(1, NERROR_INCONSISTENT_PARAMETERS);
		fseek(pfile, start_file, SEEK_SET);
		return 0;
	}
	if (datapacker_header.DataPackTableValidEntry != pdpacker->DataPackTableValidEntry)
	{
		NErrorIf(1, NERROR_INCONSISTENT_PARAMETERS);
		fseek(pfile, start_file, SEEK_SET);
		return 0;
	}
	// 2) Initialization: check consistency with incomming Data type
	fprintf(plog, "\n+-------------------------------------------------------------------------------------------------------------------------\n");
	fprintf(plog, "|\n");
	fprintf(plog, "|	2) Initialization: check consistency with incomming Data type\n");
	fprintf(plog, "|\n");
	fprintf(plog, "+-------------------------------------------------------------------------------------------------------------------------\n");


	fprintf(plog, "\nfor DATAPACK in DATAPACKTABLELIST Only valid Entry !!! (%d)\n", datapacker_header.DataPackTableValidEntry);
	fprintf(plog, "	|\n");
	Nu32 wkey;
	for (i = 0; i < datapacker_header.DataPackTableValidEntry; i++)
	{
		// read
		fprintf(plog, "[%d]	+--read NDATAPACK#%d_HEADER[size=%d]\n", ftell(pfile), i, sizeof(Nu32));
		if (fread(&wkey, sizeof(Nu32), 1, pfile) != 1)
		{
			fseek(pfile, start_file, SEEK_SET);
			return 0;
		}
		// and check...
		// Read key ( wkey ) must match with all valid entries in the current DataPacker configuration
		if ((wkey >= pdpacker->DataPackTableSize) || (pdpacker->pDataPack[wkey].Header.Key != wkey) )
		{
			NErrorIf(1, NERROR_INCONSISTENT_PARAMETERS);
			fseek(pfile, start_file, SEEK_SET);
			return 0;
		}
	}

	// 3) All DataPacks READ initializations
	// 
	fprintf(plog, "\n+-------------------------------------------------------------------------------------------------------------------------\n");
	fprintf(plog, "|\n");
	fprintf(plog, "|	3) All DataPacks READ initializations\n");
	fprintf(plog, "|\n");
	fprintf(plog, "+-------------------------------------------------------------------------------------------------------------------------\n");


	fprintf(plog, "\nfor DATAPACK in DATAPACKTABLELIST(%d)\n", pdpacker->DataPackTableSize);
	NDATAPACK* pdatapack = pdpacker->pDataPack;
	for (i = 0; i < pdpacker->DataPackTableSize; i++, pdatapack++)
	{
		// just pass invalid DataPack
		if (pdatapack->Header.Key == NDPKEY_INVALID)
			continue;

		// ...read for a valid one:
		fprintf(plog, "		|\n");
		fprintf(plog, "[%d]		+--DATAPACK#%d read DATAPACKHEADER [size=%d]\n",ftell(pfile), i, sizeof(NDATAPACKHEADER));
		if (fread(&datapack_header, sizeof(NDATAPACKHEADER), 1, pfile) != 1)
			return 0;
		// first of all: Consistency check ...
		if ( (datapack_header.Key != pdatapack->Header.Key) || (datapack_header.Key != i) )
		{
			NErrorIf(1, NERROR_INCONSISTENT_PARAMETERS);
			fseek(pfile, start_file, SEEK_SET);
			return 0;
		}
		if (datapack_header.ElementSize != pdatapack->Header.ElementSize)
		{
			NErrorIf(1, NERROR_INCONSISTENT_PARAMETERS);
			fseek(pfile, start_file, SEEK_SET);
			return 0;
		}
		if (datapack_header.ArrayOfDataPackNodesSize != pdatapack->ArrayOfDataPackNodes.Size)
		{
			NErrorIf(1, NERROR_INCONSISTENT_PARAMETERS);
			fseek(pfile, start_file, SEEK_SET);
			return 0;
		}
		// Then, if we arrive here, that means all things are consistent...
		// So, parse all the DataPAckNodes and insert all their pointers according to their specificities
		// !!! IF NECESSARY ALLOCATE MEMORY FOR SOME OF THE OBJECTS THESE POPINTERS ARE SUPPOSED TO POINT ON !!!

		// Reset the process pointer Array ( written/read )
		fprintf(plog, "		+--DATAPACK#%d reset ArrayOfPtr size to ZERO\n", i);
		NEraseArray(&pdatapack->ArrayOfPtr, NULL);
		pdatapack->Processed = 0;

		if (pdatapack->ArrayOfDataPackNodes.Size)
		{
			fprintf(plog, "		+--DATAPACK#%d for DATAPACKNODE in DATAPACKNODESLIST(%d)\n", i, pdatapack->ArrayOfDataPackNodes.Size);
			fprintf(plog, "			|\n");
		}
		pdatapacknode = (NDATAPACKNODE*)pdatapack->ArrayOfDataPackNodes.pFirst;
		for (j = 0; j < pdatapack->ArrayOfDataPackNodes.Size; j++, pdatapacknode++)
		{
			// a) Check 
			fprintf(plog, "[%d]			+--read NDATAPACKNODEHEADER#%d[size=%d]\n", ftell(pfile), j, sizeof(NDATAPACKNODEHEADER));
			if (fread(&datapacknode_header, sizeof(NDATAPACKNODEHEADER), 1, pfile) != 1)
			{
				NErrorIf(1, NERROR_FILE_READ_ERROR);
				fseek(pfile, start_file, SEEK_SET);
				return 0;
			}
			if ((datapacknode_header.Config != pdatapacknode->Config))
			{
				NErrorIf(1, NERROR_INCONSISTENT_PARAMETERS);
				fseek(pfile, start_file, SEEK_SET);
				return 0;
			}

			if( NIS_DPCFG_ARRAY_OR_MEMBLOC(pdatapacknode->Config) && 
				NIS_DPCFG_ARRAY_ALLOC_BY_USER(pdatapacknode->Config) &&
				(datapacknode_header.ObjectsCount != pdatapacknode->ObjectsCount))
			{
				NErrorIf(1, NERROR_INCONSISTENT_PARAMETERS);
				fseek(pfile, start_file, SEEK_SET);
				return 0;
			}


			what = NGET_DPCFG_WHAT(pdatapacknode->Config);

			switch(what)
			{
			case NDPCFG_LONELY_PTR:
				NErrorIf(NIS_DPCFG_ARRAY_ALLOC_BY_USER(pdatapacknode->Config), NERROR_INCONSISTENT_PARAMETERS); // Array/membloc specific flag must still OFF... a lonely pointer is ...  alone.
				NErrorIf(NIS_DPCFG_OF_PTRS(pdatapacknode->Config), NERROR_INCONSISTENT_PARAMETERS);				// Array/membloc specific flag must still OFF... a lonely pointer is always pointing to 'an object' 
				
				// In this specific case we know the exact value of ObjectsCount, it should be 1 ! ALWAYS !!!
				//  = 1 if OBJECT allocation is managed by user
				//  = 1 if OBJECT allocation is managed by DataPacker
				//  ... so, always 1 ! :)
				NErrorIf(datapacknode_header.ObjectsCount != pdatapacknode->ObjectsCount, NERROR_INCONSISTENT_VALUES);
				NErrorIf(datapacknode_header.ObjectsCount != 1, NERROR_INCONSISTENT_VALUES);

				if (NIS_DPCFG_OBJ_ALLOC_BY_DP(pdatapacknode->Config) )
				{
					NErrorIf(pdatapacknode->pAdress, NERROR_NON_NULL_POINTER); // In this case pAdress should be set to NULL by user 
					pdatapacknode->pAdress = (NBYTE*)Nmalloc_mem0(pdatapack->Header.ElementSize);// allocation and memory init. to ZERO !
					fprintf(plog, "			+-- DATAPACKNODE#%d Nmalloc_mem0 LONELY_PTR (0x%p) Managed by DP\n", j, pdatapacknode->pAdress);
				}
				#ifdef _DEBUG
				else // NIS_DPCFG_OBJ_ALLOC_BY_USER(psn->Config)
				{
					NErrorIf(!pdatapacknode->pAdress, NERROR_NULL_POINTER); // In this case pAdress should be NON NULL and defined by user  !
					fprintf(plog, "			+-- DATAPACKNODE#%d  LONELY_PTR (0x%p) Managed by USER\n", j, pdatapacknode->pAdress);
				}
				#endif	

				fprintf(plog, "			+-- DATAPACKNODE#%d Pushback LONELY_PTR (0x%p)\n", j, pdatapacknode->pAdress);
				NArrayPushBack(&pdatapack->ArrayOfPtr, (NBYTE*)&pdatapacknode->pAdress);
				break;

			case NDPCFG_ARRAY:
				// A 'ptr'  has a size of : sizeof(void*) so...
				NErrorIf(NIS_DPCFG_OF_PTRS(pdatapacknode->Config) && ((NARRAY*)(pdatapacknode->pAdress))->ElementSize != sizeof(void*), NERROR_ARRAY_WRONG_ELEMENT_SIZE);						// Array elementsize has to be sizeof(void*)
				// An 'Object' in NARRAY has a size of: NARRAY.ElementSize, so ...
				NErrorIf(NIS_DPCFG_OF_OBJECTS(pdatapacknode->Config) && ((NARRAY*)(pdatapacknode->pAdress))->ElementSize != pdatapack->Header.ElementSize, NERROR_ARRAY_WRONG_ELEMENT_SIZE);	// incoming and setup data must be consistent !

				if (NIS_DPCFG_ARRAY_ALLOC_BY_DP(pdatapacknode->Config))
				{
					NErrorIf(((NARRAY*)(pdatapacknode->pAdress))->Size, NERROR_ARRAY_IS_NOT_EMPTY); // Array as to be resized to NULL be user !
					fprintf(plog, "			+-- DATAPACKNODE#%d Resize ARRAY(0x%p) Managed by DP ( new size = %d )\n", j, pdatapacknode->pAdress, datapacknode_header.ObjectsCount);
					NResizeArray((NARRAY*)pdatapacknode->pAdress, datapacknode_header.ObjectsCount, NULL, NULL);
				}
				#ifdef _DEBUG
				else // NIS_DPCFG_ARRAY_ALLOC_BY_USER(pdatapacknode->Config)
				{
					NErrorIf(datapacknode_header.ObjectsCount != pdatapacknode->ObjectsCount, NERROR_INCONSISTENT_VALUES);
					NErrorIf(((NARRAY*)(pdatapacknode->pAdress))->Size != pdatapacknode->ObjectsCount, NERROR_WRONG_SIZE); // In this case array as to be set at the right size by user !
					fprintf(plog, "			+-- DATAPACKNODE#%d ARRAY(0x%p) Managed by User ( size = %d )\n", j, pdatapacknode->pAdress, datapacknode_header.ObjectsCount);
				}
				#endif	
				if (NIS_DPCFG_OF_OBJECTS(pdatapacknode->Config))
				{
					NErrorIf(((NARRAY*)(pdatapacknode->pAdress))->Size != datapacknode_header.ObjectsCount, NERROR_INCONSISTENT_VALUES);
					ptr = ((NARRAY*)(pdatapacknode->pAdress))->pFirst;
					fprintf(plog, "			+-- DATAPACKNODE#%d Pushback ARRAY(0x%p)\n", j, pdatapacknode->pAdress);
					fprintf(plog, "			    for OBJECT in  DATAPACKNODE#%d->ARRAY (%d)\n", j, datapacknode_header.ObjectsCount);
					fprintf(plog, "					|\n");
					for (k = 0; k < datapacknode_header.ObjectsCount; k++, ptr += pdatapack->Header.ElementSize)
					{
						fprintf(plog, "					+-- pushback PTR on ARRAY OBJECT#%d (0x%p)\n", k, ptr);
						NArrayPushBack(&pdatapack->ArrayOfPtr, (NBYTE*)&ptr);
					}
				}
				else
				{
					NErrorIf(((NARRAY*)(pdatapacknode->pAdress))->Size != datapacknode_header.ObjectsCount, NERROR_INCONSISTENT_VALUES);
					pptr = (NBYTE**)((NARRAY*)(pdatapacknode->pAdress))->pFirst;
					fprintf(plog, "			+-- DATAPACKNODE#%d Pushback ARRAY(0x%p)\n", j, pdatapacknode->pAdress);
					fprintf(plog, "			    for PTR in  DATAPACKNODE#%d->ARRAY (%d)\n", j, datapacknode_header.ObjectsCount);
					fprintf(plog, "					|\n");
					for (k = 0; k < datapacknode_header.ObjectsCount; k++, pptr++)
					{
						fprintf(plog, "					+-- pushback ARRAY PTR#%d (0x%p)\n", k, *pptr);
						NArrayPushBack(&pdatapack->ArrayOfPtr, (NBYTE*)pptr);
					}
				}
				break;

			case NDPCFG_MEMBLOC:
				if (NIS_DPCFG_MBLOC_ALLOC_BY_DP(pdatapacknode->Config))
				{
					fprintf(plog, "			+-- DATAPACKNODE#%d Allocate MemBLoc(0x%p) Managed by DP ( new size = %d )\n", j, pdatapacknode->pAdress, datapacknode_header.ObjectsCount);
					NErrorIf(pdatapacknode->pAdress!=NULL, NERROR_NON_NULL_POINTER); // In this case user has to set pAdress to NULL !!!
					if (NIS_DPCFG_OF_PTRS(pdatapacknode->Config))
					{
						pdatapacknode->pAdress = (void*)Nmalloc(datapacknode_header.ObjectsCount * sizeof(void*));
						fprintf(plog, "			+-- DATAPACKNODE#%d MemBLoc of Ptr Allocated (0x%p) by DP ( new size = %d )\n", j, pdatapacknode->pAdress, datapacknode_header.ObjectsCount);
					}
					else
					{
						pdatapacknode->pAdress = (void*)Nmalloc(datapacknode_header.ObjectsCount * pdatapack->Header.ElementSize);
						fprintf(plog, "			+-- DATAPACKNODE#%d MemBLoc of Objects Allocated (0x%p) by DP ( new size = %d )\n", j, pdatapacknode->pAdress, datapacknode_header.ObjectsCount);
					}
				}
				#ifdef _DEBUG
				else // NIS_DPCFG_MBLOC_ALLOC_BY_USER(pdatapacknode->Config)
				{
					NErrorIf(datapacknode_header.ObjectsCount != pdatapacknode->ObjectsCount, NERROR_INCONSISTENT_VALUES);
					fprintf(plog, "			+-- DATAPACKNODE#%d MemBloc(0x%p) Managed by User ( size = %d )\n", j, pdatapacknode->pAdress, datapacknode_header.ObjectsCount);
				}
				#endif	
				if (NIS_DPCFG_OF_OBJECTS(pdatapacknode->Config))
				{

					ptr = (NBYTE*)pdatapacknode->pAdress;
					fprintf(plog, "			+-- DATAPACKNODE#%d Pushback MemBloc(0x%p)\n", j, pdatapacknode->pAdress);
					fprintf(plog, "			    for OBJECT in  DATAPACKNODE#%d->MemBloc (%d)\n", j, datapacknode_header.ObjectsCount);
					fprintf(plog, "					|\n");
					for (k = 0; k < datapacknode_header.ObjectsCount; k++, ptr += pdatapack->Header.ElementSize)
					{
						fprintf(plog, "					+-- pushback MemBloc PTR#%d (0x%p)\n", k, ptr);
						NArrayPushBack(&pdatapack->ArrayOfPtr, (NBYTE*)&ptr);
					}
				}
				else
				{
					
					pptr = (NBYTE**)pdatapacknode->pAdress;
					fprintf(plog, "			+-- DATAPACKNODE#%d Pushback MemBloc(0x%p)\n", j, pdatapacknode->pAdress);
					fprintf(plog, "			    for PTR in  DATAPACKNODE#%d->MemBloc (%d)\n", j, datapacknode_header.ObjectsCount);
					fprintf(plog, "					|\n");

					for (k = 0; k < datapacknode_header.ObjectsCount; k++, pptr++)
					{
						fprintf(plog, "					+-- pushback PTR on MemBloc OBJECT#%d (0x%p)\n", k, *pptr);
						NArrayPushBack(&pdatapack->ArrayOfPtr, (NBYTE*)pptr);
					}
				}
				break;

			default:
				NErrorIf(1, NERROR_INCONSISTENT_PARAMETERS);
				fseek(pfile, start_file, SEEK_SET);
				return 0;
			}
		}
		// So all the DataPackNodes ( the pre-declared stuff ) are allocated and the associated pointers are pushed into ArrayOfPtr.
		NErrorIf(pdatapack->ArrayOfPtr.Size > datapack_header.ArrayOfPtrSize, NERROR_INCONSISTENT_VALUES);
		// ... we allocate right now all the other objects which belong to this datapack, one by one.
		sz = datapack_header.ArrayOfPtrSize - pdatapack->ArrayOfPtr.Size;

		if (sz)
		{
			fprintf(plog, "		+--DATAPACK#%d for OBJECT in range(%d)\n", i, sz);
			fprintf(plog, "			|\n");
		}
		else
		{
			fprintf(plog, "		+--DATAPACK#%d No more OBJECT To allocate and Pushback\n", i);
		}
		for (k = 0; k < sz; k++)
		{
			ptr = (NBYTE*)Nmalloc(pdatapack->Header.ElementSize);
			NArrayPushBack(&pdatapack->ArrayOfPtr, (NBYTE*)&ptr);
			fprintf(plog, "			+--Malloc OBJECT#%d [size=%d]\n", k, pdatapack->Header.ElementSize);
			fprintf(plog, "			+--PushBack OBJECT#%d Ptr (%p)\n", k, ptr);
		}
		
		NErrorIf(pdatapack->Processed != 0, NERROR_NON_NULL_VALUE);
		pdatapack->Processed = 0;
	}

	// 4) Reading LOOPS:
	fprintf(plog, "\n+-------------------------------------------------------------------------------------------------------------------------\n");
	fprintf(plog, "|\n");
	fprintf(plog, "|	4) Reading LOOPS\n");
	fprintf(plog, "|\n");
	fprintf(plog, "+-------------------------------------------------------------------------------------------------------------------------\n");


	// Memory allocations are all done. So, it's time to read alls the contents
	// First, reading the number of 'loops'
	Nu32 process_count = 0;
	Nu32 loops = 0;
	

	_ftell = ftell(pfile);  
	if (fread(&loops, sizeof(Nu32), 1, pfile) != 1)
	{
		// TODO: desallocation memoire SERIALIZER !!!
		return 0;
	}
	fprintf(plog, "[%d] read LOOPS[size=%d] ( Loops = %d )\n", _ftell, sizeof(Nu32), loops);
	// chaque loop correspond à "un tour complet" auprès de chaque serializable ou on va à chaque fois lire les données à rattacher à un certain nombre de pointeurs de l'array de pointeurs
	// associé au serializable. Ce nombre peut - être null , notament dans le cas de plusieurs Loops.
	// Exemple/explication:
	// Rappel:	Plusieurs loops, signifie que l'écriture d'un element d'un serializable 'n' à engendré l'inscription d'un element à enregistrer dans un serializable 'n - k ' !!!
	// Il a donc fallu redemandé "un tour" pour enregistrer les données associées à ce pointeur.
	// Lors de ce tour suplémentaire, tous les serializables n'avaient pas forcement de nouveaux pointeurs référencé ! Ils n'avaient donc rien de plus à enregistrer.
	// A chaque "loop" :  
	//						on passe auprès de chaque serializable
	//						on ecrit/lit  le nombre de pointeurs à traiter
	//						on traite ce nombre de pointeurs

	// On active le flag READ_ONLY du serializer
	FLAG_ON(pdpacker->Flags, FLAG_NDATAPACKER_READING_PROCESS);

	fprintf(plog, "\nfor Loop in LOOPS(%d)\n", loops);

	for (Nu32 l = 0; l < loops; l++)
	{
		fprintf(plog, "		|\n");
		fprintf(plog, "\n		+--for DATAPACK in DATAPACKTABLELIST(%d)\n", pdpacker->DataPackTableSize);
		fprintf(plog, "			|\n");
		pdatapack = pdpacker->pDataPack;
		for (i = 0; i < pdpacker->DataPackTableSize; i++, pdatapack++)
		{
			if (pdatapack->Header.Key == NDPKEY_INVALID)
				continue;

			NErrorIf(i != pdatapack->Header.Key, NERROR_INCONSISTENT_VALUES);
			_ftell = ftell(pfile);
			if (fread(&process_count, sizeof(Nu32), 1, pfile) != 1)
			{
				// TODO: desallocation memoire SERIALIZER !!!
				return 0;
			}
			fprintf(plog, "[%d]		+--DATAPACK#%d read process_count = %d [size=%d]\n", _ftell, i, process_count, sizeof(Nu32));
			if (process_count > 1000000)
			{
				fprintf(plog, "\n[!] Process_count = %d\n", process_count);
				fprintf(plog, "\n[%d]	CLOSE LOG\n", ftell(pfile));
				fclose(plog);
			}

			// Puis on lit chaque pointeur de l'array de pointeur du serializable.
			// !ICI cela n'est pas comme à l'écriture !
			// Pas besoin de repartir à chaque fois de pFirst pour trouver le pointeur sur pointeur  suivant ...
			// ... il n'y aura pas de reallocations au sein de "m_arrayOfPtr" tout les pointeurs on déjà été alloués !!!
			pptr = &((NBYTE**)pdatapack->ArrayOfPtr.pFirst)[pdatapack->Processed];
			for (j = 0; j < process_count; j++, pptr++)
			{
				pdatapack->SetupAndRead(*pptr, pdpacker, pfile);
				pdatapack->Processed++;
			}
		}
	}

	// On desactive le flag READ_ONLY du serializer
	FLAG_OFF(pdpacker->Flags, FLAG_NDATAPACKER_READING_PROCESS);

	fprintf(plog, "\n[%d]	CLOSE LOG\n",ftell(pfile));
	fclose(plog);

	return 1;
}

Nu32 NWriteDataPacker(NDATAPACKER* pdpacker, FILE* pfile)
{

	FILE *plog = fopen("__NWriteDataPacker_log.txt", "w");
	fprintf(plog, "\n[%d]	OPEN LOG\n", ftell(pfile));

	// chech for an already read or write process activation ...
	NErrorIf(ISFLAG_ON(pdpacker->Flags, (FLAG_NDATAPACKER_READING_PROCESS | FLAG_NDATAPACKER_WRITING_PROCESS)), NERROR_UNAUTHORIZED_ACTION);
	Nu32		_ftell;
	Nu32	i,j,k;
	Nu32	what;
	Nu32	_u32;
	NBYTE** pptr;
	NBYTE* ptr;

	NDATAPACKERHEADER		datapacker_header;
	NDATAPACKNODE			*pdatapacknode;
	NDATAPACKNODEHEADER		datapacknode_header;
	// 1) Initialization : version and header
	fprintf(plog, "\n+-------------------------------------------------------------------------------------------------------------------------\n");
	fprintf(plog, "|\n");
	fprintf(plog, "|	1) Initialization : version and header\n");
	fprintf(plog, "|\n");
	fprintf(plog, "+-------------------------------------------------------------------------------------------------------------------------\n");

	if (!pdpacker->DataPackTableSize)
	{
		NErrorIf(1, NERROR_NULL_VALUE);
		return 0;
	}


	_u32 = VERSION_NDATAPACKER_HEADER;

	fprintf(plog, "[%d] write VERSION_NDATAPACKER_HEADER[size=%d]\n", ftell(pfile), sizeof(Nu32));
	if (fwrite(&_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;
	datapacker_header.DataPackTableSize			= pdpacker->DataPackTableSize;
	datapacker_header.DataPackTableValidEntry	= pdpacker->DataPackTableValidEntry;

	fprintf(plog, "[%d] write NDATAPACKER_HEADER[size=%d]\n", ftell(pfile), sizeof(NDATAPACKERHEADER));
	if (fwrite(&datapacker_header, sizeof(NDATAPACKERHEADER), 1, pfile) != 1)
		return 0;

	// 2) Initialization: Data type used are stored here to check consistency against read process configuration 
	fprintf(plog, "\n+-------------------------------------------------------------------------------------------------------------------------\n");
	fprintf(plog, "|\n");
	fprintf(plog, "|	2) Initialization: Data type used are stored here to check consistency against read process configuration\n");
	fprintf(plog, "|\n");
	fprintf(plog, "+-------------------------------------------------------------------------------------------------------------------------\n");

	fprintf(plog, "\nfor DATAPACK in DATAPACKTABLELIST(%d)\n", pdpacker->DataPackTableSize);
	fprintf(plog, "	|\n");
	NDATAPACK* pdatapack = pdpacker->pDataPack;
	for (i = 0; i < pdpacker->DataPackTableSize; i++, pdatapack++)
	{
		if (pdatapack->Header.Key == NDPKEY_INVALID)
			continue;

		NErrorIf(i != pdatapack->Header.Key, NERROR_SYSTEM_GURU_MEDITATION);
		fprintf(plog, "[%d]	+--write NDATAPACK#%d_HEADER[size=%d]\n", ftell(pfile),i, sizeof(Nu32));
		if (fwrite(&pdatapack->Header.Key, sizeof(Nu32), 1, pfile) != 1)
			return 0;
	}

	// 3) all DataPacks WRITE initializations
	fprintf(plog, "\n+-------------------------------------------------------------------------------------------------------------------------\n");
	fprintf(plog, "|\n");
	fprintf(plog, "|	3) all DataPacks WRITE initializations\n");
	fprintf(plog, "|\n");
	fprintf(plog, "+-------------------------------------------------------------------------------------------------------------------------\n");
	//
	fprintf(plog, "\nfor DATAPACK in DATAPACKTABLELIST(%d)\n", pdpacker->DataPackTableSize);
	pdatapack = pdpacker->pDataPack;
	for (i = 0; i < pdpacker->DataPackTableSize; i++, pdatapack++)
	{
		if (pdatapack->Header.Key == NDPKEY_INVALID)
			continue;

		// Reset the process pointer Array ( written/read )
		fprintf(plog, "		|\n");
		fprintf(plog, "		+--DATAPACK#%d reset ArrayOfPtr size to ZERO\n",i);
		NEraseArray(&pdatapack->ArrayOfPtr, NULL);
		pdatapack->Processed = 0;

		// Then, parse all the DataPackNode and insert all their pointers according to their specificities
		if (pdatapack->ArrayOfDataPackNodes.Size)
		{
			fprintf(plog, "		+--DATAPACK#%d for DATAPACKNODE in DATAPACKNODESLIST(%d)\n", i,pdatapack->ArrayOfDataPackNodes.Size);
			fprintf(plog, "			|\n");
		}
		pdatapacknode = (NDATAPACKNODE*)pdatapack->ArrayOfDataPackNodes.pFirst;
		for (j = 0; j < pdatapack->ArrayOfDataPackNodes.Size; j++, pdatapacknode++)
		{
			what = NGET_DPCFG_WHAT(pdatapacknode->Config);
			
			// A simpgle pointer ? 
			if (what == NDPCFG_LONELY_PTR)
			{
				fprintf(plog, "			+-- DATAPACKNODE#%d Pushback LONELY_PTR (0x%p)\n", j, pdatapacknode->pAdress);
				NArrayPushBack(&pdatapack->ArrayOfPtr, (NBYTE*)&pdatapacknode->pAdress);
			}
			// or an array/membloc of ...
			else
			{
				if (what == NDPCFG_ARRAY)
				{
					if (pdatapacknode->ObjectsCount)
					{
						fprintf(plog, "			+-- DATAPACKNODE#%d Pushback ARRAY (0x%p)\n", j, pdatapacknode->pAdress);
						fprintf(plog, "			    for OBJECT in  DATAPACKNODE#%d->ARRAY (%d)\n", j,pdatapacknode->ObjectsCount);
						fprintf(plog, "					|\n");
					}
					ptr = ((NARRAY*)(pdatapacknode->pAdress))->pFirst;
				}
				else
				{
					if (pdatapacknode->ObjectsCount)
					{
						fprintf(plog, "			+-- DATAPACKNODE#%d Pushback MEMBLOC (0x%p)\n", j, pdatapacknode->pAdress);
						fprintf(plog, "			    for OBJECT in  DATAPACKNODE#%d->MEMBLOC (%d)\n", j, pdatapacknode->ObjectsCount);
						fprintf(plog, "					|\n");
					}
					NErrorIf(what != NDPCFG_MEMBLOC, NERROR_SYSTEM_GURU_MEDITATION); // !?!? This value was tested and validated in declaration process ... so ?
					ptr = (NBYTE*)pdatapacknode->pAdress;
				}

				if (NIS_DPCFG_OF_OBJECTS(pdatapacknode->Config))
				{
					NErrorIf((what == NDPCFG_ARRAY) && (pdatapack->Header.ElementSize != ((NARRAY*)(pdatapacknode->pAdress))->ElementSize), NERROR_SYSTEM_GURU_MEDITATION); // !?!? This value was tested and validated in declaration process ... so ?
					for (k = 0; k < pdatapacknode->ObjectsCount; k++, ptr += pdatapack->Header.ElementSize)
					{
						fprintf(plog, "					+-- pushback PTR on ARRAY OBJECT#%d (0x%p)\n",k,ptr);
						NArrayPushBack(&pdatapack->ArrayOfPtr, (NBYTE*)&ptr);
					}
				}
				else // NIS_DPCFG_OF_PTRS(psn->Config)
				{
					NErrorIf((what == NDPCFG_ARRAY) && (sizeof(void*) != ((NARRAY*)(pdatapacknode->pAdress))->ElementSize), NERROR_SYSTEM_GURU_MEDITATION); // !?!? This value was tested and validated in declaration process ... so ?
					pptr = (NBYTE**)ptr;
					for (k = 0; k < pdatapacknode->ObjectsCount; k++, pptr++)
					{
						fprintf(plog, "					+-- pushback ARRAY PTR#%d (0x%p)\n", k, *pptr);
						NArrayPushBack(&pdatapack->ArrayOfPtr, (NBYTE*)pptr);
					}
				}
			}
		}

		// Keep in mind the file position of this serial header for in process updates 
		pdatapack->HeaderPosition = ftell(pfile);
	
		// First write of Serial Header and of all its serial nodes.
		NErrorIf(pdatapack->Header.Key != i, NERROR_INCONSISTENT_VALUES);	// !?!? This value was tested and validated in Serial Creation process ... so ?
		NErrorIf(!pdatapack->Header.ElementSize, NERROR_NULL_VALUE);		// !?!? This value was tested and validated in Serial Creation process ... so ?
		pdatapack->Header.ArrayOfPtrSize			= pdatapack->ArrayOfPtr.Size;
		pdatapack->Header.ArrayOfDataPackNodesSize	= pdatapack->ArrayOfDataPackNodes.Size;
		
		// ... and Write Header:
		fprintf(plog, "[%d]		+--DATAPACK#%d write NDATAPACK#%dHEADER[size=%d]\n", ftell(pfile),i,i, sizeof(NDATAPACKHEADER));
		if (fwrite(&pdatapack->Header, sizeof(NDATAPACKHEADER), 1, pfile) != 1)
			return 0;

		// ... +  All its DataPackNodes description
		if (pdatapack->ArrayOfDataPackNodes.Size)
		{
			fprintf(plog, "		+--DATAPACK#%d for NDATAPACKNODEHEADER in  DPNODEARRAY#%d (%d)\n", i, i, pdatapack->ArrayOfDataPackNodes.Size);
			fprintf(plog, "					|\n");
		}
		pdatapacknode = (NDATAPACKNODE*)pdatapack->ArrayOfDataPackNodes.pFirst;
		for (j = 0; j < pdatapack->ArrayOfDataPackNodes.Size; j++, pdatapacknode++)
		{
			// For each node:
			// a) the node description ( config + objects count )
			datapacknode_header.Config			= pdatapacknode->Config;
			datapacknode_header.ObjectsCount	= pdatapacknode->ObjectsCount;
			fprintf(plog, "[%d]					+--write NDATAPACKNODEHEADER#%d[size=%d]\n", ftell(pfile), j, sizeof(NDATAPACKNODEHEADER));
			if (fwrite(&datapacknode_header, sizeof(NDATAPACKNODEHEADER), 1, pfile) != 1)
				return 0;
		}
	}
	fprintf(plog, "\n");

	// 4) Writting LOOPS:
	fprintf(plog, "\n+-------------------------------------------------------------------------------------------------------------------------\n");
	fprintf(plog, "|\n");
	fprintf(plog, "|	4) Writting LOOPS:\n");
	fprintf(plog, "|\n");
	fprintf(plog, "+-------------------------------------------------------------------------------------------------------------------------\n");

	Nu32 process_count = 0;
	Nu32 loops = 0;
	Nu32 loops_pos = ftell(pfile);
	fprintf(plog, "[%d] write LOOP=%d[%d]\n", ftell(pfile), loops,sizeof(Nu32));
	if (fwrite(&loops, sizeof(Nu32), 1, pfile) != 1)
		return 0;

	// WRITE_ONLY Data Packer flag activated
	fprintf(plog, "\n ____ start :: do/while\n");
	fprintf(plog, "|\n");
	fprintf(plog, "|__________\n");

	FLAG_ON(pdpacker->Flags, FLAG_NDATAPACKER_WRITING_PROCESS);
	do
	{
		FLAG_OFF(pdpacker->Flags, FLAG_NDATAPACKER_WRITING_PROCESS_AGAIN);
		loops++;
		fprintf(plog, "Loop#%d\n",loops);
		fprintf(plog, "\nfor DATAPACK in DATAPACKTABLELIST(%d)\n", pdpacker->DataPackTableSize);
		fprintf(plog, "	|\n");

		pdatapack = pdpacker->pDataPack;
		for (i = 0; i < pdpacker->DataPackTableSize; i++, pdatapack++)
		{
			if (pdatapack->Header.Key == NDPKEY_INVALID)
				continue;

			// keep in mind the current datapack, to check against it inside the  user 'pdatapack->Write' with its potential calls of 'NGetDataPackerIndex'
			pdpacker->CurrentWrittenDataPack = i;

			// Number of pointers that will be processed by NDATAPACK in this loop
			NErrorIf(pdatapack->Processed > pdatapack->ArrayOfPtr.Size, NERROR_INCONSISTENT_VALUES);
			process_count = pdatapack->ArrayOfPtr.Size - pdatapack->Processed;
			fprintf(plog, "[%d]	+--DATAPACK#%d write process_count = %d [size=%d]\n", ftell(pfile), i, process_count, sizeof(Nu32));
			if (fwrite(&process_count, sizeof(Nu32), 1, pfile) != 1)
			{
				// TODO: desallocation memoire SERIALIZER !!!
				return 0;
			}

			if (process_count)
			{
				fprintf(plog, "	+--for Ptr in ArrayOfPtr(%d)\n", process_count);
				fprintf(plog, "		|\n");
			}
			ptr = ((NBYTE**)pdatapack->ArrayOfPtr.pFirst)[pdatapack->Processed];
			for (j = 0; j < process_count; j++, ptr = ((NBYTE**)pdatapack->ArrayOfPtr.pFirst)[j + pdatapack->Processed])
			{	
				_ftell = ftell(pfile);
				pdatapack->Write(ptr, pdpacker, pfile);
				fprintf(plog, "[%d]		+-- CallBack Write function call for Ptr#%d(%p)[size=%d]\n", _ftell, j,ptr, ftell(pfile)-_ftell);
			}
			pdatapack->Processed+= process_count;
		}
	} while (ISFLAG_ON(pdpacker->Flags, FLAG_NDATAPACKER_WRITING_PROCESS_AGAIN));
	fprintf(plog, " __________\n");
	fprintf(plog, "|\n");
	fprintf(plog, "|____ End :: do/while\n\n");
	// WRITE_ONLY serializer flag deactivated
	FLAG_OFF(pdpacker->Flags, FLAG_NDATAPACKER_WRITING_PROCESS_AGAIN);

	// keep this file position:
	Nu32 end_pos = ftell(pfile);

	// ... and jump to the loops file location to write the loop number into
	fprintf(plog, "[%d] fseek to LOOP pos [pos=%d]\n", ftell(pfile), loops_pos);
	fseek(pfile, loops_pos, SEEK_SET);
	fprintf(plog, "[%d] re-write LOOP=%d[size=%d]\n", ftell(pfile), loops,sizeof(Nu32));
	if (fwrite(&loops, sizeof(Nu32), 1, pfile) != 1)
		return 0;

	// 5) And, at the whole end ...
	fprintf(plog, "\n+-------------------------------------------------------------------------------------------------------------------------\n");
	fprintf(plog, "|\n");
	fprintf(plog, "|	5) And, at the whole end ...\n");
	fprintf(plog, "|\n");
	fprintf(plog, "+-------------------------------------------------------------------------------------------------------------------------\n");

	fprintf(plog, "\nfor DATAPACK in DATAPACKTABLELIST(%d)\n", pdpacker->DataPackTableSize);
	pdatapack = pdpacker->pDataPack;
	for (i = 0; i < pdpacker->DataPackTableSize; i++, pdatapack++)
	{
		if (pdatapack->Header.Key == NDPKEY_INVALID)
			continue;

		// Some last checks done to detect bugs and other strange things ...
		NErrorIf(pdatapack->ArrayOfPtr.Size != pdatapack->Processed, NERROR_INCONSISTENT_VALUES);
		pdatapack->Header.ArrayOfPtrSize = pdatapack->ArrayOfPtr.Size;

		// and rewrite:
		fprintf(plog, "		|\n");
		fprintf(plog, "		+--DATAPACK#%d fseek to DATAPACKHEADER position [pos=%d]\n", i, pdatapack->HeaderPosition);
		fseek(pfile, pdatapack->HeaderPosition, SEEK_SET);
		fprintf(plog, "[%d]		+--DATAPACK#%d re-write DATAPACKHEADER [size=%d]\n", ftell(pfile), i, sizeof(NDATAPACKHEADER));
		fprintf(plog, "		+--DATAPACK#%d ( update ArrayOfPtrSize = %d) \n", i, pdatapack->Header.ArrayOfPtrSize);
		if (fwrite(&pdatapack->Header, sizeof(NDATAPACKHEADER), 1, pfile) != 1)
			return 0;
	}
	fseek(pfile, end_pos, SEEK_SET);

	fprintf(plog, "\n[%d]	CLOSE LOG\n", end_pos);
	fclose(plog);

	return 1;
}

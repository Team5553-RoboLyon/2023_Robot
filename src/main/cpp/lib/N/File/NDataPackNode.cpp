#include "lib/N/NMemory.h"
#include "lib/N/NErrorHandling.h"
#include "lib/N/Core/NSafeConversion.h"
/*
#include "lib/N/NCore.h"
#include "lib/N/GL/Ngl.h"
#include "lib/N/NType.h"
#include "lib/N/File/NFile.h"
#include "lib/N/File/NFileExt.h"
#include "lib/N/NString.h"
*/
#include "NDataPackNode.h"

/**********************************************************************************************************************************************************************
 *
 *																	NDATAPACKNODE
 *
 **********************************************************************************************************************************************************************
 */
NDATAPACKNODE *NSetupDataPackNode(NDATAPACKNODE *ppdn, const Nu32 cfg, const void *padress, const Nu32 membloc_object_count)
{
// **************************************************************************************************************************************************
// **************************************************************************************************************************************************
// DEBUG CHECKING : START
#ifdef _DEBUG
	NErrorIf(!ppdn, NERROR_NULL_POINTER);
	NErrorIf(!padress, NERROR_NULL_POINTER);
	NErrorIf(!cfg, NERROR_NULL_VALUE);

	NErrorIf(NIS_DPCFG_ARRAY(cfg) && ((NARRAY *)padress)->Size != membloc_object_count, NERROR_INCONSISTENT_VALUES);
	/*
	// IS WHAT Consistency Checks :
	// ----------------------------------------
	// 0) IS WHAT is VOID !
	//    -----------------
	if (NGET_SERIALCFG_WHAT(cfg) == NSRLCFG_VOID)
	{
		NErrorIf(1, NERROR_SYSTEM_GURU_MEDITATION);
	}
	// 1) IS WHAT is ARRAY or MEMBLOC !
	//    -----------------------------
	else if (NGET_SERIALCFG_WHAT(cfg) == NSRLCFG_ARRAY || NGET_SERIALCFG_WHAT(cfg) == NSRLCFG_MEMBLOC)
	{
		// specific checks for Array:
		NErrorIf((NGET_SERIALCFG_WHAT(cfg) == NSRLCFG_ARRAY) && (membloc_object_count != 0) && (membloc_object_count != ((NARRAY*)padress)->Size), NERROR_INCONSISTENT_PARAMETERS); // MUST BE = 0 or = array size. ( both are ok )
		NErrorIf((NGET_SERIALCFG_WHAT(cfg) == NSRLCFG_ARRAY) && NIS_SERIALCFG_OF_PTRS(cfg) && ((NARRAY*)padress)->ElementSize != sizeof(void*), NERROR_INCONSISTENT_PARAMETERS); // NARRAY.ElementSize must be = sizeof(void*) when elements are pointers !!!
		// specific checks for MemBlocs:
		NErrorIf((NGET_SERIALCFG_WHAT(cfg) == NSRLCFG_MEMBLOC) && (membloc_object_count != 0), NERROR_INCONSISTENT_PARAMETERS); // MUST BE > 0 !!!

		// 1.1) ARRAY/MEMBLOC MALLOC BY ( MEMORY MANAGEMENT ) !
		//		-----------------------------------------------
		// !!! An Array/Membloc must be managed ( Memory allocation ) by USER or SERIALIZER !
		NErrorIf(NGET_SERIALCFG_ARRAY_MEM_MANAGER(cfg) == NSRLCFG_VOID, NERROR_NULL_VALUE);

		// If Array/Membloc Memory allocation is managed by User or by SERIALIZER ...
		if (NGET_SERIALCFG_ARRAY_MEM_MANAGER(cfg) == NSRLCFG_SERIALIZER || NGET_SERIALCFG_ARRAY_MEM_MANAGER(cfg) == NSRLCFG_USER)
		{
			// check for Objects/Pointers Memory Allocation management:
			switch (NGET_SERIALCFG_OBJECT_MEM_MANAGER(cfg))
			{
			case NSRLCFG_VOID:
				NErrorIf(1, NERROR_NULL_VALUE); // !!! Objects/Ptrs must be managed ( Memory allocation ) by USER or SERIALIZER !
				break;
			case NSRLCFG_SERIALIZER:
				NErrorIf(pNu32bitfield, NERROR_NON_NULL_POINTER);	// !!! A Batch description is NOT needed !
				break;
			case NSRLCFG_USER:
				NErrorIf(pNu32bitfield, NERROR_NON_NULL_POINTER);	// !!! A Batch description is NOT needed !
				break;
			case NSRLCFG_BATCH:
				NErrorIf(!pNu32bitfield, NERROR_NULL_POINTER);	// !!! A Batch description is needed !
																// That means a BitField with a size of membloc_object_count bits describing ...
																// Which object is managed by user (bit(object_index) = NFALSE)
																// ... and which one is managed by serializer  (bit(object_index) = NTRUE)
				break;
			default:
				NErrorIf(1, NERROR_SYSTEM_GURU_MEDITATION);
				break;
			}
		}
		else // NGET_SERIALCFG_ARRAY_MEM_MANAGER(cfg) != NSRLCFG_VOID, != NSRLCFG_SERIALIZER, != NSRLCFG_USER, != NSRLCFG_BATCH ... !!!! WEIRD !!!!
		{
			NErrorIf(1, NERROR_SYSTEM_GURU_MEDITATION);
		}
	}
	// 2) IS WHAT is LONELY POINTER !
	//    ----------------------------
	else if (NGET_SERIALCFG_WHAT(cfg) == NSRLCFG_LONELY_PTR)
	{
		// NIS_SERIALCFG_OF_OBJECTS(cfg) or NIS_SERIALCFG_OF_PTRS(cfg) are both accepted, but there is no difference between the 2 for SERIALIZER.
		// ... using one or the other doesn't make any difference. So just put '0' instead of one of this 2 options to "mark" that: "it's not used !"
		NErrorIf(membloc_object_count, NERROR_NON_NULL_VALUE);
		NErrorIf(pNu32bitfield, NERROR_NON_NULL_POINTER);
		NErrorIf(NGET_SERIALCFG_ARRAY_MEM_MANAGER(cfg) != NSRLCFG_VOID, NERROR_INCONSISTENT_PARAMETERS);	// There is NO ARRAY to manage for a lonely Ptr ! MUST BE 'NSRLCFG_VOID'
		NErrorIf(NGET_SERIALCFG_OBJECT_MEM_MANAGER(cfg) == NSRLCFG_VOID, NERROR_INCONSISTENT_PARAMETERS);	// A Lonely Pointer may be allocated by user or by Serializer. MUST BE 'NSRLCFG_SERIALIZER' or 'NSRLCFG_USER'
		NErrorIf(NGET_SERIALCFG_OBJECT_MEM_MANAGER(cfg) == NSRLCFG_BATCH, NERROR_INCONSISTENT_PARAMETERS);	// A Lonely Pointer doesn't need a batch ! MUST BE 'NSRLCFG_SERIALIZER' or 'NSRLCFG_USER'
	}
	// 3) IS WHAT is SOMETHING ELSE ??? !
	//    -------------------------------
	else
	{
		NErrorIf(1, NERROR_SYSTEM_GURU_MEDITATION);
	}
	*/
#endif

	// DEBUG CHECKING : END
	// **************************************************************************************************************************************************
	// **************************************************************************************************************************************************

	// 1) CONFIG:
	ppdn->Config = cfg;

	// 2) POINTER:
	ppdn->pAdress = (void *)padress;
	ppdn->ObjectsCount = membloc_object_count;
	return ppdn;
}

NDATAPACKNODE *NCreateDataPackNode(const Nu32 cfg, const void *parray_or_membloc, const Nu32 membloc_object_count)
{
	return NSetupDataPackNode(NEW(NDATAPACKNODE), cfg, parray_or_membloc, membloc_object_count);
}

void NClearDataPackNode(NDATAPACKNODE *pdpn)
{
	// !!! REALLY IMPORTANT HERE !!!
	//
	//								The clear process doesn't clear/free the "pdpn->pAdress"
	//
	// So pdpn->pAdress fall in the user responsibility after Reading process !!!!
	//
	Nmem0(pdpn, NDATAPACKNODE);
}

void NDeleteDataPackNode(NDATAPACKNODE *pdpn)
{
	NClearDataPackNode(pdpn);
	Nfree(pdpn);
}

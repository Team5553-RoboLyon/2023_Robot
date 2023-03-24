#include "../NCStandard.h"
#include "../NMemory.h"
#include "../NMath.h"
#include "../NErrorHandling.h"
//#include "../Core/NSafeConversion.h"

#include "NDataPackNode.h"
#include "NDataPack.h"

/**********************************************************************************************************************************************************************
 *
 *																	NDATAPACK
 *
 **********************************************************************************************************************************************************************
*/
void _safe_clearDataPackNode(void* ptr)
{
	NClearDataPackNode((NDATAPACKNODE*)ptr);
}
NDATAPACK* NSetupDataPack(NDATAPACK* pdp, const Nu32 key, const Nu32 elementsize, NDP_WRITE write, NDP_SREAD sread)
{
	pdp->Flags										= 0;
	pdp->AvailableNu8								= 0;
	pdp->AvailableNu16								= 0;

	pdp->Header.Key									= key;
	pdp->Header.ElementSize							= elementsize;
	pdp->Header.ArrayOfPtrSize						= 0;
	pdp->Header.ArrayOfDataPackNodesSize			= 0;

	pdp->HeaderPosition								= 0;
	pdp->Processed									= 0;


	pdp->Write										= write;
	pdp->SetupAndRead								= sread;

	NSetupArray(&pdp->ArrayOfPtr, 2, sizeof(void*));
	NSetupArray(&pdp->ArrayOfDataPackNodes, 0, sizeof(NDATAPACKNODE));
	return pdp;
}

NDATAPACK* NCreateDataPack(const Nu32 key, const Nu32 elementsize, NDP_WRITE write, NDP_SREAD sread)
{
	return NSetupDataPack(NEW(NDATAPACK), key, elementsize, write, sread);
}

void	NClearDataPack(NDATAPACK* pdp)
{
	pdp->Flags										= 0;
	pdp->AvailableNu8								= 0;
	pdp->AvailableNu16								= 0;

	pdp->Header.Key									= 0;
	pdp->Header.ElementSize							= 0;
	pdp->Header.ArrayOfPtrSize						= 0;
	pdp->Header.ArrayOfDataPackNodesSize			= 0;

	pdp->HeaderPosition								= 0;
	pdp->Processed									= 0;

	pdp->Write										= NULL;
	pdp->SetupAndRead								= NULL;

	NClearArray(&pdp->ArrayOfPtr, NULL );
	NClearArray(&pdp->ArrayOfDataPackNodes, _safe_clearDataPackNode);
}
void	NDeleteDataPack(NDATAPACK* pdp)
{
	NClearDataPack(pdp);
	Nfree(pdp);
}
// ------------------------------------------------------------------------------------------
/**
 *	@brief
 *
 *
 *	@param
 *	@param
 *	@param
 *
 */
 // ------------------------------------------------------------------------------------------
void NInsertDataPackNode(NDATAPACK* pdp, const Nu32  cfg, const void* padress, const Nu32 membloc_object_count)
{
	NErrorIf(!pdp, NERROR_NULL_POINTER);
	// !!! Of course it's possible to update NDATAPACK content only if is NOT LOCKED !!!
	if (ISFLAG_OFF(pdp->Flags, FLAG_NDATAPACK_LOCKED))
	{
		// In release mode 'parray' pointer is inserted without any kind of warning check.
		NDATAPACKNODE* pn = (NDATAPACKNODE*)NArrayAllocBack(&pdp->ArrayOfDataPackNodes);
		NSetupDataPackNode(pn,cfg,padress,membloc_object_count);
	}
#ifdef _DEBUG
	else 		// FLAG_NSERIAL_LOCKED is ON !!!
	{
		NErrorIf(1, NERROR_UNAUTHORIZED_ACTION);
	}
#endif
}



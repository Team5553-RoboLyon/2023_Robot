#include "../../NErrorHandling.h"
#include "NUT_Nf32Set.h"

// ------------------------------------------------------------------------------------------
// void NUT_BuildNf32SetSequence
// ------------------------------------------------------------------------------------------
// Description :
//
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_BuildNf32SetSequence(Nf32 *pdstsequence_firstnf32set, const Nu32 stridetonext, const Nu32 sequence_size, NUT_Nf32SET_BUILDER *pfsetbuilder )
{
	NErrorIf(!pdstsequence_firstnf32set||!pfsetbuilder, NERROR_NULL_POINTER);
	NErrorIf(!stridetonext, NERROR_NULL_VALUE);
	NErrorIf(sequence_size < 2 ,NERROR_NUT_Nf32SET_SEQUENCE_SIZE_TOO_SHORT);
	NErrorIf(stridetonext < pfsetbuilder->Nf32SetSize*sizeof(Nf32), NERROR_NUT_Nf32SET_SEQUENCE_STRIDE_OVERLAPING);
	
	// START
	Nu32	i;
	Nu32	sharedbuffersize = 0;
	NUT_Nf32_PROCESS	*pfproc = pfsetbuilder->pProcess;
	for(i=pfsetbuilder->ProcessListSize;i!=0;i--,pfproc++)
	{
		sharedbuffersize = NMAX( sharedbuffersize, pfproc->Core.Start(pfproc,sequence_size) );
	}
	if(sharedbuffersize)
	{
		pfsetbuilder->pSharedBuffer = (Nf32*)Nmalloc(sharedbuffersize*sizeof(Nf32));
	}
	else
	{
		pfsetbuilder->pSharedBuffer = NULL;
	}
	
	// LOOP
	pfproc = pfsetbuilder->pProcess;
	for( i=pfsetbuilder->ProcessListSize;i!=0;i--,pfproc ++)
	{
		pfproc->Core.pSharedBuffer = pfsetbuilder->pSharedBuffer;
		pfproc->Core.Update( pfproc, pdstsequence_firstnf32set, stridetonext, sequence_size );
		pfproc->Core.pSharedBuffer = NULL;
	}

	// End
	if(pfsetbuilder->pSharedBuffer)
	{
		Nfree(pfsetbuilder->pSharedBuffer);
	}
}

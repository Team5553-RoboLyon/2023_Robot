#include "../../NErrorHandling.h"
#include "NUT_Nf32Set.h"

// ------------------------------------------------------------------------------------------
// NUT_Nf32SET_BUILDER*	NUT_SetupNf32SetBuilder
// ------------------------------------------------------------------------------------------
// Description :
//	Setup a Nf32Set builder. 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
NUT_Nf32SET_BUILDER* NUT_SetupNf32SetBuilder( NUT_Nf32SET_BUILDER *pnf32setbuilder, const Nu8 fsetsize )
{
	NErrorIf(!pnf32setbuilder,NERROR_NULL_POINTER);
	NErrorIf(!fsetsize,NERROR_NULL_VALUE);
	
	Nmem0(pnf32setbuilder,NUT_Nf32SET_BUILDER);
	pnf32setbuilder->Flags				= 0;				
	pnf32setbuilder->Nf32SetSize		= fsetsize;	
	pnf32setbuilder->ProcessListCapacity= fsetsize;
	pnf32setbuilder->ProcessListSize	= 0;
	pnf32setbuilder->pProcess			= (NUT_Nf32_PROCESS*)Nmalloc(fsetsize*sizeof(NUT_Nf32_PROCESS));
	memset(pnf32setbuilder->pProcess,0,fsetsize*sizeof(NUT_Nf32_PROCESS));
	
	// Scope Stack
	pnf32setbuilder->ScopeStackSize		= fsetsize; // because at init there are fsetsize Scope in use 
	pnf32setbuilder->pScopeStack		= (NUT_Nf32_SCOPE*)Nmalloc(pnf32setbuilder->ScopeStackSize*sizeof(NUT_Nf32_SCOPE));
	
	// Default scope setup
	NUT_Nf32_SCOPE *pscope = pnf32setbuilder->pScopeStack;
	for(Nu32 i=pnf32setbuilder->ScopeStackSize;i!=0;i--,pscope++)
	{
		// Setup Scope with default 
		pscope->Factor= 1.0f;
		NUT_GetNf32OperandHandle(NUT_Nf32SET_SET, &pscope->Operand, &pscope->OperandCst);
	}
	pnf32setbuilder->pCurrentScope = pnf32setbuilder->pScopeStack;
	return pnf32setbuilder;
}

// ------------------------------------------------------------------------------------------
// NUT_Nf32SET_BUILDER*	NUT_CreateNf32SetBuilder
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
NUT_Nf32SET_BUILDER	*NUT_CreateNf32SetBuilder(const Nu8 setsize)
{
	return NUT_SetupNf32SetBuilder(NEW(NUT_Nf32SET_BUILDER),setsize);
}

// ------------------------------------------------------------------------------------------
// void NUT_ClearNf32SetBuilder
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
void NUT_ClearNf32SetBuilder(NUT_Nf32SET_BUILDER *pnf32setbuilder)
{
	NUT_Nf32_PROCESS *pfproc = pnf32setbuilder->pProcess;
	for(Nu8 i=pnf32setbuilder->ProcessListSize;i!=0;i--,pfproc++)
	{
		NUT_ClearNf32Process(pfproc);
	}
	Nfree(pnf32setbuilder->pProcess);
	Nfree(pnf32setbuilder->pScopeStack);
	Nmem0(pnf32setbuilder,NUT_Nf32SET_BUILDER);
}

// ------------------------------------------------------------------------------------------
// void NUT_DeleteNf32SetBuilder
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
void NUT_DeleteNf32SetBuilder(NUT_Nf32SET_BUILDER *pnf32setbuilder)
{
	NUT_ClearNf32SetBuilder(pnf32setbuilder);
	Nfree(pnf32setbuilder);
}

// ------------------------------------------------------------------------------------------
// NUT_Nf32SET_BUILDER* NUT_ResetNf32SetBuilder
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
NUT_Nf32SET_BUILDER* NUT_ResetNf32SetBuilder( NUT_Nf32SET_BUILDER *pnf32setbuilder, const Nu8 fsetsize )
{
	NErrorIf(!fsetsize,NERROR_NULL_VALUE);
	NErrorIf(!pnf32setbuilder,NERROR_NULL_POINTER);
	NErrorIf(!pnf32setbuilder->ProcessListCapacity,NERROR_NULL_VALUE);
	NErrorIf(!pnf32setbuilder->pProcess,NERROR_NULL_POINTER);
	NErrorIf(!pnf32setbuilder->pScopeStack,NERROR_NULL_POINTER);

	NUT_Nf32_PROCESS *pfproc = pnf32setbuilder->pProcess;
	for(Nu8 i=pnf32setbuilder->ProcessListSize;i!=0;i--,pfproc++)
	{
		NUT_ClearNf32Process(pfproc);
	}
	pnf32setbuilder->ProcessListSize	= 0;
	
	pnf32setbuilder->Flags			= 0;				
	pnf32setbuilder->Nf32SetSize	= fsetsize;	
	
	// Reset scope stack size to its initial value (=Nf32SetSize) 
	pnf32setbuilder->ScopeStackSize = fsetsize;
	pnf32setbuilder->pScopeStack	= (NUT_Nf32_SCOPE*)Nrealloc(pnf32setbuilder->pScopeStack,pnf32setbuilder->ScopeStackSize*sizeof(NUT_Nf32_SCOPE));
	
	// Default scope setup
	NUT_Nf32_SCOPE *pscope = pnf32setbuilder->pScopeStack;
	for(Nu32 j=pnf32setbuilder->ScopeStackSize;j!=0;j--,pscope++)
	{
		// Setup Scope with default 
		pscope->Factor= 1.0f;
		NUT_GetNf32OperandHandle(NUT_Nf32SET_SET, &pscope->Operand, &pscope->OperandCst);
	}
	pnf32setbuilder->pCurrentScope = pnf32setbuilder->pScopeStack;

	//pnf32setbuilder->Nf32ProcessListCapacity still the same;

	return pnf32setbuilder;
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +
// + LOWLEVEL
// +
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ------------------------------------------------------------------------------------------
// NUT_Nf32_BUILDER* NUT_GetNf32Builder
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
//			NUT_Nf32SET_BUILDER	*pfsetbldr			
//								A valid pointer on a Nf32SET_BUILDER (Simple or composite.)
//
//			const Nbool			continuous_spread	 
//								NTRUE or NFALSE. Specify how the Nf32_BUILDER is going to behave
//								if it is used by more than one FSET member.
//								By default (NFALSE, understand NO continuous spread), if the same Nf32_BUILDER is linked to "n FSET.members",
//								these "n members" will be fill with the same Generated value ( with one Update call, thanks to the 'Cache').
//								NTRUE, specify "continuous spread", that means, in the same situation, the "n FSET.members" will be fill
//								with the different successive values, from n successive update calls ( 'Cache' is deactivated ).
//								
// Out :
//
//
// ------------------------------------------------------------------------------------------
NUT_Nf32_PROCESS* NUT_GetNf32Process(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nbool usescope)
{
	NErrorIf( targetid1 < targetid0, NERROR_NUT_Nf32SET_PROCESS_ID_OUTOFRANGE );
	
	if(pfsetbldr->ProcessListSize == pfsetbldr->ProcessListCapacity)
	{
		pfsetbldr->ProcessListCapacity += pfsetbldr->Nf32SetSize;
		pfsetbldr->pProcess = (NUT_Nf32_PROCESS*)Nrealloc(pfsetbldr->pProcess, pfsetbldr->ProcessListCapacity*sizeof(NUT_Nf32_PROCESS));
		memset(pfsetbldr->pProcess+pfsetbldr->ProcessListSize,0,pfsetbldr->Nf32SetSize);
	}

	// Generic Setup
	NUT_Nf32_PROCESS	*pfproc = &pfsetbldr->pProcess[pfsetbldr->ProcessListSize];
	Nmem0(pfproc,NUT_Nf32_PROCESS);
	pfproc->Core.TargetId0		= targetid0;
	pfproc->Core.TargetId1		= targetid1;
	pfproc->Core.Nf32SetSize	= pfsetbldr->Nf32SetSize;
	if(usescope)
	{
		pfproc->Core.pScope		= pfsetbldr->pCurrentScope;
		pfsetbldr->pCurrentScope= pfsetbldr->pScopeStack;
		
		// Scope Constants quick analyzing ...
		// By default (without any update )all scope factors are set to 1.0f.
		// That means they will have no effect ( remember... a x 1 = a ).
		// In a meaning of optimization, those 2 cases ( factors == 1 or factors != 1 ) are managed in 2 different ways.
		// So, if factors are updated, we assume without any further tests that at least one factor is different than 1 
		if(ISFLAG_ON(pfsetbldr->Flags, FLAG_NUT_Nf32SET_BUILDER_SCOPE_FACTOR_UPDATED))
		{
			FLAG_ON(pfproc->Core.Flags,FLAG_NUT_Nf32_PROCESS_USE_FACTOR);
			FLAG_OFF(pfsetbldr->Flags, FLAG_NUT_Nf32SET_BUILDER_SCOPE_FACTOR_UPDATED);
		}
	}
// Useless to precise 'else' due to previous Nmem0
// 	else
// 	{
// 		pfproc->Core.pScope	 = NULL;
// 	}
	
	
	pfsetbldr->ProcessListSize ++;

// The followings are going to stay full of ZERO here, because that's purpose of the parent function to define them.
// (parent function = the function which calls this one )
// 			Nu8							Flags;			
// 			NUT_Nf32_PROCESS_BUFSIZE	Start;
// 			NUT_Nf32_PROCESS_UPDATE		Update;
// 			NUT_Nf32_PROCESS_CLEAR		Clear;
// 			Nf32						*pSharedBuffer;
	
	return pfproc;
}

#ifdef _DEBUG
void _DEBUG_fPrintfNf32SetFCT(const Nchar *filename,const Nf32 *pfirstnf32set, const Nu8 nf32setsize,const Nu32 stridetonext, const Nu32 sequence_size )
{
	FILE *pfile = fopen(filename,"w");
	Nf32 *pf = (Nf32*)pfirstnf32set;
	Nu32 i;
	Nu8  j;
	for( i=0;i<sequence_size;i++,pf = (Nf32*)((NBYTE*)pf + stridetonext) )
	{
		fprintf(pfile,"\n");
		for(j=0;j<nf32setsize;j++)
		{
			fprintf(pfile,"%.6f	",pf[j]);
		}
	}
	fclose (pfile);
}

#endif
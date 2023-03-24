#include "../../NErrorHandling.h"
#include "../../Core/NSafeConversion.h"
#include "NUT_Nf32Set.h"



inline NUT_Nf32_SCOPE* _increase_scopestack_size(NUT_Nf32SET_BUILDER *pfsetbldr)
{
	// Before Memory Reallocation
	Nu8 i;
	NUT_Nf32_SCOPE		*pscope,*pscopeb;
	NUT_Nf32_PROCESS	*pfproc = pfsetbldr->pProcess;
	for(i=pfsetbldr->ProcessListSize;i!=0;i--,pfproc++)
	{
		pfproc->Core.pScope = (NUT_Nf32_SCOPE*)(pfproc->Core.pScope - pfsetbldr->pScopeStack); // transform pointer to ID.
	}

	// Increase stack size
	pfsetbldr->ScopeStackSize += pfsetbldr->Nf32SetSize;
	pfsetbldr->pScopeStack = (NUT_Nf32_SCOPE*)Nrealloc(pfsetbldr->pScopeStack,pfsetbldr->ScopeStackSize*sizeof(NUT_Nf32_SCOPE));
	pscope = pscopeb = &pfsetbldr->pScopeStack[pfsetbldr->ScopeStackSize-pfsetbldr->Nf32SetSize];
	
	// Default scope setup
	for(i=pfsetbldr->Nf32SetSize;i!=0;i--,pscope ++)
	{
		// Setup Scope with default 
		pscope->Factor= 1.0f;
		NUT_GetNf32OperandHandle(NUT_Nf32SET_SET, &pscope->Operand, &pscope->OperandCst);
	}

	// After Memory Reallocation
	pfproc = pfsetbldr->pProcess;
	for(i=pfsetbldr->ProcessListSize;i!=0;i--,pfproc++)
	{
		pfproc->Core.pScope = pfsetbldr->pScopeStack + ((Nu32)pfproc->Core.pScope); // transform ID to Pointer.
	}
	return pscopeb;
}
// ------------------------------------------------------------------------------------------
// void NUT_BindNf32Builder_Operand
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
void NUT_BindNf32_Operand(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr,  const NUT_Nf32SET_OPERAND_ENUM operand  )
{
	NErrorIf( targetid1 < targetid0, NERROR_NUT_Nf32SET_PROCESS_ID_OUTOFRANGE );
	NErrorIf( targetid1 >= pfsetbldr->Nf32SetSize, NERROR_NUT_Nf32SET_PROCESS_ID_OUTOFRANGE );
	NErrorIf(operand >= NUT_Nf32SET_OPERAND_ENUM_SIZE, NERROR_NUT_Nf32SET_BUILDER_OPERAND_OUTOFRANGE);
	
	// Create a new range of scope ?
	if( pfsetbldr->pCurrentScope == pfsetbldr->pScopeStack )
	{
		pfsetbldr->pCurrentScope = _increase_scopestack_size(pfsetbldr);
	}
	
	// Store Incoming Operand
	for(Nu8 j=targetid0;j<=targetid1;j++)
	{
		NUT_GetNf32OperandHandle(operand, &pfsetbldr->pCurrentScope[j].Operand, &pfsetbldr->pCurrentScope[j].OperandCst);
	}
}

void NUT_BindNf32_OperandTuple(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr,  const NUT_Nf32SET_OPERAND_ENUM *poperand_tuple  )
{
	NErrorIf( targetid1 < targetid0, NERROR_NUT_Nf32SET_PROCESS_ID_OUTOFRANGE );
	NErrorIf( targetid1 >= pfsetbldr->Nf32SetSize, NERROR_NUT_Nf32SET_PROCESS_ID_OUTOFRANGE );
	
	// Create a new range of scope ?
	if( pfsetbldr->pCurrentScope == pfsetbldr->pScopeStack )
	{
		pfsetbldr->pCurrentScope = _increase_scopestack_size(pfsetbldr);
	}

	// Store Incoming Operand
	NUT_Nf32SET_OPERAND_ENUM *pop	= (NUT_Nf32SET_OPERAND_ENUM*)poperand_tuple;
	for(Nu8 j=targetid0;j<=targetid1;j++,pop++)
	{
		NErrorIf(*pop >= NUT_Nf32SET_OPERAND_ENUM_SIZE, NERROR_NUT_Nf32SET_BUILDER_OPERAND_OUTOFRANGE);
		NUT_GetNf32OperandHandle(*pop, &pfsetbldr->pCurrentScope[j].Operand, &pfsetbldr->pCurrentScope[j].OperandCst);
	}
}

void NUT_BindNf32_Factor(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr,  const Nf32 constant  )
{
	NErrorIf( targetid1 < targetid0, NERROR_NUT_Nf32SET_PROCESS_ID_OUTOFRANGE );
	NErrorIf( targetid1 >= pfsetbldr->Nf32SetSize, NERROR_NUT_Nf32SET_PROCESS_ID_OUTOFRANGE );

	// Create a new range of scope ?
	if( pfsetbldr->pCurrentScope == pfsetbldr->pScopeStack )
	{
		pfsetbldr->pCurrentScope = _increase_scopestack_size(pfsetbldr);
	}

	// Store Incoming Constant
//	NUT_Nf32_SCOPE *pscope = &pfsetbldr->pScopeStack[pfsetbldr->ScopeStackSize-pfsetbldr->Nf32SetSize];
	for(Nu8 j=targetid0;j<=targetid1;j++)
	{
		pfsetbldr->pCurrentScope[j].Factor = constant;
	}
	FLAG_ON(pfsetbldr->Flags, FLAG_NUT_Nf32SET_BUILDER_SCOPE_FACTOR_UPDATED);
}

void NUT_BindNf32_FactorTuple(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr,  const Nf32 *pconstant_tuple  )
{
	NErrorIf( targetid1 < targetid0, NERROR_NUT_Nf32SET_PROCESS_ID_OUTOFRANGE );
	NErrorIf( targetid1 >= pfsetbldr->Nf32SetSize, NERROR_NUT_Nf32SET_PROCESS_ID_OUTOFRANGE );


	// Create a new range of scope ?
	if( pfsetbldr->pCurrentScope == pfsetbldr->pScopeStack )
	{
		pfsetbldr->pCurrentScope = _increase_scopestack_size(pfsetbldr);
	}

	// Store Incoming Operand
//	NUT_Nf32_SCOPE	*pscope	= &pfsetbldr->pScopeStack[pfsetbldr->ScopeStackSize-pfsetbldr->Nf32SetSize];
	Nf32			*pcst	= (Nf32*)pconstant_tuple;
	for(Nu8 j=targetid0;j<=targetid1;j++,pcst++)
	{
		pfsetbldr->pCurrentScope[j].Factor = *pcst;
	}
	FLAG_ON(pfsetbldr->Flags, FLAG_NUT_Nf32SET_BUILDER_SCOPE_FACTOR_UPDATED);
}

void NUT_Nf32OperandCst_Set(Nf32 *pdst, const Nf32 src, const Nf32 cst)
{
	*pdst = src*cst; 
}
void NUT_Nf32Operand_Set(Nf32 *pdst, const Nf32 src)
{
	*pdst = src; 
}

void NUT_Nf32OperandCst_Add(Nf32 *pdst, const Nf32 src, const Nf32 cst)
{
	*pdst += src*cst; 
}
void NUT_Nf32Operand_Add(Nf32 *pdst, const Nf32 src)
{
	*pdst += src; 
}

void NUT_Nf32OperandCst_Scale(Nf32 *pdst, const Nf32 src, const Nf32 cst)
{
	*pdst *= src*cst; 
}
void NUT_Nf32Operand_Scale(Nf32 *pdst, const Nf32 src)
{
	*pdst *= src; 
}

void NUT_Nf32OperandCst_Min(Nf32 *pdst, const Nf32 src, const Nf32 cst)
{
	*pdst = NMIN(*pdst,src*cst); 
}
void NUT_Nf32Operand_Min(Nf32 *pdst, const Nf32 src)
{
	*pdst = NMIN(*pdst,src); 
}

void NUT_Nf32OperandCst_Max(Nf32 *pdst, const Nf32 src, const Nf32 cst)
{
	*pdst = NMAX(*pdst,src*cst); 
}
void NUT_Nf32Operand_Max(Nf32 *pdst, const Nf32 src)
{
	*pdst = NMAX(*pdst,src); 
}

void NUT_Nf32OperandCst_Blend(Nf32 *pdst, const Nf32 src, const Nf32 cst)
{
	*pdst = *pdst + (src-*pdst)*cst; 
}
void NUT_Nf32Operand_Blend(Nf32 *pdst, const Nf32 src)
{
	*pdst = src; // No constant means constant = 1 !!!  
}

void NUT_GetNf32OperandHandle(NUT_Nf32SET_OPERAND_ENUM operand_id, NUT_Nf32_OPERAND *poperand, NUT_Nf32_OPERAND_CST *poperand_cst)
{
	switch(operand_id)
	{
		case NUT_Nf32SET_SET:
			*poperand		=  NUT_Nf32Operand_Set;
			*poperand_cst	=  NUT_Nf32OperandCst_Set;
			break;
		case NUT_Nf32SET_ADD:
			*poperand		=  NUT_Nf32Operand_Add;
			*poperand_cst	=  NUT_Nf32OperandCst_Add;
			break;
		case NUT_Nf32SET_SCALE:
			*poperand		=  NUT_Nf32Operand_Scale;
			*poperand_cst	=  NUT_Nf32OperandCst_Scale;
			break;
		case NUT_Nf32SET_BLEND:
			*poperand		=  NUT_Nf32Operand_Blend;
			*poperand_cst	=  NUT_Nf32OperandCst_Blend;
			break;
		case NUT_Nf32SET_MIN:
			*poperand		=  NUT_Nf32Operand_Min;
			*poperand_cst	=  NUT_Nf32OperandCst_Min;
			break;
		case NUT_Nf32SET_MAX:
			*poperand		=  NUT_Nf32Operand_Max;
			*poperand_cst	=  NUT_Nf32OperandCst_Max;
			break;
		default:
			NErrorIf(1,NERROR_UNAUTHORIZED_CASE);
			*poperand		=  NUT_Nf32Operand_Set;
			*poperand_cst	=  NUT_Nf32OperandCst_Set;
			break;
	}
}


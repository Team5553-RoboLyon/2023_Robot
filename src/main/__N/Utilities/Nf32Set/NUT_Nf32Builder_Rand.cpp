#include "../../NErrorHandling.h"
#include "NUT_Nf32Set.h"

// ------------------------------------------------------------------------------------------
// void NUT_SequenceNf32GenRand
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
void NUT_Nf32_BuilderRand_Update(NUT_Nf32_PROCESS *pfproc,Nf32 *pdstsequence_firstnf32set, const Nu32 stridetonext, const Nu32 sequence_size)
{
	Nu32 i,j;
	Nf32 *pf				= &pdstsequence_firstnf32set[pfproc->Core.TargetId0];
	Nu8	delta				= 1 + pfproc->Core.TargetId1 -  pfproc->Core.TargetId0;
	NUT_Nf32_SCOPE	*pscope = &pfproc->Core.pScope[pfproc->Core.TargetId0];
	Nf32 *prand				= NGetFastRandRangePtr((delta+1)*sequence_size);

	if( ISFLAG_ON(pfproc->Core.Flags,FLAG_NUT_Nf32_PROCESS_USE_FACTOR) )
	{
		switch( delta )
		{
			case 1:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand++ )
				{
					pscope->OperandCst( pf,*prand, pscope->Factor );
				}
				return;

			case 2:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand +=2 )
				{
					pscope->OperandCst( pf,*prand,pscope->Factor );
					pscope[1].OperandCst( pf+1,*(prand+1),pscope[1].Factor );
				}
				return;

			case 3:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand+=3 )
				{
					pscope->OperandCst( pf,*prand,pscope->Factor );
					pscope[1].OperandCst( pf+1,*(prand+1),pscope[1].Factor );
					pscope[2].OperandCst( pf+2,*(prand+2),pscope[2].Factor );
				}
				return;

			case 4:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand+=4 )
				{
					pscope->OperandCst( pf,*prand,pscope->Factor );
					pscope[1].OperandCst( pf+1,*(prand+1),pscope[1].Factor );
					pscope[2].OperandCst( pf+2,*(prand+2),pscope[2].Factor );
					pscope[3].OperandCst( pf+3,*(prand+3),pscope[3].Factor );
				}
				return;

			case 5:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand+=5 )
				{
					pscope->OperandCst( pf,*prand,pscope->Factor );
					pscope[1].OperandCst( pf+1,*(prand+1),pscope[1].Factor );
					pscope[2].OperandCst( pf+2,*(prand+2),pscope[2].Factor );
					pscope[3].OperandCst( pf+3,*(prand+3),pscope[3].Factor );
					pscope[4].OperandCst( pf+4,*(prand+4),pscope[4].Factor );
				}
				return;

			default:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand+=delta )
				{
					for(j=0;j<delta;j++)
					{
						pscope[j].OperandCst( pf+j,*(prand+j),pscope[j].Factor );
					}
				}
				return;
		}
	}
	else
	{
		switch( delta )
		{
			case 1:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand++ )
				{
					pscope->Operand( pf,*prand );
				}
				return;

			case 2:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand +=2 )
				{
					pscope->Operand( pf,*prand );
					pscope[1].Operand( pf+1,*(prand+1) );
				}
				return;

			case 3:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand+=3 )
				{
					pscope->Operand( pf,*prand );
					pscope[1].Operand( pf+1,*(prand+1) );
					pscope[2].Operand( pf+2,*(prand+2) );
				}
				return;

			case 4:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand+=4 )
				{
					pscope->Operand( pf,*prand );
					pscope[1].Operand( pf+1,*(prand+1) );
					pscope[2].Operand( pf+2,*(prand+2) );
					pscope[3].Operand( pf+3,*(prand+3) );
				}
				return;

			case 5:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand+=5 )
				{
					pscope->Operand( pf,*prand );
					pscope[1].Operand( pf+1,*(prand+1) );
					pscope[2].Operand( pf+2,*(prand+2) );
					pscope[3].Operand( pf+3,*(prand+3) );
					pscope[4].Operand( pf+4,*(prand+4) );
				}
				return;

			default:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand+=delta )
				{
					for(j=0;j<delta;j++)
					{
						pscope[j].Operand( pf+j,*(prand+j) );
					}
				}
				return;
		}
	}
}
void NUT_Nf32_BuilderRandOAAT_Update(NUT_Nf32_PROCESS *pfproc,Nf32 *pdstsequence_firstnf32set, const Nu32 stridetonext, const Nu32 sequence_size)
{
	Nu32 i,j;
	Nf32 *pf				= &pdstsequence_firstnf32set[pfproc->Core.TargetId0];
	Nu8	delta				= 1 + pfproc->Core.TargetId1 -  pfproc->Core.TargetId0;
	NUT_Nf32_SCOPE	*pscope = &pfproc->Core.pScope[pfproc->Core.TargetId0];
	Nf32 *prand				= NGetFastRandRangePtr(sequence_size);

	if( ISFLAG_ON(pfproc->Core.Flags,FLAG_NUT_Nf32_PROCESS_USE_FACTOR) )
	{
		switch( delta )
		{
		case 1:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand++ )
			{
				pscope->OperandCst( pf,*prand, pscope->Factor );
			}
			return;

		case 2:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand ++ )
			{
				pscope->OperandCst( pf,*prand,pscope->Factor );
				pscope[1].OperandCst( pf+1,*prand,pscope[1].Factor );
			}
			return;

		case 3:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand++ )
			{
				pscope->OperandCst( pf,*prand,pscope->Factor );
				pscope[1].OperandCst( pf+1,*prand,pscope[1].Factor );
				pscope[2].OperandCst( pf+2,*prand,pscope[2].Factor );
			}
			return;

		case 4:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand++ )
			{
				pscope->OperandCst( pf,*prand,pscope->Factor );
				pscope[1].OperandCst( pf+1,*prand,pscope[1].Factor );
				pscope[2].OperandCst( pf+2,*prand,pscope[2].Factor );
				pscope[3].OperandCst( pf+3,*prand,pscope[3].Factor );
			}
			return;

		case 5:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand++ )
			{
				pscope->OperandCst( pf,*prand,pscope->Factor );
				pscope[1].OperandCst( pf+1,*prand,pscope[1].Factor );
				pscope[2].OperandCst( pf+2,*prand,pscope[2].Factor );
				pscope[3].OperandCst( pf+3,*prand,pscope[3].Factor );
				pscope[4].OperandCst( pf+4,*prand,pscope[4].Factor );
			}
			return;

		default:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand++ )
			{
				for(j=0;j<delta;j++)
				{
					pscope[j].OperandCst( pf+j,*prand,pscope[j].Factor );
				}
			}
			return;
		}
	}
	else
	{
		switch( delta )
		{
		case 1:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand++ )
			{
				pscope->Operand( pf,*prand );
			}
			return;

		case 2:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand ++ )
			{
				pscope->Operand( pf,*prand );
				pscope[1].Operand( pf+1,*prand );
			}
			return;

		case 3:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand++ )
			{
				pscope->Operand( pf,*prand );
				pscope[1].Operand( pf+1,*prand );
				pscope[2].Operand( pf+2,*prand );
			}
			return;

		case 4:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand++ )
			{
				pscope->Operand( pf,*prand );
				pscope[1].Operand( pf+1,*prand );
				pscope[2].Operand( pf+2,*prand );
				pscope[3].Operand( pf+3,*prand );
			}
			return;

		case 5:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand++ )
			{
				pscope->Operand( pf,*prand );
				pscope[1].Operand( pf+1,*prand );
				pscope[2].Operand( pf+2,*prand );
				pscope[3].Operand( pf+3,*prand );
				pscope[4].Operand( pf+4,*prand );
			}
			return;

		default:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),prand++ )
			{
				for(j=0;j<delta;j++)
				{
					pscope[j].Operand( pf+j,*prand );
				}
			}
			return;
		}
	}
}
void NUT_Nf32_BuilderRandKey_Update(NUT_Nf32_PROCESS *pfproc,Nf32 *pdstsequence_firstnf32set, const Nu32 stridetonext, const Nu32 sequence_size)
{
	Nu32 i,j,k;
	Nf32 *pf				= &pdstsequence_firstnf32set[pfproc->Core.TargetId0];
	Nu8	delta				= 1 + pfproc->Core.TargetId1 -  pfproc->Core.TargetId0;
	NUT_Nf32_SCOPE	*pscope = &pfproc->Core.pScope[pfproc->Core.TargetId0];
	Nf32 *pr				= pfproc->Core.pSharedBuffer;
	Nf32 *pra				= &pfproc->Core.pSharedBuffer[pfproc->Core.Nf32SetSize];
	Nf32 *prb				= &pfproc->Core.pSharedBuffer[pfproc->Core.Nf32SetSize*2];
	Nf32 *pd				= &pfproc->Core.pSharedBuffer[pfproc->Core.Nf32SetSize*3];
	
	Nf32 step;	
	
	k = 0;
	for(j=0;j<delta;j++)
	{
		prb[j] = NFastRand();
	}
	step = 1.0f/(Nf32)pfproc->Rand.KeyEvery;
	if( ISFLAG_ON(pfproc->Core.Flags,FLAG_NUT_Nf32_PROCESS_USE_FACTOR) )
	{
		for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
		{
			if(!k)
			{
				k = pfproc->Rand.KeyEvery - 1;
				for(j=0;j<delta;j++)
				{
					pr[j] = pra[j] = prb[j];
					prb[j]= NFastRand();
					pd[j] = (prb[j]-pra[j])*step;
					pscope[j].OperandCst( pf+j,pr[j],pscope[j].Factor );
				}
			}
			else
			{
				k--;
				for(j=0;j<delta;j++)
				{
					pr[j] += pd[j];
					pscope[j].OperandCst( pf+j,pr[j],pscope[j].Factor );
				}
			}
		}
	}
	else
	{
		for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext))
		{
			if(!k)
			{
				k = pfproc->Rand.KeyEvery - 1;
				for(j=0;j<delta;j++)
				{
					pr[j] = pra[j] = prb[j];
					prb[j]= NFastRand();
					pd[j] = (prb[j]-pra[j])*step;
					pscope[j].Operand( pf+j,pr[j] );
				}
			}
			else
			{
				k--;
				for(j=0;j<delta;j++)
				{
					pr[j] += pd[j];
					pscope[j].Operand( pf+j,pr[j] );
				}
			}
		}
	}
}

void NUT_Nf32_BuilderRandKeyOAAT_Update(NUT_Nf32_PROCESS *pfproc,Nf32 *pdstsequence_firstnf32set, const Nu32 stridetonext, const Nu32 sequence_size)
{
	Nu32 i,j,k;
	Nf32 *pf				= &pdstsequence_firstnf32set[pfproc->Core.TargetId0];
	Nu8	delta				= 1 + pfproc->Core.TargetId1 -  pfproc->Core.TargetId0;
	NUT_Nf32_SCOPE	*pscope = &pfproc->Core.pScope[pfproc->Core.TargetId0];
	Nf32 r;		
	Nf32 ra;	
	Nf32 rb;	
	Nf32 d;		

	Nf32 step;	

	k = 0;
	rb = NFastRand();
	step = 1.0f/(Nf32)pfproc->Rand.KeyEvery;
	if( ISFLAG_ON(pfproc->Core.Flags,FLAG_NUT_Nf32_PROCESS_USE_FACTOR) )
	{
		for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
		{
			if(!k)
			{
				k = pfproc->Rand.KeyEvery - 1;
				r = ra = rb;
				rb= NFastRand();
				d = (rb-ra)*step;
			}
			else
			{
				r+=d;
				k--;
			}
			for(j=0;j<delta;j++)
			{
				pscope[j].OperandCst( pf+j,r,pscope[j].Factor );
			}
		}
	}
	else
	{
		for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext))
		{
			if(!k)
			{
				k = pfproc->Rand.KeyEvery - 1;
				r = ra = rb;
				rb= NFastRand();
				d = (rb-ra)*step;
			}
			else
			{
				r+=d;
				k--;
			}

			for(j=0;j<delta;j++)
			{
				pscope[j].Operand( pf+j,r );
			}
		}
	}
}
Nu32 NUT_Nf32_BuilderRand_Start(NUT_Nf32_PROCESS *pfproc,const Nu32 sequence_size)
{
	// This is the only one case that needs a Nf32 Buffer to work
	if(pfproc->Core.Update == NUT_Nf32_BuilderRandKey_Update)
	{
		// To allocate the shared buffer at minimum with this size.
		// Note that value is the maximum size we need ( with the duplication OFF ).
		return (Nu32)pfproc->Core.Nf32SetSize*4; 
	}
	else
	{
		return 0;
	}
}

void NUT_Nf32_BuilderRand_Clear(NUT_Nf32_PROCESS *pfproc)
{
	NUT_ClearNf32ProcessCore((NUT_Nf32_PROCESS_CORE*)pfproc);
	pfproc->Rand.KeyEvery	= 0;
}


// ------------------------------------------------------------------------------------------
// void NUT_BindNf32_BuilderRand
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
//			const Nu8			targetid0				
//			const Nu8			targetid1
//			NUT_Nf32SET_BUILDER *pfsetbldr	
//			const Nu32			randkey_every
//			const Nbool			duplicate			Usually, function rand() returns a different each time 				
//													result at each call. So if we fill up a Nf32Set randomly each Nf32
//													is going to be different.
//													To fill all the Nf32 of a set with the same rand(), and get a new rand()
//													for each Nf32set ( after each stride ) set this bool to TRUE.
//													To fill all Nf32 of all Nf32set with a different rand() value set this bool
//													to NFALSE.
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_BindNf32_BuilderRand( const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nu32 randkey_every )
{
	NUT_Nf32_PROCESS *pfproc	= NUT_GetNf32Process(targetid0,targetid1,pfsetbldr,NTRUE);
 
	// ... and setup it as a Randomize Float builder.
	pfproc->Rand.Core.Clear	= NUT_Nf32_BuilderRand_Clear;
	pfproc->Rand.Core.Start	= NUT_Nf32_BuilderRand_Start;
	pfproc->Rand.KeyEvery	= randkey_every;
	if(!pfproc->Rand.KeyEvery)
	{
		pfproc->Rand.Core.Update = NUT_Nf32_BuilderRand_Update;
	}
	else
	{
		pfproc->Rand.Core.Update = NUT_Nf32_BuilderRandKey_Update;
	}
}

// ------------------------------------------------------------------------------------------
// void NUT_BindNf32_BuilderRandOAAT
// ------------------------------------------------------------------------------------------
// Description :
//	Same purpose than NUT_BindNf32_BuilderRand BUT ...
//	A unique Rand number is generated for a whole Nf32Set and duplicated for each member if necessary.
//	OAAT means "Once At A Time"
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
//
// ------------------------------------------------------------------------------------------
void NUT_BindNf32_BuilderRandOAAT( const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nu32 randkey_every )
{
	NUT_Nf32_PROCESS *pfproc	= NUT_GetNf32Process(targetid0,targetid1,pfsetbldr,NTRUE);

	// ... and setup it as a Randomize Float builder.
	pfproc->Rand.Core.Clear	= NUT_Nf32_BuilderRand_Clear;
	pfproc->Rand.Core.Start	= NUT_Nf32_BuilderRand_Start;
	pfproc->Rand.KeyEvery	= randkey_every;
	if(!pfproc->Rand.KeyEvery)
	{
		pfproc->Rand.Core.Update = NUT_Nf32_BuilderRandOAAT_Update;
	}
	else
	{
		pfproc->Rand.Core.Update = NUT_Nf32_BuilderRandKeyOAAT_Update;
	}
}
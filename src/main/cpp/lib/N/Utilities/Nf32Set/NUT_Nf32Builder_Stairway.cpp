#include "../../NErrorHandling.h"
#include "NUT_Nf32Set.h"


Nu32 NUT_Nf32_BuilderStairway_Start(NUT_Nf32_PROCESS *pfproc,const Nu32 sequence_size)
{
/*
	Nu32 nbcall = sequence_size*pfbuilder->Core.UpdateCount;
	if(nbcall > 1)
	{
		pfbuilder->Stairway.Count		= 0;	
		pfbuilder->Stairway.CountMax	= nbcall-1;
	}
	else
	{
		pfbuilder->Stairway.Count		= 0;	
		pfbuilder->Stairway.CountMax	= 1;
	}
	
	if(ISFLAG_ON(pfbuilder->Stairway.Flags,FLAG_NUT_Nf32_BUILDER_STAIRWAY_IS_STEP_NUMBER))
	{
		NErrorIf(!pfbuilder->Stairway.StepEvery_or_StepNumber, NERROR_NULL_VALUE);	// This is WEIRD, user asks to use a number of step but gives a NULL value.
		if(pfbuilder->Stairway.StepEvery_or_StepNumber)								// CRASH in debug... (In release we will understand that the value is not suppose to evolve, so StepEvery > CountMax
			pfbuilder->Stairway.StepEvery = 1+pfbuilder->Stairway.CountMax/pfbuilder->Stairway.StepEvery_or_StepNumber;
		else
			pfbuilder->Stairway.StepEvery = NU32_MAX;
	}
	else
	{
		pfbuilder->Stairway.StepEvery = pfbuilder->Stairway.StepEvery_or_StepNumber; 
	}
*/
	return  0;
}

void NUT_Nf32_BuilderStairway_Update(NUT_Nf32_PROCESS *pfproc,Nf32 *pdstsequence_firstnf32set, const Nu32 stridetonext, const Nu32 sequence_size)
{
/*
	if( pfbuilder->Stairway.Count == pfbuilder->Stairway.StepEvery)
	{
		pfbuilder->Stairway.Count = 0;
		pfbuilder->Core.Cache += pfbuilder->Stairway.Step;
	}
	pfbuilder->Stairway.Count ++;
	return pfbuilder->Core.Cache;
*/

	NErrorIf(sequence_size < 2 ,NERROR_NUT_Nf32SET_SEQUENCE_SIZE_TOO_SHORT);

	Nf32 *pf = &pdstsequence_firstnf32set[pfproc->Core.TargetId0];
	Nu8 delta = 1 + pfproc->Core.TargetId1 -  pfproc->Core.TargetId0;
	NUT_Nf32_SCOPE	*pscope = &pfproc->Core.pScope[pfproc->Core.TargetId0];
	Nu32 i,j;
	Nu32 k = 0;
	Nf32 r = pfproc->Stairway.Start;
	k = 0;

	if(ISFLAG_ON(pfproc->Stairway.Flags,FLAG_NUT_Nf32_BUILDER_STAIRWAY_IS_STEP_NUMBER))
	{
		NErrorIf(!pfproc->Stairway.StepEvery_or_StepNumber, NERROR_NULL_VALUE);	// This is WEIRD, user asks to use a number of step but gives a NULL value.
		if(pfproc->Stairway.StepEvery_or_StepNumber)							// CRASH in debug... (In release we will understand that the value is not suppose to evolve, so StepEvery > CountMax
			pfproc->Stairway.StepEvery = 1+sequence_size/pfproc->Stairway.StepEvery_or_StepNumber;
		else
			pfproc->Stairway.StepEvery = NU32_MAX;
	}
	else
	{
		pfproc->Stairway.StepEvery = pfproc->Stairway.StepEvery_or_StepNumber; 
	}

	if( ISFLAG_ON(pfproc->Core.Flags,FLAG_NUT_Nf32_PROCESS_USE_FACTOR) )
	{
		switch( delta )
		{
			case 1:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),k++ )
				{
					if( k == pfproc->Stairway.StepEvery )
					{
						k = 0;
						r += pfproc->Stairway.Step;
					}
					pscope->OperandCst( pf,r, pscope->Factor );
				}
				return;

			case 2:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),k++ )
				{
					if( k == pfproc->Stairway.StepEvery )
					{
						k = 0;
						r += pfproc->Stairway.Step;
					}
					pscope->OperandCst( pf,r,pscope->Factor );
					pscope[1].OperandCst( pf+1,r,pscope[1].Factor );
				}
				return;

			case 3:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),k++ )
				{
					if( k == pfproc->Stairway.StepEvery )
					{
						k = 0;
						r += pfproc->Stairway.Step;
					}
					pscope->OperandCst( pf,r,pscope->Factor );
					pscope[1].OperandCst( pf+1,r,pscope[1].Factor );
					pscope[2].OperandCst( pf+2,r,pscope[2].Factor );
				}
				return;

			case 4:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),k++ )
				{
					if( k == pfproc->Stairway.StepEvery )
					{
						k = 0;
						r += pfproc->Stairway.Step;
					}
					pscope->OperandCst( pf,r,pscope->Factor );
					pscope[1].OperandCst( pf+1,r,pscope[1].Factor );
					pscope[2].OperandCst( pf+2,r,pscope[2].Factor );
					pscope[3].OperandCst( pf+3,r,pscope[3].Factor );
				}
				return;

			default:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),k++ )
				{
					if( k == pfproc->Stairway.StepEvery )
					{
						k = 0;
						r += pfproc->Stairway.Step;
					}
					for(j=0;j<delta;j++)
					{
						pscope[j].OperandCst( pf+j,r,pscope[j].Factor );
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
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),k++ )
			{
				if( k == pfproc->Stairway.StepEvery )
				{
					k = 0;
					r += pfproc->Stairway.Step;
				}
				pscope->Operand( pf,r );
			}
			return;

		case 2:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),k++ )
			{
				if( k == pfproc->Stairway.StepEvery )
				{
					k = 0;
					r += pfproc->Stairway.Step;
				}
				pscope->Operand( pf,r);
				pscope[1].Operand( pf+1,r);
			}
			return;

		case 3:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),k++ )
			{
				if( k == pfproc->Stairway.StepEvery )
				{
					k = 0;
					r += pfproc->Stairway.Step;
				}
				pscope->Operand( pf,r );
				pscope[1].Operand( pf+1,r );
				pscope[2].Operand( pf+2,r );
			}
			return;

		case 4:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),k++ )
			{
				if( k == pfproc->Stairway.StepEvery )
				{
					k = 0;
					r += pfproc->Stairway.Step;
				}
				pscope->Operand( pf,r );
				pscope[1].Operand( pf+1,r );
				pscope[2].Operand( pf+2,r );
				pscope[3].Operand( pf+3,r );
			}
			return;

		default:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),k++ )
			{
				if( k == pfproc->Stairway.StepEvery )
				{
					k = 0;
					r += pfproc->Stairway.Step;
				}
				for(j=0;j<delta;j++)
				{
					pscope[j].Operand( pf+j,r );
				}
			}
			return;
		}
	}
}


void NUT_Nf32_BuilderStairway_Clear(NUT_Nf32_PROCESS *pfproc)
{
	NUT_ClearNf32ProcessCore((NUT_Nf32_PROCESS_CORE*)pfproc);
	pfproc->Stairway.Start		= 0.0f;
	pfproc->Stairway.Step		= 0.0f;
	pfproc->Stairway.StepEvery_or_StepNumber	= 0;
	pfproc->Stairway.StepEvery	= 0;
	pfproc->Stairway.Flags		= 0;
}

void NUT_BindNf32_BuilderStairway(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nf32 start, const Nf32 step,const Nu32 stepevery_or_stepnumber, const Nbool is_step_number)
{
	NUT_Nf32_PROCESS *pfproc	= NUT_GetNf32Process(targetid0,targetid1,pfsetbldr,NTRUE);

	// ... and setup it as a from LERP Float builder.
	pfproc->Stairway.Core.Start	= NUT_Nf32_BuilderStairway_Start;
	pfproc->Stairway.Core.Update= NUT_Nf32_BuilderStairway_Update;
	pfproc->Stairway.Core.Clear	= NUT_Nf32_BuilderStairway_Clear;
 	pfproc->Stairway.Start		= start;
 	pfproc->Stairway.Step		= step;
	pfproc->Stairway.StepEvery_or_StepNumber	= stepevery_or_stepnumber;
	pfproc->Stairway.StepEvery	= 0;
	if(is_step_number)
		FLAG_ON(pfproc->Stairway.Flags,FLAG_NUT_Nf32_BUILDER_STAIRWAY_IS_STEP_NUMBER);
	else
		FLAG_OFF(pfproc->Stairway.Flags,FLAG_NUT_Nf32_BUILDER_STAIRWAY_IS_STEP_NUMBER);
}


#include "../../NErrorHandling.h"
#include "NUT_Nf32Set.h"


Nu32 NUT_Nf32Builder_Constant_Start(NUT_Nf32_PROCESS *pfproc,const Nu32 sequence_size)
{
	return 0;
}

void NUT_Nf32Builder_Constant_Update(NUT_Nf32_PROCESS *pfproc,Nf32 *pdstsequence_firstnf32set, const Nu32 stridetonext, const Nu32 sequence_size)
{
	NErrorIf(sequence_size < 2 ,NERROR_NUT_Nf32SET_SEQUENCE_SIZE_TOO_SHORT);

	Nf32 *pf = &pdstsequence_firstnf32set[pfproc->Core.TargetId0];
	Nu8 delta = 1 + pfproc->Core.TargetId1 -  pfproc->Core.TargetId0;
	NUT_Nf32_SCOPE	*pscope = &pfproc->Core.pScope[pfproc->Core.TargetId0];
	Nu32 i,j;

	if( ISFLAG_ON(pfproc->Core.Flags,FLAG_NUT_Nf32_PROCESS_USE_FACTOR) )
	{
		switch( delta )
		{
		case 1:
			for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
			{
				pscope->OperandCst( pf,pfproc->Constant.Constant, pscope->Factor );
			}
			return;

		case 2:
			for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
			{
				pscope->OperandCst( pf,pfproc->Constant.Constant,pscope->Factor );
				pscope[1].OperandCst( pf+1,pfproc->Constant.Constant,pscope[1].Factor );
			}
			return;

		case 3:
			for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
			{
				pscope->OperandCst( pf,pfproc->Constant.Constant,pscope->Factor );
				pscope[1].OperandCst( pf+1,pfproc->Constant.Constant,pscope[1].Factor );
				pscope[2].OperandCst( pf+2,pfproc->Constant.Constant,pscope[2].Factor );
			}
			return;

		case 4:
			for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
			{
				pscope->OperandCst( pf,pfproc->Constant.Constant,pscope->Factor );
				pscope[1].OperandCst( pf+1,pfproc->Constant.Constant,pscope[1].Factor );
				pscope[2].OperandCst( pf+2,pfproc->Constant.Constant,pscope[2].Factor );
				pscope[3].OperandCst( pf+3,pfproc->Constant.Constant,pscope[3].Factor );
			}
			return;

		default:
/*
			for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
			{
				for(j=0;j<delta;j++)
				{
					pscope[j].OperandCst( pf+j,pfproc->Constant.Value,pscope[j].Factor );
				}
			}
*/
			for(j=0;j<delta;j++)
			{
				pf = &pdstsequence_firstnf32set[pfproc->Core.TargetId0+j];
				for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
				{
					pscope[j].OperandCst( pf,pfproc->Constant.Constant,pscope[j].Factor );
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
			for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
			{
				pscope->Operand( pf,pfproc->Constant.Constant );
			}
			return;

		case 2:
			for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
			{
				pscope->Operand( pf,pfproc->Constant.Constant);
				pscope[1].Operand( pf+1,pfproc->Constant.Constant);
			}
			return;

		case 3:
			for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
			{
				pscope->Operand( pf,pfproc->Constant.Constant );
				pscope[1].Operand( pf+1,pfproc->Constant.Constant );
				pscope[2].Operand( pf+2,pfproc->Constant.Constant );
			}
			return;

		case 4:
			for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
			{
				pscope->Operand( pf,pfproc->Constant.Constant );
				pscope[1].Operand( pf+1,pfproc->Constant.Constant );
				pscope[2].Operand( pf+2,pfproc->Constant.Constant );
				pscope[3].Operand( pf+3,pfproc->Constant.Constant );
			}
			return;

		default:
/*
			for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
			{
				for(j=0;j<delta;j++)
				{
					pscope[j].Operand( pf+j,pfproc->Constant.Value );
				}
			}
*/
			for(j=0;j<delta;j++)
			{
				pf = &pdstsequence_firstnf32set[pfproc->Core.TargetId0+j];
				for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
				{
					pscope[j].Operand( pf,pfproc->Constant.Constant );
				}
			}

			return;
		}
	}
}

void NUT_Nf32Builder_Constant_Clear(NUT_Nf32_PROCESS *pfproc)
{
	NUT_ClearNf32ProcessCore((NUT_Nf32_PROCESS_CORE*)pfproc);
	pfproc->Constant.Constant= 0.0f;
}

void NUT_BindNf32_BuilderConstant(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nf32 constant )
{
	NUT_Nf32_PROCESS *pfproc	= NUT_GetNf32Process( targetid0,targetid1, pfsetbldr,NTRUE );
	pfproc->Core.Start	= NUT_Nf32Builder_Constant_Start;
	pfproc->Core.Update	= NUT_Nf32Builder_Constant_Update;	
	pfproc->Core.Clear	= NUT_Nf32Builder_Constant_Clear;

	// ... and setup it as a from LERP Float builder.
	pfproc->Constant.Constant = constant;
}


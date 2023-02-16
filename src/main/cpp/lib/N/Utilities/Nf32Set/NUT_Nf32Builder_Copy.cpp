#include "../../NErrorHandling.h"
#include "NUT_Nf32Set.h"


Nu32 NUT_Nf32_BuilderCopy_Start(NUT_Nf32_PROCESS *pfproc,const Nu32 sequence_size)
{
	return 0;
}

void NUT_Nf32_BuilderCopy_Update(NUT_Nf32_PROCESS *pfproc,Nf32 *pdstsequence_firstnf32set, const Nu32 stridetonext, const Nu32 sequence_size)
{
	NErrorIf(sequence_size < 2 ,NERROR_NUT_Nf32SET_SEQUENCE_SIZE_TOO_SHORT);

	Nf32 *pf = &pdstsequence_firstnf32set[pfproc->Core.TargetId0];
	Nf32 *psrc = pdstsequence_firstnf32set;
	Nu8 delta = 1 + pfproc->Core.TargetId1 -  pfproc->Core.TargetId0;
	NUT_Nf32_SCOPE	*pscope = &pfproc->Core.pScope[pfproc->Core.TargetId0];
	Nu32 i,j;
	Nu8 srcid = pfproc->Copy.SrcId;

	if( ISFLAG_ON(pfproc->Core.Flags,FLAG_NUT_Nf32_PROCESS_USE_FACTOR) )
	{
		switch( delta )
		{
		case 1:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),psrc = (Nf32*)((NBYTE*)psrc + stridetonext) )
			{
				pscope->OperandCst( pf,psrc[srcid], pscope->Factor );
			}
			return;

		case 2:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),psrc = (Nf32*)((NBYTE*)psrc + stridetonext) )
			{
				pscope->OperandCst( pf,psrc[srcid],pscope->Factor );
				pscope[1].OperandCst( pf+1,psrc[srcid],pscope[1].Factor );
			}
			return;

		case 3:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),psrc = (Nf32*)((NBYTE*)psrc + stridetonext) )
			{
				pscope->OperandCst( pf,psrc[srcid],pscope->Factor );
				pscope[1].OperandCst( pf+1,psrc[srcid],pscope[1].Factor );
				pscope[2].OperandCst( pf+2,psrc[srcid],pscope[2].Factor );
			}
			return;

		case 4:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),psrc = (Nf32*)((NBYTE*)psrc + stridetonext) )
			{
				pscope->OperandCst( pf,psrc[srcid],pscope->Factor );
				pscope[1].OperandCst( pf+1,psrc[srcid],pscope[1].Factor );
				pscope[2].OperandCst( pf+2,psrc[srcid],pscope[2].Factor );
				pscope[3].OperandCst( pf+3,psrc[srcid],pscope[3].Factor );
			}
			return;

		default:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),psrc = (Nf32*)((NBYTE*)psrc + stridetonext) )
			{
				for(j=0;j<delta;j++)
				{
					pscope[j].OperandCst( pf+j,psrc[srcid],pscope[j].Factor );
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
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),psrc = (Nf32*)((NBYTE*)psrc + stridetonext) )
			{
				pscope->Operand( pf,psrc[srcid] );
			}
			return;

		case 2:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),psrc = (Nf32*)((NBYTE*)psrc + stridetonext) )
			{
				pscope->Operand( pf,psrc[srcid]);
				pscope[1].Operand( pf+1,psrc[srcid]);
			}
			return;

		case 3:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),psrc = (Nf32*)((NBYTE*)psrc + stridetonext) )
			{
				pscope->Operand( pf,psrc[srcid] );
				pscope[1].Operand( pf+1,psrc[srcid] );
				pscope[2].Operand( pf+2,psrc[srcid] );
			}
			return;

		case 4:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),psrc = (Nf32*)((NBYTE*)psrc + stridetonext) )
			{
				pscope->Operand( pf,psrc[srcid] );
				pscope[1].Operand( pf+1,psrc[srcid] );
				pscope[2].Operand( pf+2,psrc[srcid] );
				pscope[3].Operand( pf+3,psrc[srcid] );
			}
			return;

		default:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),psrc = (Nf32*)((NBYTE*)psrc + stridetonext) )
			{
				for(j=0;j<delta;j++)
				{
					pscope[j].Operand( pf+j,psrc[srcid]);
				}
			}
			return;
		}
	}
}

void NUT_Nf32_BuilderCopy_Clear(NUT_Nf32_PROCESS *pfproc)
{
	NUT_ClearNf32ProcessCore((NUT_Nf32_PROCESS_CORE*)pfproc);
	pfproc->Copy.SrcId		= 0;
}

void NUT_BindNf32_BuilderCopy(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nu8 srcid)
{
	NErrorIf(srcid >= pfsetbldr->Nf32SetSize, NERROR_NUT_Nf32SET_BUILDER_ID_OUTOFRANGE);
	NUT_Nf32_PROCESS *pfproc	= NUT_GetNf32Process(targetid0,targetid1,pfsetbldr,NTRUE);

	// ... and setup it as a copy Float builder.
	pfproc->Copy.Core.Start	= NUT_Nf32_BuilderCopy_Start;
	pfproc->Copy.Core.Clear	= NUT_Nf32_BuilderCopy_Clear;
	pfproc->Copy.Core.Update= NUT_Nf32_BuilderCopy_Update;
	pfproc->Copy.SrcId		= srcid;
}


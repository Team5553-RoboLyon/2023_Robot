#include "../../NErrorHandling.h"
#include "NUT_Nf32Set.h"


Nu32 NUT_Nf32_BuilderLerp_Start(NUT_Nf32_PROCESS *pfproc,const Nu32 sequence_size)
{
	return 0;
}

void NUT_Nf32_BuilderLerp_Update(NUT_Nf32_PROCESS *pfproc,Nf32 *pdstsequence_firstnf32set, const Nu32 stridetonext, const Nu32 sequence_size)
{
	NErrorIf(sequence_size < 2 ,NERROR_NUT_Nf32SET_SEQUENCE_SIZE_TOO_SHORT);

	Nf32 *pf = &pdstsequence_firstnf32set[pfproc->Core.TargetId0];
	Nu8 delta = 1 + pfproc->Core.TargetId1 -  pfproc->Core.TargetId0;
	NUT_Nf32_SCOPE	*pscope = &pfproc->Core.pScope[pfproc->Core.TargetId0];
	Nu32 i,j;
	Nf32 r;
	
	if( ISFLAG_ON(pfproc->Core.Flags,FLAG_NUT_Nf32_PROCESS_USE_FACTOR) )
	{
		switch( delta )
		{
			case 1:
				for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
				{
					pscope->OperandCst( pf,pfproc->Lerp.A + (pfproc->Lerp.B - pfproc->Lerp.A)*((Nf32)i/(Nf32)(sequence_size-1)), pscope->Factor );
				}
				return;

			case 2:
				for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
				{
					r = pfproc->Lerp.A + (pfproc->Lerp.B - pfproc->Lerp.A)*((Nf32)i/(Nf32)(sequence_size-1)); 
					pscope->OperandCst( pf,r,pscope->Factor );
					pscope[1].OperandCst( pf+1,r,pscope[1].Factor );
				}
				return;

			case 3:
				for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
				{
					r = pfproc->Lerp.A + (pfproc->Lerp.B - pfproc->Lerp.A)*((Nf32)i/(Nf32)(sequence_size-1)); 
					pscope->OperandCst( pf,r,pscope->Factor );
					pscope[1].OperandCst( pf+1,r,pscope[1].Factor );
					pscope[2].OperandCst( pf+2,r,pscope[2].Factor );
				}
				return;

			case 4:
				for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
				{
					r = pfproc->Lerp.A + (pfproc->Lerp.B - pfproc->Lerp.A)*((Nf32)i/(Nf32)(sequence_size-1)); 
					pscope->OperandCst( pf,r,pscope->Factor );
					pscope[1].OperandCst( pf+1,r,pscope[1].Factor );
					pscope[2].OperandCst( pf+2,r,pscope[2].Factor );
					pscope[3].OperandCst( pf+3,r,pscope[3].Factor );
				}
				return;

			default:
				for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
				{
					r = pfproc->Lerp.A + (pfproc->Lerp.B - pfproc->Lerp.A)*((Nf32)i/(Nf32)(sequence_size-1)); 
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
				for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
				{
					pscope->Operand( pf,pfproc->Lerp.A + (pfproc->Lerp.B - pfproc->Lerp.A)*((Nf32)i/(Nf32)(sequence_size-1)) );
				}
				return;

			case 2:
				for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
				{
					r = pfproc->Lerp.A + (pfproc->Lerp.B - pfproc->Lerp.A)*((Nf32)i/(Nf32)(sequence_size-1)); 
					pscope->Operand( pf,r);
					pscope[1].Operand( pf+1,r);
				}
				return;

			case 3:
				for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
				{
					r = pfproc->Lerp.A + (pfproc->Lerp.B - pfproc->Lerp.A)*((Nf32)i/(Nf32)(sequence_size-1)); 
					pscope->Operand( pf,r );
					pscope[1].Operand( pf+1,r );
					pscope[2].Operand( pf+2,r );
				}
				return;

			case 4:
				for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
				{
					r = pfproc->Lerp.A + (pfproc->Lerp.B - pfproc->Lerp.A)*((Nf32)i/(Nf32)(sequence_size-1)); 
					pscope->Operand( pf,r );
					pscope[1].Operand( pf+1,r );
					pscope[2].Operand( pf+2,r );
					pscope[3].Operand( pf+3,r );
				}
				return;

			default:
				for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
				{
					r = pfproc->Lerp.A + (pfproc->Lerp.B - pfproc->Lerp.A)*((Nf32)i/(Nf32)(sequence_size-1)); 
					for(j=0;j<delta;j++)
					{
						pscope[j].Operand( pf+j,r );
					}
				}
				return;
		}
	}
}

void NUT_Nf32_BuilderLerp_Clear(NUT_Nf32_PROCESS *pfproc)
{
	NUT_ClearNf32ProcessCore((NUT_Nf32_PROCESS_CORE*)pfproc);
	pfproc->Lerp.A			= 0.0f;
	pfproc->Lerp.B			= 0.0f;
}

void NUT_BindNf32_BuilderLerp(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nf32 a, const Nf32 b)
{
	NUT_Nf32_PROCESS *pfproc	= NUT_GetNf32Process(targetid0,targetid1,pfsetbldr,NTRUE);

	// ... and setup it as a from LERP Float builder.
	pfproc->Lerp.Core.Start	= NUT_Nf32_BuilderLerp_Start;
	pfproc->Lerp.Core.Update= NUT_Nf32_BuilderLerp_Update;
	pfproc->Lerp.Core.Clear	= NUT_Nf32_BuilderLerp_Clear;
 	pfproc->Lerp.A			= a;
 	pfproc->Lerp.B			= b;
}


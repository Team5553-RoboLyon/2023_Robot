
#include "../../NErrorHandling.h"
#include "NUT_Nf32Set.h"



void NUT_Nf32_BuilderSinusoid_FastUpdate(NUT_Nf32_PROCESS *pfproc,Nf32 *pdstsequence_firstnf32set, const Nu32 stridetonext, const Nu32 sequence_size)
{
	NErrorIf(sequence_size < 2 ,NERROR_NUT_Nf32SET_SEQUENCE_SIZE_TOO_SHORT);

	Nf32 *pf = &pdstsequence_firstnf32set[pfproc->Core.TargetId0];
	Nu8 delta = 1 + pfproc->Core.TargetId1 -  pfproc->Core.TargetId0;
	NUT_Nf32_SCOPE	*pscope = &pfproc->Core.pScope[pfproc->Core.TargetId0];
	Nu32 i,j;
	Nf32 r;
	Nf32 a = pfproc->Sinusoid.StartAngle;
	Nf32 step = (pfproc->Sinusoid.EndAngle - pfproc->Sinusoid.StartAngle)/(Nf32)(sequence_size-1);
	if( ISFLAG_ON(pfproc->Core.Flags,FLAG_NUT_Nf32_PROCESS_USE_FACTOR) )
	{
		switch( delta )
		{
		case 1:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),a+=step )
			{
				r = pfproc->Sinusoid.SineShift + NFastSin((Nu32)a); 
				pscope->OperandCst( pf,r , pscope->Factor );
			}
			return;

		case 2:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),a+=step )
			{
				r = pfproc->Sinusoid.SineShift + NFastSin((Nu32)a); 
				pscope->OperandCst( pf,r,pscope->Factor );
				pscope[1].OperandCst( pf+1,r,pscope[1].Factor );
			}
			return;

		case 3:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),a+=step )
			{
				r = pfproc->Sinusoid.SineShift + NFastSin((Nu32)a); 
				pscope->OperandCst( pf,r,pscope->Factor );
				pscope[1].OperandCst( pf+1,r,pscope[1].Factor );
				pscope[2].OperandCst( pf+2,r,pscope[2].Factor );
			}
			return;

		case 4:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),a+=step )
			{
				r = pfproc->Sinusoid.SineShift + NFastSin((Nu32)a); 
				pscope->OperandCst( pf,r,pscope->Factor );
				pscope[1].OperandCst( pf+1,r,pscope[1].Factor );
				pscope[2].OperandCst( pf+2,r,pscope[2].Factor );
				pscope[3].OperandCst( pf+3,r,pscope[3].Factor );
			}
			return;

		default:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),a+=step )
			{
				r = pfproc->Sinusoid.SineShift + NFastSin((Nu32)a); 
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
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),a+=step )
			{
				r = pfproc->Sinusoid.SineShift + NFastSin((Nu32)a); 
				pscope->Operand( pf, r );
			}
			return;

		case 2:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),a+=step )
			{
				r = pfproc->Sinusoid.SineShift + NFastSin((Nu32)a); 
				pscope->Operand( pf,r);
				pscope[1].Operand( pf+1,r);
			}
			return;

		case 3:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),a+=step )
			{
				r = pfproc->Sinusoid.SineShift + NFastSin((Nu32)a); 
				pscope->Operand( pf,r );
				pscope[1].Operand( pf+1,r );
				pscope[2].Operand( pf+2,r );
			}
			return;

		case 4:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),a+=step )
			{
				r = pfproc->Sinusoid.SineShift + NFastSin((Nu32)a); 
				pscope->Operand( pf,r );
				pscope[1].Operand( pf+1,r );
				pscope[2].Operand( pf+2,r );
				pscope[3].Operand( pf+3,r );
			}
			return;

		default:
			for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),a+=step )
			{
				r = pfproc->Sinusoid.SineShift + NFastSin((Nu32)a); 
				for(j=0;j<delta;j++)
				{
					pscope[j].Operand( pf+j,r );
				}
			}
			return;
		}
	}
}

void NUT_Nf32_BuilderSinusoid_Update(NUT_Nf32_PROCESS *pfproc,Nf32 *pdstsequence_firstnf32set, const Nu32 stridetonext, const Nu32 sequence_size)
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
					r = pfproc->Sinusoid.SineShift + 
						sin(pfproc->Sinusoid.StartAngle + (pfproc->Sinusoid.EndAngle - pfproc->Sinusoid.StartAngle)*((Nf32)i/(Nf32)(sequence_size-1))); 
					pscope->OperandCst( pf,r , pscope->Factor );
				}
				return;

			case 2:
				for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
				{
					r = pfproc->Sinusoid.SineShift + 
						sin(pfproc->Sinusoid.StartAngle + (pfproc->Sinusoid.EndAngle - pfproc->Sinusoid.StartAngle)*((Nf32)i/(Nf32)(sequence_size-1))); 
					pscope->OperandCst( pf,r,pscope->Factor );
					pscope[1].OperandCst( pf+1,r,pscope[1].Factor );
				}
				return;

			case 3:
				for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
				{
					r = pfproc->Sinusoid.SineShift + 
						sin(pfproc->Sinusoid.StartAngle + (pfproc->Sinusoid.EndAngle - pfproc->Sinusoid.StartAngle)*((Nf32)i/(Nf32)(sequence_size-1))); 
					pscope->OperandCst( pf,r,pscope->Factor );
					pscope[1].OperandCst( pf+1,r,pscope[1].Factor );
					pscope[2].OperandCst( pf+2,r,pscope[2].Factor );
				}
				return;

			case 4:
				for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
				{
					r = pfproc->Sinusoid.SineShift + 
						sin(pfproc->Sinusoid.StartAngle + (pfproc->Sinusoid.EndAngle - pfproc->Sinusoid.StartAngle)*((Nf32)i/(Nf32)(sequence_size-1))); 
					pscope->OperandCst( pf,r,pscope->Factor );
					pscope[1].OperandCst( pf+1,r,pscope[1].Factor );
					pscope[2].OperandCst( pf+2,r,pscope[2].Factor );
					pscope[3].OperandCst( pf+3,r,pscope[3].Factor );
				}
				return;

			default:
				for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
				{
					r = pfproc->Sinusoid.SineShift + 
						sin(pfproc->Sinusoid.StartAngle + (pfproc->Sinusoid.EndAngle - pfproc->Sinusoid.StartAngle)*((Nf32)i/(Nf32)(sequence_size-1))); 
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
					r = pfproc->Sinusoid.SineShift + 
						sin(pfproc->Sinusoid.StartAngle + (pfproc->Sinusoid.EndAngle - pfproc->Sinusoid.StartAngle)*((Nf32)i/(Nf32)(sequence_size-1))); 
					pscope->Operand( pf, r );
				}
				return;

			case 2:
				for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
				{
					r = pfproc->Sinusoid.SineShift + 
						sin(pfproc->Sinusoid.StartAngle + (pfproc->Sinusoid.EndAngle - pfproc->Sinusoid.StartAngle)*((Nf32)i/(Nf32)(sequence_size-1))); 
					pscope->Operand( pf,r);
					pscope[1].Operand( pf+1,r);
				}
				return;

			case 3:
				for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
				{
					r = pfproc->Sinusoid.SineShift + 
						sin(pfproc->Sinusoid.StartAngle + (pfproc->Sinusoid.EndAngle - pfproc->Sinusoid.StartAngle)*((Nf32)i/(Nf32)(sequence_size-1))); 
					pscope->Operand( pf,r );
					pscope[1].Operand( pf+1,r );
					pscope[2].Operand( pf+2,r );
				}
				return;

			case 4:
				for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
				{
					r = pfproc->Sinusoid.SineShift + 
						sin(pfproc->Sinusoid.StartAngle + (pfproc->Sinusoid.EndAngle - pfproc->Sinusoid.StartAngle)*((Nf32)i/(Nf32)(sequence_size-1))); 
					pscope->Operand( pf,r );
					pscope[1].Operand( pf+1,r );
					pscope[2].Operand( pf+2,r );
					pscope[3].Operand( pf+3,r );
				}
				return;

			default:
				for(i=0;i<sequence_size;i++, pf = (Nf32*)((NBYTE*)pf + stridetonext) )
				{
					r = pfproc->Sinusoid.SineShift + 
						sin(pfproc->Sinusoid.StartAngle + (pfproc->Sinusoid.EndAngle - pfproc->Sinusoid.StartAngle)*((Nf32)i/(Nf32)(sequence_size-1))); 
					for(j=0;j<delta;j++)
					{
						pscope[j].Operand( pf+j,r );
					}
				}
				return;
			}
	}
}


Nu32 NUT_Nf32_BuilderSinusoid_Start(NUT_Nf32_PROCESS *pfproc,const Nu32 sequence_size)
{
	return 0;
}


void NUT_Nf32_BuilderSinusoid_Clear(NUT_Nf32_PROCESS *pfproc)
{
	NUT_ClearNf32ProcessCore((NUT_Nf32_PROCESS_CORE*)pfproc);
	pfproc->Sinusoid.StartAngle	= 0.0f;
	pfproc->Sinusoid.EndAngle	= 0.0f;
	pfproc->Sinusoid.SineShift	= 0.0f;
}

void NUT_BindNf32_BuilderSinusoid(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nf32 rad_angle_start, const Nf32 rad_angle_end, const Nf32 sine_shifting )
{
	NUT_Nf32_PROCESS *pfproc	= NUT_GetNf32Process(targetid0,targetid1,pfsetbldr,NTRUE);

	// ... and setup it as a from Sinusoid Float builder.
	pfproc->Sinusoid.Core.Start	= NUT_Nf32_BuilderSinusoid_Start;
	pfproc->Sinusoid.Core.Clear	= NUT_Nf32_BuilderSinusoid_Clear;
	pfproc->Sinusoid.Core.Update= NUT_Nf32_BuilderSinusoid_Update;

	pfproc->Sinusoid.StartAngle	= rad_angle_start;
	pfproc->Sinusoid.EndAngle	= rad_angle_end;
	pfproc->Sinusoid.SineShift	= sine_shifting;
}

void NUT_BindNf32_BuilderFastSinusoid(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nu32 angle_start, const Nu32 angle_end, const Nf32 sine_shifting )
{
	NUT_Nf32_PROCESS *pfproc	= NUT_GetNf32Process(targetid0,targetid1,pfsetbldr,NTRUE);

	// ... and setup it as a from Sinusoid Float builder.
	pfproc->Sinusoid.Core.Start	= NUT_Nf32_BuilderSinusoid_Start;
	pfproc->Sinusoid.Core.Clear	= NUT_Nf32_BuilderSinusoid_Clear;
	pfproc->Sinusoid.Core.Update= NUT_Nf32_BuilderSinusoid_FastUpdate;

	pfproc->Sinusoid.StartAngle	= (Nf32)angle_start;
	pfproc->Sinusoid.EndAngle	= (Nf32)angle_end;
	pfproc->Sinusoid.SineShift	= sine_shifting;
}


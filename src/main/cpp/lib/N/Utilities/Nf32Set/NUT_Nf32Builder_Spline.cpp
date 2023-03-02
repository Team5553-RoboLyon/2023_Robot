#include "../../NErrorHandling.h"
#include "NUT_Nf32Set.h"

// ------------------------------------------------------------------------------------------
// void NUT_Nf32Builder_Spline_Update
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
void NUT_Nf32_BuilderSpline_Update(NUT_Nf32_PROCESS *pfproc,Nf32 *pdstsequence_firstnf32set, const Nu32 stridetonext, const Nu32 sequence_size)
{

	Nf32			*pf = &pdstsequence_firstnf32set[pfproc->Core.TargetId0];
	Nu8				delta = 1 + pfproc->Core.TargetId1 -  pfproc->Core.TargetId0;
	NUT_Nf32_SCOPE	*pscope = &pfproc->Core.pScope[pfproc->Core.TargetId0];
	Nu32			i,j;
	Nf32			step;
	NVEC2		R;

	if(sequence_size > 1)
		step = 1.0f/(Nf32)(sequence_size - 1);
	else
		step = 0.0f;

	R.x	= 0.0f;
	R.y = 0.0f;

	if( ISFLAG_ON(pfproc->Core.Flags,FLAG_NUT_Nf32_PROCESS_USE_FACTOR) )
	{
		switch( delta )
		{
			case 1:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),R.x+=step )
				{
					if( NUT_2DVerticalLineXSplineExtended( &R, &pfproc->Spline.PersistentXResult ) )
					{
						pscope->OperandCst( pf,pfproc->Spline.PersistentXResult.XResult.I.y, pscope->Factor );
					}
					else
					{
						NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION);	// It's not a BUG but, It seems Spline is not well configured.
						pscope->OperandCst( pf,0.0f, pscope->Factor );
					}										  
				}
				return;

			case 2:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),R.x+=step )
				{
					if( NUT_2DVerticalLineXSplineExtended( &R, &pfproc->Spline.PersistentXResult ) )
					{
						pscope->OperandCst( pf,pfproc->Spline.PersistentXResult.XResult.I.y, pscope->Factor );
						pscope[1].OperandCst( pf+1,pfproc->Spline.PersistentXResult.XResult.I.y,pscope[1].Factor );
					}
					else
					{
						NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION);	// It's not a BUG but, It seems Spline is not well configured.
						pscope->OperandCst( pf,0.0f, pscope->Factor );
						pscope[1].OperandCst( pf+1,0.0f,pscope[1].Factor );
					}										  
				}
				return;

			case 3:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),R.x+=step )
				{
					if( NUT_2DVerticalLineXSplineExtended( &R, &pfproc->Spline.PersistentXResult ) )
					{
						pscope->OperandCst( pf,pfproc->Spline.PersistentXResult.XResult.I.y, pscope->Factor );
						pscope[1].OperandCst( pf+1,pfproc->Spline.PersistentXResult.XResult.I.y,pscope[1].Factor );
						pscope[2].OperandCst( pf+2,pfproc->Spline.PersistentXResult.XResult.I.y,pscope[2].Factor );
					}
					else
					{
						NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION);	// It's not a BUG but, It seems Spline is not well configured.
						pscope->OperandCst( pf,0.0f, pscope->Factor );
						pscope[1].OperandCst( pf+1,0.0f,pscope[1].Factor );
						pscope[2].OperandCst( pf+2,0.0f,pscope[2].Factor );
					}										  
				}
				return;

			case 4:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),R.x+=step )
				{
					if( NUT_2DVerticalLineXSplineExtended( &R, &pfproc->Spline.PersistentXResult ) )
					{
						pscope->OperandCst( pf,pfproc->Spline.PersistentXResult.XResult.I.y, pscope->Factor );
						pscope[1].OperandCst( pf+1,pfproc->Spline.PersistentXResult.XResult.I.y,pscope[1].Factor );
						pscope[2].OperandCst( pf+2,pfproc->Spline.PersistentXResult.XResult.I.y,pscope[2].Factor );
						pscope[3].OperandCst( pf+3,pfproc->Spline.PersistentXResult.XResult.I.y,pscope[3].Factor );
					}
					else
					{
						NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION);	// It's not a BUG but, It seems Spline is not well configured.
						pscope->OperandCst( pf,0.0f, pscope->Factor );
						pscope[1].OperandCst( pf+1,0.0f,pscope[1].Factor );
						pscope[2].OperandCst( pf+2,0.0f,pscope[2].Factor );
						pscope[3].OperandCst( pf+3,0.0f,pscope[3].Factor );
					}										  
				}
				return;

			case 5:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),R.x+=step )
				{
					if( NUT_2DVerticalLineXSplineExtended( &R, &pfproc->Spline.PersistentXResult ) )
					{
						pscope->OperandCst( pf,pfproc->Spline.PersistentXResult.XResult.I.y, pscope->Factor );
						pscope[1].OperandCst( pf+1,pfproc->Spline.PersistentXResult.XResult.I.y,pscope[1].Factor );
						pscope[2].OperandCst( pf+2,pfproc->Spline.PersistentXResult.XResult.I.y,pscope[2].Factor );
						pscope[3].OperandCst( pf+3,pfproc->Spline.PersistentXResult.XResult.I.y,pscope[3].Factor );
						pscope[4].OperandCst( pf+4,pfproc->Spline.PersistentXResult.XResult.I.y,pscope[4].Factor );
					}
					else
					{
						NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION);	// It's not a BUG but, It seems Spline is not well configured.
						pscope->OperandCst( pf,0.0f, pscope->Factor );
						pscope[1].OperandCst( pf+1,0.0f,pscope[1].Factor );
						pscope[2].OperandCst( pf+2,0.0f,pscope[2].Factor );
						pscope[3].OperandCst( pf+3,0.0f,pscope[3].Factor );
						pscope[4].OperandCst( pf+4,0.0f,pscope[4].Factor );
					}										  
				}
				return;

			default:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),R.x+=step )
				{
					for(j=0;j<delta;j++)
					{
						if( NUT_2DVerticalLineXSplineExtended( &R, &pfproc->Spline.PersistentXResult ) )
						{
							pscope[j].OperandCst( pf+j,pfproc->Spline.PersistentXResult.XResult.I.y,pscope[j].Factor );
						}
						else
						{
							NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION);	// It's not a BUG but, It seems Spline is not well configured.
							pscope[j].OperandCst( pf+j,0.0f,pscope[j].Factor );
						}										  
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
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),R.x+=step )
				{
					if( NUT_2DVerticalLineXSplineExtended( &R, &pfproc->Spline.PersistentXResult ) )
					{
						pscope->Operand( pf,pfproc->Spline.PersistentXResult.XResult.I.y );
					}
					else
					{
						NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION);	// It's not a BUG but, It seems Spline is not well configured.
						pscope->Operand( pf,0.0f );
					}										  
				}
				return;

			case 2:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),R.x+=step )
				{
					if( NUT_2DVerticalLineXSplineExtended( &R, &pfproc->Spline.PersistentXResult ) )
					{
						pscope->Operand( pf,pfproc->Spline.PersistentXResult.XResult.I.y );
						pscope[1].Operand( pf+1,pfproc->Spline.PersistentXResult.XResult.I.y );
					}
					else
					{
						NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION);	// It's not a BUG but, It seems Spline is not well configured.
						pscope->Operand( pf,0.0f );
						pscope[1].Operand( pf+1,0.0f );
					}										  
				}
				return;

			case 3:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),R.x+=step )
				{
					if( NUT_2DVerticalLineXSplineExtended( &R, &pfproc->Spline.PersistentXResult ) )
					{
						pscope->Operand( pf,pfproc->Spline.PersistentXResult.XResult.I.y );
						pscope[1].Operand( pf+1,pfproc->Spline.PersistentXResult.XResult.I.y );
						pscope[2].Operand( pf+2,pfproc->Spline.PersistentXResult.XResult.I.y );
					}
					else
					{
						NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION);	// It's not a BUG but, It seems Spline is not well configured.
						pscope->Operand( pf,0.0f );
						pscope[1].Operand( pf+1,0.0f );
						pscope[2].Operand( pf+2,0.0f );
					}										  
				}
				return;

			case 4:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),R.x+=step )
				{
					if( NUT_2DVerticalLineXSplineExtended( &R, &pfproc->Spline.PersistentXResult ) )
					{
						pscope->Operand( pf,pfproc->Spline.PersistentXResult.XResult.I.y );
						pscope[1].Operand( pf+1,pfproc->Spline.PersistentXResult.XResult.I.y );
						pscope[2].Operand( pf+2,pfproc->Spline.PersistentXResult.XResult.I.y );
						pscope[3].Operand( pf+3,pfproc->Spline.PersistentXResult.XResult.I.y );
					}
					else
					{
						NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION);	// It's not a BUG but, It seems Spline is not well configured.
						pscope->Operand( pf,0.0f );
						pscope[1].Operand( pf+1,0.0f );
						pscope[2].Operand( pf+2,0.0f );
						pscope[3].Operand( pf+3,0.0f );
					}										  
				}
				return;

			case 5:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),R.x+=step )
				{
					if( NUT_2DVerticalLineXSplineExtended( &R, &pfproc->Spline.PersistentXResult ) )
					{
						pscope->Operand( pf,pfproc->Spline.PersistentXResult.XResult.I.y );
						pscope[1].Operand( pf+1,pfproc->Spline.PersistentXResult.XResult.I.y );
						pscope[2].Operand( pf+2,pfproc->Spline.PersistentXResult.XResult.I.y );
						pscope[3].Operand( pf+3,pfproc->Spline.PersistentXResult.XResult.I.y );
						pscope[4].Operand( pf+4,pfproc->Spline.PersistentXResult.XResult.I.y );
					}
					else
					{
						NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION);	// It's not a BUG but, It seems Spline is not well configured.
						pscope->Operand( pf,0.0f );
						pscope[1].Operand( pf+1,0.0f );
						pscope[2].Operand( pf+2,0.0f );
						pscope[3].Operand( pf+3,0.0f );
						pscope[4].Operand( pf+4,0.0f );
					}										  
				}
				return;

			default:
				for(i=sequence_size;i!=0;i--, pf = (Nf32*)((NBYTE*)pf + stridetonext),R.x+=step )
				{
					for(j=0;j<delta;j++)
					{
						if( NUT_2DVerticalLineXSplineExtended( &R, &pfproc->Spline.PersistentXResult ) )
						{
							pscope[j].Operand( pf+j,pfproc->Spline.PersistentXResult.XResult.I.y );
						}
						else
						{
							NErrorIf(1,NERROR_SYSTEM_GURU_MEDITATION);	// It's not a BUG but, It seems Spline is not well configured.
							pscope[j].Operand( pf+j,0.0f );
						}										  
					}
				}
				return;
			}
	}

}


Nu32 NUT_Nf32_BuilderSpline_Start(NUT_Nf32_PROCESS *pfproc,const Nu32 sequence_size)
{
	NUT_Reset2DSplinePersistentXResult( &pfproc->Spline.PersistentXResult );

	return 0;
	// Total number of Call alonf the entire sequence is going to be:
	//
	// "number of call" = "sequence_size" X "number of active references of this builder" 
	//
/*
	Nu32 nbcall = sequence_size*pfbuilder->Core.UpdateCount;

	if(nbcall > 1)
	{
		pfbuilder->Spline.Step		= 1.0f/(Nf32)(nbcall - 1);
		pfbuilder->Spline.Progress	= 0.0f;
	}
	else
	{
		pfbuilder->Spline.Step		= 0.0f;
		pfbuilder->Spline.Progress	= 0.0f;
	}
*/
}

void NUT_Nf32_BuilderSpline_Clear(NUT_Nf32_PROCESS *pfproc)
{
	NUT_ClearNf32ProcessCore((NUT_Nf32_PROCESS_CORE*)pfproc);
	NUT_Clear2DSplinePersistentXResult( &pfproc->Spline.PersistentXResult );
}

void NUT_BindNf32_BuilderSpline(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const NSPLINE *pspline)
{
	NUT_Nf32_PROCESS *pfproc	= NUT_GetNf32Process(targetid0,targetid1,pfsetbldr,NTRUE);

	// ... and setup it as a from Spline Float builder.
	NUT_Setup2DSplinePersistentXResult(&pfproc->Spline.PersistentXResult,pspline);
	pfproc->Spline.Core.Start	= NUT_Nf32_BuilderSpline_Start;
	pfproc->Spline.Core.Clear	= NUT_Nf32_BuilderSpline_Clear;
	pfproc->Spline.Core.Update	= NUT_Nf32_BuilderSpline_Update;
}


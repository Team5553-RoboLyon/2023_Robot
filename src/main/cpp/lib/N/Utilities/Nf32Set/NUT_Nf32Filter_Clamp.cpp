#include "../../NErrorHandling.h"
#include "NUT_Nf32Set.h"

Nu32 NUT_Nf32_FilterClamp_Start(NUT_Nf32_PROCESS *pfproc,const Nu32 sequence_size)
{
	return 0;
}

void NUT_Nf32_FilterClamp_Update(NUT_Nf32_PROCESS *pfproc,Nf32 *pdstsequence_firstnf32set, const Nu32 stridetonext, const Nu32 sequence_size)
{
	Nf32	*pf = &pdstsequence_firstnf32set[pfproc->Core.TargetId0];
	Nu8		delta = 1 + pfproc->Core.TargetId1 -  pfproc->Core.TargetId0;
	Nf32	newmin = pfproc->Clamp.Min;
	Nf32	newmax = pfproc->Clamp.Max;
	Nu32	i;
	Nu8		j;
	
	switch( delta )
	{
		case 1:
			for( i=sequence_size; i!=0; i--, pf=(Nf32*)( (NBYTE*)pf+stridetonext ) )
			{
				*pf = NCLAMP(newmin,*pf,newmax);
			}
			return;

		case 2:
			for( i=sequence_size; i!=0; i--, pf=(Nf32*)( (NBYTE*)pf+stridetonext ) )
			{
				*pf = NCLAMP(newmin,*pf,newmax);
				pf[1] = NCLAMP(newmin,pf[1],newmax);
			}
			return;

		case 3:
			for( i=sequence_size; i!=0; i--, pf=(Nf32*)( (NBYTE*)pf+stridetonext ) )
			{
				*pf = NCLAMP(newmin,*pf,newmax);
				pf[1] = NCLAMP(newmin,pf[1],newmax);
				pf[2] = NCLAMP(newmin,pf[2],newmax);
			}
			return;

		case 4:
			for( i=sequence_size; i!=0; i--, pf=(Nf32*)( (NBYTE*)pf+stridetonext ) )
			{
				*pf = NCLAMP(newmin,*pf,newmax);
				pf[1] = NCLAMP(newmin,pf[1],newmax);
				pf[2] = NCLAMP(newmin,pf[2],newmax);
				pf[3] = NCLAMP(newmin,pf[3],newmax);
			}
			return;

		default:
/*
			// #1:
			for( i=sequence_size; i!=0; i--, pf=(Nf32*)( (NBYTE*)pf+stridetonext ) )
			{
				for(j=0;j<delta;j++)
				{
					pf[j] = NCLAMP(newmin,pf[j],newmax);
				}
			}
*/
			// #2: This way number of loops is suppose to be lower than the method #1
			for(j=pfproc->Core.TargetId0;j<=pfproc->Core.TargetId1;j++)
			{
				pf = &pdstsequence_firstnf32set[j];
				for( i=sequence_size; i!=0; i--, pf=(Nf32*)( (NBYTE*)pf+stridetonext ) )
				{
					*pf = NCLAMP(newmin,*pf,newmax);
				}
			}

			return;
	}
}

void NUT_Nf32_FilterClamp_Clear(NUT_Nf32_PROCESS *pfproc)
{
	NUT_ClearNf32ProcessCore((NUT_Nf32_PROCESS_CORE*)pfproc);
	pfproc->Clamp.Min	= 0;
	pfproc->Clamp.Max	= 0;
}

void NUT_BindNf32_FilterClamp(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nf32 min, const Nf32 max )
{
	NUT_Nf32_PROCESS *pfproc	= NUT_GetNf32Process(targetid0,targetid1,pfsetbldr,NFALSE);

	// ... and setup it as a from LERP Float builder.
	pfproc->Core.Start	= NUT_Nf32_FilterClamp_Start;
	pfproc->Core.Update	= NUT_Nf32_FilterClamp_Update;
	pfproc->Core.Clear	= NUT_Nf32_FilterClamp_Clear;
	pfproc->Clamp.Min	= min;
	pfproc->Clamp.Max	= max;
}


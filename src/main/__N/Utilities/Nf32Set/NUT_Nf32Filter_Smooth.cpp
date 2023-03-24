#include "../../NErrorHandling.h"
#include "NUT_Nf32Set.h"

Nu32 NUT_Nf32_FilterSmooth_Start(NUT_Nf32_PROCESS *pfproc,const Nu32 sequence_size)
{
	return sequence_size + 2*pfproc->Smooth.Range;
}

void NUT_Nf32_FilterSmooth_Update(NUT_Nf32_PROCESS *pfproc,Nf32 *pdstsequence_firstnf32set, const Nu32 stridetonext, const Nu32 sequence_size)
{
/*
	NErrorIf(sequence_size < 2 ,NERROR_NUT_Nf32SET_SEQUENCE_SIZE_TOO_SHORT);

	Nf32 *pf = &pdstsequence_firstnf32set[pfproc->Core.TargetId0];
	Nu8 delta = 1 + pfproc->Core.TargetId1 -  pfproc->Core.TargetId0;
	NUT_Nf32_SCOPE	*pscope = &pfproc->Core.pScope[pfproc->Core.TargetId0];
	Nu32 i,j;
	Nf32 r;
*/


	Nu32	i;
	Nf32	*pf;
	Nf32	inv_coefsum;
	Nf32	*pdata,*pd,*prevpd;
	Nu16	p;
	Nf32	tmp;
	Nu8		src;
	pdata = pfproc->Core.pSharedBuffer;
	inv_coefsum = 1.0f/( (Nf32)pfproc->Smooth.Range*2.0f+1.0f );

	for( src=pfproc->Core.TargetId0;src<=pfproc->Core.TargetId1;src++ )
	{
		for(p=pfproc->Smooth.Pass;p!=0;p--)
		{
			pf = &pdstsequence_firstnf32set[src];
			
			pd = pdata;
			// All divided in one pass !
			// 0) The first
			*pd = tmp = *pf*inv_coefsum; 
			prevpd = pd;
			pd ++;
			// 1) The left "out of range" part ! 
			for(i=pfproc->Smooth.Range-1;i!=0;i--,pd++,prevpd++)
			{
				*pd = *prevpd + tmp;	
			}
			// 2) The main part ! 
			for( i=sequence_size; i!=0; i--, pd++, prevpd++,pf=(Nf32*)( (NBYTE*)pf+stridetonext ) )
			{
				*pd = *prevpd + *pf*inv_coefsum;
			}
			// 3) The right "out of range" part ! 
			tmp = *prevpd - *(prevpd-1);
			for(i=pfproc->Smooth.Range;i!=0;i--,pd++,prevpd++)
			{
				*pd = *prevpd + tmp;	
			}
			// 4a)Final pass -COPY DATA from buffer to Nf32Set- : start !
			pf		= &pdstsequence_firstnf32set[src];
			prevpd  = pdata;
			pd		= &pdata[2*pfproc->Smooth.Range];
			*pf		= *pd;
			// 4b)Final pass-COPY DATA from buffer to Nf32Set- : loop !
			pf=(Nf32*)( (NBYTE*)pf+stridetonext );
			pd++;
			for( i=sequence_size-1;i!=0;i--, pd++, prevpd++, pf=(Nf32*)( (NBYTE*)pf+stridetonext ) )
			{
				*pf = *pd - *prevpd;
			}
		}
	}
}

void NUT_Nf32_FilterSmooth_Clear(NUT_Nf32_PROCESS *pfproc)
{
	NUT_ClearNf32ProcessCore((NUT_Nf32_PROCESS_CORE*)pfproc);
	pfproc->Smooth.Range= 0;
	pfproc->Smooth.Pass	= 0;
}

void NUT_BindNf32_FilterSmooth(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nu16 range, const Nu16 pass)
{
	NUT_Nf32_PROCESS *pfproc	= NUT_GetNf32Process(targetid0,targetid1,pfsetbldr,NFALSE);

	// ... and setup it as a from LERP Float builder.
	pfproc->Core.Start	= NUT_Nf32_FilterSmooth_Start;
	pfproc->Core.Update	= NUT_Nf32_FilterSmooth_Update;
	pfproc->Core.Clear	= NUT_Nf32_FilterSmooth_Clear;
	pfproc->Smooth.Range= range;
	pfproc->Smooth.Pass	= pass;
}


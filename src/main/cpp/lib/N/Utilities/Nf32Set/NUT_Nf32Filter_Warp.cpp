#include "../../NErrorHandling.h"
#include "NUT_Nf32Set.h"

Nu32 NUT_Nf32_FilterWarp_Start(NUT_Nf32_PROCESS *pfproc,const Nu32 sequence_size)
{
	return 0;
}

void NUT_Nf32_FilterWarp_Update(NUT_Nf32_PROCESS *pfproc,Nf32 *pdstsequence_firstnf32set, const Nu32 stridetonext, const Nu32 sequence_size)
{
	Nu32	i;
	Nf32	*pf;
	Nu8		src;
	Nf32	curmin,curmax;
	Nf32	ratio;

	for( src=pfproc->Core.TargetId0;src<=pfproc->Core.TargetId1;src++ )
	{
		pf = &pdstsequence_firstnf32set[src];
		
		// Find LUT current min and max
		curmin = curmax = *pf;
		for( i=sequence_size;i!=0;i--,pf=(Nf32*)( (NBYTE*)pf+stridetonext ) )
		{
			if(*pf<curmin)
				curmin = *pf;
			else if(*pf>curmax)
				curmax = *pf;
		}

		// Incoming min/max values constraint
		ratio = (pfproc->Warp.Max - pfproc->Warp.Min)/(curmax-curmin);
		pf = &pdstsequence_firstnf32set[src];
		for( i=sequence_size;i!=0;i--,pf=(Nf32*)( (NBYTE*)pf+stridetonext ) )
		{
			*pf = pfproc->Warp.Min + (*pf-curmin)*ratio;
		}
	}
}

void NUT_Nf32_FilterWarp_Clear(NUT_Nf32_PROCESS *pfproc)
{
	NUT_ClearNf32ProcessCore((NUT_Nf32_PROCESS_CORE*)pfproc);
	pfproc->Warp.Min	= 0;
	pfproc->Warp.Max	= 0;
}

void NUT_BindNf32_FilterWarp(const Nu8 targetid0,const Nu8 targetid1, NUT_Nf32SET_BUILDER *pfsetbldr, const Nf32 min, const Nf32 max )
{
	NUT_Nf32_PROCESS *pfproc	= NUT_GetNf32Process(targetid0,targetid1,pfsetbldr,NFALSE);

	// ... and setup it as a from LERP Float builder.
	pfproc->Core.Start	= NUT_Nf32_FilterWarp_Start;
	pfproc->Core.Update	= NUT_Nf32_FilterWarp_Update;
	pfproc->Core.Clear	= NUT_Nf32_FilterWarp_Clear;
	pfproc->Warp.Min	= min;
	pfproc->Warp.Max	= max;
}


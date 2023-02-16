#include "../NCStandard.h"
#include "../GL/Ngl.h"
#include "../Containers/NArray.h"
#include "../NViewport.h"
#include "NUT_X.h"

Nbool NUT_2DLineXSpline(const NVEC2 *plineA,const NVEC2 *plineB, const NSPLINE *pspline, NUT_2DSPLINE_XRESULT *pxr)
{

	Nu32				i,j;
	NVEC3			RA,RB;
	NSPLINEKNOT			*spa;
	NSPLINEKNOT			*spb =(NSPLINEKNOT*)NGetFirstArrayPtr(&pspline->KnotArray)+1;
	Nf32				fac = 1.0f/(Nf32)pspline->BezierAccuracy;
	Nf32				fr;	

	for(i=NGetArraySize(&pspline->KnotArray)-1;i!=0;i--,spb++)
	{
		spa = spb-1;
		RA = spa->Position;
		fr = fac;
		// Because of Nf32 imprecision it will be impossible for 'fr' to reach exactly '1.0f' by adding 'fac' several times.
		// So to avoid this problem we perform the last segment test by hand ! 
		for( j = pspline->BezierAccuracy-1; j!=0; j--,fr+=fac )
		{
			NCasteljauPoint3f32(&RB, fr, spa, spb );
			if( NUT_2DSegXLine((NVEC2 *)&RA,(NVEC2 *)&RB,plineA, plineB, (NUT_2DLINE_XRESULT*)pxr) )
			{
				pxr->pSplineKnot1						= spb;
				pxr->SegA_Position						= RA;
				pxr->SegB_Position						= RB;
				pxr->SegB_BezierParamCoord			= fr;
				pxr->SegA_BezierParamCoord			= fr-fac;
				return NTRUE;
			}
			RA = RB;
		}
		// The last one by hand to force using the exact last SplineKnot position.
		if( NUT_2DSegXLine( (NVEC2*)&RA,(NVEC2*)&spb->Position,plineA, plineB, (NUT_2DLINE_XRESULT*)pxr) )
		{
			pxr->pSplineKnot1						= spb;
			pxr->SegA_Position						= RA;
			pxr->SegB_Position						= spb->Position;
			pxr->SegB_BezierParamCoord			= 1.0f;
			pxr->SegA_BezierParamCoord			= 1.0f-fac;

			return NTRUE;
		}
	}
	return NFALSE;
}

Nbool NUT_2DVerticalLineXSpline(const NVEC2 *plineA, const NSPLINE *pspline, NUT_2DSPLINE_XRESULT *pxr)
{

	NSPLINEKNOT			*spa;
	NSPLINEKNOT			*spb= (NSPLINEKNOT*)NGetFirstArrayPtr(&pspline->KnotArray) + 1;
	Nf32				fac = 1.0f/(Nf32)pspline->BezierAccuracy;
	Nf32				fr;
	NVEC3			RA,RB;
	Nu32				i,j;

	for(i=NGetArraySize(&pspline->KnotArray)-1;i!=0;i--,spb++)
	{
		spa = spb-1;
		if( NUT_2DVerticalLineXBezierFastAABB(plineA,spa,spb) )
		{
			RA = spa->Position;
			fr = fac;
			
			// Because of Nf32 imprecision it will be impossible for 'fr' to reach exactly '1.0f' by adding 'fac' several times.
			// So to avoid this problem we perform the last segment test by hand ! 
			for(j=pspline->BezierAccuracy-1 ;j!=0;j--, fr+=fac )// pspline->BezierAccuracy-1 to let us testing the last one by hand ...
			{												
				NCasteljauPoint3f32(&RB,fr, spa, spb);
				if( NUT_2DSegXVerticalLine( (NVEC2*)&RA,(NVEC2*)&RB,plineA, (NUT_2DLINE_XRESULT*)pxr) )
				{
					pxr->pSplineKnot1						= spb;
					pxr->SegA_Position						= RA;
					pxr->SegB_Position						= RB;
					pxr->SegB_BezierParamCoord			= fr;
					pxr->SegA_BezierParamCoord			= fr-fac;

					return NTRUE;
				}
				RA = RB;
			}
			// The last one by hand to force using the exact last SplineKnot position.
			if( NUT_2DSegXVerticalLine( (NVEC2*)&RA,(NVEC2*)&spb->Position,plineA, (NUT_2DLINE_XRESULT*)pxr) )
			{
				pxr->pSplineKnot1						= spb;
				pxr->SegA_Position						= RA;
				pxr->SegB_Position						= spb->Position;
				pxr->SegB_BezierParamCoord			= 1.0f;
				pxr->SegA_BezierParamCoord			= 1.0f-fac;

				return NTRUE;
			}
		}
	}
	return NFALSE;
}

Nbool NUT_2DVerticalLineXSplineExtended(const NVEC2 *plineA, NUT_2DSPLINE_PERSISTENT_XRESULT *ppersistent)
{
	Nf32				fr,frstart;	
	NVEC3			R0,R1;
	NSPLINEKNOT			*spa,*spb,*plimit;
	NVEC3			*prefpos;



	Nf32 fac	= 1.0f/(Nf32)ppersistent->pSpline->BezierAccuracy;

	// Two possible ways to look for a result: Forward or backward from the previous detected intersection.
	// To know which way using:
	// FORWARD !
	if(plineA->x >= ppersistent->XResult.I.x)
	{
		frstart	= ppersistent->SegB_BezierParamCoord;//BezierRatio_1; 
		prefpos = &ppersistent->SegA_Position;//ReferencePosition_0;
		plimit	= (NSPLINEKNOT*)NGetLastArrayPtr(&ppersistent->pSpline->KnotArray);

		for(spb=ppersistent->pSplineKnot1;spb <= plimit; spb++)
		{
			spa = spb-1;

			if( NUT_2DVerticalLineXBezierFastAABB(plineA,spa,spb) )
			{
				//NVec3Copy(&RA,&spa->Position);
				R0 = *prefpos;
				for( fr = frstart;fr<(1.0f+fac);fr+=fac ) // (fr<1.0f+fac) instead of (fr<=1.0f) , because float imprecision ! 
				{
					NCasteljauPoint3f32(&R1,fr, spa, spb);
					if( NUT_2DSegXVerticalLine( (NVEC2*)&R0,(NVEC2*)&R1, plineA, &ppersistent->XResult) )
					{
						ppersistent->pSplineKnot1				= spb;
						ppersistent->SegA_Position				= R0;
						ppersistent->SegB_Position				= R1;
						ppersistent->SegB_BezierParamCoord	= fr;
						ppersistent->SegA_BezierParamCoord	= fr-fac;
						return NTRUE;
					}
					R0=R1;
				}
			}
			frstart = fac;
			prefpos = &spb->Position; // Because it going to be spA next turn ... ! copy ?
			//prefpos = &RB;
		}
	}
	// BACKWARD !
	else
	{
		frstart	= ppersistent->SegA_BezierParamCoord; 
		prefpos = &ppersistent->SegB_Position;
		plimit	= (NSPLINEKNOT*)NGetFirstArrayPtr(&ppersistent->pSpline->KnotArray);

		for(spb = ppersistent->pSplineKnot1;spb > plimit;spb--)
		{
			spa = spb-1;
			// 			Nu32 i = NGetArrayIndex(&ppersistent->pSpline->KnotArray,(NBYTE*)spa);
			// 			Nu32 j = NGetArrayIndex(&ppersistent->pSpline->KnotArray,(NBYTE*)spb);

			if( NUT_2DVerticalLineXBezierFastAABB(plineA,spa,spb) )
			{
				//NVec3Copy(&RA,&spa->Position);
				R1=*prefpos;
				for( fr = frstart; fr > -fac;fr-=fac )  // (fr>-fac) instead of (fr>=0.0f) , because float imprecision ! 
														// !! float imprecision makes a bug here !! fr reaches a value close to 0 but less of 0,
														// so the last value was rejected by the for test (fr>=0.0f)!
				{
					NCasteljauPoint3f32(&R0,fr, spa, spb);
					// NCasteljauPoint3f32(0.02f, spa, spb, &Rtest);same result, so bug isn't here dude !

					if( NUT_2DSegXVerticalLine( (NVEC2*)&R0,(NVEC2*)&R1,plineA, &ppersistent->XResult) )
					{
						ppersistent->pSplineKnot1				= spb;
						ppersistent->SegA_Position				= R0;
						ppersistent->SegB_Position				= R1;
						ppersistent->SegA_BezierParamCoord	= fr;
						ppersistent->SegB_BezierParamCoord	= fr+fac;
						return NTRUE;
					}
					R1 = R0;
				}
			}
			frstart = 1.0f-fac;
			prefpos = &spa->Position; // Because it going to be spA next turn ... ! copy ?
			//prefpos = &RA; // Because it going to be spA next turn ... ! copy ?
		}
	}
	return NFALSE;
}

Nbool NUT_2DHorizontalLineXSpline(const NVEC2 *plineA, const NSPLINE *pspline, NUT_2DSPLINE_XRESULT *pxr)
{

	NVEC3		RA,RB;
	NSPLINEKNOT		*spa;
	NSPLINEKNOT		*spb = (NSPLINEKNOT*)NGetFirstArrayPtr(&pspline->KnotArray) + 1;
	Nf32			fac	 = 1.0f/(Nf32)pspline->BezierAccuracy;
	Nf32			fr;	
	Nu32			i,j;
	
	for(i=NGetArraySize(&pspline->KnotArray)-1;i!=0;i--,spb++)
	{
		spa = spb-1;

		if( NUT_2DHorizontalLineXBezierFastAABB(plineA,spa,spb) )
		{
			RA = spa->Position;
			fr=fac;
			// Because of Nf32 imprecision it will be impossible for 'fr' to reach exactly '1.0f' by adding 'fac' several times.
			// So to avoid this problem we perform the last segment test by hand ! 
			for(j=pspline->BezierAccuracy-1 ;j!=0;j--, fr+=fac )// pspline->BezierAccuracy-1 to let us testing the last one by hand ...
			{
				NCasteljauPoint3f32(&RB,fr, spa, spb);
				if( NUT_2DSegXHorizontalLine( (NVEC2*)&RA,(NVEC2*)&RB,plineA, (NUT_2DLINE_XRESULT*)pxr) )
				{
					pxr->pSplineKnot1						= spb;
					pxr->SegA_Position						= RA;
					pxr->SegB_Position						= RB;
					pxr->SegB_BezierParamCoord			= fr;
					pxr->SegA_BezierParamCoord			= fr-fac;

					return NTRUE;
				}
				RA = RB;
			}
			// The last one by hand to force using the exact last SplineKnot position.
			if( NUT_2DSegXHorizontalLine( (NVEC2*)&RA,(NVEC2*)&spb->Position,plineA, (NUT_2DLINE_XRESULT*)pxr) )
			{
				pxr->pSplineKnot1						= spb;
				pxr->SegA_Position						= RA;
				pxr->SegB_Position						= spb->Position;
				pxr->SegB_BezierParamCoord			= 1.0f;
				pxr->SegA_BezierParamCoord			= 1.0f-fac;

				return NTRUE;
			}

		}
	}
	return NFALSE;
}

Nbool NUT_2DHorizontalLineXSplineExtended(const NVEC2 *plineA, NUT_2DSPLINE_PERSISTENT_XRESULT *ppersistent)
{
	Nf32				fr,fac,frstart;	
	NVEC3			R0,R1;
	NSPLINEKNOT			*spa,*spb,*plimit;
	NVEC3			*prefpos;



	fac		= 1.0f/(Nf32)ppersistent->pSpline->BezierAccuracy;

	// Two possible ways to look for a result: Forward or backward from the previous detected intersection.
	// To know which way using:
	// FORWARD !
	if(plineA->y >= ppersistent->XResult.I.y)
	{
		frstart	= ppersistent->SegB_BezierParamCoord; 
		prefpos = &ppersistent->SegA_Position;
		plimit	= (NSPLINEKNOT*)NGetLastArrayPtr(&ppersistent->pSpline->KnotArray);

		for(spb=ppersistent->pSplineKnot1;spb <= plimit; spb++)
		{
			spa = spb-1;

			if( NUT_2DHorizontalLineXBezierFastAABB(plineA,spa,spb) )
			{
				
				R0=*prefpos;
				for( fr = frstart;fr<(1.0f+fac);fr+=fac ) // (fr<1.0f+fac) instead of (fr<=1.0f) , because float imprecision ! 
				{
					NCasteljauPoint3f32(&R1,fr, spa, spb);
					if( NUT_2DSegXHorizontalLine( (NVEC2*)&R0,(NVEC2*)&R1, plineA, &ppersistent->XResult) )
					{
						ppersistent->pSplineKnot1				= spb;
						ppersistent->SegA_Position				= R0;
						ppersistent->SegB_Position				= R1;
						ppersistent->SegB_BezierParamCoord	= fr;
						ppersistent->SegA_BezierParamCoord	= fr-fac;
						return NTRUE;
					}
					R0 = R1;
				}
			}
			frstart = fac;
			prefpos = &spb->Position; // Because it going to be spA next turn ... ! copy ?
			//prefpos = &RB;
		}
	}
	// BACKWARD !
	else
	{
		frstart	= ppersistent->SegA_BezierParamCoord; 
		prefpos = &ppersistent->SegB_Position;
		plimit	= (NSPLINEKNOT*)NGetFirstArrayPtr(&ppersistent->pSpline->KnotArray);

		for(spb = ppersistent->pSplineKnot1;spb > plimit;spb--)
		{
			spa = spb-1;
			// 			Nu32 i = NGetArrayIndex(&ppersistent->pSpline->KnotArray,(NBYTE*)spa);
			// 			Nu32 j = NGetArrayIndex(&ppersistent->pSpline->KnotArray,(NBYTE*)spb);

			if( NUT_2DHorizontalLineXBezierFastAABB(plineA,spa,spb) )
			{
				//NVec3Copy(&RA,&spa->Position);
				R1 = *prefpos;
				for( fr = frstart; fr > -fac;fr-=fac )  // (fr>-fac) instead of (fr>=0.0f) , because float imprecision ! 
					// !! float imprecision makes a bug here !! fr reaches a value close to 0 but less of 0,
					// so the last value was rejected by the for test (fr>=0.0f)!
				{
					NCasteljauPoint3f32(&R0,fr, spa, spb);
					// NCasteljauPoint3f32(0.02f, spa, spb, &Rtest);same result, so bug isn't here dude !

					if( NUT_2DSegXHorizontalLine( (NVEC2*)&R0,(NVEC2*)&R1,plineA, &ppersistent->XResult) )
					{
						ppersistent->pSplineKnot1				= spb;
						ppersistent->SegA_Position				= R0;
						ppersistent->SegB_Position				= R1;
						ppersistent->SegA_BezierParamCoord	= fr;
						ppersistent->SegB_BezierParamCoord	= fr+fac;
						return NTRUE;
					}
					R1 = R0;
				}
			}
			frstart = 1.0f-fac;
			prefpos = &spa->Position; // Because it going to be spA next turn ... ! copy ?
			//prefpos = &RA; // Because it going to be spA next turn ... ! copy ?
		}
	}
	return NFALSE;
}

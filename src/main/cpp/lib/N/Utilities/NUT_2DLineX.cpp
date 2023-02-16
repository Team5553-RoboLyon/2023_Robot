#include "../NCStandard.h"
#include "../GL/Ngl.h"

#include "../Containers/NArray.h"
#include "../NViewport.h"
#include "NUT_X.h"

Nbool NUT_2DLineXLine( const NVEC2 *A, const NVEC2 *B, const NVEC2 *C, const NVEC2 *D, NUT_2DLINE_XRESULT *pxr)
{
	Nf32		d,t;
	NVEC2	AB,AC,CD;

	// AB Cross CD ?
	// ==========================
	//				D
	//		 	   |
	//			  |		
	//		 	 |		
	//		 A ..+...........B
	//		    |
	//		   |
	//		  |
	//		 C

	AB.x = B->x - A->x;
	AB.y = B->y - A->y;
	CD.x = D->x - C->x;
	CD.y = D->y - C->y;

	d = AB.x*CD.y - CD.x*AB.y;
	if(d==0.0f) // or maybe <NFLOAT_EPSILON
		return NFALSE;

	if(pxr)
	{
		AC.x = C->x - A->x;
		AC.y = C->y - A->y;
		t = AC.x*CD.y - AC.y*CD.x;
		t/=d;

		pxr->I.x			= A->x + (t*AB.x);
		pxr->I.y			= A->y + (t*AB.y);
		pxr->ParamCoordAB	= t;
	}

	return NTRUE;
}

Nbool NUT_2DLineXLine_VDir( const NVEC2 *A, const NVEC2 *ADir, const NVEC2 *C, const NVEC2 *CDir, NUT_2DLINE_XRESULT *pxr)
{
	Nf32		d,t;
	NVEC2	AC;

	// A,ADir Cross C,CDir ?
	// ==========================
	//				^ CDir
	//		 	   |
	//			  |		
	//		 	 |		
	//		 A ..+...........> ADir
	//		    |
	//		   |
	//		  |
	//		 C
/*
	AB.x = B->x - A->x;
	AB.y = B->y - A->y;
	CD.x = D->x - C->x;
	CD.y = D->y - C->y;
*/
	d = ADir->x*CDir->y - CDir->x*ADir->y;
	if(d==0.0f) // or maybe <NFLOAT_EPSILON
		return NFALSE;

	if(pxr)
	{
		AC.x = C->x - A->x;
		AC.y = C->y - A->y;
		t = AC.x*CDir->y - AC.y*CDir->x;
		t/=d;

		pxr->I.x			= A->x + (t*ADir->x);
		pxr->I.y			= A->y + (t*ADir->y);
		pxr->ParamCoordAB	= t;
	}

	return NTRUE;
}


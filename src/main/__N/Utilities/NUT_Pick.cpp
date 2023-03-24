#include "../NCStandard.h"
#include "../GL/Ngl.h"
#include "../Containers/NArray.h"
#include "../NViewport.h"

#include "NUT_3DMouse.h"
#include "NUT_Pick.h"


/*
// ------------------------------------------------------------------------------------------
// NUT_Pick3DPoint
// ------------------------------------------------------------------------------------------
// Description :
//		Check if the screen projection of a 3D point is "close enough" to a 2D point
//		Useful to pick a 3D vertex with mouse pointer.
//		This function calls "NUT_Get3DMouseRay" at its start to get the 3D ray used to perform
//		the tests.
//		Prefer using "NUT_Pick3DPointEx" if you have to test several 3D points with the same camera.
// ------------------------------------------------------------------------------------------
// In  :
//			pmouse2d:		a valid pointer on a 2d screen point structure.
//			pickradius:		maximum distance (3d) accepted between "pv" and the line
//							which starts from the camera position and pass trough screen plane at the "pmouse2d" 2d point.
//			pv:				a valid pointer on a NVEC3 (3coords of a 3D points)
// Out :
//			a positive Nf32 < pickradius if pv is close enough or -1 if is not. 		
// ------------------------------------------------------------------------------------------
Nf32 NUT_Pick3DPoint(const NVEC2s16 *pmouse2d,const Nf32 pickradius,const NVEC3 *pv)
{

	NVEC3	vdir;
	NVEC3	OA,OP,PA;
	NVEC3	O,A,P;
	Nf32		fdot;
	Nf32		fdist;

	NUT_Get3DMouseRay(&O,&vdir,NULL,pmouse2d);
	NVec3Normalize(&vdir);

	NVec3Copy(&A,pv);
	NVec3Sub(&OA,&A,&O);
	fdot = NVec3DotProduct(&vdir,&OA);
	NVec3Scale(&OP,&vdir,fdot);
	NVec3Add(&P,&OP,&O);
	NVec3Sub(&PA,&A,&P);
	fdist = NVec3FastNormalize(&PA); //fdist = NVec3Normalize(&u); 
	if(fdist <= pickradius)
		return fdist;

	return -1.0f;
}

// ------------------------------------------------------------------------------------------
// NUT_Pick3DPointEx
// ------------------------------------------------------------------------------------------
// Description :
//		Check if a 3D point is "close enough" to a 3D line.
//		Use "NUT_Get3DMouseRay" to get "prayorigin" and "prayvdir" from a 2D screen point.
// ------------------------------------------------------------------------------------------
// In  :
//			prayorigin:		a valid pointer on a 3d point, which the line pass trough. 
//			prayvdir:		a valid pointer on a 3d normalized vector, which represent the direction of the line.
//			pickradius:		maximum distance (3d) accepted between "pv" and the line
//							which starts from the camera position and pass trough screen plane at the "pmouse2d" 2d point.
//			pv:				a valid pointer on a NVEC3 (3coords of a 3D points)
// Out :
//			a positive Nf32 <= pickradius if pv is close enough or -1 if is not. 		
// ------------------------------------------------------------------------------------------
Nf32 NUT_Pick3DPointEx(const NVEC3 *pray_origin,const NVEC3 *pray_vnormdir,const Nf32 pickradius,const NVEC3 *pA)
{
	NErrorIf( NABS(1.0f-NVec3Length(pray_vnormdir))>NF32_EPSILON_VECTOR_LENGTH,NERROR_NON_UNIT_VECTOR );
	
	NVEC3	OA,OP,PA;
	NVEC3	P;
	Nf32		fdot;
	Nf32		fdist;
	
	NVec3Sub(&OA,pA,pray_origin);
	fdot = NVec3DotProduct(pray_vnormdir,&OA);
	NVec3Scale(&OP,pray_vnormdir,fdot);
	NVec3Add(&P,&OP,pray_origin);
	NVec3Sub(&PA,pA,&P);
	fdist = NVec3FastNormalize(&PA); //fdist = NVec3Normalize(&u); 
	if(fdist <= pickradius)
		return fdist;

	return -1.0f;
}
// ------------------------------------------------------------------------------------------
// NUT_Pick3DPointEx_SquareDist
// ------------------------------------------------------------------------------------------
// Description :
//		Check if a 3D point is "close enough" to a 3D line.
//		This ALgorythm is slight bit different (and more efficient) than "NUT_Pick3DPointEx"
//		because it uses square distance instead of distance to manage.
//		Use "NUT_Get3DMouseRay" to get "prayorigin" and "prayvdir" from a 2D screen point.
// ------------------------------------------------------------------------------------------
// In  :
//			prayorigin:			a valid pointer on a 3d point, which the line pass trough. 
//			prayvdir:			a valid pointer on a 3d vector, which represent the direction of the line.
//			picksquareradius:	maximum "SQUARE distance" (3d) accepted between "pv" and the line
//								which starts from the camera position and pass trough screen plane at the "pmouse2d" 2d point.
//			pv:					a valid pointer on a NVEC3 (3coords of a 3D points)
// Out :
//			a positive Nf32 < picksquareradius if pv is close enough or -1 if is not. 		
// ------------------------------------------------------------------------------------------
Nf32 NUT_Pick3DPointEx_SquareDist(const NVEC3 *pray_origin,const NVEC3 *pray_vnormdir,const Nf32 picksquareradius,const NVEC3 *pA)
{
	NErrorIf( NABS(1.0f-NVec3Length(pray_vnormdir))>NF32_EPSILON_VECTOR_LENGTH,NERROR_NON_UNIT_VECTOR );

	NVEC3	OA,OP,PA;
	NVEC3	P;
	Nf32		fdot;
	Nf32		fsquaredist;


	NVec3Sub(&OA,pA,pray_origin);
	fdot = NVec3DotProduct(pray_vnormdir,&OA);
	NVec3Scale(&OP,pray_vnormdir,fdot);
	NVec3Add(&P,&OP,pray_origin);
	NVec3Sub(&PA,pA,&P);
	fsquaredist = NVec3SquareLength(&PA);
	if( fsquaredist <= picksquareradius )
		return fsquaredist;

	return -1.0f;
}
*/
// ------------------------------------------------------------------------------------------
// NUT_Pick3DSpline
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
Nbool NUT_Pick3DSpline(NUT_PICKSPLINE *presult, const NVEC2s16 *pmouse2d,const Nf32 pickradius, const NSPLINE *pspline)
{
	NVEC3		vdir;
	NVEC3		OA,OP,PA;
	NVEC3		O,A,P;
	Nf32			fdot;
	Nf32			fdist;

	Nu32			i,j,bindex;
	NSPLINEKNOT	*spa,*spb;
	NVEC3		L1,L2,L3,L4,L5;
	NVEC3		RA;
	NVEC3		mta,mtb;
	NVEC3		mtp;
	Nf32			fr,fac;
	Nf32			fmin,factor;
	Nf32			fmdot;

	NUT_Get3DMouseRay(&O,&vdir,NULL,pmouse2d);
	NVec3Normalize(&vdir);

	// Test all the spline points
	// FIND nearest point in the "CURVE"
	factor	= -1.0f;
	fmdot	= 0.0f;
	fmin	= pickradius;

	spb =(NSPLINEKNOT*)NGetFirstArrayPtr(&pspline->KnotArray);
	spb++;
	for(i=1;i<NGetArraySize(&pspline->KnotArray);i++,spb++)
	{
		spa = spb-1;

		fr = fac = 1.0f/(Nf32)pspline->BezierAccuracy;
		for( j = 0; j<=pspline->BezierAccuracy; j++,fr+=fac )
		{
			NVec3Lerp(&L1,&spa->Position,&spa->TB,fr);
			NVec3Lerp(&L2,&spa->TB,&spb->TA,fr);
			NVec3Lerp(&L3,&spb->TA,&spb->Position,fr);

			NVec3Lerp(&L4,&L1,&L2,fr);
			NVec3Lerp(&L5,&L2,&L3,fr);

			NVec3Lerp(&RA,&L4,&L5,fr);

			// Distance from RA to MouseRay:
			A = RA;

			NVec3Sub(&OA,&A,&O);
			fdot = NVec3DotProduct(&vdir,&OA);
			NVec3Scale(&OP,&vdir,fdot);
			NVec3Add(&P,&OP,&O);
			NVec3Sub(&PA,&A,&P);
			fdist = NVec3FastNormalize(&PA); //fdist = NVec3Normalize(&u); 

			if( fdist<fmin )
			{
				fmin = fdist;
				factor = fr;
				fmdot = fdot;
				bindex=i;
				mtp = RA;

				// try to define/estimate TA and TB at this point ...
				mta = L4;
				mtb = L5;
			}
		}
	}

	if( factor != -1.0f  )
	{
		presult->BSplinePointIndex = bindex;
		presult->LerpFactor = factor;
		presult->fDot		= fmdot;
		presult->PickedPosition = mtp;
		presult->EstimatedTA = mta;
		presult->EstimatedTB = mtb;

		return NTRUE;
	}

	return NFALSE;
}

// ------------------------------------------------------------------------------------------
// NUT_PickVector3Array
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//		'NVOID' if there is no close enough element.
//		or 
//		The index of the closest element if there is one. 
//
//		note:  0 < index < NArraySize(pv3array)
//
// ------------------------------------------------------------------------------------------
Nu32	NUT_PickVector3Array(NUT_PICKV3ARRAY *presult, const NVEC2s16 *pmouse2d, const Nf32 pickradius, const NARRAY *pv3array)
{
	NVEC3		vdir;
	NVEC3		OA,OP,PA;
	NVEC3		O,P;
	Nf32			fdist;

	Nu32			i,index;
	Nf32			fmin;
	Nf32			fdot;
	NVEC3		*pv;
	NVEC3		*pvresult;
	Nbool			bresult;

	NUT_Get3DMouseRay(&O,&vdir,NULL,pmouse2d);
	NVec3Normalize(&vdir);

	// Test all the V3 Array Elements
	// FIND the closest one with the 3DMouseRay
	fmin		= pickradius;
	index		= NVOID;
	pvresult	= NULL;
	bresult		= NFALSE;

	pv =(NVEC3*)NGetFirstArrayPtr(pv3array);
	for(i=0;i<NGetArraySize(pv3array);i++,pv++)
	{
		NVec3Sub(&OA,pv,&O);
		fdot = NVec3DotProduct(&vdir,&OA);
		NVec3Scale(&OP,&vdir,fdot);
		NVec3Add(&P,&OP,&O);
		NVec3Sub(&PA,pv,&P);
		fdist = NVec3Length(&PA);
		
		if( fdist<fmin )
		{
			fmin		= fdist;
			index		= i;
			pvresult	= pv;
			bresult		= NTRUE;
		}
	}

	if(presult)
	{
		presult->PickingDistance	= fmin;
		presult->PickedElementIndex = index;
		presult->pPickedV3Element	= pvresult;
	}
	return bresult;
}


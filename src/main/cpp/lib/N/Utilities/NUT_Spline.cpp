#include "lib/N/NCStandard.h"
#include "lib/N/GL/Ngl.h"

#include "../Containers/NArray.h"
#include "../NViewport.h"
#include "NUT_Spline.h"
#include "NUT_X.h"

// Debug code

NSPLINE *NUT_CreatePlanarSpline(const NUT_SPLINEMODEL splinemodel, const Nu32 accuracy, const Nf32 scalex, const Nf32 scaley)
{
	NSPLINEKNOTDESC desc[3]; // 4 because, at maximum we need 3 ! It can be increase for new SPLINEMODEL if they need more.
	NSPLINE *pspline;

	pspline = NULL;
	switch (splinemodel)
	{
	case NUT_SPLINEMODEL_DECREASE_ACCELERATED:
		desc[0].x = 0.0f;
		desc[1].x = scalex;
		desc[0].y = scaley;
		desc[1].y = 0.0f;
		desc[0].z = 0.0f;
		desc[1].z = 0.0f;
		// 			desc[0].TBuildCode = NSKTANGENTSPACK( NSKT_NULL, NSKT_WORLDR_XALIGN_P1of2 );
		// 			desc[1].TBuildCode = NSKTANGENTSPACK( NSKT_WORLDR_YALIGN_P1of2,	NSKT_NULL );
		desc[0].TBuildCode = NSKTANGENTSPACKI(0, 0, 0, 0, NSKT_WORLD_NRX, NSKT_P1o2, 0, 0);
		desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRY, 0, NSKT_P1o2, 0, 0, 0, 0, 0);
		pspline = NCreateSpline(accuracy, 2, desc);
		break;

	case NUT_SPLINEMODEL_DECREASE_DECELERATED:
		desc[0].x = 0.0f;
		desc[1].x = scalex;
		desc[0].y = scaley;
		desc[1].y = 0.0f;
		desc[0].z = 0.0f;
		desc[1].z = 0.0f;
		// 			desc[0].TBuildCode = NSKTANGENTSPACK( NSKT_NULL, NSKT_WORLDR_YALIGN_M1of2 );
		// 			desc[1].TBuildCode = NSKTANGENTSPACK( NSKT_WORLDR_XALIGN_M1of2,	NSKT_NULL );
		desc[0].TBuildCode = NSKTANGENTSPACKI(0, 0, 0, 0, NSKT_WORLD_NRY, NSKT_M1o2, 0, 0);
		desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRX, NSKT_M1o2, 0, 0, 0, 0, 0, 0);
		pspline = NCreateSpline(accuracy, 2, desc);
		break;

	case NUT_SPLINEMODEL_DECREASE_LINEAR:
		desc[0].x = 0.0f;
		desc[1].x = scalex;
		desc[0].y = scaley;
		desc[1].y = 0.0f;
		desc[0].z = 0.0f;
		desc[1].z = 0.0f;
		desc[0].TBuildCode = NSKTANGENTSPACKI(0, 0, 0, 0, NSKT_LOCAL_NRL, 0, 0, NSKT_P1o3);
		desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_LOCAL_NRL, 0, 0, NSKT_M1o3, 0, 0, 0, 0);
		//			or (same result but an another way to achieve ...)
		//			desc[0].TBuildCode = NSKTANGENTSPACKI(NSKT_3DWORLD_RELATIVE, NSKT0,NSKT0,NSKT0,	NSKT_3DWORLD_RELATIVE,NSKT_P1o3,NSKT_M1o3,NSKT0	);
		//			desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_3DWORLD_RELATIVE, NSKT_M1o3,NSKT_P1o3,NSKT0,	NSKT_3DWORLD_RELATIVE,NSKT0,NSKT0,NSKT0	);
		pspline = NCreateSpline(accuracy, 2, desc);
		break;

	case NUT_SPLINEMODEL_DECREASE_SMOOTH:
		desc[0].x = 0.0f;
		desc[1].x = scalex;
		desc[0].y = scaley;
		desc[1].y = 0.0f;
		desc[0].z = 0.0f;
		desc[1].z = 0.0f;
		desc[0].TBuildCode = NSKTANGENTSPACKI(0, 0, 0, 0, NSKT_WORLD_NRX, NSKT_P1o2, 0, 0);
		desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRX, NSKT_M1o2, 0, 0, 0, 0, 0, 0);
		// 			desc[0].TBuildCode = NSKTANGENTSPACK( NSKT_NULL,	NSKT_WORLDR_XALIGN_P1of2 );
		// 			desc[1].TBuildCode = NSKTANGENTSPACK( NSKT_WORLDR_XALIGN_M1of2, NSKT_NULL );
		pspline = NCreateSpline(accuracy, 2, desc);
		break;

	case NUT_SPLINEMODEL_DECREASE_EARLY:
		desc[0].x = 0.0f;
		desc[1].x = scalex;
		desc[0].y = scaley;
		desc[1].y = 0.0f;
		desc[0].z = 0.0f;
		desc[1].z = 0.0f;
		desc[0].TBuildCode = NSKTANGENTSPACKI(0, 0, 0, 0, NSKT_WORLD_NRY, 0, NSKT_M1, 0);
		desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRX, NSKT_M1, 0, 0, 0, 0, 0, 0);
		// 			desc[0].TBuildCode = NSKTANGENTSPACK( NSKT_NULL, NSKT_WORLDR_YALIGN_M1 );
		// 			desc[1].TBuildCode = NSKTANGENTSPACK( NSKT_WORLDR_XALIGN_M1,	NSKT_NULL );
		pspline = NCreateSpline(accuracy, 2, desc);
		break;

	case NUT_SPLINEMODEL_DECREASE_LATE:
		desc[0].x = 0.0f;
		desc[1].x = scalex;
		desc[0].y = scaley;
		desc[1].y = 0.0f;
		desc[0].z = 0.0f;
		desc[1].z = 0.0f;
		desc[0].TBuildCode = NSKTANGENTSPACKI(0, 0, 0, 0, NSKT_WORLD_NRX, NSKT_P1, 0, 0);
		desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRY, 0, NSKT_P1, 0, 0, 0, 0, 0);
		// 			desc[0].TBuildCode = NSKTANGENTSPACK( NSKT_NULL, NSKT_WORLDR_XALIGN_P1 );
		// 			desc[1].TBuildCode = NSKTANGENTSPACK( NSKT_WORLDR_YALIGN_P1,	NSKT_NULL );
		pspline = NCreateSpline(accuracy, 2, desc);
		break;

	case NUT_SPLINEMODEL_INCREASE_ACCELERATED:
		desc[0].x = 0.0f;
		desc[1].x = scalex;
		desc[0].y = 0.0f;
		desc[1].y = scaley;
		desc[0].z = 0.0f;
		desc[1].z = 0.0f;
		desc[0].TBuildCode = NSKTANGENTSPACKI(0, 0, 0, 0, NSKT_WORLD_NRX, NSKT_P1o2, 0, 0);
		desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRY, 0, NSKT_M1o2, 0, 0, 0, 0, 0);
		// 			desc[0].TBuildCode = NSKTANGENTSPACK( NSKT_NULL,	NSKT_WORLDR_XALIGN_P1of2 );
		// 			desc[1].TBuildCode = NSKTANGENTSPACK( NSKT_WORLDR_YALIGN_M1of2, NSKT_NULL );
		pspline = NCreateSpline(accuracy, 2, desc);
		break;

	case NUT_SPLINEMODEL_INCREASE_DECELERATED:
		desc[0].x = 0.0f;
		desc[1].x = scalex;
		desc[0].y = 0.0f;
		desc[1].y = scaley;
		desc[0].z = 0.0f;
		desc[1].z = 0.0f;
		desc[0].TBuildCode = NSKTANGENTSPACKI(0, 0, 0, 0, NSKT_WORLD_NRY, 0, NSKT_P1o2, 0);
		desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRX, NSKT_M1o2, 0, 0, 0, 0, 0, 0);
		// 			desc[0].TBuildCode = NSKTANGENTSPACK( NSKT_NULL, NSKT_WORLDR_YALIGN_P1of2 );
		// 			desc[1].TBuildCode = NSKTANGENTSPACK( NSKT_WORLDR_XALIGN_M1of2, NSKT_NULL );
		pspline = NCreateSpline(accuracy, 2, desc);
		break;

	case NUT_SPLINEMODEL_INCREASE_LINEAR:
		desc[0].x = 0.0f;
		desc[1].x = scalex;
		desc[0].y = 0.0f;
		desc[1].y = scaley;
		desc[0].z = 0.0f;
		desc[1].z = 0.0f;
		desc[0].TBuildCode = NSKTANGENTSPACKI(0, 0, 0, 0, NSKT_LOCAL_NRL, 0, 0, NSKT_P1o3);
		desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_LOCAL_NRL, 0, 0, NSKT_M1o3, 0, 0, 0, 0);
		pspline = NCreateSpline(accuracy, 2, desc);
		break;

	case NUT_SPLINEMODEL_INCREASE_SMOOTH:
		desc[0].x = 0.0f;
		desc[1].x = scalex;
		desc[0].y = 0.0f;
		desc[1].y = scaley;
		desc[0].z = 0.0f;
		desc[1].z = 0.0f;
		desc[0].TBuildCode = NSKTANGENTSPACKI(0, 0, 0, 0, NSKT_WORLD_NRX, NSKT_P1o2, 0, 0);
		desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRX, NSKT_M1o2, 0, 0, 0, 0, 0, 0);
		// 			desc[0].TBuildCode = NSKTANGENTSPACK( NSKT_NULL, NSKT_WORLDR_XALIGN_P1of2 );
		// 			desc[1].TBuildCode = NSKTANGENTSPACK( NSKT_WORLDR_XALIGN_M1of2, NSKT_NULL );
		pspline = NCreateSpline(accuracy, 2, desc);
		break;

	case NUT_SPLINEMODEL_INCREASE_EARLY:
		desc[0].x = 0.0f;
		desc[1].x = scalex;
		desc[0].y = 0.0f;
		desc[1].y = scaley;
		desc[0].z = 0.0f;
		desc[1].z = 0.0f;
		desc[0].TBuildCode = NSKTANGENTSPACKI(0, 0, 0, 0, NSKT_WORLD_NRY, 0, NSKT_P1, 0);
		desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRX, NSKT_M1, 0, 0, 0, 0, 0, 0);
		// 			desc[0].TBuildCode = NSKTANGENTSPACK( NSKT_NULL, NSKT_WORLDR_YALIGN_P1 );
		// 			desc[1].TBuildCode = NSKTANGENTSPACK( NSKT_WORLDR_XALIGN_M1, NSKT_NULL );
		pspline = NCreateSpline(accuracy, 2, desc);
		break;

	case NUT_SPLINEMODEL_INCREASE_LATE:
		desc[0].x = 0.0f;
		desc[1].x = scalex;
		desc[0].y = 0.0f;
		desc[1].y = scaley;
		desc[0].z = 0.0f;
		desc[1].z = 0.0f;
		desc[0].TBuildCode = NSKTANGENTSPACKI(0, 0, 0, 0, NSKT_WORLD_NRX, NSKT_P1, 0, 0);
		desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRY, 0, NSKT_M1, 0, 0, 0, 0, 0);
		// 			desc[0].TBuildCode = NSKTANGENTSPACK( NSKT_NULL, NSKT_WORLDR_XALIGN_P1 );
		// 			desc[1].TBuildCode = NSKTANGENTSPACK( NSKT_WORLDR_XALIGN_M1, NSKT_NULL );
		pspline = NCreateSpline(accuracy, 2, desc);
		break;

	case NUT_SPLINEMODEL_BELL:
		desc[0].x = 0.0f;
		desc[1].x = scalex * 0.5f;
		desc[2].x = scalex;
		desc[0].y = 0.0f;
		desc[1].y = scaley;
		desc[2].y = 0.0f;
		desc[0].z = 0.0f;
		desc[1].z = 0.0f;
		desc[2].z = 0.0f;

		// 			desc[0].TBuildCode = NSKTANGENTSPACK(NSKT_NULL,					NSKT_WORLDR_XALIGN_P1 );
		// 			desc[1].TBuildCode = NSKTANGENTSPACK(NSKT_WORLDR_XALIGN_M1,		NSKT_WORLDR_XALIGN_P1 );
		// 			desc[2].TBuildCode = NSKTANGENTSPACK(NSKT_WORLDR_XALIGN_M1,		NSKT_NULL			  );

		desc[0].TBuildCode = NSKTANGENTSPACKI(0, 0, 0, 0, NSKT_WORLD_NRX, NSKT_P1, 0, 0);
		desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRX, NSKT_M1, 0, 0, NSKT_WORLD_NRX, NSKT_P1, 0, 0);
		desc[2].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRX, NSKT_M1, 0, 0, 0, 0, 0, 0);
		pspline = NCreateSpline(accuracy, 3, desc);
		break;

	case NUT_SPLINEMODEL_SHARPBELL:
		desc[0].x = 0.0f;
		desc[1].x = scalex * 0.5f;
		desc[2].x = scalex;
		desc[0].y = 0.0f;
		desc[1].y = scaley;
		desc[2].y = 0.0f;
		desc[0].z = 0.0f;
		desc[1].z = 0.0f;
		desc[2].z = 0.0f;

		// 			desc[0].TBuildCode = NSKTANGENTSPACK(NSKT_NULL,					NSKT_WORLDR_XALIGN_P1 );
		// 			desc[1].TBuildCode = NSKTANGENTSPACK(NSKT_WORLDR_XALIGN_M1,		NSKT_WORLDR_XALIGN_P1 );
		// 			desc[2].TBuildCode = NSKTANGENTSPACK(NSKT_WORLDR_XALIGN_M1,		NSKT_NULL			  );

		desc[0].TBuildCode = NSKTANGENTSPACKI(0, 0, 0, 0, NSKT_WORLD_NRX, NSKT_P1o2, 0, 0);
		desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRX, NSKT_M1o2, 0, 0, NSKT_WORLD_NRX, NSKT_P1o2, 0, 0);
		desc[2].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRX, NSKT_M1o2, 0, 0, 0, 0, 0, 0);
		pspline = NCreateSpline(accuracy, 3, desc);
		break;

	case NUT_SPLINEMODEL_DOME:
		desc[0].x = 0.0f;
		desc[1].x = scalex * 0.5f;
		desc[2].x = scalex;
		desc[0].y = 0.0f;
		desc[1].y = scaley;
		desc[2].y = 0.0f;
		desc[0].z = 0.0f;
		desc[1].z = 0.0f;
		desc[2].z = 0.0f;
		// 			desc[0].TBuildCode = NSKTANGENTSPACK(NSKT_NULL,				NSKT_WORLDR_YALIGN_P1of2);
		// 			desc[1].TBuildCode = NSKTANGENTSPACK(NSKT_WORLDR_XALIGN_M1,	NSKT_WORLDR_XALIGN_P1	);
		// 			desc[2].TBuildCode = NSKTANGENTSPACK(NSKT_WORLDR_YALIGN_P1of2,	NSKT_NULL				);

		desc[0].TBuildCode = NSKTANGENTSPACKI(0, 0, 0, 0, NSKT_WORLD_NRY, 0, NSKT_P1o2, 0);
		desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRX, NSKT_M1, 0, 0, NSKT_WORLD_NRX, NSKT_P1, 0, 0);
		desc[2].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRY, 0, NSKT_P1o2, 0, 0, 0, 0, 0);

		pspline = NCreateSpline(accuracy, 3, desc);
		break;

	case NUT_SPLINEMODEL_FLAT:
		desc[0].x = 0.0f;
		desc[1].x = scalex;
		desc[0].y = scaley;
		desc[1].y = scaley;
		desc[0].z = 0.0f;
		desc[1].z = 0.0f;
		desc[0].TBuildCode = NSKTANGENTSPACKI(0, 0, 0, 0, NSKT_WORLD_NRX, NSKT_P1o3, 0, 0);
		desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRX, NSKT_M1o3, 0, 0, 0, 0, 0, 0);
		// 			desc[0].TBuildCode = NSKTANGENTSPACK( NSKT_NULL, NSKT_WORLDR_XALIGN_P1of3 );// or NSKTANGENTSPACK( NSKT_NULL, NSKT_LOCALR_ZALIGN_P1of3 );
		// 			desc[1].TBuildCode = NSKTANGENTSPACK( NSKT_WORLDR_XALIGN_M1of3,	NSKT_NULL );// or NSKTANGENTSPACK( NSKT_LOCALR_ZALIGN_P1of3,	NSKT_NULL );
		pspline = NCreateSpline(accuracy, 2, desc);
		break;

	case NUT_SPLINEMODEL_PIC:
		desc[0].x = 0.0f;
		desc[1].x = scalex * 0.5f;
		desc[2].x = scalex;
		desc[0].y = 0.0f;
		desc[1].y = scaley;
		desc[2].y = 0.0f;
		desc[0].z = 0.0f;
		desc[1].z = 0.0f;
		desc[2].z = 0.0f;
		desc[0].TBuildCode = NSKTANGENTSPACKI(0, 0, 0, 0, NSKT_WORLD_NRX, NSKT_P1o2, 0, 0);
		desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRY, 0, NSKT_M1, 0, NSKT_WORLD_NRY, 0, NSKT_M1, 0);
		desc[2].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRX, NSKT_M1o2, 0, 0, 0, 0, 0, 0);
		// 			desc[0].TBuildCode = NSKTANGENTSPACK(NSKT_NULL,					NSKT_WORLDR_XALIGN_P1of2 );
		// 			desc[1].TBuildCode = NSKTANGENTSPACK(NSKT_WORLDR_YALIGN_M1,		NSKT_WORLDR_YALIGN_M1	 );
		// 			desc[2].TBuildCode = NSKTANGENTSPACK(NSKT_WORLDR_XALIGN_M1of2,	NSKT_NULL				 );
		pspline = NCreateSpline(accuracy, 3, desc);
		break;

	case NUT_SPLINEMODEL_SHARKFIN:
		desc[0].x = 0.0f;
		desc[1].x = scalex * 0.5f;
		desc[2].x = scalex;
		desc[0].y = 0.0f;
		desc[1].y = scaley;
		desc[2].y = 0.0f;
		desc[0].z = 0.0f;
		desc[1].z = 0.0f;
		desc[2].z = 0.0f;
		desc[0].TBuildCode = NSKTANGENTSPACKI(0, 0, 0, 0, NSKT_WORLD_NRX, NSKT_P1o2, 0, 0);
		desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRY, 0, NSKT_M1o2, 0, NSKT_WORLD_NRX, NSKT_P1, 0, 0);
		desc[2].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRX, NSKT_M1o2, 0, 0, 0, 0, 0, 0);

		// 			desc[0].TBuildCode = NSKTANGENTSPACK(NSKT_NULL,					NSKT_WORLDR_XALIGN_P1of2 );
		// 			desc[1].TBuildCode = NSKTANGENTSPACK(NSKT_WORLDR_YALIGN_M1of2,	NSKT_WORLDR_XALIGN_P1	 );
		// 			desc[2].TBuildCode = NSKTANGENTSPACK(NSKT_WORLDR_XALIGN_M1of2,	NSKT_NULL				 );
		pspline = NCreateSpline(accuracy, 3, desc);
		break;

	case NUT_SPLINEMODEL_FORWARD_WAVE:
		desc[0].x = 0.0f;
		desc[1].x = scalex * 0.75f;
		desc[2].x = scalex;
		desc[0].y = 0.0f;
		desc[1].y = scaley;
		desc[2].y = 0.0f;
		desc[0].z = 0.0f;
		desc[1].z = 0.0f;
		desc[2].z = 0.0f;
		desc[0].TBuildCode = NSKTANGENTSPACKI(0, 0, 0, 0, NSKT_WORLD_NRX, NSKT_P1, 0, 0);
		desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRX, NSKT_M1o3, 0, 0, NSKT_WORLD_NRX, NSKT_P1, 0, 0);
		desc[2].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRY, 0, NSKT_P1o4, 0, 0, 0, 0, 0);
		pspline = NCreateSpline(accuracy, 3, desc);
		break;

	case NUT_SPLINEMODEL_BACKWARD_WAVE:
		desc[0].x = 0.0f;
		desc[1].x = scalex * 0.25f;
		desc[2].x = scalex;
		desc[0].y = 0.0f;
		desc[1].y = scaley;
		desc[2].y = 0.0f;
		desc[0].z = 0.0f;
		desc[1].z = 0.0f;
		desc[2].z = 0.0f;
		desc[0].TBuildCode = NSKTANGENTSPACKI(0, 0, 0, 0, NSKT_WORLD_NRY, 0, NSKT_P1o4, 0);
		desc[1].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRX, NSKT_M1, 0, 0, NSKT_WORLD_NRX, NSKT_P1o3, 0, 0);
		desc[2].TBuildCode = NSKTANGENTSPACKI(NSKT_WORLD_NRX, NSKT_M1, 0, 0, 0, 0, 0, 0);
		pspline = NCreateSpline(accuracy, 3, desc);
		break;

	default:
		break;
	}

	return pspline;
}

// ------------------------------------------------------------------------------------------
// NUT_ExtractRegularlySpaced3DPointFromSpline
// ------------------------------------------------------------------------------------------
// Description :
//	Fill up a list of NVEC3 from a spline. First point will be at origin and all the next
//	along the spline with pointdistance between them.
// ------------------------------------------------------------------------------------------
// In  :
//		NVEC3	*ppoint			a valid pointer on the first NVEC3 to write result in.
//		const Nu32	pointnumber		number of successive valid NVEC3 to write successive results in.
//		const Nu32	pointstride		Stride in BYTE between each NVEC3.
//		const Nf32	pointdistance	Separating distance between each calculated point.
//		const Nf32	error			Because calculating spline length is an iterative estimation ( by small segments calculation )
//									User has to pass the tolerance error he may accept about pointdistance.
//									At the end of the distance between 2 points will be = pointdistance + or - error.
//		const NSPLINE *pspline		A Valid pointer on a spline with at least 2 splineknots.
//		const NVEC3 *porigin
// Out :
//		NTRUE for a success and NFALSE in case of problem (error > base)
//
// ------------------------------------------------------------------------------------------
Nu32 NUT_ExtractRegularlySpaced3DPointFromSpline(NVEC3 *ppoint, const Nu32 pointnumber, const Nu32 pointstride, const Nf32 pointdistance, const Nf32 error, const NSPLINE *pspline, const NVEC3 *porigin)
{
	NErrorIf(NGetSplineKnotsNumber(pspline) < 2, NERROR_WRONG_VALUE);

	NSPLINEKNOT *pfirst = NGetFirstSplineKnot(pspline);
	Nu32 lastid0 = NGetSplineKnotsNumber(pspline) - 2;
	Nf32 factormax = (Nf32)(lastid0 + 1);
	Nf32 length = 0.0f;
	NVEC3 *pvdest = ppoint;
	Nu32 id = 0;
	Nf32 fbasestep, fstep, factor, f0;
	NVEC3 u0, u1, v;
	Nf32 t;
	Nu32 id0;
	NVEC3 vshift;

	// Shifting vector (Only if pOrigin is set by user )
	if (porigin)
	{
		vshift = *porigin;
		NVec3SubFrom(&vshift, &pfirst->Position);
	}
	else
	{
		NVec3Set(&vshift, 0, 0, 0);
	}

	// The first Point
	u0 = pfirst->Position;
	NVec3AddTo(&u0, &vshift);
	*pvdest = u0;
	id++;
	if (id < pointnumber)
		pvdest = (NVEC3 *)((NBYTE *)pvdest + pointstride);
	else
		return id;

	fbasestep = 0.1f;
	f0 = 0.0f;
	factor = fstep = fbasestep;
	while (1)
	{
		// Get u1 and calculate segment [u0,u1] length
		id0 = (Nu32)factor;
		if (id0 > lastid0)
		{
			id0 = lastid0;
			t = 1.0f;
		}
		else
		{
			t = factor - (Nf32)(id0);
		}
		NCasteljauPoint3f32(&u1, t, pfirst + id0, pfirst + id0 + 1);
		NVec3AddTo(&u1, &vshift);
		NVec3Sub(&v, &u1, &u0);
		length = NVec3Length(&v);

		// Check segment length
		if (NABS((length - pointdistance)) <= error)
		{
			*pvdest = u1;
			id++;
			if (id < pointnumber)
			{
				pvdest = (NVEC3 *)((NBYTE *)pvdest + pointstride);
			}
			else
			{
				break;
			}

			u0 = u1;
			fstep = fbasestep;
			f0 = factor;
			factor += fstep;
		}
		else
		{
			if (length > pointdistance)
			{
				fstep /= 2.0f;
				factor = f0 + fstep;
			}
			else // length < pointdistance
			{
				if (factor >= factormax)
				{
					break;
				}
				else
				{
					f0 = factor;
					factor += fstep;
				}
			}
		}
	}

	return id;
}

// ------------------------------------------------------------------------------------------
// NUT_ExtractForwardPositionFromSpline
// ------------------------------------------------------------------------------------------
// Description :
//	Find a point on spline from a t0 position. this point will be at 'distance forward' from t0.
//	Function returns t1 and fill up the NVEC3 param.
// ------------------------------------------------------------------------------------------
// In  :
//		pposition:			ptr on a valid NVECTOR which will receive the position on spline if its possible
//
//		t0:					t0 is a special ratio, with knot information included.
//							0<t0<(Nf32)(NSplineKnotNumber-1)
//
//		distanceforward:	From the 't0' point on spline, algorithm is going to find the next point on spline
//							placed at this exact distance.
//
//		error:				value of the tolerance to find the next point (distance +o r- this error)
//
//		pspline				The spline used for calculation
//
// Out :
//		>=0.0f if a valid position on spline was found. -1 in case of no valid solution ( in that case pposition remains unchanged )
//
// ------------------------------------------------------------------------------------------
Nf32 NUT_ExtractForwardPositionFromSpline(NVEC3 *pposition, const Nf32 t0, const Nf32 distanceforward, const Nf32 error, const NSPLINE *pspline, const NVEC3 *porigin)
{
	NErrorIf(NGetSplineKnotsNumber(pspline) < 2, NERROR_WRONG_VALUE);

	NSPLINEKNOT *pfirst = NGetFirstSplineKnot(pspline);
	Nu32 lastid0 = NGetSplineKnotsNumber(pspline) - 2;
	Nf32 factormax = (Nf32)(lastid0 + 1);
	NVEC3 v0, v1, v;
	NVEC3 vshift;
	Nu32 id0;
	Nf32 t;
	Nf32 length = 0.0f;
	Nf32 fbasestep, fstep, factor, f0;

	// Shifting vector (Only if pOrigin is set by user )
	if (porigin)
	{
		vshift = *porigin;
		NVec3SubFrom(&vshift, &pfirst->Position);
	}
	else
	{
		NVec3Set(&vshift, 0, 0, 0);
	}

	// Extract Position 0
	id0 = (Nu32)t0;
	if (id0 > lastid0)
	{
		id0 = lastid0;
		t = 1.0f;
	}
	else
	{
		t = t0 - (Nf32)(id0);
	}
	NCasteljauPoint3f32(&v0, t, pfirst + id0, pfirst + id0 + 1);

	// Look for the forward position
	fbasestep = 0.1f;
	fstep = fbasestep;
	f0 = t0;
	factor = f0 + fstep;
	while (1)
	{
		// Get u1 and calculate segment [u0,u1] length
		id0 = (Nu32)factor;
		if (id0 > lastid0)
		{
			id0 = lastid0;
			t = 1.0f;
		}
		else
		{
			t = factor - (Nf32)(id0);
		}
		NCasteljauPoint3f32(&v1, t, pfirst + id0, pfirst + id0 + 1);
		NVec3Sub(&v, &v1, &v0);
		length = NVec3Length(&v);

		// Check segment length
		if (NABS((length - distanceforward)) <= error)
		{
			NVec3Add(pposition, &v1, &vshift);
			return factor;
		}
		else
		{
			if (length > distanceforward)
			{
				fstep /= 2.0f;
				factor = f0 + fstep;
			}
			else // length < pointdistance
			{
				if (factor >= factormax)
				{
					return -1.0f;
				}
				else
				{
					f0 = factor;
					factor += fstep;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// Nf32 NUT_WrapOnSpline
// ------------------------------------------------------------------------------------------
// Description :
//	Place a point on a spline. Initial distance between the incoming point position and the reference point on spline
//	given by '*pxtd_factor' is going to be wrap on the appropriate direction along the spline to obtain outgoing point position.
//	The spline length portion between the reference point and the out going point is estimated by adding all the small segments
//	length obtain with factor_step.
// ------------------------------------------------------------------------------------------
// In	:
//			NVEC3		*ppoint			Point position to wrap on spline. This NVEC3 will receive NEW wrapped position.
//			Nf32			*pxtd_factor	Reference position ON Spline. This factor will be updated to receive parametric
//											position (xtd_factor) of the NEW Wrapped position.
//			const Nf32		*pfactor_step
//			const NSPLINE	*pspline
// Out :
//			Whatever return value, the 3 params ppoint, pxtd_factor and pfactor_step were updated in the right way.
//
//			1 Wrap Process successful.
//					*ppoint contains position of the wrapped point.
//					*pxtd_factor contains parametric position (/pspline) of the wrapped point.
//					*pfactor_step contains the step used by algorithm.
//
//			0 Wrap Process interrupted because algorithm reaches spline limits.
//					*ppoint contains position of the wrapped point which is equal to one of the 2 limits here.
//					*pxtd_factor contains parametric position (/pspline) of the wrapped point which is equal to one of the 2 limits here.
//					*pfactor_step contains the step used by algorithm.
//
//		   -1 Wrap Process interrupted because algorithm reaches an acute angle on the spline.
//					*ppoint contains position of the wrapped point which is equal to the last valid intermediate calculated position with 'pfactor_step'.
//					*pxtd_factor contains parametric position (/pspline) of the wrapped point which is equal to the last valid factor used by algorithm.
//					*pfactor_step contains the step used by algorithm.
//
// ------------------------------------------------------------------------------------------
Ns32 NUT_WrapOnSpline(NVEC3 *ppoint, Nf32 *pxtd_factor, Nf32 *pfactor_step, const NSPLINE *pspline)
{
	NErrorIf(*pxtd_factor < 0.0f || *pxtd_factor > (Nf32)(pspline->KnotArray.Size - 1), NERROR_VALUE_OUTOFRANGE);

	NVEC3 A, B, C;
	NVEC3 *pA;
	NVEC3 AB, AC;
	Nf32 t0, t1;
	Nf32 xtd_fact, prevxtd_fact;
	Nf32 tmax = (Nf32)(pspline->KnotArray.Size - 1);
	Nf32 used_step;
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + DEFINE SPLINEKNOT PARSING DIRECTION (only if is undefined )
	// +		The point is knowing, at a specific location on the spline, what is the best parsing direction to follow
	// +		incoming move direction.
	// + Value of factor_step represents this parsing direction
	// +		First Knot to to Last Knot	(forward)	:  >0
	// +		Last Knot to First Knot		(backward)	: 0<
	// +		Undefined								:  =0
	// +
	if (*pfactor_step == 0.0f)
	{
		Nf32 step = 1.0f / ((Nf32)pspline->BezierAccuracy * (pspline->KnotArray.Size - 1));

		if (*pxtd_factor <= 0.0f)
		{
			t0 = 0.0f;
			t1 = step; // step < tmax, always
			pA = &A;
		}
		else if (*pxtd_factor >= tmax)
		{
			t1 = tmax;
			t0 = tmax - step; // tmax-step > 0, always
			pA = &B;		  // !!?? YES because, '*pxtd_factor' is the reference and here it will be B and not A !!! because it's the left extremity
		}
		else // 0 < xtd_factor < tmax
		{
			t0 = *pxtd_factor;
			t1 = NMIN(t0 + step, tmax); // t0+step may be greater than tmax
			pA = &A;
		}

		NSplinePointD3f32(&A, t0, pspline);
		NSplinePointD3f32(&B, t1, pspline);
		NVec3Sub(&AB, &B, &A);
		NVec3Sub(&AC, ppoint, pA); // Using pA instead of A just here to obtain the right differential vector between ref position given by *pxtd_factor  and ppoint
		if (NVec3DotProduct(&AC, &AB) < 0.0f)
		{
			used_step = -step;
		}
		else
		{
			used_step = step;
		}
	}
	else
	{
		used_step = *pfactor_step;
	}
	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + CHECK the LIMITS regarding parsing DIRECTION
	// +
	if (*pxtd_factor <= 0.0f && used_step <= 0.0f)
	{
		*pxtd_factor = 0.0f;
		*pfactor_step = used_step;
		NSplinePointD3f32(ppoint, 0.0f, pspline);
		return 0; // Wrap Process interrupted because algorithm reaches spline limits
	}
	else if (*pxtd_factor >= tmax && used_step >= 0.0f)
	{
		*pxtd_factor = tmax;
		*pfactor_step = used_step;
		NSplinePointD3f32(ppoint, tmax, pspline);
		return 0; // Wrap Process interrupted because algorithm reaches spline limits
	}

	// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// +
	// + LIE ALL THE MOVEMENT QUANTITY ON PARSING DIRECTION
	// +
	NUT_CLOSESTPOINT_ON_LINE_RESULT result;
	xtd_fact = *pxtd_factor;
	NSplinePointD3f32(&B, xtd_fact, pspline);
	C = *ppoint;
	while (1)
	{
		// Define A and B
		A = B;
		prevxtd_fact = xtd_fact;
		xtd_fact = NCLAMP(0.0f, xtd_fact + used_step, tmax);
		NSplinePointD3f32(&B, xtd_fact, pspline);
		// get P ( C projection on AB)
		NVec3Sub(&AB, &B, &A);
		NUT_GetClosestPointOnLine(&result, &A, &AB, &C);
		if (result.ParamCoord > 1.0f)
		{
			if (xtd_fact == 0.0f || xtd_fact == tmax)
			{
				*ppoint = B;
				*pxtd_factor = xtd_fact;
				*pfactor_step = used_step;
				return 0; // Wrap Process interrupted because algorithm reaches spline limits
			}
			else
			{
				C = result.PointOnLine; // Get the Projected of C as the next C for the next turn !
			}
		}
		else if (result.ParamCoord >= 0.0f && result.ParamCoord <= 1.0f)
		{
			xtd_fact = NCLAMP(0.0f, prevxtd_fact + result.ParamCoord * used_step, tmax);
			NSplinePointD3f32(ppoint, xtd_fact, pspline);
			*pxtd_factor = xtd_fact;
			*pfactor_step = used_step;
			return 1; // Wrap Process successful
		}
		else // < 0.0f that means Spline makes an Acute angle at a point
		{	 // We are going to get stuck, but... 1)on the spline and 2) as far as possible into the process.
			// So ... the previous valid intermediate position is the One.
			// At the right beginning of the loop A = B =  current position on Spline xtd_fact
			// later into the loop, A is always the previous B. So A is always a valid point on spline
			// with a valid associated extended factor, it's just that is an intermediate step in the iterative calculation
			// not the final result ... but ... it seems impossible to lie all the movement on spline so we stop were we are, and
			// that's it !
			*ppoint = A;
			*pxtd_factor = prevxtd_fact;
			*pfactor_step = used_step;
			return -1; // Wrap Process interrupted because algorithm reaches an acute angle on the spline.
		}
	}
}
#ifdef _NEDITOR
// -----------------------------------------------------------------------
#include "../N/NCStandard.h"
#include "../N/UI/NUI.h"
#include "../N/NCamera.h"
#include "../N/Maths/NVec3f32.h"
#include "../N/NCoordinateSystemConversions.h"
#include "../N/NEditor.h"
#include "../N/Utilities/NUT_X.h"
#include "../N/Utilities/NUT_3DMouse.h"
#include "../N/Utilities/Maths/NUT_MathsMisc.h"
#include "../N/Utilities/NUT_Spline.h"
#include "../N/Utilities/NUT_Slider.h"
#include "../N/Utilities/Draw/NUT_Draw.h"
#include "../N/Utilities/Draw/NUT_DrawPencil.h"


#include "../N/Event/NEventTouch.h"
#include "../N/Maths/NMatrix3x3.h"
#include "../N/Maths/NVecLimits.h"
#include "../N/NMath.h"

#include "../NL/MotionControl/Path/Builders/WayPoints/NLPathBuilderWayPoints.h"
#include "../NL/MotionControl/Path/Builders/WayPoints/NLPathSection.h"


#include "../NL/NL2DOrthogonalCoordinateSystem.h"
#include "App_WayPoints_Tools.h"


//extern NL2DOCS	PathVelocityLimitsOcs;
NUT_SLIDER	Slider;		// Graphic Slider utilisé pour définir les valeurs ICRA ICRC et BCR ( InsertedCircleRatioA, InsertedCircleRatioC et BuildCircleRatio)


// Quelques "outils" à usage local uniquemnt:



APP_WAYPOINT_TOOLDATA*	App_SetupWayPointsToolData(APP_WAYPOINT_TOOLDATA* pwpack, NLPATH_WORKBENCH *pworkbench)
{
	Nmem0(pwpack, APP_WAYPOINT_TOOLDATA);
	pwpack->m_flags				= 0;
	pwpack->m_pWorkbench		= pworkbench;

	pwpack->m_ICircleHandleExtend.x =		DEFAULT_APP_ICIRCLE_HANDLE_XEXTEND;
	pwpack->m_ICircleHandleExtend.y =		DEFAULT_APP_ICIRCLE_HANDLE_YEXTEND;

	NSetColorf(&pwpack->m_SelectionColorA,	DEFAULT_APP_WAYPOINT_SELECTION_COLORA	);
	NSetColorf(&pwpack->m_SelectionColorB,	DEFAULT_APP_WAYPOINT_SELECTION_COLORB	);
	NSetColorf(&pwpack->m_HelpersColor,		DEFAULT_APP_HELPERS_COLOR				);

	/*
	pwpack->m_TgtSize = DEFAULT_APP_WAYPOINT_TGT_SIZE;
	pwpack->m_WayPointExtend.x = DEFAULT_APP_WAYPOINT_XEXTEND;
	pwpack->m_WayPointExtend.y = DEFAULT_APP_WAYPOINT_YEXTEND;
	pwpack->m_KeyPointExtend.x = DEFAULT_APP_KEYPOINT_XEXTEND;
	pwpack->m_KeyPointExtend.y = DEFAULT_APP_KEYPOINT_YEXTEND;

	NSetColorf(&pwpack->m_HandleColor,		DEFAULT_APP_HANDLE_COLOR,				1);
	NSetColorf(&pwpack->m_SegmentColor,		DEFAULT_APP_SEGMENT_COLOR,				1);
	NSetColorf(&pwpack->m_ClothoidColorA,	DEFAULT_APP_CLOTHOID_COLORA,			1);
	NSetColorf(&pwpack->m_ClothoidColorB,	DEFAULT_APP_CLOTHOID_COLORB,			1);
	NSetColorf(&pwpack->m_ArcColor,			DEFAULT_APP_ARC_COLOR,					1);
	NSetColorf(&pwpack->m_KeyPointColor,	DEFAULT_APP_KEYPOINT_COLOR,				1);
	NSetColorf(&pwpack->m_KeyPointIDColor,	DEFAULT_APP_KEYPOINTID_COLOR,			1);
	NSetColorf(&pwpack->m_WayPointPColor,	DEFAULT_APP_WAYPOINT_P_COLOR,			1);
	NSetColorf(&pwpack->m_WayPointUColor,	DEFAULT_APP_WAYPOINT_U_COLOR,			1);
	*/
	return pwpack;
}

APP_WAYPOINT_TOOLDATA*	App_CreateWayPointsToolData( NLPATH_WORKBENCH *pworkbench )
{
	return App_SetupWayPointsToolData(NEW(APP_WAYPOINT_TOOLDATA), pworkbench);
}

void App_ClearWayPointsToolData(APP_WAYPOINT_TOOLDATA *pwpack)
{
	Nmem0(pwpack, APP_WAYPOINT_TOOLDATA);
}

void App_DeleteWayPointsToolData(APP_WAYPOINT_TOOLDATA *pwpack)
{
	App_ClearWayPointsToolData(pwpack);
	Nfree(pwpack);
}

static inline void _reset_helpersnapline(APP_WAYPOINT_TOOLDATA *pwpack)
{
	FLAG_OFF(pwpack->m_flags, FLAG_APP_WAYPOINT_TOOLDATA_DRAW_HELPER_SNAPLINE0);
	FLAG_OFF(pwpack->m_flags, FLAG_APP_WAYPOINT_TOOLDATA_DRAW_HELPER_SNAPLINE1);
	Nmem0(&pwpack->m_SnapLine[0], APP_HELPER_SNAPLINE);
	Nmem0(&pwpack->m_SnapLine[1], APP_HELPER_SNAPLINE);
}
/*
static inline void _snap_waypoint_u0(NLPATH_WAYPOINT *pwpout, APP_HELPER_SNAPLINE *phelper, const NVEC2f32 *pincomingposition, const NARRAY *pwparray)
{
	Nf32								dot;
	Nf32								lv2;
	Nf32								mindist;
	NVEC2f32							result;
	NLPATH_WAYPOINT						*pwpclosest;
	NLPATH_WAYPOINT						*pwp;
	Nu32								i;
	NUT_2DCLOSESTPOINT_ON_LINE_RESULT	x2dresult;
	NVEC2f32							v2;

	mindist = APP_WAYPOINT_SNAP_DISTANCE;
	pwpclosest = NULL;

	Nmem0(&phelper[0], APP_HELPER_SNAPLINE);
	Nmem0(&phelper[1], APP_HELPER_SNAPLINE);

	pwp = (NLPATH_WAYPOINT*)pwparray->pFirst;
	for (i = 0; i < pwparray->Size; i++, pwp++)
	{
		if (pwp == pwpout)
			continue;

		// 1)Check d'un alignement prealable existant 
		v2.x = pwpout->p.x - pwp->p.x;
		v2.y = pwpout->p.y - pwp->p.y;
		dot = NABS( NVec2DotProduct(&v2, &pwp->u) / NVec2Length(&v2) );
		if (NABS(1.0f - dot) > NF32_EPSILON_VECTOR_DOTPRODUCT)
			continue;
		
		// 2)Alignement envisageable
		NUT_2DGetClosestPointOnLine(&x2dresult, &pwp->p, &pwp->u, pincomingposition);
		v2.x = x2dresult.I.x - pincomingposition->x;
		v2.y = x2dresult.I.y - pincomingposition->y;
		lv2 = NVec2Length(&v2);
		if (lv2 < mindist)
		{
			result		= x2dresult.I;
			pwpclosest	= pwp;
			mindist		= lv2;
		}
	}

	if (pwpclosest)
	{
		v2.x = result.x - pwpout->p.x;
		v2.y = result.y - pwpout->p.y;
		NVec2Normalize(&v2);

		pwpout->u	= v2;
		FLAG_ON(pwpack->m_flags, FLAG_APP_WAYPOINT_TOOLDATA_DRAW_HELPER_SNAPLINE0);
		phelper[0].m_p	= pwpclosest->p;
		phelper[0].m_u	= pwpclosest->u;
		return;
	}
	else
	{
		v2.x = pincomingposition->x - pwpout->p.x;
		v2.y = pincomingposition->y - pwpout->p.y;
		NVec2Normalize(&v2);

		pwpout->u = v2;
		return;
	}
}
*/
static inline void _snap_waypoint_u(NLPATH_WAYPOINT *pwpout, const NVEC2f32 *pincomingposition, APP_WAYPOINT_TOOLDATA *pdata)
{
	NLPATH_BUILDER_WAYPOINTS* pwpbuilder = (NLPATH_BUILDER_WAYPOINTS*)pdata->m_pWorkbench->m_pPathBuilder;
	Nf32								lv2;
	Nf32								mindist;
	NVEC2f32							result;
	NLPATH_WAYPOINT						*pwpclosest;
	NLPATH_WAYPOINT						*pwp;
	Nu32								i;
	NUT_2DCLOSESTPOINT_ON_LINE_RESULT	x2dresult;
	NVEC2f32							v2;
	


	mindist = APP_WAYPOINT_SNAP_DISTANCE;
	pwpclosest = NULL;


	_reset_helpersnapline(pdata);

	NARRAY *pwparray = pwpbuilder->getArrayOfWayPoints();
	pwp = (NLPATH_WAYPOINT*)pwparray->pFirst;
	for (i = 0; i < pwparray->Size; i++, pwp++)
	{
		if (pwp == pwpout)
			continue;

		v2.x = pwp->p.x - pwpout->p.x;
		v2.y = pwp->p.y - pwpout->p.y;
		NUT_2DGetClosestPointOnLine(&x2dresult, &pwpout->p, &v2, pincomingposition);
		v2.x = x2dresult.I.x - pincomingposition->x;
		v2.y = x2dresult.I.y - pincomingposition->y;
		lv2 = NVec2Length(&v2);
		if (lv2 < mindist)
		{
			result = x2dresult.I;
			pwpclosest = pwp;
			mindist = lv2;
		}
		
		NUT_2DGetClosestPointOnLine(&x2dresult, &pwpout->p, &pwp->u, pincomingposition);
		v2.x = x2dresult.I.x - pincomingposition->x;
		v2.y = x2dresult.I.y - pincomingposition->y;
		lv2 = NVec2Length(&v2);
		if (lv2 < mindist)
		{
			result = x2dresult.I;
			pwpclosest = pwp;
			mindist = lv2;
		}
	}

	if (pwpclosest)
	{
		v2.x = result.x - pwpout->p.x;
		v2.y = result.y - pwpout->p.y;
		NVec2Normalize(&v2);

		pwpout->u = v2;

		FLAG_ON(pdata->m_flags, FLAG_APP_WAYPOINT_TOOLDATA_DRAW_HELPER_SNAPLINE0);
		pdata->m_SnapLine[0].m_p	= pwpclosest->p;
		pdata->m_SnapLine[0].m_u.x	= pwpclosest->p.x - pwpout->p.x;
		pdata->m_SnapLine[0].m_u.y  = pwpclosest->p.y - pwpout->p.y;
		return;
	}
	else
	{
		v2.x = pincomingposition->x - pwpout->p.x;
		v2.y = pincomingposition->y - pwpout->p.y;
		NVec2Normalize(&v2);

		pwpout->u = v2;
		return;
	}
}

static inline void _snap_waypoint_p(NLPATH_WAYPOINT *pwpout, const NVEC2f32 *pincomingposition, APP_WAYPOINT_TOOLDATA *pdata)
{
	NLPATH_BUILDER_WAYPOINTS* pwpbuilder = (NLPATH_BUILDER_WAYPOINTS*)pdata->m_pWorkbench->m_pPathBuilder;
	Nf32								lv2;
	Nf32								mindist[2];
	NVEC2f32							result[2];
	NLPATH_WAYPOINT						*pwpclosest[2];
	NLPATH_WAYPOINT						*pwp;
	Nu32								i;
	NUT_2DCLOSESTPOINT_ON_LINE_RESULT	x2dresult;
	NVEC2f32							v2;

	mindist[0]		= APP_WAYPOINT_SNAP_DISTANCE;
	mindist[1]		= APP_WAYPOINT_SNAP_DISTANCE;
	pwpclosest[0]	= NULL;
	pwpclosest[1]	= NULL;

	_reset_helpersnapline(pdata);

	NARRAY *pwparray = pwpbuilder->getArrayOfWayPoints();
	pwp = (NLPATH_WAYPOINT*)pwparray->pFirst;
	for (i = 0; i < pwparray->Size; i++, pwp++)
	{
		if (pwp == pwpout)
			continue;

		NUT_2DGetClosestPointOnLine(&x2dresult, &pwp->p, &pwp->u, pincomingposition);
		v2.x	= x2dresult.I.x - pincomingposition->x;
		v2.y	= x2dresult.I.y - pincomingposition->y;
		lv2 = NVec2Length(&v2);
		if (lv2 < mindist[0])
		{
			result[0]		= x2dresult.I;
			pwpclosest[0]	= pwp;
			mindist[0]		= lv2;
		}
		else if (lv2 < mindist[1])
		{
			result[1]		= x2dresult.I;
			pwpclosest[1]	= pwp;
			mindist[1]		= lv2;
		}
	}

	if (pwpclosest[1])
	{
		if (NUT_2DLineXLine_VDir(&pwpclosest[0]->p, &pwpclosest[0]->u, &pwpclosest[1]->p, &pwpclosest[1]->u, &x2dresult))
		{
			v2.x = x2dresult.I.x - pincomingposition->x;
			v2.y = x2dresult.I.y - pincomingposition->y;
			if (NVec2Length(&v2) < APP_WAYPOINT_SNAP_DISTANCE)
			{
				pwpout->p	= x2dresult.I;
				
				FLAG_ON(pdata->m_flags, FLAG_APP_WAYPOINT_TOOLDATA_DRAW_HELPER_SNAPLINE0);
				pdata->m_SnapLine[0].m_p = pwpclosest[0]->p;
				pdata->m_SnapLine[0].m_u = pwpclosest[0]->u;
				FLAG_ON(pdata->m_flags, FLAG_APP_WAYPOINT_TOOLDATA_DRAW_HELPER_SNAPLINE1);
				pdata->m_SnapLine[1].m_p = pwpclosest[1]->p;
				pdata->m_SnapLine[1].m_u = pwpclosest[1]->u;
				return;
			}
		}

		pwpout->p = result[0];
		FLAG_ON(pdata->m_flags, FLAG_APP_WAYPOINT_TOOLDATA_DRAW_HELPER_SNAPLINE0);
		pdata->m_SnapLine[0].m_p = pwpclosest[0]->p;
		pdata->m_SnapLine[0].m_u = pwpclosest[0]->u;
		FLAG_ON(pdata->m_flags, FLAG_APP_WAYPOINT_TOOLDATA_DRAW_HELPER_SNAPLINE1);
		pdata->m_SnapLine[1].m_p = pwpclosest[1]->p;
		pdata->m_SnapLine[1].m_u = pwpclosest[1]->u;
		return;
	}
	else if (pwpclosest[0])
	{
		pwpout->p	= result[0];
		FLAG_ON(pdata->m_flags, FLAG_APP_WAYPOINT_TOOLDATA_DRAW_HELPER_SNAPLINE0);
		pdata->m_SnapLine[0].m_p = pwpclosest[0]->p;
		pdata->m_SnapLine[0].m_u = pwpclosest[0]->u;
		return;
	}
	else
	{
		pwpout->p = *pincomingposition;
		return;
	}
}

#define FILL_SELECTION_AND_BREAK_THE_LOOP(pselect,pbuilder,ptr,tp,i){									\
														pselect->pSelectedOwner = (void*)pbuilder;		\
														pselect->pSelected		= (void*)ptr;			\
														pselect->SelectParam	= NMAKELONG(tp, i);		\
														i						= (NU32_MAX-1); }			// To stop the loop

/*
static inline void _lookfor_waypoint_element(NLPATH_BUILDER_WAYPOINTS *pbuilder, const Nf32 tgt_size, NLPATH *ppath, const NVEC2s16 *ppointer, NEDITOR_SELECTION *pselect, NVEC3 *phitpoint)
{
	Nu32							i,j;
	NVEC3							mouselineA,mouselineB;
	NUT_3DLINEXLINE_RESULT			result;
	Nf32							squared_dist,min_squared_dist;
	NVEC3							vp,vt;
	NVEC3							AB;
	NLPATH_WAYPOINT					*pwp;
	NVEC3							keep_hitpoint;

	NARRAY							*pwparray	= pbuilder->getArrayOfWayPoints();
	NARRAY							*psecarray = pbuilder->getArrayOfSections();
	NARRAY							*pprimarray = &ppath->m_geometry.m_primitivesArray;
	NARRAY							*pkparray	= &ppath->m_geometry.m_pathPointsArray;

	// Full 3D research.
	// At this stage we just have a ray ( mouse 3D ray ) and an array of elements to look into...
	// So we are to determine the closest element to the mouse 3D ray and if this element is close enough
	// to the ray it will be the One !
	// Lets go !
	NResetToolSelection(pselect);
	NUT_Get3DMouseSegment(&mouselineA, &mouselineB, NULL, ppointer);
	NVec3Sub(&AB, &mouselineB, &mouselineA);

	min_squared_dist = APP_WAYPOINT_DETECTION_DISTANCE_SQUARED;
	pwp = (NLPATH_WAYPOINT*)pwparray->pFirst;
	for(i=0;i<pwparray->Size;i++, pwp ++)
	{

		//#0 POSITION
		vp.x = pwp->p.x;
		vp.y = pwp->p.y;
		vp.z = 0.0f;
		NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&result.PointOnAB,&mouselineA,&AB,&vp);
		NVec3SubFrom(&vp,&result.PointOnAB);
		squared_dist = NVec3SquareLength(&vp);
		if(squared_dist<min_squared_dist)
		{
			min_squared_dist		= squared_dist;
			pselect->pSelectedOwner	= (void*)pbuilder;
			pselect->pSelected		= (void*)pwp;
			pselect->SelectParam	= NMAKELONG( APP_WAYPOINT_POSITION, i );
			keep_hitpoint			= result.PointOnAB;
			break;
		}

		//#1 TANGENTE AND RR HANDLE
		// Get the shortest distance between 3D Mouse Ray and Tangent A segment and analyze it ! 
		vp.x = pwp->p.x;
		vp.y = pwp->p.y;
		vp.z = 0.0f;

		vt.x = vp.x + pwp->u.x*(2.0f + pwp->rr)*tgt_size / 3.0f;
		vt.y = vp.y + pwp->u.y*(2.0f + pwp->rr)*tgt_size / 3.0f;
		vt.z = 0.0f;
		NUT_LineXSeg(&result, &mouselineA, &mouselineB, &vp, &vt);
		squared_dist = NUT_3DLineXLineResult_Extract_EstimateShortestSquareDistance(&result);
		if (squared_dist < min_squared_dist)
		{
			if ((result.ParamCoordCD > (1.0f - CONSTANT_NL_WAYPOINT_TANGENT_ARROWHEAD_SIZE_RATIO)) &&(i != pwparray->Size - 1) )
			{
				min_squared_dist = squared_dist;
				pselect->pSelectedOwner = (void*)pbuilder;
				pselect->pSelected = (void*)pwp;
				pselect->SelectParam = NMAKELONG(APP_WAYPOINT_BCR, i);
				keep_hitpoint = result.PointOnCD;
				break;
			}
			else
			{
				min_squared_dist = squared_dist;
				pselect->pSelectedOwner = (void*)pbuilder;
				pselect->pSelected = (void*)pwp;
				pselect->SelectParam = NMAKELONG(APP_WAYPOINT_TANGENT, i);
				keep_hitpoint = result.PointOnCD;
				break;
			}
		}
	}

	if (pselect->pSelected)
	{
		*phitpoint = keep_hitpoint;
		return;
	}


	//#2 INSERTED CIRCLE
	NLPATH_PRIMITIVE				*pprim;
	NLPATH_POINT					*pkp;
	NLPATH_SECTION					*psec;
	Nu32							a, b, c;
	NVEC3							vh;

	min_squared_dist = APP_WAYPOINT_DETECTION_DISTANCE_SQUARED;
	pwp		= (NLPATH_WAYPOINT*)pwparray->pFirst;
	pkp		= (NLPATH_POINT*)pkparray->pFirst;
	pprim	= (NLPATH_PRIMITIVE*)pprimarray->pFirst;
	psec	= (NLPATH_SECTION*)psecarray->pFirst;

	for (i = 0; i < psecarray->Size; j = GET_NLPATH_SECTION_PRIMITIVES_NB(psec->m_uid), pkp += j, pprim += j, i++, psec++, pwp++)
	{
		a		= GET_NLPATH_SECTION_PRIMITIVES_NB_A(psec->m_uid);
		b		= GET_NLPATH_SECTION_PRIMITIVES_NB_B(psec->m_uid);
		c		= GET_NLPATH_SECTION_PRIMITIVES_NB_C(psec->m_uid);
		
		// Check SECTION PART A
		switch (a)
		{
			// 1 seule primitive ... C'est un arc de cercle sans clothoide ! 
			case 1:
				NL_ExtractArcHandle((NVEC2f32*)&vh, pprim); vh.z = 0.0f;
				NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&result.PointOnAB, &mouselineA, &AB, &vh);
				vp.x = vh.x	- result.PointOnAB.x;
				vp.y = vh.y - result.PointOnAB.y;
				vp.z = vh.z - result.PointOnAB.z;

				squared_dist = NVec3SquareLength(&vp);
				if (squared_dist < min_squared_dist)
				{
					FILL_SELECTION_AND_BREAK_THE_LOOP(pselect, pbuilder, psec, APP_WAYPOINT_ICRA, i);
					keep_hitpoint = result.PointOnAB;
					continue;
				}
				break;

			// 2 primitives ... C'est une paire de Clothoides ! ( Sans arc de cercle inséré )
			case 2:
				vh.x = pkp[1].p.x;
				vh.y = pkp[1].p.y;
				vh.z = 0.0f;
				NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&result.PointOnAB, &mouselineA, &AB, &vh);
				vp.x = vh.x - result.PointOnAB.x;
				vp.y = vh.y - result.PointOnAB.y;
				vp.z = vh.z - result.PointOnAB.z;

				squared_dist = NVec3SquareLength(&vp);
				if (squared_dist < min_squared_dist)
				{
					FILL_SELECTION_AND_BREAK_THE_LOOP(pselect, pbuilder, psec, APP_WAYPOINT_ICRA, i);
					keep_hitpoint = result.PointOnAB;
					continue;
				}
				break;

			// 3 primitives ... Cette une paire de Clothoides avec un arc de cercle inséré entre les deux !
			case 3:
				NL_ExtractArcHandle((NVEC2f32 *)&vh, &pprim[1]); vh.z = 0.0f;
				NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&result.PointOnAB, &mouselineA, &AB, &vh);
				vp.x = vh.x - result.PointOnAB.x;
				vp.y = vh.y - result.PointOnAB.y;
				vp.z = vh.z - result.PointOnAB.z;

				squared_dist = NVec3SquareLength(&vp);
				if (squared_dist < min_squared_dist)
				{
					FILL_SELECTION_AND_BREAK_THE_LOOP(pselect, pbuilder, psec, APP_WAYPOINT_ICRA, i);
					keep_hitpoint = result.PointOnAB;
					continue;
				}
				break;

			// 4 primitives ... se sont 2 paire de Clothoides ! ( Sans arc de cercles insérés )
			case 4:
				vh.x = pkp[1].p.x;
				vh.y = pkp[1].p.y;
				vh.z = 0.0f;
				NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&result.PointOnAB, &mouselineA, &AB, &vh);
				vp.x = vh.x - result.PointOnAB.x;
				vp.y = vh.y - result.PointOnAB.y;
				vp.z = vh.z - result.PointOnAB.z;

				squared_dist = NVec3SquareLength(&vp);
				if (squared_dist < min_squared_dist)
				{
					FILL_SELECTION_AND_BREAK_THE_LOOP(pselect, pbuilder, psec, APP_WAYPOINT_ICRA, i);
					keep_hitpoint = result.PointOnAB;
					continue;
				}
				else
				{
					vh.x = pkp[3].p.x;
					vh.y = pkp[3].p.y;
					vh.z = 0.0f;
					NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&result.PointOnAB, &mouselineA, &AB, &vh);
					vp.x = vh.x - result.PointOnAB.x;
					vp.y = vh.y - result.PointOnAB.y;
					vp.z = vh.z - result.PointOnAB.z;

					squared_dist = NVec3SquareLength(&vp);
					if (squared_dist < min_squared_dist)
					{
						FILL_SELECTION_AND_BREAK_THE_LOOP(pselect, pbuilder, psec, APP_WAYPOINT_ICRA, i);
						keep_hitpoint = result.PointOnAB;
						continue;
					}
				}
				break;


			// 6 primitives ... se sont 2 paires de Clothoides avec , pour chaque paire un arc de cercle inséré !
			case 6:
				NL_ExtractArcHandle((NVEC2f32 *)&vh, &pprim[1]); vh.z = 0.0f;
				NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&result.PointOnAB, &mouselineA, &AB, &vh);
				vp.x = vh.x - result.PointOnAB.x;
				vp.y = vh.y - result.PointOnAB.y;
				vp.z = vh.z - result.PointOnAB.z;

				squared_dist = NVec3SquareLength(&vp);
				if (squared_dist < min_squared_dist)
				{
					FILL_SELECTION_AND_BREAK_THE_LOOP(pselect, pbuilder, psec, APP_WAYPOINT_ICRA, i);
					keep_hitpoint = result.PointOnAB;
					continue;
				}
				else
				{
					NL_ExtractArcHandle((NVEC2f32 *)&vh, &pprim[4]); vh.z = 0.0f;
					NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&result.PointOnAB, &mouselineA, &AB, &vh);
					vp.x = vh.x - result.PointOnAB.x;
					vp.y = vh.y - result.PointOnAB.y;
					vp.z = vh.z - result.PointOnAB.z;

					squared_dist = NVec3SquareLength(&vp);
					if (squared_dist < min_squared_dist)
					{
						FILL_SELECTION_AND_BREAK_THE_LOOP(pselect, pbuilder, psec, APP_WAYPOINT_ICRA, i);
						keep_hitpoint = result.PointOnAB;
						continue;
					}
				}
				break;

			default:
				break;
		}

		// Check SECTION PART C
		switch (c)
		{
			// 1 seule primitive ... C'est un arc de cercle sans clothoide ! 
			case 1:
				NL_ExtractArcHandle((NVEC2f32 *)&vh, &pprim[a + b]); vh.z = 0.0f;
				NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&result.PointOnAB, &mouselineA, &AB, &vh);
				vp.x = vh.x - result.PointOnAB.x;
				vp.y = vh.y - result.PointOnAB.y;
				vp.z = vh.z - result.PointOnAB.z;

				squared_dist = NVec3SquareLength(&vp);
				if (squared_dist < min_squared_dist)
				{
					FILL_SELECTION_AND_BREAK_THE_LOOP(pselect, pbuilder, psec, APP_WAYPOINT_ICRC, i);
					keep_hitpoint = result.PointOnAB;
					continue;
				}
				break;

			case 2:
				vh.x = pkp[a+b+1].p.x;
				vh.y = pkp[a + b + 1].p.y;
				vh.z = 0.0f;
				NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&result.PointOnAB, &mouselineA, &AB, &vh);
				vp.x = vh.x - result.PointOnAB.x;
				vp.y = vh.y - result.PointOnAB.y;
				vp.z = vh.z - result.PointOnAB.z;

				squared_dist = NVec3SquareLength(&vp);
				if (squared_dist < min_squared_dist)
				{
					FILL_SELECTION_AND_BREAK_THE_LOOP(pselect, pbuilder, psec, APP_WAYPOINT_ICRC, i);
					keep_hitpoint = result.PointOnAB;
					continue;
				}
				break;

			case 3:
				NL_ExtractArcHandle((NVEC2f32 *)&vh, &pprim[a + b + 1]); vh.z = 0.0f;
				NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&result.PointOnAB, &mouselineA, &AB, &vh);
				vp.x = vh.x - result.PointOnAB.x;
				vp.y = vh.y - result.PointOnAB.y;
				vp.z = vh.z - result.PointOnAB.z;

				squared_dist = NVec3SquareLength(&vp);
				if (squared_dist < min_squared_dist)
				{
					FILL_SELECTION_AND_BREAK_THE_LOOP(pselect, pbuilder, psec, APP_WAYPOINT_ICRC, i);
					keep_hitpoint = result.PointOnAB;
					continue;
				}
				break;

			case 4:
				vh.x = pkp[a + b + 1].p.x;
				vh.y = pkp[a + b + 1].p.y;
				vh.z = 0.0f;
				NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&result.PointOnAB, &mouselineA, &AB, &vh);
				vp.x = vh.x - result.PointOnAB.x;
				vp.y = vh.y - result.PointOnAB.y;
				vp.z = vh.z - result.PointOnAB.z;

				squared_dist = NVec3SquareLength(&vp);
				if (squared_dist < min_squared_dist)
				{
					FILL_SELECTION_AND_BREAK_THE_LOOP(pselect, pbuilder, psec, APP_WAYPOINT_ICRC, i);
					keep_hitpoint = result.PointOnAB;
					continue;
				}
				else
				{
					vh.x = pkp[a + b + 3].p.x;
					vh.y = pkp[a + b + 3].p.y;
					vh.z = 0.0f;
					NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&result.PointOnAB, &mouselineA, &AB, &vh);
					vp.x = vh.x - result.PointOnAB.x;
					vp.y = vh.y - result.PointOnAB.y;
					vp.z = vh.z - result.PointOnAB.z;

					squared_dist = NVec3SquareLength(&vp);
					if (squared_dist < min_squared_dist)
					{
						FILL_SELECTION_AND_BREAK_THE_LOOP(pselect, pbuilder, psec, APP_WAYPOINT_ICRC, i);
						keep_hitpoint = result.PointOnAB;
						continue;
					}
				}
				break;

			case 6:
				NL_ExtractArcHandle((NVEC2f32 *)&vh, &pprim[a + b + 1]); vh.z = 0.0f;
				NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&result.PointOnAB, &mouselineA, &AB, &vh);
				vp.x = vh.x - result.PointOnAB.x;
				vp.y = vh.y - result.PointOnAB.y;
				vp.z = vh.z - result.PointOnAB.z;

				squared_dist = NVec3SquareLength(&vp);
				if (squared_dist < min_squared_dist)
				{
					FILL_SELECTION_AND_BREAK_THE_LOOP(pselect, pbuilder, psec, APP_WAYPOINT_ICRC, i);
					keep_hitpoint = result.PointOnAB;
					continue;
				}
				else
				{
					NL_ExtractArcHandle((NVEC2f32 *)&vh, &pprim[a + b + 4]); vh.z = 0.0f;
					NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&result.PointOnAB, &mouselineA, &AB, &vh);
					vp.x = vh.x - result.PointOnAB.x;
					vp.y = vh.y - result.PointOnAB.y;
					vp.z = vh.z - result.PointOnAB.z;

					squared_dist = NVec3SquareLength(&vp);
					if (squared_dist < min_squared_dist)
					{
						FILL_SELECTION_AND_BREAK_THE_LOOP(pselect, pbuilder, psec, APP_WAYPOINT_ICRC, i);
						keep_hitpoint = result.PointOnAB;
						continue;
					}
				}
				break;

			default:
				break;
		}

	}


	if (pselect->pSelected)
	{
		*phitpoint = keep_hitpoint;
		return;
	}
}
*/
static inline void _lookfor_waypoint_elementB(NLPATH_BUILDER_WAYPOINTS* pbuilder, const Nf32 tgt_size, NLPATH* ppath, const NVEC2s16* ppointer, NEDITOR_SELECTION* pselect, NVEC3* phitpoint)
{
	Nu32							i;
	NVEC3							mouselineA, mouselineB;
	NUT_3DLINEXLINE_RESULT			result;
	Nf32							squared_dist, min_squared_dist;
	NVEC3							vp, vt;
	NVEC3							AB;
	NLPATH_WAYPOINT* pwp;
	NVEC3							keep_hitpoint;

	NARRAY* pwparray = pbuilder->getArrayOfWayPoints();
	NARRAY* psecarray = pbuilder->getArrayOfSections();
	NARRAY* pprimarray = &ppath->m_geometry.m_primitivesArray;
	NARRAY* pkparray = &ppath->m_geometry.m_pathPointsArray;

	// Full 3D research.
	// At this stage we just have a ray ( mouse 3D ray ) and an array of elements to look into...
	// So we are to determine the closest element to the mouse 3D ray and if this element is close enough
	// to the ray it will be the One !
	// Lets go !
	NResetToolSelection(pselect);
	NUT_Get3DMouseSegment(&mouselineA, &mouselineB, NULL, ppointer);
	NVec3Sub(&AB, &mouselineB, &mouselineA);

	min_squared_dist = APP_WAYPOINT_DETECTION_DISTANCE_SQUARED;
	pwp = (NLPATH_WAYPOINT*)pwparray->pFirst;
	for (i = 0; i < pwparray->Size; i++, pwp++)
	{

		//#0 POSITION
		vp.x = pwp->p.x;
		vp.y = pwp->p.y;
		vp.z = 0.0f;
		NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT*)&result.PointOnAB, &mouselineA, &AB, &vp);
		NVec3SubFrom(&vp, &result.PointOnAB);
		squared_dist = NVec3SquareLength(&vp);
		if (squared_dist < min_squared_dist)
		{
			min_squared_dist = squared_dist;
			pselect->pSelectedOwner = (void*)pbuilder;
			pselect->pSelected = (void*)pwp;
			pselect->SelectParam = NMAKELONG(APP_WAYPOINT_POSITION, i);
			keep_hitpoint = result.PointOnAB;
			break;
		}

		//#1 TANGENTE AND RR HANDLE
		// Get the shortest distance between 3D Mouse Ray and Tangent A segment and analyze it ! 
		vp.x = pwp->p.x;
		vp.y = pwp->p.y;
		vp.z = 0.0f;

		vt.x = vp.x + pwp->u.x * (2.0f + pwp->rr) * tgt_size / 3.0f;
		vt.y = vp.y + pwp->u.y * (2.0f + pwp->rr) * tgt_size / 3.0f;
		vt.z = 0.0f;
		NUT_LineXSeg(&result, &mouselineA, &mouselineB, &vp, &vt);
		squared_dist = NUT_3DLineXLineResult_Extract_EstimateShortestSquareDistance(&result);
		if (squared_dist < min_squared_dist)
		{
			if ((result.ParamCoordCD > (1.0f - CONSTANT_NL_WAYPOINT_TANGENT_ARROWHEAD_SIZE_RATIO)) && (i != pwparray->Size - 1))
			{
				min_squared_dist = squared_dist;
				pselect->pSelectedOwner = (void*)pbuilder;
				pselect->pSelected = (void*)pwp;
				pselect->SelectParam = NMAKELONG(APP_WAYPOINT_BCR, i);
				keep_hitpoint = result.PointOnCD;
				break;
			}
			else
			{
				min_squared_dist = squared_dist;
				pselect->pSelectedOwner = (void*)pbuilder;
				pselect->pSelected = (void*)pwp;
				pselect->SelectParam = NMAKELONG(APP_WAYPOINT_TANGENT, i);
				keep_hitpoint = result.PointOnCD;
				break;
			}
		}
	}

	if (pselect->pSelected)
	{
		*phitpoint = keep_hitpoint;
		return;
	}


	//#2 INSERTED CIRCLE
	NLPATH_PRIMITIVE*	pprim;
	NLPATH_POINT*		pkp;
	NVEC3				vh;

	min_squared_dist = APP_WAYPOINT_DETECTION_DISTANCE_SQUARED;
	//pwp = (NLPATH_WAYPOINT*)pwparray->pFirst;
	pkp = (NLPATH_POINT*)pkparray->pFirst;
	pprim = (NLPATH_PRIMITIVE*)pprimarray->pFirst;
	
	//psec = (NLPATH_SECTION*)psecarray->pFirst;

	for (i = 0; i < pprimarray->Size; i++, pprim ++,pkp++)
	{
		if (pprim->m_core.m_id == NLPATH_PRIMITIVE_ID_ARC)
		{
			NL_ExtractArcHandle((NVEC2f32*)&vh, &ppath->m_matrix,pprim); vh.z = 0.0f;
			NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT*)&result.PointOnAB, &mouselineA, &AB, &vh);
			vp.x = vh.x - result.PointOnAB.x;
			vp.y = vh.y - result.PointOnAB.y;
			vp.z = vh.z - result.PointOnAB.z;

			squared_dist = NVec3SquareLength(&vp);
			if (squared_dist < min_squared_dist)
			{
				pselect->pSelectedOwner = (void*)pbuilder;
				pselect->pSelected		= (void*)pprim;	// temporaire sera ensuite remplacé par un pointeur sur le waypoint
				pselect->SelectParam	= i;			// temporaire sera ensuite remplacé par l'index du waypoint

				keep_hitpoint = result.PointOnAB;
			}
		}
	}

	if (pselect->pSelected)
	{
		*phitpoint = keep_hitpoint;
		i = pbuilder->getSectionIndexXtd(pselect->SelectParam);
		pselect->pSelected  = (NLPATH_WAYPOINT*)NGetArrayPtr(&pbuilder->m_wayPointsArray, NLOWORD(i));
		switch (NHIWORD(i))
		{
		case 0:
			pselect->SelectParam = NMAKELONG(APP_WAYPOINT_ICRA, NLOWORD(i));
			break;
		case 1:
			NErrorIf(1,NERROR_INCONSISTENT_VALUES); // la part trouvée étant 1, cela correspond à la primitive segment !!!???
			break;
		case 2:
			pselect->SelectParam = NMAKELONG(APP_WAYPOINT_ICRC, NLOWORD(i));
			break;

		}
		return;
	}
}








// ------------------------------------------------------------------------------------------
// NUT_NVec3f32PackTool_Move
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
// Notes:
//		This Generic Tool uses Callback to help you to perform specific actions.
//		CallBack Calls are working like this:
//		
//		NTOUCH_MOVE:	"A Point is selected and moved (position changes)."
//			CallBack is going to be called  only if there is a selected Point and
//			after moving it. 
//			The "NEDITOR_SELECTION"	structure contains the selected point informations.
//
//		NTOUCH_END:
//			CallBack is going to be called  ONLY if a real move occurred before.
//			( obviously if there is a selected Point ... )
//			Be aware that the selected point position is exactly the same than the last 'NTOUCH_MOVE' callback call.
// ------------------------------------------------------------------------------------------
NEDITOR_RESULT App_WayPointsTool_Move(const NEVENT *pevent,NEDITOR_SELECTION *pselect,const Nu32 tool_param)
{
	static	Nbool						bmoved = NFALSE;
	static	NVEC3						hitpoint ={0,0,0};
	static	Nf32						additional_tangent_portion;
	static	Nu32						touch_id = 0;

	APP_WAYPOINT_TOOLDATA				*pdata = (APP_WAYPOINT_TOOLDATA *)tool_param;
	NLPATH_BUILDER_WAYPOINTS			*pwpbuilder = (NLPATH_BUILDER_WAYPOINTS*)pdata->m_pWorkbench->m_pPathBuilder;
	NARRAY								*pwparray = pwpbuilder->getArrayOfWayPoints();

	NLPATH_WAYPOINT						*pwp;
	NVEC3								mouseray_origin,mouseray_dirvect;
	NVEC3								planenormal,mouse_on_workplane;
	NVEC2								v2;
	NVEC2f32							sliderxtend;

	switch(pevent->Type)
	{
		case NTOUCH_START:
			bmoved = NFALSE;
			_lookfor_waypoint_elementB(pwpbuilder, pwpbuilder->m_TgtSize, pdata->m_pWorkbench->m_pPath, NGetEventTouchPositionPtr(pevent,0), pselect, &hitpoint);
			if (pselect->pSelected)
			{
				switch (NLOWORD(pselect->SelectParam))
				{
					case APP_WAYPOINT_ICRA:
						pwp = (NLPATH_WAYPOINT*)NGetArrayPtr(pwparray, NHIWORD(pselect->SelectParam));
						sliderxtend.x = 0.3f;
						sliderxtend.y = 0.02f;
						NUT_SetupSlider(&Slider, 0.0f, 1.0f, NULL, &sliderxtend, 19, CONSTANT_NUT_SLIDER_SNAP_STRENGTH_MEDIUM,NULL);
						NUT_EnableSliderRelativeHandle(&Slider);
						NUT_SetSliderLockedDownRectf(&Slider, NCoord_NormToHRel_X(0.05f), NCoord_NormToHRel_X(0.95f), NCoord_NormToHRel_Y(-0.95f), NCoord_NormToHRel_Y(-0.05f), NTRUE);
						NUT_PlaceSliderOnScreenPlane(&Slider, pwp->icr_A, NGetEventTouchPositionPtr(pevent, 0));
						/*
						sliderxtend.x = 100.0f;
						sliderxtend.y = 5.0f;
						v2.x = 0.0f;
						v2.y = 1.0f;
						NUT_SetupSlider(&Slider, 0.0f, 1.0f, &v2, &sliderxtend, 19, CONSTANT_NUT_SLIDER_SNAP_STRENGTH_MEDIUM);

						NIdentityMatrix(&MX);
						//NRotationMatrix_AxisY(&MX, NDEGtoRAD(90.0f));
						MX.Origin.x = hitpoint.x;
						MX.Origin.y = hitpoint.y;
						MX.Origin.z = hitpoint.z;
						NUT_PlaceSlider(&Slider, pwp->icr_A, NGetEventTouchPositionPtr(pevent, 0),&MX);
						*/
						break;

					case  APP_WAYPOINT_ICRC:
						pwp = (NLPATH_WAYPOINT*)NGetArrayPtr(pwparray, NHIWORD(pselect->SelectParam));
						sliderxtend.x = 0.3f;
						sliderxtend.y = 0.02f;
						NUT_SetupSlider(&Slider, 0.0f, 1.0f, NULL, &sliderxtend, 19, CONSTANT_NUT_SLIDER_SNAP_STRENGTH_MEDIUM,NULL);
						NUT_EnableSliderRelativeHandle(&Slider);
						NUT_SetSliderLockedDownRectf(&Slider, NCoord_NormToHRel_X(0.05f), NCoord_NormToHRel_X(0.95f), NCoord_NormToHRel_Y(-0.95f), NCoord_NormToHRel_Y(-0.05f), NTRUE);
						NUT_PlaceSliderOnScreenPlane(&Slider, (pwp+1)->icr_C, NGetEventTouchPositionPtr(pevent, 0));
						break;

					case  APP_WAYPOINT_BCR:
						pwp = (NLPATH_WAYPOINT*)NGetArrayPtr(pwparray, NHIWORD(pselect->SelectParam));
						sliderxtend.x = 0.3f;
						sliderxtend.y = 0.02f;
						NUT_SetupSlider(&Slider, 0.0f, 1.0f, &pwp->u, &sliderxtend, 19, CONSTANT_NUT_SLIDER_SNAP_STRENGTH_MEDIUM,NULL);
						NUT_EnableSliderRelativeHandle(&Slider);
						NUT_SetSliderLockedDownRectf(&Slider, NCoord_NormToHRel_X(0.05f), NCoord_NormToHRel_X(0.95f), NCoord_NormToHRel_Y(-0.95f), NCoord_NormToHRel_Y(-0.05f), NTRUE);
						NUT_PlaceSliderOnScreenPlane(&Slider, pwp->rr, NGetEventTouchPositionPtr(pevent, 0));
						break;

					default:
						break;
				}
				return NEDITOR_RESULT_TOOL_ON;
			}
			break;

		case NTOUCH_MOVE:
			
			if(pselect->pSelected)
			{
				NGetEditorWorkPlaneNormalVector(&planenormal);
				NUT_Get3DMouseRay(&mouseray_origin,&mouseray_dirvect,NULL,NGetEventTouchPositionPtr(pevent,0));
				
				if (NUT_LineXPLane(&mouseray_origin, &mouseray_dirvect, &hitpoint, &planenormal, &mouse_on_workplane))
				{
					switch (NLOWORD(pselect->SelectParam))
					{
					case APP_WAYPOINT_POSITION:
						pwp = (NLPATH_WAYPOINT*)pselect->pSelected;
						// Mouvements spéciaux, alignements:
						if(ISFLAG_ON(pdata->m_flags, FLAG_APP_WAYPOINT_TOOLDATA_SNAP_ON))
							_snap_waypoint_p(pwp, (const NVEC2f32 *)&mouse_on_workplane,pdata);
						else
						{
							pwp->p.x = mouse_on_workplane.x;
							pwp->p.y = mouse_on_workplane.y;
						}
						bmoved = NTRUE;
						pdata->m_pWorkbench->requestPathBuild();
						return NEDITOR_RESULT_TOOL_ON;

					case APP_WAYPOINT_TANGENT:
						pwp = (NLPATH_WAYPOINT*)pselect->pSelected;
						if (ISFLAG_ON(pdata->m_flags, FLAG_APP_WAYPOINT_TOOLDATA_SNAP_ON))
							_snap_waypoint_u(pwp, (const NVEC2f32 *)&mouse_on_workplane, pdata);
						else
						{
							v2.x = mouse_on_workplane.x - pwp->p.x;
							v2.y = mouse_on_workplane.y - pwp->p.y;
							NVec2Normalize(&v2);
							pwp->u.x = v2.x;
							pwp->u.y = v2.y;
						}
						bmoved = NTRUE;
						pdata->m_pWorkbench->requestPathBuild();
						return NEDITOR_RESULT_TOOL_ON;

					case APP_WAYPOINT_BCR:
						pwp = (NLPATH_WAYPOINT*)pselect->pSelected;
						/*
						v2.x = mouse_on_workplane.x - pwp->p.x;
						v2.y = mouse_on_workplane.y - pwp->p.y;
						dot = NVec2DotProduct(&v2, &pwp->u);

						pwp->rr = NCLAMP(0.01f, ((2.0f*dot)/ pdata->m_TgtSize) - 1.0f  , 1.0f);
						*/
						pwp->rr = NUT_UpdateSliderOnScreenPlane(&Slider, NGetEventTouchPositionPtr(pevent, 0));
						bmoved = NTRUE;
						pdata->m_pWorkbench->requestPathBuild();
						return NEDITOR_RESULT_TOOL_ON;

					case APP_WAYPOINT_ICRA:
						pwp		= (NLPATH_WAYPOINT*)NGetArrayPtr(pwparray, NHIWORD(pselect->SelectParam));
						pwp->icr_A = NUT_UpdateSliderOnScreenPlane(&Slider, NGetEventTouchPositionPtr(pevent, 0));
						//pwp->icr_A = NUT_UpdateSlider(&Slider, NGetEventTouchPositionPtr(pevent, 0),&MX);

						bmoved = NTRUE;
						pdata->m_pWorkbench->requestPathBuild();
						return NEDITOR_RESULT_TOOL_ON;

					case APP_WAYPOINT_ICRC:
						pwp = (NLPATH_WAYPOINT*)NGetArrayPtr(pwparray, NHIWORD(pselect->SelectParam));
						(pwp+1)->icr_C = NUT_UpdateSliderOnScreenPlane(&Slider, NGetEventTouchPositionPtr(pevent, 0));
						//pwp->icr_A = NUT_UpdateSlider(&Slider, NGetEventTouchPositionPtr(pevent, 0),&MX);

						bmoved = NTRUE;
						pdata->m_pWorkbench->requestPathBuild();
						return NEDITOR_RESULT_TOOL_ON;

					default:
						break;
					}
				}
			}
			break;
		
		case NTOUCH_END:

			_reset_helpersnapline(pdata);

			if( pselect->pSelected && bmoved )
			{
				if ( NLOWORD(pselect->SelectParam) == APP_WAYPOINT_ICRA || NLOWORD(pselect->SelectParam) == APP_WAYPOINT_ICRC || NLOWORD(pselect->SelectParam) == APP_WAYPOINT_BCR)
					NUT_ClearSlider(&Slider);
				
				NResetToolSelection(pselect);
				bmoved = NFALSE;
				return NEDITOR_RESULT_TOOL_ON;
			}
			else
			{
				if (NLOWORD(pselect->SelectParam) == APP_WAYPOINT_ICRA || NLOWORD(pselect->SelectParam) == APP_WAYPOINT_ICRC)
					NUT_ClearSlider(&Slider);

				NResetToolSelection(pselect);
			}
			break;

		default:
			break;
	}


	return NEDITOR_RESULT_TOOL_OFF;
}


// ------------------------------------------------------------------------------------------
// NUT_NVec3f32PackTool_Erase
// ------------------------------------------------------------------------------------------
// Description :
// 
// ------------------------------------------------------------------------------------------
// In	:
// 
// 
// Out :
//
// Notes:
//		This Generic Tool uses Callback to help you to perform specific actions.
//		CallBack Calls are working like this:
//	
//		NTOUCH_END:	"Selected SplinePoint was deleted. Remaining accessible selection informations are obsolete."
//			CallBack is going to be called  ONLY if a SplinePoint is really deleted, and it will be call
//			AFTER the deleting process, BUT before reseting "NEDITOR_SELECTION" data. So, deleted SplinePoint
//			informations remain accessible even if they are obsolete.
//			Take care ! the deleted spline point is really deleted !!!
//			Don't try to use the "NEDITOR_SELECTION" informations to access ...
// ------------------------------------------------------------------------------------------
NEDITOR_RESULT App_WayPointsTool_Erase(const NEVENT *pevent,NEDITOR_SELECTION *pselect, const Nu32 tool_param )
{
	APP_WAYPOINT_TOOLDATA* pdata = (APP_WAYPOINT_TOOLDATA*)tool_param;
	NLPATH_BUILDER_WAYPOINTS* pwpbuilder = (NLPATH_BUILDER_WAYPOINTS*)pdata->m_pWorkbench->m_pPathBuilder;

	NVEC3f32					localhitpoint;

	#ifdef _DEBUG
	Nu32 bdel = 0;
	#endif

	switch(pevent->Type)
	{
		case NTOUCH_START:
			_lookfor_waypoint_elementB(pwpbuilder, pwpbuilder->m_TgtSize, pdata->m_pWorkbench->m_pPath, NGetEventTouchPositionPtr(pevent, 0), pselect, &localhitpoint);

			if (pselect->pSelected && NLOWORD(pselect->SelectParam)==APP_WAYPOINT_POSITION || NLOWORD(pselect->SelectParam) == APP_WAYPOINT_TANGENT)
			{
				return NEDITOR_RESULT_TOOL_ON;
			}
			else
			{
				NResetToolSelection(pselect);
			}
			break;

		case NTOUCH_MOVE:
			// The tool didn't do something, except un-select the splinepoint if there was one !
			NResetToolSelection(pselect);
			break;

		case NTOUCH_END:
			if(pselect->pSelected)
			{
				NEraseArrayElement(pwpbuilder->getArrayOfWayPoints(), NHIWORD(pselect->SelectParam), NULL);
				
				pselect->pSelectedOwner	= NULL;
				pselect->pSelected		= NULL;
				pselect->SelectParam	= 0;
				pselect->ZfromCam		= 0;
				pdata->m_pWorkbench->requestPathBuild();
				return NEDITOR_RESULT_TOOL_ON;
			}
			break;

		default:
			break;
	}
	return NEDITOR_RESULT_TOOL_OFF;
}

NEDITOR_RESULT App_WayPointsTool_Insert(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param)
{
	static	Nbool			bmoved = NFALSE;
	NVEC2f32				pos,v2;
	NLPATH_WAYPOINT			*pwp;
	Nu32					index;
	NVEC3					plane_point, plane_dir;
	NVEC3					mouseline_point, mouseline_dir;
	NVEC3					result;
	APP_WAYPOINT_TOOLDATA	*pdata		= (APP_WAYPOINT_TOOLDATA *)tool_param;
	NLPATH_BUILDER_WAYPOINTS* pwpbuilder = (NLPATH_BUILDER_WAYPOINTS*)pdata->m_pWorkbench->m_pPathBuilder;

	switch (pevent->Type)
	{
	case NTOUCH_START:
		NUT_Get3DMouseRay(&mouseline_point,&mouseline_dir,NULL, NGetEventTouchPositionPtr(pevent, 0));
		NGetEditorWorkPlaneNormalVector(&plane_dir);
		NGetEditorWorkPlanePoint(&plane_point);
		NUT_LineXPLane(&mouseline_point,&mouseline_dir,&plane_point,&plane_dir, &result);

		// CALL BACK "INSERT"

		pwp		= NULL;
		pos.x	= result.x;
		pos.y	= result.y;
		index	= pwpbuilder->getWayPointsNumber();
		pwp		= pwpbuilder->pushBackWayPoint(&pos);
		
		if(pwp)
		{
			pselect->pSelectedOwner	= (void*)pwpbuilder;
			pselect->pSelected		= (void*)pwp;
			pselect->SelectParam	= NMAKELONG(APP_WAYPOINT_POSITION,index);
			pselect->ZfromCam		= 0.0f; // Unused ?
			pdata->m_pWorkbench->requestPathBuild();
		}
		else
		{
			NResetToolSelection(pselect);
		}
		return NEDITOR_RESULT_TOOL_ON;
	
	case NTOUCH_MOVE:
		pwp = (NLPATH_WAYPOINT*)pselect->pSelected;
		if (pwp)
		{
			NGetEditorWorkPlaneNormalVector(&plane_dir);
			NGetEditorWorkPlanePoint(&plane_point);
			NUT_Get3DMouseRay(&mouseline_point, &mouseline_dir, NULL, NGetEventTouchPositionPtr(pevent, 0));

			if (NUT_LineXPLane(&mouseline_point, &mouseline_dir, &plane_point, &plane_dir, &result))
			{
				switch (NLOWORD(pselect->SelectParam))
				{
					case APP_WAYPOINT_POSITION:
						// Mouvements spéciaux, alignements:
						if (ISFLAG_ON(pdata->m_flags, FLAG_APP_WAYPOINT_TOOLDATA_SNAP_ON))
							_snap_waypoint_p(pwp, (const NVEC2f32 *)&result, pdata);
						else
						{
							pwp->p.x = result.x;
							pwp->p.y = result.y;
						}
						bmoved = NTRUE;
						pdata->m_pWorkbench->requestPathBuild();
						return NEDITOR_RESULT_TOOL_ON;

					case APP_WAYPOINT_TANGENT:
						if (ISFLAG_ON(pdata->m_flags, FLAG_APP_WAYPOINT_TOOLDATA_SNAP_ON))
							_snap_waypoint_u(pwp, (const NVEC2f32 *)&result, pdata);
						else
						{
							v2.x = result.x - pwp->p.x;
							v2.y = result.y - pwp->p.y;
							NVec2Normalize(&v2);
							pwp->u.x = v2.x;
							pwp->u.y = v2.y;
						}
						bmoved = NTRUE;
						pdata->m_pWorkbench->requestPathBuild();
						return NEDITOR_RESULT_TOOL_ON;

				default:
					break;
				}
			}
		}
		break;

	case NTOUCH_END:
		if (pselect->pSelected && bmoved)
		{
			// 				NCallCurrentToolCallBack(pevent);
			NResetToolSelection(pselect);
			bmoved = NFALSE;
			return NEDITOR_RESULT_TOOL_ON;
		}
		else
		{
			NResetToolSelection(pselect);
		}
	default:
		break;
	}
	return NEDITOR_RESULT_TOOL_OFF;
}



void App_WayPointsTool_TimeHandle(const NTIME* ptime, const NEDITOR_SELECTION *pselection, const Nu32 tool_param)
{
	APP_WAYPOINT_TOOLDATA		*pdata = (APP_WAYPOINT_TOOLDATA*)tool_param;
	NLPATH_BUILDER_WAYPOINTS	*pwpbuilder = (NLPATH_BUILDER_WAYPOINTS*)pdata->m_pWorkbench->m_pPathBuilder;
	NARRAY						*pwparray	= pwpbuilder->getArrayOfWayPoints();
	NARRAY						*psecarray = pwpbuilder->getArrayOfSections();

	NARRAY						*pprimarray = &pdata->m_pWorkbench->m_pPath->m_geometry.m_primitivesArray;
	NARRAY						*pkparray	= &pdata->m_pWorkbench->m_pPath->m_geometry.m_pathPointsArray;


	NLPATH_WAYPOINT				*pwp;
	NLPATH_POINT				*pkp;
	Nu32						i,j;
	NVEC3						v0, v1;
	NUT_DRAW_ELLIPSE			ellipse;

	NLPATH_PRIMITIVE			*pprim;

	NVEC2f32		xtd;

	NUT_DRAWTEXT	params;

	Nchar			txt[32];
	// +-------------------------------------------------------------------------------------------
	// |
	// |
	// |	MISE EN LUMIERE DU WAYPOINT SELECTIONNE
	// |	
	// |	L'élément spécifique en cours d'édition, c'est à dire la position (.p) , la tangente (.u) où les différents ratios ( ICRA, ICRC, BCR )
	// |	engendre un tracé additionnel spécifique. Ainsi quand l'utilisateur sélectionne le Waypoint pour le déplacer ( changer sa position ) 
	// |	Un Quad supplémentaire légèrement plus grand est dessiné en plus du Quad déjà en place et tracé plus tôt par la fonction PathBuilder.draw(...) 
	// |
	NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_3D);
	NUT_SetDrawConstructionPlane(_PLANE_XY);
	if (pselection->pSelected)
	{
		switch (NLOWORD(pselection->SelectParam))
		{
			case APP_WAYPOINT_POSITION:
				pwp = (NLPATH_WAYPOINT*)pselection->pSelected;
				v0.x = pwp->p.x;
				v0.y = pwp->p.y;
				v0.z = 0.0f;

				xtd = pwpbuilder->m_WayPointExtend;
				xtd.x *= 1.5f;
				xtd.y *= 1.5f;

				NUT_Draw_Quad(&v0, &xtd, &pdata->m_SelectionColorA);

				NSetColorf(&params.Color, NCOLOR_PRESET3F_WHITE, 1.0f);
				params.Size = 0.0125f;
				//NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
				sprintf(txt, "[%.3f , %.3f]", v0.x, v0.y);
				NUT_Draw_Text(txt, &v0, &params);
				break;

			case APP_WAYPOINT_TANGENT:
				pwp = (NLPATH_WAYPOINT*)pselection->pSelected;

				Nmem0(&ellipse, NUT_DRAW_ELLIPSE);
				ellipse.Color = pdata->m_SelectionColorA;
				ellipse.Extents.x	= pwpbuilder->m_WayPointExtend.x*1.75f;
				ellipse.Extents.y	= pwpbuilder->m_WayPointExtend.y*1.75f;
				ellipse.SliceNb		= 16;
				ellipse.Center.z	= 0.0f;
				ellipse.Center.x	= pwp->p.x;
				ellipse.Center.y	= pwp->p.y;
				NUT_Draw_Ellipse(&ellipse);

				v0.x = pwp->p.x;
				v0.y = pwp->p.y;
				v0.z = 0.0f;

				v1.x = pwp->p.x + pwp->u.x * pwpbuilder->m_TgtSize * 0.5f;
				v1.y = pwp->p.y + pwp->u.y * pwpbuilder->m_TgtSize * 0.5f;
				v1.z = 0.0f;

				NUT_Draw_DoubleSegment(&v0, &v1, 0.03f, &pdata->m_SelectionColorA);

				break;

			// ICRA ( Inserted Circle Ratio of section A )
			case APP_WAYPOINT_ICRA:
				NSetColorf(&params.Color, NCOLOR_PRESET3F_WHITE, 1.0f);
				params.Size = 0.0125f;
				NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
				NUT_Draw_SliderOnScreenPlane(&Slider, &params.Color, &params);
				break;

			// ICRC ( Inserted Circle Ratio of section C )
			case APP_WAYPOINT_ICRC:
				NSetColorf(&params.Color, NCOLOR_PRESET3F_WHITE, 1.0f);
				params.Size = 0.0125f;
				NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
				NUT_Draw_SliderOnScreenPlane(&Slider, &params.Color, &params);
				break;

			// BCR ( Build Circle Ratio )
			case APP_WAYPOINT_BCR:
				NSetColorf(&params.Color, NCOLOR_PRESET3F_WHITE, 1.0f);
				params.Size = 0.0125f;
				NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
				NUT_Draw_SliderOnScreenPlane(&Slider, &params.Color, &params);
				break;

			default:
				break;
		}
	}
	// |
	// |
	// +-------------------------------------------------------------------------------------------

	// +-------------------------------------------------------------------------------------------
	// |
	// |
	// |	TRACAGE DES LIGNES DE SNAP ...
	// |	
	if (ISFLAG_ON(pdata->m_flags, FLAG_APP_WAYPOINT_TOOLDATA_DRAW_HELPER_SNAPLINE0))
	{
		v0.x = pdata->m_SnapLine[0].m_p.x + pdata->m_SnapLine[0].m_u.x*APP_HELPERS_LINE_HALF_LENGTH;
		v0.y = pdata->m_SnapLine[0].m_p.y + pdata->m_SnapLine[0].m_u.y*APP_HELPERS_LINE_HALF_LENGTH;
		v0.z = 0.0f;

		v1.x = pdata->m_SnapLine[0].m_p.x - pdata->m_SnapLine[0].m_u.x*APP_HELPERS_LINE_HALF_LENGTH;
		v1.y = pdata->m_SnapLine[0].m_p.y - pdata->m_SnapLine[0].m_u.y*APP_HELPERS_LINE_HALF_LENGTH;
		v1.z = 0.0f;

		NUT_Draw_Segment(&v0, &v1, &pdata->m_HelpersColor);
	}

	if (ISFLAG_ON(pdata->m_flags, FLAG_APP_WAYPOINT_TOOLDATA_DRAW_HELPER_SNAPLINE1))
	{
		v0.x = pdata->m_SnapLine[1].m_p.x + pdata->m_SnapLine[1].m_u.x*APP_HELPERS_LINE_HALF_LENGTH;
		v0.y = pdata->m_SnapLine[1].m_p.y + pdata->m_SnapLine[1].m_u.y*APP_HELPERS_LINE_HALF_LENGTH;
		v0.z = 0.0f;

		v1.x = pdata->m_SnapLine[1].m_p.x - pdata->m_SnapLine[1].m_u.x*APP_HELPERS_LINE_HALF_LENGTH;
		v1.y = pdata->m_SnapLine[1].m_p.y - pdata->m_SnapLine[1].m_u.y*APP_HELPERS_LINE_HALF_LENGTH;
		v1.z = 0.0f;

		NUT_Draw_Segment(&v0, &v1, &pdata->m_HelpersColor);
	}
	// |
	// |
	// +-------------------------------------------------------------------------------------------


	// +-------------------------------------------------------------------------------------------
	// |
	// |
	// |	TRACAGE poignées ( Arc Handle ) permettant l'édition des 'Clothoides / Arcs de cercles insérés'.
	// |	L'utilisateur peut cliquer sur ces Poignées 
	// |
	// |
	// NLPATH_BARC barc;
	Nmem0(&ellipse, NUT_DRAW_ELLIPSE);
	ellipse.Color	= pdata->m_HelpersColor;
	ellipse.Extents = pdata->m_ICircleHandleExtend;
	ellipse.SliceNb = 16;
	ellipse.Center.z = 0.0f;
	pwp		= (NLPATH_WAYPOINT*)pwparray->pFirst;
	pkp		= (NLPATH_POINT*)pkparray->pFirst;
	pprim	= (NLPATH_PRIMITIVE*)pprimarray->pFirst;

	NLPATH_SECTION *psec = (NLPATH_SECTION*)psecarray->pFirst;
	Nu32	a, b, c;
	for (i = 0; i < psecarray->Size; j = GET_NLPATH_SECTION_PRIMITIVES_NB(psec->m_uid), pkp+=j,pprim+=j,i++, psec++,pwp++)
	{
		a = GET_NLPATH_SECTION_PRIMITIVES_NB_A(psec->m_uid);
		b = GET_NLPATH_SECTION_PRIMITIVES_NB_B(psec->m_uid);
		c = GET_NLPATH_SECTION_PRIMITIVES_NB_C(psec->m_uid);
		switch (a)
		{
			case 1:
				NL_ExtractArcHandle((NVEC2f32 *)&ellipse.Center, &pdata->m_pWorkbench->m_pPath->m_matrix, pprim);
				NUT_Draw_Ellipse(&ellipse);
				break;
		
			case 2:
				ellipse.Center.x = pkp[1].p.x;
				ellipse.Center.y = pkp[1].p.y;
				NUT_Draw_Ellipse(&ellipse);
				break;

			case 3:
				NL_ExtractArcHandle((NVEC2f32 *)&ellipse.Center, &pdata->m_pWorkbench->m_pPath->m_matrix, &pprim[1]);
				NUT_Draw_Ellipse(&ellipse);
				break;

			case 4:
				ellipse.Center.x = pkp[1].p.x;
				ellipse.Center.y = pkp[1].p.y;
				NUT_Draw_Ellipse(&ellipse);

				ellipse.Center.x = pkp[3].p.x;
				ellipse.Center.y = pkp[3].p.y;
				NUT_Draw_Ellipse(&ellipse);
				break;

			case 6:
				NL_ExtractArcHandle((NVEC2f32 *)&ellipse.Center, &pdata->m_pWorkbench->m_pPath->m_matrix, &pprim[1]);
				NUT_Draw_Ellipse(&ellipse);

				NL_ExtractArcHandle((NVEC2f32 *)&ellipse.Center, &pdata->m_pWorkbench->m_pPath->m_matrix, &pprim[4]);
				NUT_Draw_Ellipse(&ellipse);
				break;

			default:
				break;
		}

		switch (c)
		{
			case 1:
				NL_ExtractArcHandle((NVEC2f32 *)&ellipse.Center, &pdata->m_pWorkbench->m_pPath->m_matrix, &pprim[a + b]);
				NUT_Draw_Ellipse(&ellipse);
				break;
		
			case 2:
				ellipse.Center.x = pkp[a + b + 1].p.x;
				ellipse.Center.y = pkp[a + b + 1].p.y;
				NUT_Draw_Ellipse(&ellipse);
				break;

			case 3:
				NL_ExtractArcHandle((NVEC2f32 *)&ellipse.Center, &pdata->m_pWorkbench->m_pPath->m_matrix, &pprim[a+b+1]);
				NUT_Draw_Ellipse(&ellipse);
				break;

			case 4:
				ellipse.Center.x = pkp[a + b + 1].p.x;
				ellipse.Center.y = pkp[a + b + 1].p.y;
				NUT_Draw_Ellipse(&ellipse);

				ellipse.Center.x = pkp[a + b + 3].p.x;
				ellipse.Center.y = pkp[a + b + 3].p.y;
				NUT_Draw_Ellipse(&ellipse);
				break;

			case 6:
				NL_ExtractArcHandle((NVEC2f32 *)&ellipse.Center, &pdata->m_pWorkbench->m_pPath->m_matrix, &pprim[a + b + 1]);
				NUT_Draw_Ellipse(&ellipse);

				NL_ExtractArcHandle((NVEC2f32 *)&ellipse.Center, &pdata->m_pWorkbench->m_pPath->m_matrix, &pprim[a + b + 4]);
				NUT_Draw_Ellipse(&ellipse);
				break;

			default:
				break;
		}
	}
	// |
	// |
	// +-------------------------------------------------------------------------------------------
}


// -----------------------------------------------------------------------
#endif	//_NEDITOR
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


#include "../NL/NL2DOrthogonalCoordinateSystem.h"
#include "App_PolygonalChain_Tools.h"
#include "../NL/MotionControl/Path/Builders/PolygonalChain/NLPathBuilderPolygonalVertex.h"



NUT_SLIDER	PolygonalHandleSlider;		// Graphic Slider utilisé pour définir les valeurs rr et icr des vertex.


// Quelques "outils" à usage local uniquemnt:



APP_PLG_CHAIN_TOOLDATA* App_SetupPolygonalChainToolData(APP_PLG_CHAIN_TOOLDATA* pvpack, NLPATH_WORKBENCH* pworkbench)
{
	Nmem0(pvpack, APP_PLG_CHAIN_TOOLDATA);
	pvpack->m_flags				= 0;
	pvpack->m_pWorkbench		= pworkbench;

	NSetColorf(&pvpack->m_SelectionColorA, DEFAULT_APP_PLG_CHAIN_SELECTION_COLORA);
	NSetColorf(&pvpack->m_SelectionColorB, DEFAULT_APP_PLG_CHAIN_SELECTION_COLORB);

	return pvpack;
}

APP_PLG_CHAIN_TOOLDATA*	App_CreatePolygonalChainToolData(NLPATH_WORKBENCH* pworkbench)
{
	return App_SetupPolygonalChainToolData(NEW(APP_PLG_CHAIN_TOOLDATA), pworkbench);
}

void App_ClearPolygonalChainToolData(APP_PLG_CHAIN_TOOLDATA*pvpack)
{
	Nmem0(pvpack, APP_PLG_CHAIN_TOOLDATA);
}

void App_DeletePolygonalChainToolData(APP_PLG_CHAIN_TOOLDATA*pvpack)
{
	App_ClearPolygonalChainToolData(pvpack);
	Nfree(pvpack);
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
/*
#define FILL_SELECTION_AND_BREAK_THE_LOOP(pselect,ppath,ptr,tp,i){										\
														pselect->pSelectedOwner = (void*)ppath;			\
														pselect->pSelected		= (void*)ptr;			\
														pselect->SelectParam	= NMAKELONG(tp, i);		\
														i						= (NU32_MAX-1); }			// To stop the loop

*/
static inline void _lookfor_vertex_element(const APP_PLG_CHAIN_TOOLDATA* pdata,  const NVEC2s16 *ppointer, NEDITOR_SELECTION *pselect, NVEC3 *phitpoint)
{
	Nu32							i;
	NVEC3							mouselineA,mouselineB;
	NUT_3DLINEXLINE_RESULT			result;
	Nf32							squared_dist,min_squared_dist;
	NLPATH_BUILDER_POLYGONAL_CHAIN* pplgbuilder = (NLPATH_BUILDER_POLYGONAL_CHAIN*)pdata->m_pWorkbench->m_pPathBuilder;
	const Nf32						xtd = NVec2Length(&pplgbuilder->m_handleExtend);
	NVEC3							vp;
	NVEC3							AB;
	NLPATH_BUILDER_PLG_VERTEX		*pv;
	NVEC3							keep_hitpoint;
	NARRAY							*pvertexarray = pplgbuilder->getArrayOfVertex();
	// Full 3D research.
	// At this stage we just have a ray ( mouse 3D ray ) and an array of elements to look into...
	// So we are to determine the closest element to the mouse 3D ray and if this element is close enough
	// to the ray it will be the One !
	// Lets go !
	NResetToolSelection(pselect);
	NUT_Get3DMouseSegment(&mouselineA, &mouselineB, NULL, ppointer);
	NVec3Sub(&AB, &mouselineB, &mouselineA);

	min_squared_dist = xtd*xtd;
	pv = (NLPATH_BUILDER_PLG_VERTEX*)pvertexarray->pFirst;
	for(i=0;i< pvertexarray->Size;i++, pv ++)
	{
		//#0 POSITION
		vp.x = pv->p.x;
		vp.y = pv->p.y;
		vp.z = 0.0f;
		NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT *)&result.PointOnAB,&mouselineA,&AB,&vp);
		NVec3SubFrom(&vp,&result.PointOnAB);
		squared_dist = NVec3SquareLength(&vp);
		if(squared_dist<min_squared_dist)
		{
			min_squared_dist		= squared_dist;
			pselect->pSelectedOwner	= (void*)pplgbuilder;
			pselect->pSelected		= (void*)pv;
			pselect->SelectParam	= NMAKELONG(APP_PLG_VERTEX_POSITION, i );
			keep_hitpoint			= result.PointOnAB;
		}
		//#1 HANDLEA
		vp.x = pv->p.x - pv->bisector.x* pplgbuilder->m_handleLength  - pv->bisector.y*xtd ;
		vp.y = pv->p.y - pv->bisector.y* pplgbuilder->m_handleLength + pv->bisector.x*xtd;
		vp.z = 0.0f;
		NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT*)&result.PointOnAB, &mouselineA, &AB, &vp);
		NVec3SubFrom(&vp, &result.PointOnAB);
		squared_dist = NVec3SquareLength(&vp);
		if (squared_dist < min_squared_dist)
		{
			min_squared_dist = squared_dist;
			pselect->pSelectedOwner = (void*)pplgbuilder;
			pselect->pSelected = (void*)pv;
			pselect->SelectParam = NMAKELONG(APP_PLG_VERTEX_HANDLE_A, i);
			keep_hitpoint = result.PointOnAB;
		}
		//#1 HANDLEB
		vp.x = pv->p.x - pv->bisector.x * pplgbuilder->m_handleLength + pv->bisector.y * xtd;
		vp.y = pv->p.y - pv->bisector.y * pplgbuilder->m_handleLength - pv->bisector.x * xtd;
		vp.z = 0.0f;
		NUT_GetClosestPointOnLine((NUT_CLOSESTPOINT_ON_LINE_RESULT*)&result.PointOnAB, &mouselineA, &AB, &vp);
		NVec3SubFrom(&vp, &result.PointOnAB);
		squared_dist = NVec3SquareLength(&vp);
		if (squared_dist < min_squared_dist)
		{
			min_squared_dist = squared_dist;
			pselect->pSelectedOwner = (void*)pplgbuilder;
			pselect->pSelected = (void*)pv;
			pselect->SelectParam = NMAKELONG(APP_PLG_VERTEX_HANDLE_B, i);
			keep_hitpoint = result.PointOnAB;
		}
	}

	if (pselect->pSelected)
	{
		*phitpoint = keep_hitpoint;
	}
	return;
}



// ------------------------------------------------------------------------------------------
// App_PolygonalChainTool_Edit
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
NEDITOR_RESULT App_PolygonalChainTool_Edit(const NEVENT *pevent,NEDITOR_SELECTION *pselect,const Nu32 tool_param)
{
	static	Nbool						bmoved = NFALSE;
	static	NVEC3						hitpoint ={0,0,0};
	static	Nu32						touch_id = 0;

	static	Nf32						used_empty_space = 0.0f;
	static	Nf32						used_ratio = 0.0f;

	APP_PLG_CHAIN_TOOLDATA				*pdata = (APP_PLG_CHAIN_TOOLDATA*)tool_param;
	NLPATH_BUILDER_POLYGONAL_CHAIN		*pplgbuilder = (NLPATH_BUILDER_POLYGONAL_CHAIN*)pdata->m_pWorkbench->m_pPathBuilder;
	NARRAY								*pvertexarray = pplgbuilder->getArrayOfVertex();
	NLPATH_BUILDER_PLG_VERTEX			*pv, *pv0, *pv1;

	NVEC3								mouseray_origin,mouseray_dirvect;
	NVEC3								planenormal,mouse_on_workplane;
	NVEC2f32							sliderxtend;

	Nf32								empty_space_a, empty_space_b;

	switch (pevent->Type)			
	{
		case NTOUCH_START:
			bmoved = NFALSE;
			_lookfor_vertex_element(pdata, NGetEventTouchPositionPtr(pevent,0), pselect, &hitpoint);
			if (pselect->pSelected)
			{
				switch (NLOWORD(pselect->SelectParam))
				{
					// Definition/edition du rayon de l'arc de cercle de construction
					case  APP_PLG_VERTEX_HANDLE_A:
						pv = (NLPATH_BUILDER_PLG_VERTEX*)pselect->pSelected;//NGetArrayPtr(pvertexarray, NHIWORD(pselect->SelectParam));
						// Il n'est pas possible normalement d'avoir un handle A selectionnable pour un vertex autre qu'un vertex d'intérieur de chaine
						// ( les vertex isolés si ils peuvent exister, les vertex START et END ne peuvent pas avoir de poignée handle ... )
						NErrorIf(!FLAGS_TEST(pv->flags, MASK_NLPATH_BUILDER_PLG_VERTEX_CHAIN_EDGES, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_INSIDE_A_CHAIN), NERROR_INCONSISTENT_FLAGS);
						pv0 = pv - 1;
						pv1 = pv + 1;
						// Definition des "Espaces Disponibles" et choix du plus petit comme reference.
						empty_space_a = (pv->lA - pv0->l);
						empty_space_b = (pv->lB - pv1->l);

						used_empty_space = NMIN(empty_space_a, empty_space_b);
						if (used_empty_space)
							used_ratio = pv->l / used_empty_space;
						else
							used_ratio = 0.0f;


						sliderxtend.x = 0.3f;
						sliderxtend.y = 0.02f;
						NUT_SetupSlider(&PolygonalHandleSlider, 0.0f, 1.0f, NULL, &sliderxtend, 19, CONSTANT_NUT_SLIDER_SNAP_STRENGTH_MEDIUM, NULL);
						NUT_EnableSliderRelativeHandle(&PolygonalHandleSlider);
						NUT_SetSliderLockedDownRectf(&PolygonalHandleSlider, NCoord_NormToHRel_X(0.05f), NCoord_NormToHRel_X(0.95f), NCoord_NormToHRel_Y(-0.95f), NCoord_NormToHRel_Y(-0.05f), NTRUE);
						NUT_PlaceSliderOnScreenPlane(&PolygonalHandleSlider, used_ratio, NGetEventTouchPositionPtr(pevent, 0));
						break;
					
					// Definition/edition de la longueur des clothoides
					case  APP_PLG_VERTEX_HANDLE_B:
						pv = (NLPATH_BUILDER_PLG_VERTEX*)pselect->pSelected;//NGetArrayPtr(pvertexarray, NHIWORD(pselect->SelectParam));
						sliderxtend.x = 0.3f;
						sliderxtend.y = 0.02f;
						NUT_SetupSlider(&PolygonalHandleSlider, 0.0f, 1.0f, NULL, &sliderxtend, 19, CONSTANT_NUT_SLIDER_SNAP_STRENGTH_MEDIUM, NULL);
						NUT_EnableSliderRelativeHandle(&PolygonalHandleSlider);
						NUT_SetSliderLockedDownRectf(&PolygonalHandleSlider, NCoord_NormToHRel_X(0.05f), NCoord_NormToHRel_X(0.95f), NCoord_NormToHRel_Y(-0.95f), NCoord_NormToHRel_Y(-0.05f), NTRUE);
						NUT_PlaceSliderOnScreenPlane(&PolygonalHandleSlider, pv->icr, NGetEventTouchPositionPtr(pevent, 0));
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
					case APP_PLG_VERTEX_POSITION:
						//pv = (NLPATH_BUILDER_PLG_VERTEX*)pselect->pSelected;//NGetArrayPtr(pvertexarray, NHIWORD(pselect->SelectParam));
						// Mouvements spéciaux, alignements:
						//pv->p.x = mouse_on_workplane.x;
						//pv->p.y = mouse_on_workplane.y;
						//pdata->m_pPathBuilder->setVertexPosition((NLPATH_BUILDER_PLG_VERTEX*)pselect->pSelected, (NVEC2f32*)&mouse_on_workplane);
						((NLPATH_BUILDER_PLG_VERTEX*)pselect->pSelected)->setPosition((NVEC2f32*)&mouse_on_workplane);
						bmoved = NTRUE;
						pdata->m_pWorkbench->requestPathBuild();
						return NEDITOR_RESULT_TOOL_ON;

					// Definition/edition du rayon de l'arc de cercle de construction
					case APP_PLG_VERTEX_HANDLE_A:
						pv = (NLPATH_BUILDER_PLG_VERTEX*)pselect->pSelected;
						// Il n'est pas possible normalement d'avoir un handle A selectionnable pour un vertex autre qu'un vertex d'intérieur de chaine
						// ( les vertex isolés si ils peuvent exister, les vertex START et END ne peuvent pas avoir de poignée handle ... )
						NErrorIf(!FLAGS_TEST(pv->flags, MASK_NLPATH_BUILDER_PLG_VERTEX_CHAIN_EDGES, FLAG_NLPATH_BUILDER_PLG_VERTEX_IS_INSIDE_A_CHAIN), NERROR_INCONSISTENT_FLAGS);
						
						used_ratio = NUT_UpdateSliderOnScreenPlane(&PolygonalHandleSlider, NGetEventTouchPositionPtr(pevent, 0));

						pv0 = pv - 1;
						pv1 = pv + 1;
						
						pv->tl = pv->l = used_empty_space * used_ratio;
						
						bmoved = NTRUE;
						pdata->m_pWorkbench->requestPathBuild();
						return NEDITOR_RESULT_TOOL_ON;

					case APP_PLG_VERTEX_HANDLE_B:
						pv = (NLPATH_BUILDER_PLG_VERTEX*)pselect->pSelected;
						pv->icr = NUT_UpdateSliderOnScreenPlane(&PolygonalHandleSlider, NGetEventTouchPositionPtr(pevent, 0));
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
			if( pselect->pSelected && bmoved )
			{
				if ( NLOWORD(pselect->SelectParam) == APP_PLG_VERTEX_HANDLE_A || NLOWORD(pselect->SelectParam) == APP_PLG_VERTEX_HANDLE_B)
					NUT_ClearSlider(&PolygonalHandleSlider);
				
				NResetToolSelection(pselect);
				bmoved = NFALSE;
				return NEDITOR_RESULT_TOOL_ON;
			}
			else
			{
				if (NLOWORD(pselect->SelectParam) == APP_PLG_VERTEX_HANDLE_A || NLOWORD(pselect->SelectParam) == APP_PLG_VERTEX_HANDLE_B)
					NUT_ClearSlider(&PolygonalHandleSlider);

				NResetToolSelection(pselect);
			}
			break;

		default:
			break;
	}


	return NEDITOR_RESULT_TOOL_OFF;
}


// ------------------------------------------------------------------------------------------
// App_PolygonalChainTool_Erase
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
NEDITOR_RESULT App_PolygonalChainTool_Erase(const NEVENT *pevent,NEDITOR_SELECTION *pselect, const Nu32 tool_param )
{
	APP_PLG_CHAIN_TOOLDATA			*pdata = (APP_PLG_CHAIN_TOOLDATA*)tool_param;
	NLPATH_BUILDER_POLYGONAL_CHAIN	*pplgbuilder = (NLPATH_BUILDER_POLYGONAL_CHAIN*)pdata->m_pWorkbench->m_pPathBuilder;
	NVEC3f32					localhitpoint;

	#ifdef _DEBUG
	Nu32 bdel = 0;
	#endif

	switch(pevent->Type)
	{
		case NTOUCH_START:
			_lookfor_vertex_element(pdata, NGetEventTouchPositionPtr(pevent, 0), pselect, &localhitpoint);

			if (NLOWORD(pselect->SelectParam) == APP_PLG_VERTEX_POSITION || NLOWORD(pselect->SelectParam) == APP_PLG_VERTEX_HANDLE_A || NLOWORD(pselect->SelectParam) == APP_PLG_VERTEX_HANDLE_B)
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
				pplgbuilder->eraseVertex(NHIWORD(pselect->SelectParam));
				//NEraseArrayElement(pdata->m_pPathBuilder->getArrayOfVertex(), NHIWORD(pselect->SelectParam), NULL);
				
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

NEDITOR_RESULT App_PolygonalChainTool_Create(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param)
{
	static	Nbool			bmoved = NFALSE;
	NVEC2f32				pos;
	NLPATH_BUILDER_PLG_VERTEX	*pv;
	Nu32					index;
	NVEC3					plane_point, plane_dir;
	NVEC3					mouseline_point, mouseline_dir;
	NVEC3					result;
	APP_PLG_CHAIN_TOOLDATA			*pdata		 = (APP_PLG_CHAIN_TOOLDATA*)tool_param;
	NLPATH_BUILDER_POLYGONAL_CHAIN	*pplgbuilder = (NLPATH_BUILDER_POLYGONAL_CHAIN*)pdata->m_pWorkbench->m_pPathBuilder;
	NARRAY					*pvertexarray	= pplgbuilder->getArrayOfVertex();

	switch (pevent->Type)
	{
	case NTOUCH_START:
		NUT_Get3DMouseRay(&mouseline_point,&mouseline_dir,NULL, NGetEventTouchPositionPtr(pevent, 0));
		NGetEditorWorkPlaneNormalVector(&plane_dir);
		NGetEditorWorkPlanePoint(&plane_point);
		NUT_LineXPLane(&mouseline_point,&mouseline_dir,&plane_point,&plane_dir, &result);

		// CALL BACK "INSERT"

		pv		= NULL;
		pos.x	= result.x;
		pos.y	= result.y;
		index	= pvertexarray->Size;
		pv		= pplgbuilder->pushBackVertex(&pos);
		if(pv)
		{
			//FLAG_ON(pv->flags, FLAG_NLPATH_BUILDER_PLG_VERTEX_RELATIVE_ARC_RADIUS);

			pselect->pSelectedOwner	= (void*)pplgbuilder;
			pselect->pSelected		= (void*)pv;
			pselect->SelectParam	= NMAKELONG(APP_PLG_VERTEX_POSITION,index);
			pselect->ZfromCam		= 0.0f; // Unused ?

			pdata->m_pWorkbench->requestPathBuild();
		}
		else
		{
			NResetToolSelection(pselect);
		}
		return NEDITOR_RESULT_TOOL_ON;
	
	case NTOUCH_MOVE:
		//pv = (NLPATH_BUILDER_PLG_VERTEX*)pselect->pSelected;
		if (pselect->pSelected)
		{
			NGetEditorWorkPlaneNormalVector(&plane_dir);
			NGetEditorWorkPlanePoint(&plane_point);
			NUT_Get3DMouseRay(&mouseline_point, &mouseline_dir, NULL, NGetEventTouchPositionPtr(pevent, 0));

			if (NUT_LineXPLane(&mouseline_point, &mouseline_dir, &plane_point, &plane_dir, &result))
			{
				switch (NLOWORD(pselect->SelectParam))
				{
					case APP_PLG_VERTEX_POSITION:
						//pv->p.x = result.x;
						//pv->p.y = result.y;
						//pdata->m_pPathBuilder->setVertexPosition((NLPATH_BUILDER_PLG_VERTEX*)pselect->pSelected, (NVEC2f32*)&result);
						((NLPATH_BUILDER_PLG_VERTEX*)pselect->pSelected)->setPosition((NVEC2f32*)&result);
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



void App_PolygonalChainTool_TimeHandle(const NTIME* ptime, const NEDITOR_SELECTION *pselect, const Nu32 tool_param)
{
	NVEC3f32					v0;
	NVEC2f32					 vxtd;
	NUT_DRAW_ELLIPSE			ellipse;

	NLPATH_BUILDER_PLG_VERTEX	*pv;
	APP_PLG_CHAIN_TOOLDATA			*pdata = (APP_PLG_CHAIN_TOOLDATA*)tool_param;
	NLPATH_BUILDER_POLYGONAL_CHAIN	*pplgbuilder = (NLPATH_BUILDER_POLYGONAL_CHAIN*)pdata->m_pWorkbench->m_pPathBuilder;
	const Nf32					xtd = NVec2Length(&pplgbuilder->m_handleExtend);

	NUT_DRAWTEXT				params;
	Nchar			txt[32];

	NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_3D);
	NUT_SetDrawConstructionPlane(_PLANE_XY);

	if (pselect->pSelected)
	{
		switch (NLOWORD(pselect->SelectParam))
		{
			case APP_PLG_VERTEX_POSITION:
				pv = (NLPATH_BUILDER_PLG_VERTEX*)pselect->pSelected;
				
				v0.x = pv->p.x;
				v0.y = pv->p.y;
				v0.z = 0.0f;

				vxtd = pplgbuilder->m_handleExtend;
				vxtd.x *= 1.5f;
				vxtd.y *= 1.5f;

				NUT_Draw_Quad(&v0, &vxtd, &pdata->m_SelectionColorA);

				NSetColorf(&params.Color, NCOLOR_PRESET3F_WHITE, 1.0f);
				params.Size = 0.125f;
				//NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
				sprintf(txt, "[%.3f , %.3f]", v0.x, v0.y);
				NUT_Draw_Text(txt, &v0, &params);
				break;

			case APP_PLG_VERTEX_HANDLE_A:
				pv = (NLPATH_BUILDER_PLG_VERTEX*)pselect->pSelected;

				Nmem0(&ellipse, NUT_DRAW_ELLIPSE);
				ellipse.Color		= pdata->m_SelectionColorA;
				ellipse.Extents.x	= xtd;
				ellipse.Extents.y	= xtd;
				ellipse.SliceNb		= 16;
				ellipse.Center.z	= 0.0f;
				ellipse.Center.x	= pv->p.x - pv->bisector.x * pplgbuilder->m_handleLength - pv->bisector.y * xtd;
				ellipse.Center.y	= pv->p.y - pv->bisector.y * pplgbuilder->m_handleLength + pv->bisector.x * xtd;
				NUT_Draw_Ellipse(&ellipse);

				NSetColorf(&params.Color, NCOLOR_PRESET3F_WHITE, 1.0f);
				params.Size = 0.0125f;
				NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
				NUT_Draw_SliderOnScreenPlane(&PolygonalHandleSlider, &params.Color, &params);
				NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_3D);
				break;

			case APP_PLG_VERTEX_HANDLE_B:
				pv = (NLPATH_BUILDER_PLG_VERTEX*)pselect->pSelected;
				Nmem0(&ellipse, NUT_DRAW_ELLIPSE);
				ellipse.Color = pdata->m_SelectionColorA;
				ellipse.Extents.x = pplgbuilder->m_handleExtend.x;
				ellipse.Extents.y = pplgbuilder->m_handleExtend.y;
				ellipse.SliceNb = 16;
				ellipse.Center.z = 0.0f;
				ellipse.Center.x = pv->p.x - pv->bisector.x * pplgbuilder->m_handleLength + pv->bisector.y * pplgbuilder->m_handleExtend.x;
				ellipse.Center.y = pv->p.y - pv->bisector.y * pplgbuilder->m_handleLength - pv->bisector.x * pplgbuilder->m_handleExtend.y;
				NUT_Draw_Ellipse(&ellipse);

				NSetColorf(&params.Color, NCOLOR_PRESET3F_WHITE, 1.0f);
				params.Size = 0.0125f;
				NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_ORTHOGRAPHIC);
				NUT_Draw_SliderOnScreenPlane(&PolygonalHandleSlider, &params.Color, &params);
				NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_3D);
				break;

			default:
				break;
		}
	}


	if (pplgbuilder)
		pplgbuilder->draw();
}


// -----------------------------------------------------------------------
#endif	//_NEDITOR
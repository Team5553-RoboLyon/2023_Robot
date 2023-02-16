#ifdef _NEDITOR
// -----------------------------------------------------------------------
#include "../../N/NCStandard.h"
#include "../../N/UI/NUI.h"
#include "../../N/NCamera.h"
#include "../../N/Maths/NVec3f32.h"
#include "../../N/NCoordinateSystemConversions.h"
#include "../../N/NEditor.h"
#include "../../N/Utilities/NUT_X.h"
#include "../../N/Utilities/NUT_3DMouse.h"
#include "../../N/Utilities/Maths/NUT_MathsMisc.h"
#include "../../N/Utilities/Draw/NUT_Draw.h"
#include "../../N/Utilities/Draw/NUT_DrawPencil.h"
#include "../../N/Event/NEventTouch.h"
#include "../../N/Maths/NMatrix3x3.h"
#include "../../N/Maths/NVecLimits.h"
#include "../../N/NMath.h"

#include"../../NL/MotionControl/Trajectory/NLVstage.h"
#include "../../NL/MotionControl/NLPathWorkbench.h"

#include "../../NL/NL2DOrthogonalCoordinateSystem.h"

#include "App_UserVZones_Tools.h"



APP_USERVZONE_TOOLDATA::APP_USERVZONE_TOOLDATA()
{
	Nmem0(this, APP_USERVZONE_TOOLDATA);
	m_SnapThreshold = DEFAULT_APP_USERVZONE_SNAP_S_THRESHOLD;
}

APP_USERVZONE_TOOLDATA::APP_USERVZONE_TOOLDATA(NLPATH_WORKBENCH* pwb)
{
	Nmem0(this, APP_USERVZONE_TOOLDATA);
	m_pWorkbench = pwb;
	m_SnapThreshold = DEFAULT_APP_USERVZONE_SNAP_S_THRESHOLD;
}

APP_USERVZONE_TOOLDATA::~APP_USERVZONE_TOOLDATA()
{
}

NLPATH_POINT* APP_USERVZONE_TOOLDATA::getClosestPointOnCurrentPath(const NVEC2s16* p, const Nf32 max_path_dist)
{
//	if (!m_pWorkbench)
//		return NULL;
	return NULL;

/*
	NVEC3					plane_point, plane_dir;
	NVEC3					mouseline_point, mouseline_dir;
	NVEC3f32				hitpoint;
	NVEC2f32				local_hitpoint;

	NUT_Get3DMouseRay(&mouseline_point, &mouseline_dir, NULL, p);
	NGetEditorWorkPlaneNormalVector(&plane_dir);
	NGetEditorWorkPlanePoint(&plane_point);
	NUT_LineXPLane(&mouseline_point, &mouseline_dir, &plane_point, &plane_dir, &hitpoint);
	// Recherche du point le + proche sur le Path
	// 1/ on exprime le hit point dans la base du chemin:
	m_pWorkbench->getPath()->convertGlobalPositionToLocal(&local_hitpoint, (NVEC2f32*)&hitpoint);
	
	// 2/ on recherche le point le plus proche sur le chemin
	m_PersistentTrackingData.forwardSetup(&m_pWorkbench->getPath()->m_geometry, max_path_dist);
	m_PersistentTrackingData.trackForward((NVEC2f32*)&local_hitpoint);

	return &m_PersistentTrackingData.m_pointOnPath;
*/

/*
	// 3/	on verifie que ces deux points ( le hitpoint et point le plus proche sur le chemin ) sont suffisement proche l'un de l'autre
	//		Ce qui revient à vérifier que l'utilisateur à cliquer suffisament près du chemin.
	v.x = m_PersistentTrackingData.m_pointOnPath.p.x - local_hitpoint.x;
	v.y = m_PersistentTrackingData.m_pointOnPath.p.y - local_hitpoint.y;
	square_dist = NVec2SquareLength(&v);
	if (square_dist < max_path_square_dist)
	{
		if (m_PersistentTrackingData.m_pTrackedKeyPoint->s - m_PersistentTrackingData.m_pointOnPath.s < m_SnapThreshold) // pdata->m_PersistentTrackingData.m_pointOnPath.s est le second point de la primitive dont est issus la solution.
		{
			 return m_PersistentTrackingData.m_pTrackedKeyPoint;
		}
		else if (m_PersistentTrackingData.m_pointOnPath.s - (m_PersistentTrackingData.m_pTrackedKeyPoint - 1)->s < m_SnapThreshold)
		{
			return m_PersistentTrackingData.m_pTrackedKeyPoint - 1;
		}
		else
		{
			return &m_PersistentTrackingData.m_pointOnPath;
		}
	}
	else
	{
		return NULL;
	}
*/
}


// ------------------------------------------------------------------------------------------
// App_UserVZoneTool_Insert
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
// ------------------------------------------------------------------------------------------
NEDITOR_RESULT App_UserVZoneTool_Insert(const NEVENT *pevent,NEDITOR_SELECTION *pselect, const Nu32 tool_param )
{
	APP_USERVZONE_TOOLDATA	*pdata;

	NLPATH_POINT	path_point;

	NVEC3f32				hitpoint;
	NVEC2f32				local_hitpoint;

	NVEC3					plane_point, plane_dir;
	NVEC3					mouseline_point, mouseline_dir;

	NLVSTAGE			vsdesc;

	#ifdef _DEBUG
	Nu32 bdel = 0;
	#endif

	switch(pevent->Type)
	{
		case NTOUCH_START:
			pdata = (APP_USERVZONE_TOOLDATA*)tool_param;
			FLAG_OFF(pdata->m_flags, FLAG_APP_USERVZONE_TOOLDATA_VALID_USERKEY);
			FLAG_ON(pdata->m_flags, FLAG_APP_USERVZONE_TOOLDATA_EDIT_INTERVAL_END);
			NUT_Get3DMouseRay(&mouseline_point, &mouseline_dir, NULL, NGetEventTouchPositionPtr(pevent, 0));
			NGetEditorWorkPlaneNormalVector(&plane_dir);
			NGetEditorWorkPlanePoint(&plane_point);
			NUT_LineXPLane(&mouseline_point, &mouseline_dir, &plane_point, &plane_dir, &hitpoint);
			pdata->m_pWorkbench->getPath()->convertGlobalPositionToLocal(&local_hitpoint, (NVEC2f32*)&hitpoint);

			// 2/ on recherche le point le plus proche sur le chemin
			pdata->m_PersistentTrackingData.forwardSetup(&pdata->m_pWorkbench->getPath()->m_geometry, APP_USERVZONE_DETECTION_DISTANCE);
			if (pdata->m_PersistentTrackingData.trackForward(&pdata->m_Start, (NVEC2f32*)&local_hitpoint) != NLPATH_PERSISTENT_TRACKING_DATA::TRCK_RESULT::NOT_DETECTED)
			{
				FLAG_ON(pdata->m_flags, FLAG_APP_USERVZONE_TOOLDATA_INSERT_STARTED);
				return NEDITOR_RESULT_TOOL_ON;
			}
			else
			{
				FLAG_OFF(pdata->m_flags, FLAG_APP_USERVZONE_TOOLDATA_INSERT_STARTED);
			}
			break;

		case NTOUCH_MOVE:
			pdata = (APP_USERVZONE_TOOLDATA*)tool_param;
			if (ISFLAG_ON(pdata->m_flags, FLAG_APP_USERVZONE_TOOLDATA_INSERT_STARTED))
			{
				NUT_Get3DMouseRay(&mouseline_point, &mouseline_dir, NULL, NGetEventTouchPositionPtr(pevent, 0));
				NGetEditorWorkPlaneNormalVector(&plane_dir);
				NGetEditorWorkPlanePoint(&plane_point);
				NUT_LineXPLane(&mouseline_point, &mouseline_dir, &plane_point, &plane_dir, &hitpoint);
				pdata->m_pWorkbench->getPath()->convertGlobalPositionToLocal(&local_hitpoint, (NVEC2f32*)&hitpoint);

				pdata->m_PersistentTrackingData.reset();
				if (pdata->m_PersistentTrackingData.trackForward(&path_point, (NVEC2f32*)&local_hitpoint) != NLPATH_PERSISTENT_TRACKING_DATA::TRCK_RESULT::NOT_DETECTED)
				{
					if (ISFLAG_ON(pdata->m_flags, FLAG_APP_USERVZONE_TOOLDATA_EDIT_INTERVAL_END))
					{
						if (path_point.s >= pdata->m_Start.s)
						{
							pdata->m_End = path_point;
						}
						else
						{
							pdata->m_End	= pdata->m_Start;
							pdata->m_Start	= path_point;
							FLAG_OFF(pdata->m_flags, FLAG_APP_USERVZONE_TOOLDATA_EDIT_INTERVAL_END);
						}
					}
					else// !ISFLAG_ON(pdata->m_flags, FLAG_APP_USERVZONE_TOOLDATA_EDIT_INTERVAL_END)
					{
						if (path_point.s <= pdata->m_End.s)
						{
							pdata->m_Start = path_point;
						}
						else
						{
							pdata->m_Start = pdata->m_End;
							pdata->m_End = path_point;

							FLAG_ON(pdata->m_flags, FLAG_APP_USERVZONE_TOOLDATA_EDIT_INTERVAL_END);
						}
					}
					FLAG_ON(pdata->m_flags, FLAG_APP_USERVZONE_TOOLDATA_INSERT_ENDED);
				}
				else
				{
					FLAG_OFF(pdata->m_flags, FLAG_APP_USERVZONE_TOOLDATA_INSERT_ENDED);
				}
				return NEDITOR_RESULT_TOOL_ON;
			}
			break;

		case NTOUCH_END:
			pdata = (APP_USERVZONE_TOOLDATA*)tool_param;
			if (FLAGS_TEST(pdata->m_flags, MASK_APP_USERVZONE_TOOLDATA_ALL_INSERT_FLAGS, MASK_APP_USERVZONE_TOOLDATA_ALL_INSERT_FLAGS))
			{
				NErrorIf(pdata->m_Start.s > pdata->m_End.s, NERROR_SYSTEM_GURU_MEDITATION);
				
				if (pdata->m_End.s > pdata->m_Start.s)
				{
					vsdesc.set(pdata->m_Start.s, pdata->m_End.s, 0.5f,NLVSTAGE::RELATIVE_VALUE);
					pdata->m_pWorkbench->getTrajectory()->insertUserVelocityStage(&vsdesc);
					pdata->m_pWorkbench->requestTrajectoryBuild();
				}
				
				FLAG_ON(pdata->m_flags, FLAG_APP_USERVZONE_TOOLDATA_VALID_USERKEY);
			}
			else
			{
				FLAG_OFF(pdata->m_flags, FLAG_APP_USERVZONE_TOOLDATA_VALID_USERKEY);
			}
			FLAG_OFF(pdata->m_flags, FLAG_APP_USERVZONE_TOOLDATA_INSERT_STARTED);
			FLAG_OFF(pdata->m_flags, FLAG_APP_USERVZONE_TOOLDATA_INSERT_ENDED);
			return NEDITOR_RESULT_TOOL_ON;
			
			break;

		default:
			break;
	}
	return NEDITOR_RESULT_TOOL_OFF;
}

NEDITOR_RESULT App_UserVZoneTool_Edit(const NEVENT* pevent, NEDITOR_SELECTION* pselect, const Nu32 tool_param)
{
	return NEDITOR_RESULT_TOOL_OFF;
}

NEDITOR_RESULT App_UserVZoneTool_Delete(const NEVENT* pevent, NEDITOR_SELECTION* pselect, const Nu32 tool_param)
{
	return NEDITOR_RESULT_TOOL_OFF;
}

void draw_open_bracket(NLPATH *ppath, NLPATH_POINT *pp, const NVEC2f32 *pxtd, const NCOLOR *pcol)
{
	NUTDRAWVERTEX vrtx;
	NVEC2 u;
	NVEC2 o;
	vrtx.Color0_4f = *pcol;

	ppath->convertLocalPositionToGlobal(&o, &pp->p);
	ppath->convertLocalDirectionToGlobal(&u, &pp->u);
	vrtx.Position_3f.z = 0.0f;

	//		1---0
	//		|
	//	    o	
	//		|
	//		2---3

	// 0
	vrtx.Position_3f.x = o.x - pxtd->y * u.y + pxtd->x * u.x;
	vrtx.Position_3f.y = o.y + pxtd->y * u.x + pxtd->x * u.y;
	NUT_DrawPencil_From(&vrtx);
	// 1
	vrtx.Position_3f.x = o.x - pxtd->y * u.y;
	vrtx.Position_3f.y = o.y + pxtd->y * u.x;
	NUT_DrawPencil_LineTo(&vrtx);
	// 2
	vrtx.Position_3f.x = o.x + pxtd->y * u.y;
	vrtx.Position_3f.y = o.y - pxtd->y * u.x;
	NUT_DrawPencil_LineTo(&vrtx);
	// 3
	vrtx.Position_3f.x = o.x + pxtd->y * u.y + pxtd->x * u.x;
	vrtx.Position_3f.y = o.y - pxtd->y * u.x + pxtd->x * u.y;
	NUT_DrawPencil_LineTo(&vrtx);
}

void draw_close_bracket(NLPATH* ppath, NLPATH_POINT* pp, const NVEC2f32* pxtd, const NCOLOR* pcol)
{
	NUTDRAWVERTEX vrtx;
	NVEC2 u;
	NVEC2 o;
	vrtx.Color0_4f = *pcol;

	ppath->convertLocalPositionToGlobal(&o, &pp->p);
	ppath->convertLocalDirectionToGlobal(&u, &pp->u);
	vrtx.Position_3f.z = 0.0f;

	//		1---0
	//		|
	//	    o	
	//		|
	//		2---3

	// 0
	vrtx.Position_3f.x = o.x - pxtd->y * u.y - pxtd->x * u.x;
	vrtx.Position_3f.y = o.y + pxtd->y * u.x - pxtd->x * u.y;
	NUT_DrawPencil_From(&vrtx);
	// 1
	vrtx.Position_3f.x = o.x - pxtd->y * u.y;
	vrtx.Position_3f.y = o.y + pxtd->y * u.x;
	NUT_DrawPencil_LineTo(&vrtx);
	// 2
	vrtx.Position_3f.x = o.x + pxtd->y * u.y;
	vrtx.Position_3f.y = o.y - pxtd->y * u.x;
	NUT_DrawPencil_LineTo(&vrtx);
	// 3
	vrtx.Position_3f.x = o.x + pxtd->y * u.y - pxtd->x * u.x;
	vrtx.Position_3f.y = o.y - pxtd->y * u.x - pxtd->x * u.y;
	NUT_DrawPencil_LineTo(&vrtx);
}

void App_UserVZoneTool_TimeHandle(const NTIME* ptime, const NEDITOR_SELECTION *pselection, const Nu32 tool_param)
{
	APP_USERVZONE_TOOLDATA*pdata = (APP_USERVZONE_TOOLDATA*)tool_param;
	NCOLOR						color = { NCOLOR_PRESET3F_BRONZE_GOLD,1.0f };

	NVEC2						xtd = { 0.1f,0.1f };
	NVEC3						o;


	NVEC3						v;
	NLPATH_POINT				point;


	v.z = 0.0f;
	o.z = 0.0f;

	NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_3D);
	NUT_SetDrawConstructionPlane(_PLANE_XY);

	pdata->m_pWorkbench->getTrajectory()->drawLimitedUserVStagesArray(0.1f,0);

	if (ISFLAG_ON(pdata->m_flags, FLAG_APP_USERVZONE_TOOLDATA_VALID_USERKEY))
	{
		pdata->m_pWorkbench->getPath()->m_geometry.getPathPoint(&point, pdata->m_Start.s);
		pdata->m_pWorkbench->getPath()->convertLocalPositionToGlobal((NVEC2*)&o, &point.p);
		NUT_Draw_Cross(&o, &xtd, &color);

		pdata->m_pWorkbench->getPath()->m_geometry.getPathPoint(&point, pdata->m_End.s);
		pdata->m_pWorkbench->getPath()->convertLocalPositionToGlobal((NVEC2*)&o, &point.p);
		NUT_Draw_Cross(&o, &xtd, &color);


		draw_open_bracket(pdata->m_pWorkbench->getPath(), &pdata->m_Start, &xtd, &color);
		draw_close_bracket(pdata->m_pWorkbench->getPath(), &pdata->m_End, &xtd, &color);
		/*
		pdata->m_pWorkbench->getPath()->convertLocalPositionToGlobal((NVEC2f32*)&v, &pdata->m_Start.p);
		NUT_Draw_Cross(&v, &xtd, &color);
		pdata->m_pWorkbench->getPath()->convertLocalPositionToGlobal((NVEC2f32*)&v, &pdata->m_End.p);
		NUT_Draw_Cross(&v, &xtd, &color);
		*/
	}
	else
	{
		if (ISFLAG_ON(pdata->m_flags, FLAG_APP_USERVZONE_TOOLDATA_INSERT_STARTED))
		{
			pdata->m_pWorkbench->getPath()->m_geometry.getPathPoint(&point, pdata->m_Start.s);
			pdata->m_pWorkbench->getPath()->convertLocalPositionToGlobal((NVEC2*)&o, &point.p);
			NUT_Draw_Cross(&o, &xtd, &color);

			draw_open_bracket(pdata->m_pWorkbench->getPath(), &pdata->m_Start, &xtd, &color);

//			pdata->m_pWorkbench->getPath()->convertLocalPositionToGlobal((NVEC2f32*)&v, &pdata->m_Start.p);
//			NUT_Draw_Cross(&v, &xtd, &color);
		}

		if (ISFLAG_ON(pdata->m_flags, FLAG_APP_USERVZONE_TOOLDATA_INSERT_ENDED))
		{
			pdata->m_pWorkbench->getPath()->m_geometry.getPathPoint(&point, pdata->m_End.s);
			pdata->m_pWorkbench->getPath()->convertLocalPositionToGlobal((NVEC2*)&o, &point.p);
			NUT_Draw_Cross(&o, &xtd, &color);

			draw_close_bracket(pdata->m_pWorkbench->getPath(), &pdata->m_End, &xtd, &color);
			
		//	pdata->m_pWorkbench->getPath()->convertLocalPositionToGlobal((NVEC2f32*)&v, &pdata->m_End.p);
		//	NUT_Draw_Cross(&v, &xtd, &color);
		}
	}
	

}


// -----------------------------------------------------------------------
#endif	//_NEDITOR

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


#include "../NL/MotionControl/NLPathWorkbench.h"

#include "../NL/NL2DOrthogonalCoordinateSystem.h"

#include "App_TestAndDebug_Tools.h"


/*
// Differential Steering Joystick Algorithm
// ========================================
// Converts a single dual-axis joystick into a differential
// drive motor control, with support for both drive, turn
// and pivot operations.
//

typedef struct VA	VA;
struct VA
{
	double m_speed;
	double m_acceleration;
};

void getSpeedsAndAccelerations(VA *pva_left, VA *pva_right, const VA *pvamax, const double jx, const double jy)
{
	double premix_left;		//	(left) premixed output(-1.. + 1)
	double premix_right;	//	(right)premixed output(-1.. + 1)

	double omega;			//	pivot speed
	double blend;

	// blend_threshold  : The threshold at which the pivot action starts
	//                This threshold is measured in units on the Y-axis
	//                away from the X-axis (Y=0). A greater value will assign
	//                more of the joystick's range to pivot actions.
	//                Allowable range: (0..1)
	double blend_threshold  = 0.25;

	if (jy >= 0)
	{
		// Forward
		premix_left		= (jx >= 0.0) ? 1.0 : (127.0 + jx);
		premix_right	= (jx >= 0.0) ? (1.0 - jx) : 1.0;
	}
	else
	{
		// Reverse
		premix_left		= (jx >= 0.0) ? (1.0 - jx) : 1.0;
		premix_right	= (jx >= 0.0) ? 1.0 : (1.0 + jx);
	}

	// Scale Drive output due to Joystick Y input (throttle)
	premix_left		*= jy;
	premix_right	*= jy;

	// Now calculate pivot amount
// - Strength of pivot (nPivSpeed) based on Joystick X input
// - Blending of pivot vs drive (blend) based on Joystick Y input
	omega = jx;
	blend = (NABS(jy) > blend_threshold) ? 0.0 : (1.0 - NABS(jy) / blend_threshold);

	// Calculate final mix of Drive and Pivot
	nMotMixL = (1.0 - fPivScale)*nMotPremixL + fPivScale * (nPivSpeed);
	nMotMixR = (1.0 - fPivScale)*nMotPremixR + fPivScale * (-nPivSpeed);
}
*/







APP_TEST_TOOLDATA::APP_TEST_TOOLDATA()
{
}

APP_TEST_TOOLDATA::APP_TEST_TOOLDATA(const NLPATH_WORKBENCH* pwb)
{
	m_flags = 0;

	m_RobotTestPosition.x = 0.0f;
	m_RobotTestPosition.y = 0.0f;
	m_RobotTestPosition.z = 0.0f;

	m_pWorkbench	= (NLPATH_WORKBENCH*)pwb;
	
	if(m_pWorkbench && m_pWorkbench->getPath())
		m_PersistentTrackingData.forwardSetup(&m_pWorkbench->getPath()->m_geometry,0.1f);

	m_kin.null();
}

APP_TEST_TOOLDATA::~APP_TEST_TOOLDATA()
{
}

void APP_TEST_TOOLDATA::drawCurrentKin(NL2DOCS * p2docs)
{
	NVEC3			v3;
	NVEC2			xtd = { 0.003f,0.003f };
	NCOLOR			color = { NCOLOR_PRESET3F_ORANGE_AMBER,1 };
	v3.x = p2docs->transformX(m_kin.m_s);
	v3.y = p2docs->transformY(m_kin.m_v);
	v3.z = 0.0f;
	NUT_Draw_Cross(&v3, &xtd, &color);
}


// ------------------------------------------------------------------------------------------
// App_TestTool_RobotPositionVsPath
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
NEDITOR_RESULT App_TestTool_RobotPositionVsPath(const NEVENT *pevent,NEDITOR_SELECTION *pselect, const Nu32 tool_param )
{
	APP_TEST_TOOLDATA		*pdata;
	NVEC3f32				localhitpoint;

	NVEC3					plane_point, plane_dir;
	NVEC3					mouseline_point, mouseline_dir;


	#ifdef _DEBUG
	Nu32 bdel = 0;
	#endif

	switch(pevent->Type)
	{
		case NTOUCH_START:
			
			pdata = (APP_TEST_TOOLDATA *)tool_param;
			NUT_Get3DMouseRay(&mouseline_point, &mouseline_dir, NULL, NGetEventTouchPositionPtr(pevent, 0));
			NGetEditorWorkPlaneNormalVector(&plane_dir);
			NGetEditorWorkPlanePoint(&plane_point);
			NUT_LineXPLane(&mouseline_point, &mouseline_dir, &plane_point, &plane_dir, &pdata->m_RobotTestPosition);
			FLAG_ON(pdata->m_flags, FLAG_APP_TEST_TOOLDATA_TESTPOSITION_ON);

			//pdata->m_PersistentTrackingData.forwardSetup(pdata->m_pPath);
			//pdata->m_PersistentTrackingData.forwardUpdate((NVEC2f32*)&pdata->m_RobotTestPosition);
			pdata->m_PersistentTrackingData.forwardSetup(&pdata->m_pWorkbench->getPath()->m_geometry,0.1f);
			pdata->m_PersistentTrackingData.trackForward((NVEC2f32*)&pdata->m_RobotTestPosition);

			return NEDITOR_RESULT_TOOL_ON;
			break;

		case NTOUCH_MOVE:
			pdata = (APP_TEST_TOOLDATA *)tool_param;
			if (ISFLAG_ON(pdata->m_flags, FLAG_APP_TEST_TOOLDATA_TESTPOSITION_ON))
			{
				NUT_Get3DMouseRay(&mouseline_point, &mouseline_dir, NULL, NGetEventTouchPositionPtr(pevent, 0));
				NGetEditorWorkPlaneNormalVector(&plane_dir);
				NGetEditorWorkPlanePoint(&plane_point);
				NUT_LineXPLane(&mouseline_point, &mouseline_dir, &plane_point, &plane_dir, &pdata->m_RobotTestPosition);
				//pdata->m_PersistentTrackingData.forwardUpdate((NVEC2f32*)&pdata->m_RobotTestPosition);
				pdata->m_PersistentTrackingData.trackForward((NVEC2f32*)&pdata->m_RobotTestPosition);
				pdata->m_pWorkbench->getTrajectory()->getKinAtS(&pdata->m_kin, pdata->m_PersistentTrackingData.m_pointOnPath.s);
				return NEDITOR_RESULT_TOOL_ON;
			}
			break;

		case NTOUCH_END:
			pdata = (APP_TEST_TOOLDATA *)tool_param;
			FLAG_OFF(pdata->m_flags, FLAG_APP_TEST_TOOLDATA_TESTPOSITION_ON);
			return NEDITOR_RESULT_TOOL_ON;
			
			break;

		default:
			break;
	}
	return NEDITOR_RESULT_TOOL_OFF;
}


void App_TestTool_RobotPositionVsPath_TimeHandle(const NTIME* ptime, const NEDITOR_SELECTION *pselection, const Nu32 tool_param)
{
	APP_TEST_TOOLDATA			*pdata = (APP_TEST_TOOLDATA*)tool_param;
	NUT_SetDrawMode(NUT_DRAW_RENDERING_MODE_3D);
	NUT_SetDrawConstructionPlane(_PLANE_XY);

	if (ISFLAG_ON(pdata->m_flags, FLAG_APP_TEST_TOOLDATA_TESTPOSITION_ON))
	{
		NCOLOR						color = { NCOLOR_PRESET3F_BRONZE_GOLD,1.0f };
		NVEC2						xtd = { 0.1f,0.1f };
		NUT_Draw_Cross(&pdata->m_RobotTestPosition, &xtd, &color);
		pdata->m_PersistentTrackingData.draw(&xtd, &color);
	}

}


// -----------------------------------------------------------------------
#endif	//_NEDITOR

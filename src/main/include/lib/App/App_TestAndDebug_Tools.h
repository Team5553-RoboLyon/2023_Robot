#pragma once



// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							App_TestAndDebug_Tools.h								**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../N/Event/NEvent.h"
#include "../N/Core/NTime.h"
#include "../N/UI/NUI.h"
#include "../N/NEditor.h"
#include "../N/NUsualCoordinates.h"
#include "../N/Utilities/Draw/NUT_Draw.h"

#include "../NL/MotionControl/Path/NLPathPersistentTrackingData.h"
#include "../NL//MotionControl/NLKin.h"

#include "../NL/MotionControl/NLPathWorkbench.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
#define FLAG_APP_TEST_TOOLDATA_TESTPOSITION_ON		BIT_0

typedef struct APP_TEST_TOOLDATA	APP_TEST_TOOLDATA;
struct  APP_TEST_TOOLDATA
{
	APP_TEST_TOOLDATA();
	APP_TEST_TOOLDATA(const NLPATH_WORKBENCH *pwb);
	~APP_TEST_TOOLDATA();

	Nu32								 m_flags;
	NVEC3f32							 m_RobotTestPosition;
	NLPATH_WORKBENCH					*m_pWorkbench;

	NLPATH_PERSISTENT_TRACKING_DATA		m_PersistentTrackingData;
	NLKIN								m_kin;
	void								drawCurrentKin(NL2DOCS * p2docs);

};

// ToolData
// -------------------------------------
//APP_TEST_TOOLDATA*	App_SetupTestToolData(APP_TEST_TOOLDATA* pt);
//APP_TEST_TOOLDATA*	App_CreateTestToolData();
//void					App_ClearWayTestToolData(APP_TEST_TOOLDATA *pt);
//void					App_DeleteTestToolData(APP_TEST_TOOLDATA *pt);

//TOOLS
// -----------------------------------------------------------------------------------------------------------------------------
NEDITOR_RESULT			App_TestTool_RobotPositionVsPath(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param);
void					App_TestTool_RobotPositionVsPath_TimeHandle(const NTIME* ptime, const NEDITOR_SELECTION *pselection, const Nu32 tool_param);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus

  
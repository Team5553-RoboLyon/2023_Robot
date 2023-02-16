#pragma once



// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							App_UserVZones_Tools.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "../../N/Event/NEvent.h"
#include "../../N/Core/NTime.h"
#include "../../N/UI/NUI.h"
#include "../../N/NEditor.h"
#include "../../N/NUsualCoordinates.h"
#include "../../N/Utilities/Draw/NUT_Draw.h"

#include "../../NL/MotionControl/Path/NLPathPersistentTrackingData.h"
#include "../../NL//MotionControl/NLKin.h"

#include "../../NL/MotionControl/NLPathWorkbench.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
#define APP_USERVZONE_DETECTION_DISTANCE							0.3f
//#define APP_USERVZONE_DETECTION_DISTANCE_SQUARED					(APP_USERVZONE_DETECTION_DISTANCE*APP_USERVZONE_DETECTION_DISTANCE)

#define DEFAULT_APP_USERVZONE_SNAP_S_THRESHOLD					0.05f

#define FLAG_APP_USERVZONE_TOOLDATA_INSERT_STARTED			BIT_0
#define FLAG_APP_USERVZONE_TOOLDATA_INSERT_ENDED				BIT_1
#define FLAG_APP_USERVZONE_TOOLDATA_VALID_USERKEY				BIT_2
#define FLAG_APP_USERVZONE_TOOLDATA_EDIT_INTERVAL_END			BIT_3
#define MASK_APP_USERVZONE_TOOLDATA_ALL_INSERT_FLAGS			(BIT_0|BIT_1)



typedef struct APP_USERVZONE_TOOLDATA	APP_USERVZONE_TOOLDATA;
struct  APP_USERVZONE_TOOLDATA
{
	APP_USERVZONE_TOOLDATA();
	APP_USERVZONE_TOOLDATA(NLPATH_WORKBENCH *pwb);
	~APP_USERVZONE_TOOLDATA();

	NLPATH_POINT* getClosestPointOnCurrentPath(const NVEC2s16* p, const Nf32 max_path_square_dist);

	Nu32								m_flags;
	//NVEC3f32							m_hitPoint;		// Ou l'utilisateur clique !

	NLPATH_WORKBENCH					*m_pWorkbench;
	NLPATH_PERSISTENT_TRACKING_DATA		m_PersistentTrackingData;
	Nf32								m_SnapThreshold;

	NLPATH_POINT						m_Start;	// Point trouvé sur le chemin au touch_start
	NLPATH_POINT						m_End;		// Point trouvé sur le chemin au touch_end

};


//TOOLS
// -----------------------------------------------------------------------------------------------------------------------------
NEDITOR_RESULT			App_UserVZoneTool_Insert(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param);
NEDITOR_RESULT			App_UserVZoneTool_Edit(const NEVENT* pevent, NEDITOR_SELECTION* pselect, const Nu32 tool_param);
NEDITOR_RESULT			App_UserVZoneTool_Delete(const NEVENT* pevent, NEDITOR_SELECTION* pselect, const Nu32 tool_param);
void					App_UserVZoneTool_TimeHandle(const NTIME* ptime, const NEDITOR_SELECTION *pselection, const Nu32 tool_param);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif	// __cpluplus

  
#pragma once

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							App_WayPoints_Tools.h									**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/Event/NEvent.h"
#include "lib/N/Core/NTime.h"
#include "lib/N/UI/NUI.h"
#include "lib/N/NEditor.h"
#include "lib/N/NUsualCoordinates.h"
#include "lib/N/Utilities/Draw/NUT_Draw.h"
// #include "../NL/MotionControl/NLPath.h"
// #include "../NL/MotionControl/Path/Builders/WayPoints/NLPathBuilderWayPoints.h"
#include "lib/NL/MotionControl/NLPathWorkbench.h"

#ifdef __cplusplus
extern "C"
{
#endif
// --------------------------------------------------------------------------------------------------------------------------------------------------------------
#define APP_HELPERS_LINE_HALF_LENGTH 100.0f
#define APP_WAYPOINT_SNAP_DISTANCE 0.075f
#define APP_WAYPOINT_DETECTION_DISTANCE 0.075f // Used to test if cursor is close to a point.
#define APP_WAYPOINT_DETECTION_DISTANCE_SQUARED (APP_WAYPOINT_DETECTION_DISTANCE * APP_WAYPOINT_DETECTION_DISTANCE)

/*
#define APP_CLOTHOID_DRAW_ACCURACY								32
#define APP_ARC_DRAW_ACCURACY									64
*/

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// La fonction draw() de la classe NLPath ne dessine pas de poign�es pour manipuler les arcs de cercle ins�r�s.
#define DEFAULT_APP_ICIRCLE_HANDLE_XEXTEND 0.05f
#define DEFAULT_APP_ICIRCLE_HANDLE_YEXTEND 0.05f

	/*
	#define DEFAULT_APP_KEYPOINT_XEXTEND		 0.35f
	#define DEFAULT_APP_KEYPOINT_YEXTEND		 0.35f
	#define DEFAULT_APP_WAYPOINT_XEXTEND		 0.75f
	#define DEFAULT_APP_WAYPOINT_YEXTEND		 0.75f
	#define DEFAULT_APP_WAYPOINT_TGT_SIZE		 20.0f
	*/

#define DEFAULT_APP_WAYPOINT_SELECTION_COLORA NCOLOR_PRESET3F_WHITE, 1
#define DEFAULT_APP_WAYPOINT_SELECTION_COLORB NCOLOR_PRESET3F_BRONZE_DEEP, 1
#define DEFAULT_APP_HELPERS_COLOR NCOLOR_PRESET3F_BLUE_CERULEAN, 1
/*
#define DEFAULT_APP_WAYPOINT_COLOR				NCOLOR_PRESET3F_GREEN_JADE
#define DEFAULT_APP_HANDLE_COLOR				NCOLOR_PRESET3F_GREEN_HARLEQUIN

#define DEFAULT_APP_SEGMENT_COLOR				NCOLOR_PRESET3F_YELLOW_LEMON
#define DEFAULT_APP_CLOTHOID_COLORA				NCOLOR_PRESET3F_BRONZE
#define DEFAULT_APP_CLOTHOID_COLORB				NCOLOR_PRESET3F_BRONZE_GOLD
#define DEFAULT_APP_ARC_COLOR					NCOLOR_PRESET3F_GREEN_SPRING
#define DEFAULT_APP_KEYPOINT_COLOR				NCOLOR_PRESET3F_GREEN_EMERALD
#define DEFAULT_APP_KEYPOINTID_COLOR			NCOLOR_PRESET3F_ORANGE_GOLD
#define DEFAULT_APP_WAYPOINT_P_COLOR			NCOLOR_PRESET3F_BLUE_ELECTRIC
#define DEFAULT_APP_WAYPOINT_U_COLOR			NCOLOR_PRESET3F_BLUE_AQUAMARINE
*/
//																|												|	pSelectOwner		|		pSelected		|		LOWWORD			|		HIWORD						|
//	-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define APP_WAYPOINT_POSITION 1 //	|	WayPoint Position							|		Path Ptr		|		WayPoint Ptr	|APP_WAYPOINT_POSITION	| Index in Path.m_wayPointsArray	|
#define APP_WAYPOINT_TANGENT 2	//	|	Way Point Tangent							|		Path Ptr		|		WayPoint Ptr	|APP_WAYPOINT_TANGENT	| Index in Path.m_wayPointsArray	|
#define APP_WAYPOINT_ICRA 3		//	|	Inserted Circle Ratio (Section Part A)		|		Path Ptr		|		Section Ptr		|APP_WAYPOINT_ICRA		| Index in Path.m_waySectionsArray	|
#define APP_WAYPOINT_ICRC 4		//	|	Inserted Circle Ratio (Section Part C)		|		Path Ptr		|		Section Ptr		|APP_WAYPOINT_ICRC		| Index in Path.m_waySectionsArray	|
#define APP_WAYPOINT_BCR 5		//	|	Build Circle Ratio							|		Path Ptr		|		WayPoint Ptr	|APP_WAYPOINT_BCR		| Index in Path.m_wayPointsArray	|

#define FLAG_APP_WAYPOINT_TOOLDATA_SNAP_ON BIT_0
#define FLAG_APP_WAYPOINT_TOOLDATA_DRAW_HELPER_SNAPLINE0 BIT_30
#define FLAG_APP_WAYPOINT_TOOLDATA_DRAW_HELPER_SNAPLINE1 BIT_31

	typedef struct
	{
		NVEC2f32 m_p;
		NVEC2f32 m_u;
	} APP_HELPER_SNAPLINE;

	typedef struct
	{
		Nu32 m_flags;
		// NLPATH_BUILDER_WAYPOINTS	*m_pPathBuilder;
		NLPATH_WORKBENCH *m_pWorkbench;

		APP_HELPER_SNAPLINE m_SnapLine[2]; // 2 structures utilis�es pour tracer deux lignes ( "helper") de snap.

		NVEC2f32 m_ICircleHandleExtend;
		/*
		// Sizes
		Nf32					m_TgtSize;
		NVEC2f32				m_KeyPointExtend;
		NVEC2f32				m_WayPointExtend;
		*/
		// Colors
		NCOLOR m_HelpersColor;
		NCOLOR m_SelectionColorA;
		NCOLOR m_SelectionColorB;
		/*
		NCOLOR					m_HandleColor;
		NCOLOR					m_KeyPointColor;
		NCOLOR					m_KeyPointIDColor;
		NCOLOR					m_SegmentColor;
		NCOLOR					m_ArcColor;
		NCOLOR					m_ClothoidColorA;
		NCOLOR					m_ClothoidColorB;
		NCOLOR					m_WayPointPColor;
		NCOLOR					m_WayPointUColor;
		*/
	} APP_WAYPOINT_TOOLDATA;

	// ToolData
	// -------------------------------------
	APP_WAYPOINT_TOOLDATA *App_SetupWayPointsToolData(APP_WAYPOINT_TOOLDATA *pwpack, NLPATH_WORKBENCH *pworkbench);
	APP_WAYPOINT_TOOLDATA *App_CreateWayPointsToolData(NLPATH_WORKBENCH *pworkbench);
	void App_ClearWayPointsToolData(APP_WAYPOINT_TOOLDATA *pwpack);
	void App_DeleteWayPointsToolData(APP_WAYPOINT_TOOLDATA *pwpack);

	// TOOLS
	//  -----------------------------------------------------------------------------------------------------------------------------
	NEDITOR_RESULT App_WayPointsTool_Insert(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param);
	NEDITOR_RESULT App_WayPointsTool_Erase(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param);
	NEDITOR_RESULT App_WayPointsTool_Move(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param);

	void App_WayPointsTool_TimeHandle(const NTIME *ptime, const NEDITOR_SELECTION *pselection, const Nu32 tool_param);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus

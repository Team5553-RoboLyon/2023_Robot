#pragma once

// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **							App_Polygonal_Vertex_Tools.h							**
// **																					**
// ***************************************************************************************
// ***************************************************************************************
#include "lib/N/Event/NEvent.h"
#include "lib/N/Core/NTime.h"
#include "lib/N/UI/NUI.h"
#include "lib/N/NEditor.h"
#include "lib/N/NUsualCoordinates.h"
#include "lib/N/Utilities/Draw/NUT_Draw.h"
#include "lib/NL/MotionControl/Path/Builders/PolygonalChain/NLPathBuilderPolygonalChain.h"
#include "lib/NL/MotionControl/NLPathWorkbench.h"

// #include "../NL/MotionControl/NLPath.h"

#ifdef __cplusplus
extern "C"
{
#endif

	// --------------------------------------------------------------------------------------------------------------------------------------------------------------
	// #define APP_PLG_VERTEX_DETECTION_DISTANCE							0.075f	//Used to test if cursor is close to a point.
	// #define APP_PLG_VERTEX_DETECTION_DISTANCE_SQUARED					(APP_PLG_VERTEX_DETECTION_DISTANCE*APP_PLG_VERTEX_DETECTION_DISTANCE)

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// La fonction draw() de la classe NLPath ne dessine pas de poign�es pour manipuler les arcs de cercle ins�r�s.

#define DEFAULT_APP_PLG_CHAIN_SELECTION_COLORA NCOLOR_PRESET3F_WHITE, 1
#define DEFAULT_APP_PLG_CHAIN_SELECTION_COLORB NCOLOR_PRESET3F_BRONZE_DEEP, 1

//																|												|	pSelectOwner		|		pSelected		|		LOWWORD				|		HIWORD					|
//	-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define APP_PLG_VERTEX_POSITION 1 //	|	Vertex Position								|	PathBuilder Ptr		|		Vertex Ptr		|APP_PLG_VERTEX_POSITION	| Index in m_vertexArray		|
#define APP_PLG_VERTEX_HANDLE_A 2 //	|	Vertex Handle A: BuildCircle Radius			|	PathBuilder Ptr		|		Vertex Ptr		|APP_PLG_VERTEX_HANDLE_A	| Index in m_vertexArray		|
#define APP_PLG_VERTEX_HANDLE_B 3 //	|	Vertex Handle B: Inserted Circle/Clothoids	|	PathBuilder Ptr		|		Vertex Ptr		|APP_PLG_VERTEX_HANDLE_B	| Index in m_vertexArray		|

	/*
	#define FLAG_APP_WAYPOINT_TOOLDATA_SNAP_ON					BIT_0
	#define FLAG_APP_WAYPOINT_TOOLDATA_DRAW_HELPER_SNAPLINE0	BIT_30
	#define FLAG_APP_WAYPOINT_TOOLDATA_DRAW_HELPER_SNAPLINE1	BIT_31
	*/

	typedef struct
	{
		Nu32 m_flags;
		// NLPATH_BUILDER_POLYGONAL_CHAIN	*m_pPathBuilder;
		NLPATH_WORKBENCH *m_pWorkbench;
		// Colors
		NCOLOR m_SelectionColorA;
		NCOLOR m_SelectionColorB;
	} APP_PLG_CHAIN_TOOLDATA;

	// ToolData
	// -------------------------------------
	APP_PLG_CHAIN_TOOLDATA *App_SetupPolygonalChainToolData(APP_PLG_CHAIN_TOOLDATA *pvpack, NLPATH_WORKBENCH *pworkbench);
	APP_PLG_CHAIN_TOOLDATA *App_CreatePolygonalChainToolData(NLPATH_WORKBENCH *pworkbench);
	void App_ClearPolygonalChainToolData(APP_PLG_CHAIN_TOOLDATA *pvpack);
	void App_DeletePolygonalChainToolData(APP_PLG_CHAIN_TOOLDATA *pvpack);

	// TOOLS
	//  -----------------------------------------------------------------------------------------------------------------------------
	NEDITOR_RESULT App_PolygonalChainTool_Create(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param);
	NEDITOR_RESULT App_PolygonalChainTool_Erase(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param);
	NEDITOR_RESULT App_PolygonalChainTool_Edit(const NEVENT *pevent, NEDITOR_SELECTION *pselect, const Nu32 tool_param);

	void App_PolygonalChainTool_TimeHandle(const NTIME *ptime, const NEDITOR_SELECTION *pselection, const Nu32 tool_param);
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cpluplus

#pragma once

#include "lib/NL/MotionControl/NLPathWorkbench.h"

#define EDIT_TXT_FRMT_STR "%.4f"
// Specific Windows Messages:													WPARAM							LPARAM
//																		( = message sender ID )			( = message parameter )
//
#define WM_MESSAGE_SET_CURRENT_PATH_WORKBENCH (WM_USER + 1)		   //		APP_EXPLORER_WINDOW				pCurrentWorkbench					Send to all APP WINDOW(s) when user select a new Path Workbench to Edit.
#define WM_MESSAGE_DELETE_CURRENT_PATH_WORKBENCH (WM_USER + 2)	   //		APP_EXPLORER_WINDOW				pCurrentWorkbench					Send to all APP WINDOW(s) when user delete the current edited Path Workbench.
#define WM_MESSAGE_UPDATE_CURRENT_PATH_WORKBENCH_PTR (WM_USER + 3) //		APP_EXPLORER_WINDOW				pCurrentWorkbench					Send to all APP WINDOW(s) when current edited Path Workbench Ptr is updated ( les datas sont les m�mes, juste le pointeur est different ).

#define WM_MESSAGE_APP_WINDOW_CLOSED (WM_USER + 4)		 //		APP_WINDOW_ENUM					App Window HWND						Send to Application "Explorer" Window when an App window/Dialog is going to be closed.
#define WM_MESSAGE_SPECS_KINLIMITS_UPDATED (WM_USER + 5) //		APP_WINDOW_ENUM					pcurrent_dtspecs					Send to Application "Explorer" Window when Robot Specs ( Kin Limits ) is updated ( by Robot KinLimits BDD ).
														 //		APP_EXPLORER_WINDOW				pcurrent_dtspecs					... And then to all the App BDD by Explorer
#define WM_MESSAGE_SPECS_PHYSICS_UPDATED (WM_USER + 6)	 //		APP_WINDOW_ENUM					pcurrent_dtspecs					Send to Application "Explorer" Window when Robot Specs ( Physics ) is updated ( by Robot Physics BDD ).
														 //		APP_EXPLORER_WINDOW				pcurrent_dtspecs					... And then to all the App BDD by Explorer

#define WM_MESSAGE_SET_CURRENT_SELECTED_TRJKEY (WM_USER + 7)		 //	APP_EXPLORER_WINDOW				keyIndex							Send to all APP WINDOW(s) when current SelectedKey is updated ( = NVOID si pas de key selectionn�e ).
#define WM_MESSAGE_UPDATE_CURRENT_SELECTED_TRJKEY_DATA (WM_USER + 8) //	APP_EXPLORER_WINDOW				keyIndex							Send to all APP WINDOW(s) when current SelectedKey data are updated

#define WM_MESSAGE_SET_CURRENT_SPECS (WM_USER + 50) //		APP_EXPLORER_WINDOW				&DriveTrainSpecifications			Send to all App WINDOW(s) just after creation.
// #define WM_MESSAGE_DELETE_CURRENT_ROBOT_SPECS			(WM_USER+51) //		APP_EXPLORER_WINDOW				pDriveTrainSpecifications			Send to all App WINDOW(s) just after creation.		! Pas encore utilis� car une seule DTSpec pour l'instant !
// #define WM_MESSAGE_UPDATE_CURRENT_ROBOT_SPECS			(WM_USER+52) //		APP_EXPLORER_WINDOW				&DriveTrainSpecifications			Send to all App WINDOW(s) just after creation.		! Pas encore utilis� car une seule DTSpec pour l'instant !

#define WM_MESSAGE_REFRESH_PROJECT_TREEVIEW (WM_USER + 100) //		0								0									Send to Window Explorer by MAIN WINDOW. Il faut rafraichir le Treeview

#define WM_MESSAGE_KEY_INSERTED_AND_SELECTED (WM_USER + 200) //		NLPATH_WORKBENCH*				index in Trajectory->m_keysArray	Send to Window Explorer by a Tool. Une nouvelle clef a �t� ins�r�e et selectionn�e. L'index pass� est celui de la clef ins�r�e.
#define WM_MESSAGE_KEY_SELECTED (WM_USER + 201)				 //		NLPATH_WORKBENCH*				index in Trajectory->m_keysArray	Send to Window Explorer by a Tool. Une nouvelle clef a �t� s�lectionn�e (cliqu�e). L'index pass� est celui de la clef.
#define WM_MESSAGE_KEY_DELETED (WM_USER + 202)				 //		NLPATH_WORKBENCH*				index in Trajectory->m_keysArray	Send to Window Explorer by a Tool. Une clef a �t� supprim�e.  L'index pass� est celui de la clef supprim�e.
// #define WM_MESSAGE_KEY_MOVESTART						(WM_USER+203)//		NLPATH_WORKBENCH*				index in Trajectory->m_keysArray	Send to Window Explorer by a Tool. Une clef vient d'�tre cliqu�e avec l'outil MOVE/EDIT. Elle n'a pas encore �t� d�plac�e. L'index pass� est celui de cette clef.
#define WM_MESSAGE_KEY_MOVED (WM_USER + 203) //		NLPATH_WORKBENCH*				index in Trajectory->m_keysArray	Send to Window Explorer by a Tool. Une clef a �t� d�plac�e.  L'index pass� est celui de la clef d�plac�e.

// #define WM_MESSAGE_ACTION_ID							(WM_USER+203)//		NLPATH_WORKBENCH*				index in Trajectory->m_keysArray	Send to Window Explorer by a Tool. Une clef a �t� d�plac�e.  L'index pass� est celui de la clef d�plac�e.

#define WM_MESSAGE_SET_CURRENT_ACTION_WINDOW (WM_USER + 1000) //																		Message local utilis� uniquement pour communiquer entre les fenetres "key Properties" et "Action"
#define WM_MESSAGE_GET_CURRENT_ACTION_WINDOW (WM_USER + 1001) //																		Message local utilis� uniquement pour communiquer entre les fenetres "key Properties" et "Action"

#define WM_MESSAGE_UPDATE_ACTION_WINDOW_CONTENT (WM_USER + 1002)	 //	TRUE or FALSE					WND_KEYACTION_CREATE_DATA*			Message local utilis� uniquement pour communiquer entre les fenetres "key Properties" et "Action"
#define WM_MESSAGE_DISPLAY_UPDATED_ACTION_TIMESHIFT (WM_USER + 1003) //	0								0									Message local utilis� uniquement pour communiquer entre les fenetres "key Properties" et "Action"

#define WM_MESSAGE_TRAJECTORY_REBUILDED (WM_USER + 1100) //	NLPATH_WORKBENCH*																	Message local utilis� uniquement pour communiquer entre les fenetres "key Properties" et "Action"
#define WM_MESSAGE_PATH_REBUILDED (WM_USER + 1101)		 //	NLPATH_WORKBENCH*																	Message local utilis� uniquement pour communiquer entre les fenetres "key Properties" et "Action"

typedef enum
{
	// APP_WIN_GLOBALSETTINGS = 0,
	// APP_WIN_EXPLORER,

	WB_WIN_TOOLS = 0,
	WB_WIN_KINLIMITS,
	WB_WIN_KTWEAKS,
	WB_WIN_WP_BUILD_STRATEGY,		   // Way Point building strategy window
	WB_WIN_TRAJECTORY_USERVZONE_TOOLS, // User VZone Tools window  ( Insert, edit, delete )
	WB_WIN_TRAJECTORY_KEY_TOOLS,	   // (User) KEY Tools window  ( Insert, edit, delete )
	WB_WIN_TRAJECTORY_KEY_PROPERTIES,  // (User) KEY Properties window  ( Actions descriptions )

	RB_WIN_SIMULATION,
	RB_WIN_KINLIMITS,
	RB_WIN_PHYSICS,
	RB_WIN_GEOMETRY,
	RB_WIN_CHARACTERIZATION,
	// -----------------------------
	APP_WIN_ENUM_SIZE
} APP_WINDOW_ENUM;

#define APP_EXPLORER_WINDOW (APP_WIN_ENUM_SIZE + 1)
#define APP_TOOL (APP_WIN_ENUM_SIZE + 2)

/*
typedef enum
{
	RB_WIN_SIMULATION = 0,
	RB_WIN_KINLIMITS,
	RB_WIN_PHYSICS,
	RB_WIN_GEOMETRY,
	RB_WIN_CHARACTERIZATION,
	// -----------------------------
	RB_WIN_ENUM_SIZE
}ROBOT_WINDOW_ENUM;

typedef enum
{
	WB_WIN_TOOLS	= 0,
	WB_WIN_KINLIMITS,
	WB_WIN_KTWEAKS,
	WB_WIN_WP_BUILD_STRATEGY,
// -----------------------------
	WB_WIN_ENUM_SIZE
}WORKBENCH_WINDOW_ENUM;
*/
// -----------------------------------------------------
//
// Path builder Tools interface
// To work with 'PathBuilderToolIDTable[][]'
#define PATHBUILDER_TOOL_CREATE 0
#define PATHBUILDER_TOOL_EDIT 1
#define PATHBUILDER_TOOL_DELETE 2
#define PATHBUILDER_TOOL_SIZE 3 // PathBuilderToolIDTable[NLPATH_WORKBENCH_ID_ENUM_SIZE][PATHBUILDER_TOOL_SIZE]

//
// -----------------------------------------------------

// Tools ID ( to Register NTool )
// PATH TOOLS
#define NTOOL_FIRST_PATH_BUILDER_ID 0		  // |
#define NTOOL_LAST_PATH_BUILDER_ID 5		  // |
											  // |
#define NTOOL_ID_PATH_TOOL_NEW_WAY_POINT 0	  // |
#define NTOOL_ID_PATH_TOOL_ERASE_WAY_POINT 1  // |
#define NTOOL_ID_PATH_TOOL_EDIT_WAY_POINT 2	  // |-- From 0 to 5 : Path Builder Tools ( cf NTOOL_FIRST_PATH_BUILDER_ID et NTOOL_LAST_PATH_BUILDER_ID )
											  // |
#define NTOOL_ID_PATH_TOOL_NEW_PLG_VERTEX 3	  // |
#define NTOOL_ID_PATH_TOOL_ERASE_PLG_VERTEX 4 // |
#define NTOOL_ID_PATH_TOOL_EDIT_PLG_VERTEX 5  // |

#define NTOOL_ID_2DOCS_FREE_TRANSFORM 6

// USER VZONES TOOLS
#define NTOOL_FIRST_USERVZONE_ID 7	// |
#define NTOOL_LAST_USERVZONE_ID 9	// |
									// |-- From 7 to 9 : UserVZone Tools
#define NTOOL_ID_INSERT_USERVZONE 7 // |
#define NTOOL_ID_EDIT_USERVZONE 8	// |
#define NTOOL_ID_DELETE_USERVZONE 9 // |

// TRAJECTORY KEY TOOLS
#define NTOOL_FIRST_TRJKEY_ID 10  // |
#define NTOOL_LAST_TRJKEY_ID 12	  // |
								  // |-- From 10 to 12 : Trajectory Key Tools
#define NTOOL_ID_INSERT_TRJKEY 10 // |
#define NTOOL_ID_EDIT_TRJKEY 11	  // |
#define NTOOL_ID_DELETE_TRJKEY 12 // |

// Utilisation des NL2DOCS_COORDS_STYLEs
// Le setup des NL2DOCS_COORDS_STYLEs se fait dans App_Init.cpp
// NL2DOCS_COORDS_STYLE_0.......................... Defini et utilis� pour les valeurs en abscisses sur les NL2DOCS
// NL2DOCS_COORDS_STYLE_1.......................... Defini et utilis� pour les valeurs en ordinn�es sur les NL2DOCS
// NL2DOCS_COORDS_STYLE_2.......................... Defini et utilis� pour les INDEX en abscisses sur les NL2DOCS

/*
void _clear_path(void* ptr);
void _clear_path_builder(void* ptr);
void _clear_trajectory(void* ptr);
*/

void App_UpToDateActionMessagesLUT_Init(const Nchar *pfilename);
void App_UpToDateActionMessagesLUT_End();

void App_Render_Init();
void App_Render_End();

void App_Tools_Init();
void App_Tools_End();

void App_Simulation_Init();
void App_Simulation_End();

void App_Camera_Init();
void App_ClearSurface_Init();

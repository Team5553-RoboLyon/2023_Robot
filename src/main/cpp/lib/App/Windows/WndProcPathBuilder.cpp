#include "../../../resource.h"
#include "../App_Init.h"
#include "../App_WayPoints_Tools.h"
#include "../App_PolygonalChain_Tools.h"
#include "../../NL/MotionControl/NLPathWorkbench.h"
#include "../Windows/WindowUtilities.h"
#include "WndProcPathBuilder.h"


extern HWND							AppWindowsTable[APP_WINDOW_ENUM::APP_WIN_ENUM_SIZE];
extern Nu8							PathBuilderToolIDTable[NLPATH_WORKBENCH::ID::NLPATH_WORKBENCH_ID_ENUM_SIZE][PATHBUILDER_TOOL_SIZE];
extern APP_WAYPOINT_TOOLDATA		WayPointsToolData;
extern APP_PLG_CHAIN_TOOLDATA		PolygonalChainToolData;

static void _setToolDataPathWorkbenchPtr(NLPATH_WORKBENCH* pwb)
{
	if (pwb)
	{
		switch (pwb->getId())
		{
		case NLPATH_WORKBENCH::ID::WB_WAYPOINTS:
			WayPointsToolData.m_pWorkbench		= pwb;
			PolygonalChainToolData.m_pWorkbench = NULL;
			break;
		case NLPATH_WORKBENCH::ID::WB_PLG_CHAIN:
			WayPointsToolData.m_pWorkbench		= NULL;
			PolygonalChainToolData.m_pWorkbench = pwb;
			break;
		default:
			NErrorIf(1, NERROR_UNAUTHORIZED_CASE);
			WayPointsToolData.m_pWorkbench		= NULL;
			PolygonalChainToolData.m_pWorkbench = NULL;
			break;
		}
	}
	else
	{
		WayPointsToolData.m_pWorkbench = NULL;
		PolygonalChainToolData.m_pWorkbench = NULL;
	}
}


BOOL CALLBACK WndProc_PathBuilder(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static Nu32 static_Tool = NVOID;
	NLPATH_WORKBENCH	*pwb		= (NLPATH_WORKBENCH*)GetWindowLong(hwndDlg, GWL_USERDATA);
	NLPATH_WORKBENCH	*pwb_new	= nullptr;



	switch (message)
	{
	case WM_INITDIALOG:
		/*
		if (pwb)
		{
			EnableWindow(GetDlgItem(hwndDlg, IDC_CREATE), TRUE);
			EnableWindow(GetDlgItem(hwndDlg, IDC_DELETE), TRUE);
			EnableWindow(GetDlgItem(hwndDlg, IDC_EDIT), TRUE);
			EnableWindow(GetDlgItem(hwndDlg, IDC_CHECK_PATH_BUILDER_SNAP), TRUE);
		}
		else
		{
			EnableWindow(GetDlgItem(hwndDlg, IDC_CREATE), FALSE);
			EnableWindow(GetDlgItem(hwndDlg, IDC_DELETE), FALSE);
			EnableWindow(GetDlgItem(hwndDlg, IDC_EDIT), FALSE);
			EnableWindow(GetDlgItem(hwndDlg, IDC_CHECK_PATH_BUILDER_SNAP), FALSE);
		}
		*/
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			SendMessage(GetParent(hwndDlg), WM_MESSAGE_APP_WINDOW_CLOSED, (WPARAM)APP_WINDOW_ENUM::WB_WIN_TOOLS, (LPARAM)hwndDlg);
			static_Tool = NVOID;
			_setToolDataPathWorkbenchPtr(NULL);
			// SetWindowLong(hwndDlg, GWL_USERDATA, (LONG)NULL); ... on ne le fait pas car on destroy la BDD juste après
			DestroyWindow(hwndDlg);
			return TRUE;

		case IDC_CREATE:
			NSetCurrentTool(PathBuilderToolIDTable[pwb->getId()][PATHBUILDER_TOOL_CREATE] );
			static_Tool = PATHBUILDER_TOOL_CREATE;
			return TRUE;

		case IDC_DELETE:
			NSetCurrentTool(PathBuilderToolIDTable[pwb->getId()][PATHBUILDER_TOOL_DELETE]);
			static_Tool = PATHBUILDER_TOOL_DELETE;
			return TRUE;


		case IDC_EDIT:
			NSetCurrentTool(PathBuilderToolIDTable[pwb->getId()][PATHBUILDER_TOOL_EDIT]);
			static_Tool = PATHBUILDER_TOOL_EDIT;
			return TRUE;

		case IDC_CHECK_PATH_BUILDER_SNAP:
			switch (pwb->getId())
			{
			case NLPATH_WORKBENCH::ID::WB_WAYPOINTS:
				if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_PATH_WAYPOINT_SNAP) == BST_CHECKED)
				{
					FLAG_ON(WayPointsToolData.m_flags, FLAG_APP_WAYPOINT_TOOLDATA_SNAP_ON);
				}
				else
				{
					FLAG_OFF(WayPointsToolData.m_flags, FLAG_APP_WAYPOINT_TOOLDATA_SNAP_ON);
				}
				break;

			case NLPATH_WORKBENCH::ID::WB_PLG_CHAIN:
				break;
			default:
				break;
			}
			return TRUE;
		}
		break; // case WM_COMMAND:

	// Messages relatifs au PATH WORKBENCH:
	// |
	// -------------------------------------------------------------------------------------------------------------
	case WM_MESSAGE_SET_CURRENT_PATH_WORKBENCH:
		NErrorIf(wParam != APP_EXPLORER_WINDOW, NERROR_INCONSISTENT_PARAMETERS); // pas une erreur juste une verif...
		// WPARAM : APP_EXPLORER_WINDOW ( Id de l'explorer )
		// LPARAM : NEW  REGISTRED WORKBENCH
		pwb_new = (NLPATH_WORKBENCH*)lParam;
		NErrorIf(pwb == pwb_new, NERROR_INCONSISTENT_PARAMETERS); // Ils devraient être différents !

		// un PathWorkbench est dejà enregistré en tant que USERDATA de cette BDD. ( rappel: il s'agit de pwb )
		if (pwb)
		{
			// Maintient du tool courant?
			// 1) Le Ntool courant est-il 1 des Ntools géré par la dialog box... ?
			Nu32 current_tool = NGetCurrentTool();
			if ((current_tool >= NTOOL_FIRST_PATH_BUILDER_ID) && (current_tool <= NTOOL_LAST_PATH_BUILDER_ID))
			{
				NSetCurrentTool(PathBuilderToolIDTable[pwb_new->getId()][static_Tool]);
			}
		}
		_setToolDataPathWorkbenchPtr(pwb_new);
		SetWindowLong(hwndDlg, GWL_USERDATA, (LONG)pwb_new);
		EnumChildWindows(hwndDlg, EnableCtrl_Callback, 0);
		return TRUE;

	case WM_MESSAGE_DELETE_CURRENT_PATH_WORKBENCH:
		NErrorIf(wParam != APP_EXPLORER_WINDOW, NERROR_INCONSISTENT_PARAMETERS); // pas une erreur juste une verif...
		// WPARAM : APP_EXPLORER_WINDOW ( Id de l'explorer )
		// LPARAM : TO BE DELETE  REGISTRED WORKBENCH
		pwb_new = (NLPATH_WORKBENCH*)lParam; // ici pwb_new représente en fait le WORKBENCH qui va être supprimé !

		if (pwb)
		{
			if (pwb == pwb_new)
			{
				// Arrêt du tool courant?
				// 1) Le Ntool courant est-il 1 des Ntools géré par la dialog box... ?
				Nu32 current_tool = NGetCurrentTool();
				if ((current_tool >= NTOOL_FIRST_PATH_BUILDER_ID) && (current_tool <= NTOOL_LAST_PATH_BUILDER_ID))
				{
					NSetCurrentTool(NVOID);
				}
			}
		}
		_setToolDataPathWorkbenchPtr(NULL);
		SetWindowLong(hwndDlg, GWL_USERDATA, 0);
		EnumChildWindows(hwndDlg, DisableCtrl_Callback, 0);
		return TRUE;

	case WM_MESSAGE_UPDATE_CURRENT_PATH_WORKBENCH_PTR:
		NErrorIf(wParam != APP_EXPLORER_WINDOW, NERROR_INCONSISTENT_PARAMETERS); // pas une erreur juste une verif...
		// WPARAM : APP_EXPLORER_WINDOW ( Id de l'explorer )
		pwb_new = (NLPATH_WORKBENCH*)lParam;
		_setToolDataPathWorkbenchPtr(pwb_new);
		SetWindowLong(hwndDlg, GWL_USERDATA, (LONG)pwb_new);
		return TRUE;

	default:
		break;
	}
	return FALSE;
}

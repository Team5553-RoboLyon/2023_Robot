#include "../../../../resource.h"
#include "../../App_Init.h"
#include "../../App_WayPoints_Tools.h"
#include "../../App_PolygonalChain_Tools.h"
#include "../../../NL/MotionControl/NLPathWorkbench.h"
#include "../../Windows/WindowUtilities.h"
#include "../../Tools/App_TrajectoryKey_Tools.h"
#include "WndProcTrajectoryKeys.h"


extern HWND							AppWindowsTable[APP_WINDOW_ENUM::APP_WIN_ENUM_SIZE];
extern APP_TRJKEY_TOOLDATA			TrajectoryKeyToolData;;



BOOL CALLBACK WndProc_TrajectoryKeys(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
			SendMessage(GetParent(hwndDlg), WM_MESSAGE_APP_WINDOW_CLOSED, (WPARAM)APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_KEY_TOOLS, (LPARAM)hwndDlg);
			static_Tool = NVOID;
			//SetWindowLong(hwndDlg, GWL_USERDATA, (LONG)NULL); ... on ne le fait pas car on destroy la BDD juste après
			DestroyWindow(hwndDlg);
			return TRUE;

		case IDC_INSERT:
			NSetCurrentTool(NTOOL_ID_INSERT_TRJKEY);
			static_Tool = NTOOL_ID_INSERT_TRJKEY;
			return TRUE;

		case IDC_DELETE:
			NSetCurrentTool(NTOOL_ID_DELETE_TRJKEY);
			static_Tool = NTOOL_ID_DELETE_TRJKEY;
			return TRUE;


		case IDC_EDIT:
			NSetCurrentTool(NTOOL_ID_EDIT_TRJKEY);
			static_Tool = NTOOL_ID_EDIT_TRJKEY;
			return TRUE;

		case IDC_CHECK_KEY_IS_STOP:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_KEY_IS_STOP) == BST_CHECKED)
			{
				FLAG_ON(TrajectoryKeyToolData.m_keyCreationFlags,FLAG_NLTRJKEY_STOP);
			}
			else
			{
				FLAG_OFF(TrajectoryKeyToolData.m_keyCreationFlags, FLAG_NLTRJKEY_STOP);
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
		SetWindowLong(hwndDlg, GWL_USERDATA, (LONG)pwb_new);
		TrajectoryKeyToolData.m_pWorkbench = pwb_new;
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
				if ((current_tool >= NTOOL_FIRST_TRJKEY_ID) && (current_tool <= NTOOL_LAST_TRJKEY_ID))
				{
					NSetCurrentTool(NVOID);
				}
			}
		}
		SetWindowLong(hwndDlg, GWL_USERDATA, 0);
		TrajectoryKeyToolData.m_pWorkbench = NULL;
		EnumChildWindows(hwndDlg, DisableCtrl_Callback, 0);
		return TRUE;

	case WM_MESSAGE_UPDATE_CURRENT_PATH_WORKBENCH_PTR:
		NErrorIf(wParam != APP_EXPLORER_WINDOW, NERROR_INCONSISTENT_PARAMETERS); // pas une erreur juste une verif...
		// WPARAM : APP_EXPLORER_WINDOW ( Id de l'explorer )
		pwb_new = (NLPATH_WORKBENCH*)lParam;

		SetWindowLong(hwndDlg, GWL_USERDATA, (LONG)pwb_new);
		TrajectoryKeyToolData.m_pWorkbench = pwb_new;
		return TRUE;

	default:
		break;
	}
	return FALSE;
}

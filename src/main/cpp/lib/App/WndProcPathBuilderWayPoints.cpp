#include "../../resource.h"
#include "../N/NEditor.h"
#include "../NL/MotionControl/NLKin.h"
#include "../NL/MotionControl/Path/Builders/WayPoints/NLPathHeuristics.h"
#include "../NL/MotionControl/NLKinTweak.h"
#include "../NL/MotionControl/Path/NLPath.h"

#include "App_WayPoints_Tools.h"
#include "App_InfoStrings.h"
#include "App_Init.h"

#include "WndProcPathBuilderWayPoints.h"


extern APP_WAYPOINT_TOOLDATA	WayPointsToolData;
extern NLKINTWEAK				Ktweaks;


HWND hwnd_PathBuilder_WayPoints = NULL;

BOOL CALLBACK WndProc_PathBuilder_WayPoints(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static		Nf32	k0, k2, k4, l1, l2, l3;


	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			DestroyWindow(hwndDlg);
			hwnd_PathBuilder_WayPoints = NULL;
			return TRUE;
		/*
		case IDC_NEW_WAYPOINT:
			NSetCurrentTool(NTOOL_ID_PATH_TOOL_NEW_WAY_POINT);
			return TRUE;

		case IDC_DELETE_WAYPOINT:
			NSetCurrentTool(NTOOL_ID_PATH_TOOL_ERASE_WAY_POINT);
			return TRUE;


		case IDC_MOVE_WAYPOINT:
			NSetCurrentTool(NTOOL_ID_PATH_TOOL_EDIT_WAY_POINT);
			return TRUE;

		case IDC_CHECK_PATH_WAYPOINT_SNAP:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_PATH_WAYPOINT_SNAP) == BST_CHECKED)
			{
				FLAG_ON(WayPointsToolData.m_flags, FLAG_APP_WAYPOINT_TOOLDATA_SNAP_ON);
			}
			else
			{
				FLAG_OFF(WayPointsToolData.m_flags, FLAG_APP_WAYPOINT_TOOLDATA_SNAP_ON);
			}
			return TRUE;

		case IDC_CHECK_DRAW_PATH:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_PATH) == BST_CHECKED)
			{
				FLAG_ON(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_PATH);
			}
			else
			{
				FLAG_OFF(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_PATH);
			}
			return TRUE;

		case IDC_CHECK_DRAW_PATH_HANDLES:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_PATH_HANDLES) == BST_CHECKED)
			{
				FLAG_ON(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_HANDLES);
			}
			else
			{
				FLAG_OFF(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_HANDLES);
			}
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_CIRCLES:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_CIRCLES) == BST_CHECKED)
			{
				FLAG_ON(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_BUILDING_CIRCLES);
			}
			else
			{
				FLAG_OFF(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_BUILDING_CIRCLES);
			}
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_PAIR_ALPHA_ANGLES:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_PAIR_ALPHA_ANGLES) == BST_CHECKED)
			{
				FLAG_ON(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_BUILDING_PAIR_ALPHA_ANGLES);
			}
			else
			{
				FLAG_OFF(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_BUILDING_PAIR_ALPHA_ANGLES);
			}
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_PAIR_BETA_ANGLES:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_PAIR_BETA_ANGLES) == BST_CHECKED)
			{
				FLAG_ON(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_BUILDING_PAIR_BETA_ANGLES);
			}
			else
			{
				FLAG_OFF(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_BUILDING_PAIR_BETA_ANGLES);
			}
			return TRUE;


		case IDC_CHECK_DRAW_BUILDING_BASES:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_BASES) == BST_CHECKED)
			{
				FLAG_ON(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_BUILDING_BASES);
			}
			else
			{
				FLAG_OFF(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_BUILDING_BASES);
			}
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_PAIR_LINES:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_PAIR_LINES) == BST_CHECKED)
			{
				FLAG_ON(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_BUILDING_PAIR_LINES);
				
			}
			else
			{
				FLAG_OFF(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_BUILDING_PAIR_LINES);
			}
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_PAIR_0:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_PAIR_0) == BST_CHECKED)
			{
				FLAG_ON(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_BUILDING_PAIR_0);
				// infos ON:
				SetWindowText(GetDlgItem(hwndDlg, IDC_PATH_BUILDING_DETAILS_INFOS), WNDPROCPATHBUILD_DRAW_BUILDING_PAIR );
			}
			else
			{
				FLAG_OFF(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_BUILDING_PAIR_0);
				// infos OFF
				SetWindowText(GetDlgItem(hwndDlg, IDC_PATH_BUILDING_DETAILS_INFOS), DEFAULT_INFOS);
			}
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_PAIR_1:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_PAIR_1) == BST_CHECKED)
			{
				FLAG_ON(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_BUILDING_PAIR_1);
				// infos ON:
				SetWindowText(GetDlgItem(hwndDlg, IDC_PATH_BUILDING_DETAILS_INFOS), WNDPROCPATHBUILD_DRAW_BUILDING_PAIR);
			}
			else
			{
				FLAG_OFF(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_BUILDING_PAIR_1);
				// infos OFF
				SetWindowText(GetDlgItem(hwndDlg, IDC_PATH_BUILDING_DETAILS_INFOS), DEFAULT_INFOS);
			}
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_PAIR_2:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_PAIR_2) == BST_CHECKED)
			{
				FLAG_ON(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_BUILDING_PAIR_2);
				// infos ON:
				SetWindowText(GetDlgItem(hwndDlg, IDC_PATH_BUILDING_DETAILS_INFOS), WNDPROCPATHBUILD_DRAW_BUILDING_PAIR);
			}
			else
			{
				FLAG_OFF(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_BUILDING_PAIR_2);
				// infos OFF
				SetWindowText(GetDlgItem(hwndDlg, IDC_PATH_BUILDING_DETAILS_INFOS), DEFAULT_INFOS);
			}
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_PAIR_3:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_PAIR_3) == BST_CHECKED)
			{
				FLAG_ON(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_BUILDING_PAIR_3);
				// infos ON:
				SetWindowText(GetDlgItem(hwndDlg, IDC_PATH_BUILDING_DETAILS_INFOS), WNDPROCPATHBUILD_DRAW_BUILDING_PAIR);
			}
			else
			{
				FLAG_OFF(Path.m_flags, FLAG_NLPATH_EDITING_DRAW_BUILDING_PAIR_3);
				// infos OFF
				SetWindowText(GetDlgItem(hwndDlg, IDC_PATH_BUILDING_DETAILS_INFOS), DEFAULT_INFOS);
			}
			return TRUE;

		case IDC_RADIO_SHORTEST:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_SHORTEST) == BST_CHECKED)
			{
				WayPointsToolData.m_pPath->setHeuristic(NLPATH_heuristic_shortest);
				RequestCurrentPathBuild;
			}
			return TRUE;
		case IDC_RADIO_LESSCURVY:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_LESSCURVY) == BST_CHECKED)
			{
				WayPointsToolData.m_pPath->setHeuristic(NLPATH_heuristic_lessCurvy);
				RequestCurrentPathBuild;
			}
			return TRUE;
		case IDC_RADIO_QUICKEST:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_QUICKEST) == BST_CHECKED)
			{
				WayPointsToolData.m_pPath->setHeuristic(NLPATH_heuristic_quickest);
				RequestCurrentPathBuild;
			}
			return TRUE;
			*/
		}
	}
	return FALSE;
}

#include "../../../resource.h"
#include "../../NL/MotionControl/NLKin.h"
#include "../../NL/MotionControl/Path/NLPath.h"

#include "../App_WayPoints_Tools.h"
#include "../App_InfoStrings.h"
#include "../App_Init.h"
#include "../../NL/MotionControl/NLPathWorkbench.h"
#include "../../NL/MotionControl/Path/Builders/WayPoints/NLPathBuilderWayPoints.h"
#include "WndProcPathBuilderWayPointsDisplay.h"


extern APP_WAYPOINT_TOOLDATA	WayPointsToolData;
extern NLKINTWEAK				Ktweaks;

/*
HWND hwnd_PathBuilder_WayPoints_Display = NULL;
BOOL CALLBACK WndProc_PathBuilder_WayPointsDisplay(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	NLPATH_WORKBENCH			*pworkbench	= (NLPATH_WORKBENCH*)GetWindowLong(hwndDlg, GWL_USERDATA);
	NLPATH_BUILDER_WAYPOINTS	*pbuilder	= (NLPATH_BUILDER_WAYPOINTS*)pworkbench->getPathBuilder();

	
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
			hwnd_PathBuilder_WayPoints_Display = NULL;
			return TRUE;

		case IDC_CHECK_DRAW_PATH:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_PATH) == BST_CHECKED)
			{
				pworkbench->showPath();
			}
			else
			{
				pworkbench->hidePath();
			}
			return TRUE;

		case IDC_CHECK_DRAW_PATH_HANDLES:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_PATH_HANDLES) == BST_CHECKED)
			{
				FLAG_ON(pbuilder->m_flags, FLAG_NLPBW_DRAW_HANDLES);
			}
			else
			{
				FLAG_OFF(pbuilder->m_flags, FLAG_NLPBW_DRAW_HANDLES);
			}
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_CIRCLES:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_CIRCLES) == BST_CHECKED)
			{
				FLAG_ON(pbuilder->m_flags, FLAG_NLPBW_DRAW_BUILD_CIRCLES);
			}
			else
			{
				FLAG_OFF(pbuilder->m_flags, FLAG_NLPBW_DRAW_BUILD_CIRCLES);
			}
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_PAIR_ALPHA_ANGLES:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_PAIR_ALPHA_ANGLES) == BST_CHECKED)
			{
				FLAG_ON(pbuilder->m_flags, FLAG_NLPBW_DRAW_BUILD_PAIR_ALPHA_ANGLES);
			}
			else
			{
				FLAG_OFF(pbuilder->m_flags, FLAG_NLPBW_DRAW_BUILD_PAIR_ALPHA_ANGLES);
			}
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_PAIR_BETA_ANGLES:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_PAIR_BETA_ANGLES) == BST_CHECKED)
			{
				FLAG_ON(pbuilder->m_flags, FLAG_NLPBW_DRAW_BUILD_PAIR_BETA_ANGLES);
			}
			else
			{
				FLAG_OFF(pbuilder->m_flags, FLAG_NLPBW_DRAW_BUILD_PAIR_BETA_ANGLES);
			}
			return TRUE;


		case IDC_CHECK_DRAW_BUILDING_BASES:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_BASES) == BST_CHECKED)
			{
				FLAG_ON(pbuilder->m_flags, FLAG_NLPBW_DRAW_BUILD_BASES);
			}
			else
			{
				FLAG_OFF(pbuilder->m_flags, FLAG_NLPBW_DRAW_BUILD_BASES);
			}
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_PAIR_LINES:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_PAIR_LINES) == BST_CHECKED)
			{
				FLAG_ON(pbuilder->m_flags, FLAG_NLPBW_DRAW_BUILD_PAIR_LINES);
				
			}
			else
			{
				FLAG_OFF(pbuilder->m_flags, FLAG_NLPBW_DRAW_BUILD_PAIR_LINES);
			}
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_PAIR_0:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_PAIR_0) == BST_CHECKED)
			{
				FLAG_ON(pbuilder->m_flags, FLAG_NLPBW_DRAW_BUILD_PAIR_0);
				// infos ON:
				SetWindowText(GetDlgItem(hwndDlg, IDC_PATH_BUILDING_DETAILS_INFOS), WNDPROCPATHBUILD_DRAW_BUILDING_PAIR );
			}
			else
			{
				FLAG_OFF(pbuilder->m_flags, FLAG_NLPBW_DRAW_BUILD_PAIR_0);
				// infos OFF
				SetWindowText(GetDlgItem(hwndDlg, IDC_PATH_BUILDING_DETAILS_INFOS), DEFAULT_INFOS);
			}
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_PAIR_1:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_PAIR_1) == BST_CHECKED)
			{
				FLAG_ON(pbuilder->m_flags, FLAG_NLPBW_DRAW_BUILD_PAIR_1);
				// infos ON:
				SetWindowText(GetDlgItem(hwndDlg, IDC_PATH_BUILDING_DETAILS_INFOS), WNDPROCPATHBUILD_DRAW_BUILDING_PAIR);
			}
			else
			{
				FLAG_OFF(pbuilder->m_flags, FLAG_NLPBW_DRAW_BUILD_PAIR_1);
				// infos OFF
				SetWindowText(GetDlgItem(hwndDlg, IDC_PATH_BUILDING_DETAILS_INFOS), DEFAULT_INFOS);
			}
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_PAIR_2:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_PAIR_2) == BST_CHECKED)
			{
				FLAG_ON(pbuilder->m_flags, FLAG_NLPBW_DRAW_BUILD_PAIR_2);
				// infos ON:
				SetWindowText(GetDlgItem(hwndDlg, IDC_PATH_BUILDING_DETAILS_INFOS), WNDPROCPATHBUILD_DRAW_BUILDING_PAIR);
			}
			else
			{
				FLAG_OFF(pbuilder->m_flags, FLAG_NLPBW_DRAW_BUILD_PAIR_2);
				// infos OFF
				SetWindowText(GetDlgItem(hwndDlg, IDC_PATH_BUILDING_DETAILS_INFOS), DEFAULT_INFOS);
			}
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_PAIR_3:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_PAIR_3) == BST_CHECKED)
			{
				FLAG_ON(pbuilder->m_flags, FLAG_NLPBW_DRAW_BUILD_PAIR_3);
				// infos ON:
				SetWindowText(GetDlgItem(hwndDlg, IDC_PATH_BUILDING_DETAILS_INFOS), WNDPROCPATHBUILD_DRAW_BUILDING_PAIR);
			}
			else
			{
				FLAG_OFF(pbuilder->m_flags, FLAG_NLPBW_DRAW_BUILD_PAIR_3);
				// infos OFF
				SetWindowText(GetDlgItem(hwndDlg, IDC_PATH_BUILDING_DETAILS_INFOS), DEFAULT_INFOS);
			}
			return TRUE;

		default:
			break;
		}
	}
	return FALSE;
}
*/

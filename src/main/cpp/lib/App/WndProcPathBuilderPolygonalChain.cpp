#include "../../resource.h"
#include "../N/NEditor.h"
#include "../NL/MotionControl/NLKin.h"

#include "../NL/MotionControl/NLKinTweak.h"

#include "App_WayPoints_Tools.h"
#include "App_InfoStrings.h"
#include "App_Init.h"

#include "WndProcPathBuilderPolygonalChain.h"


//extern NLMOTIONCONTROLER		Path;
extern NLPATH					Path;



HWND hwnd_PathBuilder_PolygonalChain = NULL;

BOOL CALLBACK WndProc_PathBuilder_PolygonalChain(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static		Nf32	k0, k2, k4, l1, l2, l3;

	// SCROLLINFO	sinf;


	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	/*
	case WM_HSCROLL:
		if (LOWORD(wParam) == SB_THUMBTRACK)
		{
			switch (GetDlgCtrlID((HWND)lParam))
			{
			case IDC_TWEAK_ACCEL:
				Ktweaks.m_aHoldLocalLimitRatio = (Nf32)HIWORD(wParam) / 100.0f;
				sprintf(txtout, "Acc ratio: %.2f", Ktweaks.m_aHoldLocalLimitRatio);
				SetWindowText(GetDlgItem(hwndDlg, IDC_OUT_TWEAK_ACC), txtout);

				break;

			case IDC_TWEAK_JERK:
				Ktweaks.m_jHoldLocalLimitRatio = (Nf32)HIWORD(wParam) / 100.0f;
				sprintf(txtout, "J ratio: %.2f", Ktweaks.m_jHoldLocalLimitRatio);
				SetWindowText(GetDlgItem(hwndDlg, IDC_OUT_TWEAK_JERK), txtout);
				break;
			}
		}
		return TRUE;
	*/
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			DestroyWindow(hwndDlg);
			hwnd_PathBuilder_PolygonalChain = NULL;
			return TRUE;

		case IDC_NEW_PLG_VERTEX:
			NSetCurrentTool(NTOOL_ID_PATH_TOOL_NEW_PLG_VERTEX);
			return TRUE;

		case IDC_DELETE_PLG_VERTEX:
			NSetCurrentTool(NTOOL_ID_PATH_TOOL_ERASE_PLG_VERTEX);
			return TRUE;


		case IDC_EDIT_PLG_VERTEX:
			NSetCurrentTool(NTOOL_ID_PATH_TOOL_EDIT_PLG_VERTEX);
			return TRUE;

/*
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

		case IDC_RADIO_SHORTEST:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_SHORTEST) == BST_CHECKED)
				WayPointsToolData.m_pPath->setHeuristic(NLPATH_heuristic_shortest);
			return TRUE;
		case IDC_RADIO_LESSCURVY:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_LESSCURVY) == BST_CHECKED)
				WayPointsToolData.m_pPath->setHeuristic(NLPATH_heuristic_lessCurvy);
			return TRUE;
		case IDC_RADIO_QUICKEST:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_QUICKEST) == BST_CHECKED)
				WayPointsToolData.m_pPath->setHeuristic(NLPATH_heuristic_quickest);
			return TRUE;
*/
		}
	}
	return FALSE;
}

#include <iostream>
#include <windows.h>
#include <CommCtrl.h>

#include "../../../../resource.h"
#include "../../../NL/MotionControl/NLKinTweak.h"
#include "../../App_Init.h"
#include "../../../NL/MotionControl/Path/Builders/WayPoints/NLPathBuilderWayPoints.h"
#include "../WindowUtilities.h"
#include "WndProcPathBuilderWayPointsStrategy.h"

#define TRACKBAR_SIZE					250				// The whole trackbar Range (100% de la trackbar = 250  , cat la trackbar compte en nbre entier !!! )
#define TRACKBAR_ARC_THRESHOLD_MIN		0.005f			// metres ( = 5 mm )
#define TRACKBAR_ARC_THRESHOLD_MAX		0.5f			// metres ( = 50 cm )
#define TRACKBAR_ARC_THRESHOLD_RANGE	( TRACKBAR_ARC_THRESHOLD_MAX - TRACKBAR_ARC_THRESHOLD_MIN )



#define TXTOUT_SIZE		128
static void init(HWND hwnd, NLPATH_WORKBENCH* pcurrent_workbench)
{
	Nchar		txtout[TXTOUT_SIZE];
	Nu32		d;
	Nf32		f;

	CheckDlgButton(hwnd, IDC_CHECK_USE_ROBOT_KINLIMITS, BST_UNCHECKED);

	// ARC LENGTH THRESHOLD
	f = ((NLPATH_BUILDER_WAYPOINTS*)(pcurrent_workbench->m_pPathBuilder))->m_arcLengthThreshold;
	sprintf(txtout, "%.4f", f);
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_ARC_THRESHOLD), txtout);
	d = TRACKBAR_SIZE - (Nu32)((Nf32)TRACKBAR_SIZE*(f - TRACKBAR_ARC_THRESHOLD_MIN) / TRACKBAR_ARC_THRESHOLD_RANGE);
	SendMessage(GetDlgItem(hwnd, IDC_ARC_THRESHOLD), TBM_SETPOS, TRUE, d);

	// Heuristic:
	if (((NLPATH_BUILDER_WAYPOINTS*)(pcurrent_workbench->m_pPathBuilder))->m_heuristic == NLPATH_heuristic_lessCurvy)
	{
		CheckDlgButton(hwnd, IDC_RADIO_LESSCURVY, BST_CHECKED);
		CheckDlgButton(hwnd, IDC_RADIO_SHORTEST, BST_UNCHECKED);
	}
	else
	{
		NErrorIf(((NLPATH_BUILDER_WAYPOINTS*)(pcurrent_workbench->m_pPathBuilder))->m_heuristic != NLPATH_heuristic_shortest, NERROR_INCONSISTENT_PARAMETERS);
		CheckDlgButton(hwnd, IDC_RADIO_SHORTEST, BST_CHECKED);
		CheckDlgButton(hwnd, IDC_RADIO_LESSCURVY, BST_UNCHECKED);
	}

	EnumChildWindows(hwnd, EnableCtrl_Callback, 0);
}


BOOL CALLBACK WndProc_PathBuilder_WayPointsStrategy(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	Nchar				txtout[TXTOUT_SIZE];
	Nf32				f;
	Nu32				d;
	NLPATH_WORKBENCH* pwb;
	NLPATH_WORKBENCH* pcurrent_workbench = (NLPATH_WORKBENCH*)GetWindowLong(hwndDlg, GWL_USERDATA);

	switch (message)
	{
	case WM_INITDIALOG:
		SendMessage(GetDlgItem(hwndDlg, IDC_ARC_THRESHOLD), TBM_SETRANGE, TRUE, NMAKELONG(0, TRACKBAR_SIZE));
		//RedrawWindow(hwndDlg, NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_INVALIDATE);
		//InvalidateRect(hwndDlg, NULL, FALSE);
		//UpdateWindow(hwndDlg);
		return TRUE;

		// TRACKBAR VALUES: LIMITS			
	case WM_VSCROLL:
		if (LOWORD(wParam) == SB_THUMBTRACK)
		{
			switch (GetDlgCtrlID((HWND)lParam))
			{
			case IDC_ARC_THRESHOLD:
				f = TRACKBAR_ARC_THRESHOLD_MIN + (Nf32)(TRACKBAR_SIZE-HIWORD(wParam)) * (Nf32)(TRACKBAR_ARC_THRESHOLD_RANGE) / (Nf32)(TRACKBAR_SIZE);
				((NLPATH_BUILDER_WAYPOINTS*)pcurrent_workbench->m_pPathBuilder)->m_arcLengthThreshold = f;
				sprintf(txtout, "%.4f", f);
				SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT_ARC_THRESHOLD), txtout);
				//InvalidateRect(GetDlgItem(hwndDlg, IDC_EDIT_ARC_THRESHOLD), NULL, FALSE);
				UpdateWindow(GetDlgItem(hwndDlg, IDC_EDIT_ARC_THRESHOLD));
				//RedrawWindow(GetDlgItem(hwndDlg, IDC_EDIT_ARC_THRESHOLD), NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_INVALIDATE);

				pcurrent_workbench->requestPathBuild();
				break;
			}
		}
		
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			SendMessage(GetParent(hwndDlg), WM_MESSAGE_APP_WINDOW_CLOSED, (WPARAM)APP_WINDOW_ENUM::WB_WIN_WP_BUILD_STRATEGY, (LPARAM)hwndDlg );
			DestroyWindow(hwndDlg);
			return TRUE;
	
		// EDIT 
		case IDC_EDIT_ARC_THRESHOLD:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				f = GetNf32FromEditControl((HWND)lParam, TRACKBAR_ARC_THRESHOLD_MIN, TRACKBAR_ARC_THRESHOLD_MAX);
				((NLPATH_BUILDER_WAYPOINTS*)pcurrent_workbench->m_pPathBuilder)->m_arcLengthThreshold = f;
				d = TRACKBAR_SIZE - (Nu32)((Nf32)TRACKBAR_SIZE * (f - TRACKBAR_ARC_THRESHOLD_MIN) / TRACKBAR_ARC_THRESHOLD_RANGE);
				SendMessage(GetDlgItem(hwndDlg, IDC_ARC_THRESHOLD), TBM_SETPOS, TRUE, d);
				pcurrent_workbench->requestPathBuild();
				break;

			default:
				break;
			}
			return TRUE;


		// RADIO BUTTONS:
		case IDC_RADIO_LESSCURVY:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_LESSCURVY) == BST_CHECKED)
			{
				((NLPATH_BUILDER_WAYPOINTS*)pcurrent_workbench->m_pPathBuilder)->setHeuristic(NLPATH_heuristic_lessCurvy);
				pcurrent_workbench->requestPathBuild();
			}
			return TRUE;

		case IDC_RADIO_SHORTEST:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_SHORTEST) == BST_CHECKED)
			{
				((NLPATH_BUILDER_WAYPOINTS*)pcurrent_workbench->m_pPathBuilder)->setHeuristic(NLPATH_heuristic_shortest);
				pcurrent_workbench->requestPathBuild();
			}
			return TRUE;

		default:
			break;
		}
		return TRUE;

	case WM_MESSAGE_SET_CURRENT_PATH_WORKBENCH:
		NErrorIf(wParam != APP_EXPLORER_WINDOW, NERROR_INCONSISTENT_PARAMETERS); // pas une erreur juste une verif...
		// WPARAM : APP_EXPLORER_WINDOW ( Id de l'explorer )
		// LPARAM : NEW  REGISTRED WORKBENCH
		pwb = (NLPATH_WORKBENCH*)lParam;
		NErrorIf(pwb == pcurrent_workbench, NERROR_INCONSISTENT_PARAMETERS); // Ils devraient être différents !
		if (pwb->getId() == NLPATH_WORKBENCH::ID::WB_WAYPOINTS)
		{
			SetWindowLong(hwndDlg, GWL_USERDATA, (LONG)pwb);
			init(hwndDlg, pwb);
		}
		else
		{
			SetWindowLong(hwndDlg, GWL_USERDATA, 0);
			EnumChildWindows(hwndDlg, DisableCtrl_Callback, 0);
		}
		return TRUE;

	case WM_MESSAGE_DELETE_CURRENT_PATH_WORKBENCH:
		NErrorIf(wParam != APP_EXPLORER_WINDOW, NERROR_INCONSISTENT_PARAMETERS); // pas une erreur juste une verif...
		if (pcurrent_workbench)
		{
			// WPARAM : APP_EXPLORER_WINDOW ( Id de l'explorer )
			// LPARAM : TO BE DELETE REGISTRED WORKBENCH
			pwb = (NLPATH_WORKBENCH*)lParam;
			NErrorIf(pwb != pcurrent_workbench, NERROR_INCONSISTENT_PARAMETERS); // Ils devraient être égaux !
			SetWindowLong(hwndDlg, GWL_USERDATA, 0);
			EnumChildWindows(hwndDlg, DisableCtrl_Callback, 0);
		}
		return TRUE;

	/*
	// On ne gere pas ce message car ici, Rien à faire que les specs du robot aient été updatées ... !
	case WM_MESSAGE_ROBOT_SPECS_KINLIMITS_UPDATED:
		return TRUE;
	*/

	default:
		break;
	}
	return FALSE;
}

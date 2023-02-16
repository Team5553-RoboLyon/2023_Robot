#include <iostream>
#include <windows.h>
#include <CommCtrl.h>

#include "../../../resource.h"
#include "../../NL/MotionControl/NLKinTweak.h"
#include "../App_Init.h"
#include "WindowUtilities.h"
#include "WndProcTrajectoryKinematicLimitTweaks.h"

#define TRACKBAR_SIZE				250		// The whole trackbar Range (100% de la trackbar = 250 )
#define TRACKBAR_TWEAK_JERK_MAX		1.0f
#define TRACKBAR_TWEAK_ACCEL_MAX	1.0f
#define TXTOUT_SIZE					32
static void init(HWND hwnd, NLPATH_WORKBENCH* pcurrent_workbench)
{
	Nchar				txtout[TXTOUT_SIZE];
	Nu32				d;

	NErrorIf(!pcurrent_workbench->m_pTrajectory, NERROR_NULL_POINTER);

	// JERK
	sprintf(txtout, "%.4f", pcurrent_workbench->m_pTrajectory->m_kinTweak.m_jHoldLocalLimitRatio);
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_JERK), txtout);
	d = TRACKBAR_SIZE - (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*pcurrent_workbench->m_pTrajectory->m_kinTweak.m_jHoldLocalLimitRatio / (Nf32)(TRACKBAR_TWEAK_JERK_MAX));
	SendMessage(GetDlgItem(hwnd, IDC_TWEAK_JERK), TBM_SETPOS, TRUE, d);
	// Accel
	sprintf(txtout, "%.4f", pcurrent_workbench->m_pTrajectory->m_kinTweak.m_aHoldLocalLimitRatio);
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_ACCEL), txtout);
	d = TRACKBAR_SIZE - (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*pcurrent_workbench->m_pTrajectory->m_kinTweak.m_aHoldLocalLimitRatio / (Nf32)(TRACKBAR_TWEAK_ACCEL_MAX));
	SendMessage(GetDlgItem(hwnd, IDC_TWEAK_ACCEL), TBM_SETPOS, TRUE, d);
	EnumChildWindows(hwnd, EnableCtrl_Callback, 0);
}

BOOL CALLBACK WndProc_TrajectoryKinematicLimitTweaks(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	Nf32				f;
	Nu32				d;
	Nchar				txtout[TXTOUT_SIZE];
	NLPATH_WORKBENCH	*pwb;
	NLPATH_WORKBENCH	*pcurrent_workbench = (NLPATH_WORKBENCH*)GetWindowLong(hwndDlg, GWL_USERDATA);

	switch (message)
	{
	case WM_INITDIALOG:
		SendMessage(GetDlgItem(hwndDlg, IDC_TWEAK_ACCEL), TBM_SETRANGE, TRUE, NMAKELONG(0, TRACKBAR_SIZE));
		SendMessage(GetDlgItem(hwndDlg, IDC_TWEAK_ACCEL), TBM_SETTIC, 0, TRACKBAR_SIZE/2);

		SendMessage(GetDlgItem(hwndDlg, IDC_TWEAK_JERK), TBM_SETRANGE, TRUE, NMAKELONG(0, TRACKBAR_SIZE));
		SendMessage(GetDlgItem(hwndDlg, IDC_TWEAK_JERK), TBM_SETTIC, 0, TRACKBAR_SIZE / 2);
		InvalidateRect(hwndDlg, NULL, FALSE);
		return TRUE;

	case WM_VSCROLL:
		if (LOWORD(wParam) == SB_THUMBTRACK)
		{
			switch (GetDlgCtrlID((HWND)lParam))
			{

			case IDC_TWEAK_ACCEL:
				f = (Nf32)(TRACKBAR_SIZE-HIWORD(wParam)) * (Nf32)(TRACKBAR_TWEAK_ACCEL_MAX) / (Nf32)(TRACKBAR_SIZE);
				pcurrent_workbench->m_pTrajectory->m_kinTweak.m_aHoldLocalLimitRatio = f;
				sprintf(txtout, "%.4f", f);
				SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT_ACCEL), txtout);
				pcurrent_workbench->requestTrajectoryBuild();
				break;

			case IDC_TWEAK_JERK:
				f = (Nf32)(TRACKBAR_SIZE - HIWORD(wParam)) * (Nf32)(TRACKBAR_TWEAK_JERK_MAX) / (Nf32)(TRACKBAR_SIZE);
				pcurrent_workbench->m_pTrajectory->m_kinTweak.m_jHoldLocalLimitRatio = f;
				sprintf(txtout, "%.4f", f);
				SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT_JERK), txtout);
				pcurrent_workbench->requestTrajectoryBuild();
				break;

			default:
				break;
			}
			InvalidateRect(hwndDlg, NULL, FALSE);
		}
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			SendMessage(GetParent(hwndDlg), WM_MESSAGE_APP_WINDOW_CLOSED, (WPARAM)APP_WINDOW_ENUM::WB_WIN_KTWEAKS, (LPARAM)hwndDlg);
			DestroyWindow(hwndDlg);
			return TRUE;
		//EDIT:
		case IDC_EDIT_JERK:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				pcurrent_workbench->m_pTrajectory->m_kinTweak.m_jHoldLocalLimitRatio = GetNf32FromEditControl((HWND)lParam, 0.0f, TRACKBAR_TWEAK_JERK_MAX);
				d = TRACKBAR_SIZE - (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*pcurrent_workbench->m_pTrajectory->m_kinTweak.m_jHoldLocalLimitRatio / (Nf32)(TRACKBAR_TWEAK_JERK_MAX));
				SendMessage(GetDlgItem(hwndDlg, IDC_TWEAK_JERK), TBM_SETPOS, TRUE, d);
				pcurrent_workbench->requestTrajectoryBuild();
				break;

			default:
				break;
			}
			return TRUE;

		case IDC_EDIT_ACCEL:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				pcurrent_workbench->m_pTrajectory->m_kinTweak.m_aHoldLocalLimitRatio = GetNf32FromEditControl((HWND)lParam, 0.0f, TRACKBAR_TWEAK_ACCEL_MAX);
				d = TRACKBAR_SIZE - (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*pcurrent_workbench->m_pTrajectory->m_kinTweak.m_aHoldLocalLimitRatio / (Nf32)(TRACKBAR_TWEAK_ACCEL_MAX));
				SendMessage(GetDlgItem(hwndDlg, IDC_TWEAK_ACCEL), TBM_SETPOS, TRUE, d);
				pcurrent_workbench->requestTrajectoryBuild();
				break;

			default:
				break;
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
		SetWindowLong(hwndDlg, GWL_USERDATA, (LONG)pwb);
		init(hwndDlg, pwb);
		return TRUE;

	case WM_MESSAGE_DELETE_CURRENT_PATH_WORKBENCH:
		NErrorIf(wParam != APP_EXPLORER_WINDOW, NERROR_INCONSISTENT_PARAMETERS); // pas une erreur juste une verif...
		// WPARAM : APP_EXPLORER_WINDOW ( Id de l'explorer )
		// LPARAM : TO BE DELETE  REGISTRED WORKBENCH
		pwb = (NLPATH_WORKBENCH*)lParam;
		NErrorIf(pwb != pcurrent_workbench, NERROR_INCONSISTENT_PARAMETERS); // Ils devraient être égaux !
		SetWindowLong(hwndDlg, GWL_USERDATA, 0);
		EnumChildWindows(hwndDlg, DisableCtrl_Callback, 0);
		return TRUE;

	case WM_MESSAGE_UPDATE_CURRENT_PATH_WORKBENCH_PTR:
		NErrorIf(wParam != APP_EXPLORER_WINDOW, NERROR_INCONSISTENT_PARAMETERS); // pas une erreur juste une verif...
		// WPARAM : APP_EXPLORER_WINDOW ( Id de l'explorer )
		pwb = (NLPATH_WORKBENCH*)lParam;
		SetWindowLong(hwndDlg, GWL_USERDATA, (LONG)pwb);
		return TRUE;

	default:
		break;
	}
	return FALSE;
}

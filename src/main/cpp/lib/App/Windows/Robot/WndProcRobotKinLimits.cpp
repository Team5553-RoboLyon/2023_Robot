#include <iostream>
#include <windows.h>
#include <CommCtrl.h>

#include "../../../../resource.h"
#include "../../../NL/MotionControl/NLKinTweak.h"
#include "../../App_Init.h"
#include "../WindowUtilities.h"
#include "WndProcRobotKinLimits.h"


void updateKinLimitsCtrls(HWND hwnd, const NLKINLIMITS* pk, const Nu32 flags, Nchar* pstr)
{
	if (ISFLAG_ON(flags, FLAG_JERK))
	{
		Nf32 f = pk->getJerkMax();
		Nu32 d = (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*f / (Nf32)(TRACKBAR_JERK_MAX));

		sprintf(pstr, EDIT_TXT_FRMT_STR, f);
		SetWindowText(GetDlgItem(hwnd, IDC_EDIT_JERK), pstr);
		SendMessage(GetDlgItem(hwnd, IDC_JERK), TBM_SETPOS, TRUE, d);
	}

	if (ISFLAG_ON(flags, FLAG_ACCELERATION))
	{
		Nf32 f = pk->getAccelerationMax();
		Nu32 d = (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*f / (Nf32)(TRACKBAR_ACCEL_MAX));

		sprintf(pstr, EDIT_TXT_FRMT_STR, f);
		SetWindowText(GetDlgItem(hwnd, IDC_EDIT_ACCEL), pstr);
		SendMessage(GetDlgItem(hwnd, IDC_ACCEL), TBM_SETPOS, TRUE, d);
	}

	if (ISFLAG_ON(flags, FLAG_VELOCITY))
	{
		Nf32 f = pk->getVelocityMax();
		Nu32 d = (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*f / (Nf32)(TRACKBAR_VELOCITY_MAX));

		sprintf(pstr, EDIT_TXT_FRMT_STR, f);
		SetWindowText(GetDlgItem(hwnd, IDC_EDIT_VELOCITY), pstr);
		SendMessage(GetDlgItem(hwnd, IDC_VELOCITY), TBM_SETPOS, TRUE, d);
	}
}


static void init(HWND hwnd, NLDRIVETRAINSPECS* pdtspecs)
{
	Nchar				txtout[TXTOUT_SIZE];
	Nu32				d;

	sprintf(txtout, "%.4f", pdtspecs->m_limits.getJerkMax());
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_JERK), txtout);
	d = (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*pdtspecs->m_limits.getJerkMax() / (Nf32)(TRACKBAR_JERK_MAX));
	SendMessage(GetDlgItem(hwnd, IDC_JERK), TBM_SETPOS, TRUE, d);

	sprintf(txtout, "%.4f", pdtspecs->m_limits.getAccelerationMax());
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_ACCEL), txtout);
	d = (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*pdtspecs->m_limits.getAccelerationMax() / (Nf32)(TRACKBAR_ACCEL_MAX));
	SendMessage(GetDlgItem(hwnd, IDC_ACCEL), TBM_SETPOS, TRUE, d);

	sprintf(txtout, "%.4f", pdtspecs->m_limits.getVelocityMax());
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_VELOCITY), txtout);
	d = (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*pdtspecs->m_limits.getVelocityMax() / (Nf32)(TRACKBAR_VELOCITY_MAX));
	SendMessage(GetDlgItem(hwnd, IDC_VELOCITY), TBM_SETPOS, TRUE, d);
}

BOOL CALLBACK WndProc_RobotKinLimits(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	Nchar				txtout[TXTOUT_SIZE];
	Nf32				f;
	Nu32				d;
	Nu32				r;
	NLDRIVETRAINSPECS	*pdtspecs;
	NLDRIVETRAINSPECS	*pcurrent_dtspecs = (NLDRIVETRAINSPECS*)GetWindowLong(hwndDlg, GWL_USERDATA);

	switch (message)
	{
	case WM_INITDIALOG:
		SendMessage(GetDlgItem(hwndDlg, IDC_JERK), TBM_SETRANGE, TRUE, NMAKELONG(0, TRACKBAR_SIZE));
		SendMessage(GetDlgItem(hwndDlg, IDC_VELOCITY), TBM_SETRANGE, TRUE, NMAKELONG(0, TRACKBAR_SIZE));
		SendMessage(GetDlgItem(hwndDlg, IDC_ACCEL), TBM_SETRANGE, TRUE, NMAKELONG(0, TRACKBAR_SIZE));
		return TRUE;

	// TRACKBAR VALUES: LIMITS			
	case WM_HSCROLL:
		if (LOWORD(wParam) == SB_THUMBTRACK)
		{
			switch (GetDlgCtrlID((HWND)lParam))
			{
			case IDC_JERK:
				f = (Nf32)HIWORD(wParam) * (Nf32)(TRACKBAR_JERK_MAX) / (Nf32)(TRACKBAR_SIZE);
				//f = NMAX(DEFAULT_MIN_NLKINLIMITS_JERK_MAX, f);
				if(r=pcurrent_dtspecs->m_limits.setJerkMax(f, FLAG_FALLBACK_MIN | FLAG_MINIMIZE_CHANGE | FLAG_JERK))
					updateKinLimitsCtrls(hwndDlg, &pcurrent_dtspecs->m_limits, r, txtout);
				else
				{
					sprintf(txtout, "%.4f", f);
					SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT_JERK), txtout);
				}
				NErrorIf(!pcurrent_dtspecs->m_limits.isConsistent(), NERROR_INCONSISTENT_VALUES);
				// On prévient Explorer d'une mise à jour de 'pcurrent_dtspecs->m_limits'
				SendMessage(GetParent(hwndDlg), WM_MESSAGE_SPECS_KINLIMITS_UPDATED,  (WPARAM)APP_WINDOW_ENUM::RB_WIN_KINLIMITS, (LPARAM)pcurrent_dtspecs );
				break;

			case IDC_VELOCITY:
				f = (Nf32)HIWORD(wParam) * (Nf32)(TRACKBAR_VELOCITY_MAX) / (Nf32)(TRACKBAR_SIZE);
				//f = NMAX(DEFAULT_MIN_NLKINLIMITS_VELOCITY_MAX, f);
				if (r = pcurrent_dtspecs->m_limits.setVelocityMax(f, FLAG_FALLBACK_MIN | FLAG_MINIMIZE_CHANGE | FLAG_VELOCITY))
					updateKinLimitsCtrls(hwndDlg, &pcurrent_dtspecs->m_limits, r, txtout);
				else
				{
					sprintf(txtout, "%.4f", f);
					SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT_VELOCITY), txtout);
				}
				NErrorIf(!pcurrent_dtspecs->m_limits.isConsistent(), NERROR_INCONSISTENT_VALUES);
				// On prévient Explorer d'une mise à jour de 'pcurrent_dtspecs->m_limits'
				SendMessage(GetParent(hwndDlg), WM_MESSAGE_SPECS_KINLIMITS_UPDATED, (WPARAM)APP_WINDOW_ENUM::RB_WIN_KINLIMITS, (LPARAM)pcurrent_dtspecs);
				break;

			case IDC_ACCEL:
				f = (Nf32)HIWORD(wParam) * (Nf32)(TRACKBAR_ACCEL_MAX) / (Nf32)(TRACKBAR_SIZE);
				//f = NMAX(DEFAULT_MIN_NLKINLIMITS_ACCELERATION_MAX, f);
				if (r = pcurrent_dtspecs->m_limits.setAccelerationMax(f, FLAG_FALLBACK_MIN | FLAG_MINIMIZE_CHANGE | FLAG_ACCELERATION))
					updateKinLimitsCtrls(hwndDlg, &pcurrent_dtspecs->m_limits, r, txtout);
				else
				{
					sprintf(txtout, "%.4f", f);
					SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT_ACCEL), txtout);
				}
				NErrorIf(!pcurrent_dtspecs->m_limits.isConsistent(), NERROR_INCONSISTENT_VALUES);
				// On prévient Explorer d'une mise à jour de 'pcurrent_dtspecs->m_limits'
				SendMessage(GetParent(hwndDlg), WM_MESSAGE_SPECS_KINLIMITS_UPDATED, (WPARAM)APP_WINDOW_ENUM::RB_WIN_KINLIMITS, (LPARAM)pcurrent_dtspecs);
				break;
			}
		}
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			// On prévient Explorer de la destruction de la fenetre
			SendMessage(GetParent(hwndDlg), WM_MESSAGE_APP_WINDOW_CLOSED, (WPARAM)APP_WINDOW_ENUM::RB_WIN_KINLIMITS, (LPARAM)hwndDlg);
			DestroyWindow(hwndDlg);
			return TRUE;
			// EDIT VALUES: LIMITS			
		
		case IDC_EDIT_JERK:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				if (r = pcurrent_dtspecs->m_limits.setJerkMax(GetNf32FromEditControl((HWND)lParam, DEFAULT_MIN_NLKINLIMITS_JERK_MAX, TRACKBAR_JERK_MAX, NULL), FLAG_FALLBACK_MIN | FLAG_MINIMIZE_CHANGE | FLAG_JERK))
					updateKinLimitsCtrls(hwndDlg, &pcurrent_dtspecs->m_limits, r, txtout);
				else
				{
					d = (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*pcurrent_dtspecs->m_limits.getJerkMax() / (Nf32)(TRACKBAR_JERK_MAX));
					SendMessage(GetDlgItem(hwndDlg, IDC_JERK), TBM_SETPOS, TRUE, d);
				}
				NErrorIf(!pcurrent_dtspecs->m_limits.isConsistent(), NERROR_INCONSISTENT_VALUES);
				// On prévient Explorer d'une mise à jour de 'pcurrent_dtspecs->m_limits'
				SendMessage(GetParent(hwndDlg), WM_MESSAGE_SPECS_KINLIMITS_UPDATED, (WPARAM)APP_WINDOW_ENUM::RB_WIN_KINLIMITS, (LPARAM)pcurrent_dtspecs);
				break;

			default:
				break;
			}
			return TRUE;

		case IDC_EDIT_ACCEL:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				if (r = pcurrent_dtspecs->m_limits.setAccelerationMax(GetNf32FromEditControl((HWND)lParam, DEFAULT_MIN_NLKINLIMITS_ACCELERATION_MAX, TRACKBAR_ACCEL_MAX), FLAG_FALLBACK_MIN | FLAG_MINIMIZE_CHANGE | FLAG_ACCELERATION))
					updateKinLimitsCtrls(hwndDlg, &pcurrent_dtspecs->m_limits, r, txtout);
				else
				{
					d = (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*pcurrent_dtspecs->m_limits.getAccelerationMax() / (Nf32)(TRACKBAR_ACCEL_MAX));
					SendMessage(GetDlgItem(hwndDlg, IDC_ACCEL), TBM_SETPOS, TRUE, d);
				}
				NErrorIf(!pcurrent_dtspecs->m_limits.isConsistent(), NERROR_INCONSISTENT_VALUES);
				// On prévient Explorer d'une mise à jour de 'pcurrent_dtspecs->m_limits'
				SendMessage(GetParent(hwndDlg), WM_MESSAGE_SPECS_KINLIMITS_UPDATED, (WPARAM)APP_WINDOW_ENUM::RB_WIN_KINLIMITS, (LPARAM)pcurrent_dtspecs);
				break;

			default:
				break;
			}
			return TRUE;

		case IDC_EDIT_VELOCITY:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				if (r = pcurrent_dtspecs->m_limits.setVelocityMax(GetNf32FromEditControl((HWND)lParam, DEFAULT_MIN_NLKINLIMITS_VELOCITY_MAX, TRACKBAR_VELOCITY_MAX), FLAG_FALLBACK_MIN | FLAG_MINIMIZE_CHANGE | FLAG_VELOCITY))
					updateKinLimitsCtrls(hwndDlg, &pcurrent_dtspecs->m_limits, r, txtout);
				else
				{
					d = (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*pcurrent_dtspecs->m_limits.getVelocityMax() / (Nf32)(TRACKBAR_VELOCITY_MAX));
					SendMessage(GetDlgItem(hwndDlg, IDC_VELOCITY), TBM_SETPOS, TRUE, d);
				}
				NErrorIf(!pcurrent_dtspecs->m_limits.isConsistent(), NERROR_INCONSISTENT_VALUES);
				// On prévient Explorer d'une mise à jour de 'pcurrent_dtspecs->m_limits'
				SendMessage(GetParent(hwndDlg), WM_MESSAGE_SPECS_KINLIMITS_UPDATED, (WPARAM)APP_WINDOW_ENUM::RB_WIN_KINLIMITS, (LPARAM)pcurrent_dtspecs);
				break;

			default:
				break;
			}
			return TRUE;
		
		default:
			break;
		}
		return TRUE;

	// Explorer associe une structure DTSpecs à cette BDD:
	case WM_MESSAGE_SET_CURRENT_SPECS:
		NErrorIf(wParam != APP_EXPLORER_WINDOW, NERROR_INCONSISTENT_PARAMETERS); // pas une erreur juste une verif...
		// WPARAM : APP_EXPLORER_WINDOW ( Id de l'explorer )
		// LPARAM : NEW  REGISTRED WORKBENCH
		pdtspecs = (NLDRIVETRAINSPECS*)lParam;
		NErrorIf(pdtspecs == pcurrent_dtspecs, NERROR_INCONSISTENT_PARAMETERS); // Ils devraient être différents !
		SetWindowLong(hwndDlg, GWL_USERDATA, (LONG)pdtspecs);
		init(hwndDlg, pdtspecs);
		return TRUE;

	default:
		break;
	}
	return FALSE;
}

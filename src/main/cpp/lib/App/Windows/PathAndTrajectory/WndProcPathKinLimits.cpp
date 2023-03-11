#include <iostream>
#include <windows.h>
#include <CommCtrl.h>

#include "../../../../resource.h"
#include "../../../NL/MotionControl/NLKinTweak.h"
#include "../../App_Init.h"
#include "../WindowUtilities.h"

#include "../Robot/WndProcRobotKinLimits.h"
#include "WndProcPathKinLimits.h"

void init(HWND hwnd, NLPATH_WORKBENCH* pcurrent_workbench)
{
	Nchar				txtout[TXTOUT_SIZE];
	Nu32				d;
	
	NErrorIf(!pcurrent_workbench->m_pTrajectory, NERROR_NULL_POINTER);

	if( ISFLAG_ON(pcurrent_workbench->m_pTrajectory->m_flags, FLAG_NLTRJ_SPECIFIC_KINLIMITS) )
	{
		CheckDlgButton(hwnd, IDC_CHECK_USE_ROBOT_KINLIMITS, BST_UNCHECKED);

		// JERK
		sprintf(txtout, "%.4f", pcurrent_workbench->m_pTrajectory->m_specificKinLimits.getJerkMax());
		SetWindowText(GetDlgItem(hwnd, IDC_EDIT_JERK), txtout);
		d = (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*pcurrent_workbench->m_pTrajectory->m_specificKinLimits.getJerkMax() / (Nf32)(TRACKBAR_JERK_MAX));
		SendMessage(GetDlgItem(hwnd, IDC_JERK), TBM_SETPOS, TRUE, d);
		// Accel
		sprintf(txtout, "%.4f", pcurrent_workbench->m_pTrajectory->m_specificKinLimits.getAccelerationMax());
		SetWindowText(GetDlgItem(hwnd, IDC_EDIT_ACCEL), txtout);
		d = (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*pcurrent_workbench->m_pTrajectory->m_specificKinLimits.getAccelerationMax() / (Nf32)(TRACKBAR_ACCEL_MAX));
		SendMessage(GetDlgItem(hwnd, IDC_ACCEL), TBM_SETPOS, TRUE, d);
		// Velocity
		sprintf(txtout, "%.4f", pcurrent_workbench->m_pTrajectory->m_specificKinLimits.getVelocityMax());
		SetWindowText(GetDlgItem(hwnd, IDC_EDIT_VELOCITY), txtout);
		d = (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*pcurrent_workbench->m_pTrajectory->m_specificKinLimits.getVelocityMax() / (Nf32)(TRACKBAR_VELOCITY_MAX));
		SendMessage(GetDlgItem(hwnd, IDC_VELOCITY), TBM_SETPOS, TRUE, d);
		//SetWindowText(GetDlgItem(hwnd, IDC_STATIC_INFOS), pcurrent_workbench->getKinLimitsInfoString(txtout, TXTOUT_SIZE));
		EnumChildWindows(hwnd, EnableCtrl_Callback, 0);
	}
	else
	{
		CheckDlgButton(hwnd, IDC_CHECK_USE_ROBOT_KINLIMITS, BST_CHECKED);

		// JERK
		sprintf(txtout, "%.4f", pcurrent_workbench->m_pDriveTrainSpecifications->m_limits.getJerkMax());
		SetWindowText(GetDlgItem(hwnd, IDC_EDIT_JERK), txtout);
		d = (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*pcurrent_workbench->m_pDriveTrainSpecifications->m_limits.getJerkMax() / (Nf32)(TRACKBAR_JERK_MAX));
		SendMessage(GetDlgItem(hwnd, IDC_JERK), TBM_SETPOS, TRUE, d);
		// Accel
		sprintf(txtout, "%.4f", pcurrent_workbench->m_pDriveTrainSpecifications->m_limits.getAccelerationMax());
		SetWindowText(GetDlgItem(hwnd, IDC_EDIT_ACCEL), txtout);
		d = (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*pcurrent_workbench->m_pDriveTrainSpecifications->m_limits.getAccelerationMax() / (Nf32)(TRACKBAR_ACCEL_MAX));
		SendMessage(GetDlgItem(hwnd, IDC_ACCEL), TBM_SETPOS, TRUE, d);
		// Velocity
		sprintf(txtout, "%.4f", pcurrent_workbench->m_pDriveTrainSpecifications->m_limits.getVelocityMax());
		SetWindowText(GetDlgItem(hwnd, IDC_EDIT_VELOCITY), txtout);
		d = (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*pcurrent_workbench->m_pDriveTrainSpecifications->m_limits.getVelocityMax() / (Nf32)(TRACKBAR_VELOCITY_MAX));
		SendMessage(GetDlgItem(hwnd, IDC_VELOCITY), TBM_SETPOS, TRUE, d);
		//SetWindowText(GetDlgItem(hwnd, IDC_STATIC_INFOS), pcurrent_workbench->getKinLimitsInfoString(txtout, TXTOUT_SIZE));
		
		EnumChildWindows(hwnd, DisableCtrlButLParam_Callback, (LPARAM)GetDlgItem(hwnd, IDC_CHECK_USE_ROBOT_KINLIMITS));
		EnableWindow(GetDlgItem(hwnd, IDC_STATIC_INFOS), TRUE);
	}
}


BOOL CALLBACK WndProc_PathKinLimits(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	Nchar				txtout[TXTOUT_SIZE];
	Nf32				f;
	Nu32				d;
	Nu32				r;
	NLPATH_WORKBENCH* pwb;
	NLPATH_WORKBENCH* pcurrent_workbench = (NLPATH_WORKBENCH*)GetWindowLong(hwndDlg, GWL_USERDATA);

	NLDRIVETRAINSPECS	*pdtspecs;
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
				if (r = pcurrent_workbench->m_pTrajectory->m_specificKinLimits.setJerkMax(f, FLAG_FALLBACK_MIN | FLAG_MINIMIZE_CHANGE | FLAG_JERK))
					updateKinLimitsCtrls(hwndDlg, &pcurrent_workbench->m_pTrajectory->m_specificKinLimits, r, txtout);
				else
				{
					sprintf(txtout, EDIT_TXT_FRMT_STR, f);
					SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT_JERK), txtout);
				}

				SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_INFOS), pcurrent_workbench->getKinLimitsInfoString(txtout, TXTOUT_SIZE));
				pcurrent_workbench->requestTrajectoryBuild();
				break;

			case IDC_VELOCITY:
				f = (Nf32)HIWORD(wParam) * (Nf32)(TRACKBAR_VELOCITY_MAX) / (Nf32)(TRACKBAR_SIZE);
				//f = NMAX(DEFAULT_MIN_NLKINLIMITS_VELOCITY_MAX, f);
				if(r=pcurrent_workbench->m_pTrajectory->m_specificKinLimits.setVelocityMax(f, FLAG_FALLBACK_MIN | FLAG_MINIMIZE_CHANGE | FLAG_VELOCITY))
					updateKinLimitsCtrls(hwndDlg, &pcurrent_workbench->m_pTrajectory->m_specificKinLimits, r, txtout);
				else
				{
					sprintf(txtout, EDIT_TXT_FRMT_STR, f);
					SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT_VELOCITY), txtout);
				}

				SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_INFOS), pcurrent_workbench->getKinLimitsInfoString(txtout, TXTOUT_SIZE));
				pcurrent_workbench->requestTrajectoryBuild();
				break;

			case IDC_ACCEL:
				f = (Nf32)HIWORD(wParam) * (Nf32)(TRACKBAR_ACCEL_MAX) / (Nf32)(TRACKBAR_SIZE);
				//f = NMAX(DEFAULT_MIN_NLKINLIMITS_ACCELERATION_MAX, f);
				if(r=pcurrent_workbench->m_pTrajectory->m_specificKinLimits.setAccelerationMax(f, FLAG_FALLBACK_MIN | FLAG_MINIMIZE_CHANGE | FLAG_ACCELERATION))
					updateKinLimitsCtrls(hwndDlg, &pcurrent_workbench->m_pTrajectory->m_specificKinLimits, r, txtout);
				else
				{
					sprintf(txtout, EDIT_TXT_FRMT_STR, f);
					SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT_ACCEL), txtout);
				}
				//sprintf(txtout, "Accel Max: %.4f m/s/s", f);
				//SetWindowText(GetDlgItem(hwndDlg, IDC_OUT_ACCELERATION), txtout);
				SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_INFOS), pcurrent_workbench->getKinLimitsInfoString(txtout, TXTOUT_SIZE));
				pcurrent_workbench->requestTrajectoryBuild();
				break;
			}
		}
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			SendMessage(GetParent(hwndDlg), WM_MESSAGE_APP_WINDOW_CLOSED, (WPARAM)APP_WINDOW_ENUM::WB_WIN_KINLIMITS, (LPARAM)hwndDlg );
			DestroyWindow(hwndDlg);
			return TRUE;
		
		// EDIT:
		case IDC_EDIT_JERK:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				if( r = pcurrent_workbench->m_pTrajectory->m_specificKinLimits.setJerkMax(GetNf32FromEditControl((HWND)lParam, DEFAULT_MIN_NLKINLIMITS_JERK_MAX, TRACKBAR_JERK_MAX, NULL), FLAG_FALLBACK_MIN | FLAG_MINIMIZE_CHANGE | FLAG_JERK ))
					updateKinLimitsCtrls(hwndDlg, &pcurrent_workbench->m_pTrajectory->m_specificKinLimits, r, txtout);
				else
				{
					d = (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*pcurrent_workbench->m_pTrajectory->m_specificKinLimits.getJerkMax() / (Nf32)(TRACKBAR_JERK_MAX));
					SendMessage(GetDlgItem(hwndDlg, IDC_JERK), TBM_SETPOS, TRUE, d);
				}
				
				SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_INFOS), pcurrent_workbench->getKinLimitsInfoString(txtout, TXTOUT_SIZE));
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
				if(r = pcurrent_workbench->m_pTrajectory->m_specificKinLimits.setAccelerationMax(GetNf32FromEditControl((HWND)lParam, DEFAULT_MIN_NLKINLIMITS_ACCELERATION_MAX, TRACKBAR_ACCEL_MAX), FLAG_FALLBACK_MIN | FLAG_MINIMIZE_CHANGE | FLAG_ACCELERATION))
					updateKinLimitsCtrls(hwndDlg, &pcurrent_workbench->m_pTrajectory->m_specificKinLimits, r, txtout);
				else
				{
					d = (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*pcurrent_workbench->m_pTrajectory->m_specificKinLimits.getAccelerationMax() / (Nf32)(TRACKBAR_ACCEL_MAX));
					SendMessage(GetDlgItem(hwndDlg, IDC_ACCEL), TBM_SETPOS, TRUE, d);
				}

				SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_INFOS), pcurrent_workbench->getKinLimitsInfoString(txtout, TXTOUT_SIZE));
				pcurrent_workbench->requestTrajectoryBuild();
				break;

			default:
				break;
			}
			return TRUE;

		case IDC_EDIT_VELOCITY:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				if(r=pcurrent_workbench->m_pTrajectory->m_specificKinLimits.setVelocityMax(GetNf32FromEditControl((HWND)lParam, DEFAULT_MIN_NLKINLIMITS_VELOCITY_MAX, TRACKBAR_VELOCITY_MAX), FLAG_FALLBACK_MIN | FLAG_MINIMIZE_CHANGE | FLAG_VELOCITY))
					updateKinLimitsCtrls(hwndDlg, &pcurrent_workbench->m_pTrajectory->m_specificKinLimits, r, txtout);
				else
				{
					d = (Nu32)ceilf((Nf32)(TRACKBAR_SIZE)*pcurrent_workbench->m_pTrajectory->m_specificKinLimits.getVelocityMax() / (Nf32)(TRACKBAR_VELOCITY_MAX));
					SendMessage(GetDlgItem(hwndDlg, IDC_VELOCITY), TBM_SETPOS, TRUE, d);
				}
				SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_INFOS), pcurrent_workbench->getKinLimitsInfoString(txtout, TXTOUT_SIZE));
				pcurrent_workbench->requestTrajectoryBuild();
				break;

			default:
				break;
			}
			return TRUE;
		// CHECK BOXES
		case IDC_CHECK_USE_ROBOT_KINLIMITS:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_USE_ROBOT_KINLIMITS) == BST_CHECKED)
			{
				FLAG_OFF(pcurrent_workbench->m_pTrajectory->m_flags, FLAG_NLTRJ_SPECIFIC_KINLIMITS);
			}
			else
			{
				FLAG_ON(pcurrent_workbench->m_pTrajectory->m_flags, FLAG_NLTRJ_SPECIFIC_KINLIMITS);
			}
			
			init(hwndDlg, pcurrent_workbench);
			SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_INFOS), pcurrent_workbench->getKinLimitsInfoString(txtout, TXTOUT_SIZE));
			pcurrent_workbench->requestTrajectoryBuild();
			return TRUE;
			
		case IDC_CHECK_ENFORCE_PATH_KLIM_VEL:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_ENFORCE_PATH_KLIM_VEL) == BST_CHECKED)
			{
				FLAG_ON(pcurrent_workbench->m_pTrajectory->m_flags, FLAG_NLTRJ_ENFORCE_VEL);
			}
			else
			{
				FLAG_OFF(pcurrent_workbench->m_pTrajectory->m_flags, FLAG_NLTRJ_ENFORCE_VEL);
			}
			SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_INFOS), pcurrent_workbench->getKinLimitsInfoString(txtout, TXTOUT_SIZE));
			pcurrent_workbench->requestTrajectoryBuild();
			return TRUE;

		case IDC_CHECK_ENFORCE_PATH_KLIM_ACC:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_ENFORCE_PATH_KLIM_ACC) == BST_CHECKED)
			{
				FLAG_ON(pcurrent_workbench->m_pTrajectory->m_flags, FLAG_NLTRJ_ENFORCE_ACC);
			}
			else
			{
				FLAG_OFF(pcurrent_workbench->m_pTrajectory->m_flags, FLAG_NLTRJ_ENFORCE_ACC);
			}
			SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_INFOS), pcurrent_workbench->getKinLimitsInfoString(txtout, TXTOUT_SIZE));
			pcurrent_workbench->requestTrajectoryBuild();
			return TRUE;

		case IDC_CHECK_ENFORCE_PATH_KLIM_JRK:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_ENFORCE_PATH_KLIM_JRK) == BST_CHECKED)
			{
				FLAG_ON(pcurrent_workbench->m_pTrajectory->m_flags, FLAG_NLTRJ_ENFORCE_JRK);
			}
			else
			{
				FLAG_OFF(pcurrent_workbench->m_pTrajectory->m_flags, FLAG_NLTRJ_ENFORCE_JRK);
			}
			SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_INFOS), pcurrent_workbench->getKinLimitsInfoString(txtout, TXTOUT_SIZE));
			pcurrent_workbench->requestTrajectoryBuild();
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
		SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_INFOS), pwb->getKinLimitsInfoString(txtout, TXTOUT_SIZE));
		return TRUE;

	case WM_MESSAGE_DELETE_CURRENT_PATH_WORKBENCH:
		NErrorIf(wParam != APP_EXPLORER_WINDOW, NERROR_INCONSISTENT_PARAMETERS); // pas une erreur juste une verif...
		// WPARAM : APP_EXPLORER_WINDOW ( Id de l'explorer )
		// LPARAM : TO BE DELETE REGISTRED WORKBENCH
		pwb = (NLPATH_WORKBENCH*)lParam;
		NErrorIf(pwb != pcurrent_workbench, NERROR_INCONSISTENT_PARAMETERS); // Ils devraient être égaux !
		SetWindowLong(hwndDlg, GWL_USERDATA, 0);
		EnumChildWindows(hwndDlg, DisableCtrl_Callback, 0);
		return TRUE;

	case WM_MESSAGE_SPECS_KINLIMITS_UPDATED:
		pdtspecs = (NLDRIVETRAINSPECS*)lParam;
		// La  DTSpecs modifiée est celle ref. par le workbench, obligatoireement, car il n'y en a qu'une pour le moment.
		// mais... un jour on gerera peut-être plusieurs robots, donc plusieurs DT specs ... 
		if (pcurrent_workbench && (pdtspecs == pcurrent_workbench->m_pDriveTrainSpecifications)) // pcurrent_workbench peut être null si ce dernier à été recement détruit et la fenetre restée ouverte ...
		{
			init(hwndDlg, pcurrent_workbench);
			SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_INFOS), pcurrent_workbench->getKinLimitsInfoString(txtout, TXTOUT_SIZE));
		}
		return TRUE;

	default:
		break;
	}
	return FALSE;
}

#include "../../../../resource.h"
#include "../../App_Init.h"
#include "../WindowUtilities.h"
#include "WndProcRobotGeometry.h"

#define TXTOUT_SIZE 32
#define TXTGET_SIZE	16

static void init(HWND hwnd, NLDRIVETRAINSPECS* pdtspecs)
{
	Nchar				txtout[TXTOUT_SIZE];

	// Base:
	sprintf(txtout, "%.4f", pdtspecs->m_axleTrack);
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_AXLETRACK), txtout);
	sprintf(txtout, "%.4f", pdtspecs->m_wheelBase);
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_WHEELBASE), txtout);

	// Frame:
	sprintf(txtout, "%.4f", pdtspecs->m_size.x);
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_FRAME_WIDTH), txtout);
	sprintf(txtout, "%.4f", pdtspecs->m_size.y);
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_FRAME_LENGTH), txtout);
	sprintf(txtout, "%.4f", pdtspecs->m_size.z);
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_FRAME_HEIGHT), txtout);

	// WHEELS
	sprintf(txtout, "%.4f", pdtspecs->m_wheelRadius);
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_WHEEL_RADIUS), txtout);
	sprintf(txtout, "%.4f", pdtspecs->m_wheelWidth);
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_WHEEL_WIDTH), txtout);
}

BOOL CALLBACK WndProc_RobotGeometry(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	NLDRIVETRAINSPECS* pdtspecs;
	NLDRIVETRAINSPECS* pcurrent_dtspecs = (NLDRIVETRAINSPECS*)GetWindowLong(hwndDlg, GWL_USERDATA);

	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			SendMessage(GetParent(hwndDlg), WM_MESSAGE_APP_WINDOW_CLOSED,(WPARAM)APP_WINDOW_ENUM::RB_WIN_GEOMETRY,(LPARAM)hwndDlg);
			DestroyWindow(hwndDlg);
			return TRUE;

		// BASE
		case IDC_EDIT_AXLETRACK:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				pcurrent_dtspecs->m_axleTrack = GetNf32FromEditControl((HWND)lParam);
				// On prévient Explorer d'une mise à jour de 'pcurrent_dtspecs'
				SendMessage(GetParent(hwndDlg), WM_MESSAGE_SPECS_PHYSICS_UPDATED, (WPARAM)APP_WINDOW_ENUM::RB_WIN_GEOMETRY, (LPARAM)pcurrent_dtspecs);
				//RequestTrajectoryBuild;
				break;
			default:
				break;
			}
			return TRUE;

		case IDC_EDIT_WHEELBASE:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				pcurrent_dtspecs->m_wheelBase = GetNf32FromEditControl((HWND)lParam);
				// On prévient Explorer d'une mise à jour de 'pcurrent_dtspecs'
				SendMessage(GetParent(hwndDlg), WM_MESSAGE_SPECS_PHYSICS_UPDATED, (WPARAM)APP_WINDOW_ENUM::RB_WIN_GEOMETRY, (LPARAM)pcurrent_dtspecs);
				break;
			default:
				break;
			}
			return TRUE;

		// FRAME
		case IDC_EDIT_FRAME_WIDTH:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				pcurrent_dtspecs->m_size.x = GetNf32FromEditControl((HWND)lParam);
				// On prévient Explorer d'une mise à jour de 'pcurrent_dtspecs'
				SendMessage(GetParent(hwndDlg), WM_MESSAGE_SPECS_PHYSICS_UPDATED, (WPARAM)APP_WINDOW_ENUM::RB_WIN_GEOMETRY, (LPARAM)pcurrent_dtspecs);
				break;
			default:
				break;
			}
			return TRUE;
		case IDC_EDIT_FRAME_LENGTH:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				pcurrent_dtspecs->m_size.y = GetNf32FromEditControl((HWND)lParam);
				// On prévient Explorer d'une mise à jour de 'pcurrent_dtspecs'
				SendMessage(GetParent(hwndDlg), WM_MESSAGE_SPECS_PHYSICS_UPDATED, (WPARAM)APP_WINDOW_ENUM::RB_WIN_GEOMETRY, (LPARAM)pcurrent_dtspecs);
				break;
			default:
				break;
			}
			return TRUE;

		case IDC_EDIT_FRAME_HEIGHT:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				pcurrent_dtspecs->m_size.z = GetNf32FromEditControl((HWND)lParam);
				// On prévient Explorer d'une mise à jour de 'pcurrent_dtspecs'
				SendMessage(GetParent(hwndDlg), WM_MESSAGE_SPECS_PHYSICS_UPDATED, (WPARAM)APP_WINDOW_ENUM::RB_WIN_GEOMETRY, (LPARAM)pcurrent_dtspecs);
				break;
			default:
				break;
			}
			return TRUE;

		// WHEELS
		case IDC_EDIT_WHEEL_RADIUS:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				pcurrent_dtspecs->m_wheelRadius = GetNf32FromEditControl((HWND)lParam);
				// On prévient Explorer d'une mise à jour de 'pcurrent_dtspecs'
				SendMessage(GetParent(hwndDlg), WM_MESSAGE_SPECS_PHYSICS_UPDATED, (WPARAM)APP_WINDOW_ENUM::RB_WIN_GEOMETRY, (LPARAM)pcurrent_dtspecs);
				break;
			default:
				break;
			}
			return TRUE;

		case IDC_EDIT_WHEEL_WIDTH:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				pcurrent_dtspecs->m_wheelWidth = GetNf32FromEditControl((HWND)lParam);
				// On prévient Explorer d'une mise à jour de 'pcurrent_dtspecs'
				SendMessage(GetParent(hwndDlg), WM_MESSAGE_SPECS_PHYSICS_UPDATED, (WPARAM)APP_WINDOW_ENUM::RB_WIN_GEOMETRY, (LPARAM)pcurrent_dtspecs);
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

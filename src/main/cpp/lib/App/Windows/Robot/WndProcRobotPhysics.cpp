#include "../../../../resource.h"
#include "../../../NL/MotionControl/NLKinTweak.h"
#include "../../../NL/MotionControl/NLPhysics.h"

#include "../../App_Init.h"
#include "../WindowUtilities.h"
#include "WndProcRobotPhysics.h"

#define TXTOUT_SIZE 32
#define TXTGET_SIZE	16

static void init(HWND hwnd, NLDRIVETRAINSPECS* pdtspecs)
{
	Nchar				txtout[TXTOUT_SIZE];

	// Center of Mass:
	NVEC3f32	vcm;
	pdtspecs->getCenterOfMass(&vcm);

	sprintf(txtout, "%.4f", vcm.x);
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_CENTEROFMASS_X), txtout);
	sprintf(txtout, "%.4f", vcm.y);
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_CENTEROFMASS_Y), txtout);
	sprintf(txtout, "%.4f", vcm.z);
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_CENTEROFMASS_Z), txtout);

	// Mass & Weight:
	sprintf(txtout, "%.4f", pdtspecs->m_mass);
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_MASS), txtout);
	sprintf(txtout, "%.4f", pdtspecs->m_mass* NLPHYSICS_GRAVITY);
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_WEIGHT), txtout);

	// Static Friction ( of traction Wheels on the ground )
	sprintf(txtout, "%.4f", pdtspecs->getStaticFriction());
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_STATIC_FRICTION), txtout);

}

BOOL CALLBACK WndProc_RobotPhysics(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	Nchar				txtout[TXTOUT_SIZE];
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
			SendMessage(GetParent(hwndDlg), WM_MESSAGE_APP_WINDOW_CLOSED,(WPARAM)APP_WINDOW_ENUM::RB_WIN_PHYSICS,(LPARAM)hwndDlg);
			DestroyWindow(hwndDlg);
			return TRUE;

		case IDC_EDIT_MASS:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				pcurrent_dtspecs->m_mass = GetNf32FromEditControl((HWND)lParam);
				pcurrent_dtspecs->m_weight = pcurrent_dtspecs->m_mass * NLPHYSICS_GRAVITY;
				sprintf(txtout, "%.4f", pcurrent_dtspecs->m_weight);
				SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT_WEIGHT), txtout);
				// On prévient Explorer d'une mise à jour de 'pcurrent_dtspecs'
				SendMessage(GetParent(hwndDlg), WM_MESSAGE_SPECS_PHYSICS_UPDATED, (WPARAM)APP_WINDOW_ENUM::RB_WIN_PHYSICS, (LPARAM)pcurrent_dtspecs);
				break;
			default:
				break;
			}
			return TRUE;

		case IDC_EDIT_STATIC_FRICTION:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				pcurrent_dtspecs->setStaticFriction(GetNf32FromEditControl((HWND)lParam));
				// On prévient Explorer d'une mise à jour de 'pcurrent_dtspecs'
				SendMessage(GetParent(hwndDlg), WM_MESSAGE_SPECS_PHYSICS_UPDATED, (WPARAM)APP_WINDOW_ENUM::RB_WIN_PHYSICS, (LPARAM)pcurrent_dtspecs);
				break;
			default:
				break;
			}
			return TRUE;

		// Center of Mass:
		case IDC_EDIT_CENTEROFMASS_X:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				pcurrent_dtspecs->setCenterOfMassX( GetNf32FromEditControl((HWND)lParam) );
				// On prévient Explorer d'une mise à jour de 'pcurrent_dtspecs'
				SendMessage(GetParent(hwndDlg), WM_MESSAGE_SPECS_PHYSICS_UPDATED, (WPARAM)APP_WINDOW_ENUM::RB_WIN_PHYSICS, (LPARAM)pcurrent_dtspecs);
				break;
			default:
				break;
			}
			return TRUE;
		case IDC_EDIT_CENTEROFMASS_Y:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				pcurrent_dtspecs->setCenterOfMassY( GetNf32FromEditControl((HWND)lParam));
				// On prévient Explorer d'une mise à jour de 'pcurrent_dtspecs'
				SendMessage(GetParent(hwndDlg), WM_MESSAGE_SPECS_PHYSICS_UPDATED, (WPARAM)APP_WINDOW_ENUM::RB_WIN_PHYSICS, (LPARAM)pcurrent_dtspecs);
				break;
			default:
				break;
			}
			return TRUE;
		case IDC_EDIT_CENTEROFMASS_Z:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				pcurrent_dtspecs->setCenterOfMassZ( GetNf32FromEditControl((HWND)lParam));
				// On prévient Explorer d'une mise à jour de 'pcurrent_dtspecs'
				SendMessage(GetParent(hwndDlg), WM_MESSAGE_SPECS_PHYSICS_UPDATED, (WPARAM)APP_WINDOW_ENUM::RB_WIN_PHYSICS, (LPARAM)pcurrent_dtspecs);
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

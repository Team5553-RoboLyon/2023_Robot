#include "../../../resource.h"
#include "../App_Init.h"
#include "../Windows/WindowUtilities.h"
#include "../../NL/NLProject.h"

#include "WndProcGlobalSettings.h"

extern NLPROJECT		Project;
extern HWND				hwnd_Explorer;

HWND					hwnd_GlobalSettings; 

#define TXTOUT_SIZE 32
static void init(HWND hwnd, NLPROJECT *pprj)
{
	Nchar				txtout[TXTOUT_SIZE];

	sprintf(txtout, "%.4f", pprj->m_ramseteParams.m_b);
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_RAMSETE_B), txtout);

	sprintf(txtout, "%.4f", pprj->m_ramseteParams.m_zeta);
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_RAMSETE_ZETA), txtout);
}

BOOL CALLBACK WndProc_GlobalSettings(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
		init(hwndDlg, &Project);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			DestroyWindow(hwndDlg);
			hwnd_GlobalSettings = NULL;
			return TRUE;

		case IDC_EDIT_RAMSETE_B:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				Project.m_ramseteParams.m_b = GetNf32FromEditControl((HWND)lParam);
				// On prévient Explorer d'une mise à jour de 'RAMSETE global settings'
				// SendMessage(hwnd_Explorer, WM_MESSAGE_GLOBAL_SETTINGS_RAMSETE_UPDATED, NVOID, (LPARAM)&Project.m_ramseteParams);
				break;
			default:
				break;
			}
			return TRUE;

		case IDC_EDIT_RAMSETE_ZETA:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				Project.m_ramseteParams.m_zeta = GetNf32FromEditControl((HWND)lParam);
				// On prévient Explorer d'une mise à jour de 'RAMSETE global settings'
				// SendMessage(hwnd_Explorer, WM_MESSAGE_GLOBAL_SETTINGS_RAMSETE_UPDATED, NVOID, (LPARAM)&Project.m_ramseteParams);
				break;
			default:
				break;
			}
			return TRUE;
		}
		break; // case WM_COMMAND:

	default:
		break;
	}
	return FALSE;
}

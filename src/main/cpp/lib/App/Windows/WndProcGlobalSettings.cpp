#include "../../../resource.h"
#include "../App_Init.h"
#include "../Windows/WindowUtilities.h"
#include "../../NL/NLProject.h"
#include "../../NL/MotionControl/Path/Builders/PolygonalChain/NLPathBuilderPolygonalChain.h"
#include "../../NL/MotionControl/Path/Builders/WayPoints/NLPathBuilderWayPoints.h"

#include "WndProcGlobalSettings.h"

extern NLPROJECT		Project;
extern HWND				hwnd_Explorer;

//HWND					hwnd_GlobalSettings; 

#define TXTOUT_SIZE 32
static void init(HWND hwnd, NLPROJECT *pprj)
{
	Nchar				txtout[TXTOUT_SIZE];

	sprintf(txtout, "%.4f", pprj->m_ramseteParams.m_b);
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_RAMSETE_B), txtout);

	sprintf(txtout, "%.4f", pprj->m_ramseteParams.m_zeta);
	SetWindowText(GetDlgItem(hwnd, IDC_EDIT_RAMSETE_ZETA), txtout);
}
void updateBuildingPairSubdisplayCtrl(const HWND hwnd, const Nu32 flags)
{
	static Nu32 enable = 1;

	if (enable == 1)
	{
		if (FLAGS_TEST(flags, MASK__NLPBW_DRAW_BUILD_PAIRS, 0))
		{
			EnableWindow(GetDlgItem(hwnd,IDC_CHECK_DRAW_BUILDING_PAIR_BETA_ANGLES),FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_CHECK_DRAW_BUILDING_PAIR_ALPHA_ANGLES), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_CHECK_DRAW_BUILDING_PAIR_LINES), FALSE);

			enable = 0;
		}
	}
	else
	{
		if (!FLAGS_TEST(flags, MASK__NLPBW_DRAW_BUILD_PAIRS, 0))
		{
			EnableWindow(GetDlgItem(hwnd, IDC_CHECK_DRAW_BUILDING_PAIR_BETA_ANGLES), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_CHECK_DRAW_BUILDING_PAIR_ALPHA_ANGLES), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_CHECK_DRAW_BUILDING_PAIR_LINES), TRUE);

			enable = 1;
		}
	}
}

void updatePathBuildersFlags(const HWND hwnd, const Nu32 flags,const NLPROJECT* pprj)
{
	Nu32 all = 0;

	if (IsDlgButtonChecked(hwnd, IDC_CHECK_APPLY_TO_ALL_PATHBUILDERS) == BST_CHECKED)
		all = 1;

	NLPATH_WORKBENCH* pwb = nullptr;

	if (all) // all pathbuilders
	{
		pwb = (NLPATH_WORKBENCH*)pprj->m_workbenchArray.pFirst;
		for(Nu32 i=0;i<pprj->m_workbenchArray.Size;i++,pwb++)
			pwb->m_pPathBuilder->setFlags(flags);
	}
	else
	{
		pwb = (NLPATH_WORKBENCH*)GetWindowLong(hwnd, GWL_USERDATA);
		NErrorIf(pprj->m_pCurrentWorkbench != pwb, NERROR_INCONSISTENT_VALUES);
		if(pwb)
			pwb->m_pPathBuilder->setFlags(flags);
	}
}

static Nu32 FlagsWaypoint	= NLPBS_WAYPOINT;
static Nu32 FlagsPolygon	= NLPBS_POLYGON;

BOOL CALLBACK WndProc_GlobalSettings(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	NLPATH_WORKBENCH* pwb		= (NLPATH_WORKBENCH*)GetWindowLong(hwndDlg, GWL_USERDATA);
	NLPATH_WORKBENCH* pwb_new	= nullptr;


	switch (message)
	{
	case WM_INITDIALOG:
		init(hwndDlg, &Project);
		
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			SendMessage(GetParent(hwndDlg), WM_MESSAGE_APP_WINDOW_CLOSED, (WPARAM)APP_WINDOW_ENUM::APP_WIN_GLOBALSETTINGS, (LPARAM)hwndDlg);
			DestroyWindow(hwndDlg);
			return TRUE;

		case IDC_CHECK_APPLY_TO_ALL_PATHBUILDERS:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_APPLY_TO_ALL_PATHBUILDERS) == BST_CHECKED)
			{
				updatePathBuildersFlags(hwndDlg, FlagsWaypoint, &Project);
				updatePathBuildersFlags(hwndDlg, FlagsPolygon, &Project);
			}
				return TRUE;
/*
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
*/
		// FLAGS SETUP FOR POLYGONAL BUILDERS
		case IDC_CHECK_DRAW_PLG_HANDLE:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_PLG_HANDLE) == BST_CHECKED)
			{
				FLAG_ON(FlagsPolygon, FLAG_NLPATH_BUILDER_POLYGONAL_CHAIN_EDITING_DRAW_HANDLE);
				//pbuilder->setFlagsOn(NLPBS_WAYPOINT, FLAG_NLPBW_DRAW_HANDLES);
			}
			else
			{
				FLAG_OFF(FlagsPolygon, FLAG_NLPATH_BUILDER_POLYGONAL_CHAIN_EDITING_DRAW_HANDLE);
				//pbuilder->setFlagsOff(NLPBS_WAYPOINT, FLAG_NLPBW_DRAW_HANDLES);
			}
			updatePathBuildersFlags(hwndDlg, FlagsPolygon, &Project);
			return TRUE;

		case IDC_CHECK_DRAW_PLG_SEGMENT:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_PLG_SEGMENT) == BST_CHECKED)
			{
				FLAG_ON(FlagsPolygon, FLAG_NLPATH_BUILDER_POLYGONAL_CHAIN_EDITING_DRAW_SEGMENT);
				//pbuilder->setFlagsOn(NLPBS_WAYPOINT, FLAG_NLPBW_DRAW_HANDLES);
			}
			else
			{
				FLAG_OFF(FlagsPolygon, FLAG_NLPATH_BUILDER_POLYGONAL_CHAIN_EDITING_DRAW_SEGMENT);
				//pbuilder->setFlagsOff(NLPBS_WAYPOINT, FLAG_NLPBW_DRAW_HANDLES);
			}
			updatePathBuildersFlags(hwndDlg, FlagsPolygon, &Project);
			return TRUE;

		case IDC_CHECK_DRAW_PLG_VERTEX:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_PLG_VERTEX) == BST_CHECKED)
			{
				FLAG_ON(FlagsPolygon, FLAG_NLPATH_BUILDER_POLYGONAL_CHAIN_EDITING_DRAW_VERTEX);
				//pbuilder->setFlagsOn(NLPBS_WAYPOINT, FLAG_NLPBW_DRAW_HANDLES);
			}
			else
			{
				FLAG_OFF(FlagsPolygon, FLAG_NLPATH_BUILDER_POLYGONAL_CHAIN_EDITING_DRAW_VERTEX);
				//pbuilder->setFlagsOff(NLPBS_WAYPOINT, FLAG_NLPBW_DRAW_HANDLES);
			}
			updatePathBuildersFlags(hwndDlg, FlagsPolygon, &Project);
			return TRUE;

		// FLAGS SETUP FOR WAYPOINT BUILDERS
		case IDC_CHECK_DRAW_PATH_HANDLES:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_PATH_HANDLES) == BST_CHECKED)
			{
				FLAG_ON(FlagsWaypoint, FLAG_NLPBW_DRAW_HANDLES);
				//pbuilder->setFlagsOn(NLPBS_WAYPOINT, FLAG_NLPBW_DRAW_HANDLES);
			}
			else
			{
				FLAG_OFF(FlagsWaypoint, FLAG_NLPBW_DRAW_HANDLES);
				//pbuilder->setFlagsOff(NLPBS_WAYPOINT, FLAG_NLPBW_DRAW_HANDLES);
			}
			updatePathBuildersFlags(hwndDlg, FlagsWaypoint, &Project);
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_CIRCLES:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_CIRCLES) == BST_CHECKED)
			{
				FLAG_ON(FlagsWaypoint, FLAG_NLPBW_DRAW_BUILD_CIRCLES);
			}
			else
			{
				FLAG_OFF(FlagsWaypoint, FLAG_NLPBW_DRAW_BUILD_CIRCLES);
			}
			updatePathBuildersFlags(hwndDlg, FlagsWaypoint, &Project);
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_PAIR_ALPHA_ANGLES:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_PAIR_ALPHA_ANGLES) == BST_CHECKED)
			{
				FLAG_ON(FlagsWaypoint, FLAG_NLPBW_DRAW_BUILD_PAIR_ALPHA_ANGLES);
			}
			else
			{
				FLAG_OFF(FlagsWaypoint, FLAG_NLPBW_DRAW_BUILD_PAIR_ALPHA_ANGLES);
			}
			updatePathBuildersFlags(hwndDlg, FlagsWaypoint, &Project);
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_PAIR_BETA_ANGLES:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_PAIR_BETA_ANGLES) == BST_CHECKED)
			{
				FLAG_ON(FlagsWaypoint, FLAG_NLPBW_DRAW_BUILD_PAIR_BETA_ANGLES);
			}
			else
			{
				FLAG_OFF(FlagsWaypoint, FLAG_NLPBW_DRAW_BUILD_PAIR_BETA_ANGLES);
			}
			updatePathBuildersFlags(hwndDlg, FlagsWaypoint, &Project);
			return TRUE;

/*
		case IDC_CHECK_DRAW_BUILDING_BASES:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_BASES) == BST_CHECKED)
			{
				FLAG_ON(FlagsWaypoint, FLAG_NLPBW_DRAW_BUILD_BASES);
			}
			else
			{
				FLAG_OFF(FlagsWaypoint, FLAG_NLPBW_DRAW_BUILD_BASES);
			}
			updatePathBuildersFlags(hwndDlg, FlagsWaypoint, &Project);
			return TRUE;
*/
		case IDC_CHECK_DRAW_BUILDING_PAIR_LINES:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_PAIR_LINES) == BST_CHECKED)
			{
				FLAG_ON(FlagsWaypoint, FLAG_NLPBW_DRAW_BUILD_PAIR_LINES);
			}
			else
			{
				FLAG_OFF(FlagsWaypoint, FLAG_NLPBW_DRAW_BUILD_PAIR_LINES);
			}
			updatePathBuildersFlags(hwndDlg, FlagsWaypoint, &Project);
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_PAIR_0:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_PAIR_0) == BST_CHECKED)
			{
				FLAG_ON(FlagsWaypoint, FLAG_NLPBW_DRAW_BUILD_PAIR_0);
			}
			else
			{
				FLAG_OFF(FlagsWaypoint, FLAG_NLPBW_DRAW_BUILD_PAIR_0);
			}
			updateBuildingPairSubdisplayCtrl(hwndDlg, FlagsWaypoint);
			updatePathBuildersFlags(hwndDlg, FlagsWaypoint, &Project);
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_PAIR_1:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_PAIR_1) == BST_CHECKED)
			{
				FLAG_ON(FlagsWaypoint, FLAG_NLPBW_DRAW_BUILD_PAIR_1);
			}
			else
			{
				FLAG_OFF(FlagsWaypoint, FLAG_NLPBW_DRAW_BUILD_PAIR_1);
			}
			updateBuildingPairSubdisplayCtrl(hwndDlg, FlagsWaypoint);
			updatePathBuildersFlags(hwndDlg, FlagsWaypoint, &Project);
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_PAIR_2:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_PAIR_2) == BST_CHECKED)
			{
				FLAG_ON(FlagsWaypoint, FLAG_NLPBW_DRAW_BUILD_PAIR_2);
			}
			else
			{
				FLAG_OFF(FlagsWaypoint, FLAG_NLPBW_DRAW_BUILD_PAIR_2);
			}
			updateBuildingPairSubdisplayCtrl(hwndDlg, FlagsWaypoint);
			updatePathBuildersFlags(hwndDlg, FlagsWaypoint, &Project);
			return TRUE;

		case IDC_CHECK_DRAW_BUILDING_PAIR_3:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DRAW_BUILDING_PAIR_3) == BST_CHECKED)
			{
				FLAG_ON(FlagsWaypoint, FLAG_NLPBW_DRAW_BUILD_PAIR_3);
			}
			else
			{
				FLAG_OFF(FlagsWaypoint, FLAG_NLPBW_DRAW_BUILD_PAIR_3);
			}
			updateBuildingPairSubdisplayCtrl(hwndDlg, FlagsWaypoint);
			updatePathBuildersFlags(hwndDlg, FlagsWaypoint, &Project);
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


		// Messages relatifs au PATH WORKBENCH:
		// |
		// -------------------------------------------------------------------------------------------------------------
		case WM_MESSAGE_SET_CURRENT_PATH_WORKBENCH:
			NErrorIf(wParam != APP_EXPLORER_WINDOW, NERROR_INCONSISTENT_PARAMETERS); // pas une erreur juste une verif...
			// WPARAM : APP_EXPLORER_WINDOW ( Id de l'explorer )
			// LPARAM : NEW  REGISTRED WORKBENCH
			pwb_new = (NLPATH_WORKBENCH*)lParam;
			//NErrorIf(pwb == pwb_new, NERROR_INCONSISTENT_PARAMETERS); // Ils devraient être différents !
			SetWindowLong(hwndDlg, GWL_USERDATA, (LONG)pwb_new);
			return TRUE;

		case WM_MESSAGE_DELETE_CURRENT_PATH_WORKBENCH:
			NErrorIf(wParam != APP_EXPLORER_WINDOW, NERROR_INCONSISTENT_PARAMETERS); // pas une erreur juste une verif...
			// WPARAM : APP_EXPLORER_WINDOW ( Id de l'explorer )
			// LPARAM : TO BE DELETE  REGISTRED WORKBENCH
			pwb_new = (NLPATH_WORKBENCH*)lParam; // ici pwb_new représente en fait le WORKBENCH qui va être supprimé !
			SetWindowLong(hwndDlg, GWL_USERDATA, 0);
			return TRUE;

		case WM_MESSAGE_UPDATE_CURRENT_PATH_WORKBENCH_PTR:
			NErrorIf(wParam != APP_EXPLORER_WINDOW, NERROR_INCONSISTENT_PARAMETERS); // pas une erreur juste une verif...
			// WPARAM : APP_EXPLORER_WINDOW ( Id de l'explorer )
			// LPARAM : NEW ADRESS OF THE SAME WORKBENCH
			pwb_new = (NLPATH_WORKBENCH*)lParam;
			SetWindowLong(hwndDlg, GWL_USERDATA, (LONG)pwb_new);
			return TRUE;


	default:
		break;
	}
	return FALSE;
}

#include <iostream>
#include <windows.h>
#include "../../resource.h"
#include "../N/NEditor.h"

#include "App_Init.h"

#include "../NL/NL2DOrthogonalCoordinateSystem_MotionProfileFlags.h"
#include "../NL/NL2DOrthogonalCoordinateSystemToolData.h"
#include "../NL/MotionControl/Trajectory/NLFollowerTank.h"

#include "WndProcDashboard.h"

extern NL2DOCS_TOOLDATA		OrthogonalCoordinateSystemToolData;
extern NL2DOCS				OrthogonalCoordinateSystem_A;
//extern NL2DOCS				OrthogonalCoordinateSystem_RobotDashboard;

HWND hwnd_Dashboard = NULL;

/*
BOOL CALLBACK EnumChild_disable_Proc(_In_ HWND   hwnd, _In_ LPARAM lParam)
{
	EnableWindow(hwnd, FALSE);
	return TRUE;
}
*/
void enableViewDashboardControls(const HWND hwndDlg, const Nbool benable)
{
}

void enableViewMotionProfileControls(const HWND hwndDlg, const Nbool benable)
{
	int ctrlid[] = {	IDC_RADIO_MP_FT,IDC_RADIO_MP_FS,IDC_CHECK_DISPLAY_MAIN_INFOS,IDC_CHECK_DISPLAY_SUB_INFOS ,
						IDC_CHECK_MP_JERK,
						IDC_CHECK_MP_ACC ,
						IDC_CHECK_MP_VELOCITY, 
						IDC_CHECK_MP_REVERSE_VELOCITY_CHECK,
						IDC_CHECK_DISPLAY_VSTAGE ,
						IDC_CHECK_DISPLAY_KEYPOINT_ID ,
						IDC_CHECK_DISPLAY_COORDS_LINE ,
						IDC_CHECK_DISPLAY_ABSCISSA,
						IDC_CHECK_DISPLAY_ORDINATE,
						IDC_RADIO_COORD_ORIENT_HORIZONTAL,
						IDC_RADIO_COORD_ORIENT_45,
						IDC_RADIO_COORD_ORIENT_135,
						IDC_RADIO_COORD_ORIENT_VERTICAL,
						IDC_RADIO_COORD_DIGIT_NONE,
						IDC_RADIO_COORD_DIGIT_2,
						IDC_RADIO_COORD_DIGIT_4,
						IDC_RADIO_COORD_DIGIT_7,
						-1 };

	Nu32 i = 0;
	while(ctrlid[i]!=-1)
	{
		EnableWindow(GetDlgItem(hwndDlg, ctrlid[i]), benable);
		i++;
	}
}

BOOL CALLBACK WndProc_Dashboard(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{


	switch (message)
	{
	case WM_INITDIALOG:
		CheckDlgButton(hwndDlg, IDC_RADIO_COORD_ORIENT_HORIZONTAL, BST_CHECKED);
		CheckDlgButton(hwndDlg, IDC_RADIO_COORD_DIGIT_2, BST_CHECKED);
		CheckDlgButton(hwndDlg, IDC_CHECK_DISPLAY_ABSCISSA, BST_CHECKED);
		CheckDlgButton(hwndDlg, IDC_CHECK_DISPLAY_ORDINATE, BST_CHECKED);
		CheckDlgButton(hwndDlg, IDC_CHECK_DISPLAY_COORDS_LINE, BST_CHECKED);
		CheckDlgButton(hwndDlg, IDC_RADIO_MP_FS, BST_CHECKED);

		CheckDlgButton(hwndDlg, IDC_CHECK_MP_VIEW, BST_UNCHECKED);
		CheckDlgButton(hwndDlg, IDC_CHECK_TRACE_VIEW, BST_UNCHECKED);
		// Tout disable sauf exeptions:
		enableViewMotionProfileControls(hwndDlg, NFALSE);
		enableViewDashboardControls(hwndDlg, NFALSE);

		//RedrawWindow(hwndDlg, NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_INVALIDATE);
		InvalidateRect(hwndDlg, NULL, FALSE);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			DestroyWindow(hwndDlg);
			hwnd_Dashboard = NULL;
			return TRUE;
		
		// Outils d'édition
		case IDC_LAYOUT_FREE_TRANSFORM:
			NSetCurrentTool(NTOOL_ID_2DOCS_FREE_TRANSFORM);
			return TRUE;

		// ------------------------------------------------------------------------------
		// VIEW MP ( on/off )
		//
		case IDC_CHECK_MP_VIEW:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_MP_VIEW) == BST_CHECKED)
			{
				enableViewMotionProfileControls(hwndDlg, NTRUE);

				if (IS_NL2DOCS_DASHBOARD_OFF(OrthogonalCoordinateSystem_A.m_userFlags))
					OrthogonalCoordinateSystemToolData.bindOcs(&OrthogonalCoordinateSystem_A);

				OrthogonalCoordinateSystem_A.setUserFlagOn(FLAG_NL2DOCS_DASHBOARD_MP);
			}
			else
			{
				enableViewMotionProfileControls(hwndDlg, NFALSE);

				OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_DASHBOARD_MP);

				if (IS_NL2DOCS_DASHBOARD_OFF(OrthogonalCoordinateSystem_A.m_userFlags))
					OrthogonalCoordinateSystemToolData.unbindOcs(&OrthogonalCoordinateSystem_A);
			}
			return TRUE;

		// ------------------------------------------------------------------------------
		// VIEW MP
		//	|____ Représentation en fonction de ... TEMPS ou Distance
		//
		case IDC_RADIO_MP_FT:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_MP_FT) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(FLAG_NL2DOCS_DASHBOARD_MP_FT);
			}
			return TRUE;
		case IDC_RADIO_MP_FS:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_MP_FS) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_DASHBOARD_MP_FT);
			}
			return TRUE;
			
		// ------------------------------------------------------------------------------
		// VIEW MP
		//	|____ Affichage Infos Main/Sub
		//
		case IDC_CHECK_DISPLAY_MAIN_INFOS:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISPLAY_MAIN_INFOS) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(FLAG_NL2DOCS_DASHBOARD_MP_INFO);
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_DASHBOARD_MP_INFO);
			}
			return TRUE;
			/*
		case IDC_CHECK_DISPLAY_SUB_INFOS:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISPLAY_SUB_INFOS) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(FLAG_NL2DOCS_DASHBOARD_MP_INFO);
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_DASHBOARD_MP_INFO);
			}
			return TRUE;
			*/
		// ------------------------------------------------------------------------------
		// VIEW MP
		//	|____ Affichage Key Point Id
		//
		case IDC_CHECK_DISPLAY_KEYPOINT_ID:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISPLAY_KEYPOINT_ID) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(FLAG_NL2DOCS_DASHBOARD_MP_KEYPOINT_ID);
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_DASHBOARD_MP_KEYPOINT_ID);
			}
			return TRUE;
		// ------------------------------------------------------------------------------
		// VIEW MP
		//	|____ Affichage courbes Vitesse / acceleration / Jerk
		//
		case IDC_CHECK_MP_REVERSE_VELOCITY_CHECK:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_MP_REVERSE_VELOCITY_CHECK) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(FLAG_NL2DOCS_DASHBOARD_MP_VEL_CHECK);
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_DASHBOARD_MP_VEL_CHECK);
			}
			return TRUE;
		case IDC_CHECK_MP_VELOCITY:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_MP_VELOCITY) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(FLAG_NL2DOCS_DASHBOARD_MP_VEL);
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_DASHBOARD_MP_VEL);
			}
			return TRUE;
		case IDC_CHECK_MP_ACC:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_MP_ACC) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(FLAG_NL2DOCS_DASHBOARD_MP_ACCEL);
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_DASHBOARD_MP_ACCEL);
			}
			return TRUE;
		case IDC_CHECK_MP_JERK:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_MP_JERK) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(FLAG_NL2DOCS_DASHBOARD_MP_JERK);
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_DASHBOARD_MP_JERK);
			}
			return TRUE;

		// ------------------------------------------------------------------------------
		// VIEW MP
		//	|____ Affichage Velocity Stage
		//
		case IDC_CHECK_DISPLAY_VSTAGE:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISPLAY_VSTAGE) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(FLAG_NL2DOCS_DASHBOARD_MP_VSTAGE);
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_DASHBOARD_MP_VSTAGE);
			}
			return TRUE;

		// ------------------------------------------------------------------------------
		// VIEW MP
		//	|____ Affichage coordonnées / Abscisses (valeurs)
		//
		case IDC_CHECK_DISPLAY_ABSCISSA:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISPLAY_ABSCISSA) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.showCoordsValue(NL2DOCS_COORDS_STYLE_0); // Style 0 pour l'abscisse
			}
			else
			{
				OrthogonalCoordinateSystem_A.hideCoordsValue(NL2DOCS_COORDS_STYLE_0);
			}
			return TRUE;

		// ------------------------------------------------------------------------------
		// VIEW MP
		//	|____ Affichage coordonnées / Ordonnées (valeurs)
		//
		case IDC_CHECK_DISPLAY_ORDINATE:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISPLAY_ORDINATE) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.showCoordsValue(NL2DOCS_COORDS_STYLE_1); // Style 1 pour l'ordonnée
			}
			else
			{
				OrthogonalCoordinateSystem_A.hideCoordsValue(NL2DOCS_COORDS_STYLE_1);
			}
			return TRUE;

		// ------------------------------------------------------------------------------
		// VIEW MP
		//	|____ Affichage coordonnées (Orientation)
		//
		case IDC_RADIO_COORD_ORIENT_HORIZONTAL:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_COORD_ORIENT_HORIZONTAL) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setCoordsOrientation(NL2DOCS_COORDS_STYLE_0,NL2DOCS_ORIENT_HORIZONTAL);
				OrthogonalCoordinateSystem_A.setCoordsOrientation(NL2DOCS_COORDS_STYLE_1, NL2DOCS_ORIENT_HORIZONTAL);
			}
			return TRUE;
		case IDC_RADIO_COORD_ORIENT_45:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_COORD_ORIENT_45) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setCoordsOrientation(NL2DOCS_COORDS_STYLE_0, NL2DOCS_ORIENT_45);
				OrthogonalCoordinateSystem_A.setCoordsOrientation(NL2DOCS_COORDS_STYLE_1, NL2DOCS_ORIENT_45);
			}
			return TRUE;
		case IDC_RADIO_COORD_ORIENT_135:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_COORD_ORIENT_135) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setCoordsOrientation(NL2DOCS_COORDS_STYLE_0, NL2DOCS_ORIENT_135);
				OrthogonalCoordinateSystem_A.setCoordsOrientation(NL2DOCS_COORDS_STYLE_1, NL2DOCS_ORIENT_135);
			}
			return TRUE;
		case IDC_RADIO_COORD_ORIENT_VERTICAL:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_COORD_ORIENT_VERTICAL) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setCoordsOrientation(NL2DOCS_COORDS_STYLE_0, NL2DOCS_ORIENT_VERTICAL);
				OrthogonalCoordinateSystem_A.setCoordsOrientation(NL2DOCS_COORDS_STYLE_1, NL2DOCS_ORIENT_VERTICAL);
			}
			return TRUE;

		// ------------------------------------------------------------------------------
		// VIEW MP
		//	|____ Affichage coordonnées (Digits)
		//
		case IDC_RADIO_COORD_DIGIT_NONE:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_COORD_DIGIT_NONE) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setCoordsDigits(NL2DOCS_COORDS_STYLE_0,0);
				OrthogonalCoordinateSystem_A.setCoordsDigits(NL2DOCS_COORDS_STYLE_1, 0);
			}
			return TRUE;
		case IDC_RADIO_COORD_DIGIT_2:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_COORD_DIGIT_2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setCoordsDigits(NL2DOCS_COORDS_STYLE_0, 2);
				OrthogonalCoordinateSystem_A.setCoordsDigits(NL2DOCS_COORDS_STYLE_1, 2);
			}
			return TRUE;
		case IDC_RADIO_COORD_DIGIT_4:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_COORD_DIGIT_4) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setCoordsDigits(NL2DOCS_COORDS_STYLE_0, 4);
				OrthogonalCoordinateSystem_A.setCoordsDigits(NL2DOCS_COORDS_STYLE_1, 4);
			}
			return TRUE;
		case IDC_RADIO_COORD_DIGIT_7:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_COORD_DIGIT_7) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setCoordsDigits(NL2DOCS_COORDS_STYLE_0, 7);
				OrthogonalCoordinateSystem_A.setCoordsDigits(NL2DOCS_COORDS_STYLE_1, 7);
			}
			return TRUE;
		// ------------------------------------------------------------------------------
		// VIEW MP
		//	|____ Affichage coordonnées (Lignes)
		//
		case IDC_CHECK_DISPLAY_COORDS_LINE:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISPLAY_COORDS_LINE) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.showCoordsLine(NL2DOCS_COORDS_STYLE_0); // Style 0 pour l'abscisse
				OrthogonalCoordinateSystem_A.showCoordsLine(NL2DOCS_COORDS_STYLE_1); // Style 1 pour l'ordonnée
				OrthogonalCoordinateSystem_A.showCoordsLine(NL2DOCS_COORDS_STYLE_2); // Style 2 pour les index en abscisse
			}
			else
			{
				OrthogonalCoordinateSystem_A.hideCoordsLine(NL2DOCS_COORDS_STYLE_0);
				OrthogonalCoordinateSystem_A.hideCoordsLine(NL2DOCS_COORDS_STYLE_1);
				OrthogonalCoordinateSystem_A.hideCoordsLine(NL2DOCS_COORDS_STYLE_2); 
			}
			return TRUE;

		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		// ------------------------------------------------------------------------------
		// VIEW TRACE (on/off)
		//
		case IDC_CHECK_TRACE_VIEW:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_TRACE_VIEW) == BST_CHECKED)
			{
				enableViewDashboardControls(hwndDlg, NTRUE);

				if(IS_NL2DOCS_DASHBOARD_OFF(OrthogonalCoordinateSystem_A.m_userFlags))
					OrthogonalCoordinateSystemToolData.bindOcs(&OrthogonalCoordinateSystem_A);

				OrthogonalCoordinateSystem_A.setUserFlagOn(FLAG_NL2DOCS_DASHBOARD_TRACE);
			}
			else
			{
				enableViewDashboardControls(hwndDlg, NFALSE);

				OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_DASHBOARD_TRACE);
				
				if (IS_NL2DOCS_DASHBOARD_OFF(OrthogonalCoordinateSystem_A.m_userFlags))
					OrthogonalCoordinateSystemToolData.unbindOcs(&OrthogonalCoordinateSystem_A);
			}
			return TRUE;

		// ------------------------------------------------------------------------------
		// VIEW TRACE
		//	|____ Affichage Velocity,Acceleration,Angular Velocity , angular Acceleration
		//
		case IDC_CHECK_TRACE_DT_VELOCITY:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_TRACE_DT_VELOCITY) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(1<<(NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_V));
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_V));
			}
			return TRUE;

		case IDC_CHECK_TRACE_DT_ACCELERATION:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_TRACE_DT_ACCELERATION) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_A));
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_A));
			}
			return TRUE;

		case IDC_CHECK_TRACE_DT_ANGULAR_VELOCITY:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_TRACE_DT_ANGULAR_VELOCITY) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_W));
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_W));
			}
			return TRUE;
			
		case IDC_CHECK_TRACE_DT_ANGULAR_ACCELERATION:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_TRACE_DT_ANGULAR_ACCELERATION) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_ALPHA));
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_ALPHA));
			}
			return TRUE;
		// ------------------------------------------------------------------------------
		// VIEW TRACE
		//	|____ Affichage Error x,y,angular
		//
		case IDC_CHECK_TRACE_ERR_X:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_TRACE_ERR_X) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_ERR_X));
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_ERR_X));
			}
			return TRUE;
		case IDC_CHECK_TRACE_ERR_Y:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_TRACE_ERR_Y) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_ERR_Y));
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING  + NLFTDD_PARAM_ID_ERR_Y));
			}
			return TRUE;
		case IDC_CHECK_TRACE_ERR_ANG:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_TRACE_ERR_ANG) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_ERR_ANG));
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_ERR_ANG));
			}
			return TRUE;
		// ------------------------------------------------------------------------------
		// VIEW TRACE
		//	|____ Affichage Encoder
		//
		case IDC_CHECK_TRACE_LEFT_WHEEL_DIST:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_TRACE_LEFT_WHEEL_DIST) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_ENCOD_LEFT));
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_ENCOD_LEFT));
			}
			return TRUE;
		case IDC_CHECK_TRACE_RIGHT_WHEEL_DIST:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_TRACE_RIGHT_WHEEL_DIST) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_ENCOD_RIGHT));
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_ENCOD_RIGHT));
			}
			return TRUE;
		case IDC_CHECK_TRACE_LEFT_WHEEL_DELTA_D:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_TRACE_LEFT_WHEEL_DELTA_D) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_ENCOD_LEFT_VAR));
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_ENCOD_LEFT_VAR));
			}
			return TRUE;
		case IDC_CHECK_TRACE_RIGHT_WHEEL_DELTA_D:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_TRACE_RIGHT_WHEEL_DELTA_D) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_ENCOD_RIGHT_VAR));
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_ENCOD_RIGHT_VAR));
			}
			return TRUE;
		// ------------------------------------------------------------------------------
		// VIEW TRACE
		//	|____ Affichage Output
		//
		case IDC_CHECK_TRACE_OUTPUT_LEFT_VELOCITY:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_TRACE_OUTPUT_LEFT_VELOCITY) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_OUTPUT_LEFT_V));
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_OUTPUT_LEFT_V));
			}
			return TRUE;
		case IDC_CHECK_TRACE_OUTPUT_LEFT_ACCELERATION:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_TRACE_OUTPUT_LEFT_ACCELERATION) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_OUTPUT_LEFT_A));
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_OUTPUT_LEFT_A));
			}
			return TRUE;
		case IDC_CHECK_TRACE_OUTPUT_RIGHT_VELOCITY:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_TRACE_OUTPUT_RIGHT_VELOCITY) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_OUTPUT_RIGHT_V));
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_OUTPUT_RIGHT_V));
			}
			return TRUE;
		case IDC_CHECK_TRACE_OUTPUT_RIGHT_ACCELERATION:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_TRACE_OUTPUT_RIGHT_ACCELERATION) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_OUTPUT_RIGHT_A));
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(1 << (NLFTDD_PARAM_BIT_ID_SHIFTING + NLFTDD_PARAM_ID_OUTPUT_RIGHT_A));
			}
			return TRUE;


		}

		return FALSE;
	}
	return FALSE;
}

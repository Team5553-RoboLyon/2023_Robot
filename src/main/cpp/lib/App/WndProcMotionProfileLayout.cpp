#include <iostream>
#include <windows.h>
#include "../../resource.h"
#include "../N/NEditor.h"

#include "App_Init.h"

#include "../NL/NL2DOrthogonalCoordinateSystem_MotionProfileFlags.h"
#include "../NL/NL2DOrthogonalCoordinateSystemToolData.h"

#include "WndProcMotionProfileLayout.h"

extern NL2DOCS_TOOLDATA		OrthogonalCoordinateSystemToolData;
extern NL2DOCS				OrthogonalCoordinateSystem_A;
extern NL2DOCS				OrthogonalCoordinateSystem_B;

HWND hwnd_MotionProfileLayout = NULL;

/*
BOOL CALLBACK EnumChild_disable_Proc(_In_ HWND   hwnd, _In_ LPARAM lParam)
{
	EnableWindow(hwnd, FALSE);
	return TRUE;
}
*/

void enableViewAControls(const HWND hwndDlg, const Nbool benable)
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
void enableViewBControls(const HWND hwndDlg, const Nbool benable)
{
	int ctrlid[] = { IDC_RADIO_MP_FT2,IDC_RADIO_MP_FS2,IDC_CHECK_DISPLAY_MAIN_INFOS2,IDC_CHECK_DISPLAY_SUB_INFOS2 ,
						IDC_CHECK_MP_JERK2,
						IDC_CHECK_MP_ACC2 ,
						IDC_CHECK_MP_VELOCITY2,
						IDC_CHECK_MP_REVERSE_VELOCITY_CHECK2,
						IDC_CHECK_DISPLAY_VSTAGE2 ,
						IDC_CHECK_DISPLAY_KEYPOINT_ID2 ,
						IDC_CHECK_DISPLAY_COORDS_LINE2 ,
						IDC_CHECK_DISPLAY_ABSCISSA2,
						IDC_CHECK_DISPLAY_ORDINATE2,
						IDC_RADIO_COORD_ORIENT_HORIZONTAL2,
						IDC_RADIO_COORD_ORIENT_45_2,
						IDC_RADIO_COORD_ORIENT_135_2,
						IDC_RADIO_COORD_ORIENT_VERTICAL2,
						IDC_RADIO_COORD_DIGIT_NONE2,
						IDC_RADIO_COORD_DIGIT_2_2,
						IDC_RADIO_COORD_DIGIT_4_2,
						IDC_RADIO_COORD_DIGIT_7_2,
						-1 };

	Nu32 i = 0;
	while (ctrlid[i] != -1)
	{
		EnableWindow(GetDlgItem(hwndDlg, ctrlid[i]), benable);
		i++;
	}
}
BOOL CALLBACK WndProc_MotionProfileLayout(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{


	switch (message)
	{
	case WM_INITDIALOG:
		CheckDlgButton(hwndDlg, IDC_RADIO_COORD_ORIENT_HORIZONTAL, BST_CHECKED);
		CheckDlgButton(hwndDlg, IDC_RADIO_COORD_ORIENT_HORIZONTAL2, BST_CHECKED);

		CheckDlgButton(hwndDlg, IDC_RADIO_COORD_DIGIT_2, BST_CHECKED);
		CheckDlgButton(hwndDlg, IDC_RADIO_COORD_DIGIT_2_2, BST_CHECKED);

		CheckDlgButton(hwndDlg, IDC_CHECK_DISPLAY_ABSCISSA, BST_CHECKED);
		CheckDlgButton(hwndDlg, IDC_CHECK_DISPLAY_ABSCISSA2, BST_CHECKED);

		CheckDlgButton(hwndDlg, IDC_CHECK_DISPLAY_ORDINATE, BST_CHECKED);
		CheckDlgButton(hwndDlg, IDC_CHECK_DISPLAY_ORDINATE2, BST_CHECKED);

		CheckDlgButton(hwndDlg, IDC_CHECK_DISPLAY_COORDS_LINE, BST_CHECKED);
		CheckDlgButton(hwndDlg, IDC_CHECK_DISPLAY_COORDS_LINE2, BST_CHECKED);

		CheckDlgButton(hwndDlg, IDC_RADIO_MP_FS, BST_CHECKED);
		CheckDlgButton(hwndDlg, IDC_RADIO_MP_FS2, BST_CHECKED);

		// Tout disable sauf exeptions:
		// EnumChildWindows(hwndDlg, EnumChild_disable_Proc,0);
		enableViewAControls(hwndDlg, NFALSE);
		enableViewBControls(hwndDlg, NFALSE);

		//RedrawWindow(hwndDlg, NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_INVALIDATE);
		InvalidateRect(hwndDlg, NULL, FALSE);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			DestroyWindow(hwndDlg);
			hwnd_MotionProfileLayout = NULL;
			return TRUE;
		
		// Outils d'édition
		case IDC_LAYOUT_FREE_TRANSFORM:
			NSetCurrentTool(NTOOL_ID_2DOCS_FREE_TRANSFORM);
			return TRUE;

		// ------------------------------------------------------------------------------
		// VIEW A ( on/off )
		// ------------------------------------------------------------------------------
		case IDC_CHECK_MP_VIEW_A:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_MP_VIEW_A) == BST_CHECKED)
			{
				OrthogonalCoordinateSystemToolData.bindOcs(&OrthogonalCoordinateSystem_A);
				enableViewAControls(hwndDlg, NTRUE);
			}
			else
			{
				OrthogonalCoordinateSystemToolData.unbindOcs(&OrthogonalCoordinateSystem_A);
				enableViewAControls(hwndDlg, NFALSE);
			}
			return TRUE;

		// ------------------------------------------------------------------------------
		// VIEW B ( on/off )
		// ------------------------------------------------------------------------------
		case IDC_CHECK_MP_VIEW_B:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_MP_VIEW_B) == BST_CHECKED)
			{
				OrthogonalCoordinateSystemToolData.bindOcs(&OrthogonalCoordinateSystem_B);
				enableViewBControls(hwndDlg, NTRUE);
			}
			else
			{
				OrthogonalCoordinateSystemToolData.unbindOcs(&OrthogonalCoordinateSystem_B);
				enableViewBControls(hwndDlg, NFALSE);
			}
			return TRUE;

		// ------------------------------------------------------------------------------
		// VIEW A / B
		//	|
		//	|____ Représentation en fonction du TEMPS
		//
		// ------------------------------------------------------------------------------
		case IDC_RADIO_MP_FT:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_MP_FT) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_FT);
			}
			return TRUE;
		case IDC_RADIO_MP_FT2:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_MP_FT2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_B.setUserFlagOn(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_FT);
			}
			return TRUE;

		// ------------------------------------------------------------------------------
		// VIEW A / B
		//	|
		//	|____ Représentation en fonction de la distance parcourue
		//
		// ------------------------------------------------------------------------------
		case IDC_RADIO_MP_FS:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_MP_FS) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_FT);
			}
			return TRUE;
		case IDC_RADIO_MP_FS2:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_MP_FS2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_B.setUserFlagOff(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_FT);
			}
			return TRUE;


			
		// ------------------------------------------------------------------------------
		// VIEW A / B
		//	|
		//	|____ Affichage Main Infos
		//
		// ------------------------------------------------------------------------------
		case IDC_CHECK_DISPLAY_MAIN_INFOS:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISPLAY_MAIN_INFOS) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_INFO_MAIN);
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_INFO_MAIN);
			}
			return TRUE;
		case IDC_CHECK_DISPLAY_MAIN_INFOS2:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISPLAY_MAIN_INFOS2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_B.setUserFlagOn(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_INFO_MAIN);
			}
			else
			{
				OrthogonalCoordinateSystem_B.setUserFlagOff(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_INFO_MAIN);
			}
			return TRUE;
		// ------------------------------------------------------------------------------
		// VIEW A / B
		//	|
		//	|____ Affichage Sub Infos
		//
		// ------------------------------------------------------------------------------
		case IDC_CHECK_DISPLAY_SUB_INFOS:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISPLAY_SUB_INFOS) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_INFO_SUB);
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_INFO_SUB);
			}
			return TRUE;
		case IDC_CHECK_DISPLAY_SUB_INFOS2:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISPLAY_SUB_INFOS2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_B.setUserFlagOn(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_INFO_SUB);
			}
			else
			{
				OrthogonalCoordinateSystem_B.setUserFlagOff(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_INFO_SUB);
			}
			return TRUE;
		// ------------------------------------------------------------------------------
		// VIEW A / B
		//	|
		//	|____ Affichage Key Point Id
		//
		// ------------------------------------------------------------------------------
		case IDC_CHECK_DISPLAY_KEYPOINT_ID:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISPLAY_KEYPOINT_ID) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_KEYPOINT_ID);
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_KEYPOINT_ID);
			}
			return TRUE;
		case IDC_CHECK_DISPLAY_KEYPOINT_ID2:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISPLAY_KEYPOINT_ID2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_B.setUserFlagOn(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_KEYPOINT_ID);
			}
			else
			{
				OrthogonalCoordinateSystem_B.setUserFlagOff(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_KEYPOINT_ID);
			}
			return TRUE;

		// ------------------------------------------------------------------------------
		// VIEW A / B
		//	|
		//	|____ Affichage courbe de la vitesse
		//
		// ------------------------------------------------------------------------------
		case IDC_CHECK_MP_REVERSE_VELOCITY_CHECK:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_MP_REVERSE_VELOCITY_CHECK) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_VEL_CHECK);
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_VEL_CHECK);
			}
			return TRUE;
		case IDC_CHECK_MP_REVERSE_VELOCITY_CHECK2:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_MP_REVERSE_VELOCITY_CHECK2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_B.setUserFlagOn(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_VEL_CHECK);
			}
			else
			{
				OrthogonalCoordinateSystem_B.setUserFlagOff(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_VEL_CHECK);
			}
			return TRUE;
		// ------------------------------------------------------------------------------
		// VIEW A / B
		//	|
		//	|____ Affichage courbe de la vitesse
		//
		// ------------------------------------------------------------------------------
		case IDC_CHECK_MP_VELOCITY:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_MP_VELOCITY) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_VEL);
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_VEL);
			}
			return TRUE;
		case IDC_CHECK_MP_VELOCITY2:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_MP_VELOCITY2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_B.setUserFlagOn(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_VEL);
			}
			else
			{
				OrthogonalCoordinateSystem_B.setUserFlagOff(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_VEL);
			}
			return TRUE;

		// ------------------------------------------------------------------------------
		// VIEW A / B
		//	|
		//	|____ Affichage courbe de l'acceleration
		//
		// ------------------------------------------------------------------------------
		case IDC_CHECK_MP_ACC:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_MP_ACC) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_ACCEL);
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_ACCEL);
			}
			return TRUE;
		case IDC_CHECK_MP_ACC2:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_MP_ACC2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_B.setUserFlagOn(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_ACCEL);
			}
			else
			{
				OrthogonalCoordinateSystem_B.setUserFlagOff(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_ACCEL);
			}
			return TRUE;

		// ------------------------------------------------------------------------------
		// VIEW A / B
		//	|
		//	|____ Affichage courbe du Jerk
		//
		// ------------------------------------------------------------------------------
		case IDC_CHECK_MP_JERK:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_MP_JERK) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_JERK);
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_JERK);
			}
			return TRUE;
		case IDC_CHECK_MP_JERK2:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_MP_JERK2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_B.setUserFlagOn(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_JERK);
			}
			else
			{
				OrthogonalCoordinateSystem_B.setUserFlagOff(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_JERK);
			}
			return TRUE;

		// ------------------------------------------------------------------------------
		// VIEW A / B
		//	|
		//	|____ Affichage Velocity Stage
		//
		// ------------------------------------------------------------------------------
		case IDC_CHECK_DISPLAY_VSTAGE:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISPLAY_VSTAGE) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_A.setUserFlagOn(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_VSTAGE);
			}
			else
			{
				OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_VSTAGE);
			}
			return TRUE;
		case IDC_CHECK_DISPLAY_VSTAGE2:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISPLAY_VSTAGE2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_B.setUserFlagOn(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_VSTAGE);
			}
			else
			{
				OrthogonalCoordinateSystem_B.setUserFlagOff(FLAG_NL2DOCS_MOTIONPROFILE_LAYER_VIEW_VSTAGE);
			}
			return TRUE;

		// ------------------------------------------------------------------------------
		// VIEW A / B
		//	|
		//	|____ Affichage coordonnées / Abscisses (valeurs)
		//
		// ------------------------------------------------------------------------------
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
		case IDC_CHECK_DISPLAY_ABSCISSA2:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISPLAY_ABSCISSA2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_B.showCoordsValue(NL2DOCS_COORDS_STYLE_0);
			}
			else
			{
				OrthogonalCoordinateSystem_B.hideCoordsValue(NL2DOCS_COORDS_STYLE_0);
			}
			return TRUE;

		// ------------------------------------------------------------------------------
		// VIEW A / B
		//	|
		//	|____ Affichage coordonnées / Ordonnées (valeurs)
		//
		// ------------------------------------------------------------------------------
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
		case IDC_CHECK_DISPLAY_ORDINATE2:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISPLAY_ORDINATE2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_B.showCoordsValue(NL2DOCS_COORDS_STYLE_1);
			}
			else
			{
				OrthogonalCoordinateSystem_B.hideCoordsValue(NL2DOCS_COORDS_STYLE_1);
			}
			return TRUE;
		// ------------------------------------------------------------------------------
		// VIEW A / B
		//	|
		//	|____ Affichage coordonnées (Orientation)
		//
		// ------------------------------------------------------------------------------
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

		case IDC_RADIO_COORD_ORIENT_HORIZONTAL2:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_COORD_ORIENT_HORIZONTAL2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_B.setCoordsOrientation(NL2DOCS_COORDS_STYLE_0, NL2DOCS_ORIENT_HORIZONTAL);
				OrthogonalCoordinateSystem_B.setCoordsOrientation(NL2DOCS_COORDS_STYLE_1, NL2DOCS_ORIENT_HORIZONTAL);
			}
			return TRUE;
		case IDC_RADIO_COORD_ORIENT_45_2:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_COORD_ORIENT_45_2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_B.setCoordsOrientation(NL2DOCS_COORDS_STYLE_0, NL2DOCS_ORIENT_45);
				OrthogonalCoordinateSystem_B.setCoordsOrientation(NL2DOCS_COORDS_STYLE_1, NL2DOCS_ORIENT_45);
			}
			return TRUE;
		case IDC_RADIO_COORD_ORIENT_135_2:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_COORD_ORIENT_135_2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_B.setCoordsOrientation(NL2DOCS_COORDS_STYLE_0, NL2DOCS_ORIENT_135);
				OrthogonalCoordinateSystem_B.setCoordsOrientation(NL2DOCS_COORDS_STYLE_1, NL2DOCS_ORIENT_135);
			}
			return TRUE;
		case IDC_RADIO_COORD_ORIENT_VERTICAL2:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_COORD_ORIENT_VERTICAL2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_B.setCoordsOrientation(NL2DOCS_COORDS_STYLE_0, NL2DOCS_ORIENT_VERTICAL);
				OrthogonalCoordinateSystem_B.setCoordsOrientation(NL2DOCS_COORDS_STYLE_1, NL2DOCS_ORIENT_VERTICAL);
			}
			return TRUE;

		// ------------------------------------------------------------------------------
		// VIEW A / B
		//	|
		//	|____ Affichage coordonnées (Digits)
		//
		// ------------------------------------------------------------------------------
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

		case IDC_RADIO_COORD_DIGIT_NONE2:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_COORD_DIGIT_NONE2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_B.setCoordsDigits(NL2DOCS_COORDS_STYLE_0, 0);
				OrthogonalCoordinateSystem_B.setCoordsDigits(NL2DOCS_COORDS_STYLE_1, 0);
			}
			return TRUE;
		case IDC_RADIO_COORD_DIGIT_2_2:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_COORD_DIGIT_2_2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_B.setCoordsDigits(NL2DOCS_COORDS_STYLE_0, 2);
				OrthogonalCoordinateSystem_B.setCoordsDigits(NL2DOCS_COORDS_STYLE_1, 2);
			}
			return TRUE;
		case IDC_RADIO_COORD_DIGIT_4_2:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_COORD_DIGIT_4_2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_B.setCoordsDigits(NL2DOCS_COORDS_STYLE_0, 4);
				OrthogonalCoordinateSystem_B.setCoordsDigits(NL2DOCS_COORDS_STYLE_1, 4);
			}
			return TRUE;
		case IDC_RADIO_COORD_DIGIT_7_2:
			if (IsDlgButtonChecked(hwndDlg, IDC_RADIO_COORD_DIGIT_7_2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_B.setCoordsDigits(NL2DOCS_COORDS_STYLE_0, 7);
				OrthogonalCoordinateSystem_B.setCoordsDigits(NL2DOCS_COORDS_STYLE_1, 7);
			}
			return TRUE;
		// ------------------------------------------------------------------------------
		// VIEW A / B
		//	|
		//	|____ Affichage coordonnées (Lignes)
		//
		// ------------------------------------------------------------------------------
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
		case IDC_CHECK_DISPLAY_COORDS_LINE2:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISPLAY_COORDS_LINE2) == BST_CHECKED)
			{
				OrthogonalCoordinateSystem_B.showCoordsLine(NL2DOCS_COORDS_STYLE_0);
				OrthogonalCoordinateSystem_B.showCoordsLine(NL2DOCS_COORDS_STYLE_1);
				OrthogonalCoordinateSystem_B.showCoordsLine(NL2DOCS_COORDS_STYLE_2); // Style 2 pour les index en abscisse
			}
			else
			{
				OrthogonalCoordinateSystem_B.hideCoordsLine(NL2DOCS_COORDS_STYLE_0);
				OrthogonalCoordinateSystem_B.hideCoordsLine(NL2DOCS_COORDS_STYLE_1);
				OrthogonalCoordinateSystem_B.hideCoordsLine(NL2DOCS_COORDS_STYLE_2);
			}
			return TRUE;

		}

		return FALSE;
	}
	return FALSE;
}

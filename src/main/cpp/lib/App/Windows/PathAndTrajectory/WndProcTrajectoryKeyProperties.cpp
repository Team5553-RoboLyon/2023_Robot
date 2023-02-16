#include "../../../../resource.h"
#include "../../App_Init.h"
#include "../../../NL/MotionControl/NLPathWorkbench.h"
#include "../../../NL/MotionControl/Trajectory/NLTrajectoryKey.h"
#include "../../Windows/WindowUtilities.h"

#include "WndProcTrajectoryKeyAction.h"
#include "WndProcTrajectoryKeyProperties.h"

#define TXTOUT_SIZE		64



extern HWND							AppWindowsTable[APP_WINDOW_ENUM::APP_WIN_ENUM_SIZE];
extern HINSTANCE					hInstance;
extern HFONT						hFontTahoma;

void updateKeyTextinfos(HWND hwnd, NLTRJKEY* pkey, const Nu32 keyidx)
{
	if (pkey)
	{
		char txt[64];
		sprintf(txt, "%.2f Sec", pkey->m_timeStamp.start);
		SetWindowText(GetDlgItem(hwnd, IDC_STATIC_KEY_TIMESTAMP_START), txt);

		sprintf(txt, "Restart: %.2f Sec", pkey->m_timeStamp.end);
		SetWindowText(GetDlgItem(hwnd, IDC_STATIC_KEY_TIMESTAMP_END), txt);

		sprintf(txt, "%.2f", pkey->getDuration());
		SetWindowText(GetDlgItem(hwnd, IDC_EDIT_KEY_DURATION), txt);

		sprintf(txt, "Key Index: %d  S[ %.2f ]", keyidx, pkey->m_s);
		SetWindowText(GetDlgItem(hwnd, IDC_STATIC_KEY_PROPERTIES), txt);
	}
	else
	{
		SetWindowText(GetDlgItem(hwnd, IDC_STATIC_KEY_TIMESTAMP_START), "- Sec");
		SetWindowText(GetDlgItem(hwnd, IDC_STATIC_KEY_TIMESTAMP_END), "Restart: - Sec");
		SetWindowText(GetDlgItem(hwnd, IDC_EDIT_KEY_DURATION), "");
		SetWindowText(GetDlgItem(hwnd, IDC_STATIC_KEY_PROPERTIES), "Key Index: None  S[ - ]");
	}
}

void setKeyRestartCtrlsPos(HWND hwnd, const Ns32 y )
{
	SetWindowPos(GetDlgItem(hwnd, IDC_STATIC_PLUS), NULL,				NL_ACTIONWINDOW_X0 + NL_ACTIONWINDOW_LEFT_MARGIN,		y + 7, 0 , 0 , SWP_NOSIZE | SWP_NOZORDER);
	SetWindowPos(GetDlgItem(hwnd, IDC_EDIT_KEY_DURATION), NULL,			NL_ACTIONWINDOW_X0 + NL_ACTIONWINDOW_LEFT_MARGIN + 16,	y + 5, 48, 20, SWP_NOZORDER);
	SetWindowPos(GetDlgItem(hwnd, STATIC_KEY_TIMESHIFT_UNIT), NULL,		NL_ACTIONWINDOW_X0 + NL_ACTIONWINDOW_LEFT_MARGIN + 66,  y + 7, 48, 20, SWP_NOZORDER);
	
	SetWindowPos(GetDlgItem(hwnd, IDC_CHECK_AUTO_KEY_DURATION), NULL,	NL_ACTIONWINDOW_X0 + NL_ACTIONWINDOW_LEFT_MARGIN + 170,	y + 5, 0 , 0 , SWP_NOSIZE | SWP_NOZORDER);
	SetWindowPos(GetDlgItem(hwnd, IDC_STATIC_KEY_TIMESTAMP_END), NULL,	NL_ACTIONWINDOW_X0 + NL_ACTIONWINDOW_WIDTH - 190,		y    , 0 , 0 , SWP_NOSIZE | SWP_NOZORDER);
	
}


BOOL CALLBACK WndProc_TrajectoryKeyProperties(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	NLPATH_WORKBENCH	*pwb		= (NLPATH_WORKBENCH*)GetWindowLong(hwndDlg, GWL_USERDATA);
	NLPATH_WORKBENCH	*pwb_new	= nullptr;
	Nu32	y = 0;
	Nf32	f = 0.0f;
	Nf32	dt = 0.0f;
	//HWND	hwnd_actionstable = GetDlgItem(hwndDlg, IDC_ACTIONS_TABLE);
	NLTRJKEY* pkey;
	HWND hwnd;

	// TODO: à faire mieux que ça mais bon pour pour l'instant on sait qu'il n'y a qu'une seule fenetre key properties en même temps donc une static ira bien
	// |
	// |
	static HWND static_selectedActionHwnd	= NULL;
	static Nu32 static_selectedKeyIndex		= NVOID;
	static Nu32 static_flags				= 0;
	// |
	// +------------------------------------------------------------------------------------------------------------------------------------------------------
	
	Nu32 selected_action_idx = NVOID;

	switch (message)
	{
	case WM_INITDIALOG:
		hwnd = GetDlgItem(hwndDlg, IDC_STATIC_KEY_TIMESTAMP_START);
		SendMessage(hwnd, WM_SETFONT, (WPARAM)hFontTahoma, true);

		hwnd = GetDlgItem(hwndDlg, IDC_STATIC_KEY_TIMESTAMP_END);
		SendMessage(hwnd, WM_SETFONT, (WPARAM)hFontTahoma, true);

		hwnd = GetDlgItem(hwndDlg, IDC_STATIC_KEY_PROPERTIES);
		SendMessage(hwnd, WM_SETFONT, (WPARAM)hFontTahoma, true);

		hwnd = GetDlgItem(hwndDlg, IDC_EDIT_KEY_DURATION);
		SetWindowSubclass(hwnd, subEditProcRp, 0, 0);

		//updateKeyTextinfos(hwndDlg, NULL, static_selectedKeyIndex);
		//setEndTimeCtrlsPos(hwndDlg, 50);


		return TRUE;

/*	case WM_PAINT:
		break;*/



	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			SendMessage(GetParent(hwndDlg), WM_MESSAGE_APP_WINDOW_CLOSED, (WPARAM)APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_KEY_PROPERTIES, (LPARAM)hwndDlg);
			//SetWindowLong(hwndDlg, GWL_USERDATA, (LONG)NULL); ... on ne le fait pas car on destroy la BDD juste après
			DestroyWindow(hwndDlg);
			return TRUE;

		case IDC_CHECK_AUTO_KEY_DURATION:
			if (pwb && static_selectedKeyIndex != NVOID)
			{
				pkey = (NLTRJKEY*)NGetArrayPtr(pwb->getTrajectory()->getKeysArray(), static_selectedKeyIndex);

				if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_AUTO_KEY_DURATION))
				{
					FLAG_ON(pkey->m_flags,FLAG_NLTRJKEY_AUTO_STOP_TIMESPAN);
					EnableWindow(GetDlgItem(hwndDlg, IDC_EDIT_KEY_DURATION), FALSE);
					EnumChildWindows(hwndDlg, enableActionAutoCheckBox_Callback, TRUE);
				}
				else
				{
					FLAG_OFF(pkey->m_flags, FLAG_NLTRJKEY_AUTO_STOP_TIMESPAN);
					EnableWindow(GetDlgItem(hwndDlg, IDC_EDIT_KEY_DURATION), TRUE);
					EnumChildWindows(hwndDlg, enableActionAutoCheckBox_Callback, FALSE);
				}

				SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_UPTODATE_INFOS), "Obsolet Displayed Infos Trajectory Build Requested");
				pwb->requestTrajectoryBuild();
				UpdateWindow(hwndDlg);
			}
			return TRUE;

		case IDC_EDIT_KEY_DURATION:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS://EN_UPDATE:
				if (pwb && static_selectedKeyIndex != NVOID)
				{
					pkey = (NLTRJKEY*)NGetArrayPtr(pwb->getTrajectory()->getKeysArray(), static_selectedKeyIndex);
					f = GetNf32FromEditControl(GetDlgItem(hwndDlg, IDC_EDIT_KEY_DURATION), 0.0f, 3600.0f, NULL);
					if (f != pkey->getDuration())
					{
						pkey->setDuration(FALSE, f);
						updateKeyTextinfos(hwndDlg, pkey, static_selectedKeyIndex);

						updateActionsTable_afterKeyDurationUpdate(hwndDlg, pwb, static_selectedKeyIndex);

						SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_UPTODATE_INFOS), "Obsolet Displayed Infos Trajectory Build Requested");
						pwb->requestTrajectoryBuild();
						UpdateWindow(hwndDlg);
					}
				}
				break;

			default:
				break;
			}
			return TRUE;

		case IDC_BUTTON_ADD_ACTION:
			// On insert une action uniquement si il y a une key sélectionnée
			if (pwb && static_selectedKeyIndex != NVOID)
			{
				pkey = (NLTRJKEY*)NGetArrayPtr(pwb->getTrajectory()->getKeysArray(), static_selectedKeyIndex);
				setTrjKeyPropertiesSize(hwndDlg, pkey->m_actionsArray.Size + 1); // +1 car on anticipe la création de l'action.

				selected_action_idx = getHwndActionIndex(static_selectedActionHwnd);
				if (selected_action_idx == NVOID)
				{
					pkey->insertAction();
					updateActionsTable_afterActionInsertion(hwndDlg, pwb, static_selectedKeyIndex, 0);
					hwnd = insertActionHwnd(hwndDlg, pwb, static_selectedKeyIndex, 0);
				}
				else
				{
					pkey->insertAction((NLTRJACTION*)NGetArrayPtr(&pkey->m_actionsArray, selected_action_idx));
					updateActionsTable_afterActionInsertion(hwndDlg, pwb, static_selectedKeyIndex, selected_action_idx);
					hwnd = insertActionHwnd(hwndDlg, pwb, static_selectedKeyIndex, selected_action_idx);
				}
				SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_UPTODATE_INFOS), "Obsolet Displayed Infos Trajectory Build Requested");
				pwb->requestTrajectoryBuild();
				UpdateWindow(hwndDlg);
			}
			return TRUE;

		case IDC_BUTTON_DEL_ACTION:
			// On delete une action uniquement si il y a  une action et bien sûr une key sélectionnée 
			selected_action_idx = getHwndActionIndex(static_selectedActionHwnd);
			if(selected_action_idx != NVOID)
			{
				NErrorIf((!pwb) || (static_selectedKeyIndex == NVOID), NERROR_INCONSISTENT_PARAMETERS);
				pkey = (NLTRJKEY*)NGetArrayPtr(pwb->getTrajectory()->getKeysArray(), static_selectedKeyIndex);
				NErrorIf(!pkey->m_actionsArray.Size, NERROR_NULL_SIZE);
				pkey->eraseAction(selected_action_idx);
				setTrjKeyPropertiesSize(hwndDlg, pkey->m_actionsArray.Size);
				DestroyActionHwnd(static_selectedActionHwnd);
				updateActionsTable_afterActionErase(hwndDlg,pwb,static_selectedKeyIndex, selected_action_idx);

				SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_UPTODATE_INFOS), "Obsolet Displayed Infos Trajectory Build Requested");
				pwb->requestTrajectoryBuild();
				UpdateWindow(hwndDlg);
				static_selectedActionHwnd	= NULL;
			}
			return TRUE;

		case IDC_BUTTON_MOVE_ACTION_UP:
			// On insert une action uniquement si il y a une key sélectionnée
			if (pwb && static_selectedKeyIndex != NVOID)
			{
				pkey = (NLTRJKEY*)NGetArrayPtr(pwb->getTrajectory()->getKeysArray(), static_selectedKeyIndex);
				selected_action_idx = getHwndActionIndex(static_selectedActionHwnd);
				if (pkey->moveActionUp(selected_action_idx) == selected_action_idx - 1)
					updateActionsTable_afterActionIndexShifting(hwndDlg, pwb, static_selectedKeyIndex, selected_action_idx, selected_action_idx-1);

				SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_UPTODATE_INFOS), "Obsolet Displayed Infos Trajectory Build Requested");
				pwb->requestTrajectoryBuild();
				UpdateWindow(hwndDlg);
			}
			return TRUE;

		case IDC_BUTTON_MOVE_ACTION_DOWN:
			// On insert une action uniquement si il y a une key sélectionnée
			if (pwb && static_selectedKeyIndex != NVOID)
			{
				pkey = (NLTRJKEY*)NGetArrayPtr(pwb->getTrajectory()->getKeysArray(), static_selectedKeyIndex);
				selected_action_idx = getHwndActionIndex(static_selectedActionHwnd);
				if (pkey->moveActionDown(selected_action_idx) == selected_action_idx + 1)
					updateActionsTable_afterActionIndexShifting(hwndDlg, pwb, static_selectedKeyIndex, selected_action_idx, selected_action_idx + 1);

				SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_UPTODATE_INFOS), "Obsolet Displayed Infos Trajectory Build Requested");
				pwb->requestTrajectoryBuild();
				UpdateWindow(hwndDlg);
			}
			return TRUE;

		}
		break; // case WM_COMMAND:


	// Messages Privés locaux:
	case WM_MESSAGE_SET_CURRENT_ACTION_WINDOW:
		if ((HWND)wParam != static_selectedActionHwnd)
		{
			hwnd = static_selectedActionHwnd;
			static_selectedActionHwnd	= (HWND)wParam;
			InvalidateRect(hwnd, NULL, NTRUE);
			UpdateWindow(hwndDlg);
		}
		return TRUE;

	case WM_MESSAGE_GET_CURRENT_ACTION_WINDOW:
		*((HWND*)wParam) = static_selectedActionHwnd;
		return TRUE;

	// Messages relatifs au PATH WORKBENCH:
	// |
	// -------------------------------------------------------------------------------------------------------------
	case WM_MESSAGE_SET_CURRENT_PATH_WORKBENCH:
		NErrorIf(wParam != APP_EXPLORER_WINDOW, NERROR_INCONSISTENT_PARAMETERS); // pas une erreur juste une verif...
		// WPARAM : APP_EXPLORER_WINDOW ( Id de l'explorer )
		// LPARAM : NEW  REGISTRED WORKBENCH
		pwb_new = (NLPATH_WORKBENCH*)lParam;
		NErrorIf(pwb == pwb_new, NERROR_INCONSISTENT_PARAMETERS); // Ils devraient être différents !
		SetWindowLong(hwndDlg, GWL_USERDATA, (LONG)pwb_new);

		EnumChildWindows(hwndDlg, EnableCtrl_Callback, 0);
		return TRUE;

	case WM_MESSAGE_DELETE_CURRENT_PATH_WORKBENCH:
		NErrorIf(wParam != APP_EXPLORER_WINDOW, NERROR_INCONSISTENT_PARAMETERS); // pas une erreur juste une verif...
		// WPARAM : APP_EXPLORER_WINDOW ( Id de l'explorer )
		// LPARAM : TO BE DELETE  REGISTRED WORKBENCH
		pwb_new = (NLPATH_WORKBENCH*)lParam; // ici pwb_new représente en fait le WORKBENCH qui va être supprimé !
		SetWindowLong(hwndDlg, GWL_USERDATA, 0);
		EnumChildWindows(hwndDlg, DisableCtrl_Callback, 0);
		return TRUE;

	case WM_MESSAGE_UPDATE_CURRENT_PATH_WORKBENCH_PTR:
		NErrorIf(wParam != APP_EXPLORER_WINDOW, NERROR_INCONSISTENT_PARAMETERS); // pas une erreur juste une verif...
		// WPARAM : APP_EXPLORER_WINDOW ( Id de l'explorer )
		pwb_new = (NLPATH_WORKBENCH*)lParam;

		SetWindowLong(hwndDlg, GWL_USERDATA, (LONG)pwb_new);
		return TRUE;

	case WM_MESSAGE_SET_CURRENT_SELECTED_TRJKEY:
		if (lParam != static_selectedKeyIndex)
		{
			static_selectedKeyIndex		= lParam;
			static_selectedActionHwnd	= NULL;
			
			if (static_selectedKeyIndex != NVOID)
			{
				pkey = (NLTRJKEY*)NGetArrayPtr(pwb->getTrajectory()->getKeysArray(), static_selectedKeyIndex);

				if (IS_NLTRJKEY_AUTODURATION(pkey->m_flags))
				{
					CheckDlgButton(hwndDlg, IDC_CHECK_AUTO_KEY_DURATION, BST_CHECKED);
					EnableWindow(GetDlgItem(hwndDlg, IDC_EDIT_KEY_DURATION), FALSE);
				}
				else
				{
					CheckDlgButton(hwndDlg, IDC_CHECK_AUTO_KEY_DURATION, BST_UNCHECKED);
					EnableWindow(GetDlgItem(hwndDlg, IDC_EDIT_KEY_DURATION), TRUE);
				}

				setTrjKeyPropertiesSize(hwndDlg, pkey->m_actionsArray.Size);
				updateKeyTextinfos(hwndDlg, pkey, static_selectedKeyIndex);
				updateActionsTable_afterKeyIndexUpdate(hwndDlg, pwb, static_selectedKeyIndex);
			}
			else
			{
				
				setTrjKeyPropertiesSize(hwndDlg, 0);
				updateKeyTextinfos(hwndDlg, NULL, static_selectedKeyIndex);
				EnumChildWindows(hwndDlg, DestroyWindowLParam_Callback, (LPARAM)"ActionWinClass");
			}
			UpdateWindow(hwndDlg);
			//InvalidateRect(hwndDlg, NULL, 0);

		}
		return TRUE;

	case WM_MESSAGE_UPDATE_CURRENT_SELECTED_TRJKEY_DATA:
		if (lParam == static_selectedKeyIndex)
		{
			if (static_selectedKeyIndex != NVOID)
			{
				pkey = (NLTRJKEY*)NGetArrayPtr(pwb->getTrajectory()->getKeysArray(), static_selectedKeyIndex);
				updateKeyTextinfos(hwndDlg, pkey, static_selectedKeyIndex);
			}
			UpdateWindow(hwndDlg);
			//InvalidateRect(hwndDlg, NULL, 0);
		}
		return TRUE;

	case WM_MESSAGE_TRAJECTORY_REBUILDED:
		//InvalidateRect(hwndDlg, NULL, 0);
		//
		//RedrawWindow(hwndDlg, NULL, NULL, RDW_ALLCHILDREN);
		if (pwb && static_selectedKeyIndex != NVOID)
		{
			pkey = (NLTRJKEY*)NGetArrayPtr(pwb->getTrajectory()->getKeysArray(), static_selectedKeyIndex);
			updateKeyTextinfos(hwndDlg, pkey, static_selectedKeyIndex);
			EnumChildWindows(hwndDlg, redrawActionWindows_Callback, (LPARAM)RDW_ALLCHILDREN);
			updateKeyTextinfos(hwndDlg, pkey, static_selectedKeyIndex);
			SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_UPTODATE_INFOS), "Rebuilded Trajectory - Updated Display");
			UpdateWindow(hwndDlg);
		}
		return TRUE;

	default:
		break;
	}
	return FALSE;
}

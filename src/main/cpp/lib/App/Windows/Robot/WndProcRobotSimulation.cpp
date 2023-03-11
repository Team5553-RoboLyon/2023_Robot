#include <iostream>
#include <windows.h>
#include <CommCtrl.h>

#include "../../../../resource.h"
#include "../../../N/NEditor.h"
#include "../../../NL/NLProject.h"
#include "../../../NL/NL2DOrthogonalCoordinateSystem_MotionProfileFlags.h"
#include "../../../NL/NL2DOrthogonalCoordinateSystemToolData.h"
#include "../../../NL/MotionControl/DriveTrain/NLDriveTrainSpecs.h"
#include "../../../NL/MotionControl/NLPhysics.h"
#include "../../../NL/Simulation/VirtualRobot/NLVirtualRobot.h"
//#include "../../../NL/Simulation/NLRobot.h"

#include "../../App_Init.h"
#include "../WindowUtilities.h"
#include "WndProcRobotSimulation.h"

extern NLVIRTUAL_ROBOT				VirtualRobot;
extern NLPROJECT					Project;
#define TXTOUT_SIZE 32

BOOL CALLBACK WndProc_RobotSimulation(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static Nu32			static_SimStatus = 0;

	Nf32				f;
	NLPATH_WORKBENCH*	pcurrent_workbench = (NLPATH_WORKBENCH*)GetWindowLong(hwndDlg, GWL_USERDATA);
	NLPATH_WORKBENCH*	pwb = nullptr;
	Nchar				txt[64];

	switch (message)
	{
	case WM_INITDIALOG:
		SetWindowSubclass(GetDlgItem(hwndDlg, IDC_EDIT_SIMULATION_TIME), subEditProcRp, 0, 0);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			SendMessage(GetParent(hwndDlg), WM_MESSAGE_APP_WINDOW_CLOSED, (WPARAM)APP_WINDOW_ENUM::RB_WIN_SIMULATION, (LPARAM)hwndDlg);
			DestroyWindow(hwndDlg);
			return TRUE;

		case IDC_START_SIMULATION:
			if (VirtualRobot.IsOn())
			{
				VirtualRobot.switchOff();
				SetWindowText(GetDlgItem(hwndDlg, IDC_START_SIMULATION), "Start Simulation");
				SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_SIMULATION_TIME_LEFT), "Time left: .");
			}
			else
			{
				if (pcurrent_workbench)
				{
					VirtualRobot.switchOn(pcurrent_workbench,pcurrent_workbench->getTrajectory()->getDt()+2.0f);
					SetWindowText(GetDlgItem(hwndDlg, IDC_START_SIMULATION), "Stop Simulation");
				}
			}
			return TRUE;

		case IDC_EDIT_SIMULATION_TIME:
			switch (HIWORD(wParam))
			{
			case EN_KILLFOCUS:
				 f = GetNf32FromEditControl((HWND)lParam, CONSTANT_NLPROJECT_DT_MIN,NF32_MAX,"%.6f");
				 Project.setSimulationDt(f);
				break;
			default:
				break;
			}
			return TRUE;

		case IDC_CHECK_DISPLAY_ROBOT_TARGET:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISPLAY_ROBOT_TARGET))
				VirtualRobot.setFlagsOn(FLAG_NLVIRTUAL_ROBOT_DRAW_TARGET);
			else
				VirtualRobot.setFlagsOff(FLAG_NLVIRTUAL_ROBOT_DRAW_TARGET);
			return TRUE;

		case IDC_CHECK_DISPLAY_ROBOT_PERFECT:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISPLAY_ROBOT_PERFECT))
				VirtualRobot.setFlagsOn(FLAG_NLVIRTUAL_ROBOT_DRAW_PERFECT);
			else
				VirtualRobot.setFlagsOff(FLAG_NLVIRTUAL_ROBOT_DRAW_PERFECT);
			return TRUE;

		case IDC_CHECK_DISPLAY_ROBOT_SIM:
			if (IsDlgButtonChecked(hwndDlg, IDC_CHECK_DISPLAY_ROBOT_SIM))
				VirtualRobot.setFlagsOn(FLAG_NLVIRTUAL_ROBOT_DRAW_SIM);
			else
				VirtualRobot.setFlagsOff(FLAG_NLVIRTUAL_ROBOT_DRAW_SIM);
			return TRUE;


		default:
			break;
		}
		return FALSE;

	case WM_MESSAGE_SET_CURRENT_PATH_WORKBENCH:
		NErrorIf(wParam != APP_EXPLORER_WINDOW, NERROR_INCONSISTENT_PARAMETERS); // pas une erreur juste une verif...
		// WPARAM : APP_EXPLORER_WINDOW ( Id de l'explorer )
		// LPARAM : NEW  REGISTRED WORKBENCH
		pwb = (NLPATH_WORKBENCH*)lParam;
		NErrorIf((pwb == pcurrent_workbench) && (pcurrent_workbench != NULL), NERROR_INCONSISTENT_PARAMETERS); // Si ils sont non nulls ils doivent être différents !

		// Si une simulation est en cours, on la stoppe ! Et on la relance ! 
		if (VirtualRobot.IsOn())
		{
			VirtualRobot.switchOff();

			if (pwb)
				VirtualRobot.switchOn(pwb);
			else
				SetWindowText(GetDlgItem(hwndDlg, IDC_START_SIMULATION), "Start Simulation");
			
			SetWindowText(GetDlgItem(hwndDlg, IDC_START_SIMULATION), "Start Simulation");
		}

		SetWindowLong(hwndDlg, GWL_USERDATA, (LONG)pwb);
		return TRUE;

	case WM_MESSAGE_DELETE_CURRENT_PATH_WORKBENCH:
		NErrorIf(wParam != APP_EXPLORER_WINDOW, NERROR_INCONSISTENT_PARAMETERS); // pas une erreur juste une verif...
		if (pcurrent_workbench)
		{
			// WPARAM : APP_EXPLORER_WINDOW ( Id de l'explorer )
			// LPARAM : TO BE DELETE REGISTRED WORKBENCH
			pwb = (NLPATH_WORKBENCH*)lParam;
			NErrorIf(pwb != pcurrent_workbench, NERROR_INCONSISTENT_PARAMETERS); // Ils devraient être égaux !
			
			if (VirtualRobot.IsOn())
			{
				VirtualRobot.switchOff();
				SetWindowText(GetDlgItem(hwndDlg, IDC_START_SIMULATION), "Start Simulation");
			}


			SetWindowLong(hwndDlg, GWL_USERDATA, 0);
		}
		return TRUE;

	case WM_MESSAGE_VIRTUAL_ROBOT_UPDATED:
		if (((NLVIRTUAL_ROBOT*)wParam)->IsOn())
		{
			sprintf(txt, "Time left: %.2f sec", ((NLVIRTUAL_ROBOT*)wParam)->getSimulationLeftTime());
			SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_SIMULATION_TIME_LEFT), txt);
		}
		else
		{
			SetWindowText(GetDlgItem(hwndDlg, IDC_START_SIMULATION), "Start Simulation");
			SetWindowText(GetDlgItem(hwndDlg, IDC_STATIC_SIMULATION_TIME_LEFT), "Time left: 0.0 sec");
		}
		return TRUE;


	default:
		break;
	}
	return FALSE;
}

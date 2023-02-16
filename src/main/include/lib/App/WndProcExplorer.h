#pragma once
#include "../N/NCStandard.h"
#include "../NL/NLProject.h"


typedef enum
{
	NOT_AN_ITEM = 0,			//
	ROOT_ROBOT_FOLDER,			//		ROBOT
	ROBOT_SIMULATION,			//			|___SIMULATION											---> IDD_DIALOG_ROBOT_SIMULATION					
	ROBOT_KINLIMITS,			//			|___KINLIMITS											---> IDD_DIALOG_ROBOT_KINLIMITS					x
	ROBOT_PHYSICS,				//			|___PHYSICS												---> IDD_DIALOG_ROBOT_PHYSICS
	ROBOT_GEOMETRY,				//			|___GEOMETRY											---> IDD_DIALOG_ROBOT_GEOMETRY
	ROBOT_CHARACTERIZATION,		//			|___CHARACTERIZATION									---> IDD_DIALOG_ROBOT_CHARACTERIZATION

	ROOT_PATH_FOLDER,			//		PATH
	PATH,						//			|___ PATH[waypoints/polygonal/...]						---> IDD_DIALOG_PATHBUILDER						x
	PATH_STRATEGY,				//					|___ STRATEGY ( only waypoints)					---> IDD_DIALOG_PATH_WAYPOINTS_STRATEGY
	TRAJECTORY,					//					|___ TRAJECTORY
	TRAJECTORY_KINLIMITS,		//								|___KINLIMITS						---> IDD_DIALOG_PATH_KINLIMITS					
	TRAJECTORY_KTWEAKS,			//								|___KTWEAKS							---> IDD_DIALOG_KTWEAKS							x
	TRAJECTORY_VZONES,			//								|___VZONES							---> IDD_DIALOG_USERVZONE
	TRAJECTORY_VZONE,			//								|		|___VZONE					---> 
	TRAJECTORY_KEYS,			//								|___KEYS							---> IDD_DIALOG_USERKEY
	TRAJECTORY_KEY,				//								|		|___KEY						---> 
	TRAJECTORY_MOTION_PROFILE,	//								|___MOTIONPROFILE ?					?
	EXPLORER_ITEM_ID_ENUM_SIZE
}EXPLORER_ITEM_ID_ENUM;

BOOL CALLBACK WndProc_Explorer(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);


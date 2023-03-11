#pragma once
#include "../../../N/NCStandard.h"
/*
#include "../../../NL/MotionControl/NLPathWorkbench.h"

typedef struct DATA_WndProc_TrajectoryKeyProperties	DATA_WndProc_TrajectoryKeyProperties;
struct  DATA_WndProc_TrajectoryKeyProperties
{
	NLPATH_WORKBENCH	*m_pathWorkBench;
	Nu32				m_currentKeyId;
};
*/

BOOL CALLBACK WndProc_TrajectoryKeyProperties(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);



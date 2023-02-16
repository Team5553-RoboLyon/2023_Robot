#pragma once
#include "../../../N/NCStandard.h"

// ATTENTION Define commun à 'WndProc_RobotKinLimits' et 'WndProcPathKinLimits'
#define TRACKBAR_SIZE				250		// The whole trackbar Range (100% de la trackbar = 250 )
#define TRACKBAR_JERK_MAX			100.0f
#define TRACKBAR_ACCEL_MAX			15.0f
#define TRACKBAR_VELOCITY_MAX		10.0f

#define TXTOUT_SIZE					128
void updateKinLimitsCtrls(HWND hwnd, const NLKINLIMITS* pk, const Nu32 flags, Nchar* pstr);

BOOL CALLBACK WndProc_RobotKinLimits(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

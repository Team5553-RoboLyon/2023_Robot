#pragma once
#include "../../../N/NCStandard.h"
#include "../../../NL/MotionControl/NLPathWorkbench.h"
#include "../../../NL/MotionControl/Trajectory/NLTrajectoryKey.h"

// Positions et dimensions des fenêtres actions
#define NL_ACTIONWINDOW_X0				32
#define NL_ACTIONWINDOW_Y0				50
#define NL_ACTIONWINDOW_WIDTH			750
#define NL_ACTIONWINDOW_HEIGHT			32
#define NL_ACTIONWINDOW_YSHIFT			(NL_ACTIONWINDOW_HEIGHT + 4)

#define NL_ACTIONWINDOW_LEFT_MARGIN		6		// Marge interne gauche ( position x / actionWindow, du controle le + à gauche )
#define NL_ACTIONWINDOW_RIGHT_MARGIN	32
#define NL_ACTIONWINDOW_BOTTOM_MARGIN	48

#define NL_TKP_RESTART_LINE_HEIGHT		40		//  Hauteur de la ligne contenant les controles d'édition du restart de la clef.


// IDs des contrôles des fenêtres Actions ( contôles créés manuellement )
#define COMBO_ACTIONS_ID						(WM_USER+1000)
#define COMBO_MESSAGES_ID						(WM_USER+1001)

#define EDIT_ROTATION_DEG						(WM_USER+1010)
#define EDIT_ROTATION_VELOCITY_RATIO			(WM_USER+1011)
#define STATIC_ROTATION_DEG_UNIT				(WM_USER+1012)
#define STATIC_ROTATION_VELOCITY_RATIO_UNIT		(WM_USER+1013)


#define EDIT_ACTION_TIMESHIFT					(WM_USER+1020)
#define STATIC_ACTION_PLUS						(WM_USER+1021)
#define STATIC_ACTION_TIMESHIFT_UNIT			(WM_USER+1022)

#define STATIC_ACTION_TIME_STAMP				(WM_USER+1030)
#define STATIC_ACTION_TIME_DT					(WM_USER+1031)

#define AUTO_KEY_TIME_ID						(WM_USER+1040)

// structure utilisée à la creation d'une fenêtre Action
typedef struct WND_KEYACTION_CREATE_DATA	WND_KEYACTION_CREATE_DATA;
struct WND_KEYACTION_CREATE_DATA
{
	NLTRJKEY*			getKeyPtr() { return ((m_keyIndex == NVOID)||(!m_pathWorkBench)) ? NULL : (NLTRJKEY*)NGetArrayPtr(m_pathWorkBench->getTrajectory()->getKeysArray(), m_keyIndex); };
	NLTRJACTION*		getActionPtr(){ return (m_actionIndex == NVOID)? NULL: (NLTRJACTION*)NGetArrayPtr(&getKeyPtr()->m_actionsArray, m_actionIndex);};

	NLPATH_WORKBENCH*	m_pathWorkBench;
	Nu32				m_keyIndex;
	Nu32				m_actionIndex;
	//NLTRJKEY*			m_pKey;
	//NLTRJACTION*		m_pAction;
};

// structure de passage de paramètres via 'EnumChildWindows'

typedef struct ECW_ACTION_DATA	ECW_ACTION_DATA;
struct ECW_ACTION_DATA
{
	NLTRJKEY* getKeyPtr() { return ((m_keyIndex == NVOID) || (!m_pathWorkBench)) ? NULL : (NLTRJKEY*)NGetArrayPtr(m_pathWorkBench->getTrajectory()->getKeysArray(), m_keyIndex); };
	NLTRJACTION* getActionPtr0() { return (m_actionIdx0 == NVOID) ? NULL : (NLTRJACTION*)NGetArrayPtr(&getKeyPtr()->m_actionsArray, m_actionIdx0); };
	NLTRJACTION* getActionPtr1() { return (m_actionIdx1 == NVOID) ? NULL : (NLTRJACTION*)NGetArrayPtr(&getKeyPtr()->m_actionsArray, m_actionIdx1); };

	NLPATH_WORKBENCH*	m_pathWorkBench;// Workbench courant
	Nu32				m_keyIndex;		// index de la clef en cours d'affichage
	Nu32				m_actionIdx0;	// index de l'action insérée, ou de l'action supprimée, ou index "avant" le déplacement d'une action à déplacer.
	Nu32				m_actionIdx1;	// index de l'action déplacée, "après" son déplacement.
}; // 'EnumChildWindows' Action Data


Nu32 getHwndActionIndex(HWND hwnd);
#define setTrjKeyPropertiesSize(hwnd,nbactions)	SetWindowPos(hwnd, NULL, 0, 0, NL_ACTIONWINDOW_X0 + NL_ACTIONWINDOW_WIDTH + NL_ACTIONWINDOW_RIGHT_MARGIN, NL_ACTIONWINDOW_Y0 + (nbactions) * NL_ACTIONWINDOW_YSHIFT + NL_TKP_RESTART_LINE_HEIGHT + NL_ACTIONWINDOW_BOTTOM_MARGIN , SWP_NOMOVE | SWP_NOZORDER)

BOOL CALLBACK	redrawActionWindows_Callback(HWND hwnd, LPARAM lParam);
BOOL CALLBACK	enableActionAutoCheckBox_Callback(HWND hwnd, LPARAM lParam);
BOOL CALLBACK	updateAction_Callback(HWND   hwnd, LPARAM lParam);
BOOL CALLBACK	updateActionPosition_Callback(HWND   hwnd, LPARAM lParam);
BOOL CALLBACK	afterActionErase_Callback(HWND   hwnd, LPARAM lParam);
BOOL CALLBACK	afterActionInsertion_Callback(HWND   hwnd, LPARAM lParam);

void updateActionsTable_afterActionErase(HWND hwndDlg, NLPATH_WORKBENCH* pwb, const Nu32 keyidx, const Nu32 erasedactionidx);
void updateActionsTable_afterActionInsertion(HWND hwndDlg, NLPATH_WORKBENCH* pwb, const Nu32 keyidx, const Nu32 newactionidx);
void updateActionsTable_afterActionIndexShifting(HWND hwndDlg, NLPATH_WORKBENCH* pwb, const Nu32 keyidx, const Nu32 idx0, const Nu32 idx1);
void updateActionsTable_afterKeyIndexUpdate(HWND hwndDlg, NLPATH_WORKBENCH* pwb, const Nu32 keyidx);
void updateActionsTable_afterKeyDurationUpdate(HWND hwnd, NLPATH_WORKBENCH* pwb, const Nu32 keyidx);

HWND insertActionHwnd(HWND hwndDlg, NLPATH_WORKBENCH* pcurrent_workbench, const Nu32 keyid, const Nu32 insertedactionIndex);
#define DestroyActionHwnd(hwnd)		DestroyWindow(hwnd)

LRESULT CALLBACK WndProc_TrajectoryKeyAction(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);


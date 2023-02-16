#include <iostream>
#include <windows.h>
#include <CommCtrl.h>
//#include <new>

#include "../../resource.h"
#include "../N/Containers/NArray.h"
#include "../N/NString.h"
#include "../NL/NLProject.h"
#include "../NL/MotionControl/Path/NLPath.h"
#include "../NL/MotionControl/DriveTrain/NLDriveTrainSpecs.h"
#include "../NL/MotionControl/Path/Builders/PolygonalChain/NLPathBuilderPolygonalChain.h"
#include "../NL/MotionControl/Path/Builders/WayPoints/NLPathBuilderWayPoints.h"
#include "../App/App_WayPoints_Tools.h"
#include "../App/App_PolygonalChain_Tools.h"
#include "App_Init.h"

#include "./Windows/WndProcPathBuilder.h"
#include "./Windows/Robot/WndProcRobotSimulation.h"
#include "./Windows/Robot/WndProcRobotKinLimits.h"
#include "./Windows/Robot/WndProcRobotPhysics.h"
#include "./Windows/Robot/WndProcRobotGeometry.h"

#include "./Windows/PathAndTrajectory/WndProcPathBuilderWayPointsStrategy.h"
#include "./Windows/PathAndTrajectory/WndProcTrajectoryUserVZones.h"
#include "./Windows/PathAndTrajectory/WndProcTrajectoryKeys.h"
#include "./Windows/PathAndTrajectory/WndProcTrajectoryKeyProperties.h"

#include "./Windows/WndProcTrajectoryKinematicLimitTweaks.h"
#include "./Windows/PathAndTrajectory/WndProcPathKinLimits.h"

#include "WndProcExplorer.h"

// local use only
#define IDPOPUPMENU_CREATE_NEW_WAYPOINTS_PATH_WB	2001
#define IDPOPUPMENU_CREATE_NEW_PLG_CHAIN_PATH_WB	2002

#define IDPOPUPMENU_DELETE_PATH_WB					3001
#define IDPOPUPMENU_EDIT_PATH_WB					3002
#define IDPOPUPMENU_EDIT_PATH_WP_STRATEGY			3003
#define IDPOPUPMENU_EDIT_TRJ_KINLIMITS				3004
#define IDPOPUPMENU_EDIT_TRJ_KTWEAKS				3005
#define IDPOPUPMENU_EDIT_TRJ_VZONES					3006
#define IDPOPUPMENU_EDIT_TRJ_KEYS					3007
#define IDPOPUPMENU_EDIT_TRJ_KEY_PROPERTIES			3008

#define IDPOPUPMENU_EDIT_ROBOT_SIMULATION			4001	
#define IDPOPUPMENU_EDIT_ROBOT_KINLIMITS			4002
#define IDPOPUPMENU_EDIT_ROBOT_PHYSICS				4003
#define IDPOPUPMENU_EDIT_ROBOT_GEOMETRY				4004
#define IDPOPUPMENU_EDIT_ROBOT_CHARACTERIZATION		4005

#define IDPOPUPMENU_SAVE_TRAJECTORY_PACKS			5001


#define TREEVIEW_NAME_SIZE							32
/*
extern NLDRIVETRAINSPECS			Project.m_driveTrainSpecifications;
extern NARRAY						Project.m_workbenchArray;
extern NLPATH_WORKBENCH* Project.m_pCurrentWorkbench;
extern NLKINTWEAK					Ktweaks;
*/
extern NLPROJECT					Project;
extern HWND							hwnd_PathBuilder;
extern HWND							hwnd_Ktweaks;


//extern HWND							WorkBenchWindowsTable[];
//extern HWND							RobotWindowsTable[];
extern HWND							AppWindowsTable[];

extern APP_WAYPOINT_TOOLDATA		WayPointsToolData;
extern APP_PLG_CHAIN_TOOLDATA		PolygonalChainToolData;




HWND hwnd_Explorer = NULL;


static void DeletePathWorkbenchComponents(void* ptr)
{
	NLPATH_WORKBENCH* pwb = (NLPATH_WORKBENCH*)ptr;
	
	delete (pwb->getPathBuilder());
	pwb->getTrajectory()->setPath(NULL);
	delete (pwb->getPath());
	delete (pwb->getTrajectory());
//	delete (pwb->getTrajectoryStatePack());
//	delete (pwb->getTrajectoryStateSPack());
}


void _redrawTreeview(HWND htv)
{
	//SendMessage(htv, WM_SETREDRAW, TRUE, 0);
	//InvalidateRect(htv, NULL, FALSE);
	//UpdateWindow(htv);
	RedrawWindow(htv, NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_INVALIDATE);
	//RedrawWindow(htv, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
}

HTREEITEM InsertTreeViewItem(HWND htreeview, HTREEITEM hparent, const EXPLORER_ITEM_ID_ENUM item_id, Nchar* name, Nu32 img, Nu32 selimg)
{
	TVINSERTSTRUCT	tvinsert;

	tvinsert.hParent = hparent;
	tvinsert.hInsertAfter = TVI_LAST;
	ZeroMemory(&(tvinsert.itemex), sizeof(TVITEMEXA));
	tvinsert.itemex.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvinsert.item.iImage = img;
	tvinsert.item.iSelectedImage = selimg;
	tvinsert.item.lParam = NMAKELONG(item_id, 0);
	tvinsert.item.pszText = name;
	tvinsert.item.cchTextMax = TREEVIEW_NAME_SIZE;
	return TreeView_InsertItem(htreeview, &tvinsert);
}

void UpdateTreeViewItemKeyParamIds(HWND htreeview, NLPATH_WORKBENCH* pwb )
{
	TVITEMEX	tvix;
	Nu32		idtype, idkey;
	Nchar		str[TREEVIEW_NAME_SIZE];
	NLTRJKEY* pk;
#ifdef _DEBUG
	ZeroMemory(&(tvix), sizeof(TVITEMEXA));
	tvix.mask = TVIF_PARAM /*| TVIF_TEXT*/;
	tvix.hItem = pwb->m_hTreeViewKeys;
	TreeView_GetItem(htreeview, &tvix);
	idtype = NLOWORD(tvix.lParam);
	NErrorIf(idtype != EXPLORER_ITEM_ID_ENUM::TRAJECTORY_KEYS, NERROR_INCONSISTENT_VALUES);
#endif

	// Update des index des keys
	HTREEITEM hitem = TreeView_GetChild(htreeview, pwb->m_hTreeViewKeys);
	Nu32 i = 0;
	ZeroMemory(&(tvix), sizeof(TVITEMEXA));
	tvix.mask = TVIF_PARAM | TVIF_TEXT;
	while (hitem)
	{
		tvix.hItem = hitem;
		TreeView_GetItem(htreeview, &tvix);
		idtype = NLOWORD(tvix.lParam); // Le type: une key. Juste pour verif.
		idkey = NHIWORD(tvix.lParam);	// L'index, de cette key dans le tree view

		NErrorIf(idtype != EXPLORER_ITEM_ID_ENUM::TRAJECTORY_KEY, NERROR_INCONSISTENT_VALUES);

		if (idkey != 0xFFFF && idkey != 0)
		{
			tvix.lParam = NMAKELONG(EXPLORER_ITEM_ID_ENUM::TRAJECTORY_KEY, i);
			pk = (NLTRJKEY*)NGetArrayPtr(pwb->getTrajectoryKeysArray(),i);
			sprintf(str, "key%d %.2f m", i, pk->m_s);
			tvix.pszText = str;
			tvix.cchTextMax = TREEVIEW_NAME_SIZE;

			TreeView_SetItem(htreeview, &tvix);
		}
		hitem = TreeView_GetNextSibling(htreeview, hitem);
		i++;
	}
}
HTREEITEM GetTreeViewItemKey(HWND htreeview, NLPATH_WORKBENCH* pwb, const Nu32 trajectory_key_index)
{
	HTREEITEM	hitem = TreeView_GetChild(htreeview, pwb->m_hTreeViewKeys);
	Nu32 i = 0;
	while (i < trajectory_key_index) { hitem = TreeView_GetNextSibling(htreeview, hitem); i++; }
	/*
#ifdef _DEBUG
	TVITEMEX	tvix;
	Nu32		idkey,idtype;
	ZeroMemory(&(tvix), sizeof(TVITEMEXA));
	tvix.mask = TVIF_PARAM / *| TVIF_TEXT* /;
	tvix.hItem = hitem;
	TreeView_GetItem(htreeview, &tvix);
	idtype	= NLOWORD(tvix.lParam); // Le type: une key. Juste pour verif.
	idkey	= NHIWORD(tvix.lParam);	// L'index, de cette key dans le tree view
	NErrorIf(idtype != EXPLORER_ITEM_ID_ENUM::TRAJECTORY_KEY, NERROR_INCONSISTENT_VALUES);
	NErrorIf(idkey  != i, NERROR_INCONSISTENT_VALUES);
#endif
*/
	return hitem;
}

void SelectTreeViewItemKey(HWND htreeview, NLPATH_WORKBENCH* pwb, const Nu32 keyid)
{
	HTREEITEM	hitem = TreeView_GetChild(htreeview, pwb->m_hTreeViewKeys);
	TVITEMEX	tvix;
	ZeroMemory(&(tvix), sizeof(TVITEMEXA));

	while (hitem)
	{
		tvix.mask = TVIF_PARAM;
		tvix.hItem = hitem;
		TreeView_GetItem(htreeview, &tvix);

		if (keyid == NHIWORD(tvix.lParam))
		{
			TreeView_SelectItem(htreeview, hitem);
			break;
		}
		hitem = TreeView_GetNextSibling(htreeview, hitem);
	}

}

HTREEITEM InsertTreeViewItemKey(HWND htreeview, NLPATH_WORKBENCH* pwb, const Nu32 keyid)
{
	NErrorIf(keyid > 0xFFFF, NERROR_4BITS_VALUE_OUTOFRANGE); // 0xFFFF est réservé comme index max ! et correspond toujours à la key de fin d'array !

	Nchar			str[TREEVIEW_NAME_SIZE];
	HTREEITEM		hitem;

	TVINSERTSTRUCT	tvinsert;
	ZeroMemory(&(tvinsert.itemex), sizeof(TVITEMEXA));
	tvinsert.hParent = pwb->m_hTreeViewKeys;
	tvinsert.itemex.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvinsert.item.iImage = 2;
	tvinsert.item.iSelectedImage = 2;

	if (keyid == 0xFFFF)
	{
		tvinsert.hInsertAfter = TVI_LAST;
		tvinsert.item.pszText = "End";
	}
	else if (keyid == 0)
	{
		tvinsert.hInsertAfter = TVI_FIRST;
		tvinsert.item.pszText = "Start";
	}
	else
	{
		NLTRJKEY* pk = (NLTRJKEY*)NGetArrayPtr(pwb->getTrajectoryKeysArray(), keyid);
		tvinsert.hInsertAfter = GetTreeViewItemKey(htreeview, pwb, keyid-1);
		sprintf(str, "key%d %.2f m",keyid, pk->m_s );
		tvinsert.item.pszText = str;

	}
	tvinsert.item.lParam = NMAKELONG(EXPLORER_ITEM_ID_ENUM::TRAJECTORY_KEY, keyid);
	tvinsert.item.cchTextMax = TREEVIEW_NAME_SIZE;
	hitem = TreeView_InsertItem(htreeview, &tvinsert);
	return hitem;
}
HTREEITEM MoveTreeViewItemKey(HWND htreeview, NLPATH_WORKBENCH* pwb, const Nu32 pack)
{
	Nu32		id1 = NLOWORD(pack); // index "réel" qui restera sélectionné par les outils NTOOL après cycle total.
	Nu32		id0 = NHIWORD(pack); // index actuellement sélectionné par les outils NTOOL 	
									 // potentiellement id0 et id1 sont different. Cela signifie que actuellement  id0 est selectionné, mais qu'il y a eut "passage par dessus"	
									 // dans un sens ou dans l'autre et du coup il conviendra à la fin du cycle de décaler de 1, vers la droite ou la gauche la key selectionnée par les NTOOL et donc celle sélectionnée dan sle TREEVIEW
	Nu32		i = 0;
	HTREEITEM	hitem,hitem0;
	TVITEMEX	tvix;
	Nu32		idtype, idkey;
	Nchar		str[TREEVIEW_NAME_SIZE];

	if (id0 == id1)
	{
		NLTRJKEY* pk = (NLTRJKEY*)NGetArrayPtr(pwb->getTrajectoryKeysArray(), id0);
		hitem = TreeView_GetChild(htreeview, pwb->m_hTreeViewKeys);
		ZeroMemory(&(tvix), sizeof(TVITEMEXA));

		while (hitem)
		{
			tvix.mask = TVIF_PARAM;
			tvix.hItem = hitem;
			TreeView_GetItem(htreeview, &tvix);

			idtype = NLOWORD(tvix.lParam); // Le type: une key. Juste pour verif.
			idkey = NHIWORD(tvix.lParam);	// L'index, de cette key dans le tree view

			if (i == id0)
			{
				sprintf(str, "key%d %.2f m", i, pk->m_s);
				tvix.mask = TVIF_TEXT;
				tvix.hItem = hitem;
				tvix.pszText = str;
				tvix.cchTextMax = TREEVIEW_NAME_SIZE;
				TreeView_SetItem(htreeview, &tvix);
				//SelectTreeViewItemKey(htreeview, pwb, id1);
				//TreeView_SelectItem(htreeview, hitem);
				break;
			}
			hitem = TreeView_GetNextSibling(htreeview, hitem);
			i++;
		}
	}
	else
	{
		hitem0 = GetTreeViewItemKey(htreeview, pwb, id0);
		TreeView_DeleteItem(htreeview, hitem0);
		
		hitem = InsertTreeViewItemKey(htreeview, pwb, id1);
		//TreeView_SelectItem(htreeview, hitem);
		//SelectTreeViewItemKey(htreeview, pwb, id1);

	}
	return hitem;
}

void DeleteTreeViewItemKey(HWND htreeview, NLPATH_WORKBENCH* pwb, const Nu32 delkeyid)
{
	TreeView_DeleteItem(htreeview, GetTreeViewItemKey(htreeview, pwb, delkeyid));
}

HTREEITEM InsertTreeViewPathItemHierarchy(HWND htreeview, HTREEITEM hparent, NLPATH_WORKBENCH *pwb, const Nu32 wb_index, const NLPATH_WORKBENCH::ID wb_id)
{
	TVINSERTSTRUCT	tvinsert;
	HTREEITEM		hitem_path;
	HTREEITEM		hitem_trajectory;
	HTREEITEM		hitem_vzones;
	HTREEITEM		hitem_keys;

	//sprintf(txt, "#%d %s", path_index, suffix);
	//sprintf(txt, "Path N°%d", path_index);
	tvinsert.hParent = hparent;
	tvinsert.hInsertAfter = TVI_LAST;
	ZeroMemory(&(tvinsert.itemex), sizeof(TVITEMEXA));
	tvinsert.itemex.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvinsert.item.iImage = 4;
	tvinsert.item.iSelectedImage = 4;
	tvinsert.item.lParam = NMAKELONG(EXPLORER_ITEM_ID_ENUM::PATH, wb_index);
	tvinsert.item.pszText = pwb->m_name;
	tvinsert.item.cchTextMax = TREEVIEW_NAME_SIZE;
	hitem_path = TreeView_InsertItem(htreeview, &tvinsert);

	// insertion de l'item STRATEGY pour les WAY POINTS uniquement
	if (wb_id == NLPATH_WORKBENCH::ID::WB_WAYPOINTS)
	{
		tvinsert.hParent = hitem_path;
		tvinsert.hInsertAfter = TVI_LAST;
		ZeroMemory(&(tvinsert.itemex), sizeof(TVITEMEXA));
		tvinsert.itemex.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
		tvinsert.item.iImage			= 2;
		tvinsert.item.iSelectedImage	= 2;
		tvinsert.item.lParam			= NMAKELONG(EXPLORER_ITEM_ID_ENUM::PATH_STRATEGY, wb_index);
		tvinsert.item.pszText			= "Strategy";
		tvinsert.item.cchTextMax		= TREEVIEW_NAME_SIZE;
		TreeView_InsertItem(htreeview, &tvinsert);
	}


	// insertion de l'item Trajectory
	tvinsert.hParent = hitem_path;
	tvinsert.hInsertAfter = TVI_LAST;
	ZeroMemory(&(tvinsert.itemex), sizeof(TVITEMEXA));
	tvinsert.itemex.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvinsert.item.iImage			= 4;
	tvinsert.item.iSelectedImage	= 4;
	tvinsert.item.lParam = NMAKELONG(EXPLORER_ITEM_ID_ENUM::TRAJECTORY, wb_index);
	tvinsert.item.pszText = "Trajectory";
	tvinsert.item.cchTextMax = TREEVIEW_NAME_SIZE;
	hitem_trajectory = TreeView_InsertItem(htreeview, &tvinsert);

	// insertion de l'item Kinematics Limits ( pour tous les paths )
	tvinsert.hParent = hitem_trajectory;
	tvinsert.hInsertAfter = TVI_LAST;
	ZeroMemory(&(tvinsert.itemex), sizeof(TVITEMEXA));
	tvinsert.itemex.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvinsert.item.iImage = 2;
	tvinsert.item.iSelectedImage = 2;
	tvinsert.item.lParam = NMAKELONG(EXPLORER_ITEM_ID_ENUM::TRAJECTORY_KINLIMITS, wb_index);
	tvinsert.item.pszText = "Specific Kinematics Limits";
	tvinsert.item.cchTextMax = TREEVIEW_NAME_SIZE;
	TreeView_InsertItem(htreeview, &tvinsert);

	// insertion de l'item KTweaks ( pour toutes les Trajectory )
	tvinsert.hParent = hitem_trajectory;
	tvinsert.hInsertAfter = TVI_LAST;
	ZeroMemory(&(tvinsert.itemex), sizeof(TVITEMEXA));
	tvinsert.itemex.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvinsert.item.iImage = 2;
	tvinsert.item.iSelectedImage = 2;
	tvinsert.item.lParam = NMAKELONG(EXPLORER_ITEM_ID_ENUM::TRAJECTORY_KTWEAKS, wb_index);
	tvinsert.item.pszText = "Kinematic Tweaks";
	tvinsert.item.cchTextMax = TREEVIEW_NAME_SIZE;
	TreeView_InsertItem(htreeview, &tvinsert);

	// insertion de l'item VZone ( pour toutes les Trajectory )
	tvinsert.hParent = hitem_trajectory;
	tvinsert.hInsertAfter = TVI_LAST;
	ZeroMemory(&(tvinsert.itemex), sizeof(TVITEMEXA));
	tvinsert.itemex.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvinsert.item.iImage = 2;
	tvinsert.item.iSelectedImage = 2;
	tvinsert.item.lParam = NMAKELONG(EXPLORER_ITEM_ID_ENUM::TRAJECTORY_VZONES, wb_index);
	tvinsert.item.pszText = "Custom Velocity Zones";
	tvinsert.item.cchTextMax = TREEVIEW_NAME_SIZE;
	hitem_vzones = TreeView_InsertItem(htreeview, &tvinsert);
	pwb->m_hTreeViewVzones = hitem_vzones;

	// insertion de l'item Keys ( pour toutes les Trajectory )
	tvinsert.hParent = hitem_trajectory;
	tvinsert.hInsertAfter = TVI_LAST;
	ZeroMemory(&(tvinsert.itemex), sizeof(TVITEMEXA));
	tvinsert.itemex.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvinsert.item.iImage = 2;
	tvinsert.item.iSelectedImage = 2;
	tvinsert.item.lParam = NMAKELONG(EXPLORER_ITEM_ID_ENUM::TRAJECTORY_KEYS, wb_index);
	tvinsert.item.pszText = "Keys";
	tvinsert.item.cchTextMax = TREEVIEW_NAME_SIZE;
	hitem_keys = TreeView_InsertItem(htreeview, &tvinsert);
	pwb->m_hTreeViewKeys = hitem_keys;

	// populate keys
//	InsertTreeViewItemKey(htreeview, pwb, 0);
//	InsertTreeViewItemKey(htreeview, pwb, 0xFFFF);
	NARRAY* pkarray = pwb->m_pTrajectory->getKeysArray();
	NErrorIf(pkarray->Size < 2, NERROR_WRONG_SIZE);
	for (Nu32 i = 0; i < pkarray->Size-1; i++)
	{
		InsertTreeViewItemKey(htreeview, pwb, i);
	}
	InsertTreeViewItemKey(htreeview, pwb, 0xFFFF);
	return hitem_path;
}


static void InitializeTreeViewIcons(HWND htreeview)
{
	HINSTANCE dllhinst;
	HICON hIcon;
	HIMAGELIST himgList;

	// 1) ICONES UTILISEES PAR LE TREEVIEW
	himgList = ImageList_Create(GetSystemMetrics(SM_CXICON)/2, GetSystemMetrics(SM_CYICON)/2, ILC_COLOR32, 3, 3);
	//himgList = ImageList_Create(24, 24, ILC_COLOR32, 3, 3);
	ImageList_SetBkColor(himgList, GetSysColor(COLOR_WINDOW));
	dllhinst = LoadLibrary("shell32.dll");
	hIcon = LoadIcon(dllhinst, MAKEINTRESOURCE(13));	// 0: composant electronique genre mofset
	ImageList_AddIcon(himgList, hIcon);
	hIcon = LoadIcon(dllhinst, MAKEINTRESOURCE(5));		// 1:Dossier vide vertical
	ImageList_AddIcon(himgList, hIcon);
	hIcon = LoadIcon(dllhinst, MAKEINTRESOURCE(151));	// 2:document + gear ( params ) format vertical
	ImageList_AddIcon(himgList, hIcon);
	hIcon = LoadIcon(dllhinst, MAKEINTRESOURCE(173));	// 3:Dossier vertical avec etoile
	ImageList_AddIcon(himgList, hIcon);
	hIcon = LoadIcon(dllhinst, MAKEINTRESOURCE(44));	// 4:Etoile
	ImageList_AddIcon(himgList, hIcon);
	hIcon = LoadIcon(dllhinst, MAKEINTRESOURCE(246));	// 5:Bouton Play Bleu
	ImageList_AddIcon(himgList, hIcon);
	hIcon = LoadIcon(dllhinst, MAKEINTRESOURCE(167));	// 6:mur de brique façon tetris
	ImageList_AddIcon(himgList, hIcon);
	hIcon = LoadIcon(dllhinst, MAKEINTRESOURCE(23));	// 7:Loupe
	ImageList_AddIcon(himgList, hIcon);


	FreeLibrary(dllhinst);
	TreeView_SetImageList(htreeview, himgList, TVSIL_NORMAL);
}


static HTREEITEM InitializeTreeViewItems(HWND htreeview, const NLDRIVETRAINSPECS *pdts)
{
	static Nu32 TreeViewStatus = 0;

	// 1) Si le Treeview à déjà été initialisé, on efface tout et on recommence !
	if (TreeViewStatus == 1) // means already initialized
	{
		DWORD styles = GetWindowLong(htreeview, GWL_STYLE);
		TreeView_DeleteAllItems(htreeview);
		SetWindowLong(htreeview, GWL_STYLE, styles);
	}

	// 2) INSERTIONS DES ELEMENTS:
	// ----------------------------------------------------------------------------------
	HTREEITEM hitem_robot, hitem_path_folder;

	hitem_robot = InsertTreeViewItem(htreeview, TVI_ROOT, EXPLORER_ITEM_ID_ENUM::ROOT_ROBOT_FOLDER, "Robot", 0, 0);

	// 2.1.1) ROBOT Kinematics
	InsertTreeViewItem(htreeview, hitem_robot, EXPLORER_ITEM_ID_ENUM::ROBOT_SIMULATION, "Simulation", 6, 6);

	// 2.1.1) ROBOT Kinematics
	InsertTreeViewItem(htreeview, hitem_robot, EXPLORER_ITEM_ID_ENUM::ROBOT_KINLIMITS, "Kinematics Limits", 2, 2);

	// 2.1.2) ROBOT Physics
	InsertTreeViewItem(htreeview, hitem_robot, EXPLORER_ITEM_ID_ENUM::ROBOT_PHYSICS, "Physics", 2, 2);

	// 2.1.3) ROBOT Geometry
	InsertTreeViewItem(htreeview, hitem_robot, EXPLORER_ITEM_ID_ENUM::ROBOT_GEOMETRY, "Geometry", 2, 2);

	// 2.1.4) ROBOT Characterization
	InsertTreeViewItem(htreeview, hitem_robot, EXPLORER_ITEM_ID_ENUM::ROBOT_CHARACTERIZATION, "Characterization", 2, 2);

	// ----------------------------------------------------------------------------------
	// 2.2) PATH FOLDER
	hitem_path_folder = InsertTreeViewItem(htreeview, TVI_ROOT, EXPLORER_ITEM_ID_ENUM::ROOT_PATH_FOLDER, "Paths", 3, 3);

	return hitem_path_folder;
}

void RefreshTreeView(HWND htreeview, NLPROJECT* pprj)
{
	HTREEITEM		hitem_pathfolder;
	TreeView_DeleteAllItems(htreeview);
	hitem_pathfolder = InitializeTreeViewItems(htreeview, &pprj->m_driveTrainSpecifications);

	NLPATH_WORKBENCH* pwb = (NLPATH_WORKBENCH*)pprj->m_workbenchArray.pFirst;
	for (Nu32 i = 0; i < pprj->m_workbenchArray.Size; i++, pwb++)
	{
		InsertTreeViewPathItemHierarchy(htreeview, hitem_pathfolder, pwb, i, (NLPATH_WORKBENCH::ID)pwb->getId());
	}
}



inline void SendMessageToAllAppWindows(UINT   Msg, WPARAM wParam, LPARAM lParam)
{
	for (Nu32 i = 0; i < APP_WINDOW_ENUM::APP_WIN_ENUM_SIZE; i++)
	{
		if (AppWindowsTable[i])
			SendMessage(AppWindowsTable[i], Msg, wParam, lParam);
	}
}

BOOL CALLBACK WndProc_Explorer(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	static HTREEITEM		static_hitem_CurrentEdited		= nullptr;
	static Nu32				static_currentworkbench_index	= NVOID;
	static Nu32				static_currentTrajectoryKey_index = NVOID;

	HWND					htreeview = GetDlgItem(hwndDlg, IDC_TREE_EXPLORER);
	HTREEITEM				hitem,hitem_parent;
	TVITEMEX				tvix,tvi_update_apparence,tvix_parent;
	LPNMTVDISPINFO			ptvdisplay;
	char					txt[32];


	NLPATH_WORKBENCH	*pworkbench,*pw;
	Nu32				item_id;
	Nu32				item_index, deleted_item_index;					// Tree view item
	Nu32				wb_index,current_wb_index;	// Workbench (in the Project.m_workbenchArray)
	
	Nu32		i = 0;


	NLPATH_WORKBENCH	*pwb;
	LPNMTREEVIEW		pnmtv;
	TV_ITEM				tvi;
	LPNM_TREEVIEW		pntv;
	
	Nu32 popupmenu_choice = 0;
	
	switch (message)
	{
	case WM_INITDIALOG:
		InitializeTreeViewIcons(htreeview);
		InitializeTreeViewItems(htreeview,NULL);
		_redrawTreeview(htreeview);
		//InvalidateRect(htreeview, NULL, false);
		return TRUE;


	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			DestroyWindow(hwndDlg);
			hwnd_Explorer = NULL;
			return TRUE;

		default:
			return FALSE;
		//case IDC_xxx:
		//	return TRUE;

		}
		return FALSE;

	case WM_NOTIFY:
		switch (LOWORD(wParam))
		{
		case IDC_TREE_EXPLORER:


			pntv = (LPNM_TREEVIEW)lParam;
			
			// Click droit sur le TreeView
			switch (pntv->hdr.code)
			{
			case TVN_ITEMEXPANDED:
				_redrawTreeview(htreeview);
				return TRUE;

			case TVN_SELCHANGED:
				pnmtv = (LPNMTREEVIEW)lParam;
				if (pnmtv->action == TVC_BYMOUSE)
					return FALSE;

				hitem = nullptr;
				htreeview = GetDlgItem(hwndDlg, IDC_TREE_EXPLORER);

				// Retrieve current selected Tree view item :
				hitem = (HTREEITEM)SendDlgItemMessage(hwndDlg, IDC_TREE_EXPLORER, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)hitem);
				if (hitem != NULL)
				{
					TreeView_EnsureVisible(htreeview, hitem); // S'assure que l'item sélectionné courant dan sle treeview est bien visible, en faisant scroller et en ouvrant les parents si besoin ... 
					ZeroMemory(&(tvix), sizeof(TVITEMEXA));
					tvix.mask = TVIF_TEXT | TVIF_PARAM;
					tvix.hItem = hitem;
					tvix.pszText = txt;
					tvix.cchTextMax = 32;
					TreeView_GetItem(htreeview, &tvix);

					switch (NLOWORD(tvix.lParam))
					{
					case EXPLORER_ITEM_ID_ENUM::TRAJECTORY_KEY:
						hitem_parent = TreeView_GetParent(htreeview, hitem);
						ZeroMemory(&(tvix_parent), sizeof(TVITEMEXA));
						tvix_parent.mask = TVIF_PARAM;
						tvix_parent.hItem = hitem_parent;
						TreeView_GetItem(htreeview, &tvix_parent);
						if (NHIWORD(tvix_parent.lParam) == static_currentworkbench_index)
						{
							i = NHIWORD(tvix.lParam); // recup' key_id

						}
						break;

					default:
						break;
					}
				}
				return FALSE; // or FALSE ... la valeur de retour est ignorée ( cf doc microsoft )

			case NM_RCLICK:
				htreeview = GetDlgItem(hwndDlg, IDC_TREE_EXPLORER);
				hitem = nullptr;
				memset(&tvi, 0, sizeof(tvi));
				
				// Retrieve current selected Tree view item :
				hitem = (HTREEITEM)SendDlgItemMessage(hwndDlg, IDC_TREE_EXPLORER, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)hitem);
				if (hitem != NULL)
				{
					POINT pt,localpt;
					RECT  rect;
					TreeView_EnsureVisible(htreeview, hitem); // S'assure que l'item sélectionné courant dan sle treeview est bien visible, en faisant scroller et en ouvrant les parents si besoin ... 

					GetCursorPos(&pt);
					localpt = pt;
					ScreenToClient(hwndDlg, &localpt);
					TreeView_GetItemRect(htreeview, hitem, &rect, TRUE);

					// le cursor est il sur l'item actuellement selectionné ( et visible )?
					// Si oui alors : popup menu
					if (PtInRect(&rect, localpt))
					{
						ZeroMemory(&(tvix), sizeof(TVITEMEXA));
						tvix.mask = TVIF_TEXT| TVIF_PARAM;
						tvix.hItem = hitem;
						tvix.pszText = txt;
						tvix.cchTextMax = 32;
						TreeView_GetItem(htreeview,&tvix);

						HMENU hPopupMenu = CreatePopupMenu();
						switch ( NLOWORD(tvix.lParam) )
						{
							case EXPLORER_ITEM_ID_ENUM::ROOT_ROBOT_FOLDER:
								/*
								InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, 0, txt);
								InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, 1, "Create New Path/Trajectory");
								InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, 2, "Export All Path/Trajectory as Path Pack");
								InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, 3, "Export All Path/Trajectory as Trajectory Simple Pack");
								InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, 4, "Export All Path/Trajectory as Trajectory Full Pack");
								SetForegroundWindow(hwndDlg);
								popupmenu_choice = TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, hwndDlg, NULL);
								*/
								break;

							case EXPLORER_ITEM_ID_ENUM::ROBOT_SIMULATION:
								InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, IDPOPUPMENU_EDIT_ROBOT_SIMULATION, "Edit");
								SetForegroundWindow(hwndDlg);
								popupmenu_choice = TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, hwndDlg, NULL);
								break;

							case EXPLORER_ITEM_ID_ENUM::ROBOT_KINLIMITS:
								InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, IDPOPUPMENU_EDIT_ROBOT_KINLIMITS, "Edit");
								SetForegroundWindow(hwndDlg);
								popupmenu_choice = TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, hwndDlg, NULL);
								break;

							case EXPLORER_ITEM_ID_ENUM::ROBOT_PHYSICS:
								InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, IDPOPUPMENU_EDIT_ROBOT_PHYSICS, "Edit");
								SetForegroundWindow(hwndDlg);
								popupmenu_choice = TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, hwndDlg, NULL);
								break;

							case EXPLORER_ITEM_ID_ENUM::ROBOT_GEOMETRY:
								InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, IDPOPUPMENU_EDIT_ROBOT_GEOMETRY, "Edit");
								SetForegroundWindow(hwndDlg);
								popupmenu_choice = TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, hwndDlg, NULL);
								break;

							case EXPLORER_ITEM_ID_ENUM::ROOT_PATH_FOLDER:
								InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, IDPOPUPMENU_CREATE_NEW_WAYPOINTS_PATH_WB, "Create [Way Points] Path/Trajectory");
								InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, IDPOPUPMENU_CREATE_NEW_PLG_CHAIN_PATH_WB, "Create [Polygonal Chain] Path/Trajectory");
								InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, 0, "Import Path/Trajectory");
								InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, 0, "Export All Paths/Trajectories");
								SetForegroundWindow(hwndDlg);
								popupmenu_choice = TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, hwndDlg, NULL);
								break;

							case EXPLORER_ITEM_ID_ENUM::PATH:
								InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, IDPOPUPMENU_DELETE_PATH_WB, "Delete");
								InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, IDPOPUPMENU_EDIT_PATH_WB, "Edit");
								SetForegroundWindow(hwndDlg);
								popupmenu_choice = TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, hwndDlg, NULL);
								break;

							case EXPLORER_ITEM_ID_ENUM::TRAJECTORY_KINLIMITS:
								if (NHIWORD(tvix.lParam) == static_currentworkbench_index)
								{
									InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, IDPOPUPMENU_EDIT_TRJ_KINLIMITS, "Edit");
									SetForegroundWindow(hwndDlg);
									popupmenu_choice = TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, hwndDlg, NULL);
								}
								break;

							case EXPLORER_ITEM_ID_ENUM::TRAJECTORY_KTWEAKS:
								if (NHIWORD(tvix.lParam) == static_currentworkbench_index)
								{
									InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, IDPOPUPMENU_EDIT_TRJ_KTWEAKS, "Edit");
									SetForegroundWindow(hwndDlg);
									popupmenu_choice = TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, hwndDlg, NULL);
								}
								break;

							case EXPLORER_ITEM_ID_ENUM::TRAJECTORY_VZONES:
								if (NHIWORD(tvix.lParam) == static_currentworkbench_index)
								{
									InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, IDPOPUPMENU_EDIT_TRJ_VZONES, "Edit");
									SetForegroundWindow(hwndDlg);
									popupmenu_choice = TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, hwndDlg, NULL);
								}
								break;

							case EXPLORER_ITEM_ID_ENUM::TRAJECTORY_KEYS:
								if (NHIWORD(tvix.lParam) == static_currentworkbench_index)
								{
									InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, IDPOPUPMENU_EDIT_TRJ_KEYS, "Edit");

									InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_SEPARATOR, 0, "");
									InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING | MF_UNCHECKED, IDPOPUPMENU_EDIT_TRJ_KEYS, "Stop");
									InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING | MF_CHECKED, IDPOPUPMENU_EDIT_TRJ_KEYS, "AutoLink");
									InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_SEPARATOR, 0, "");



									SetForegroundWindow(hwndDlg);
									popupmenu_choice = TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, hwndDlg, NULL);
								}
								break;

							case EXPLORER_ITEM_ID_ENUM::TRAJECTORY_KEY:
								hitem_parent = TreeView_GetParent(htreeview, hitem);
								ZeroMemory(&(tvix_parent), sizeof(TVITEMEXA));
								tvix_parent.mask = TVIF_TEXT | TVIF_PARAM;
								tvix_parent.hItem = hitem_parent;
								tvix_parent.pszText = txt;
								tvix_parent.cchTextMax = 32;
								TreeView_GetItem(htreeview, &tvix_parent);
								if (NHIWORD(tvix_parent.lParam) == static_currentworkbench_index)
								{
									InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, IDPOPUPMENU_EDIT_TRJ_KEY_PROPERTIES, "Properties");
									SetForegroundWindow(hwndDlg);
									popupmenu_choice = TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, hwndDlg, NULL);
								}
								break;

							case EXPLORER_ITEM_ID_ENUM::PATH_STRATEGY:
								if (NHIWORD(tvix.lParam) == static_currentworkbench_index)
								{
									InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, IDPOPUPMENU_EDIT_PATH_WP_STRATEGY, "Edit");
									SetForegroundWindow(hwndDlg);
									popupmenu_choice = TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, hwndDlg, NULL);
								}
								break;

							case EXPLORER_ITEM_ID_ENUM::TRAJECTORY:
								if (NHIWORD(tvix.lParam) == static_currentworkbench_index)
								{
									InsertMenu(hPopupMenu, -1, MF_BYPOSITION | MF_STRING, IDPOPUPMENU_SAVE_TRAJECTORY_PACKS, "Export Packs");
									SetForegroundWindow(hwndDlg);
									popupmenu_choice = TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, hwndDlg, NULL);
								}
								break;

							case EXPLORER_ITEM_ID_ENUM::TRAJECTORY_MOTION_PROFILE:
								break;
						}
						DestroyMenu(hPopupMenu);

						switch (popupmenu_choice)
						{
						// Create New Path/Trajectory
						case IDPOPUPMENU_CREATE_NEW_WAYPOINTS_PATH_WB:
							// La création d'un nouveau Workbench peut entrainer une reallocation de memoire et modifier l'adresse de Project.m_pCurrentWorkbench...
							// Donc avant de créer un nouveau WorkBench ... on calcul son index dans l'array
							if(Project.m_pCurrentWorkbench)
								current_wb_index = NGetArrayIndex(&Project.m_workbenchArray, (NBYTE*)Project.m_pCurrentWorkbench);

							// Création d'un nouveau chemin, de type WayPoint
							wb_index = Project.m_workbenchArray.Size;
							pworkbench = (NLPATH_WORKBENCH*)NArrayAllocBack0(&Project.m_workbenchArray);
							pw = new(pworkbench)NLPATH_WORKBENCH;
							/*(NLPATH_WORKBENCH::ID::WB_WAYPOINTS,
																			"[Way Points Builder]",
																			FLAG_NLPATH_WORKBENCH_SMART_BUILD,
																			(NLPATH_BUILDER*)new NLPATH_BUILDER_WAYPOINTS,
																			(NLPATH*)new NLPATH,
																			(NLTRAJECTORY*)new NLTRAJECTORY,
																			(NLTRAJECTORY_PACK*)new NLTRAJECTORY_PACK,
																			(NLTRAJECTORY_BASIC_PACK*)new NLTRAJECTORY_BASIC_PACK,
																			&Project.m_driveTrainSpecifications
																		);
							*/
							pworkbench = pw;
							pworkbench->activateSmartBuild();
							pworkbench->setId(NLPATH_WORKBENCH::ID::WB_WAYPOINTS);
							pworkbench->setPathBuilder((NLPATH_BUILDER*)new NLPATH_BUILDER_WAYPOINTS, NTRUE);
							pworkbench->setPath((NLPATH*)new NLPATH, NFALSE);
							pworkbench->setTrajectory((NLTRAJECTORY*)new NLTRAJECTORY, NFALSE);
	//						pworkbench->setTrajectoryPack((NLTRAJECTORY_PACK*)new NLTRAJECTORY_PACK, NFALSE);
	//						pworkbench->setTrajectoryBasicPack((NLTRAJECTORY_BASIC_PACK*)new NLTRAJECTORY_BASIC_PACK, NFALSE);
							pworkbench->setDriveTrainSpecifications(&Project.m_driveTrainSpecifications, NFALSE);
							
							// ... Après creation du nouveau Workbench on reaafecte pCurrentWorkbenc à partir de son index calculé juste avant la création.
							if (Project.m_pCurrentWorkbench)
								Project.m_pCurrentWorkbench = (NLPATH_WORKBENCH*)NGetArrayPtr(&Project.m_workbenchArray, current_wb_index);

							NStrCopy(pworkbench->m_name,"[Way Points Builder]",32);
							hitem = InsertTreeViewPathItemHierarchy(htreeview, hitem, pworkbench, wb_index, NLPATH_WORKBENCH::ID::WB_WAYPOINTS);
							/*
							sprintf(txt, "Path N°%d [Way Points Builder]", wb_index);
							tvinsert.hParent		 = hitem;
							tvinsert.hInsertAfter = TVI_LAST;
							ZeroMemory(&(tvinsert.itemex), sizeof(TVITEMEXA));
							tvinsert.itemex.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
							tvinsert.item.iImage = 4;
							tvinsert.item.iSelectedImage = 4;
							tvinsert.item.lParam = NMAKELONG(EXPLORER_ITEM_ID_ENUM::PATH, wb_index);
							tvinsert.item.pszText = txt;// "Way Points Path";
							tvinsert.item.cchTextMax = 32;
							hitem = TreeView_InsertItem(htreeview, &tvinsert);
							*/

							_redrawTreeview(htreeview);
							break;

						case IDPOPUPMENU_CREATE_NEW_PLG_CHAIN_PATH_WB:
							// La création d'un nouveau Workbench peut entrainer une reallocation de memoire et modifier l'adresse de Project.m_pCurrentWorkbench...
							// Donc avant de créer un nouveau WorkBench ... on calcul son index dans l'array
							if (Project.m_pCurrentWorkbench)
							{
								current_wb_index = NGetArrayIndex(&Project.m_workbenchArray, (NBYTE*)Project.m_pCurrentWorkbench);
								NErrorIf(current_wb_index != static_currentworkbench_index, NERROR_INCONSISTENT_VALUES);
							}

							// Création d'un nouveau chemin, de type Polygonal
							wb_index = Project.m_workbenchArray.Size;
							pworkbench = (NLPATH_WORKBENCH*)NArrayAllocBack0(&Project.m_workbenchArray);

							pworkbench = new(pworkbench)NLPATH_WORKBENCH;
							/*
							(NLPATH_WORKBENCH::ID::WB_PLG_CHAIN,
																			"[Polygonal builder]",
																			FLAG_NLPATH_WORKBENCH_SMART_BUILD,
																			(NLPATH_BUILDER*)new NLPATH_BUILDER_POLYGONAL_CHAIN,
																			(NLPATH*)new NLPATH,
																			(NLTRAJECTORY*)new NLTRAJECTORY,
																			(NLTRAJECTORY_PACK*)new NLTRAJECTORY_PACK,
																			(NLTRAJECTORY_BASIC_PACK*)new NLTRAJECTORY_BASIC_PACK,
																			&Project.m_driveTrainSpecifications
																		);
							*/
							pworkbench->activateSmartBuild();
							pworkbench->setId(NLPATH_WORKBENCH::ID::WB_PLG_CHAIN);
							pworkbench->setPathBuilder((NLPATH_BUILDER*)new NLPATH_BUILDER_POLYGONAL_CHAIN, NTRUE);
							pworkbench->setPath((NLPATH*)new NLPATH, NFALSE);
							pworkbench->setTrajectory((NLTRAJECTORY*)new NLTRAJECTORY, NFALSE);
//							pworkbench->setTrajectoryPack((NLTRAJECTORY_PACK*)new NLTRAJECTORY_PACK, NFALSE);
//							pworkbench->setTrajectoryBasicPack((NLTRAJECTORY_BASIC_PACK*)new NLTRAJECTORY_BASIC_PACK, NFALSE);
							pworkbench->setDriveTrainSpecifications(&Project.m_driveTrainSpecifications, NFALSE);
							pworkbench->setRamseteParams(&Project.m_ramseteParams);
							// ... Après creation du nouveau Workbench on reaafecte pCurrentWorkbenc à partir de son index calculé juste avant la création.
							if (Project.m_pCurrentWorkbench)
								Project.m_pCurrentWorkbench = (NLPATH_WORKBENCH*)NGetArrayPtr(&Project.m_workbenchArray, current_wb_index);
							NStrCopy(pworkbench->m_name, "[Polygonal builder]", 32);
							hitem = InsertTreeViewPathItemHierarchy(htreeview, hitem, pworkbench, wb_index,NLPATH_WORKBENCH::ID::WB_PLG_CHAIN);
							/*
							sprintf(txt, "Path N°%d [Polygonal builder]", wb_index);
							tvinsert.hParent = hitem;
							tvinsert.hInsertAfter = TVI_LAST;
							ZeroMemory(&(tvinsert.itemex), sizeof(TVITEMEXA));
							tvinsert.itemex.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
							tvinsert.item.iImage = 4;
							tvinsert.item.iSelectedImage = 4;
							tvinsert.item.lParam = NMAKELONG(EXPLORER_ITEM_ID_ENUM::PATH, wb_index);
							tvinsert.item.pszText = txt;// "Polygonal Path [builder]";
							tvinsert.item.cchTextMax = 32;
							hitem = TreeView_InsertItem(htreeview, &tvinsert);
							*/
							_redrawTreeview(htreeview);
							break;

						case IDPOPUPMENU_DELETE_PATH_WB:
							// d'abord on recupere un pointeur sur le dossier parent:
							//hitem = (HTREEITEM)SendDlgItemMessage(hwndDlg, IDC_TREE_EXPLORER, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)hitem);
							hitem_parent = TreeView_GetParent(htreeview, hitem);
							// Delete Path:
							deleted_item_index = NHIWORD(tvix.lParam);
							if (Project.m_pCurrentWorkbench == (NLPATH_WORKBENCH*)NGetArrayPtr(&Project.m_workbenchArray, deleted_item_index))
							{
								NErrorIf(deleted_item_index != static_currentworkbench_index, NERROR_INCONSISTENT_VALUES);

								SendMessageToAllAppWindows(WM_MESSAGE_DELETE_CURRENT_PATH_WORKBENCH,(WPARAM)APP_EXPLORER_WINDOW, (LPARAM)Project.m_pCurrentWorkbench);
								Project.m_pCurrentWorkbench				= NULL;
								static_currentworkbench_index			= NVOID;
							}
							#ifdef _DEBUG
							else if (Project.m_pCurrentWorkbench)
							{
								NErrorIf(static_currentworkbench_index!= NGetArrayIndex(&Project.m_workbenchArray, (NBYTE*)Project.m_pCurrentWorkbench), NERROR_INCONSISTENT_VALUES);
							}
							#endif			

							// Delete le path workbench de l'array des workbench
							NEraseArrayElement(&Project.m_workbenchArray, deleted_item_index, DeletePathWorkbenchComponents );

							// Le TreeView
							TreeView_DeleteItem(htreeview, hitem);
							
							// Update des index de PathBuilder stockés dans les Items du TreeView
							hitem = TreeView_GetChild(htreeview, hitem_parent);
							ZeroMemory(&(tvix), sizeof(TVITEMEXA));
							tvix.mask = TVIF_PARAM;
							while (hitem)
							{
								tvix.hItem = hitem;
								TreeView_GetItem(htreeview, &tvix);
								item_id		= NLOWORD(tvix.lParam); // Le type: un folder robot ? une path ?
								item_index	= NHIWORD(tvix.lParam);	// L'index, de l'élément associé dans sa liste. Par exemple, l'index du Path_WorkBench dans l'Array des PathWorkBenchs

								if (item_index > deleted_item_index)
								{
									tvix.lParam = NMAKELONG(item_id, (item_index - 1));
									TreeView_SetItem(htreeview, &tvix);
								}
								hitem = TreeView_GetNextSibling(htreeview, hitem);
							}

							// ... Après destruction du Workbench on reaafecte Project.m_pCurrentWorkbench
							// à partir de son index calculé juste avant la création et potentielement décalé de 1 suite à la suppression.
							if (Project.m_pCurrentWorkbench)
							{
								if (static_currentworkbench_index > deleted_item_index)
								{
									static_currentworkbench_index -= 1;
									Project.m_pCurrentWorkbench = (NLPATH_WORKBENCH*)NGetArrayPtr(&Project.m_workbenchArray, static_currentworkbench_index);
									SendMessageToAllAppWindows(WM_MESSAGE_UPDATE_CURRENT_PATH_WORKBENCH_PTR, (WPARAM)APP_EXPLORER_WINDOW, (LPARAM)Project.m_pCurrentWorkbench);
								}
							}
							_redrawTreeview(htreeview);
							break;

						case IDPOPUPMENU_EDIT_PATH_WB:
							// Update de l'apparence du TreeView:
							// Le TreeView item représentant le PATH/TRAJ. avtuellement en cours d'édition apparait avec une icone specifique:
							if (static_hitem_CurrentEdited != tvix.hItem)
							{
								ZeroMemory(&(tvi_update_apparence), sizeof(TVITEMEXA));
								tvi_update_apparence.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

								// D'abord on reset l'icone du current Edited
								tvi_update_apparence.hItem = static_hitem_CurrentEdited;
								tvi_update_apparence.iImage = 4;
								tvi_update_apparence.iSelectedImage = 4;
								TreeView_SetItem(htreeview, &tvi_update_apparence);
								
								// Ensuite on change l'icone du nouveau current edited
								tvi_update_apparence.hItem = tvix.hItem;
								tvi_update_apparence.iImage = 5;
								tvi_update_apparence.iSelectedImage = 5;
								TreeView_SetItem(htreeview, &tvi_update_apparence);

								static_hitem_CurrentEdited = tvix.hItem;
							}

							wb_index = NHIWORD(tvix.lParam);
							pwb = (NLPATH_WORKBENCH*)NGetArrayPtr(&Project.m_workbenchArray, wb_index);

							if (Project.m_pCurrentWorkbench != pwb)
							{
								NErrorIf(static_currentworkbench_index == wb_index, NERROR_INCONSISTENT_VALUES);
								
								Project.m_pCurrentWorkbench				= pwb;			
								static_currentworkbench_index			= wb_index;
								/*

								// transféré dans "WndProcPathBuilder.cpp"

								switch (Project.m_pCurrentWorkbench->getId())
								{
								case NLPATH_WORKBENCH::ID::WB_WAYPOINTS:
									WayPointsToolData.m_pWorkbench = Project.m_pCurrentWorkbench;
									PolygonalChainToolData.m_pWorkbench = NULL;
									break;
								case NLPATH_WORKBENCH::ID::WB_PLG_CHAIN:
									WayPointsToolData.m_pWorkbench = NULL;
									PolygonalChainToolData.m_pWorkbench = Project.m_pCurrentWorkbench;
									break;
								default:
									WayPointsToolData.m_pWorkbench = NULL;
									PolygonalChainToolData.m_pWorkbench = NULL;
									break;
								}
								*/
								// On prévient toutes les fenêtres concernées
								SendMessageToAllAppWindows(WM_MESSAGE_SET_CURRENT_PATH_WORKBENCH, (WPARAM)APP_EXPLORER_WINDOW, (LPARAM)Project.m_pCurrentWorkbench);
							}
							#ifdef _DEBUG
							else
							{
								NErrorIf(static_currentworkbench_index != wb_index, NERROR_INCONSISTENT_VALUES);
							}
							#endif

							// Faut-il créer la DialogBox PathBuilder ?
							if ( AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TOOLS] == NULL)
							{
								// Creation boite de dialogue additionnelle
								AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TOOLS] = CreateDialog(GetWindowInstance(hwndDlg), MAKEINTRESOURCE(IDD_DIALOG_PATHBUILDER), hwndDlg, (DLGPROC)WndProc_PathBuilder);
								SendMessage(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TOOLS], WM_MESSAGE_SET_CURRENT_PATH_WORKBENCH, (WPARAM)APP_EXPLORER_WINDOW, (LPARAM)Project.m_pCurrentWorkbench);
								ShowWindow( AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TOOLS] , SW_SHOW );
								UpdateWindow(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TOOLS]);
							}

							break;

							
						case IDPOPUPMENU_EDIT_PATH_WP_STRATEGY:
							// Faut-il créer la DialogBox Path Strategy ?
							if (AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_WP_BUILD_STRATEGY] == NULL)
							{
								// Creation boite de dialogue additionnelle
								AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_WP_BUILD_STRATEGY] = CreateDialog(GetWindowInstance(hwndDlg), MAKEINTRESOURCE(IDD_DIALOG_PATHBUILDER_WAY_POINTS_STRATEGY), hwndDlg, (DLGPROC)WndProc_PathBuilder_WayPointsStrategy);
								SendMessage(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_WP_BUILD_STRATEGY], WM_MESSAGE_SET_CURRENT_PATH_WORKBENCH, (WPARAM)APP_EXPLORER_WINDOW, (LPARAM)Project.m_pCurrentWorkbench);
								ShowWindow(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_WP_BUILD_STRATEGY], SW_SHOW);
								UpdateWindow(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_WP_BUILD_STRATEGY]);
							}
							// Sinon, Rien à faire ... la fenêtre est déjà en place et donc à jour...
							break;


						case IDPOPUPMENU_EDIT_TRJ_KINLIMITS:
							// Faut-il créer la DialogBox Path Kin Limits ?
							if (AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_KINLIMITS] == NULL)
							{
								// Creation boite de dialogue additionnelle
								AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_KINLIMITS] = CreateDialog(GetWindowInstance(hwndDlg), MAKEINTRESOURCE(IDD_DIALOG_PATH_KINLIMITS), hwndDlg, (DLGPROC)WndProc_PathKinLimits);
								SendMessage(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_KINLIMITS], WM_MESSAGE_SET_CURRENT_PATH_WORKBENCH, (WPARAM)APP_EXPLORER_WINDOW, (LPARAM)Project.m_pCurrentWorkbench);
								ShowWindow(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_KINLIMITS], SW_SHOW);
								UpdateWindow(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_KINLIMITS]);
							}
							// Sinon, Rien à faire ... la fenêtre est déjà en place et donc à jour...
							break;

						case IDPOPUPMENU_EDIT_TRJ_KTWEAKS:
							// Faut-il créer la DialogBox Path Kin Tweaks ?
							if (AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_KTWEAKS] == NULL)
							{
								// Creation boite de dialogue additionnelle
								AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_KTWEAKS] = CreateDialog(GetWindowInstance(hwndDlg), MAKEINTRESOURCE(IDD_DIALOG_KTWEAKS), hwndDlg, (DLGPROC)WndProc_TrajectoryKinematicLimitTweaks);
								SendMessage(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_KTWEAKS], WM_MESSAGE_SET_CURRENT_PATH_WORKBENCH, (WPARAM)APP_EXPLORER_WINDOW, (LPARAM)Project.m_pCurrentWorkbench);
								ShowWindow(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_KTWEAKS], SW_SHOW);
								UpdateWindow(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_KTWEAKS]);
							}
							// Sinon, Rien à faire ... la fenêtre est déjà en place et donc à jour...
							break;

						case IDPOPUPMENU_EDIT_TRJ_VZONES:
							// Faut-il créer la DialogBox Trajectory VZONES ?
							if (AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_USERVZONE_TOOLS] == NULL)
							{
								// Creation boite de dialogue additionnelle
								AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_USERVZONE_TOOLS] = CreateDialog(GetWindowInstance(hwndDlg), MAKEINTRESOURCE(IDD_DIALOG_TRAJECTORY_USERVZONE), hwndDlg, (DLGPROC)WndProc_TrajectoryUserVZones);
								SendMessage(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_USERVZONE_TOOLS], WM_MESSAGE_SET_CURRENT_PATH_WORKBENCH, (WPARAM)APP_EXPLORER_WINDOW, (LPARAM)Project.m_pCurrentWorkbench);
								ShowWindow(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_USERVZONE_TOOLS], SW_SHOW);
								UpdateWindow(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_USERVZONE_TOOLS]);
							}
							// Sinon, Rien à faire ... la fenêtre est déjà en place et donc à jour...
							break;

						case IDPOPUPMENU_EDIT_TRJ_KEYS:
							// Faut-il créer la DialogBox Trajectory KEYS ?
							if (AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_KEY_TOOLS] == NULL)
							{
								// Creation boite de dialogue additionnelle
								AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_KEY_TOOLS] = CreateDialog(GetWindowInstance(hwndDlg), MAKEINTRESOURCE(IDD_DIALOG_TRAJECTORY_KEY), hwndDlg, (DLGPROC)WndProc_TrajectoryKeys);
								SendMessage(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_KEY_TOOLS], WM_MESSAGE_SET_CURRENT_PATH_WORKBENCH, (WPARAM)APP_EXPLORER_WINDOW, (LPARAM)Project.m_pCurrentWorkbench);
								
								ShowWindow(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_KEY_TOOLS], SW_SHOW);
								SetFocus(hwndDlg);
								UpdateWindow(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_KEY_TOOLS]);

							}
							// Sinon, Rien à faire ... la fenêtre est déjà en place et donc à jour...
							break;

						case IDPOPUPMENU_EDIT_TRJ_KEY_PROPERTIES:
							// Faut-il créer la DialogBox Trajectory KEY PROPERTIES ?
							if (AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_KEY_PROPERTIES] == NULL)
							{
								// Creation boite de dialogue additionnelle
								AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_KEY_PROPERTIES] = CreateDialog(GetWindowInstance(hwndDlg), MAKEINTRESOURCE(IDD_DIALOG_TRAJECTORY_KEY_PROPERTIES), hwndDlg, (DLGPROC)WndProc_TrajectoryKeyProperties);
								SendMessage(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_KEY_PROPERTIES], WM_MESSAGE_SET_CURRENT_PATH_WORKBENCH, (WPARAM)APP_EXPLORER_WINDOW, (LPARAM)Project.m_pCurrentWorkbench);
								SendMessage(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_KEY_PROPERTIES], WM_MESSAGE_SET_CURRENT_SELECTED_TRJKEY, (WPARAM)APP_EXPLORER_WINDOW, (LPARAM)static_currentTrajectoryKey_index);
								ShowWindow(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_KEY_PROPERTIES], SW_SHOW);
								UpdateWindow(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_KEY_PROPERTIES]);
							}
							// Sinon, Rien à faire ... la fenêtre est déjà en place et donc à jour...
							break;



						case IDPOPUPMENU_EDIT_ROBOT_SIMULATION:
							// Faut-il créer la DialogBox Robot SIMULATION  ?
							if (AppWindowsTable[APP_WINDOW_ENUM::RB_WIN_SIMULATION] == NULL)
							{
								// Creation boite de dialogue additionnelle
								AppWindowsTable[APP_WINDOW_ENUM::RB_WIN_SIMULATION] = CreateDialog(GetWindowInstance(hwndDlg), MAKEINTRESOURCE(IDD_DIALOG_ROBOT_SIMULATION), hwndDlg, (DLGPROC)WndProc_RobotSimulation);
								SendMessage(AppWindowsTable[APP_WINDOW_ENUM::RB_WIN_SIMULATION], WM_MESSAGE_SET_CURRENT_PATH_WORKBENCH, (WPARAM)APP_EXPLORER_WINDOW, (LPARAM)Project.m_pCurrentWorkbench);
								ShowWindow(AppWindowsTable[APP_WINDOW_ENUM::RB_WIN_SIMULATION], SW_SHOW);
								UpdateWindow(AppWindowsTable[APP_WINDOW_ENUM::RB_WIN_SIMULATION]);
							}
							break;

						case IDPOPUPMENU_EDIT_ROBOT_KINLIMITS:
							// Faut-il créer la DialogBox Robot Kin Limits  ?
							if (AppWindowsTable[APP_WINDOW_ENUM::RB_WIN_KINLIMITS] == NULL)
							{
								// Creation boite de dialogue additionnelle
								AppWindowsTable[APP_WINDOW_ENUM::RB_WIN_KINLIMITS] = CreateDialog(GetWindowInstance(hwndDlg), MAKEINTRESOURCE(IDD_DIALOG_ROBOT_KINLIMITS), hwndDlg, (DLGPROC)WndProc_RobotKinLimits);
								SendMessage(AppWindowsTable[APP_WINDOW_ENUM::RB_WIN_KINLIMITS], WM_MESSAGE_SET_CURRENT_SPECS,(WPARAM)APP_EXPLORER_WINDOW, (LPARAM)&Project.m_driveTrainSpecifications);
								ShowWindow(AppWindowsTable[APP_WINDOW_ENUM::RB_WIN_KINLIMITS] , SW_SHOW);
								UpdateWindow(AppWindowsTable[APP_WINDOW_ENUM::RB_WIN_KINLIMITS]);
							}
							break;

						case IDPOPUPMENU_EDIT_ROBOT_PHYSICS:
							// Faut-il créer la DialogBox Robot Physics  ?
							if (AppWindowsTable[APP_WINDOW_ENUM::RB_WIN_PHYSICS] == NULL)
							{
								// Creation boite de dialogue additionnelle
								AppWindowsTable[APP_WINDOW_ENUM::RB_WIN_PHYSICS] = CreateDialog(GetWindowInstance(hwndDlg), MAKEINTRESOURCE(IDD_DIALOG_ROBOT_PHYSICS), hwndDlg, (DLGPROC)WndProc_RobotPhysics);
								SendMessage(AppWindowsTable[APP_WINDOW_ENUM::RB_WIN_PHYSICS], WM_MESSAGE_SET_CURRENT_SPECS, (WPARAM)APP_EXPLORER_WINDOW, (LPARAM)&Project.m_driveTrainSpecifications);
								ShowWindow(AppWindowsTable[APP_WINDOW_ENUM::RB_WIN_PHYSICS], SW_SHOW);
								UpdateWindow(AppWindowsTable[APP_WINDOW_ENUM::RB_WIN_PHYSICS]);
							}
							break;

						case IDPOPUPMENU_EDIT_ROBOT_GEOMETRY:
							// Faut-il créer la DialogBox Robot Physics  ?
							if (AppWindowsTable[APP_WINDOW_ENUM::RB_WIN_GEOMETRY] == NULL)
							{
								// Creation boite de dialogue additionnelle
								AppWindowsTable[APP_WINDOW_ENUM::RB_WIN_GEOMETRY] = CreateDialog(GetWindowInstance(hwndDlg), MAKEINTRESOURCE(IDD_DIALOG_ROBOT_GEOMETRY), hwndDlg, (DLGPROC)WndProc_RobotGeometry);
								SendMessage(AppWindowsTable[APP_WINDOW_ENUM::RB_WIN_GEOMETRY], WM_MESSAGE_SET_CURRENT_SPECS, (WPARAM)APP_EXPLORER_WINDOW, (LPARAM)&Project.m_driveTrainSpecifications);
								ShowWindow(AppWindowsTable[APP_WINDOW_ENUM::RB_WIN_GEOMETRY], SW_SHOW);
								UpdateWindow(AppWindowsTable[APP_WINDOW_ENUM::RB_WIN_GEOMETRY]);
							}
							break;

						case IDPOPUPMENU_SAVE_TRAJECTORY_PACKS:
							if (Project.m_pCurrentWorkbench)
							{
								char			filename[MAX_PATH];
								char			filename2[MAX_PATH];

								OPENFILENAME	ofn;
								ZeroMemory(&filename, sizeof(filename));
								ZeroMemory(&ofn, sizeof(ofn));
								ofn.lStructSize = sizeof(ofn);
								ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
								//ofn.lpstrFilter = "TrajectoryStatePack Files\0*.sp0\0*.sp1\0\0";
								ofn.lpstrFilter = "TrajectoryPack Files\0*.pak\0\0";
								ofn.lpstrFile = filename;
								ofn.nMaxFile = MAX_PATH;
								ofn.lpstrTitle = "choose a name for Trajectory Packs File";
								ofn.Flags = OFN_DONTADDTORECENT;
								if (GetSaveFileNameA(&ofn))
								{
									sprintf(filename2, "%s.pak", filename);
									Project.m_pCurrentWorkbench->m_pTrajectory->getPack()->save(filename2);

/*
									sprintf(filename2, "%s.sp0", filename);
									FILE* pfile = fopen(filename2, "wb");
									Project.m_pCurrentWorkbench->m_pTrajectoryStateSPack->write(pfile);
									fclose(pfile);

									sprintf(filename2, "%s.sp1", filename);
									pfile = fopen(filename2, "wb");
									Project.m_pCurrentWorkbench->m_pTrajectoryStatePack->write(pfile);
									fclose(pfile);
*/
								}
							}
							break;

						default:
							break;
						}
					}



					_redrawTreeview(htreeview);

				}
				break;

			case TVN_BEGINLABELEDIT:
				ptvdisplay = (LPNMTVDISPINFO)lParam;
				
				if (NLOWORD(ptvdisplay->item.lParam) == EXPLORER_ITEM_ID_ENUM::PATH)
					return 0; // Est censé canceller l'edition mais ne marche pas .... ???
				else
					return NVOID;

				break;

			case TVN_ENDLABELEDIT:
				ptvdisplay = (LPNMTVDISPINFO)lParam;

				if ( ( NLOWORD(ptvdisplay->item.lParam) == EXPLORER_ITEM_ID_ENUM::PATH ) && ( ptvdisplay->item.pszText ) )
				{
					pworkbench = (NLPATH_WORKBENCH*)NGetArrayPtr(&Project.m_workbenchArray, NHIWORD(ptvdisplay->item.lParam));
					NStrCopy(pworkbench->m_name, ptvdisplay->item.pszText, NLPATH_WORKBENCH_NAME_SIZE);
					ptvdisplay->item.pszText = pworkbench->m_name;
					ptvdisplay->item.cchTextMax = NLPATH_WORKBENCH_NAME_SIZE;
					SendMessage(htreeview, TVM_SETITEM, 0, (LPARAM)&ptvdisplay->item);
					return TRUE;
				}
				else
				{
					return 0;
				}
				break;


			}
			return FALSE;
		default:
			return FALSE;
		}



		return FALSE;

		
	case WM_MESSAGE_APP_WINDOW_CLOSED:
		NErrorIf(AppWindowsTable[(APP_WINDOW_ENUM)wParam] != (HWND)lParam, NERROR_INCONSISTENT_VALUES);
		AppWindowsTable[(APP_WINDOW_ENUM)wParam] = nullptr;
		return TRUE;

	case WM_MESSAGE_SPECS_KINLIMITS_UPDATED:
		NErrorIf((NLDRIVETRAINSPECS*)lParam != &Project.m_driveTrainSpecifications, NERROR_INCONSISTENT_PARAMETERS);	// Not an error, mais plus tard ...
																											// si on veut gérer plusieurs Robots en même temps dans le logiciel ...
																											// il faudra remplacer l'UNIQUE 'Project.m_driveTrainSpecifications'	par une liste ...
		// En réponse:
		// 1) on prévient toutes les fenêtres WorkBench valides:
		SendMessageToAllAppWindows(WM_MESSAGE_SPECS_KINLIMITS_UPDATED, APP_EXPLORER_WINDOW, lParam);
		// 2) on demande le recalcul de toutes les trajectoires associées 
		pworkbench = (NLPATH_WORKBENCH*)Project.m_workbenchArray.pFirst;
		for (i = 0; i < Project.m_workbenchArray.Size; i++,pworkbench++)
		{
			if (pworkbench->m_pDriveTrainSpecifications == (NLDRIVETRAINSPECS*)lParam)
				pworkbench->requestTrajectoryBuild();
		}
		return TRUE;

	case WM_MESSAGE_SPECS_PHYSICS_UPDATED:
		NErrorIf((NLDRIVETRAINSPECS*)lParam != &Project.m_driveTrainSpecifications, NERROR_INCONSISTENT_PARAMETERS);	// Not an error, mais plus tard ...
																											// si on veut gérer plusieurs Robots en même temps dans le logiciel ...
																											// il faudra remplacer l'UNIQUE 'Project.m_driveTrainSpecifications'	par une liste ...
		// En réponse:
		// 1) on prévient toutes les fenêtres WorkBench valides:
		SendMessageToAllAppWindows(WM_MESSAGE_SPECS_PHYSICS_UPDATED, APP_EXPLORER_WINDOW, lParam);
		// 2) on demande le recalcul de toutes les trajectoires associées 
		pworkbench = (NLPATH_WORKBENCH*)Project.m_workbenchArray.pFirst;
		for (i = 0; i < Project.m_workbenchArray.Size; i++, pworkbench++)
		{
			if (pworkbench->m_pDriveTrainSpecifications == (NLDRIVETRAINSPECS*)lParam)
				pworkbench->requestTrajectoryBuild();
		}
		return TRUE;

	case WM_MESSAGE_REFRESH_PROJECT_TREEVIEW:
		if (Project.m_pCurrentWorkbench)
		{
			SendMessageToAllAppWindows(WM_MESSAGE_DELETE_CURRENT_PATH_WORKBENCH, (WPARAM)APP_EXPLORER_WINDOW, (LPARAM)Project.m_pCurrentWorkbench);
			Project.m_pCurrentWorkbench = NULL;
			static_currentworkbench_index = NVOID;
		}

		RefreshTreeView(htreeview, &Project);

		_redrawTreeview(htreeview);
		return TRUE;

	// TRAJECTORY KEY
	case WM_MESSAGE_KEY_INSERTED_AND_SELECTED:
		if (Project.m_pCurrentWorkbench == (NLPATH_WORKBENCH*)wParam)
		{
			static_currentTrajectoryKey_index = lParam;
			TreeView_SelectItem(htreeview, InsertTreeViewItemKey(htreeview, Project.m_pCurrentWorkbench, static_currentTrajectoryKey_index));
			UpdateTreeViewItemKeyParamIds(htreeview, Project.m_pCurrentWorkbench);
			_redrawTreeview(htreeview);
			SendMessage(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_KEY_PROPERTIES], WM_MESSAGE_SET_CURRENT_SELECTED_TRJKEY, (WPARAM)APP_EXPLORER_WINDOW, (LPARAM)static_currentTrajectoryKey_index);


		}
		return TRUE;

	case WM_MESSAGE_KEY_DELETED:
		if (Project.m_pCurrentWorkbench == (NLPATH_WORKBENCH*)wParam)
		{
			NErrorIf(static_currentTrajectoryKey_index != lParam, NERROR_INCONSISTENT_VALUES);
			static_currentTrajectoryKey_index = NVOID;
			DeleteTreeViewItemKey(htreeview, Project.m_pCurrentWorkbench, lParam);
			UpdateTreeViewItemKeyParamIds(htreeview, Project.m_pCurrentWorkbench);
			_redrawTreeview(htreeview);

			SendMessage(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_KEY_PROPERTIES], WM_MESSAGE_SET_CURRENT_SELECTED_TRJKEY, (WPARAM)APP_EXPLORER_WINDOW, (LPARAM)static_currentTrajectoryKey_index);
		}
		return TRUE;

	case WM_MESSAGE_KEY_SELECTED:
		if (Project.m_pCurrentWorkbench == (NLPATH_WORKBENCH*)wParam)
		{
			if (static_currentTrajectoryKey_index != lParam)
			{
				static_currentTrajectoryKey_index = lParam;
				TreeView_SelectItem(htreeview, GetTreeViewItemKey(htreeview, Project.m_pCurrentWorkbench, static_currentTrajectoryKey_index));
				UpdateTreeViewItemKeyParamIds(htreeview, Project.m_pCurrentWorkbench);
				_redrawTreeview(htreeview);

				SendMessage(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_KEY_PROPERTIES], WM_MESSAGE_SET_CURRENT_SELECTED_TRJKEY, (WPARAM)APP_EXPLORER_WINDOW, (LPARAM)static_currentTrajectoryKey_index);
			}
		}
		return TRUE;

	case WM_MESSAGE_KEY_MOVED:
		if (Project.m_pCurrentWorkbench == (NLPATH_WORKBENCH*)wParam)
		{
			//NErrorIf( static_currentTrajectoryKey_index != lParam, NERROR_INCONSISTENT_VALUES);
			//TreeView_SelectItem(htreeview, MoveTreeViewItemKey(htreeview, Project.m_pCurrentWorkbench, static_currentTrajectoryKey_index));
			/*
			id1 = NLOWORD(lParam);
			id0 = NHIWORD(lParam);
			if (id0 != id1)
			{
				hitem0 = GetTreeViewItemKey(htreeview, Project.m_pCurrentWorkbench, id0);
				TreeView_DeleteItem(htreeview, hitem0);

				hitem = InsertTreeViewItemKey(htreeview, pwb, id1);
				TreeView_SelectItem(htreeview, hitem);
			}
			*/
			UpdateTreeViewItemKeyParamIds(htreeview, Project.m_pCurrentWorkbench);
			_redrawTreeview(htreeview);
			SendMessage(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_KEY_PROPERTIES], WM_MESSAGE_UPDATE_CURRENT_SELECTED_TRJKEY_DATA, (WPARAM)APP_EXPLORER_WINDOW, (LPARAM)static_currentTrajectoryKey_index);

		}
		return TRUE;

	case WM_MESSAGE_TRAJECTORY_REBUILDED:
		SendMessage(AppWindowsTable[APP_WINDOW_ENUM::WB_WIN_TRAJECTORY_KEY_PROPERTIES], WM_MESSAGE_TRAJECTORY_REBUILDED, wParam, lParam);
		return TRUE;
	}

	

	return FALSE;

}


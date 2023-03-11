#include <iostream>
#include <windows.h>
#include <CommCtrl.h>
#include "../../resource.h"
// System
#include "../N/NCore.h"
#include "../N/NTexture.h"
#include "../N/NEditor.h"
#include "../N/NObject.h"
#include "../NL/NLProject.h"
#include "../NL/MotionControl/NLKin.h"
#include "../App/App_Init.h"

// Tools
#include "App_WayPoints_Tools.h"
// Dialog Boxes
#include "WndProcPathBuilderWayPoints.h"
#include "WndProcPathBuilderPolygonalChain.h"
#include "./Windows/WndProcPlayground.h"
#include "WndProcDashboard.h"
#include "WndProcExplorer.h"
#include "./Windows/WndProcGlobalSettings.h"

#include "WndProcMain.h"



extern NLPROJECT			Project;

//extern NXNODELIST			MainXlist;
//extern NOBJECT				PlaygroundObject;;

// Dialog Boxes Handles:
extern HWND					hwnd_PathBuilder_WayPoints;			// [Defini dans WndProcPathBuild.cpp]
extern HWND					hwnd_Playground;					// [Defini dans WndProcPlayground.cpp]
extern HWND					hwnd_Dashboard;			// [Defini dans WndProcLayout.cpp]
extern HWND					hwnd_PathBuilder_PolygonalChain;	// [Defini dans WndProcPathBuilderPolygonalChain.cpp]
extern HWND					hwnd_Explorer;						// [Defini dans WndProcExplorer.cpp]	
extern HWND					hwnd_GlobalSettings;				// [Defini dans WndProcGlobalSettings.cpp]	

extern HWND					AppWindowsTable[];
extern NARRAY				UpToDateActionMessagesLUT;

LRESULT CALLBACK WndProc_Main(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	OPENFILENAME ofn;
	static HGLRC OpenGLRenderingContext;


	switch (message)
	{
	case WM_CREATE:
	{
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			/*PFD_DRAW_TO_WINDOW | */PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_SWAP_EXCHANGE,    //Flags
			PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
			32,                        //Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,                       //Number of bits for the depthbuffer
			8,                        //Number of bits for the stencilbuffer
			0,                        //Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		HDC ourWindowHandleToDeviceContext = GetDC(hWnd);

		int  letWindowsChooseThisPixelFormat;
		letWindowsChooseThisPixelFormat = ChoosePixelFormat(ourWindowHandleToDeviceContext, &pfd);
		SetPixelFormat(ourWindowHandleToDeviceContext, letWindowsChooseThisPixelFormat, &pfd);

		OpenGLRenderingContext = wglCreateContext(ourWindowHandleToDeviceContext);
		wglMakeCurrent(ourWindowHandleToDeviceContext, OpenGLRenderingContext);
		//MessageBoxA(0,(char*)glGetString(GL_VERSION), "OPENGL VERSION",0);

		//wglDeleteContext(ourOpenGLRenderingContext);
		//PostQuitMessage(0);

		RECT	rect;
		GetWindowRect(hWnd, &rect);
		Ngl_Init();
	}
	break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDM_ROBOT:
			break;

		case IDM_PATHBUILDER_WAYPOINTS:
			if (hwnd_PathBuilder_WayPoints == NULL)
			{
				// Creation boite de dialogue additionnelle
				hwnd_PathBuilder_WayPoints = CreateDialog(GetWindowInstance(hWnd), MAKEINTRESOURCE(IDD_DIALOG_PATHBUILDER_WAY_POINTS), hWnd, (DLGPROC)WndProc_PathBuilder_WayPoints);
				ShowWindow(hwnd_PathBuilder_WayPoints, SW_SHOW);
			}
			break;

		case IDM_PATHBUILDER_POLYGONAL_CHAIN:
			if (hwnd_PathBuilder_PolygonalChain == NULL)
			{
				// Creation boite de dialogue additionnelle
				hwnd_PathBuilder_PolygonalChain = CreateDialog(GetWindowInstance(hWnd), MAKEINTRESOURCE(IDD_DIALOG_PATHBUILDER_POLYGONAL_CHAIN), hWnd, (DLGPROC)WndProc_PathBuilder_PolygonalChain);
				ShowWindow(hwnd_PathBuilder_PolygonalChain, SW_SHOW);
			}
			break;

		case IDM_PLAYGROUND:
			if (hwnd_Playground == NULL)
			{
				// Creation boite de dialogue additionnelle
				hwnd_Playground = CreateDialog(GetWindowInstance(hWnd), MAKEINTRESOURCE(IDD_DIALOG_PLAYGROUND), hWnd, (DLGPROC)WndProc_Playground);
				ShowWindow(hwnd_Playground, SW_SHOW);
			}
			break;

		case IDM_DASHBOARD:
			if (hwnd_Dashboard == NULL)
			{
				// Creation boite de dialogue additionnelle
				hwnd_Dashboard = CreateDialog(GetWindowInstance(hWnd), MAKEINTRESOURCE(IDD_DIALOG_LAYOUT), hWnd, (DLGPROC)WndProc_Dashboard);
				ShowWindow(hwnd_Dashboard, SW_SHOW);
			}
			break;

		case IDM_EXPLORER:
			if (hwnd_Explorer == NULL)
			{
				// Creation boite de dialogue additionnelle
				hwnd_Explorer = CreateDialog(GetWindowInstance(hWnd), MAKEINTRESOURCE(IDD_DIALOG_EXPLORER), hWnd, (DLGPROC)WndProc_Explorer);
				SendMessage(hwnd_Explorer, WM_MESSAGE_REFRESH_PROJECT_TREEVIEW, 0, 0);
				ShowWindow(hwnd_Explorer, SW_SHOW);
			}
			break;

		case IDM_GLOBALSETTINGS:
			/*
			if (hwnd_GlobalSettings == NULL)
			{
				// Creation boite de dialogue additionnelle
				hwnd_GlobalSettings = CreateDialog(GetWindowInstance(hWnd), MAKEINTRESOURCE(IDD_DIALOG_GLOBAL_SETTINGS), hWnd, (DLGPROC)WndProc_GlobalSettings);
				ShowWindow(hwnd_GlobalSettings, SW_SHOW);
			}
			replace by
			*/
			if (AppWindowsTable[APP_WINDOW_ENUM::APP_WIN_GLOBALSETTINGS] == NULL)
			{
				// Creation boite de dialogue additionnelle
				AppWindowsTable[APP_WINDOW_ENUM::APP_WIN_GLOBALSETTINGS] = CreateDialog(GetWindowInstance(hWnd), MAKEINTRESOURCE(IDD_DIALOG_GLOBAL_SETTINGS), hWnd, (DLGPROC)WndProc_GlobalSettings);
				SendMessage(AppWindowsTable[APP_WINDOW_ENUM::APP_WIN_GLOBALSETTINGS], WM_MESSAGE_SET_CURRENT_PATH_WORKBENCH, (WPARAM)APP_EXPLORER_WINDOW, (LPARAM)Project.m_pCurrentWorkbench);
				ShowWindow(AppWindowsTable[APP_WINDOW_ENUM::APP_WIN_GLOBALSETTINGS], SW_SHOW);
				UpdateWindow(AppWindowsTable[APP_WINDOW_ENUM::APP_WIN_GLOBALSETTINGS]);
			}



			break;

		case IDM_FILE_OPENPLAYGROUND:
			char playgroundfilename[MAX_PATH];
			ZeroMemory(&playgroundfilename, sizeof(playgroundfilename));
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
			ofn.lpstrFilter = "TGA Files\0*.tga\0\0";
			ofn.lpstrFile = playgroundfilename;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Select a TGA File for Playground.";
			ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
			if (GetOpenFileNameA(&ofn))
			{
				NSwitchToFullPathFileSystem();
				Project.m_playground.setTexture(NCreateTextureFromTga("playground", playgroundfilename),NTRUE);
				NSwitchToRootRelativePathFileSystem();
			}
			break;

		case IDM_FILE_SAVEPROJECT:
			char savefilename[MAX_PATH];
			ZeroMemory(&savefilename, sizeof(savefilename));
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
			ofn.lpstrFilter = "RBL Files\0*.rbl\0\0";
			ofn.lpstrFile = savefilename;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Select a Project File.";
			ofn.Flags = 0/*OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST*/;
			if (GetSaveFileNameA(&ofn))
			{
				Project.save(savefilename);
			}
			/*
			ZeroMemory(&filename, sizeof(filename));
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
			ofn.lpstrFilter = "TrajectoryStatePack Files\0*.tsp\0\0";
			ofn.lpstrFile = filename;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Select a Trajectory State Pack File";
			ofn.Flags = OFN_DONTADDTORECENT;
			if (GetSaveFileNameA(&ofn))
			{
				FILE *pfile = fopen(filename, "wb");
				TrajectoryStateSPack.write(pfile);
				fclose(pfile);
			}
			*/
			break;

		case IDM_FILE_OPENPROJECT:
			char openfilename[MAX_PATH];
			ZeroMemory(&openfilename, sizeof(openfilename));
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
			ofn.lpstrFilter = "RBL Files\0*.rbl\0\0";
			ofn.lpstrFile = openfilename;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Select a Project File.";
			ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
			if (GetOpenFileNameA(&ofn))
			{
				Project.close();
				Project.open(openfilename);

				// MISE A JOUR POTENTIELLE DE LA LUT DES MESSAGES:
				if (!Project.isActionMessagesLutUpToDate(&UpToDateActionMessagesLUT))
				{
					if ( MessageBox(hWnd, "Le dictionnaire de messages du projet diffère de celui du logiciel. \nOUI pour mettre à jour le dictionnaire du projet.\nNON pour le conserver.",
						 "Différentes Versions du Dictionnaire de Messages.",
						 MB_YESNO | MB_APPLMODAL)
						 == IDOK
						)
					{
						Project.UpdateActionMessagesLut(&UpToDateActionMessagesLUT);
					}
				}


				SendMessage(hwnd_Explorer, WM_MESSAGE_REFRESH_PROJECT_TREEVIEW, 0, 0);
			}
			break;



		default:
			break;
		}
	}
	break;

	case WM_DESTROY:
	{
		wglDeleteContext(OpenGLRenderingContext);
	}
	break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;

}


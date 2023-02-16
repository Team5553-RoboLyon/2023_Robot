// ***************************************************************************************
// ***************************************************************************************
// **																					**
// **								Main.cpp											**
// **																					**
// ***************************************************************************************
// ***************************************************************************************

#ifdef _NWINDOWS    
//#include <SDL.h>
//#include <new>
#include <Windows.h>
#include <CommCtrl.h>
#include "../../resource.h"
//#include "winres.h"
#include "../N/NCStandard.h"
#include "../N/GL/Ngl.h"
#include "../N/NCore.h"
#include "../N/File/NFile.h"
#include "../N/NViewport.h"
#include "../N/Audio/NAudio.h"
#include "../N/Core/NTime.h"
#include "../N/Event/NEvent.h"
#include "../N/NCamera.h"

#include "../N/Utilities/NUT_Core.h"
#include "../N/Utilities/NUT_Logging.h"

#include "../N/Touch/NTouchListener.h"
#include "../N/GameLoop/NGameStates.h"
#include "../N/GameLoop/NGameStatesUID.h"
#include "../N/NEditor.h"
#include "../N/Utilities/Draw/NUT_Draw.h"

#include "App_GameState_Main_HandleFunctions.h"
#include "App_Init.h"
#include "../NL/MotionControl/NLPhysics.h"
#include "../NL/MotionControl/DriveTrain/NLDriveTrainSpecs.h"

#include "./Windows/PathAndTrajectory/WndProcTrajectoryKeyAction.h"
#include "WndProcMain.h"

#include "../N/Containers/NArray.h"
#include "../NL/MotionControl/Trajectory/NLTrajectoryChunk.h"
#include "../NL/MotionControl/Trajectory/NLTrajectoryChunkR.h"
#include "../NL/MotionControl/Trajectory/NLTrajectoryChunkT.h"

/*
#include "../NL/MotionControl/Path/Builders/PolygonalChain/NLPathBuilderPolygonalChain.h"
#include "../NL/MotionControl/Path/Builders/WayPoints/NLPathBuilderWayPoints.h"
#include "../NL/NLProject.h"
extern NLPROJECT			Project;
*/

extern NTIME				MainTimer;
extern NLDRIVETRAINSPECS	DriveTrainSpecifications;
Nu32						App_ClearFlags = FLAG_CLEAR_COLOR_BUFFER | FLAG_CLEAR_DEPTH_BUFFER;

HINSTANCE					hInstance;
HFONT						hFontTahoma;

bool is0(Nu32 p, Nu32 d)
{
	if (p % d)
		return 1;
	else
		return 0;
}
bool is1(Nu32 p, Nu32 d)
{
	if ((p/d)*d == p)
		return 0;
	else
		return 1;
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
// 	SDL_Surface				*ecran;
#ifdef _DEBUG
	_CrtSetReportMode(_CRT_ASSERT, /*_CRTDBG_MODE_DEBUG | */_CRTDBG_MODE_WNDW);
#endif

	hInstance = hinstance;

	/*
	Test zone pour constructeurs/ destructeurs ordre d'appel
	NARRAY _array;
	//	NErrorIf(sizeof(NLTRAJECTORY_CHUNK) != sizeof(NLTRAJECTORY_CHUNK_T), NERROR_SYSTEM_GURU_MEDITATION);
//	NErrorIf(sizeof(NLTRAJECTORY_CHUNK) != sizeof(NLTRAJECTORY_CHUNK_R), NERROR_SYSTEM_GURU_MEDITATION);
	Nu32 size = NMAX(sizeof(NLTRAJECTORY_CHUNK_R), sizeof(NLTRAJECTORY_CHUNK_T));
	// size = NMAX(size, sizeof(NLTRAJECTORY_CHUNK)); Ligne inutile car NLTRAJECTORY_CHUNK_R et NLTRAJECTORY_CHUNK_T hérite de NLTRAJECTORY_CHUNK donc possède une taille au moins égale à celle de leur classe mère.
	NSetupArray(&_array, 4, size);
	printf("hello world");
	new(NArrayAllocBack(&_array)) NLTRAJECTORY_CHUNK_T;
	new(NArrayAllocBack(&_array)) NLTRAJECTORY_CHUNK_R;
	new(NArrayAllocBack(&_array)) NLTRAJECTORY_CHUNK_T;
	NLTRAJECTORY_CHUNK* ptr = (NLTRAJECTORY_CHUNK*)NGetArrayPtr(&_array, 0);
	ptr->getOutput(0.0f);
	//ptr->~NLTRAJECTORY_CHUNK();
	ptr = (NLTRAJECTORY_CHUNK*)NGetArrayPtr(&_array, 1);
	ptr->getOutput(0.0f);
	//ptr->~NLTRAJECTORY_CHUNK();
	ptr = (NLTRAJECTORY_CHUNK*)NGetArrayPtr(&_array, 2);
	ptr->getOutput(0.0f);
	//ptr->~NLTRAJECTORY_CHUNK();
	NClearArray(&_array, NLclearNLTrajectoryChunkInArrayCallBack);
	*/


// ============================================================================================
// A: ALL INITIALISATIONS
// ============================================================================================
	MSG			msg	= {0};
	WNDCLASS	wc	= {0};
	
	InitCommonControls();
	/*
	INITCOMMONCONTROLSEX	com;
	com.dwSize = sizeof(INITCOMMONCONTROLSEX);
	com.dwICC = ICC_TREEVIEW_CLASSES;
	InitCommonControlsEx(&com);
	*/

	wc.style = 0;
	wc.lpfnWndProc = WndProc_Main;
	//wc.cbClsExtra = 0;
	//wc.cbWndExtra = 0;
	wc.hInstance = hinstance;
	//wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU_MAIN);
	wc.lpszClassName = "OpenglWinClass";

	if(!RegisterClass(&wc)) return 0; // check if '1' is the appropriate return value in that case

	HWND hwnd = CreateWindow(	wc.lpszClassName, "NEngine", 
								WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |WS_VISIBLE,	//WS_OVERLAPPEDWINDOW
								CW_USEDEFAULT, CW_USEDEFAULT, 1800, 1000,
								NULL, NULL, hinstance, NULL);
	if (!hwnd) return 0;




 	ShowWindow(hwnd, nCmdShow);
 	UpdateWindow(hwnd);


	// register others winclass
	WNDCLASS	wc2 = { 0 };

	wc2.style = 0;
	wc2.lpfnWndProc = WndProc_TrajectoryKeyAction;
	//wc2.cbClsExtra = 0;
	//wc2.cbWndExtra = 0;
	wc2.hInstance = hinstance;
	//wc2.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//wc2.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc2.hbrBackground = GetSysColorBrush(COLOR_3DFACE);// (HBRUSH)GetStockObject(LTGRAY_BRUSH); ;// (HBRUSH)(COLOR_BACKGROUND);
	//wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU_MAIN);
	wc2.lpszClassName = "ActionWinClass";
	if (!RegisterClass(&wc2))
		return 0; // check if '1' is the appropriate return value in that case
	/*
	WNDCLASS classinfos;
	if (GetClassInfo(hinstance, "ActionWinClass", &classinfos))
	{
		Nu32 a = 0;
		classinfos.
	}
	*/
	// GLOBAL FONTs:
	// -------------
	// voir: https://docs.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createfonta
	hFontTahoma = CreateFont(	18,					// Height		( 0 pour utiliser la hauteur par défaut )
								0,					// cWidth		( 0 pour laisser le mapper choisir )
								0,					// cEscapement	( 0 pour laisser le mapper choisir )
								0,					// cOrientation (L'angle, en dixièmes de degrés, entre la ligne de base de chaque caractère et l'axe x du device ).
								FW_THIN,			// Weight ( FW_DONTCARE, FW_THIN, FW_EXTRALIGHT,FW_LIGHT,FW_REGULAR,FW_MEDIUM,FW_SEMIBOLD,FW_BOLD,FW_EXTRABOLD,FW_HEAVY)
								FALSE,				// bItalic (TRUE or FALSE )		... police en italique
								FALSE,				// bUnderline ( TRUE or FALSE ) ... police soulignée
								FALSE,				// bStrikeOut ( TRUE or FALSE ) ... police barrée
								DEFAULT_CHARSET,	// iCharSet ( ANSI_CHARSET, BALTIC_CHARSET, CHINESEBIG5_CHARSET, DEFAULT_CHARSET, ... ) ... la table de caractères
								OUT_OUTLINE_PRECIS,	// iOutPrecision.  ( OUT_CHARACTER_PRECIS, OUT_DEFAULT_PRECIS ... ) La précision de sortie définit à quel point la sortie doit correspondre à la hauteur, la largeur, l'orientation des caractères, l'échappement, le pas et le type de police demandés.
								CLIP_DEFAULT_PRECIS,// iClipPrecision
								CLEARTYPE_QUALITY,	//
								VARIABLE_PITCH,		//
								TEXT("Tahoma"));	//



Nu64			timestart,timeend,frequency;
Nf64			time0,time1,time2,invfrequency;
QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
invfrequency = (Nf64)1000.0/frequency;
	
QueryPerformanceCounter((LARGE_INTEGER*)&timestart);	
N_Init();														// NEngine Init
	
NSetViewPortReferenceParams(960,640,NVIEWPORT_REFERENCEMODE_HEIGHT);

NAudio_Init();
QueryPerformanceCounter((LARGE_INTEGER*)&timeend);	
time0 = (Nf64)(timeend-timestart)*invfrequency;
	
QueryPerformanceCounter((LARGE_INTEGER*)&timestart);	
	NUT_Init();														// NUtilities Init
QueryPerformanceCounter((LARGE_INTEGER*)&timeend);	
time1 = (Nf64)(timeend-timestart)*invfrequency;
	
QueryPerformanceCounter((LARGE_INTEGER*)&timestart);	
	//Init_App();													// App Init

#ifdef _NWINDOWS
	// Define RootRelative Path in Windows ONLY ....
//NSetupRootRelativePath(NROOTRELATIVEPATH_APPDATA, "C:/Users/hatch/Documents/GitHub/Simulateur/Simulateur/data/");
NSetupRootRelativePath(NROOTRELATIVEPATH_APPDATA, "D:/_PROJETS/FIRST/C++/Simulateur/Simulateur/data/");


//NSetupRootRelativePath(NROOTRELATIVEPATH_USERDATA, "data_user/");
#endif

NUT_InitLogging(FLAG_NUT_CORE_LOGGING_STREAM| FLAG_NUT_CORE_LOGGING_STDOUT,NULL);

NSwitchToRootRelativePathFileSystem();
NSetCurrentRootRelativePath(NROOTRELATIVEPATH_APPDATA);

NInitializeEditor(32); // Setup "NEditor" avec une capacité initiale de 32 "tools"

App_Render_Init();
App_Tools_Init();
App_Camera_Init();
App_ClearSurface_Init();
App_Simulation_Init();
App_UpToDateActionMessagesLUT_Init("D:/_PROJETS/FIRST/C++/Simulateur/Simulateur/data/_actionmessages.txt");

//NRegisterSingleTouchTool(NEDITOR_TOOL_STARTSTOPHANDLE ptool_starthandle, NEDITOR_TOOL_STARTSTOPHANDLE ptool_stophandle, NEDITOR_TOOL_EVENTHANDLE ptool_eventhandle, NEDITOR_TOOL_TIMEHANDLE ptool_timehandle, const Nu32 tool_param);
/*
NRegisterSingleTouchTool(NULL, NULL, SplinesPackTool_Move, Snks_GameEditor_LevelEditor_SplinesPackTool_TimeHandle, (Nu32)&Snks_Editor.ToolData_LevelSplinesPack));
NRegisterSingleTouchTool(Snks_Editor_SpPack_ToolAdd_Start, NULL, SplinesPackTool_Add, Snks_GameEditor_LevelEditor_SplinesPackTool_TimeHandle, (Nu32)&Snks_Editor.ToolData_LevelSplinesPack);
NRegisterSingleTouchTool(NULL, NULL, SplinesPackTool_Supr, Snks_GameEditor_LevelEditor_SplinesPackTool_TimeHandle, (Nu32)&Snks_Editor.ToolData_LevelSplinesPack);
*/


QueryPerformanceCounter((LARGE_INTEGER*)&timeend);	
time2 = (Nf64)(timeend-timestart)*invfrequency;
	
printf("\n N_Init : %20f",time0);
printf("\n NUT_Init : %20f",time1);
printf("\n InitSnks : %20f",time2);

/*
QueryPerformanceCounter((LARGE_INTEGER*)&timestart);

for (Nu32 i = 0; i < 100000000; i++)
	is0(10+rand(), 7);

QueryPerformanceCounter((LARGE_INTEGER*)&timeend);
time0 = (Nf64)(timeend - timestart) * invfrequency;

QueryPerformanceCounter((LARGE_INTEGER*)&timestart);

for (Nu32 i = 0; i < 100000000; i++)
	is1(10+rand(),7);

QueryPerformanceCounter((LARGE_INTEGER*)&timeend);
time1 = (Nf64)(timeend - timestart) * invfrequency;

printf("\n modulo : %20f", time0);
printf("\n ops : %20f", time1);
*/


/*
// ***********************************************************************
Nu32 it = 1000000;
FILE *pfile = fopen("d:/_PROJETS/cubicRoot.txt", "w");
fprintf(pfile, "\n timer de racine cubique");
fprintf(pfile, "%d itérations	", it);

QueryPerformanceCounter((LARGE_INTEGER*)&timestart);
Nu32 i;
Nf32 f1, f2;
for (i = 0; i<it; i=i+1)
{
	f1 = Ncbrtf32(-NF32_PI);
	//printf("%20f", f1);
}
QueryPerformanceCounter((LARGE_INTEGER*)&timeend);
time3 = (Nf64)(timeend - timestart)*invfrequency;
fprintf(pfile, "\n Ncbrtf32::\t\t CubicRoot of PI:%.20f, [%.10f par appel]. Temps total pour %d itérations: %.10f )", f1, time3 / (Nf64)it, it, time3);

QueryPerformanceCounter((LARGE_INTEGER*)&timestart);
for (i = 0; i < it; i = i + 1)
{
	f1 = NNewtoncbrtf32(-NF32_PI,0.000001f);
	//printf("%20f", f1);
}
QueryPerformanceCounter((LARGE_INTEGER*)&timeend);
time3 = (Nf64)(timeend - timestart)*invfrequency;
fprintf(pfile, "\n NNewtoncbrtf32::\t CubicRoot of PI:%.20f, [%.10f par appel]. Temps total pour %d itérations: %.10f )", f1, time3 / (Nf64)it, it, time3);

fclose(pfile);
// ***********************************************************************
*/


printf("\n PI  : %.15f",3.1415927000f);
printf("\n PI/2  : %.15f %.15f",3.1415927000f/2.0f,1.5707963500f);
printf("\n 2PI : %.15f %.15f",2.0f*3.1415927000f,6.2831854000f);


// Init:
// Game State Initialization
NBindGameStateStartHandle(STATE_MAIN, App_GameState_Main_StartHandle);
NBindGameStateEndHandle(STATE_MAIN, App_GameState_Main_EndHandle);
NBindGameStateEventDrivenHandle(STATE_MAIN, App_GameState_Main_EventHandle);
NBindGameStateTimeDrivenHandle(STATE_MAIN, App_GameState_Main_TimeHandle);
NSetGameStateTouchCountMax(STATE_MAIN, 1);
NSetGameStateParent(STATE_MAIN, NSTATEUID_NULL); // pas de parent
NSetCurrentGameState(STATE_MAIN);

// ============================================================================================
// B: UPDATE LOOP
// ============================================================================================
	Nu32	_continue = 1;
	HDC dc = GetDC(hwnd);
	Nu32 test = 0;


	//NSetCLearSurfaceColorModulationf(0.99f, 0.99f, 0.99f, 1.0f);

	do
	{
		if( PeekMessage(&msg,NULL,0,0,PM_REMOVE) )
		{
			// Convert message to NEvent for N update.
			if(msg.hwnd == hwnd) // only if message is for the Main Window
				NCreateNEventFromMSG(&msg);

			// Transfer message to WINDOW
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
			_continue = N_Update();
			// Rendering ! Phase A: cameraUpdate 
			NCameraUpdatePerspective();	// TODO : Maybe NCameraUpdatePerspective() and NCameraUpdateLookAt()
			NCameraUpdateLookAt();		// can be called at the same time in the same function ....
			// Rendering ! Phase B: ...Rendering
			NClearColorf(NCOLOR_PRESET3F_GREEN_EMERALD, 1.0f);
			NClear(FLAG_CLEAR_SURFACE | FLAG_CLEAR_DEPTH_BUFFER);//| FLAG_CLEAR_COLOR_BUFFER
			N_Draw();											// NEngine draws all its Objects

#ifdef _NEDITOR
			NClear(FLAG_CLEAR_DEPTH_BUFFER);//| FLAG_CLEAR_COLOR_BUFFER
			NUT_RenderDrawings();								// NUtilities draws all its stuff
#endif

			SwapBuffers(dc);							// Screen swap
	}while( _continue );

// ============================================================================================
// C:QUIT
// ============================================================================================
	// Fonts:
	DeleteObject(hFontTahoma);

	ReleaseDC(hwnd,dc);

	App_UpToDateActionMessagesLUT_End();
	App_Simulation_End();
	// App_ClearSurface_End();	doesn't exist
	// App_Camera_End();		doesn't exist
	App_Tools_End();
	App_Render_End();


	NQuitEditor();

	//Disable_Snks();// Game Quit
	NUT_Disable();	// Engine utilities Quit
	NAudio_Quit();	
	N_Disable();	// Engine Quit
	//SDL_Quit();	// SDL Quit 
	
#ifdef _NWINDOWS
	NClearAllRootRelativePath();
#endif

	NMemoryStats();
	return 0; 
}


/*
loop function called by iphone
{
	N_Update();
	
	NCameraUpdatePerspective();	
	NCameraUpdateLookAt();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	N_Draw();// NEngine draws all its Objects, Primitives, etc ...
	NUT_Render(NTRUE);		
}	

	a faire en objective c: SDL_GL_SwapBuffers();
*/

/*
int main()
{
	//cout << "Output standard\n";
	//cerr << "Output error\n";

	return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWNORMAL);
}
*/
#endif // _NWINDOWS
#include <iostream>
#include <windows.h>
#include <CommCtrl.h>
#include "../N/NCore.h"
#include "../N/NEditor.h"
#include "../N/NCamera.h"
#include "../N/NObject.h"
#include "../N/Utilities/SplineTools/NUT_Editor_SplinesPack_Tools.h"
#include "../N/Utilities/NVec3f32Tools/NUT_Editor_NVec3f32Pack_Tools.h"

#include "../N/Utilities/Draw/NUT_Draw.h"
#include "../N/Utilities/NUT_Shape.h"

#include "../N/Containers/NXNode.h"
#include "../N/Render/Renderable/NRenderableScanner.h"
#include "../N/NObject.h"
#include "../N/Image/NImage.h"
#include "../N/NString.h"

//#include "../NL/NLDrawContext.h"
#include "../NL/NLProject.h"
#include "../NL/MotionControl/NLPathWorkbench.h"
#include "../NL/NL2DOrthogonalCoordinateSystem.h"
#include "../NL/NL2DOrthogonalCoordinateSystemTools.h"
#include "../NL/NL2DOrthogonalCoordinateSystemToolData.h"



#include "../NL/MotionControl/Path/NLPath.h"
#include "../NL/MotionControl/Trajectory/NLTrajectory.h"
#include "../NL/MotionControl/Trajectory/NLTrajectoryActionMessage.h"

#include "../NL/Simulation/VirtualRobot/NLVirtualRobot.h"
#include "../NL/Simulation/NLRobot.h"

#include "../NL/NLGfx.h"

#include "App_WayPoints_Tools.h"
#include "App_PolygonalChain_Tools.h"
//#include "App_TestAndDebug_Tools.h"
#include "./Tools/App_UserVZones_Tools.h"
#include "./Tools/App_TrajectoryKey_Tools.h"
#include "../NL/NL2DOrthogonalCoordinateSystem_MotionProfileFlags.h"

#include "App_Init.h"

// WINDOWS:
//HWND WorkBenchWindowsTable[WORKBENCH_WINDOW_ENUM::WB_WIN_ENUM_SIZE];
//HWND RobotWindowsTable[ROBOT_WINDOW_ENUM::RB_WIN_ENUM_SIZE];
HWND AppWindowsTable[APP_WINDOW_ENUM::APP_WIN_ENUM_SIZE];

// TOOLS:
Nu8	 PathBuilderToolIDTable[NLPATH_WORKBENCH::ID::NLPATH_WORKBENCH_ID_ENUM_SIZE][PATHBUILDER_TOOL_SIZE];


APP_WAYPOINT_TOOLDATA		WayPointsToolData;
APP_PLG_CHAIN_TOOLDATA		PolygonalChainToolData;
//APP_TEST_TOOLDATA			TestToolData;
APP_USERVZONE_TOOLDATA		UserVZoneToolData;
APP_TRJKEY_TOOLDATA			TrajectoryKeyToolData;

// Basics:
NLPROJECT					Project;
/*
// STRUCTURE UNIQUE: DRIVE TRAIN SPECIFICATIONS
//												 mass		Center of Mass			Axletrack	WheelRadius		StaticFriction	Vmax, Amax, Jmax		
NLDRIVETRAINSPECS			DriveTrainSpecifications(70.0f,	0.0f, 0.0f, 0.15f,		0.6f,		0.0768f,		1.1f,			3.4f, 5.2f, 60.0f);
NARRAY						WorkbenchArray;
NLPATH_WORKBENCH*			pCurrentWorkbench;
*/
// Structures utilisées par les outils d'edition:


/*
NARRAY							PathBuilderPtrArray;
NARRAY							PathPtrArray;
NARRAY							TrajectoryPtrArray;
NLPATH							Path;
NLTRAJECTORY					Trajectory;
NLTRAJECTORY_BASIC_PACK	TrajectoryStateSPack;
NLTRAJECTORY_PACK		TrajectoryStatePack;
NLPATH_BUILDER_POLYGONAL_CHAIN	PathBuilderPolygonalChain(&DriveTrainSpecifications);
*/




// Graphs
NL2DOCS						OrthogonalCoordinateSystem_A;
//NL2DOCS						OrthogonalCoordinateSystem_RobotDashboard;

NL2DOCS_TOOLDATA			OrthogonalCoordinateSystemToolData(NULL);

NLGFX						Gfx;



// Rendering
//NXNODELIST				MainXlist;			// Liste de rendu
NGEOMETRY				ClearGeometry;		// Geometrie du Fond 
//NOBJECT					PlaygroundObject;	// Le terrain ( rectangle avec/sans texture )

//Simulation
NLVIRTUAL_ROBOT	VirtualRobot(&Project.m_driveTrainSpecifications,NULL,NULL);
//NLROBOT			Robot;

NARRAY			UpToDateActionMessagesLUT;	// NLPROJECT contient la liste des ActionMessages qui seront utilisées par le logiciel 
											// A chaque chargement/sauvegarde, la liste des ActionMessages du Projet sera comparée avec cette "liste à jour"
											// En cas de diff. Il sera demandé à l'utilisateur de choisir quelle liste utiliser ( i-e conserver la liste pas à jour 
											// contenue dans le projet ou mettre à jour la liste du projet avec la liste uptodate !!! )
											// La liste UptoDate est créée à partir du fichier data/_actionmessages.txt qui fait donc reference pour le logiciel.

/*
Nu32 NLCHARACTERIZATION_TABLE::importTxt(const Nchar * pfilename, const Nbool bclear_table_before_loading)
{
	NErrorIf(!pfilename, NERROR_NULL_POINTER);
	/ * -----------------------------------------------------------------------------------------------------------------
	*
	*  Check extension
	*
	* /
if (!NStrCheckEnd(pfilename, EXTENSION_NLCHARACTERIZATION_TABLE_TXT))
{
	NErrorIf(1, NERROR_FILE_OPENING_ERROR);
	return 0;
}

FILE* pfile;
Nchar								tempstring[1024];
Nchar								name[32];
Nchar* pstr;
Ns32								gearbox_nb = 0;
Ns32								motor_nb = 0;
Ns32								inverted = 0;
Nf32								gb_ratio = 1.0f;
Nf32								wscale = 0.0f;
Nu8									g, m;
NLCHARACTERIZATION_TABLE_ROW		row;

// Reset de la table avant chargement ?
if (bclear_table_before_loading)
{
	NEraseArray(&m_table, NULL);
	m_gearBoxNb = 0;
}
pfile = fopen(pfilename, "r");		// ouverture du fichier
fseek(pfile, 0, SEEK_SET);			// on se place au début du fichier

// recupérer la siganture du fichier
pstr = fgets(tempstring, 1024, pfile);
pstr = NStrGet_String_AfterLabel(pstr, "signature= ", name);
if (strcmp(name, SIGNATURE_NLCHARACTERIZATION_TABLE_TXT))
{
	fclose(pfile);
	return 0;
}
// recupérer le nombre de gearbox décrites dans le fichier
pstr = fgets(tempstring, 1024, pfile);
pstr = NStrGet_Ns32_AfterLabel(pstr, "gearbox= ", &gearbox_nb);
// Si le nombre de gearbox décrites dans le fichier est superieur au nombre MAX de gearbox  on génère une erreur !
// Si cela produit, il faut, augmenter la valeur static du nombre max de moteurs .. ou vérifier qu'il n'y a pas une erreur dans le fichier entrant.
NErrorIf((gearbox_nb <= 0) || ((gearbox_nb + m_gearBoxNb) > NU8_MAX), NERROR_INDEX_OUTOFRANGE);

for (g = 0; g < gearbox_nb; g++)
{
	pstr = fgets(tempstring, 1024, pfile);
	pstr = NStrGet_Ns32_AfterLabel(pstr, "motors= ", &motor_nb);

	pstr = NStrGet_Nf32_AfterLabel(pstr, "ratio= ", &row.m_gearRatio);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "wscale= ", &row.m_angularVelocityScaleFactor);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "vmax+= ", &row.m_forwardVelocityMax);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "amax+= ", &row.m_forwardAccelerationMax);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "vmax-= ", &row.m_backwardVelocityMax);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "amax-= ", &row.m_backwardAccelerationMax);

	// Si le nombre de moteurs décrit dans le fichier est superieur au nombre MAX de moteurs  on génère une erreur !
	// Si cela produit, il faut, augmenter la valeur static du nombre max de moteurs .. ou vérifier qu'il n'y a pas une erreur dans le fichier entrant.
	NErrorIf((motor_nb <= 0) || (motor_nb > NU8_MAX), NERROR_INDEX_OUTOFRANGE);
	// On charge autant de lignes que de moteurs
	for (m = 0; m < motor_nb; m++)
	{
		pstr = fgets(tempstring, 1024, pfile);

		row.m_flags = 0;
		pstr = NStrGet_Ns32_AfterLabel(pstr, "inv= ", &inverted);
		if (inverted)
			FLAG_ON(row.m_flags, FLAG_NLCHARACTERIZATION_TABLE_ROW_IS_INVERTED);

		row.m_gearboxId = m_gearBoxNb + g;
		row.m_motorId = m;

		pstr = NStrGet_String_BetweenLabels(pstr, "name= ", " ", name);
		NStrCopy(row.m_motorName, name, 8);

		// Forward Kv, Ka, et intercept
		pstr = NStrGet_Nf32_AfterLabel(pstr, "kv+= ", &row.m_characterization.m_forwardKv);
		pstr = NStrGet_Nf32_AfterLabel(pstr, "ka+= ", &row.m_characterization.m_forwardKa);
		pstr = NStrGet_Nf32_AfterLabel(pstr, "intercept+= ", &row.m_characterization.m_forwardIntercept);

		// Backward Kv, Ka, et intercept
		pstr = NStrGet_Nf32_AfterLabel(pstr, "kv-= ", &row.m_characterization.m_backwardKv);
		pstr = NStrGet_Nf32_AfterLabel(pstr, "ka-= ", &row.m_characterization.m_backwardKa);
		pstr = NStrGet_Nf32_AfterLabel(pstr, "intercept-= ", &row.m_characterization.m_backwardIntercept);

		NArrayPushBack(&m_table, (NBYTE*)&row);
	}
}
m_gearBoxNb += _SafeNs32ToNu8(gearbox_nb);
fclose(pfile);
return 1;
}
*/



void App_UpToDateActionMessagesLUT_Init(const Nchar* pfilename)
{
	FILE*		pfile;
	Nchar		str[1024];
	Nchar		str2[1024];

	Nchar*	pstr;
	NLACTIONMESSAGE	msg;
	Nu16			id = 0;
	NSetupArray(&UpToDateActionMessagesLUT, 16, sizeof(NLACTIONMESSAGE));

	pfile = fopen(pfilename, "r");		// ouverture du fichier
	NErrorIf(!pfile, NERROR_NULL_POINTER);
	if (pfile)
	{
		fseek(pfile, 0, SEEK_SET);			// on se place au début du fichier
		while (pstr = fgets(str, 1024, pfile))
		{
			id += 1;
			sprintf(str2, "[%d]%s", id, str);
			NStrCopy(msg.m_name, str2, 32);
			NArrayPushBack(&UpToDateActionMessagesLUT, (NBYTE*)&msg);
		}
		fclose(pfile);
	}
	Project.initActionMessagesLut(&UpToDateActionMessagesLUT);
}
void App_UpToDateActionMessagesLUT_End()
{
	NClearArray(&UpToDateActionMessagesLUT, NULL);
}

void App_Simulation_Init()
{
	VirtualRobot.Init();
	//Robot.RobotInit(&VirtualRobot, Project.m_pCurrentWorkbench);
}
void App_Simulation_End()
{

}


void App_Render_Init()
{
	Project.initRendering();
}

void App_Render_End()
{
	Project.endRendering();
}

void App_Tools_Init()
{	
	Nu32 i;
	for (i = 0; i < APP_WINDOW_ENUM::APP_WIN_ENUM_SIZE; i++)
		AppWindowsTable[i] = NULL;

	// TOOLS PATH BUILDERS

// --------------------------------------------------------------
// maintenant tout es dans le projet
//	pCurrentWorkbench = NULL;
//	NSetupArray(&WorkbenchArray, 8, sizeof(NLPATH_WORKBENCH));
// --------------------------------------------------------------

	// 1) WAY POINTS
	App_SetupWayPointsToolData(&WayPointsToolData, NULL);
	NRegisterSingleTouchTool(NULL, NULL, App_WayPointsTool_Insert,		App_WayPointsTool_TimeHandle,		(Nu32)&WayPointsToolData);		// TOOL 0: NTOOL_ID_PATH_TOOL_NEW_WAY_POINT
	NRegisterSingleTouchTool(NULL, NULL, App_WayPointsTool_Erase,		App_WayPointsTool_TimeHandle,		(Nu32)&WayPointsToolData);		// TOOL 1: NTOOL_ID_PATH_TOOL_ERASE_WAY_POINT
	NRegisterSingleTouchTool(NULL, NULL, App_WayPointsTool_Move,		App_WayPointsTool_TimeHandle,		(Nu32)&WayPointsToolData);		// TOOL 2: NTOOL_ID_PATH_TOOL_EDIT_WAY_POINT
	PathBuilderToolIDTable[NLPATH_WORKBENCH::ID::WB_WAYPOINTS][PATHBUILDER_TOOL_CREATE]	= NTOOL_ID_PATH_TOOL_NEW_WAY_POINT;
	PathBuilderToolIDTable[NLPATH_WORKBENCH::ID::WB_WAYPOINTS][PATHBUILDER_TOOL_EDIT]		= NTOOL_ID_PATH_TOOL_EDIT_WAY_POINT;
	PathBuilderToolIDTable[NLPATH_WORKBENCH::ID::WB_WAYPOINTS][PATHBUILDER_TOOL_DELETE]	= NTOOL_ID_PATH_TOOL_ERASE_WAY_POINT;

	// 2) POLYGONAL CHAIN
	App_SetupPolygonalChainToolData(&PolygonalChainToolData, NULL);
	NRegisterSingleTouchTool(NULL, NULL, App_PolygonalChainTool_Create, App_PolygonalChainTool_TimeHandle,	(Nu32)&PolygonalChainToolData);	// TOOL 3: NTOOL_ID_PATH_TOOL_NEW_PLG_VERTEX
	NRegisterSingleTouchTool(NULL, NULL, App_PolygonalChainTool_Erase,	App_PolygonalChainTool_TimeHandle,	(Nu32)&PolygonalChainToolData);	// TOOL 4: NTOOL_ID_PATH_TOOL_ERASE_PLG_VERTEX
	NRegisterSingleTouchTool(NULL, NULL, App_PolygonalChainTool_Edit,	App_PolygonalChainTool_TimeHandle,	(Nu32)&PolygonalChainToolData);	// TOOL 5: NTOOL_ID_PATH_TOOL_EDIT_PLG_VERTEX
	PathBuilderToolIDTable[NLPATH_WORKBENCH::ID::WB_PLG_CHAIN][PATHBUILDER_TOOL_CREATE]		= NTOOL_ID_PATH_TOOL_NEW_PLG_VERTEX;
	PathBuilderToolIDTable[NLPATH_WORKBENCH::ID::WB_PLG_CHAIN][PATHBUILDER_TOOL_EDIT]		= NTOOL_ID_PATH_TOOL_EDIT_PLG_VERTEX;
	PathBuilderToolIDTable[NLPATH_WORKBENCH::ID::WB_PLG_CHAIN][PATHBUILDER_TOOL_DELETE]		= NTOOL_ID_PATH_TOOL_ERASE_PLG_VERTEX;

	// GRAPHIC CONFIGURATION
	Gfx.setup();

	// TOOLS OCS
	NRECTf32 rect = { 0.1f, -0.1f, 0.5f,-0.5f };
	NVEC2f32 unit = { 1.0f,1.0f };
	OrthogonalCoordinateSystem_A.setup("Motion Profile - VIEW A", BITS_NL2DOCS_ORIGIN_LEFT_BOTTOM, &rect, &unit);
	OrthogonalCoordinateSystem_A.setCaptionColor(NCOLOR_PRESET3F_BLUE_AQUAMARINE,1.0f);

	//OrthogonalCoordinateSystem_RobotDashboard.setup("Robot Dashboard", BITS_NL2DOCS_ORIGIN_LEFT_BOTTOM, &rect, &unit);
	//OrthogonalCoordinateSystem_RobotDashboard.setCaptionColor(NCOLOR_PRESET3F_RED, 1.0f);
	
	OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_DASHBOARD_MP);
	OrthogonalCoordinateSystem_A.setUserFlagOff(FLAG_NL2DOCS_DASHBOARD_TRACE);

	//OrthogonalCoordinateSystem_RobotDashboard.setUserFlagOff(FLAGS_NL2DOCS_MOTIONPROFILE_ID);
	//OrthogonalCoordinateSystem_RobotDashboard.setUserFlagOn(FLAGS_NL2DOCS_ROBOT_DASHBOARD);

	// Initialisation des Styles de coordonnées de NL2DOCS
// NL2DOCS_COORDS_STYLE_0.......................... Defini et utilisé pour les valeurs en abscisses sur les NL2DOCS
// Coords style
	OrthogonalCoordinateSystem_A.hideCoordsIndex(NL2DOCS_COORDS_STYLE_0);
	OrthogonalCoordinateSystem_A.showCoordsValue(NL2DOCS_COORDS_STYLE_0);
	OrthogonalCoordinateSystem_A.showCoordsLine(NL2DOCS_COORDS_STYLE_0);
	OrthogonalCoordinateSystem_A.setCoordsColor(NL2DOCS_COORDS_STYLE_0, 5);
	OrthogonalCoordinateSystem_A.setCoordsDigits(NL2DOCS_COORDS_STYLE_0, 2);
	OrthogonalCoordinateSystem_A.setCoordsOrientation(NL2DOCS_COORDS_STYLE_0, NL2DOCS_ORIENT_ENUM::NL2DOCS_ORIENT_HORIZONTAL);

	// NL2DOCS_COORDS_STYLE_1.......................... Defini et utilisé pour les valeurs en ordonnées sur les NL2DOCS
	OrthogonalCoordinateSystem_A.hideCoordsIndex(NL2DOCS_COORDS_STYLE_1);
	OrthogonalCoordinateSystem_A.showCoordsValue(NL2DOCS_COORDS_STYLE_1);
	OrthogonalCoordinateSystem_A.showCoordsLine(NL2DOCS_COORDS_STYLE_1);
	OrthogonalCoordinateSystem_A.setCoordsColor(NL2DOCS_COORDS_STYLE_1, 5);
	OrthogonalCoordinateSystem_A.setCoordsDigits(NL2DOCS_COORDS_STYLE_1, 2);
	OrthogonalCoordinateSystem_A.setCoordsOrientation(NL2DOCS_COORDS_STYLE_1, NL2DOCS_ORIENT_ENUM::NL2DOCS_ORIENT_HORIZONTAL);

	// NL2DOCS_COORDS_STYLE_2.......................... Defini et utilisé pour les INDEX en abscisses sur les NL2DOCS
	OrthogonalCoordinateSystem_A.showCoordsIndex(NL2DOCS_COORDS_STYLE_2);
	OrthogonalCoordinateSystem_A.hideCoordsValue(NL2DOCS_COORDS_STYLE_2);
	OrthogonalCoordinateSystem_A.showCoordsLine(NL2DOCS_COORDS_STYLE_2);
	OrthogonalCoordinateSystem_A.setCoordsColor(NL2DOCS_COORDS_STYLE_2, 5);
	OrthogonalCoordinateSystem_A.setCoordsDigits(NL2DOCS_COORDS_STYLE_2, 2);
	OrthogonalCoordinateSystem_A.setCoordsOrientation(NL2DOCS_COORDS_STYLE_2, NL2DOCS_ORIENT_ENUM::NL2DOCS_ORIENT_HORIZONTAL);


	//OrthogonalCoordinateSystemToolData.bindOcs(&OrthogonalCoordinateSystem_RobotDashboard);
	NRegisterSingleTouchTool(NULL, NULL, NL2DOCS_FreeTransform, NL2DOCS_TimeHandle, (Nu32)&OrthogonalCoordinateSystemToolData);			// TOOL 6:NTOOL_ID_2DOCS_FREE_TRANSFORM

	// TOOLS USERVZONES:
	NRegisterSingleTouchTool(NULL, NULL, App_UserVZoneTool_Insert, App_UserVZoneTool_TimeHandle, (Nu32)&UserVZoneToolData);				// TOOL 7:	NTOOL_ID_INSERT_USERVZONE						
	NRegisterSingleTouchTool(NULL, NULL, App_UserVZoneTool_Edit, App_UserVZoneTool_TimeHandle, (Nu32)&UserVZoneToolData);				// TOOL 8:	NTOOL_ID_EDIT_USERVZONE						
	NRegisterSingleTouchTool(NULL, NULL, App_UserVZoneTool_Delete, App_UserVZoneTool_TimeHandle, (Nu32)&UserVZoneToolData);				// TOOL 9:	NTOOL_ID_DELETE_USERZONE						

	// TOOLS TRJKEYS:
	NRegisterSingleTouchTool(NULL, NULL, App_TrajectoryKeyTool_Insert,	App_TrajectoryKeyTool_TimeHandle, (Nu32)&TrajectoryKeyToolData);	// TOOL 10:	NTOOL_ID_INSERT_TRJKEY						
	NRegisterSingleTouchTool(NULL, NULL, App_TrajectoryKeyTool_Edit,	App_TrajectoryKeyTool_TimeHandle, (Nu32)&TrajectoryKeyToolData);	// TOOL 11:	NTOOL_ID_EDIT_TRJKEY						
	NRegisterSingleTouchTool(NULL, NULL, App_TrajectoryKeyTool_Delete,	App_TrajectoryKeyTool_TimeHandle, (Nu32)&TrajectoryKeyToolData);	// TOOL 12:	NTOOL_ID_DELETE_TRJKEY						


	// TOOLS DIVERS:
	//NRegisterSingleTouchTool(NULL, NULL, App_TestTool_RobotPositionVsPath, App_TestTool_RobotPositionVsPath_TimeHandle, (Nu32)&TestToolData);	// TOOL 7:NTOOL_ID_TEST_ROBOT_POSITION


}
/*
void _clear_path(void*ptr){delete(*(NLPATH**)ptr);}
void _clear_path_builder(void* ptr) {delete(*(NLPATH_BUILDER**)ptr); }
void _clear_trajectory(void* ptr) { delete(*(NLTRAJECTORY**)ptr); }
*/

void App_Tools_End()
{
	App_ClearWayPointsToolData(&WayPointsToolData);
	App_ClearPolygonalChainToolData(&PolygonalChainToolData);

	// Tous les Registred Tools seront clear automatiquement par la methode Quit de NEditor

// --------------------------------------------------------------
// Maintenant Tout est dans le projet
//
//	NErrorIf(WorkbenchArray.Size, NERROR_ARRAY_NOT_EMPTY); // L'array doit être complètement vide !
//	NClearArray(&WorkbenchArray,NULL);
// --------------------------------------------------------------

}


void App_Camera_Init()
{
	NCameraSetVectorUpf(0.0f, 1.0f, 0.0f);
	NSetCameraPositionf(0.0f, 0.0f, 20.0f);
	NSetCameraTargetPositionf(0.0f, 0.0f, 0.0f);
	NCameraSetAspectRatio(NGetViewPort_AspectRatio());
	NCameraSetFov(30.0f);
	NCameraSetZNear(0.1f);
	NCameraSetZFar(100.0f);

	NCameraUpdatePerspective();
	NCameraUpdateLookAt();
}

void App_ClearSurface_Init()
{
	NGEOMETRY_DESC		gdesc;
	NUT_SURFACE_DESC	desc;
	NCOLOR				color[4];
//	NTEXTURE			*ptexture;

	NFillupGeometryDesc(&gdesc, NGEOMETRY_FORMAT_COLORFLATTRIANGLE_TU0_V3C4_TRIANGLE, 0, 2, 1);
	NSetupGeometry(&ClearGeometry, &gdesc);

	Nmem0(&desc, NUT_SURFACE_DESC);
	FLAG_ON(desc.CoreDesc.Flags, FLAG_NUTSCD_USEPARAM_SIZE);
	FLAG_ON(desc.CoreDesc.Flags, FLAG_NUTSCD_USEPARAM_CONSTRUCTIONPLANE);
	FLAG_ON(desc.CoreDesc.Flags, FLAG_SURFACE_DESC_USEPARAMS_SEGMENTS);
	FLAG_ON(desc.CoreDesc.ColorBuild.Flags, FLAG_NBUILDBASE_COLOR_USEPARAMS_COLORRANGE);
/*
	NSetColorf(&color[0], NCOLOR_PRESET3F_BLUE_MONASTRAL, 1);
	NSetColorf(&color[1], NCOLOR_PRESET3F_BLUE_PRUSSIAN, 1);
	NSetColorf(&color[2], NCOLOR_PRESET3F_BLUE_PERSIAN, 1);
	NSetColorf(&color[3], NCOLOR_PRESET3F_BLUE_CERULEAN, 1);
*/
	NSetColorf(&color[0], 0.278f, 0.286f, 0.525f, 1.0f);
	NSetColorf(&color[1], 0.165f, 0.231f, 0.318f, 1.0f);
	NSetColorf(&color[2], 0.104f, 0.110f, 0.308f, 1.0f);
	NSetColorf(&color[3], 0.180f, 0.140f, 0.322f, 1.0f);

	desc.CoreDesc.ColorBuild.pColorRange = color;
	desc.CoreDesc.ColorBuild.ColorRangeSize = 4;

	desc.XSegments = 1;
	desc.YSegments = 1;
	desc.CoreDesc.Size.x = NGetViewPort_AspectRatio();
	desc.CoreDesc.Size.y = 1;
	desc.CoreDesc.Construction_Plane = _PLANE_XY;
	NUT_InsertSurface(&ClearGeometry, &desc);

	NBindClearSurface(&ClearGeometry);
}


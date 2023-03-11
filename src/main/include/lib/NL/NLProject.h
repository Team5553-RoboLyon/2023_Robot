#pragma once
#include "lib/N/NType.h"
// #include "lib/N/File/NFile.h"
#include "lib/N/File/NDataPacker.h"
#include "lib/N/Containers/NArray.h"
#include "lib/N/Core/NVersion.h"

#include "NLPlayground.h"
#include "lib/NL/MotionControl/NLPathWorkbench.h"
#include "lib/NL/MotionControl/DriveTrain/NLDriveTrainSpecs.h"
#include "lib/NL/MotionControl/DriveTrain/NLRamsete.h"

#define DEFAULT_INITIAL_ACTIONMESSAGESLUT_ARRAY_CAPACITY 8

//														 mass		Center of Mass	 Axletrack	WheelRadius	StaticFriction	Vmax, Amax, Jmax
//														  |		  _______|_______		 |			|			 |			 |		|	 |
//														  |		 |		 |		 |		 |			|			 |			 |		|	 |
#define DEFAULT_NLPROJECT_DT_CHARACTERIZATION 70.0f, 0.0f, 0.0f, 0.15f, 0.6f, 0.0768f, 1.1f, 3.4f, 5.2f, 60.0f
#define DEFAULT_INITIAL_WORKBENCH_ARRAY_CAPACITY 8

#define CONSTANT_NLPROJECT_DT_MIN 1e-6f
class NLPROJECT
{
public:
	enum DPKEY_ID
	{
		WORK_BENCH = NDPKEY_USER,
		DT_SPECS = NDPKEY_USER + 1,
		PATH = NDPKEY_USER + 2,
		PATH_BLDR_WP = NDPKEY_USER + 3,
		PATH_BLDR_PLG = NDPKEY_USER + 4,
		TRAJECTORY = NDPKEY_USER + 5,
		//		STATE_PACK		= NDPKEY_USER + 6,
		//		STATE_S_PACK	= NDPKEY_USER + 7
	};
#define DPKEY_NB 6

	NLPROJECT();
	~NLPROJECT();

	Nu32 createPathWorkBench(NLPATH_WORKBENCH::ID id);
	void erasePathWorkBench(const Nu32 index);
	void eraseAllPathWorkBenches();

	Nu32 save(Nchar *pfilename);
	Nu32 open(Nchar *pfilename);
	void setSimulationDt(const Nf32 dt) { m_simulationDt = NMAX(CONSTANT_NLPROJECT_DT_MIN, dt); }
	void close();

	void initActionMessagesLut(const NARRAY *plut);
	Nu32 isActionMessagesLutUpToDate(const NARRAY *preflut);
	void UpdateActionMessagesLut(const NARRAY *preflut);

#ifdef _NEDITOR
	void initRendering();
	void endRendering();
	NXNODELIST m_renderList;
	NLPLAYGROUND m_playground;

	void draw();
#endif

	NARRAY m_workbenchArray;
	NLPATH_WORKBENCH *m_pCurrentWorkbench;

	// Other Data
	NLDRIVETRAINSPECS m_driveTrainSpecifications;
	NLRAMSETE m_ramseteParams; // Param�tres par d�faut des followers RAMSETE ( il y a un set de ces params par WorkBench + ce set "par d�faut" (�ditable) inclu dans le projet )
	Nf32 m_simulationDt;	   //

	NARRAY m_actionMessagesLut;
};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + FILE
// +
// File Extensions
#define FILE_EXTENSION_NLPROJECT ".rbl" // RoBot Projet
// File Signatures
#define FILE_SIGNATURE_NLPROJECT NMAKESIGNATURE('R', 'B', 'L', 'P') // RoBo'Lyon Projet

#define VERSION_NLPROJECT_HEADER NMAKEVERSION(0, 0, 0) // 2022/10/03
/*
* premiere version, pour m�moire  >>> 2021/04/03
typedef struct
{
	Nu32	m_available;	// juste un 32 bits vide
}NLPROJECT_HEADER;
*/
typedef struct
{
	NARRAYBOUNDS m_actionMessagesLutBounds;
} NLPROJECT_HEADER;

Nu32 writeDataPack_DTSpecs(void *pl, NDATAPACKER *pdpacker, FILE *pfile);			   // ok
Nu32 writeDataPack_Work_Bench(void *pl, NDATAPACKER *pdpacker, FILE *pfile);		   // ok
Nu32 writeDataPack_Path_Builder_Wp(void *pl, NDATAPACKER *pdpacker, FILE *pfile);	   // ok
Nu32 writeDataPack_Path_Builder_Plg(void *pl, NDATAPACKER *pdpacker, FILE *pfile);	   // ok
Nu32 writeDataPack_Path(void *pl, NDATAPACKER *pdpacker, FILE *pfile);				   // ok
Nu32 writeDataPack_Trajectory(void *pl, NDATAPACKER *pdpacker, FILE *pfile);		   // ok
Nu32 writeDataPack_TrajectoryStatePack(void *pl, NDATAPACKER *pdpacker, FILE *pfile);  // ok
Nu32 writeDataPack_TrajectoryStateSPack(void *pl, NDATAPACKER *pdpacker, FILE *pfile); // ok

// Read Callbacks
Nu32 readDataPack_DTSpecs(void *pl, NDATAPACKER *pdpacker, FILE *pfile);
Nu32 readDataPack_Work_Bench(void *pl, NDATAPACKER *pdpacker, FILE *pfile);
Nu32 readDataPack_Path_Builder_Wp(void *pl, NDATAPACKER *pdpacker, FILE *pfile);
Nu32 readDataPack_Path_Builder_Plg(void *pl, NDATAPACKER *pdpacker, FILE *pfile);
Nu32 readDataPack_Path(void *pl, NDATAPACKER *pdpacker, FILE *pfile);
Nu32 readDataPack_Trajectory(void *pl, NDATAPACKER *pdpacker, FILE *pfile);
Nu32 readDataPack_TrajectoryStatePack(void *pl, NDATAPACKER *pdpacker, FILE *pfile);
Nu32 readDataPack_TrajectoryStateSPack(void *pl, NDATAPACKER *pdpacker, FILE *pfile);

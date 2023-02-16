#pragma once
#ifdef _NEDITOR
#include <iostream>
#include <windows.h>
#include <CommCtrl.h>
#endif
#include "../../N/NType.h"
#include "../../N/NFlags.h"
#include "../../N/File/NDataPacker.h"
#include "../../N/Core/NVersion.h"

#include "./DriveTrain/NLRamsete.h"

#include "./Path/NLPath.h"
#include "./Path/Builders/NLPathBuilder.h"
#include "./Trajectory/NLTrajectory.h"
#include "./Trajectory/NLTrajectoryPack.h"




#define FLAG_NLPATH_WORKBENCH_PATH_BUILD_REQUEST						BIT_15
#define FLAG_NLPATH_WORKBENCH_TRAJECTORY_BUILD_REQUEST					BIT_14
#define FLAG_NLPATH_WORKBENCH_PATH_BUILDED								BIT_13
#define FLAG_NLPATH_WORKBENCH_TRAJECTORY_BUILDED						BIT_12
#define FLAG_NLPATH_WORKBENCH_SMART_BUILD								BIT_11
//#define FLAG_NLPATH_WORKBENCH_USE_SPECIFIC_KINLIMITS					BIT_9		// ON on utilise "NLPATH_WORKBENCH.m_kinLimits" / OFF on utilise NLPATH_WORKBENCH.m_pDriveTrainSpecifications.m_limits
//#define FLAG_NLPATH_WORKBENCH_USE_SPECIFIC_RAMSETEPARAMS				BIT_9		// ON on utilise "NLPATH_WORKBENCH.m_specificRamseteParams" / OFF on utilise NLPROJECT.m_ramseteParams

//																		.
//																		.
//																		.
//#define FLAG_NLPATH_WORKBENCH_xxx										BIT_1
#define FLAG_NLPATH_WORKBENCH_DRAW_PATH									BIT_0



#define NLPATH_WORKBENCH_NAME_SIZE										32	// null terminated character included
class NLPATH_WORKBENCH
{
public:
	enum ID
	{
		WB_WAYPOINTS = 0,
		WB_PLG_CHAIN = 1,
		WB_UNDEFINED = 2,
		//---------------------------------
		NLPATH_WORKBENCH_ID_ENUM_SIZE
	};

	NLPATH_WORKBENCH();
	NLPATH_WORKBENCH(const Nu8 id, const Nchar* pname, const Nu16 flags, NLPATH_BUILDER* ppathbuilder, NLPATH* ppath, NLTRAJECTORY* ptrajectory,  NLDRIVETRAINSPECS* pdts, NLRAMSETE* prms);
	~NLPATH_WORKBENCH();
	
	// 
	Nu32	write(NDATAPACKER *pdpacker, FILE* pfile);
	Nu32	read(NDATAPACKER* pdpacker, FILE* pfile);

	// Activate/deactivate smart build ( en cas d'update du chemin alors la trajectoire s'update egalement, en cas d'update de la trajectoire les packs s'update eux aussi )
	inline void					showPath() { FLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_DRAW_PATH); }
	inline void					hidePath() { FLAG_OFF(m_flags, FLAG_NLPATH_WORKBENCH_DRAW_PATH); }

	inline void					activateSmartBuild() { FLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_SMART_BUILD); }
	inline void					deactivateSmartBuild() { FLAG_OFF(m_flags, FLAG_NLPATH_WORKBENCH_SMART_BUILD); }

	inline void					requestPathBuild() { FLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_PATH_BUILD_REQUEST); }
	inline void					requestTrajectoryBuild() { FLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_BUILD_REQUEST); }							// in case, for example of driveTrainSpecifications update
	//inline void					requestTrajectoryStatePackBuild() { FLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_STATE_PACK_BUILD_REQUEST); }		
	//inline void					requestTrajectoryStateSPackBuild() { FLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_STATE_S_PACK_BUILD_REQUEST); }	
	//inline void					requestAllTrajectoryStatePacksBuild() { FLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_STATE_PACK_BUILD_REQUEST| FLAG_NLPATH_WORKBENCH_TRAJECTORY_STATE_S_PACK_BUILD_REQUEST); }

	// Update/Set Value(s)
	void						setPathBuilder(NLPATH_BUILDER* ppathbuilder, Nbool rebuild = NFALSE);

	void						setPath(NLPATH* ppath, Nbool rebuild=NFALSE);

	void						setTrajectory(NLTRAJECTORY* ptrajectory, Nbool rebuild=NFALSE);
	/*
	void						setTrajectoryPack(NLTRAJECTORY_PACK* ppack, Nbool rebuild=NFALSE);
	void						setTrajectoryBasicPack(NLTRAJECTORY_BASIC_PACK* pspack, Nbool rebuild=NFALSE);
	*/
	void						setDriveTrainSpecifications(NLDRIVETRAINSPECS* pdts, Nbool rebuild=NFALSE);
	void						setRamseteParams(NLRAMSETE* prms);


	inline void					setId(const Nu8 id) { m_id = id; }
	//inline void					setuserData(const Nu8 data) { m_userData = data; }

	//inline Nu8					getUserData() { return m_userData; }

	inline Nu8							getId() const { return m_id; }

	inline NLPATH_BUILDER*				getPathBuilder() const { return m_pPathBuilder; }
	inline NLPATH*						getPath() const { return m_pPath; }
	inline NLPATH_GEOMETRY*				getPathGeometry() const { return m_pPath ? &m_pPath->m_geometry : NULL; }

	inline NLTRAJECTORY*				getTrajectory() const { return m_pTrajectory; }
	inline NARRAY*						getTrajectoryKeysArray() const { return m_pTrajectory ? &m_pTrajectory->m_keysArray:NULL; }
	/*
	inline NLTRAJECTORY_PACK*		getTrajectoryStatePack() const { return m_pTrajectoryStatePack; }
	inline NLTRAJECTORY_BASIC_PACK*	getTrajectoryStateSPack() const { return m_pTrajectoryStateSPack; }
	*/
	inline NLDRIVETRAINSPECS*			getDriveTrainSpecifications() const { return m_pDriveTrainSpecifications; }
	


	void						build();

#ifdef _NEDITOR
	void						draw();
	Nchar*						getKinLimitsInfoString(Nchar* info, Nu32 charmax_null_terminated_char_included);

	// Treeview Links:
	HTREEITEM					m_hTreeViewVzones;
	HTREEITEM					m_hTreeViewKeys;

	//HTREEITEM					m_hTreeSelectedKey;	// Current Selected Key in Treeview.
	
#endif

//private:	
	Nchar						m_name[NLPATH_WORKBENCH_NAME_SIZE];			// ( null terminated character included ! )
	NLPATH_BUILDER				*m_pPathBuilder;
	NLPATH						*m_pPath;
	NLTRAJECTORY				*m_pTrajectory;
//	NLTRAJECTORY_PACK			*m_pTrajectoryStatePack;
//	NLTRAJECTORY_BASIC_PACK		*m_pTrajectoryStateSPack;
	
	// other Data:
	NLDRIVETRAINSPECS			*m_pDriveTrainSpecifications;				// pointeur vers les DT Specifications de Reference. ( supposée être UNIQUE )
	NLRAMSETE					*m_pRamsete;								// pointeur vers les paramètres Ramsete de Reference. (supposés être uniques )
	NLRAMSETE					m_specificRamsete;							// Paramètres RAMSETE spécifics ( m_specificRamsete.xx = NF32_MAX signifie utiliser m_pRamsete.xx
	Nu16						m_flags;
	Nu8							m_id;
	Nu8							m_availableNu8;
};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + FILE
// +
#define VERSION_NLPATH_WORKBENCH_HEADER		NMAKEVERSION(0,0,0)				// >>> 2021/04/03
typedef struct
{
	Nchar						m_name[NLPATH_WORKBENCH_NAME_SIZE];

	Nu16						m_flags;
	Nu8							m_id;
	Nu8							m_availableNu8;

	Nu32						m_pathBuilder;
	Nu32						m_path;
	Nu32						m_trajectory;
//	Nu32						m_trajectoryStatePack;
//	Nu32						m_trajectoryStateSPack;
	Nu32						m_driveTrainSpecifications;

}NLPATH_WORKBENCH_HEADER;

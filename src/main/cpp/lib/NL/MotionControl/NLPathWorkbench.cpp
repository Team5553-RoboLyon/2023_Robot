#include "../../N/NString.h"
#include "../NLProject.h"
#include "../../App/App_Init.h"
#include "NLPathWorkbench.h"

extern HWND hwnd_Explorer;

NLPATH_WORKBENCH::NLPATH_WORKBENCH():	m_id(ID::WB_UNDEFINED),
										m_flags(FLAG_NLPATH_WORKBENCH_SMART_BUILD),
										m_pPathBuilder(NULL),
										m_pDriveTrainSpecifications(NULL),
										m_pRamsete(NULL),
										m_pPath(NULL),
										m_availableNu8(0),
										m_hTreeViewVzones(NULL),
										m_hTreeViewKeys(NULL) { /*Nmem0(this, NLPATH_WORKBENCH);*/ }

NLPATH_WORKBENCH::NLPATH_WORKBENCH(const Nu8 id, const	Nchar * pname, const Nu16 flags,
														NLPATH_BUILDER* ppathbuilder,
														NLPATH* ppath,
														NLTRAJECTORY* ptrajectory,
														NLDRIVETRAINSPECS* pdts,
														NLRAMSETE*	prms) :	m_id(id),
																			m_flags(flags),
																			m_pPathBuilder(ppathbuilder), 
																			m_pDriveTrainSpecifications(pdts),
																			m_pRamsete(prms),
																			m_pPath(ppath),
																			m_availableNu8(0),
																			m_hTreeViewVzones(NULL),
																			m_hTreeViewKeys(NULL)

{
	// Name:
	if(pname)
		NStrCopy(m_name, pname, NLPATH_WORKBENCH_NAME_SIZE);
	else
		NStrCopy(m_name, "New Path WorkBench", NLPATH_WORKBENCH_NAME_SIZE);

	// Trajectory
	setTrajectory(ptrajectory, NFALSE);

	// Specific Ramsete:
	m_specificRamsete.m_b	= NF32_MAX;
	m_specificRamsete.m_zeta = NF32_MAX;
}



/*{
	m_pPathBuilder				= ppathbuilder;
	m_pDriveTrainSpecifications	= pdts;
	
	m_pPath						= ppath;
	m_pTrajectory				= ptrajectory;
	m_pTrajectoryStatePack		= ppack;
	m_pTrajectoryStateSPack		= pspack;

	m_flags						= 0;
	m_id						= id;
}*/

NLPATH_WORKBENCH::~NLPATH_WORKBENCH()
{
	NErrorIf(m_pPathBuilder,				NERROR_NON_NULL_POINTER);
	NErrorIf(m_pDriveTrainSpecifications,	NERROR_NON_NULL_POINTER);

	NErrorIf(m_pPath,						NERROR_NON_NULL_POINTER);
	NErrorIf(m_pTrajectory,					NERROR_NON_NULL_POINTER);
//	NErrorIf(m_pTrajectoryStatePack,		NERROR_NON_NULL_POINTER);
//	NErrorIf(m_pTrajectoryStateSPack,		NERROR_NON_NULL_POINTER);
}

Nu32 NLPATH_WORKBENCH::write(NDATAPACKER* pdpacker, FILE* pfile)
{
	// 1) écriture Version
	Nu32	version_u32 = VERSION_NLPATH_WORKBENCH_HEADER;
	if (fwrite(&version_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;

	// 2) écriture Header
	NLPATH_WORKBENCH_HEADER header;
	NStrCopy(header.m_name,m_name,NLPATH_WORKBENCH_NAME_SIZE);
	header.m_flags			= m_flags;
	header.m_id				= m_id;
	header.m_availableNu8	= m_availableNu8;

	if(m_id == NLPATH_WORKBENCH::ID::WB_WAYPOINTS)
		header.m_pathBuilder = NGetDataPackerIndex(pdpacker,m_pPathBuilder,NLPROJECT::DPKEY_ID::PATH_BLDR_WP);
	else
		header.m_pathBuilder = NGetDataPackerIndex(pdpacker, m_pPathBuilder, NLPROJECT::DPKEY_ID::PATH_BLDR_PLG);

	header.m_path						= NGetDataPackerIndex(pdpacker,m_pPath,						NLPROJECT::DPKEY_ID::PATH);
	header.m_trajectory					= NGetDataPackerIndex(pdpacker,m_pTrajectory,				NLPROJECT::DPKEY_ID::TRAJECTORY);
//	header.m_trajectoryStatePack		= NGetDataPackerIndex(pdpacker,m_pTrajectoryStatePack,		NLPROJECT::DPKEY_ID::STATE_PACK);
//	header.m_trajectoryStateSPack		= NGetDataPackerIndex(pdpacker,m_pTrajectoryStateSPack,		NLPROJECT::DPKEY_ID::STATE_S_PACK);
	header.m_driveTrainSpecifications	= NGetDataPackerIndex(pdpacker,m_pDriveTrainSpecifications,	NLPROJECT::DPKEY_ID::DT_SPECS);
	
	if (fwrite(&header, sizeof(NLPATH_WORKBENCH_HEADER), 1, pfile) != 1)
		return 0;

	// 3) Ecriture autres DATA:
	if (m_specificRamsete.write(pfile) != 1)
		return 0;


	return 1;
}

Nu32 NLPATH_WORKBENCH::read(NDATAPACKER* pdpacker, FILE* pfile)
{
	// 1) lecture Version
	Nu32	version_u32;
	if (fread(&version_u32, sizeof(Nu32), 1, pfile) != 1)
		return 0;

	// 2) lecture Header
	NLPATH_WORKBENCH_HEADER header;

	switch (NGETVERSION_MAIN(version_u32))
	{
		// all versions 0.0.x
	case NGETVERSION_MAIN(VERSION_NLPATH_WORKBENCH_HEADER):
		if (fread(&header, sizeof(NLPATH_WORKBENCH_HEADER), 1, pfile) != 1)
			return 0;

		NStrCopy(m_name, header.m_name, NLPATH_WORKBENCH_NAME_SIZE);
		m_flags = header.m_flags| FLAG_NLPATH_WORKBENCH_SMART_BUILD;
		m_id = header.m_id;
		m_availableNu8 = header.m_availableNu8;

		if (m_id == NLPATH_WORKBENCH::ID::WB_WAYPOINTS)
			m_pPathBuilder = (NLPATH_BUILDER*)NGetDataPackerPointer(pdpacker, header.m_pathBuilder, NLPROJECT::DPKEY_ID::PATH_BLDR_WP);
		else
			m_pPathBuilder = (NLPATH_BUILDER*)NGetDataPackerPointer(pdpacker, header.m_pathBuilder, NLPROJECT::DPKEY_ID::PATH_BLDR_PLG);

		m_pPath						= (NLPATH*)NGetDataPackerPointer(pdpacker, header.m_path, NLPROJECT::DPKEY_ID::PATH);
		m_pTrajectory				= (NLTRAJECTORY*)NGetDataPackerPointer(pdpacker, header.m_trajectory, NLPROJECT::DPKEY_ID::TRAJECTORY);
//		m_pTrajectoryStatePack		= (NLTRAJECTORY_PACK*)NGetDataPackerPointer(pdpacker, header.m_trajectoryStatePack, NLPROJECT::DPKEY_ID::STATE_PACK);
//		m_pTrajectoryStateSPack		= (NLTRAJECTORY_BASIC_PACK*)NGetDataPackerPointer(pdpacker, header.m_trajectoryStateSPack, NLPROJECT::DPKEY_ID::STATE_S_PACK);
		m_pDriveTrainSpecifications = (NLDRIVETRAINSPECS*)NGetDataPackerPointer(pdpacker, header.m_driveTrainSpecifications, NLPROJECT::DPKEY_ID::DT_SPECS);
		
		// 3) Lecture autres DATA:
		if (m_specificRamsete.read(pfile) != 1)
			return 0;

		return 1;

	default:
		break;
	}
	return 0;
}

void NLPATH_WORKBENCH::build()
{

	// 1) PATH BUILD
	FLAG_OFF(m_flags, FLAG_NLPATH_WORKBENCH_PATH_BUILDED);
	if (ISFLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_PATH_BUILD_REQUEST))
	{
		m_pPathBuilder->build(m_pPath);
		FLAG_OFF(m_flags, FLAG_NLPATH_WORKBENCH_PATH_BUILD_REQUEST);
		FLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_PATH_BUILDED);

		if (ISFLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_SMART_BUILD) && m_pTrajectory)
			FLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_BUILD_REQUEST);
	}
	
	// 2) TRAJECTORY BUILD
	FLAG_OFF(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_BUILDED);
	if (ISFLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_BUILD_REQUEST))
	{
		m_pTrajectory->build(m_pDriveTrainSpecifications);
		FLAG_OFF(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_BUILD_REQUEST);
		FLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_BUILDED);
	}

	if (ISFLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_PATH_BUILDED))
	{
		SendMessage(hwnd_Explorer, WM_MESSAGE_PATH_REBUILDED, (WPARAM)this, (LPARAM)0);
		FLAG_OFF(m_flags, FLAG_NLPATH_WORKBENCH_PATH_BUILDED);
	}

	if (ISFLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_BUILDED))
	{
		SendMessage(hwnd_Explorer, WM_MESSAGE_TRAJECTORY_REBUILDED, (WPARAM)this, (LPARAM)0);
		FLAG_OFF(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_BUILDED);
	}

	/*
	// 3) TRAJECTORY STATES PACKS
	if (ISFLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_STATE_PACK_BUILD_REQUEST))
	{
		m_pTrajectory->buildTrajectoryPack(m_pTrajectoryStatePack);
		FLAG_OFF(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_STATE_PACK_BUILD_REQUEST);
	}

	if (ISFLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_STATE_S_PACK_BUILD_REQUEST))
	{
		m_pTrajectory->buildTrajectoryPack(m_pTrajectoryStateSPack);
		FLAG_OFF(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_STATE_S_PACK_BUILD_REQUEST);
	}
	*/
}

#ifdef _NEDITOR
void NLPATH_WORKBENCH::draw()
{
}

Nchar* NLPATH_WORKBENCH::getKinLimitsInfoString(Nchar* info, Nu32 charmax_null_terminated_char_included)
{
	Nf32 v;
	Nf32 a;
	Nf32 j;
	Nchar* pvstr, *pastr,*pjstr;
	Nchar local[256];

	if (m_pTrajectory)
	{
		if (ISFLAG_ON(m_pTrajectory->m_flags, FLAG_NLTRJ_SPECIFIC_KINLIMITS))
		{
			if (ISFLAG_ON(m_pTrajectory->m_flags, FLAG_NLTRJ_ENFORCE_VEL))
			{
				v = m_pTrajectory->m_specificKinLimits.m_v;
				if (v > m_pDriveTrainSpecifications->m_limits.m_v)
					pvstr = "! Overshoot !";
				else
					pvstr = " ";
			}
			else
			{
				v = NMIN(m_pDriveTrainSpecifications->m_limits.m_v, m_pTrajectory->m_specificKinLimits.m_v);
				if (v < m_pTrajectory->m_specificKinLimits.m_v)
					pvstr = " = Robot Vel. Max";
				else
					pvstr = " ";
			}

			if (ISFLAG_ON(m_pTrajectory->m_flags, FLAG_NLTRJ_ENFORCE_ACC))
			{
				a = m_pTrajectory->m_specificKinLimits.m_a;
				if (a > m_pDriveTrainSpecifications->m_limits.m_a)
					pastr = "! Overshoot !";
				else
					pastr = " ";
			}
			else
			{
				a = NMIN(m_pDriveTrainSpecifications->m_limits.m_a, m_pTrajectory->m_specificKinLimits.m_a);
				if (a < m_pTrajectory->m_specificKinLimits.m_a)
					pastr = " = Robot Acc. Max";
				else
					pastr = " ";
			}

			if (ISFLAG_ON(m_pTrajectory->m_flags, FLAG_NLTRJ_ENFORCE_JRK))
			{
				j = m_pTrajectory->m_specificKinLimits.m_j;
				if (j > m_pDriveTrainSpecifications->m_limits.m_j)
					pjstr = "! Overshoot !";
				else
					pjstr = " ";
			}
			else
			{
				j = NMIN(m_pDriveTrainSpecifications->m_limits.m_j, m_pTrajectory->m_specificKinLimits.m_j);
				if (j < m_pTrajectory->m_specificKinLimits.m_j)
					pjstr = " = Robot Jerk Max";
				else
					pjstr = " ";
			}
		}
		else
		{
			v = m_pDriveTrainSpecifications->m_limits.m_v; pvstr = " = Robot Vel. Max";
			a = m_pDriveTrainSpecifications->m_limits.m_a; pastr = " = Robot Acc. Max";
			j = m_pDriveTrainSpecifications->m_limits.m_j; pjstr = " = Robot Jerk Max";
		}

		sprintf(local, "Kinematics Limits used :\n\nJerk: %.4f %s\nAccel.: %.4f %s\nVelocity: %.4f %s", j, pjstr, a, pastr, v, pvstr);

	}
	else
	{
		sprintf(local, "No Trajectory to build in !");
	}

	
	return NStrCopy(info,local, charmax_null_terminated_char_included);
}
#endif

void NLPATH_WORKBENCH::setPathBuilder(NLPATH_BUILDER* ppathbuilder, Nbool rebuild)
{
	m_pPathBuilder = ppathbuilder; 
	if (rebuild)
		FLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_PATH_BUILD_REQUEST); 
}
/*
void NLPATH_WORKBENCH::setPath(NLPATH* ppath, Nbool rebuild)
{
	NErrorIf(!ppath && rebuild, NERROR_INCONSISTENT_FLAGS);

	m_pTrajectory = NULL;
	FLAG_OFF(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_BUILD_REQUEST);

	m_pPath = ppath;
	if (rebuild && m_pPath)
		FLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_PATH_BUILD_REQUEST);
}
*/
void NLPATH_WORKBENCH::setPath(NLPATH* ppath, Nbool rebuild)
{
	NErrorIf(!ppath && rebuild, NERROR_INCONSISTENT_FLAGS);

	// Une trajectoire est deja referencee:
	if (m_pTrajectory)
	{
		// La trajectoire associée est 'vierge' elle n'a pas encore de path associé, on prend les devant on associe.
		if (m_pTrajectory->m_pPath == NULL)
		{
			m_pTrajectory->setPath(ppath); // ppath peut ici être NULL ou non NULL !
			m_pPath	= ppath;
		}
		// La trajectoire associée possede deja un path... et qui doit bien sur être dejà affecté au workbench
		else
		{
			
			
			if (m_pTrajectory->m_pPath == ppath)
			{
				// Ici on a donc, m_pTrajectory->m_pPath == ppath
				// ... ET m_pTrajectory->m_pPath != NULL
				// DONC,  ppath != NULL
				
				// il est possible que m_pPath soit NULL alors que m_pTrajectory->m_pPath = ppath.
				// En effet, l'user a pu associer la trajectoire vierge au workbench, 
				// puis il a associé ppath à trajectory
				// et enfin il associe ppath au Workbench ...
				if (!m_pPath)									
				{												
					m_pPath = ppath;							//	>>>		m_pTrajectory->m_pPath == ppath == m_pPath 				
				}
				// Si par contre ppath et m_pTrajectory->m_pPath sont identiques 
				// Mais que m_pPath est non NULL et différent, alors il y a une incohérence...
				else if (ppath != m_pPath)
				{
					// Aïe !
					NErrorIf(1, NERROR_INCONSISTENT_VALUES);	//	>>>		m_pTrajectory->m_pPath == ppath != m_pPath
					return;
				}
				//else
				//{
				//  Rien à faire, ici  on a		...................	>>>		m_pTrajectory->m_pPath == ppath == m_pPath 				
				//}
			}
		}
	}
	// Aucune trajectoire referencee ( signifie que m_pTrajectory = NULL ):
	else
	{
		m_pPath = ppath;
	}

	// et pour finir:
	if (rebuild && m_pPath)
		FLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_PATH_BUILD_REQUEST);
}


void NLPATH_WORKBENCH::setTrajectory(NLTRAJECTORY* ptrajectory, Nbool rebuild)
{
	NErrorIf(!ptrajectory && rebuild, NERROR_INCONSISTENT_FLAGS);

	if (ptrajectory)
	{
		if (ptrajectory->m_pPath)							
		{
			if(m_pPath == ptrajectory->m_pPath)				//	>>>		m_pTrajectory = ptrajectory			>>>		m_pTrajectory->m_pPath == m_pPath
				m_pTrajectory = ptrajectory;
			else if (!m_pPath)								
			{
				m_pTrajectory	= ptrajectory;		
				m_pPath			= ptrajectory->m_pPath;
			}
			else // ( m_pPath != ptrajectory->m_pPath )  &&  ( ptrajectory->m_pPath != NULL )  &&  ( m_pPath != NULL )
			{
				// Aïe !
				NErrorIf(1, NERROR_INCONSISTENT_VALUES);	//	>>>		Normalement c'est impossible .... donc 
				return;
			}
		}
		else // (ptrajectory->m_pPath == NULL)
		{
			m_pTrajectory	= ptrajectory;
			m_pTrajectory->setPath(m_pPath);		// m_pPath peut ici être NULL ou non NULL
		}
	}
	else
	// ptrajectory = NULL !
	{
		m_pTrajectory = NULL;
	}

	if (rebuild)
		FLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_BUILD_REQUEST);

	/*
	if (ptrajectory)
	{
		NErrorIf(!ptrajectory->m_pPath, NERROR_NULL_POINTER);
		
		m_pPath			= ptrajectory->m_pPath;
		m_pTrajectory	= ptrajectory;

		if (rebuild)
			FLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_BUILD_REQUEST);
	}
	else
	{
		m_pTrajectory	= NULL;
		m_pPath			= NULL;
		FLAG_OFF(m_flags, FLAG_NLPATH_WORKBENCH_PATH_BUILD_REQUEST);
		FLAG_OFF(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_BUILD_REQUEST);
	}
	*/
}
/*
void NLPATH_WORKBENCH::setTrajectoryPack(NLTRAJECTORY_PACK* ppack, Nbool rebuild)
{
	m_pTrajectoryStatePack = ppack; 
	if (rebuild)
		FLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_STATE_PACK_BUILD_REQUEST); 
}
void NLPATH_WORKBENCH::setTrajectoryBasicPack(NLTRAJECTORY_BASIC_PACK* pspack, Nbool rebuild) 
{
	m_pTrajectoryStateSPack = pspack; 
	if (rebuild)
		FLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_STATE_S_PACK_BUILD_REQUEST); 
}
*/

void NLPATH_WORKBENCH::setDriveTrainSpecifications(NLDRIVETRAINSPECS* pdts, Nbool rebuild)
{
	m_pDriveTrainSpecifications = pdts;
	if (rebuild)
		FLAG_ON(m_flags, FLAG_NLPATH_WORKBENCH_TRAJECTORY_BUILD_REQUEST); 
}

void NLPATH_WORKBENCH::setRamseteParams(NLRAMSETE* prms)
{
	m_pRamsete = prms;
}


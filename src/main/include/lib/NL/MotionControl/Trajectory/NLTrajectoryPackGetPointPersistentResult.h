#pragma once
#include "NLTrajectoryPoint.h"
#include "NLTrajectoryPointDesc.h"
#include "../NLKin.h"

// forward declarations:
class NLTRAJECTORY_PACK;


typedef struct NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT		NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT;
struct NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT
{
	NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT() : m_pTrjPointDsc(NULL){ Nmem0(&m_result, NLTRAJECTORY_POINT); };
	~NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT() {};

	void	initialize(const NLTRAJECTORY_PACK* ppack);
	NLTRAJECTORY_POINT			m_result;		// Doit rester le premier membre de la structure ! car ...
												//	NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT peut-être casté en ...
												//	NLTRAJECTORY_POINT et/ou en 
												//	NLPATH_POINT !!!		
	NLTRJPOINT_DESC*	m_pTrjPointDsc;	// Persistent pointer
};

/*
typedef struct NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT		NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT;
struct NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT
{
	NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT() :	m_pChunk(NULL),
														m_pChunkR_Kin(NULL),
														m_pChunkT_TrajectoryPointDesc(NULL) {};
	~NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT() {};
	void	initialize(const NLTRAJECTORY_PACK* ppack);
	NLTRAJECTORY_POINT			m_result;	// Doit rester le premier membre de la structure !
											// car NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT peut-être casté en
											// NLTRAJECTORY_POINT et/ou en NLPATH_POINT !!!		
	// Persistent pointers
	NLTRAJECTORY_CHUNK*			m_pChunk;
	NLKIN*						m_pChunkR_Kin;
	NLTRJPOINT_DESC*	m_pChunkT_TrajectoryPointDesc;
};
*/
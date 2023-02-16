#include "NLTrajectoryPack.h"
#include "NLTrajectoryPackGetPointPersistentResult.h"

void NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT::initialize(const NLTRAJECTORY_PACK* ppack)
{
	m_pTrjPointDsc = (NLTRJPOINT_DESC*)ppack->m_trajectoryPointDescArray.pFirst;
	NErrorIf(!m_pTrjPointDsc, NERROR_NULL_POINTER);
}

#include "lib/NL/MotionControl/Trajectory/NLTrajectoryPack.h"
#include "lib/NL/MotionControl/Trajectory/NLTrajectoryPackGetPointPersistentResult.h"

void NLTRAJECTORY_PACK_GETPOINT_PERSISTENT_RESULT::initialize(const NLTRAJECTORY_PACK* ppack)
{
	m_pTrjPointDsc = (NLTRJPOINT_DESC*)ppack->m_trajectoryPointDescArray.pFirst;
	NErrorIf(!m_pTrjPointDsc, NERROR_NULL_POINTER);
}

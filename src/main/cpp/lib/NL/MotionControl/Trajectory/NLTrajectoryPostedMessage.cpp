#include "lib/NL/MotionControl/Trajectory/NLTrajectoryPostedMessage.h"

void NLTRJ_POSTED_MESSAGE::null()
{
	m_timeStamp = 0.0f;
	m_user		= 0;
	m_id		= NVOID;
}

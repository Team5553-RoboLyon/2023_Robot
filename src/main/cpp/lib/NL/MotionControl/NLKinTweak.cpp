#include "lib/NL/MotionControl/NLKinTweak.h"

NLKINTWEAK::NLKINTWEAK(NLKINTWEAK const &tobecopied)
{
	*this = tobecopied;
}

NLKINTWEAK::NLKINTWEAK(const NLKINTWEAK *pktwk)
{
	if (pktwk)
		*this = *pktwk;
	else
	{
		m_aHoldLocalLimitRatio = 0.0f;
		m_jHoldLocalLimitRatio = 0.0f;
	}
}

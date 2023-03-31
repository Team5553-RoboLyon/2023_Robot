//#include "../../../../N/File/NFile.h"
#include "lib/N/Miscellaneous/NString.h"
#include "lib/NL/MotionControl/Drivetrain/Characterization/NLMotorCharacterization.h"

Nchar* NLMOTOR_CHARACTERIZATION::read(Nchar * pstr)
{
	// Forward Kv, Ka, et intercept
	pstr = NStrGet_Nf32_AfterLabel(pstr, "_kv+= ",			&m_kv[0]);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "_ka+= ",			&m_ka[0]);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "_intercept+= ",	&m_intercept[0]);

	// Backward Kv, Ka, et intercept
	pstr = NStrGet_Nf32_AfterLabel(pstr, "_kv-= ",			&m_kv[1]);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "_ka-= ",			&m_ka[1]);
	pstr = NStrGet_Nf32_AfterLabel(pstr, "_intercept-= ",	&m_intercept[1]);

	return pstr;
}

void NLMOTOR_CHARACTERIZATION::setFrom(const NLMOTOR_CHARACTERIZATION * psrc, const Nf32 scalefactor)
{
	m_forwardKv				= psrc->m_forwardKv*scalefactor;
	m_backwardKv			= psrc->m_backwardKv*scalefactor;
	m_forwardKa				= psrc->m_forwardKa*scalefactor;
	m_backwardKa			= psrc->m_backwardKa*scalefactor;
	m_forwardIntercept		= psrc->m_forwardIntercept;
	m_backwardIntercept		= psrc->m_backwardIntercept;
}
